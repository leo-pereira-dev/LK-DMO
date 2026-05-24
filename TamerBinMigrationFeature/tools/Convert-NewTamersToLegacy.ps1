param(
    [string]$CurrentBinDir = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english",
    [string]$SourceDataXmlDir = "C:\0-NewDmo\Dcommerce\xml\AllBins\Data",
    [string]$SourceLanguageXmlDir = "C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str",
    [string]$OutputDir = "C:\0-NewDmo\TamerBinMigrationFeature\generated\converted",
    [string]$ReportDir = "C:\0-NewDmo\TamerBinMigrationFeature\generated\reports",
    [int]$MaxLegacyTamerLevel = 120
)

$ErrorActionPreference = "Stop"

New-Item -ItemType Directory -Force -Path $OutputDir, $ReportDir | Out-Null

$TamerRecordSize = 1500
$SkillRecordSize = 736
$TamerSkillRecordSize = 36
$DmBaseRecordSize = 40

$TamerSkillSlots = @{
    80011 = @(72, 73, 74)       # Takato Matsuki
    80012 = @(75, 76, 77)       # Rika Nonaka
    80013 = @(78, 79, 80)       # Henry Wong
    80016 = @(84, 85, 86)       # Kato Jeri
    80017 = @(87, 88, 89)       # Akiyama Ryo
    80018 = @(100, 101, 102)    # Amanokawa Hiro
    80019 = @(103, 104, 105)    # Tsukiyono Ruli
    80020 = @(106, 107, 108)    # Higashimitarai Kiyoshiro
    80021 = @(109, 110, 111)    # Digimon Kaiser: source table only links 109; keep nearby global rows.
}

function Get-Xml([string]$path) {
    if (-not (Test-Path -LiteralPath $path)) {
        throw "XML not found: $path"
    }
    [xml](Get-Content -LiteralPath $path -Encoding UTF8)
}

function Get-LeafValue($node, [string]$name, $default = $null) {
    $leaf = $node.ChildNodes | Where-Object { $_.Name -eq $name } | Select-Object -First 1
    if ($null -eq $leaf) { return $default }
    if ($leaf.Attributes["value"]) { return $leaf.Attributes["value"].Value }
    return $leaf.InnerText
}

function Get-TextMap([string]$path) {
    $xml = Get-Xml $path
    $map = @{}
    $records = $xml.DocumentElement.Entries.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Record" }
    foreach ($record in $records) {
        $key = [int](Get-LeafValue $record "TextKey" 0)
        if ($key -eq 0) { continue }
        $map[$key] = [pscustomobject]@{
            A = (Get-LeafValue $record "TextA" "")
            B = (Get-LeafValue $record "TextB" "")
            C = (Get-LeafValue $record "TextC" "")
            D = (Get-LeafValue $record "TextD" "")
        }
    }
    return $map
}

function Write-UInt16([byte[]]$bytes, [int]$offset, [int]$value) {
    [BitConverter]::GetBytes([uint16]$value).CopyTo($bytes, $offset)
}

function Write-Int32([byte[]]$bytes, [int]$offset, [int]$value) {
    [BitConverter]::GetBytes([int]$value).CopyTo($bytes, $offset)
}

function Write-UInt32([byte[]]$bytes, [int]$offset, [uint32]$value) {
    [BitConverter]::GetBytes([uint32]$value).CopyTo($bytes, $offset)
}

function Write-Int64([byte[]]$bytes, [int]$offset, [int64]$value) {
    [BitConverter]::GetBytes([int64]$value).CopyTo($bytes, $offset)
}

function Write-FixedUnicode([byte[]]$bytes, [int]$offset, [int]$charCount, [string]$value) {
    for ($i = 0; $i -lt ($charCount * 2); $i++) {
        $bytes[$offset + $i] = 0
    }

    if ([string]::IsNullOrEmpty($value)) { return }
    $trimmed = if ($value.Length -ge $charCount) { $value.Substring(0, $charCount - 1) } else { $value }
    $encoded = [Text.Encoding]::Unicode.GetBytes($trimmed)
    [Array]::Copy($encoded, 0, $bytes, $offset, [Math]::Min($encoded.Length, ($charCount - 1) * 2))
}

