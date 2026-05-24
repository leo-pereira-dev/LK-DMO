param(
    [string]$BaseSkillBin = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Skill.bin",
    [string]$SkillCodeDigimonBin = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\SkillCodeDigimon.bin",
    [string]$SkillTextXml = "C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str\Skill_Str.xml",
    [string]$OutputSkillBin = "C:\0-NewDmo\DigimonBinMigrationFeature\generated\converted\Skill.bin",
    [switch]$ReplaceExisting
)

$ErrorActionPreference = "Stop"

$SkillRecordSize = 736
$TamerSkillRecordSize = 36
$AreaCheckRecordSize = 64
$SkillCodeDigimonRecordSize = 196

function Read-Int32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToInt32($bytes, $offset)
}

function Read-UInt32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToUInt32($bytes, $offset)
}

function Copy-Bytes([byte[]]$src, [int]$srcOffset, [byte[]]$dst, [int]$dstOffset, [int]$length) {
    [Array]::Copy($src, $srcOffset, $dst, $dstOffset, $length)
}

function Write-Int32LE([byte[]]$dst, [int]$offset, [int]$value) {
    Copy-Bytes ([BitConverter]::GetBytes([int]$value)) 0 $dst $offset 4
}

function Write-Name([byte[]]$dst, [int]$offset, [string]$name) {
    $bytes = [Text.Encoding]::Unicode.GetBytes($name)
    $length = [Math]::Min($bytes.Length, 62)
    Copy-Bytes $bytes 0 $dst $offset $length
}

function Write-FixedUnicode([byte[]]$dst, [int]$offset, [int]$charCount, [string]$value) {
    for ($i = 0; $i -lt ($charCount * 2); $i++) {
        $dst[$offset + $i] = 0
    }

    if ([string]::IsNullOrEmpty($value)) {
        return
    }

    $trimmed = if ($value.Length -ge $charCount) { $value.Substring(0, $charCount - 1) } else { $value }
    $bytes = [Text.Encoding]::Unicode.GetBytes($trimmed)
    Copy-Bytes $bytes 0 $dst $offset ([Math]::Min($bytes.Length, ($charCount - 1) * 2))
}

function Read-FixedUnicode([byte[]]$src, [int]$offset, [int]$charCount) {
    $bytes = New-Object byte[] ($charCount * 2)
    Copy-Bytes $src $offset $bytes 0 $bytes.Length
    $text = [Text.Encoding]::Unicode.GetString($bytes)
    $zero = $text.IndexOf([char]0)
    if ($zero -ge 0) {
        $text = $text.Substring(0, $zero)
    }
    return $text
}

function Get-LeafValue($node, [string]$name, $default = $null) {
    $leaf = $node.ChildNodes | Where-Object { $_.Name -eq $name } | Select-Object -First 1
    if ($null -eq $leaf) {
        return $default
    }
    if ($leaf.Attributes["value"]) {
        return $leaf.Attributes["value"].Value
    }
    return $leaf.InnerText
}

function Get-SkillTextMap([string]$path) {
    $map = @{}
    if (-not (Test-Path -LiteralPath $path)) {
        return $map
    }

    [xml]$xml = Get-Content -LiteralPath $path -Encoding UTF8
    $records = $xml.DocumentElement.Entries.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Record" }
    foreach ($record in $records) {
        $key = [int](Get-LeafValue $record "TextKey" 0)
        if ($key -eq 0) {
            continue
        }
        $map[$key] = [pscustomobject]@{
            Name = (Get-LeafValue $record "TextA" "")
            Comment = (Get-LeafValue $record "TextB" "")
        }
    }
    return $map
}

