param(
    [string]$CurrentDigimonBookBin = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Digimon_Book.bin",
    [string]$SourceDataXmlDir = "C:\0-NewDmo\Dcommerce\xml\AllBins\Data",
    [string]$SourceLanguageXmlDir = "C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str",
    [string]$OutputDigimonBookBin = "C:\0-NewDmo\DeckBinMigrationFeature\generated\converted\Digimon_Book.bin",
    [string]$ReportDir = "C:\0-NewDmo\DeckBinMigrationFeature\generated\reports",
    [int]$MaxLegacyOptions = 3
)

$ErrorActionPreference = "Stop"

Set-StrictMode -Version 2.0

if ($MaxLegacyOptions -ne 3) {
    throw "The current legacy client template expects exactly 3 deck option slots. Requested: $MaxLegacyOptions"
}

$script:StringTruncations = New-Object System.Collections.Generic.List[object]

function Ensure-Directory([string]$path) {
    if ([string]::IsNullOrWhiteSpace($path)) {
        return
    }

    if (-not (Test-Path $path)) {
        New-Item -ItemType Directory -Path $path -Force | Out-Null
    }
}

function Read-XmlDocument([string]$path) {
    if (-not (Test-Path $path)) {
        throw "Required XML not found: $path"
    }

    [xml]$xml = Get-Content -Path $path -Raw -Encoding UTF8
    return $xml
}

function Get-SectionRecords([xml]$xml, [string]$sectionName) {
    $section = $xml.DocumentElement.$sectionName
    if ($null -eq $section) {
        throw "Section '$sectionName' not found in $($xml.DocumentElement.Name)"
    }

    $records = @()
    foreach ($node in $section.ChildNodes) {
        if ($node.NodeType -eq [System.Xml.XmlNodeType]::Element -and $node.kind -eq "Record") {
            $records += $node
        }
    }
    return $records
}

function Get-ArrayStructs($arrayNode) {
    $items = @()
    if ($null -eq $arrayNode) {
        return $items
    }

    foreach ($node in $arrayNode.ChildNodes) {
        if ($node.NodeType -eq [System.Xml.XmlNodeType]::Element -and $node.kind -eq "Struct") {
            $items += $node
        }
    }
    return $items
}

function Get-LeafInt($node, [string]$leafName) {
    $leaf = $node.$leafName
    if ($null -eq $leaf) {
        throw "Leaf '$leafName' not found in node '$($node.Name)'"
    }

    return [int64]$leaf.value
}

function Load-TextMap([string]$path) {
    $xml = Read-XmlDocument $path
    $map = @{}
    foreach ($record in (Get-SectionRecords $xml "Entries")) {
        $key = [uint32](Get-LeafInt $record "TextKey")
        $textA = ""
        $textB = ""
        $textANode = $record.SelectSingleNode("TextA")
        $textBNode = $record.SelectSingleNode("TextB")
        if ($null -ne $textANode) {
            $textA = [string]$textANode.InnerText
        }
        if ($null -ne $textBNode) {
            $textB = [string]$textBNode.InnerText
        }

        $map[$key] = [pscustomobject]@{
            TextA = $textA
            TextB = $textB
        }
    }
    return $map
}

function Read-FixedUtf16String([IO.BinaryReader]$reader, [int]$byteLength) {
    $bytes = $reader.ReadBytes($byteLength)
    $end = $byteLength
    for ($i = 0; $i -le $byteLength - 2; $i += 2) {
        if ($bytes[$i] -eq 0 -and $bytes[$i + 1] -eq 0) {
            $end = $i
            break
        }
    }

    if ($end -le 0) {
        return ""
    }

    return [Text.Encoding]::Unicode.GetString($bytes, 0, $end)
}