function Write-FixedAscii([byte[]]$bytes, [int]$offset, [int]$length, [string]$value) {
    for ($i = 0; $i -lt $length; $i++) {
        $bytes[$offset + $i] = 0
    }

    if ([string]::IsNullOrEmpty($value)) { return }
    $safe = ($value -replace "[^A-Za-z0-9 _.-]", "")
    $trimmed = if ($safe.Length -ge $length) { $safe.Substring(0, $length - 1) } else { $safe }
    $encoded = [Text.Encoding]::ASCII.GetBytes($trimmed)
    [Array]::Copy($encoded, 0, $bytes, $offset, [Math]::Min($encoded.Length, $length - 1))
}

function Read-StdString([byte[]]$bytes, [ref]$offset) {
    $length = [BitConverter]::ToInt32($bytes, $offset.Value)
    $offset.Value += 4
    if ($length -le 0) { return "" }
    $text = [Text.Encoding]::UTF8.GetString($bytes, $offset.Value, $length)
    $offset.Value += $length
    return $text
}

function Write-StdString([IO.BinaryWriter]$writer, [string]$value) {
    if ([string]::IsNullOrEmpty($value)) {
        $writer.Write([int]0)
        return
    }

    $encoded = [Text.Encoding]::UTF8.GetBytes($value)
    $writer.Write([int]$encoded.Length)
    $writer.Write($encoded)
}

function Read-CurrentTamerRecords([string]$path) {
    $bytes = [IO.File]::ReadAllBytes($path)
    $count = [BitConverter]::ToInt32($bytes, 0)
    $records = @{}
    for ($i = 0; $i -lt $count; $i++) {
        $offset = 4 + ($i * $TamerRecordSize)
        $id = [BitConverter]::ToUInt32($bytes, $offset)
        $record = New-Object byte[] $TamerRecordSize
        [Array]::Copy($bytes, $offset, $record, 0, $TamerRecordSize)
        $records[[int]$id] = $record
    }

    $tailOffset = 4 + ($count * $TamerRecordSize)
    $tail = New-Object byte[] ($bytes.Length - $tailOffset)
    [Array]::Copy($bytes, $tailOffset, $tail, 0, $tail.Length)

    return [pscustomobject]@{
        Records = $records
        Tail = $tail
        Count = $count
    }
}

function Read-CurrentCharCreateTamers([string]$path) {
    $bytes = [IO.File]::ReadAllBytes($path)
    $offset = [ref]0
    $count = [BitConverter]::ToInt32($bytes, $offset.Value)
    $offset.Value += 4

    $tamers = @{}
    for ($i = 0; $i -lt $count; $i++) {
        $model = [BitConverter]::ToInt32($bytes, $offset.Value); $offset.Value += 4
        $show = $bytes[$offset.Value]; $offset.Value += 1
        $enable = $bytes[$offset.Value]; $offset.Value += 1
        $season = [BitConverter]::ToInt32($bytes, $offset.Value); $offset.Value += 4
        $voice = Read-StdString $bytes $offset
        $icon = [BitConverter]::ToInt32($bytes, $offset.Value); $offset.Value += 4
        $costumeCount = [BitConverter]::ToInt32($bytes, $offset.Value); $offset.Value += 4
        $costumes = @()
        for ($j = 0; $j -lt $costumeCount; $j++) {
            $costumes += [BitConverter]::ToInt32($bytes, $offset.Value)
            $offset.Value += 4
        }

        $tamers[$model] = [pscustomobject]@{
            Model = $model
            Show = $show
            Enable = $enable
            Season = $season
            Voice = $voice
            Icon = $icon
            Costumes = $costumes
        }
    }

    $digimonTail = New-Object byte[] ($bytes.Length - $offset.Value)
    [Array]::Copy($bytes, $offset.Value, $digimonTail, 0, $digimonTail.Length)

    return [pscustomobject]@{
        Tamers = $tamers
        DigimonTail = $digimonTail
    }
}

