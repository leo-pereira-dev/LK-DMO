using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record ItemBinData(
    IReadOnlyList<ItemBinRecord> Items,
    IReadOnlyList<ItemCategoryRecord> Categories,
    IReadOnlyList<ItemCoolTimeRecord> CoolTimes,
    IReadOnlyDictionary<uint, uint> DisplayTypes,
    IReadOnlyDictionary<uint, string> TypeNames,
    IReadOnlyDictionary<int, ItemRankRecord> Ranks);

public sealed record ItemBinRecord(
    uint ItemId,
    string Name,
    int Icon,
    string Comment,
    string Nif,
    ushort Class,
    string TypeComment,
    byte CodeTag,
    ushort TypeL,
    ushort TypeS,
    int TypeValue,
    int Section,
    ushort SellType,
    byte UseMode,
    ushort UseTimeGroup,
    ushort Overlap,
    ushort TamerReqMinLevel,
    ushort TamerReqMaxLevel,
    ushort DigimonReqMinLevel,
    ushort DigimonReqMaxLevel,
    ushort Possess,
    ushort EquipSeries,
    ushort UseCharacter,
    bool Dummy,
    ushort Drop,
    uint EventItemType,
    ushort EventItemPrice,
    ushort DigiCorePrice,
    uint ScanPrice,
    uint Sale,
    string ModelNif,
    string ModelEffect,
    bool ModelLoop,
    bool ModelShader,
    ushort SkillCodeType,
    uint Skill,
    byte ApplyRateMax,
    byte ApplyRateMin,
    byte ApplyElement,
    ushort SocketCount,
    ushort SoundId,
    byte Belonging,
    int Quest1,
    int Quest2,
    int Quest3,
    byte DigiviceSkillSlot,
    byte DigiviceChipsetSlot,
    uint QuestRequire,
    byte UseTimeType,
    uint UseTimeMin,
    byte UseBattle,
    ushort DoNotUseType,
    bool UseTimeTypeFirstUse);

public sealed record ItemCategoryRecord(
    ushort SellClass,
    string Name);

public sealed record ItemCoolTimeRecord(
    uint GroupId,
    bool Network,
    double TimeSeconds);

public sealed record ItemRankRecord(
    ushort DropType,
    ushort DropCount);

public sealed class ItemBinLoader : IBinLoader
{
    private const int ItemNameLen = 64;
    private const int ItemCommentLen = 512;
    private const int ItemFileLen = 64;
    private const int TCharSize = 2;

    private const int ItemInfoSize = 1596;
    private const int CategoryInfoSize = 66;
    private const int CoolTimeInfoSize = 16;
    private const int TypeNameInfoSize = 128;

    public string TableName => "CsItemMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        BinFileSnapshot snapshot = CountPrefixedBinInspector.Inspect(TableName, binPath, data);
        if (!TryParse(data, out ItemBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out ItemBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);

        if (!TryParseItems(reader, out List<ItemBinRecord>? items) || items is null ||
            !TryParseCategories(reader, out List<ItemCategoryRecord>? categories) || categories is null ||
            !TryParseCoolTimes(reader, out List<ItemCoolTimeRecord>? coolTimes) || coolTimes is null)
        {
            return false;
        }

        if (!TryParseDisplayTypes(reader, out Dictionary<uint, uint>? displayTypes) || displayTypes is null ||
            !TryParseTypeNames(reader, out Dictionary<uint, string>? typeNames) || typeNames is null ||
            !TryParseRanks(reader, out Dictionary<int, ItemRankRecord>? ranks) || ranks is null)
        {
            return false;
        }

        parsed = new ItemBinData(items, categories, coolTimes, displayTypes, typeNames, ranks);
        return true;
    }

    private static bool TryParseItems(SafeBinaryReader reader, out List<ItemBinRecord>? items)
    {
        items = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new List<ItemBinRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(ItemInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseItem(recordBytes.Span, out ItemBinRecord? item) || item is null)
            {
                return false;
            }

            if (seen.Add(item.ItemId))
            {
                records.Add(item);
            }
        }

