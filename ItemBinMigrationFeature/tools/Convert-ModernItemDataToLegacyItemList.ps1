param(
    [string]$BaseItemListBin = "C:\0-NewDmo\ItemBinMigrationFeature\generated\backups\ItemList.pack03.before_itemmerge_20260524_174039.bin",
    [string]$ItemDataBin = "C:\0-NewDmo\Dcommerce\Bin\Table\ItemData.bin",
    [string]$ItemStringBin = "C:\0-NewDmo\Dcommerce\Bin\Language\English\Item_Str.bin",
    [string]$AccessoryOptionBin = "C:\0-NewDmo\Dcommerce\Bin\Table\AccOption.bin",
    [string]$AccessoryEnchantBin = "C:\0-NewDmo\Dcommerce\Bin\Table\AccEnchant.bin",
    [string]$OutputItemListBin = "C:\0-NewDmo\ItemBinMigrationFeature\generated\converted\ItemList.bin",
    [string]$ReportPath = "C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\itemlist-modern-to-legacy.csv"
)

$ErrorActionPreference = "Stop"

$LegacyItemRecordSize = 1596
$LegacyAccessoryOptionSlotCount = 16
$ModernAccessoryOptionSlotCount = 44
$ItemTapRecordSize = 66
$ItemCoolTimeRecordSize = 16
$MapDisplayRecordSize = 8
$MapTypeNameRecordSize = 132
$RankRecordSize = 8
$ElementItemRecordSize = 4
$ExchangeRecordSize = 44
$AccessoryEnchantRecordSize = 12

function Get-AccessoryOptionRecordSize([int]$slotCount) {
    return 4 + 8 + ($slotCount * 12)
}

function Clamp-UInt16([long]$value) {
    if ($value -lt 0) { return [uint16]0 }
    if ($value -gt [uint16]::MaxValue) { return [uint16]::MaxValue }
    return [uint16]$value
}

function Clamp-Byte([long]$value) {
    if ($value -lt 0) { return [byte]0 }
    if ($value -gt [byte]::MaxValue) { return [byte]::MaxValue }
    return [byte]$value
}

function Align-Writer([IO.BinaryWriter]$writer, [int]$alignment) {
    while (($writer.BaseStream.Position % $alignment) -ne 0) {
        $writer.Write([byte]0)
    }
}

function Copy-Bytes([byte[]]$bytes, [int]$offset, [int]$count) {
    $copy = New-Object byte[] $count
    if ($count -gt 0) {
        [Array]::Copy($bytes, $offset, $copy, 0, $count)
    }
    return $copy
}

function Read-CountedUtf16([IO.BinaryReader]$reader) {
    $chars = [int]$reader.ReadUInt32()
    if ($chars -le 0) { return "" }
    $bytes = $reader.ReadBytes($chars * 2)
    return ([Text.Encoding]::Unicode.GetString($bytes)).TrimEnd([char]0)
}

function Read-CountedAscii([IO.BinaryReader]$reader) {
    $bytes = [int]$reader.ReadUInt32()
    if ($bytes -le 0) { return "" }
    return ([Text.Encoding]::ASCII.GetString($reader.ReadBytes($bytes))).TrimEnd([char]0)
}

function Write-FixedUtf16([IO.BinaryWriter]$writer, [string]$value, [int]$chars) {
    if ($null -eq $value) { $value = "" }
    if ($value.Length -ge $chars) {
        $value = $value.Substring(0, $chars - 1)
    }

    $bytes = New-Object byte[] ($chars * 2)
    $src = [Text.Encoding]::Unicode.GetBytes($value)
    [Array]::Copy($src, 0, $bytes, 0, [Math]::Min($src.Length, $bytes.Length - 2))
    $writer.Write($bytes)
}

function Write-FixedAscii([IO.BinaryWriter]$writer, [string]$value, [int]$bytes) {
    if ($null -eq $value) { $value = "" }
    $value = $value -replace '[^\x00-\x7F]', ''
    $data = New-Object byte[] $bytes
    $src = [Text.Encoding]::ASCII.GetBytes($value)
    [Array]::Copy($src, 0, $data, 0, [Math]::Min($src.Length, $data.Length - 1))
    $writer.Write($data)
}