function Convert-SkillCodeRecord([byte[]]$src, [int]$srcOffset) {
    $record = New-Object byte[] $SkillRecordSize
    $id = Read-Int32LE $src ($srcOffset + 4)

    Write-Int32LE $record 0 $id
    if ($skillTexts.ContainsKey($id)) {
        Write-FixedUnicode $record 4 32 $skillTexts[$id].Name
        Write-FixedUnicode $record 68 256 $skillTexts[$id].Comment
    }
    else {
        Write-Name $record 4 "Skill $id"
    }

    $applyCount = Read-Int32LE $src ($srcOffset + 8)
    $copyApplyCount = [Math]::Min(3, $applyCount)
    for ($i = 0; $i -lt $copyApplyCount; $i++) {
        $sourceApply = $srcOffset + 12 + ($i * 22)
        $targetApply = 580 + ($i * 24)
        Copy-Bytes $src ($sourceApply + 0) $record ($targetApply + 0) 4   # ApplyA
        Copy-Bytes $src ($sourceApply + 4) $record ($targetApply + 4) 4   # InvokeRate
        Copy-Bytes $src ($sourceApply + 8) $record ($targetApply + 8) 4   # ApplyB
        Copy-Bytes $src ($sourceApply + 12) $record ($targetApply + 12) 4 # ApplyC
        Copy-Bytes $src ($sourceApply + 16) $record ($targetApply + 16) 2 # BuffCode
        Copy-Bytes $src ($sourceApply + 18) $record ($targetApply + 18) 2 # ApplyId
        Copy-Bytes $src ($sourceApply + 20) $record ($targetApply + 20) 2 # IncreaseBPoint
    }

    Copy-Bytes $src ($srcOffset + 122) $record 652 2 # LevelupPoint
    Copy-Bytes $src ($srcOffset + 124) $record 654 2 # MaxLevel
    Copy-Bytes $src ($srcOffset + 126) $record 656 2 # AttributeType
    Copy-Bytes $src ($srcOffset + 128) $record 658 2 # NatureType
    Copy-Bytes $src ($srcOffset + 130) $record 660 2 # FamilyType
    Copy-Bytes $src ($srcOffset + 132) $record 662 2 # UseHP
    Copy-Bytes $src ($srcOffset + 134) $record 664 2 # UseDS
    Copy-Bytes $src ($srcOffset + 136) $record 666 2 # Icon
    Copy-Bytes $src ($srcOffset + 138) $record 668 2 # Target
    Copy-Bytes $src ($srcOffset + 140) $record 670 2 # AttType
    Copy-Bytes $src ($srcOffset + 142) $record 672 4 # AttRange
    Copy-Bytes $src ($srcOffset + 146) $record 676 4 # AttRangeMinDmg
    Copy-Bytes $src ($srcOffset + 150) $record 680 4 # AttRangeNorDmg
    Copy-Bytes $src ($srcOffset + 154) $record 684 4 # AttRangeMaxDmg
    Copy-Bytes $src ($srcOffset + 158) $record 688 2 # AttSphere
    Copy-Bytes $src ($srcOffset + 160) $record 692 4 # CastingTime
    Copy-Bytes $src ($srcOffset + 164) $record 696 4 # DamageTime
    Copy-Bytes $src ($srcOffset + 168) $record 704 4 # DistanceTime
    Copy-Bytes $src ($srcOffset + 172) $record 708 4 # CooldownTime
    Copy-Bytes $src ($srcOffset + 176) $record 716 4 # SkillVelocity
    Copy-Bytes $src ($srcOffset + 180) $record 720 4 # SkillAccel
    Copy-Bytes $src ($srcOffset + 184) $record 724 2 # SkillType
    Copy-Bytes $src ($srcOffset + 186) $record 726 2 # LimitLevel
    Copy-Bytes $src ($srcOffset + 188) $record 728 2 # SkillGroup
    Copy-Bytes $src ($srcOffset + 190) $record 730 2 # SkillRank
    Copy-Bytes $src ($srcOffset + 192) $record 732 2 # MemorySkill
    Copy-Bytes $src ($srcOffset + 194) $record 734 2 # ReqItem

    return $record
}

$baseBytes = [IO.File]::ReadAllBytes($BaseSkillBin)
$skillCodeBytes = [IO.File]::ReadAllBytes($SkillCodeDigimonBin)
$skillTexts = Get-SkillTextMap $SkillTextXml