function Write-FixedUtf16String([IO.BinaryWriter]$writer, [string]$text, [int]$byteLength, [string]$label) {
    $buffer = New-Object byte[] $byteLength
    if ($null -eq $text) {
        $text = ""
    }

    $bytes = [Text.Encoding]::Unicode.GetBytes($text)
    $maxCopy = [Math]::Max(0, $byteLength - 2)
    $copyLength = [Math]::Min($bytes.Length, $maxCopy)
    if (($copyLength % 2) -ne 0) {
        $copyLength--
    }

    if ($bytes.Length -gt $maxCopy) {
        $script:StringTruncations.Add([pscustomobject]@{
            Field = $label
            OriginalByteLength = $bytes.Length
            MaxByteLength = $maxCopy
            OriginalText = $text
            WrittenText = [Text.Encoding]::Unicode.GetString($bytes, 0, $copyLength)
        })
    }

    if ($copyLength -gt 0) {
        [Array]::Copy($bytes, 0, $buffer, 0, $copyLength)
    }

    $writer.Write($buffer)
}

function Read-LegacyDigimonBook([string]$path) {
    if (-not (Test-Path $path)) {
        return $null
    }

    $fs = [IO.File]::Open($path, [IO.FileMode]::Open, [IO.FileAccess]::Read, [IO.FileShare]::ReadWrite)
    $reader = New-Object IO.BinaryReader($fs)
    $digimonNames = @{}
    $deckNames = @{}
    $optionNames = @{}

    try {
        $optionCount = $reader.ReadUInt32()
        for ($i = 0; $i -lt $optionCount; $i++) {
            $id = $reader.ReadUInt32()
            $name = Read-FixedUtf16String $reader 128
            [void]$reader.ReadUInt16()
            [void](Read-FixedUtf16String $reader 1024)
            [void]$reader.ReadUInt16()
            $optionNames[[uint32]$id] = $name
        }

        $exceptionCount = $reader.ReadUInt32()
        for ($i = 0; $i -lt $exceptionCount; $i++) {
            $id = $reader.ReadUInt32()
            $name = Read-FixedUtf16String $reader 128
            if (-not [string]::IsNullOrWhiteSpace($name)) {
                $digimonNames[[uint32]$id] = $name
            }
        }

        $deckCount = $reader.ReadUInt32()
        for ($i = 0; $i -lt $deckCount; $i++) {
            $groupId = $reader.ReadUInt16()
            $name = Read-FixedUtf16String $reader 128
            [void](Read-FixedUtf16String $reader 1024)
            for ($j = 0; $j -lt 3; $j++) { [void]$reader.ReadUInt16() }
            for ($j = 0; $j -lt 3; $j++) { [void]$reader.ReadUInt16() }
            for ($j = 0; $j -lt 3; $j++) { [void]$reader.ReadUInt16() }
            for ($j = 0; $j -lt 3; $j++) { [void]$reader.ReadUInt16() }
            for ($j = 0; $j -lt 3; $j++) { [void]$reader.ReadUInt32() }
            for ($j = 0; $j -lt 3; $j++) { [void]$reader.ReadUInt32() }
            [void]$reader.ReadUInt16()
            $deckNames[[uint32]$groupId] = $name
        }

        $compositionGroupCount = $reader.ReadUInt32()
        $compositionEntryCount = 0
        for ($i = 0; $i -lt $compositionGroupCount; $i++) {
            [void]$reader.ReadUInt16()
            $listSize = $reader.ReadUInt16()
            for ($j = 0; $j -lt $listSize; $j++) {
                $baseId = $reader.ReadUInt32()
                $baseName = Read-FixedUtf16String $reader 128
                [void]$reader.ReadUInt16()
                [void]$reader.ReadUInt16()
                $destId = $reader.ReadUInt32()
                $destName = Read-FixedUtf16String $reader 128
                if (-not [string]::IsNullOrWhiteSpace($baseName)) {
                    $digimonNames[[uint32]$baseId] = $baseName
                }
                if (-not [string]::IsNullOrWhiteSpace($destName)) {
                    $digimonNames[[uint32]$destId] = $destName
                }
                $compositionEntryCount++
            }
        }

        return [pscustomobject]@{
            Path = $path
            FileLength = $fs.Length
            ReadPosition = $fs.Position
            IsExact = ($fs.Position -eq $fs.Length)
            OptionCount = [int]$optionCount
            ExceptionCount = [int]$exceptionCount
            DeckCount = [int]$deckCount
            CompositionGroupCount = [int]$compositionGroupCount
            CompositionEntryCount = [int]$compositionEntryCount
            DigimonNames = $digimonNames
            DeckNames = $deckNames
            OptionNames = $optionNames
        }
    }
    finally {
        $reader.Dispose()
        $fs.Dispose()
    }
}