function Read-ItemStrings([string]$path) {
    $fs = [IO.File]::OpenRead($path)
    $reader = [IO.BinaryReader]::new($fs)
    try {
        $count = [int]$reader.ReadUInt32()
        $map = @{}
        for ($i = 0; $i -lt $count; $i++) {
            $key = [int]$reader.ReadUInt32()
            $localName = Read-CountedUtf16 $reader
            $englishName = Read-CountedUtf16 $reader
            $comment = Read-CountedUtf16 $reader
            $name = if ([string]::IsNullOrWhiteSpace($localName)) { $englishName } else { $localName }
            $map[$key] = [pscustomobject]@{
                Name = $name
                TypeName = $englishName
                Comment = $comment
            }
        }
        return $map
    }
    finally {
        $reader.Dispose()
        $fs.Dispose()
    }
}

function Read-ModernItems([string]$path, [hashtable]$strings) {
    $fs = [IO.File]::OpenRead($path)
    $reader = [IO.BinaryReader]::new($fs)
    try {
        $count = [int]$reader.ReadUInt32()
        $items = New-Object System.Collections.Generic.List[object]
        for ($i = 0; $i -lt $count; $i++) {
            $itemTableKey = $reader.ReadUInt32()
            $itemId = $reader.ReadUInt32()
            $icon = $reader.ReadUInt32()
            $skillCodeType = $reader.ReadByte()
            $skillCode = $reader.ReadUInt32()
            $socketCount = $reader.ReadByte()
            $class = $reader.ReadUInt16()
            $typeL = $reader.ReadUInt16()
            $typeS = $reader.ReadUInt16()
            $typeValue = $reader.ReadUInt32()
            $typeSection = $reader.ReadUInt32()
            $sellType = $reader.ReadUInt16()
            $useMode = $reader.ReadByte()
            $useTimeGroup = $reader.ReadUInt16()
            $overlap = $reader.ReadUInt16()
            $tamerMin = $reader.ReadUInt16()
            $tamerMax = $reader.ReadUInt16()
            $digimonMin = $reader.ReadUInt16()
            $digimonMax = $reader.ReadUInt16()
            $possess = $reader.ReadUInt16()
            $equipSeries = $reader.ReadUInt16()
            $useCharacter = $reader.ReadUInt16()
            $drop = $reader.ReadUInt16()
            $eventItemType = $reader.ReadUInt32()
            $digicorePrice = $reader.ReadUInt16()
            $eventItemPrice = $reader.ReadUInt16()
            $scanPrice = $reader.ReadUInt32()
            $salePrice = $reader.ReadUInt32()
            $modelLoop = $reader.ReadByte()
            $modelShader = $reader.ReadByte()
            $soundId = $reader.ReadUInt16()
            $applyRateMax = $reader.ReadByte()
            $applyRateMin = $reader.ReadByte()
            $applyElement = $reader.ReadByte()
            $belonging = $reader.ReadUInt16()
            $codeTag = $reader.ReadByte()
            $questRefCount = [int]$reader.ReadUInt32()
            $quests = @(0, 0, 0)
            for ($q = 0; $q -lt $questRefCount; $q++) {
                $questId = [int]$reader.ReadUInt32()
                if ($q -lt 3) { $quests[$q] = $questId }
            }
            $digiviceSkillSlot = $reader.ReadByte()
            $digiviceChipsetSlot = $reader.ReadByte()
            $questRequire = $reader.ReadUInt32()
            $useTimeType = $reader.ReadByte()
            $useTimeMinutes = $reader.ReadUInt32()
            $useTimePassFlag = $reader.ReadByte()
            $doNotUseType = $reader.ReadUInt16()
            $useBattle = $reader.ReadByte()
            $modelNifFile = Read-CountedAscii $reader
            $modelEffectFile = Read-CountedAscii $reader
            $nifFile = Read-CountedAscii $reader

            $text = $strings[[int]$itemId]
            $items.Add([pscustomobject]@{
                ItemTableKey = $itemTableKey
                ItemId = [int]$itemId
                Name = if ($null -ne $text) { $text.Name } else { [string]$itemId }
                TypeName = if ($null -ne $text) { $text.TypeName } else { "" }
                Comment = if ($null -ne $text) { $text.Comment } else { "" }
                Icon = [int]$icon
                NifFile = $nifFile
                Class = [int]$class
                TypeComment = ""
                CodeTag = [int]$codeTag
                TypeL = [int]$typeL
                TypeS = [int]$typeS
                TypeValue = [int]$typeValue
                TypeSection = [int]$typeSection
                SellType = [int]$sellType
                UseMode = [int]$useMode
                UseTimeGroup = [int]$useTimeGroup
                Overlap = [int]$overlap
                TamerMin = [int]$tamerMin
                TamerMax = [int]$tamerMax
                DigimonMin = [int]$digimonMin
                DigimonMax = [int]$digimonMax
                Possess = [int]$possess
                EquipSeries = [int]$equipSeries
                UseCharacter = [int]$useCharacter
                Drop = [int]$drop
                EventItemType = [int]$eventItemType
                EventItemPrice = [int]$eventItemPrice
                DigicorePrice = [int]$digicorePrice
                ScanPrice = [int]$scanPrice
                SalePrice = [int]$salePrice
                ModelNifFile = $modelNifFile
                ModelEffectFile = $modelEffectFile
                ModelLoop = [int]$modelLoop
                ModelShader = [int]$modelShader
                SkillCodeType = [int]$skillCodeType
                SkillCode = [int]$skillCode
                ApplyRateMax = [int]$applyRateMax
                ApplyRateMin = [int]$applyRateMin
                ApplyElement = [int]$applyElement
                SocketCount = [int]$socketCount
                SoundId = [int]$soundId
                Belonging = [int]$belonging
                Quest1 = $quests[0]
                Quest2 = $quests[1]
                Quest3 = $quests[2]
                DigiviceSkillSlot = [int]$digiviceSkillSlot
                DigiviceChipsetSlot = [int]$digiviceChipsetSlot
                QuestRequire = [int]$questRequire
                UseTimeType = [int]$useTimeType
                UseTimeMinutes = [int]$useTimeMinutes
                UseBattle = [int]$useBattle
                DoNotUseType = [int]$doNotUseType
                UseTimePassFlag = [int]$useTimePassFlag
            })
        }
        return $items
    }
    finally {
        $reader.Dispose()
        $fs.Dispose()
    }
}

