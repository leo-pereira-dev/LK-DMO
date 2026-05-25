param(
    [string]$DockerContainer = "dmo-postgres",
    [string]$Database = "dso",
    [string]$User = "dso",
    [string]$Schema = "dso",
    [string]$BinDir = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english",
    [string]$OutputPath = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Container.bin"
)

$ErrorActionPreference = "Stop"

$ContainerMagic = [uint32]0x4E54434C # LCTN
$ContainerVersion = [uint32]1
$ChanceScale = 100.0
$RareRankThreshold = 4

function Invoke-PsqlCsv([string]$query) {
    $copy = "COPY ($query) TO STDOUT WITH CSV HEADER"
    $rows = $copy | docker exec -i $DockerContainer psql -U $User -d $Database -q -f -
    if ($LASTEXITCODE -ne 0) {
        throw "psql failed for query: $query"
    }

    $text = ($rows -join [Environment]::NewLine).Trim()
    if ([string]::IsNullOrWhiteSpace($text)) {
        return @()
    }

    return @($text | ConvertFrom-Csv)
}

function Convert-ToBool([object]$value) {
    $text = ([string]$value).Trim().ToLowerInvariant()
    return $text -eq "true" -or $text -eq "t" -or $text -eq "1"
}

function Read-CountedUtf16([IO.BinaryReader]$reader) {
    $chars = [int]$reader.ReadUInt32()
    if ($chars -le 0) {
        return ""
    }

    return [Text.Encoding]::Unicode.GetString($reader.ReadBytes($chars * 2))
}

function Read-ItemNames([string]$path) {
    $names = @{}
    if (-not (Test-Path $path)) {
        return $names
    }

    $fs = [IO.File]::OpenRead($path)
    $reader = [IO.BinaryReader]::new($fs)
    try {
        $count = [int]$reader.ReadUInt32()
        for ($i = 0; $i -lt $count; $i++) {
            $id = [int]$reader.ReadUInt32()
            $local = Read-CountedUtf16 $reader
            $english = Read-CountedUtf16 $reader
            [void](Read-CountedUtf16 $reader)
            $names[[string]$id] = if ([string]::IsNullOrWhiteSpace($english)) { $local } else { $english }
        }
    }
    finally {
        $reader.Dispose()
        $fs.Dispose()
    }

    return $names
}

function Read-CountedUtf8([IO.BinaryReader]$reader) {
    $bytes = [int]$reader.ReadUInt32()
    if ($bytes -le 0) {
        return ""
    }

    return [Text.Encoding]::UTF8.GetString($reader.ReadBytes($bytes))
}

function Read-RandomBoxList([string]$path) {
    $rows = @()
    $fs = [IO.File]::OpenRead($path)
    $reader = [IO.BinaryReader]::new($fs)
    try {
        $count = [int]$reader.ReadUInt32()
        for ($i = 0; $i -lt $count; $i++) {
            $boxId = [int]$reader.ReadUInt32()
            [void]$reader.ReadByte()

            $consumeCount = [int]$reader.ReadUInt32()
            for ($c = 0; $c -lt $consumeCount; $c++) {
                [void]$reader.ReadUInt32()
                [void]$reader.ReadUInt16()
            }

            $itemGroupId = [int]$reader.ReadUInt32()
            $rewardGroupId = [int]$reader.ReadUInt32()
            [void]$reader.ReadUInt32()
            [void]$reader.ReadUInt16()
            [void](Read-CountedUtf8 $reader)

            $rows += [pscustomobject]@{
                BoxId = $boxId
                ItemGroupId = $itemGroupId
                RewardGroupId = $rewardGroupId
            }
        }
    }
    finally {
        $reader.Dispose()
        $fs.Dispose()
    }

    return $rows
}