function Resolve-Text([hashtable]$map, [uint32]$key, [string]$field, [string]$fallback) {
    if ($map.ContainsKey($key)) {
        $value = $map[$key].$field
        if (-not [string]::IsNullOrWhiteSpace($value)) {
            return $value
        }
    }
    return $fallback
}

function Resolve-DigimonName([uint32]$id, [hashtable]$modernNames, $legacyBook, [System.Collections.Generic.List[object]]$missingNames) {
    if ($modernNames.ContainsKey($id) -and -not [string]::IsNullOrWhiteSpace($modernNames[$id].TextA)) {
        return $modernNames[$id].TextA
    }

    if ($null -ne $legacyBook -and $legacyBook.DigimonNames.ContainsKey($id)) {
        return $legacyBook.DigimonNames[$id]
    }

    $missingNames.Add([pscustomobject]@{
        DigimonId = $id
        FallbackName = "Digimon $id"
    })
    return "Digimon $id"
}

function Export-Report($items, [string]$path) {
    if ($null -eq $items) {
        return
    }

    $rows = New-Object System.Collections.Generic.List[object]
    if ($items -is [System.Collections.IEnumerable] -and -not ($items -is [string])) {
        foreach ($item in $items) {
            $rows.Add($item)
        }
    }
    else {
        $rows.Add($items)
    }

    if ($rows.Count -eq 0) {
        Set-Content -Path $path -Value "" -Encoding UTF8
        return
    }

    $rows | Export-Csv -Path $path -NoTypeInformation -Encoding UTF8
}

Ensure-Directory (Split-Path $OutputDigimonBookBin -Parent)
Ensure-Directory $ReportDir

$legacyBook = Read-LegacyDigimonBook $CurrentDigimonBookBin

$deckText = Load-TextMap (Join-Path $SourceLanguageXmlDir "DBookDeck_Str.xml")
$optionText = Load-TextMap (Join-Path $SourceLanguageXmlDir "DBookOpt_Str.xml")
$digimonText = Load-TextMap (Join-Path $SourceLanguageXmlDir "DigimonName_Str.xml")

$optionXml = Read-XmlDocument (Join-Path $SourceDataXmlDir "DBookOptionData.xml")
$exceptionXml = Read-XmlDocument (Join-Path $SourceDataXmlDir "DBookExceptionData.xml")
$deckXml = Read-XmlDocument (Join-Path $SourceDataXmlDir "DBookDeckData.xml")
$compositionXml = Read-XmlDocument (Join-Path $SourceDataXmlDir "DBookCompositionData.xml")

$options = New-Object System.Collections.Generic.List[object]
foreach ($record in (Get-SectionRecords $optionXml "Options")) {
    $optionId = [uint32](Get-LeafInt $record "OptionId")
    $iconNumber = [uint16](Get-LeafInt $record "IconNumber")
    $options.Add([pscustomobject]@{
        OptionId = $optionId
        Name = Resolve-Text $optionText $optionId "TextA" "Option $optionId"
        Explain = Resolve-Text $optionText $optionId "TextB" ""
        IconNumber = $iconNumber
    })
}

$missingNames = New-Object System.Collections.Generic.List[object]

$exceptions = New-Object System.Collections.Generic.List[object]
foreach ($record in (Get-SectionRecords $exceptionXml "ExceptionIds")) {
    $id = [uint32](Get-LeafInt $record "Id")
    $exceptions.Add([pscustomobject]@{
        DigimonId = $id
        Name = Resolve-DigimonName $id $digimonText $legacyBook $missingNames
    })
}