function Read-LegacyItemRecords([string]$path) {
    $bytes = [IO.File]::ReadAllBytes($path)
    $count = [BitConverter]::ToInt32($bytes, 0)
    $tailOffset = 4 + ($count * $LegacyItemRecordSize)
    if ($tailOffset -gt $bytes.Length) {
        throw "Base ItemList.bin is shorter than expected. count=$count tailOffset=$tailOffset size=$($bytes.Length)"
    }

    $recordsById = @{}
    $orderedIds = New-Object System.Collections.Generic.List[int]
    for ($i = 0; $i -lt $count; $i++) {
        $offset = 4 + ($i * $LegacyItemRecordSize)
        $itemId = [BitConverter]::ToInt32($bytes, $offset)
        $record = New-Object byte[] $LegacyItemRecordSize
        [Array]::Copy($bytes, $offset, $record, 0, $LegacyItemRecordSize)
        $recordsById[[string]$itemId] = $record
        $orderedIds.Add($itemId)
    }

    return [pscustomobject]@{
        Bytes = $bytes
        Count = $count
        TailOffset = $tailOffset
        TailSize = $bytes.Length - $tailOffset
        RecordsById = $recordsById
        OrderedIds = $orderedIds
    }
}

function Read-LegacyName([byte[]]$record) {
    $nameBytes = New-Object byte[] (64 * 2)
    [Array]::Copy($record, 4, $nameBytes, 0, $nameBytes.Length)
    $end = 0
    while ($end + 1 -lt $nameBytes.Length) {
        if ($nameBytes[$end] -eq 0 -and $nameBytes[$end + 1] -eq 0) { break }
        $end += 2
    }

    return [Text.Encoding]::Unicode.GetString($nameBytes, 0, $end)
}

