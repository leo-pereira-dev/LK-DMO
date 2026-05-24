param(
    [string]$WorkspaceRoot = "C:\0-NewDmo",
    [string]$GlobalXmlDir = "C:\0-NewDmo\Dcommerce\xml\AllBins\Data",
    [string]$LegacyMasterCard = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\MasterCard.bin",
    [string]$OutputDir = "C:\0-NewDmo\SealMasterFeature\generated\global-seal-sync",
    [switch]$ApplyServerBins,
    [switch]$ApplyPackStaging
)

$ErrorActionPreference = "Stop"

function Get-ElementChildren($node) {
    @($node.ChildNodes | Where-Object { $_.NodeType -eq [System.Xml.XmlNodeType]::Element })
}

function Get-LeafValue($node, [string]$name, [int]$default = 0) {
    $child = $node.SelectSingleNode($name)
    if ($null -eq $child) {
        return $default
    }

    $value = $child.GetAttribute("value")
    if ([string]::IsNullOrWhiteSpace($value)) {
        return $default
    }

    return [int]$value
}

function Get-LeafText($node, [string]$name, [string]$default = "") {
    $child = $node.SelectSingleNode($name)
    if ($null -eq $child -or [string]::IsNullOrEmpty($child.InnerText)) {
        return $default
    }

    return $child.InnerText
}

function Read-Int32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToInt32($bytes, $offset)
}

function Read-UInt32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToUInt32($bytes, $offset)
}

function Read-UInt16LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToUInt16($bytes, $offset)
}

function Read-FixedWideName([byte[]]$bytes, [int]$offset) {
    $raw = New-Object byte[] 128
    [Array]::Copy($bytes, $offset, $raw, 0, 128)
    $text = [Text.Encoding]::Unicode.GetString($raw)
    return $text.Split([char]0)[0]
}

function Read-LegacyMasterCardMetadata([string]$path) {
    $bytes = [IO.File]::ReadAllBytes($path)
    $cardSize = 268
    $leaderSize = 44
    $abilitySize = 40

    $cardCount = Read-Int32LE $bytes 0
    $namesBySeal = @{}
    $leadersById = @{}

    $offset = 4
    for ($i = 0; $i -lt $cardCount; $i++) {
        $base = $offset + ($i * $cardSize)
        $sealItem = Read-UInt32LE $bytes ($base + 148)
        $name = Read-FixedWideName $bytes ($base + 4)
        if (-not [string]::IsNullOrWhiteSpace($name)) {
            $namesBySeal[[int]$sealItem] = $name
        }
    }

    $offset += $cardCount * $cardSize
    $leaderCount = Read-Int32LE $bytes $offset
    $offset += 4
    for ($i = 0; $i -lt $leaderCount; $i++) {
        $base = $offset + ($i * $leaderSize)
        $id = Read-UInt32LE $bytes $base
        $leadersById[[int]$id] = [pscustomobject]@{
            Id = [uint32]$id
            DigimonId = [uint32](Read-UInt32LE $bytes ($base + 4))
            PetId = [uint16](Read-UInt16LE $bytes ($base + 8))
            Ani1 = [uint32](Read-UInt32LE $bytes ($base + 12))
            Ani2 = [uint32](Read-UInt32LE $bytes ($base + 16))
            Special1 = [uint32](Read-UInt32LE $bytes ($base + 20))
            Special2 = [uint32](Read-UInt32LE $bytes ($base + 24))
            Abil1 = [uint32](Read-UInt32LE $bytes ($base + 28))
            Abil2 = [uint32](Read-UInt32LE $bytes ($base + 32))
            Abil3 = [uint32](Read-UInt32LE $bytes ($base + 36))
            Abil4 = [uint32](Read-UInt32LE $bytes ($base + 40))
        }
    }

    $offset += $leaderCount * $leaderSize
    $abilityCount = Read-Int32LE $bytes $offset
    $abilityBytesLength = 4 + ($abilityCount * $abilitySize)
    $abilityBytes = New-Object byte[] $abilityBytesLength
    [Array]::Copy($bytes, $offset, $abilityBytes, 0, $abilityBytesLength)

    return [pscustomobject]@{
        NamesBySeal = $namesBySeal
        LeadersById = $leadersById
        AbilityBytes = $abilityBytes
    }
}