$decks = New-Object System.Collections.Generic.List[object]
$deckReport = New-Object System.Collections.Generic.List[object]
$truncatedReport = New-Object System.Collections.Generic.List[object]
$unsupportedReport = New-Object System.Collections.Generic.List[object]

foreach ($record in (Get-SectionRecords $deckXml "Decks")) {
    $deckId = [uint32](Get-LeafInt $record "DeckId")
    $sourceEntries = New-Object System.Collections.Generic.List[object]
    foreach ($entryNode in (Get-ArrayStructs $record.Entries)) {
        $optionId = [uint16](Get-LeafInt $entryNode "Option")
        $sourceEntries.Add([pscustomobject]@{
            Condition = [uint16](Get-LeafInt $entryNode "Condition")
            AttackType = [uint16](Get-LeafInt $entryNode "AttackType")
            Option = $optionId
            Value = [uint16](Get-LeafInt $entryNode "Value")
            Probability = [uint32](Get-LeafInt $entryNode "Probability")
            Time = [uint32](Get-LeafInt $entryNode "Time")
        })
    }

    $deckName = Resolve-Text $deckText $deckId "TextA" $(if ($null -ne $legacyBook -and $legacyBook.DeckNames.ContainsKey($deckId)) { $legacyBook.DeckNames[$deckId] } else { "Deck $deckId" })
    $deckExplain = Resolve-Text $deckText $deckId "TextB" ""

    $conditions = @(0, 0, 0)
    $attackTypes = @(0, 0, 0)
    $optionIds = @(0, 0, 0)
    $values = @(0, 0, 0)
    $probabilities = @(0, 0, 0)
    $times = @(0, 0, 0)

    for ($i = 0; $i -lt $sourceEntries.Count; $i++) {
        $entry = $sourceEntries[$i]
        $optionName = Resolve-Text $optionText ([uint32]$entry.Option) "TextA" "Option $($entry.Option)"
        if ($entry.Option -gt 6) {
            $unsupportedReport.Add([pscustomobject]@{
                DeckId = $deckId
                DeckName = $deckName
                EntryIndex = $i
                IsKeptInLegacyBin = ($i -lt $MaxLegacyOptions)
                Condition = $entry.Condition
                AttackType = $entry.AttackType
                Option = $entry.Option
                OptionName = $optionName
                Value = $entry.Value
                Probability = $entry.Probability
                Time = $entry.Time
                Note = "Client/server legacy effect code must support this option id."
            })
        }

        if ($i -lt $MaxLegacyOptions) {
            $conditions[$i] = $entry.Condition
            $attackTypes[$i] = $entry.AttackType
            $optionIds[$i] = $entry.Option
            $values[$i] = $entry.Value
            $probabilities[$i] = $entry.Probability
            $times[$i] = $entry.Time
        }
        else {
            $truncatedReport.Add([pscustomobject]@{
                DeckId = $deckId
                DeckName = $deckName
                EntryIndex = $i
                Condition = $entry.Condition
                AttackType = $entry.AttackType
                Option = $entry.Option
                OptionName = $optionName
                Value = $entry.Value
                Probability = $entry.Probability
                Time = $entry.Time
                Reason = "Legacy Digimon_Book.bin has ENCY_MAX_OPTION=3."
            })
        }
    }

    $decks.Add([pscustomobject]@{
        DeckId = $deckId
        Name = $deckName
        Explain = $deckExplain
        Conditions = $conditions
        AttackTypes = $attackTypes
        Options = $optionIds
        Values = $values
        Probabilities = $probabilities
        Times = $times
        SourceEntryCount = $sourceEntries.Count
    })

    $deckReport.Add([pscustomobject]@{
        DeckId = $deckId
        DeckName = $deckName
        SourceEntryCount = $sourceEntries.Count
        KeptEntryCount = [Math]::Min($sourceEntries.Count, $MaxLegacyOptions)
        DroppedEntryCount = [Math]::Max(0, $sourceEntries.Count - $MaxLegacyOptions)
        ContainsUnsupportedLegacyOption = (@($sourceEntries | Where-Object { $_.Option -gt 6 }).Count -gt 0)
        Options = (($sourceEntries | ForEach-Object { $_.Option }) -join ";")
    })
}