function Skip-LegacySection([byte[]]$bytes, [ref]$offset, [int]$recordSize, [string]$sectionName) {
    if ($offset.Value + 4 -gt $bytes.Length) {
        throw "ItemList tail ended before section $sectionName count."
    }

    $count = [BitConverter]::ToInt32($bytes, $offset.Value)
    if ($count -lt 0) {
        throw "ItemList section $sectionName has a negative count: $count"
    }

    $sectionSize = 4 + ($count * $recordSize)
    if ($offset.Value + $sectionSize -gt $bytes.Length) {
        throw "ItemList section $sectionName overruns file. count=$count recordSize=$recordSize offset=$($offset.Value) size=$($bytes.Length)"
    }

    $offset.Value += $sectionSize
    return $count
}

function Split-LegacyTailForAccessoryReplacement([object]$legacy) {
    $offset = $legacy.TailOffset

    [void](Skip-LegacySection $legacy.Bytes ([ref]$offset) $ItemTapRecordSize "ItemTap")
    [void](Skip-LegacySection $legacy.Bytes ([ref]$offset) $ItemCoolTimeRecordSize "ItemCoolTime")
    [void](Skip-LegacySection $legacy.Bytes ([ref]$offset) $MapDisplayRecordSize "MapDisplay")
    [void](Skip-LegacySection $legacy.Bytes ([ref]$offset) $MapTypeNameRecordSize "MapTypeName")
    [void](Skip-LegacySection $legacy.Bytes ([ref]$offset) $RankRecordSize "Rank")
    [void](Skip-LegacySection $legacy.Bytes ([ref]$offset) $ElementItemRecordSize "ElementItem1")
    [void](Skip-LegacySection $legacy.Bytes ([ref]$offset) $ElementItemRecordSize "ElementItem2")
    [void](Skip-LegacySection $legacy.Bytes ([ref]$offset) $ExchangeRecordSize "Exchange")

    $beforeAccessoryLength = $offset - $legacy.TailOffset
    $beforeAccessory = Copy-Bytes $legacy.Bytes $legacy.TailOffset $beforeAccessoryLength

    $oldAccessoryOptionCount = Skip-LegacySection $legacy.Bytes ([ref]$offset) (Get-AccessoryOptionRecordSize $LegacyAccessoryOptionSlotCount) "AccessoryOption"
    $oldAccessoryEnchantCount = Skip-LegacySection $legacy.Bytes ([ref]$offset) $AccessoryEnchantRecordSize "AccessoryEnchant"
    $afterAccessory = Copy-Bytes $legacy.Bytes $offset ($legacy.Bytes.Length - $offset)

    return [pscustomobject]@{
        BeforeAccessory = $beforeAccessory
        AfterAccessory = $afterAccessory
        OldAccessoryOptionCount = $oldAccessoryOptionCount
        OldAccessoryEnchantCount = $oldAccessoryEnchantCount
    }
}