        items = records;
        return true;
    }

    private static bool TryParseItem(ReadOnlySpan<byte> bytes, out ItemBinRecord? item)
    {
        item = null;
        if (bytes.Length != ItemInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint itemId) ||
            !reader.TryReadBytes(ItemNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadInt32(out int icon) ||
            !reader.TryReadBytes(ItemCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
            !reader.TryReadBytes(ItemFileLen, out ReadOnlyMemory<byte> nifBytes) ||
            !reader.TryReadUInt16(out ushort itemClass) ||
            !reader.TryReadBytes(ItemNameLen * TCharSize, out ReadOnlyMemory<byte> typeCommentBytes) ||
            !reader.TryReadByte(out byte codeTag) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt16(out ushort typeL) ||
            !reader.TryReadUInt16(out ushort typeS) ||
            !reader.TryReadInt32(out int typeValue) ||
            !reader.TryReadInt32(out int section) ||
            !reader.TryReadUInt16(out ushort sellType) ||
            !reader.TryReadByte(out byte useMode) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt16(out ushort useTimeGroup) ||
            !reader.TryReadUInt16(out ushort overlap) ||
            !reader.TryReadUInt16(out ushort tamerReqMinLevel) ||
            !reader.TryReadUInt16(out ushort tamerReqMaxLevel) ||
            !reader.TryReadUInt16(out ushort digimonReqMinLevel) ||
            !reader.TryReadUInt16(out ushort digimonReqMaxLevel) ||
            !reader.TryReadUInt16(out ushort possess) ||
            !reader.TryReadUInt16(out ushort equipSeries) ||
            !reader.TryReadUInt16(out ushort useCharacter) ||
            !reader.TryReadBoolean(out bool dummy) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt16(out ushort drop) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint eventItemType) ||
            !reader.TryReadUInt16(out ushort eventItemPrice) ||
            !reader.TryReadUInt16(out ushort digiCorePrice) ||
            !reader.TryReadUInt32(out uint scanPrice) ||
            !reader.TryReadUInt32(out uint sale) ||
            !reader.TryReadBytes(ItemFileLen, out ReadOnlyMemory<byte> modelNifBytes) ||
            !reader.TryReadBytes(ItemFileLen, out ReadOnlyMemory<byte> modelEffectBytes) ||
            !reader.TryReadBoolean(out bool modelLoop) ||
            !reader.TryReadBoolean(out bool modelShader) ||
            !reader.TryReadUInt16(out ushort skillCodeType) ||
            !reader.TryReadUInt32(out uint skill) ||
            !reader.TryReadByte(out byte applyRateMax) ||
            !reader.TryReadByte(out byte applyRateMin) ||
            !reader.TryReadByte(out byte applyElement) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt16(out ushort socketCount) ||
            !reader.TryReadUInt16(out ushort soundId) ||
            !reader.TryReadByte(out byte belonging) ||
            !reader.TrySkip(3) ||
            !reader.TryReadInt32(out int quest1) ||
            !reader.TryReadInt32(out int quest2) ||
            !reader.TryReadInt32(out int quest3) ||
            !reader.TryReadByte(out byte digiviceSkillSlot) ||
            !reader.TryReadByte(out byte digiviceChipsetSlot) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint questRequire) ||
            !reader.TryReadByte(out byte useTimeType) ||
            !reader.TrySkip(3) ||
            !reader.TryReadUInt32(out uint useTimeMin) ||
            !reader.TryReadByte(out byte useBattle) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt16(out ushort doNotUseType) ||
            !reader.TryReadBoolean(out bool useTimeTypeFirstUse) ||
            !reader.TrySkip(3))
        {
            return false;
        }

        item = new ItemBinRecord(
            itemId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            icon,
            reader.ReadFixedUtf16String(commentBytes.Span),
            reader.ReadFixedAnsiString(nifBytes.Span),
            itemClass,
            reader.ReadFixedUtf16String(typeCommentBytes.Span),
            codeTag,
            typeL,
            typeS,
            typeValue,
            section,
            sellType,
            useMode,
            useTimeGroup,
            overlap,
            tamerReqMinLevel,
            tamerReqMaxLevel,
            digimonReqMinLevel,
            digimonReqMaxLevel,
            possess,
            equipSeries,
            useCharacter,
            dummy,
            drop,
            eventItemType,
            eventItemPrice,
            digiCorePrice,
            scanPrice,
            sale,
            reader.ReadFixedAnsiString(modelNifBytes.Span),
            reader.ReadFixedAnsiString(modelEffectBytes.Span),
            modelLoop,
            modelShader,
            skillCodeType,
            skill,
            applyRateMax,
            applyRateMin,
            applyElement,
            socketCount,
            soundId,
            belonging,
            quest1,
            quest2,
            quest3,
            digiviceSkillSlot,
            digiviceChipsetSlot,
            questRequire,
            useTimeType,
            useTimeMin,
            useBattle,
            doNotUseType,
            useTimeTypeFirstUse);
        return reader.Position == ItemInfoSize;
    }

    private static bool TryParseCategories(SafeBinaryReader reader, out List<ItemCategoryRecord>? categories)
    {
        categories = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new List<ItemCategoryRecord>(count);
        var seen = new HashSet<ushort>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(CategoryInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            var recordReader = new SafeBinaryReader(recordBytes);
            if (!recordReader.TryReadUInt16(out ushort sellClass) ||
                !recordReader.TryReadBytes(32 * TCharSize, out ReadOnlyMemory<byte> nameBytes))
            {
                return false;
            }

            if (seen.Add(sellClass))
            {
                records.Add(new ItemCategoryRecord(sellClass, recordReader.ReadFixedUtf16String(nameBytes.Span)));
            }
        }

        categories = records;
        return true;
    }

    private static bool TryParseCoolTimes(SafeBinaryReader reader, out List<ItemCoolTimeRecord>? coolTimes)
    {
        coolTimes = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new List<ItemCoolTimeRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(CoolTimeInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            var recordReader = new SafeBinaryReader(recordBytes);
            if (!recordReader.TryReadUInt32(out uint groupId) ||
                !recordReader.TryReadBoolean(out bool network) ||
                !recordReader.TrySkip(3) ||
                !recordReader.TryReadDouble(out double timeSeconds))
            {
                return false;
            }

            if (seen.Add(groupId))
            {
                records.Add(new ItemCoolTimeRecord(groupId, network, timeSeconds));
            }
        }

        coolTimes = records;
        return true;
    }

    private static bool TryParseDisplayTypes(SafeBinaryReader reader, out Dictionary<uint, uint>? displayTypes)
    {
        displayTypes = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new Dictionary<uint, uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint type) ||
                !reader.TryReadUInt32(out uint displayId))
            {
                return false;
            }

            records.TryAdd(type, displayId);
        }

        displayTypes = records;
        return true;
    }

    private static bool TryParseTypeNames(SafeBinaryReader reader, out Dictionary<uint, string>? typeNames)
    {
        typeNames = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new Dictionary<uint, string>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint type) ||
                !reader.TryReadBytes(TypeNameInfoSize, out ReadOnlyMemory<byte> nameBytes))
            {
                return false;
            }

            records.TryAdd(type, reader.ReadFixedUtf16String(nameBytes.Span));
        }

        typeNames = records;
        return true;
    }

    private static bool TryParseRanks(SafeBinaryReader reader, out Dictionary<int, ItemRankRecord>? ranks)
    {
        ranks = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new Dictionary<int, ItemRankRecord>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int itemNo) ||
                !reader.TryReadUInt16(out ushort dropType) ||
                !reader.TryReadUInt16(out ushort dropCount))
            {
                return false;
            }

            records.TryAdd(itemNo, new ItemRankRecord(dropType, dropCount));
        }

        ranks = records;
        return true;
    }
}