function Get-SealName([hashtable]$legacyNames, [hashtable]$digimonImages, [int]$sealItemId, [int]$digimonId) {
    if ($legacyNames.ContainsKey($sealItemId)) {
        return $legacyNames[$sealItemId]
    }

    if ($digimonImages.ContainsKey($digimonId)) {
        $baseName = [IO.Path]::GetFileNameWithoutExtension($digimonImages[$digimonId])
        $baseName = $baseName -replace '^sealmaster_img_', ''
        $baseName = $baseName -replace '_RaidBoss$', ''
        $baseName = $baseName -replace '_', ' '
        return $baseName.Trim()
    }

    return "Seal $sealItemId"
}

function Write-FixedWideName([IO.BinaryWriter]$writer, [string]$value) {
    $chars = if ($null -eq $value) { "" } else { $value }
    if ($chars.Length -gt 63) {
        $chars = $chars.Substring(0, 63)
    }

    $buffer = New-Object byte[] 128
    $encoded = [Text.Encoding]::Unicode.GetBytes($chars)
    [Array]::Copy($encoded, 0, $buffer, 0, [Math]::Min($encoded.Length, $buffer.Length - 2))
    $writer.Write($buffer)
}

function Write-LegacyString([IO.BinaryWriter]$writer, [string]$value, [bool]$wide = $false) {
    $text = if ($null -eq $value) { "" } else { $value }
    $encoding = if ($wide) { [Text.Encoding]::Unicode } else { [Text.Encoding]::ASCII }
    $bytes = $encoding.GetBytes($text)
    $unitCount = if ($wide) { [int]($bytes.Length / 2) } else { $bytes.Length }
    $writer.Write([int]$unitCount)
    $writer.Write($bytes)
}

function Write-CardRecord([IO.BinaryWriter]$writer, $card, [string]$name) {
    $writer.Write([uint32]$card.LeaderId)
    Write-FixedWideName $writer $name
    $writer.Write([uint32]$card.DigimonId)
    $writer.Write([uint16]$card.IconId)
    $writer.Write([uint16]0)
    $writer.Write([uint32]0)
    $writer.Write([uint16]$card.Scale)
    $writer.Write([uint16]0)

    for ($i = 0; $i -lt 6; $i++) {
        $stage = $card.Stages[$i]
        $writer.Write([uint32]$card.SealItemId)
        $writer.Write([uint16]$stage.IconId)
        $writer.Write([uint16]$stage.MaxCount)
        $writer.Write([uint16]0)
        $writer.Write([uint16]$stage.Effect1)
        $writer.Write([uint16]$stage.Effect1Value)
        $writer.Write([uint16]$stage.Effect2)
        $writer.Write([uint16]$stage.Effect2Value)
        $writer.Write([uint16]0)
    }
}

function Write-LeaderRecord([IO.BinaryWriter]$writer, $card, [hashtable]$legacyLeaders) {
    $legacy = $legacyLeaders[[int]$card.LeaderId]
    $petId = if ($null -ne $legacy) { $legacy.PetId } else { [uint16]1 }
    $ani1 = if ($null -ne $legacy) { $legacy.Ani1 } else { [uint32]901103 }
    $ani2 = if ($null -ne $legacy) { $legacy.Ani2 } else { [uint32]901104 }
    $special1 = if ($null -ne $legacy) { $legacy.Special1 } else { [uint32]0 }
    $special2 = if ($null -ne $legacy) { $legacy.Special2 } else { [uint32]0 }
    $abil1 = if ($null -ne $legacy) { $legacy.Abil1 } else { [uint32]0 }
    $abil2 = if ($null -ne $legacy) { $legacy.Abil2 } else { [uint32]0 }
    $abil3 = if ($null -ne $legacy) { $legacy.Abil3 } else { [uint32]0 }
    $abil4 = if ($null -ne $legacy) { $legacy.Abil4 } else { [uint32]0 }

    $writer.Write([uint32]$card.LeaderId)
    $writer.Write([uint32]$card.DigimonId)
    $writer.Write([uint16]$petId)
    $writer.Write([uint16]0)
    $writer.Write([uint32]$ani1)
    $writer.Write([uint32]$ani2)
    $writer.Write([uint32]$special1)
    $writer.Write([uint32]$special2)
    $writer.Write([uint32]$abil1)
    $writer.Write([uint32]$abil2)
    $writer.Write([uint32]$abil3)
    $writer.Write([uint32]$abil4)
}