$compositionGroups = New-Object System.Collections.Generic.List[object]
$compositionEntryCount = 0
foreach ($record in (Get-SectionRecords $compositionXml "Compositions")) {
    $groupId = [uint32](Get-LeafInt $record "GroupId")
    $items = New-Object System.Collections.Generic.List[object]
    foreach ($entryNode in (Get-ArrayStructs $record.Entries)) {
        $baseId = [uint32](Get-LeafInt $entryNode "BaseDigimonId")
        $destId = [uint32](Get-LeafInt $entryNode "DestinationDigimonId")
        $items.Add([pscustomobject]@{
            BaseDigimonId = $baseId
            BaseDigimonName = Resolve-DigimonName $baseId $digimonText $legacyBook $missingNames
            EvolutionSlot = [uint16](Get-LeafInt $entryNode "EvolutionSlot")
            DestinationDigimonId = $destId
            DestinationDigimonName = Resolve-DigimonName $destId $digimonText $legacyBook $missingNames
        })
        $compositionEntryCount++
    }

    $compositionGroups.Add([pscustomobject]@{
        GroupId = $groupId
        Items = $items
    })
}

$fsOut = [IO.File]::Open($OutputDigimonBookBin, [IO.FileMode]::Create, [IO.FileAccess]::Write, [IO.FileShare]::None)
$writer = New-Object IO.BinaryWriter($fsOut)
try {
    $writer.Write([uint32]$options.Count)
    foreach ($option in ($options | Sort-Object OptionId)) {
        $writer.Write([uint32]$option.OptionId)
        Write-FixedUtf16String $writer $option.Name 128 "Option[$($option.OptionId)].Name"
        $writer.Write([uint16]$option.IconNumber)
        Write-FixedUtf16String $writer $option.Explain 1024 "Option[$($option.OptionId)].Explain"
        $writer.Write([uint16]0)
    }

    $writer.Write([uint32]$exceptions.Count)
    foreach ($exception in ($exceptions | Sort-Object DigimonId)) {
        $writer.Write([uint32]$exception.DigimonId)
        Write-FixedUtf16String $writer $exception.Name 128 "Exception[$($exception.DigimonId)].Name"
    }

    $writer.Write([uint32]$decks.Count)
    foreach ($deck in ($decks | Sort-Object DeckId)) {
        if ($deck.DeckId -gt [uint16]::MaxValue) {
            throw "DeckId $($deck.DeckId) does not fit legacy u16 GroupIndex."
        }

        $writer.Write([uint16]$deck.DeckId)
        Write-FixedUtf16String $writer $deck.Name 128 "Deck[$($deck.DeckId)].Name"
        Write-FixedUtf16String $writer $deck.Explain 1024 "Deck[$($deck.DeckId)].Explain"

        for ($i = 0; $i -lt 3; $i++) { $writer.Write([uint16]$deck.Conditions[$i]) }
        for ($i = 0; $i -lt 3; $i++) { $writer.Write([uint16]$deck.AttackTypes[$i]) }
        for ($i = 0; $i -lt 3; $i++) { $writer.Write([uint16]$deck.Options[$i]) }
        for ($i = 0; $i -lt 3; $i++) { $writer.Write([uint16]$deck.Values[$i]) }
        # CsEncy_Deck::sINFO aligns the following UINT arrays to 4 bytes after
        # the USHORT value array. Without this pad the client reads Prob/Time
        # shifted by 16 bits, producing absurd tooltip values.
        $writer.Write([uint16]0)
        for ($i = 0; $i -lt 3; $i++) { $writer.Write([uint32]$deck.Probabilities[$i]) }
        for ($i = 0; $i -lt 3; $i++) { $writer.Write([uint32]$deck.Times[$i]) }
    }

    $writer.Write([uint32]$compositionGroups.Count)
    foreach ($group in ($compositionGroups | Sort-Object GroupId)) {
        if ($group.GroupId -gt [uint16]::MaxValue) {
            throw "Composition GroupId $($group.GroupId) does not fit legacy u16 GroupIndex."
        }

        $writer.Write([uint16]$group.GroupId)
        $writer.Write([uint16]$group.Items.Count)
        foreach ($item in $group.Items) {
            $writer.Write([uint32]$item.BaseDigimonId)
            Write-FixedUtf16String $writer $item.BaseDigimonName 128 "Composition[$($group.GroupId)].Base[$($item.BaseDigimonId)].Name"
            $writer.Write([uint16]$item.EvolutionSlot)
            $writer.Write([uint16]0)
            $writer.Write([uint32]$item.DestinationDigimonId)
            Write-FixedUtf16String $writer $item.DestinationDigimonName 128 "Composition[$($group.GroupId)].Dest[$($item.DestinationDigimonId)].Name"
        }
    }
}
finally {
    $writer.Dispose()
    $fsOut.Dispose()
}