function Convert-AccessoryOptionBinToLegacySection([string]$path, [int]$slotCount) {
    if (-not (Test-Path $path)) {
        throw "Accessory option bin not found: $path"
    }

    $inStream = [IO.File]::OpenRead($path)
    $reader = [IO.BinaryReader]::new($inStream)
    $outStream = [IO.MemoryStream]::new()
    $writer = [IO.BinaryWriter]::new($outStream)
    $maxOptionCount = 0
    $truncatedRows = New-Object System.Collections.Generic.List[object]

    try {
        $count = [int]$reader.ReadUInt32()
        $writer.Write([int]$count)

        for ($i = 0; $i -lt $count; $i++) {
            $optionSetId = [int]$reader.ReadUInt32()
            $gainOptionCount = [int]$reader.ReadUInt16()
            $enchantLimit = [int]$reader.ReadUInt16()
            $optionCount = [int]$reader.ReadUInt32()
            $maxOptionCount = [Math]::Max($maxOptionCount, $optionCount)

            $options = New-Object System.Collections.Generic.List[object]
            for ($slot = 0; $slot -lt $optionCount; $slot++) {
                $options.Add([pscustomobject]@{
                    OptionType = [int]$reader.ReadUInt16()
                    Min = [uint32]$reader.ReadUInt32()
                    Max = [uint32]$reader.ReadUInt32()
                }) | Out-Null
            }

            if ($optionCount -gt $slotCount) {
                $truncatedRows.Add([pscustomobject]@{
                    OptionSetId = $optionSetId
                    OptionCount = $optionCount
                    WrittenSlots = $slotCount
                }) | Out-Null
            }

            $writer.Write([int]$optionSetId)
            $writer.Write([uint32]$optionSetId)
            $writer.Write([int16]$gainOptionCount)
            $writer.Write([int16]$enchantLimit)

            for ($slot = 0; $slot -lt $slotCount; $slot++) {
                if ($slot -lt $options.Count) {
                    $writer.Write((Clamp-UInt16 $options[$slot].OptionType))
                    Align-Writer $writer 4
                    $writer.Write([uint32]$options[$slot].Min)
                    $writer.Write([uint32]$options[$slot].Max)
                }
                else {
                    $writer.Write([int16]0)
                    Align-Writer $writer 4
                    $writer.Write([uint32]0)
                    $writer.Write([uint32]0)
                }
            }
        }

        return [pscustomobject]@{
            Bytes = $outStream.ToArray()
            Count = $count
            MaxOptionCount = $maxOptionCount
            TruncatedRows = $truncatedRows
        }
    }
    finally {
        $writer.Dispose()
        $outStream.Dispose()
        $reader.Dispose()
        $inStream.Dispose()
    }
}

function Convert-AccessoryEnchantBinToLegacySection([string]$path) {
    if (-not (Test-Path $path)) {
        throw "Accessory enchant bin not found: $path"
    }

    $inStream = [IO.File]::OpenRead($path)
    $reader = [IO.BinaryReader]::new($inStream)
    $outStream = [IO.MemoryStream]::new()
    $writer = [IO.BinaryWriter]::new($outStream)

    try {
        $count = [int]$reader.ReadUInt32()
        $writer.Write([int]$count)

        for ($i = 0; $i -lt $count; $i++) {
            $enchantId = [int]$reader.ReadUInt32()
            $optionType = [int]$reader.ReadUInt16()
            $value = [int]$reader.ReadUInt16()

            $writer.Write([int]$enchantId)
            $writer.Write([uint32]$enchantId)
            $writer.Write([int16]$optionType)
            $writer.Write([int16]$value)
        }

        return [pscustomobject]@{
            Bytes = $outStream.ToArray()
            Count = $count
        }
    }
    finally {
        $writer.Dispose()
        $outStream.Dispose()
        $reader.Dispose()
        $inStream.Dispose()
    }
}

