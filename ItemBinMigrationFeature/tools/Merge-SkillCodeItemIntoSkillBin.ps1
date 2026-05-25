param(
    [string]$BaseSkillBin = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Skill.bin",
    [string]$SkillCodeItemBin = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\SkillCodeItem.bin",
    [string]$SkillTextXml = "C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str\Skill_Str.xml",
    [string]$OutputSkillBin = "C:\0-NewDmo\ItemBinMigrationFeature\generated\converted\Skill.bin",
    [string]$ReportPath = "C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\skillcodeitem-merge.csv",
    [switch]$ReplaceExisting
)

$ErrorActionPreference = "Stop"

$SkillRecordSize = 736
$TamerSkillRecordSize = 36
$AreaCheckRecordSize = 64
$SkillCodeItemRecordSize = 196

function Read-Int32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToInt32($bytes, $offset)
}

function Copy-Bytes([byte[]]$src, [int]$srcOffset, [byte[]]$dst, [int]$dstOffset, [int]$length) {
    [Array]::Copy($src, $srcOffset, $dst, $dstOffset, $length)
}

function Write-Int32LE([byte[]]$dst, [int]$offset, [int]$value) {
    Copy-Bytes ([BitConverter]::GetBytes([int]$value)) 0 $dst $offset 4
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

function Convert-SkillCodeItemRecord([byte[]]$src, [int]$srcOffset, $skillTexts) {
    $record = New-Object byte[] $SkillRecordSize
    $id = Read-Int32LE $src ($srcOffset + 4)

    Write-Int32LE $record 0 $id
    if ($skillTexts.ContainsKey($id)) {
        Write-FixedUnicode $record 4 32 $skillTexts[$id].Name
        Write-FixedUnicode $record 68 256 $skillTexts[$id].Comment
    }
    else {
        Write-FixedUnicode $record 4 32 "Skill $id"
    }

    $applyCount = Read-Int32LE $src ($srcOffset + 8)
    $copyApplyCount = [Math]::Min(3, $applyCount)
    for ($i = 0; $i -lt $copyApplyCount; $i++) {
        $sourceApply = $srcOffset + 12 + ($i * 22)
        $targetApply = 580 + ($i * 24)
        Copy-Bytes $src ($sourceApply + 0) $record ($targetApply + 0) 4
        Copy-Bytes $src ($sourceApply + 4) $record ($targetApply + 4) 4
        Copy-Bytes $src ($sourceApply + 8) $record ($targetApply + 8) 4
        Copy-Bytes $src ($sourceApply + 12) $record ($targetApply + 12) 4
        Copy-Bytes $src ($sourceApply + 16) $record ($targetApply + 16) 2
        Copy-Bytes $src ($sourceApply + 18) $record ($targetApply + 18) 2
        Copy-Bytes $src ($sourceApply + 20) $record ($targetApply + 20) 2
    }

    Copy-Bytes $src ($srcOffset + 122) $record 652 2
    Copy-Bytes $src ($srcOffset + 124) $record 654 2
    Copy-Bytes $src ($srcOffset + 126) $record 656 2
    Copy-Bytes $src ($srcOffset + 128) $record 658 2
    Copy-Bytes $src ($srcOffset + 130) $record 660 2
    Copy-Bytes $src ($srcOffset + 132) $record 662 2
    Copy-Bytes $src ($srcOffset + 134) $record 664 2
    Copy-Bytes $src ($srcOffset + 136) $record 666 2
    Copy-Bytes $src ($srcOffset + 138) $record 668 2
    Copy-Bytes $src ($srcOffset + 140) $record 670 2
    Copy-Bytes $src ($srcOffset + 142) $record 672 4
    Copy-Bytes $src ($srcOffset + 146) $record 676 4
    Copy-Bytes $src ($srcOffset + 150) $record 680 4
    Copy-Bytes $src ($srcOffset + 154) $record 684 4
    Copy-Bytes $src ($srcOffset + 158) $record 688 2
    Copy-Bytes $src ($srcOffset + 160) $record 692 4
    Copy-Bytes $src ($srcOffset + 164) $record 696 4
    Copy-Bytes $src ($srcOffset + 168) $record 704 4
    Copy-Bytes $src ($srcOffset + 172) $record 708 4
    Copy-Bytes $src ($srcOffset + 176) $record 716 4
    Copy-Bytes $src ($srcOffset + 180) $record 720 4
    Copy-Bytes $src ($srcOffset + 184) $record 724 2
    Copy-Bytes $src ($srcOffset + 186) $record 726 2
    Copy-Bytes $src ($srcOffset + 188) $record 728 2
    Copy-Bytes $src ($srcOffset + 190) $record 730 2
    Copy-Bytes $src ($srcOffset + 192) $record 732 2
    Copy-Bytes $src ($srcOffset + 194) $record 734 2

    return $record
}

$baseBytes = [IO.File]::ReadAllBytes($BaseSkillBin)
$skillCodeBytes = [IO.File]::ReadAllBytes($SkillCodeItemBin)
$skillTexts = Get-SkillTextMap $SkillTextXml

$baseSkillCount = Read-Int32LE $baseBytes 0
$baseSkillSectionSize = 4 + ($baseSkillCount * $SkillRecordSize)
if ($baseBytes.Length -lt $baseSkillSectionSize + 4) {
    throw "Invalid Skill.bin: skill section exceeds file length."
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
$report = New-Object System.Collections.Generic.List[object]
$added = 0
$replaced = 0
$skippedExisting = 0
$truncated = 0

for ($i = 0; $i -lt $skillCodeCount; $i++) {
    $offset = 4 + ($i * $SkillCodeItemRecordSize)
    $id = Read-Int32LE $skillCodeBytes ($offset + 4)
    if ($id -le 0) {
        continue
    }

    $applyCount = Read-Int32LE $skillCodeBytes ($offset + 8)
    if ($applyCount -gt 3) {
        $truncated++
    }

    $action = "Add"
    if ($records.ContainsKey($id)) {
        if ($ReplaceExisting) {
            $action = "Replace"
            $replaced++
        }
        else {
            $action = "SkipExisting"
            $skippedExisting++
            $report.Add([pscustomobject]@{ SkillId = $id; Action = $action; ApplyCount = $applyCount; TruncatedTo3 = $applyCount -gt 3 })
            continue
        }
    }
    else {
        $added++
    }

    $records[$id] = Convert-SkillCodeItemRecord $skillCodeBytes $offset $skillTexts
    $report.Add([pscustomobject]@{ SkillId = $id; Action = $action; ApplyCount = $applyCount; TruncatedTo3 = $applyCount -gt 3 })
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
}

New-Item -ItemType Directory -Force -Path (Split-Path $OutputSkillBin -Parent) | Out-Null
New-Item -ItemType Directory -Force -Path (Split-Path $ReportPath -Parent) | Out-Null

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

$report | Export-Csv -NoTypeInformation -Encoding UTF8 -Path $ReportPath

[pscustomobject]@{
    Output = $OutputSkillBin
    Report = $ReportPath
    BaseSkillCount = $baseSkillCount
    SkillCodeItemCount = $skillCodeCount
    FinalSkillCount = $records.Count
    Added = $added
    Replaced = $replaced
    SkippedExisting = $skippedExisting
    TruncatedApplyCount = $truncated
    TamerSkillCount = $tamerSkillCount
    AreaCheckCount = $areaCheckCount
    Size = (Get-Item $OutputSkillBin).Length
}