function Read-RandomBoxGroups([string]$path) {
    $groups = @{}
    $fs = [IO.File]::OpenRead($path)
    $reader = [IO.BinaryReader]::new($fs)
    try {
        $count = [int]$reader.ReadUInt32()
        for ($i = 0; $i -lt $count; $i++) {
            $groupId = [int]$reader.ReadUInt32()
            $itemCount = [int]$reader.ReadUInt32()
            $entries = @()
            for ($j = 0; $j -lt $itemCount; $j++) {
                $entries += [pscustomobject]@{
                    ItemId = [int]$reader.ReadUInt32()
                    ChanceRaw = [uint16]$reader.ReadUInt16()
                    Rank = [byte]$reader.ReadByte()
                    Unknown = [uint32]$reader.ReadUInt32()
                }
            }
            $groups[[string]$groupId] = $entries
        }
    }
    finally {
        $reader.Dispose()
        $fs.Dispose()
    }

    return $groups
}

function Add-Container($containersByItemId, [object]$container) {
    if ($container.ItemId -le 0 -or $null -eq $container.Rewards -or $container.Rewards.Count -eq 0) {
        return
    }

    $containersByItemId[[string]$container.ItemId] = $container
}

function Write-PackedString([IO.BinaryWriter]$writer, [string]$value) {
    if ($null -eq $value) {
        $value = ""
    }

    $bytes = [Text.Encoding]::UTF8.GetBytes($value)
    if ($bytes.Length -gt [uint16]::MaxValue) {
        throw "String too long for Container.bin: $value"
    }

    $writer.Write([uint16]$bytes.Length)
    $writer.Write($bytes)
}

$itemNames = Read-ItemNames (Join-Path $BinDir "Item_Str.bin")
$containersByItemId = [System.Collections.Generic.Dictionary[string,object]]::new()

$dbContainers = Invoke-PsqlCsv "SELECT ""Id"", ""ItemId"", ""ItemName"", ""RewardAmount"" FROM ""$Schema"".""Asset_Container"" ORDER BY ""ItemId"""
$dbRewards = Invoke-PsqlCsv "SELECT ""Id"", ""ItemId"", ""ItemName"", ""MinAmount"", ""MaxAmount"", ""Chance"", ""Rare"", ""ContainerAssetId"" FROM ""$Schema"".""Asset_ContainerReward"" ORDER BY ""ContainerAssetId"", ""Id"""
$dbRewardsByContainer = @{}
foreach ($reward in $dbRewards) {
    $containerId = [string]$reward.ContainerAssetId
    if (-not $dbRewardsByContainer.ContainsKey($containerId)) {
        $dbRewardsByContainer[$containerId] = @()
    }

    $dbRewardsByContainer[$containerId] += [pscustomobject]@{
        Id = [int64]$reward.Id
        ItemId = [int]$reward.ItemId
        ItemName = if ([string]::IsNullOrWhiteSpace($reward.ItemName)) { [string]([int]$reward.ItemId) } else { [string]$reward.ItemName }
        MinAmount = [int]$reward.MinAmount
        MaxAmount = [int]$reward.MaxAmount
        Chance = [double]$reward.Chance
        Rare = Convert-ToBool $reward.Rare
    }
}

foreach ($container in $dbContainers) {
    $id = [int64]$container.Id
    $key = [string]$container.Id
    $rewards = if ($dbRewardsByContainer.ContainsKey($key)) { @($dbRewardsByContainer[$key]) } else { @() }
    $containerObject = [pscustomobject]@{
        Id = $id
        ItemId = [int]$container.ItemId
        ItemName = if ([string]::IsNullOrWhiteSpace($container.ItemName)) { [string]([int]$container.ItemId) } else { [string]$container.ItemName }
        RewardAmount = [int]$container.RewardAmount
        Rewards = $rewards
    }
    if ($containerObject.ItemId -gt 0 -and $containerObject.Rewards.Count -gt 0) {
        $containersByItemId[[string]$containerObject.ItemId] = $containerObject
    }
}

$randomBoxes = Read-RandomBoxList (Join-Path $BinDir "RandomBoxList.bin")
$itemGroups = Read-RandomBoxGroups (Join-Path $BinDir "RandomBoxItem.bin")
$rewardGroups = Read-RandomBoxGroups (Join-Path $BinDir "RandomBoxReward.bin")
$nextContainerId = 1000000
$nextRewardId = 100000000
$missingItemGroups = 0
$missingRewardGroups = 0