function Write-LegacyItemRecord([IO.BinaryWriter]$writer, [object]$item) {
    $start = $writer.BaseStream.Position

    $writer.Write([int]$item.ItemId)
    Write-FixedUtf16 $writer $item.Name 64
    $writer.Write([int]$item.Icon)
    Write-FixedUtf16 $writer $item.Comment 512
    Write-FixedAscii $writer $item.NifFile 64
    $writer.Write((Clamp-UInt16 $item.Class))
    Write-FixedUtf16 $writer $item.TypeComment 64
    $writer.Write((Clamp-Byte $item.CodeTag))
    Align-Writer $writer 2
    $writer.Write((Clamp-UInt16 $item.TypeL))
    $writer.Write((Clamp-UInt16 $item.TypeS))
    $writer.Write([int]$item.TypeValue)
    $writer.Write([int]$item.TypeSection)
    $writer.Write((Clamp-UInt16 $item.SellType))
    $writer.Write((Clamp-Byte $item.UseMode))
    Align-Writer $writer 2
    $writer.Write((Clamp-UInt16 $item.UseTimeGroup))
    $writer.Write((Clamp-UInt16 $(if ($item.Overlap -le 0) { 1 } else { $item.Overlap })))
    $writer.Write((Clamp-UInt16 $item.TamerMin))
    $writer.Write((Clamp-UInt16 $item.TamerMax))
    $writer.Write((Clamp-UInt16 $item.DigimonMin))
    $writer.Write((Clamp-UInt16 $item.DigimonMax))
    $writer.Write((Clamp-UInt16 $item.Possess))
    $writer.Write((Clamp-UInt16 $item.EquipSeries))
    $writer.Write((Clamp-UInt16 $item.UseCharacter))
    $writer.Write([byte]0)
    Align-Writer $writer 2
    $writer.Write((Clamp-UInt16 $item.Drop))
    Align-Writer $writer 4
    $writer.Write([int]$item.EventItemType)
    $writer.Write((Clamp-UInt16 $item.EventItemPrice))
    $writer.Write((Clamp-UInt16 $item.DigicorePrice))
    $writer.Write([int]$item.ScanPrice)
    $writer.Write([int]$item.SalePrice)
    Write-FixedAscii $writer $item.ModelNifFile 64
    Write-FixedAscii $writer $item.ModelEffectFile 64
    $writer.Write((Clamp-Byte $item.ModelLoop))
    $writer.Write((Clamp-Byte $item.ModelShader))
    $writer.Write((Clamp-UInt16 $item.SkillCodeType))
    $writer.Write([int]$item.SkillCode)
    $writer.Write((Clamp-Byte $item.ApplyRateMax))
    $writer.Write((Clamp-Byte $item.ApplyRateMin))
    $writer.Write((Clamp-Byte $item.ApplyElement))
    Align-Writer $writer 2
    $writer.Write((Clamp-UInt16 $item.SocketCount))
    $writer.Write((Clamp-UInt16 $item.SoundId))
    $writer.Write((Clamp-Byte $item.Belonging))
    Align-Writer $writer 4
    $writer.Write([int]$item.Quest1)
    $writer.Write([int]$item.Quest2)
    $writer.Write([int]$item.Quest3)
    $writer.Write((Clamp-Byte $item.DigiviceSkillSlot))
    $writer.Write((Clamp-Byte $item.DigiviceChipsetSlot))
    Align-Writer $writer 4
    $writer.Write([int]$item.QuestRequire)
    $writer.Write((Clamp-Byte $item.UseTimeType))
    Align-Writer $writer 4
    $writer.Write([int]$item.UseTimeMinutes)
    $writer.Write((Clamp-Byte $item.UseBattle))
    Align-Writer $writer 2
    $writer.Write((Clamp-UInt16 $item.DoNotUseType))
    $writer.Write((Clamp-Byte $item.UseTimePassFlag))
    Align-Writer $writer 4

    $written = $writer.BaseStream.Position - $start
    if ($written -ne $LegacyItemRecordSize) {
        throw "Legacy item record size mismatch for item $($item.ItemId): wrote $written, expected $LegacyItemRecordSize"
    }
}