$validation = Read-LegacyDigimonBook $OutputDigimonBookBin
if ($null -eq $validation -or -not $validation.IsExact) {
    throw "Generated Digimon_Book.bin failed legacy layout validation."
}

$summary = @(
    [pscustomobject]@{ Key = "SourceDataXmlDir"; Value = $SourceDataXmlDir },
    [pscustomobject]@{ Key = "SourceLanguageXmlDir"; Value = $SourceLanguageXmlDir },
    [pscustomobject]@{ Key = "CurrentTemplate"; Value = $CurrentDigimonBookBin },
    [pscustomobject]@{ Key = "Output"; Value = $OutputDigimonBookBin },
    [pscustomobject]@{ Key = "OutputBytes"; Value = (Get-Item $OutputDigimonBookBin).Length },
    [pscustomobject]@{ Key = "LegacyOptionSlots"; Value = $MaxLegacyOptions },
    [pscustomobject]@{ Key = "OptionCount"; Value = $options.Count },
    [pscustomobject]@{ Key = "ExceptionCount"; Value = $exceptions.Count },
    [pscustomobject]@{ Key = "DeckCount"; Value = $decks.Count },
    [pscustomobject]@{ Key = "CompositionGroupCount"; Value = $compositionGroups.Count },
    [pscustomobject]@{ Key = "CompositionEntryCount"; Value = $compositionEntryCount },
    [pscustomobject]@{ Key = "TruncatedDeckOptions"; Value = $truncatedReport.Count },
    [pscustomobject]@{ Key = "UnsupportedLegacyDeckOptions"; Value = $unsupportedReport.Count },
    [pscustomobject]@{ Key = "MissingDigimonNames"; Value = $missingNames.Count },
    [pscustomobject]@{ Key = "StringTruncations"; Value = $script:StringTruncations.Count },
    [pscustomobject]@{ Key = "ValidationExactRead"; Value = $validation.IsExact }
)

Export-Report $summary (Join-Path $ReportDir "digimon-book-summary.csv")
Export-Report $deckReport (Join-Path $ReportDir "deck-conversion.csv")
Export-Report $truncatedReport (Join-Path $ReportDir "deck-truncated-options.csv")
Export-Report $unsupportedReport (Join-Path $ReportDir "deck-unsupported-options.csv")
Export-Report $missingNames (Join-Path $ReportDir "missing-digimon-names.csv")
Export-Report $script:StringTruncations (Join-Path $ReportDir "string-truncations.csv")

$summary | Format-Table -AutoSize
Write-Host "Generated: $OutputDigimonBookBin"
Write-Host "Reports:   $ReportDir"