foreach ($box in $randomBoxes) {
    $entries = $null
    $itemGroupKey = [string]$box.ItemGroupId
    $rewardGroupKey = [string]$box.RewardGroupId
    if ($itemGroups.ContainsKey($itemGroupKey)) {
        $entries = @($itemGroups[$itemGroupKey])
    }
    elseif ($rewardGroups.ContainsKey($rewardGroupKey)) {
        $missingItemGroups++
        $entries = @($rewardGroups[$rewardGroupKey])
    }
    else {
        $missingItemGroups++
        $missingRewardGroups++
        continue
    }

    $rewards = New-Object System.Collections.Generic.List[object]
    foreach ($entry in $entries) {
        if ($entry.ItemId -le 0) {
            continue
        }

        if ($entry.ChanceRaw -eq 0) {
            $chance = 0.01
        }
        else {
            $chance = [double]($entry.ChanceRaw / $ChanceScale)
            if ($chance -lt 0.01) { $chance = 0.01 }
            if ($chance -gt 100.0) { $chance = 100.0 }
        }
        $rewards.Add([pscustomobject]@{
            Id = [int64]($nextRewardId++)
            ItemId = [int]$entry.ItemId
            ItemName = [string]([int]$entry.ItemId)
            MinAmount = 1
            MaxAmount = 1
            Chance = [double]$chance
            Rare = ([int]$entry.Rank -ge $RareRankThreshold)
        })
    }

    $rewardArray = @($rewards.ToArray())
    $containerObject = [pscustomobject]@{
        Id = [int64]($nextContainerId++)
        ItemId = [int]$box.BoxId
        ItemName = [string]([int]$box.BoxId)
        RewardAmount = 1
        Rewards = $rewardArray
    }
    if ($containerObject.ItemId -gt 0 -and $containerObject.Rewards.Count -gt 0) {
        $containersByItemId[[string]$containerObject.ItemId] = $containerObject
    }
}

$containers = @($containersByItemId.Values | Sort-Object ItemId)
$outDir = Split-Path $OutputPath -Parent
if (-not (Test-Path $outDir)) {
    New-Item -ItemType Directory -Path $outDir | Out-Null
}

$fs = [IO.File]::Open($OutputPath, [IO.FileMode]::Create, [IO.FileAccess]::Write)
$writer = [IO.BinaryWriter]::new($fs)
try {
    $writer.Write($ContainerMagic)
    $writer.Write($ContainerVersion)
    $writer.Write([uint32]$containers.Count)
    foreach ($container in $containers) {
        $writer.Write([int64]$container.Id)
        $writer.Write([int32]$container.ItemId)
        $writer.Write([int32]$container.RewardAmount)
        Write-PackedString $writer $container.ItemName
        $writer.Write([uint32]$container.Rewards.Count)
        foreach ($reward in $container.Rewards) {
            $writer.Write([int64]$reward.Id)
            $writer.Write([int32]$reward.ItemId)
            $writer.Write([int32]$reward.MinAmount)
            $writer.Write([int32]$reward.MaxAmount)
            $writer.Write([double]$reward.Chance)
            $writer.Write([byte]$(if ($reward.Rare) { 1 } else { 0 }))
            Write-PackedString $writer $reward.ItemName
        }
    }
}
finally {
    $writer.Dispose()
    $fs.Dispose()
}

$report = [pscustomobject]@{
    OutputPath = $OutputPath
    Containers = $containers.Count
    Rewards = ($containers | ForEach-Object { $_.Rewards.Count } | Measure-Object -Sum).Sum
    DatabaseContainers = $dbContainers.Count
    RandomBoxRows = $randomBoxes.Count
    RandomBoxItemGroups = $itemGroups.Count
    RandomBoxRewardGroups = $rewardGroups.Count
    MissingItemGroups = $missingItemGroups
    MissingRewardGroups = $missingRewardGroups
}

$report | Format-List