function Load-SealCards([string]$path) {
    [xml]$xml = Get-Content $path -Raw
    $cards = New-Object System.Collections.Generic.List[object]

    foreach ($record in Get-ElementChildren $xml.SealCard.Cards) {
        $stages = New-Object System.Collections.Generic.List[object]
        foreach ($stage in Get-ElementChildren $record.Stages) {
            $stages.Add([pscustomobject]@{
                Grade = Get-LeafValue $stage "Grade"
                IconId = Get-LeafValue $stage "IconId"
                MaxCount = Get-LeafValue $stage "MaxCount"
                Effect1 = Get-LeafValue $stage "Effect1"
                Effect1Value = Get-LeafValue $stage "Effect1Value"
                Effect2 = Get-LeafValue $stage "Effect2"
                Effect2Value = Get-LeafValue $stage "Effect2Value"
            })
        }

        $cards.Add([pscustomobject]@{
            SealItemId = Get-LeafValue $record "SealItemId"
            LeaderId = Get-LeafValue $record "LeaderId"
            DigimonId = Get-LeafValue $record "DigimonId"
            IconId = Get-LeafValue $record "IconId"
            Scale = Get-LeafValue $record "Scale"
            Stages = @($stages | Sort-Object Grade)
        })
    }

    return @($cards | Sort-Object SealItemId)
}

function Load-DigimonImages([string]$path) {
    [xml]$xml = Get-Content $path -Raw
    $map = @{}
    foreach ($record in Get-ElementChildren $xml.SealDigimonImage.Images) {
        $map[(Get-LeafValue $record "DigimonId")] = Get-LeafText $record "SealImagePath"
    }
    return $map
}

function Load-PlateImages([string]$path) {
    [xml]$xml = Get-Content $path -Raw
    $names = @("Normal", "Bronze", "Silver", "Gold", "Platinum", "Master")
    $records = @()
    foreach ($record in Get-ElementChildren $xml.SealPlateImage.Images) {
        $grade = Get-LeafValue $record "Grade"
        $records += [pscustomobject]@{
            Grade = $grade
            Name = if ($grade -lt $names.Count) { $names[$grade] } else { "Grade $grade" }
            NifPath = Get-LeafText $record "NifPath"
            GradeBackImagePath = Get-LeafText $record "GradeBackImagePath"
        }
    }
    return @($records | Sort-Object Grade)
}

function Load-TypeImages([string]$path, [string]$rootName, [string]$idName, [string]$pathName) {
    [xml]$xml = Get-Content $path -Raw
    $records = @()
    $root = $xml.SelectSingleNode("/$rootName/Images")
    foreach ($record in Get-ElementChildren $root) {
        $records += [pscustomobject]@{
            Id = Get-LeafValue $record $idName
            Path = Get-LeafText $record $pathName
        }
    }
    return @($records | Sort-Object Id)
}

function Load-Filters([string]$path) {
    [xml]$xml = Get-Content $path -Raw
    $records = @()
    foreach ($filter in Get-ElementChildren $xml.SealFilter.Filters) {
        $mapGroupId = Get-LeafValue $filter "MapGroupId"
        foreach ($group in Get-ElementChildren $filter.Groups) {
            $mapId = Get-LeafValue $group "MapId"
            foreach ($seal in Get-ElementChildren $group.SealItemIds) {
                $records += [pscustomobject]@{
                    MapGroupId = $mapGroupId
                    MapId = $mapId
                    SealItemId = [int]$seal.GetAttribute("value")
                }
            }
        }
    }
    return @($records | Sort-Object MapGroupId, MapId, SealItemId -Unique)
}

function Convert-EffectToSqlValues($stage) {
    $values = [ordered]@{
        ASValue = 0
        ARValue = 0
        ATValue = 0
        BLValue = 0
        CTValue = 0
        DEValue = 0
        DSValue = 0
        EVValue = 0
        HPValue = 0
        HTValue = 0
        MSValue = 0
        WSValue = 0
    }

    foreach ($pair in @(
        @{ Effect = $stage.Effect1; Value = $stage.Effect1Value },
        @{ Effect = $stage.Effect2; Value = $stage.Effect2Value }
    )) {
        switch ([int]$pair.Effect) {
            1 { $values.HPValue = [int]$pair.Value }
            3 { $values.DSValue = [int]$pair.Value }
            5 { $values.ATValue = [int]$pair.Value }
            9 { $values.CTValue = [int]$pair.Value * 100 }
            11 { $values.HTValue = [int]$pair.Value }
            13 { $values.DEValue = [int]$pair.Value }
            15 { $values.BLValue = [int]$pair.Value * 100 }
            17 { $values.EVValue = [int]$pair.Value * 100 }
        }
    }

    return $values
}