function Read-CurrentDmBaseExpByLevel([string]$path) {
    $bytes = [IO.File]::ReadAllBytes($path)
    $count = [BitConverter]::ToInt32($bytes, 0)
    $expByLevel = @{}
    $evByLevel = @{}
    $crByLevel = @{}
    for ($i = 0; $i -lt [Math]::Min($count, $MaxLegacyTamerLevel); $i++) {
        $offset = 4 + ($i * $DmBaseRecordSize)
        $level = [BitConverter]::ToUInt16($bytes, $offset + 4)
        $expByLevel[$level] = [BitConverter]::ToInt64($bytes, $offset + 8)
        $evByLevel[$level] = [BitConverter]::ToUInt16($bytes, $offset + 28)
        $crByLevel[$level] = [BitConverter]::ToUInt16($bytes, $offset + 30)
    }

    $tailOffset = 4 + ($count * $DmBaseRecordSize)
    $tail = New-Object byte[] ($bytes.Length - $tailOffset)
    [Array]::Copy($bytes, $tailOffset, $tail, 0, $tail.Length)

    return [pscustomobject]@{
        ExpByLevel = $expByLevel
        EvasionByLevel = $evByLevel
        CriticalByLevel = $crByLevel
        Tail = $tail
        Count = $count
    }
}