foreach ($dir in @(
    [IO.Path]::GetDirectoryName($OutputItemListBin),
    [IO.Path]::GetDirectoryName($ReportPath)
)) {
    if (-not [string]::IsNullOrWhiteSpace($dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }
}

$legacy = Read-LegacyItemRecords $BaseItemListBin
$tailParts = Split-LegacyTailForAccessoryReplacement $legacy
$modernAccessoryOptions = Convert-AccessoryOptionBinToLegacySection $AccessoryOptionBin $ModernAccessoryOptionSlotCount
$modernAccessoryEnchants = Convert-AccessoryEnchantBinToLegacySection $AccessoryEnchantBin

if ($modernAccessoryOptions.TruncatedRows.Count -gt 0) {
    $examples = $modernAccessoryOptions.TruncatedRows |
        Select-Object -First 5 |
        ForEach-Object { "$($_.OptionSetId):$($_.OptionCount)" }
    throw "Modern AccOption.bin has option sets wider than $ModernAccessoryOptionSlotCount slots: $($examples -join ', ')"
}

$strings = Read-ItemStrings $ItemStringBin
$items = Read-ModernItems $ItemDataBin $strings
$modernById = @{}
foreach ($item in $items) {
    $modernById[[string]$item.ItemId] = $item
}

$ids = @{}
$duplicates = New-Object System.Collections.Generic.List[int]
foreach ($item in $items) {
    if ($ids.ContainsKey($item.ItemId)) {
        $duplicates.Add($item.ItemId)
    } else {
        $ids[$item.ItemId] = $true
    }
}

if ($duplicates.Count -gt 0) {
    throw "ItemData contains duplicate ItemId values: $($duplicates | Select-Object -First 20 -Join ',')"
}

$outputItems = New-Object System.Collections.Generic.List[object]
$seenIds = @{}
$preservedLegacyCount = 0
$convertedNewCount = 0
$legacyOnlyCount = 0

foreach ($item in $items) {
    $key = [string]$item.ItemId
    $seenIds[$key] = $true

    if ($legacy.RecordsById.ContainsKey($key)) {
        $preservedLegacyCount++
        $outputItems.Add([pscustomobject]@{
            Source = "legacy"
            ItemId = $item.ItemId
            Name = Read-LegacyName $legacy.RecordsById[$key]
            RawRecord = $legacy.RecordsById[$key]
            ModernItem = $null
        })
        continue
    }

    $convertedNewCount++
    $outputItems.Add([pscustomobject]@{
        Source = "converted"
        ItemId = $item.ItemId
        Name = $item.Name
        RawRecord = $null
        ModernItem = $item
    })
}

foreach ($itemId in $legacy.OrderedIds) {
    $key = [string]$itemId
    if ($seenIds.ContainsKey($key)) {
        continue
    }

    $legacyOnlyCount++
    $outputItems.Add([pscustomobject]@{
        Source = "legacy-only"
        ItemId = $itemId
        Name = Read-LegacyName $legacy.RecordsById[$key]
        RawRecord = $legacy.RecordsById[$key]
        ModernItem = $null
    })
}

$outStream = [IO.File]::Open($OutputItemListBin, [IO.FileMode]::Create, [IO.FileAccess]::Write)
$writer = [IO.BinaryWriter]::new($outStream)
try {
    $writer.Write([int]$outputItems.Count)
    foreach ($entry in $outputItems) {
        if ($null -ne $entry.RawRecord) {
            $writer.Write([byte[]]$entry.RawRecord)
        }
        else {
            Write-LegacyItemRecord $writer $entry.ModernItem
        }
    }
    $writer.Write([byte[]]$tailParts.BeforeAccessory)
    $writer.Write([byte[]]$modernAccessoryOptions.Bytes)
    $writer.Write([byte[]]$modernAccessoryEnchants.Bytes)
    $writer.Write([byte[]]$tailParts.AfterAccessory)
}
finally {
    $writer.Dispose()
    $outStream.Dispose()
}

$outputItems |
    ForEach-Object {
        if ($null -ne $_.ModernItem) {
            $modern = $_.ModernItem
            [pscustomobject]@{
                Source = $_.Source
                ItemId = $modern.ItemId
                Name = $modern.Name
                Class = $modern.Class
                TypeL = $modern.TypeL
                TypeS = $modern.TypeS
                TypeValue = $modern.TypeValue
                TypeSection = $modern.TypeSection
                TypeName = $modern.TypeName
                SkillCode = $modern.SkillCode
                SocketCount = $modern.SocketCount
                Overlap = $modern.Overlap
                UseTimeGroup = $modern.UseTimeGroup
                UseTimeMinutes = $modern.UseTimeMinutes
                UseBattle = $modern.UseBattle
                NifFile = $modern.NifFile
                ModelNifFile = $modern.ModelNifFile
                ModelEffectFile = $modern.ModelEffectFile
            }
        }
        else {
            [pscustomobject]@{
                Source = $_.Source
                ItemId = $_.ItemId
                Name = $_.Name
                Class = ""
                TypeL = ""
                TypeS = ""
                TypeValue = ""
                TypeSection = ""
                TypeName = ""
                SkillCode = ""
                SocketCount = ""
                Overlap = ""
                UseTimeGroup = ""
                UseTimeMinutes = ""
                UseBattle = ""
                NifFile = ""
                ModelNifFile = ""
                ModelEffectFile = ""
            }
        }
    } |
    Export-Csv -Path $ReportPath -NoTypeInformation -Encoding UTF8

$reportDir = [IO.Path]::GetDirectoryName($ReportPath)
$addedItemsReport = Join-Path $reportDir "itemlist-added-items.csv"
$nameDiffReport = Join-Path $reportDir "itemlist-name-diff.csv"

$outputItems |
    Where-Object { $_.Source -eq "converted" -and $null -ne $_.ModernItem } |
    ForEach-Object {
        $modern = $_.ModernItem
        [pscustomobject]@{
            ItemId = $modern.ItemId
            Name = $modern.Name
            TypeName = $modern.TypeName
            Class = $modern.Class
            TypeL = $modern.TypeL
            TypeS = $modern.TypeS
            TypeValue = $modern.TypeValue
            TypeSection = $modern.TypeSection
            SkillCode = $modern.SkillCode
            SocketCount = $modern.SocketCount
            Overlap = $modern.Overlap
            NifFile = $modern.NifFile
            ModelNifFile = $modern.ModelNifFile
            ModelEffectFile = $modern.ModelEffectFile
        }
    } |
    Export-Csv -Path $addedItemsReport -NoTypeInformation -Encoding UTF8

$nameDiffRows = New-Object System.Collections.Generic.List[object]
foreach ($item in $items) {
    $key = [string]$item.ItemId
    if (-not $legacy.RecordsById.ContainsKey($key)) {
        $nameDiffRows.Add([pscustomobject]@{
            Action = "AddConverted"
            ItemId = $item.ItemId
            LegacyName = ""
            ModernName = $item.Name
            ModernTypeName = $item.TypeName
            NameChanged = $true
            SocketCount = $item.SocketCount
            TypeL = $item.TypeL
            TypeS = $item.TypeS
            SkillCode = $item.SkillCode
        }) | Out-Null
        continue
    }

    $legacyName = Read-LegacyName $legacy.RecordsById[$key]
    if ($legacyName -ne $item.Name) {
        $nameDiffRows.Add([pscustomobject]@{
            Action = "PreserveLegacy"
            ItemId = $item.ItemId
            LegacyName = $legacyName
            ModernName = $item.Name
            ModernTypeName = $item.TypeName
            NameChanged = $true
            SocketCount = $item.SocketCount
            TypeL = $item.TypeL
            TypeS = $item.TypeS
            SkillCode = $item.SkillCode
        }) | Out-Null
    }
}
foreach ($itemId in $legacy.OrderedIds) {
    $key = [string]$itemId
    if ($modernById.ContainsKey($key)) {
        continue
    }

    $nameDiffRows.Add([pscustomobject]@{
        Action = "LegacyOnly"
        ItemId = $itemId
        LegacyName = Read-LegacyName $legacy.RecordsById[$key]
        ModernName = ""
        ModernTypeName = ""
        NameChanged = $false
        SocketCount = ""
        TypeL = ""
        TypeS = ""
        SkillCode = ""
    }) | Out-Null
}
$nameDiffRows |
    Sort-Object ItemId |
    Export-Csv -Path $nameDiffReport -NoTypeInformation -Encoding UTF8

[pscustomobject]@{
    Output = $OutputItemListBin
    Report = $ReportPath
    AddedItemsReport = $addedItemsReport
    NameDiffReport = $nameDiffReport
    OldItemCount = $legacy.Count
    NewItemCount = $items.Count
    OutputItemCount = $outputItems.Count
    PreservedLegacyItems = $preservedLegacyCount
    ConvertedNewItems = $convertedNewCount
    LegacyOnlyItems = $legacyOnlyCount
    PreservedTailBytes = $legacy.TailSize
    OldAccessoryOptionCount = $tailParts.OldAccessoryOptionCount
    NewAccessoryOptionCount = $modernAccessoryOptions.Count
    AccessoryOptionSlots = $ModernAccessoryOptionSlotCount
    MaxModernAccessoryOptionCount = $modernAccessoryOptions.MaxOptionCount
    OldAccessoryEnchantCount = $tailParts.OldAccessoryEnchantCount
    NewAccessoryEnchantCount = $modernAccessoryEnchants.Count
    LegacyRecordSize = $LegacyItemRecordSize
    Size = (Get-Item $OutputItemListBin).Length
}