function Write-ServerSql([string]$path, [object[]]$cards) {
    $columns = @("Id", "SealId", "RequiredAmount", "SequentialId", "ASValue", "ARValue", "ATValue", "BLValue", "CTValue", "DEValue", "DSValue", "EVValue", "HPValue", "HTValue", "MSValue", "WSValue")
    $lines = New-Object System.Collections.Generic.List[string]
    $lines.Add("BEGIN;")
    $lines.Add('DELETE FROM "Asset_SealDetail";')
    $lines.Add('INSERT INTO "Asset_SealDetail" ("' + ($columns -join '", "') + '") VALUES')

    $id = 1
    $rows = New-Object System.Collections.Generic.List[string]
    foreach ($card in $cards) {
        foreach ($stage in $card.Stages) {
            $status = Convert-EffectToSqlValues $stage
            $row = @(
                $id,
                $card.SealItemId,
                $stage.MaxCount,
                $card.LeaderId,
                $status.ASValue,
                $status.ARValue,
                $status.ATValue,
                $status.BLValue,
                $status.CTValue,
                $status.DEValue,
                $status.DSValue,
                $status.EVValue,
                $status.HPValue,
                $status.HTValue,
                $status.MSValue,
                $status.WSValue
            ) -join ", "
            $rows.Add("($row)")
            $id++
        }
    }

    for ($i = 0; $i -lt $rows.Count; $i++) {
        $suffix = if ($i -eq $rows.Count - 1) { ";" } else { "," }
        $lines.Add($rows[$i] + $suffix)
    }
    $lines.Add('SELECT setval(pg_get_serial_sequence(''"Asset_SealDetail"'', ''Id''), COALESCE((SELECT MAX("Id") FROM "Asset_SealDetail"), 1), true);')
    $lines.Add("COMMIT;")
    [IO.File]::WriteAllLines($path, $lines, [Text.Encoding]::UTF8)
}

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$sealCardPath = Join-Path $GlobalXmlDir "SealCard.xml"
$sealDigimonImagePath = Join-Path $GlobalXmlDir "SealDigimonImage.xml"
$sealPlateImagePath = Join-Path $GlobalXmlDir "SealPlateImage.xml"
$sealNatureImagePath = Join-Path $GlobalXmlDir "SealNatureImage.xml"
$sealBaseImagePath = Join-Path $GlobalXmlDir "SealBaseImage.xml"
$sealFilterPath = Join-Path $GlobalXmlDir "SealFilter.xml"

$legacy = Read-LegacyMasterCardMetadata $LegacyMasterCard
$cards = Load-SealCards $sealCardPath
$digimonImages = Load-DigimonImages $sealDigimonImagePath
$plateImages = Load-PlateImages $sealPlateImagePath
$elementalImages = Load-TypeImages $sealNatureImagePath "SealNatureImage" "NatureType" "ElementalImagePath"
$attributeImages = Load-TypeImages $sealBaseImagePath "SealBaseImage" "AttributeType" "AttributeImagePath"
$filters = Load-Filters $sealFilterPath