function Get-NewTamerRows {
    $tamerXml = Get-Xml (Join-Path $SourceDataXmlDir "Tamer.xml")
    $tamerText = Get-TextMap (Join-Path $SourceLanguageXmlDir "Tamer_Str.xml")
    $rows = New-Object System.Collections.Generic.List[object]
    foreach ($record in ($tamerXml.Tamer.Tamers.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Record" })) {
        $id = [int](Get-LeafValue $record "TamerId" 0)
        if ($id -eq 0) { continue }
        $text = $tamerText[$id]
        $skillRefs = @()
        if ($record.SkillRefs) {
            foreach ($skillRef in ($record.SkillRefs.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Struct" })) {
                $skillRefs += [int](Get-LeafValue $skillRef "TamerSkillIndex" 0)
            }
        }

        $rows.Add([pscustomobject]@{
            Id = $id
            Type = [int](Get-LeafValue $record "TamerType" 0)
            Name = if ($text) { $text.A } else { (Get-LeafValue $record "Name" "") }
            Comment = if ($text) { $text.B } else { "" }
            Part = if ($text) { $text.C } else { "" }
            Gender = if ($text) { $text.D } else { "" }
            SourceSkillRefs = $skillRefs
        })
    }
    return $rows
}

function Get-NewCharCreateRows {
    $xml = Get-Xml (Join-Path $SourceDataXmlDir "CreateList_Tamer.xml")
    $rows = New-Object System.Collections.Generic.List[object]
    foreach ($record in ($xml.CreateList_Tamer.Tamers.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Record" })) {
        $costumes = @()
        if ($record.CostumeIds) {
            foreach ($costume in ($record.CostumeIds.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Leaf" })) {
                $costumes += [int]$costume.Attributes["value"].Value
            }
        }

        $rows.Add([pscustomobject]@{
            Model = [int](Get-LeafValue $record "ModelId" 0)
            Show = [byte](Get-LeafValue $record "Show" 0)
            Enable = [byte](Get-LeafValue $record "Enable" 0)
            Season = [int](Get-LeafValue $record "SeasonType" 0)
            Voice = (Get-LeafValue $record "VoiceFile" "")
            Icon = [int](Get-LeafValue $record "IconId" 0)
            Costumes = $costumes
        })
    }
    return $rows
}

function Get-NewTamerStats {
    $xml = Get-Xml (Join-Path $SourceDataXmlDir "TamerStats.xml")
    $stats = @{}
    foreach ($record in ($xml.TamerStats.Tamers.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Record" })) {
        $tamerId = [int](Get-LeafValue $record "TamerId" 0)
        if ($tamerId -eq 0) { continue }

        $levels = @{}
        foreach ($levelNode in ($record.Levels.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Struct" })) {
            $level = [int](Get-LeafValue $levelNode "Level" 0)
            if ($level -le 0 -or $level -gt $MaxLegacyTamerLevel) { continue }
            $levels[$level] = [pscustomobject]@{
                HP = [int](Get-LeafValue $levelNode "HP" 0)
                DS = [int](Get-LeafValue $levelNode "DS" 0)
                MS = [int](Get-LeafValue $levelNode "MS" 0)
                DE = [int](Get-LeafValue $levelNode "DE" 0)
                AT = [int](Get-LeafValue $levelNode "AT" 0)
                HT = [int](Get-LeafValue $levelNode "HT" 0)
            }
        }
        $stats[$tamerId] = $levels
    }
    return $stats
}

function Get-NewTamerSkillRecords {
    $xml = Get-Xml (Join-Path $SourceDataXmlDir "TamerSkill.xml")
    $records = New-Object System.Collections.Generic.List[object]
    foreach ($record in ($xml.TamerSkill.Skills.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Record" })) {
        foreach ($skill in ($record.Skills.ChildNodes | Where-Object { $_.Attributes["kind"].Value -eq "Struct" })) {
            $records.Add([pscustomobject]@{
                Index = [int](Get-LeafValue $skill "SkillIndex" 0)
                Code = [int](Get-LeafValue $skill "SkillCode" 0)
                Type = [int](Get-LeafValue $skill "SkillType" 0)
                Factor1 = [uint32](Get-LeafValue $skill "Factor1" 0)
                Factor2 = [uint32](Get-LeafValue $skill "Factor2" 0)
                TamerSeqId = [uint32](Get-LeafValue $skill "TamerSeqId" 0)
                DigimonSeqId = [uint32](Get-LeafValue $skill "DigimonSeqId" 0)
                UseState = [int](Get-LeafValue $skill "UseState" 0)
                UseAreaCheck = [int](Get-LeafValue $skill "UseAreaCheck" 0)
                Available = [int](Get-LeafValue $skill "Available" 0)
            })
        }
    }
    return $records | Sort-Object Index
}

function Get-SkillTextMap {
    $skillText = Get-TextMap (Join-Path $SourceLanguageXmlDir "Skill_Str.xml")
    return $skillText
}

function New-TamerListBin {
    $current = Read-CurrentTamerRecords (Join-Path $CurrentBinDir "TamerList.bin")
    $rows = Get-NewTamerRows
    $records = New-Object System.Collections.Generic.List[byte[]]
    $report = New-Object System.Collections.Generic.List[object]

    foreach ($row in $rows) {
        $record = if ($current.Records.ContainsKey($row.Id)) {
            $clone = New-Object byte[] $TamerRecordSize
            [Array]::Copy($current.Records[$row.Id], $clone, $TamerRecordSize)
            $clone
        } else {
            New-Object byte[] $TamerRecordSize
        }

        Write-UInt32 $record 0 ([uint32]$row.Id)
        Write-FixedUnicode $record 4 64 $row.Name
        Write-FixedAscii $record 132 64 $row.Name
        $record[196] = [byte]$row.Type

        $skills = if ($current.Records.ContainsKey($row.Id)) {
            $existing = @()
            for ($i = 0; $i -lt 5; $i++) {
                $existing += [int][BitConverter]::ToUInt32($record, 200 + ($i * 4))
            }
            if (($existing | Where-Object { $_ -ne 0 }).Count -gt 0) { $existing } else { @() }
        } else {
            @()
        }

        if ($skills.Count -eq 0) {
            if ($TamerSkillSlots.ContainsKey($row.Id)) {
                $skills = $TamerSkillSlots[$row.Id]
            } else {
                $skills = $row.SourceSkillRefs
            }
        }

        for ($i = 0; $i -lt 5; $i++) {
            $skillValue = if ($i -lt $skills.Count) { [uint32]$skills[$i] } else { [uint32]0 }
            Write-UInt32 $record (200 + ($i * 4)) $skillValue
        }

        Write-FixedUnicode $record 220 512 $row.Comment
        Write-FixedUnicode $record 1244 64 $row.Part
        Write-FixedUnicode $record 1372 64 $row.Gender

        $records.Add($record)
        $report.Add([pscustomobject]@{
            TamerId = $row.Id
            Name = $row.Name
            TamerType = $row.Type
            Status = if ($current.Records.ContainsKey($row.Id)) { "updated-existing" } else { "added-new" }
            SkillSlots = ($skills -join "|")
            SourceSkillRefs = ($row.SourceSkillRefs -join "|")
        })
    }

    $out = Join-Path $OutputDir "TamerList.bin"
    $fs = [IO.File]::Open($out, [IO.FileMode]::Create, [IO.FileAccess]::Write)
    $writer = New-Object IO.BinaryWriter($fs)
    try {
        $writer.Write([int]$records.Count)
        foreach ($record in $records) { $writer.Write($record) }
        $writer.Write($current.Tail)
    }
    finally {
        $writer.Dispose()
        $fs.Dispose()
    }

    $report | Export-Csv -LiteralPath (Join-Path $ReportDir "tamerlist-conversion.csv") -NoTypeInformation -Encoding UTF8
    return $report
}

function New-CharCreateTableBin {
    $current = Read-CurrentCharCreateTamers (Join-Path $CurrentBinDir "CharCreateTable.bin")
    $rows = Get-NewCharCreateRows
    $out = Join-Path $OutputDir "CharCreateTable.bin"
    $report = New-Object System.Collections.Generic.List[object]

    $fs = [IO.File]::Open($out, [IO.FileMode]::Create, [IO.FileAccess]::Write)
    $writer = New-Object IO.BinaryWriter($fs)
    try {
        $writer.Write([int]$rows.Count)
        foreach ($row in $rows) {
            $use = if ($current.Tamers.ContainsKey($row.Model)) { $current.Tamers[$row.Model] } else { $row }
            $writer.Write([uint32]$row.Model)
            $writer.Write([byte]$use.Show)
            $writer.Write([byte]$use.Enable)
            $writer.Write([int]$use.Season)
            Write-StdString $writer $use.Voice
            $writer.Write([int]$use.Icon)
            $writer.Write([int]$use.Costumes.Count)
            foreach ($costume in $use.Costumes) { $writer.Write([uint32]$costume) }

            $report.Add([pscustomobject]@{
                TamerId = $row.Model
                Status = if ($current.Tamers.ContainsKey($row.Model)) { "preserved-existing" } else { "added-new" }
                Show = $use.Show
                Enable = $use.Enable
                Icon = $use.Icon
                Costumes = ($use.Costumes -join "|")
            })
        }
        $writer.Write($current.DigimonTail)
    }
    finally {
        $writer.Dispose()
        $fs.Dispose()
    }

    $report | Export-Csv -LiteralPath (Join-Path $ReportDir "charcreate-tamer-conversion.csv") -NoTypeInformation -Encoding UTF8
    return $report
}

function New-DMBaseBin {
    $current = Read-CurrentDmBaseExpByLevel (Join-Path $CurrentBinDir "DMBase.bin")
    $tamers = Get-NewTamerRows
    $stats = Get-NewTamerStats
    $records = New-Object System.Collections.Generic.List[byte[]]
    $report = New-Object System.Collections.Generic.List[object]

    foreach ($tamer in $tamers) {
        if (-not $stats.ContainsKey($tamer.Id)) {
            throw "Missing TamerStats row for $($tamer.Id)"
        }

        for ($level = 1; $level -le $MaxLegacyTamerLevel; $level++) {
            if (-not $stats[$tamer.Id].ContainsKey($level)) {
                throw "Missing level $level in TamerStats row for $($tamer.Id)"
            }

            $src = $stats[$tamer.Id][$level]
            $record = New-Object byte[] $DmBaseRecordSize
            Write-UInt32 $record 0 ([uint32](($tamer.Id - 80000) * 1000 + $level))
            Write-UInt16 $record 4 $level
            Write-UInt16 $record 6 0
            Write-Int64 $record 8 ([int64]$current.ExpByLevel[$level])
            Write-Int32 $record 16 $src.HP
            Write-Int32 $record 20 $src.DS
            Write-UInt16 $record 24 $src.MS
            Write-UInt16 $record 26 $src.DE
            Write-UInt16 $record 28 $current.EvasionByLevel[$level]
            Write-UInt16 $record 30 $current.CriticalByLevel[$level]
            Write-UInt16 $record 32 $src.AT
            Write-UInt16 $record 34 $src.HT
            Write-Int32 $record 36 0
            $records.Add($record)
        }

        $level1 = $stats[$tamer.Id][1]
        $report.Add([pscustomobject]@{
            TamerId = $tamer.Id
            TamerType = $tamer.Type
            LevelsWritten = $MaxLegacyTamerLevel
            Level1Hp = $level1.HP
            Level1Ds = $level1.DS
            Level1At = $level1.AT
        })
    }

    $out = Join-Path $OutputDir "DMBase.bin"
    $fs = [IO.File]::Open($out, [IO.FileMode]::Create, [IO.FileAccess]::Write)
    $writer = New-Object IO.BinaryWriter($fs)
    try {
        $writer.Write([int]$records.Count)
        foreach ($record in $records) { $writer.Write($record) }
        $writer.Write($current.Tail)
    }
    finally {
        $writer.Dispose()
        $fs.Dispose()
    }

    $report | Export-Csv -LiteralPath (Join-Path $ReportDir "dmbase-tamerstats-conversion.csv") -NoTypeInformation -Encoding UTF8
    return $report
}

function New-SkillBin {
    $path = Join-Path $CurrentBinDir "Skill.bin"
    $bytes = [IO.File]::ReadAllBytes($path)
    $skillCount = [BitConverter]::ToInt32($bytes, 0)
    $skillSectionLength = 4 + ($skillCount * $SkillRecordSize)
    $oldTamerCount = [BitConverter]::ToInt32($bytes, $skillSectionLength)
    $areaOffset = $skillSectionLength + 4 + ($oldTamerCount * $TamerSkillRecordSize)
    $areaTail = New-Object byte[] ($bytes.Length - $areaOffset)
    [Array]::Copy($bytes, $areaOffset, $areaTail, 0, $areaTail.Length)

    $skillTexts = Get-SkillTextMap
    $tamerSkillRecords = Get-NewTamerSkillRecords
    $tamerSkillCodes = @{}
    foreach ($row in $tamerSkillRecords) {
        if ($row.Code -ne 0) { $tamerSkillCodes[$row.Code] = $true }
    }

    $patchedNameCount = 0
    for ($i = 0; $i -lt $skillCount; $i++) {
        $offset = 4 + ($i * $SkillRecordSize)
        $id = [BitConverter]::ToInt32($bytes, $offset)
        if (-not $tamerSkillCodes.ContainsKey($id)) { continue }
        if (-not $skillTexts.ContainsKey($id)) { continue }

        Write-FixedUnicode $bytes ($offset + 4) 32 $skillTexts[$id].A
        Write-FixedUnicode $bytes ($offset + 68) 256 $skillTexts[$id].B
        $patchedNameCount++
    }

    $out = Join-Path $OutputDir "Skill.bin"
    $fs = [IO.File]::Open($out, [IO.FileMode]::Create, [IO.FileAccess]::Write)
    $writer = New-Object IO.BinaryWriter($fs)
    try {
        $writer.Write($bytes, 0, $skillSectionLength)
        $writer.Write([int]$tamerSkillRecords.Count)
        foreach ($row in $tamerSkillRecords) {
            $record = New-Object byte[] $TamerSkillRecordSize
            Write-UInt16 $record 0 $row.Index
            Write-UInt16 $record 2 0
            Write-UInt32 $record 4 ([uint32]$row.Code)
            Write-UInt16 $record 8 $row.Type
            Write-UInt16 $record 10 0
            Write-UInt32 $record 12 $row.Factor1
            Write-UInt32 $record 16 $row.Factor2
            Write-UInt32 $record 20 $row.TamerSeqId
            Write-UInt32 $record 24 $row.DigimonSeqId
            Write-UInt16 $record 28 $row.UseState
            Write-UInt16 $record 30 $row.UseAreaCheck
            Write-UInt16 $record 32 $row.Available
            Write-UInt16 $record 34 0
            $writer.Write($record)
        }
        $writer.Write($areaTail)
    }
    finally {
        $writer.Dispose()
        $fs.Dispose()
    }

    $tamerSkillRecords | Export-Csv -LiteralPath (Join-Path $ReportDir "skill-tamerskill-section.csv") -NoTypeInformation -Encoding UTF8
    [pscustomobject]@{
        SkillCount = $skillCount
        OldTamerSkillCount = $oldTamerCount
        NewTamerSkillCount = $tamerSkillRecords.Count
        PatchedSkillNames = $patchedNameCount
    } | Export-Csv -LiteralPath (Join-Path $ReportDir "skillbin-summary.csv") -NoTypeInformation -Encoding UTF8
}

$tamerReport = New-TamerListBin
$charReport = New-CharCreateTableBin
$dmBaseReport = New-DMBaseBin
New-SkillBin

[pscustomobject]@{
    OutputDir = $OutputDir
    TamerListRows = $tamerReport.Count
    CharCreateTamerRows = $charReport.Count
    DmBaseTamerRows = $dmBaseReport.Count
    Reports = $ReportDir
} | Format-List