$baseSkillCount = Read-Int32LE $baseBytes 0
$baseSkillSectionSize = 4 + ($baseSkillCount * $SkillRecordSize)
if ($baseBytes.Length -lt $baseSkillSectionSize + 4) {
    throw "Invalid Skill.bin: section 1 exceeds file length."
}

$tamerSkillCount = Read-Int32LE $baseBytes $baseSkillSectionSize
$tamerSectionSize = 4 + ($tamerSkillCount * $TamerSkillRecordSize)
$areaOffset = $baseSkillSectionSize + $tamerSectionSize
$areaCheckCount = Read-Int32LE $baseBytes $areaOffset
$areaSectionSize = 4 + ($areaCheckCount * $AreaCheckRecordSize)
$tailOffset = $baseSkillSectionSize
$tailLength = $tamerSectionSize + $areaSectionSize
if ($tailOffset + $tailLength -gt $baseBytes.Length) {
    throw "Invalid Skill.bin: tail sections exceed file length."
}

$records = @{}
for ($i = 0; $i -lt $baseSkillCount; $i++) {
    $offset = 4 + ($i * $SkillRecordSize)
    $id = Read-Int32LE $baseBytes $offset
    $record = New-Object byte[] $SkillRecordSize
    Copy-Bytes $baseBytes $offset $record 0 $SkillRecordSize
    if (-not $records.ContainsKey($id)) {
        $records[$id] = $record
    }
}

$skillCodeCount = Read-Int32LE $skillCodeBytes 0
$added = New-Object System.Collections.Generic.List[int]
$replaced = New-Object System.Collections.Generic.List[int]
$patchedNames = New-Object System.Collections.Generic.List[int]
for ($i = 0; $i -lt $skillCodeCount; $i++) {
    $offset = 4 + ($i * $SkillCodeDigimonRecordSize)
    $id = Read-Int32LE $skillCodeBytes ($offset + 4)
    if ($id -le 0) {
        continue
    }

    if ($records.ContainsKey($id)) {
        if ($ReplaceExisting) {
            $records[$id] = Convert-SkillCodeRecord $skillCodeBytes $offset
            $replaced.Add($id)
        }
        continue
    }

    $records[$id] = Convert-SkillCodeRecord $skillCodeBytes $offset
    $added.Add($id)
}

foreach ($id in @($records.Keys)) {
    if (-not $skillTexts.ContainsKey([int]$id)) {
        continue
    }

    $currentName = Read-FixedUnicode $records[$id] 4 32
    if ($currentName -ne "" -and $currentName -notmatch '^Skill\s+\d+$') {
        continue
    }

    Write-FixedUnicode $records[$id] 4 32 $skillTexts[[int]$id].Name
    Write-FixedUnicode $records[$id] 68 256 $skillTexts[[int]$id].Comment
    $patchedNames.Add([int]$id)
}

$outDir = Split-Path $OutputSkillBin -Parent
New-Item -ItemType Directory -Path $outDir -Force | Out-Null
$fs = [IO.File]::Open($OutputSkillBin, [IO.FileMode]::Create, [IO.FileAccess]::Write)
$writer = New-Object IO.BinaryWriter($fs)
try {
    $orderedIds = @($records.Keys | Sort-Object)
    $writer.Write([int]$orderedIds.Count)
    foreach ($id in $orderedIds) {
        $writer.Write([byte[]]$records[$id])
    }
    $writer.Write($baseBytes, $tailOffset, $tailLength)
}
finally {
    $writer.Dispose()
    $fs.Dispose()
}

[pscustomobject]@{
    Output = $OutputSkillBin
    BaseSkillCount = $baseSkillCount
    SkillCodeDigimonCount = $skillCodeCount
    SkillTextCount = $skillTexts.Count
    FinalSkillCount = $records.Count
    Added = $added.Count
    Replaced = $replaced.Count
    PatchedPlaceholderNames = $patchedNames.Count
    TamerSkillCount = $tamerSkillCount
    AreaCheckCount = $areaCheckCount
    Size = (Get-Item $OutputSkillBin).Length
}