$masterCardOut = Join-Path $OutputDir "MasterCard.bin"
$fs = [IO.File]::Open($masterCardOut, [IO.FileMode]::Create, [IO.FileAccess]::Write)
$writer = New-Object IO.BinaryWriter($fs)
try {
    $writer.Write([int]$cards.Count)
    foreach ($card in $cards) {
        $name = Get-SealName $legacy.NamesBySeal $digimonImages $card.SealItemId $card.DigimonId
        Write-CardRecord $writer $card $name
    }

    $writer.Write([int]$cards.Count)
    foreach ($card in $cards) {
        Write-LeaderRecord $writer $card $legacy.LeadersById
    }

    $writer.Write($legacy.AbilityBytes)

    $writer.Write([int]$digimonImages.Count)
    foreach ($entry in $digimonImages.GetEnumerator() | Sort-Object Name) {
        $writer.Write([uint32]$entry.Name)
        Write-LegacyString $writer $entry.Value
    }

    $writer.Write([int]$plateImages.Count)
    foreach ($entry in $plateImages) {
        $writer.Write([int]$entry.Grade)
        Write-LegacyString $writer $entry.Name $true
        Write-LegacyString $writer $entry.NifPath
        Write-LegacyString $writer $entry.GradeBackImagePath
    }

    $writer.Write([int]$elementalImages.Count)
    foreach ($entry in $elementalImages) {
        $writer.Write([int]$entry.Id)
        Write-LegacyString $writer $entry.Path
    }

    $writer.Write([int]$attributeImages.Count)
    foreach ($entry in $attributeImages) {
        $writer.Write([int]$entry.Id)
        Write-LegacyString $writer $entry.Path
    }

    $writer.Write([int]$filters.Count)
    foreach ($entry in $filters) {
        $writer.Write([uint32]$entry.MapGroupId)
        $writer.Write([uint32]$entry.MapId)
        $writer.Write([uint32]$entry.SealItemId)
    }
}
finally {
    $writer.Dispose()
    $fs.Dispose()
}

$sqlOut = Join-Path $OutputDir "Asset_SealDetail_Global_PostgreSQL.sql"
Write-ServerSql $sqlOut $cards

$manifest = @(
    [pscustomobject]@{ File = $masterCardOut; Purpose = "Legacy client MasterCard.bin generated from Global Seal XML"; Count = $cards.Count }
    [pscustomobject]@{ File = $sqlOut; Purpose = "Server Asset_SealDetail replacement data generated from Global Seal XML"; Count = $cards.Count * 6 }
)
$manifest | Export-Csv -Path (Join-Path $OutputDir "manifest.csv") -NoTypeInformation -Encoding UTF8

if ($ApplyServerBins) {
    $serverBin = Join-Path $WorkspaceRoot "dmo-server-main-postgres\Bins\data\bin\english\MasterCard.bin"
    Copy-Item -Path $masterCardOut -Destination $serverBin -Force

    foreach ($name in @("SealCard.bin", "SealLeader.bin", "SealDigimonImage.bin", "SealFilter.bin", "SealNatureImage.bin", "SealBaseImage.bin", "SealPlateImage.bin")) {
        $source = Join-Path $WorkspaceRoot "Dcommerce\Bin\Table\$name"
        if (Test-Path $source) {
            Copy-Item -Path $source -Destination (Join-Path $WorkspaceRoot "dmo-server-main-postgres\Bins\data\bin\english\$name") -Force
        }
    }

    $languageSource = Join-Path $WorkspaceRoot "Dcommerce\Bin\Language\English\SealMasterName_Str.bin"
    if (Test-Path $languageSource) {
        Copy-Item -Path $languageSource -Destination (Join-Path $WorkspaceRoot "dmo-server-main-postgres\Bins\data\bin\english\SealMasterName_Str.bin") -Force
    }
}

if ($ApplyPackStaging) {
    $packBin = Join-Path $WorkspaceRoot "DigiCombineFeature\assets\extracted_pack03\data\bin\english"
    Copy-Item -Path $masterCardOut -Destination (Join-Path $packBin "MasterCard.bin") -Force

    foreach ($name in @("SealCard.bin", "SealLeader.bin", "SealDigimonImage.bin", "SealFilter.bin", "SealNatureImage.bin", "SealBaseImage.bin", "SealPlateImage.bin")) {
        $source = Join-Path $WorkspaceRoot "Dcommerce\Bin\Table\$name"
        if (Test-Path $source) {
            Copy-Item -Path $source -Destination (Join-Path $packBin $name) -Force
        }
    }

    $languageSource = Join-Path $WorkspaceRoot "Dcommerce\Bin\Language\English\SealMasterName_Str.bin"
    if (Test-Path $languageSource) {
        Copy-Item -Path $languageSource -Destination (Join-Path $packBin "SealMasterName_Str.bin") -Force
    }
}

Write-Host "Generated MasterCard.bin: $masterCardOut"
Write-Host "Generated server SQL: $sqlOut"
Write-Host "Seal cards: $($cards.Count)"
Write-Host "Server rows: $($cards.Count * 6)"
