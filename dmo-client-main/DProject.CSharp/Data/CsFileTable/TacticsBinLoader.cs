using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record TacticsBinData(
    IReadOnlyList<TacticsRecord> Tactics,
    IReadOnlyList<TacticsExplainRecord> Explains,
    IReadOnlyDictionary<uint, TacticsEnchantItemRecord> EnchantItems,
    IReadOnlyDictionary<int, IReadOnlyList<TacticsEnchantStatRecord>> EnchantStats,
    TacticsTranscendCorrectRecord Correct,
    IReadOnlyDictionary<int, IReadOnlyDictionary<int, TacticsTranscendInfoRecord>> TranscendInfos,
    IReadOnlyDictionary<uint, IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>> TranscendEvos,
    IReadOnlyDictionary<uint, IReadOnlyDictionary<int, uint>> ExpDigimonGroups);

public sealed record TacticsRecord(
    uint DigitamaId,
    int DigimonId,
    IReadOnlyList<int> RequiredItemTypes,
    IReadOnlyList<ushort> RequiredItemCounts,
    IReadOnlyList<byte> LimitLevels,
    IReadOnlyList<byte> ViewWarnings);

public sealed record TacticsExplainRecord(
    uint TacticsMonId,
    string Name,
    string Explain);

public sealed record TacticsEnchantItemRecord(
    uint ItemType,
    int LowLevel,
    int HighLevel,
    uint NeedMoney);

public sealed record TacticsEnchantStatRecord(
    int LowEnchantLevel,
    int HighEnchantLevel,
    int DigimonGrowMinLevel,
    int DigimonGrowMaxLevel,
    int NormalEnchantMinValue,
    int NormalEnchantMaxValue,
    int SpecialEnchantValue);

public sealed record TacticsTransNeedValueRecord(
    int NeedDigimonLevel,
    int NeedEnchantLevel,
    int NeedEvoMinType,
    int NeedEvoMaxType,
    int NeedScale,
    int NextDigimonGrowth,
    int NeedHatchLevelMin,
    int NeedHatchLevelMax);

public sealed record TacticsTranscendInfoRecord(
    TacticsTransNeedValueRecord NeedValue,
    uint TransCost,
    uint MaxExp);

public sealed record TacticsTranscendMaterialRecord(
    int NeedCount,
    uint ItemType,
    uint MinExp);

public sealed record TacticsTranscendCorrectRecord(
    int EnchantDefaultCorrect,
    IReadOnlyDictionary<int, IReadOnlyDictionary<int, float>> SameTypeCorrect);

public sealed class TacticsBinLoader : IBinLoader
{
    private const int DataMaxGrade = 2;
    private const int TacticsInfoSize = 20;
    private const int TacticsExplainInfoSize = 1156;
    private const int TacticsNameLen = 64;
    private const int TacticsExplainLen = 512;
    private const int TCharSize = 2;

    public string TableName => "CsTacticsMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        if (!TryParse(data, binPath, out TacticsBinData? parsed, out BinFileSnapshot snapshot) ||
            parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(
        ReadOnlyMemory<byte> data,
        string binPath,
        out TacticsBinData? parsed,
        out BinFileSnapshot snapshot)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        var sections = new List<BinSectionSnapshot>();

        int tacticsOffset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int tacticsCount) ||
            (long)tacticsCount * (sizeof(uint) + TacticsInfoSize) > reader.Remaining)
        {
            snapshot = Snapshot(binPath, data.Length, [new BinSectionSnapshot("Tactics", tacticsOffset, 0, TacticsInfoSize + sizeof(uint), 0, false)]);
            return false;
        }

        var tactics = new List<TacticsRecord>(tacticsCount);
        for (int i = 0; i < tacticsCount; i++)
        {
            if (!reader.TryReadUInt32(out uint digitamaId) ||
                !reader.TryReadBytes(TacticsInfoSize, out ReadOnlyMemory<byte> infoBytes) ||
                !TryParseTactics(digitamaId, infoBytes, out TacticsRecord? tactic) ||
                tactic is null)
            {
                sections.Add(new BinSectionSnapshot("Tactics", tacticsOffset, tacticsCount, TacticsInfoSize + sizeof(uint), reader.Position - tacticsOffset, false));
                snapshot = Snapshot(binPath, data.Length, sections);
                return false;
            }

            tactics.Add(tactic);
        }

        EnsureEosmonHatchTactics(tactics);
        sections.Add(new BinSectionSnapshot("Tactics", tacticsOffset, tacticsCount, TacticsInfoSize + sizeof(uint), reader.Position - tacticsOffset, true));

        int explainOffset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int explainCount) ||
            (long)explainCount * (sizeof(uint) + TacticsExplainInfoSize) > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("Explains", explainOffset, 0, TacticsExplainInfoSize + sizeof(uint), reader.Position - explainOffset, false));
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        var explains = new List<TacticsExplainRecord>(explainCount);
        for (int i = 0; i < explainCount; i++)
        {
            if (!reader.TryReadUInt32(out uint explainId) ||
                !reader.TryReadBytes(TacticsExplainInfoSize, out ReadOnlyMemory<byte> explainBytes) ||
                !TryParseTacticsExplain(explainId, explainBytes, out TacticsExplainRecord? explain) ||
                explain is null)
            {
                sections.Add(new BinSectionSnapshot("Explains", explainOffset, explainCount, TacticsExplainInfoSize + sizeof(uint), reader.Position - explainOffset, false));
                snapshot = Snapshot(binPath, data.Length, sections);
                return false;
            }

            explains.Add(explain);
        }

        sections.Add(new BinSectionSnapshot("Explains", explainOffset, explainCount, TacticsExplainInfoSize + sizeof(uint), reader.Position - explainOffset, true));

        if (!TryParseEnchantItems(reader, out Dictionary<uint, TacticsEnchantItemRecord>? enchantItems) ||
            enchantItems is null ||
            !TryParseEnchantStats(reader, out Dictionary<int, IReadOnlyList<TacticsEnchantStatRecord>>? enchantStats) ||
            enchantStats is null ||
            !TryParseCorrect(reader, out TacticsTranscendCorrectRecord? correct) ||
            correct is null ||
            !TryParseTranscendInfos(reader, out Dictionary<int, IReadOnlyDictionary<int, TacticsTranscendInfoRecord>>? transcendInfos) ||
            transcendInfos is null ||
            !TryParseTranscendEvos(reader, out Dictionary<uint, IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>>? transcendEvos) ||
            transcendEvos is null ||
            !TryParseExpDigimons(reader, out Dictionary<uint, IReadOnlyDictionary<int, uint>>? expDigimons) ||
            expDigimons is null)
        {
            sections.Add(new BinSectionSnapshot("TacticsTail", reader.Position, 0, null, 0, false));
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        bool complete = reader.EndOfData;
        sections.Add(new BinSectionSnapshot("TacticsTail", explainOffset, 0, null, reader.Position - explainOffset, complete));
        snapshot = Snapshot(binPath, data.Length, sections);
        parsed = new TacticsBinData(tactics, explains, enchantItems, enchantStats, correct, transcendInfos, transcendEvos, expDigimons);
        return complete;
    }

    private static bool TryParseTactics(
        uint digitamaId,
        ReadOnlyMemory<byte> data,
        out TacticsRecord? tactics)
    {
        tactics = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadInt32(out int digimonId))
        {
            return false;
        }

        var itemTypes = new List<int>(DataMaxGrade);
        for (int i = 0; i < DataMaxGrade; i++)
        {
            if (!reader.TryReadInt32(out int itemType))
            {
                return false;
            }

            itemTypes.Add(itemType);
        }

        var itemCounts = new List<ushort>(DataMaxGrade);
        for (int i = 0; i < DataMaxGrade; i++)
        {
            if (!reader.TryReadUInt16(out ushort count))
            {
                return false;
            }

            itemCounts.Add(count);
        }

        var limitLevels = new List<byte>(DataMaxGrade);
        for (int i = 0; i < DataMaxGrade; i++)
        {
            if (!reader.TryReadByte(out byte value))
            {
                return false;
            }

            limitLevels.Add(value);
        }

        var viewWarnings = new List<byte>(DataMaxGrade);
        for (int i = 0; i < DataMaxGrade; i++)
        {
            if (!reader.TryReadByte(out byte value))
            {
                return false;
            }

            viewWarnings.Add(value);
        }

        tactics = new TacticsRecord(digitamaId, digimonId, itemTypes, itemCounts, limitLevels, viewWarnings);
        return reader.EndOfData;
    }

    private static bool TryParseTacticsExplain(
        uint key,
        ReadOnlyMemory<byte> data,
        out TacticsExplainRecord? explain)
    {
        explain = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt32(out uint tacticsMonId) ||
            !reader.TryReadBytes(TacticsNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(TacticsExplainLen * TCharSize, out ReadOnlyMemory<byte> explainBytes) ||
            !reader.EndOfData)
        {
            return false;
        }

        explain = new TacticsExplainRecord(
            tacticsMonId == 0 ? key : tacticsMonId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedUtf16String(explainBytes.Span));
        return true;
    }

    private static bool TryParseEnchantItems(
        SafeBinaryReader reader,
        out Dictionary<uint, TacticsEnchantItemRecord>? items)
    {
        items = null;
        if (!MapBinFormat.TryReadCount(reader, out int count))
        {
            return false;
        }

        var result = new Dictionary<uint, TacticsEnchantItemRecord>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint itemCode) ||
                !reader.TryReadInt32(out int lowLevel) ||
                !reader.TryReadInt32(out int highLevel) ||
                !reader.TryReadUInt32(out uint needMoney))
            {
                return false;
            }

            result[itemCode] = new TacticsEnchantItemRecord(itemCode, lowLevel, highLevel, needMoney);
        }

        items = result;
        return true;
    }

    private static bool TryParseEnchantStats(
        SafeBinaryReader reader,
        out Dictionary<int, IReadOnlyList<TacticsEnchantStatRecord>>? stats)
    {
        stats = null;
        if (!MapBinFormat.TryReadCount(reader, out int count))
        {
            return false;
        }

        var result = new Dictionary<int, IReadOnlyList<TacticsEnchantStatRecord>>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int statIdx) ||
                !MapBinFormat.TryReadCount(reader, out int subCount) ||
                (long)subCount * 28 > reader.Remaining)
            {
                return false;
            }

            var entries = new List<TacticsEnchantStatRecord>(subCount);
            for (int j = 0; j < subCount; j++)
            {
                if (!reader.TryReadInt32(out int lowEnchantLv) ||
                    !reader.TryReadInt32(out int highEnchantLv) ||
                    !reader.TryReadInt32(out int growMin) ||
                    !reader.TryReadInt32(out int growMax) ||
                    !reader.TryReadInt32(out int normalMin) ||
                    !reader.TryReadInt32(out int normalMax) ||
                    !reader.TryReadInt32(out int specialValue))
                {
                    return false;
                }

                entries.Add(new TacticsEnchantStatRecord(lowEnchantLv, highEnchantLv, growMin, growMax, normalMin, normalMax, specialValue));
            }

            result[statIdx] = entries;
        }

        stats = result;
        return true;
    }

    private static bool TryParseCorrect(
        SafeBinaryReader reader,
        out TacticsTranscendCorrectRecord? correct)
    {
        correct = null;
        if (!reader.TryReadInt32(out int enchantDefaultCorrect) ||
            !MapBinFormat.TryReadCount(reader, out int groupCount))
        {
            return false;
        }

        var groups = new Dictionary<int, IReadOnlyDictionary<int, float>>();
        for (int i = 0; i < groupCount; i++)
        {
            if (!reader.TryReadInt32(out int sameType) ||
                !MapBinFormat.TryReadCount(reader, out int subCount) ||
                (long)subCount * 8 > reader.Remaining)
            {
                return false;
            }

            var values = new Dictionary<int, float>();
            for (int j = 0; j < subCount; j++)
            {
                if (!reader.TryReadInt32(out int growth) ||
                    !reader.TryReadSingle(out float value))
                {
                    return false;
                }

                values[growth] = value;
            }

            groups[sameType] = values;
        }

        correct = new TacticsTranscendCorrectRecord(enchantDefaultCorrect, groups);
        return true;
    }

    private static bool TryParseTranscendInfos(
        SafeBinaryReader reader,
        out Dictionary<int, IReadOnlyDictionary<int, TacticsTranscendInfoRecord>>? infos)
    {
        infos = null;
        if (!MapBinFormat.TryReadCount(reader, out int count))
        {
            return false;
        }

        var result = new Dictionary<int, IReadOnlyDictionary<int, TacticsTranscendInfoRecord>>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int targetGrowth) ||
                !MapBinFormat.TryReadCount(reader, out int subCount) ||
                (long)subCount * 44 > reader.Remaining)
            {
                return false;
            }

            var entries = new Dictionary<int, TacticsTranscendInfoRecord>();
            for (int j = 0; j < subCount; j++)
            {
                if (!reader.TryReadInt32(out int currentGrowth) ||
                    !TryReadTranscendInfo(reader, out TacticsTranscendInfoRecord? info) ||
                    info is null)
                {
                    return false;
                }

                entries[currentGrowth] = info;
            }

            result[targetGrowth] = entries;
        }

        infos = result;
        return true;
    }

    private static bool TryReadTranscendInfo(
        SafeBinaryReader reader,
        out TacticsTranscendInfoRecord? info)
    {
        info = null;
        if (!reader.TryReadInt32(out int needDigimonLv) ||
            !reader.TryReadInt32(out int needEnchantLv) ||
            !reader.TryReadInt32(out int needEvoMinType) ||
            !reader.TryReadInt32(out int needEvoMaxType) ||
            !reader.TryReadInt32(out int needScale) ||
            !reader.TryReadInt32(out int nextDigimonGrowth) ||
            !reader.TryReadInt32(out int needHatchLvMin) ||
            !reader.TryReadInt32(out int needHatchLvMax) ||
            !reader.TryReadUInt32(out uint transCost) ||
            !reader.TryReadUInt32(out uint maxExp))
        {
            return false;
        }

        info = new TacticsTranscendInfoRecord(
            new TacticsTransNeedValueRecord(
                needDigimonLv,
                needEnchantLv,
                needEvoMinType,
                needEvoMaxType,
                needScale,
                nextDigimonGrowth,
                needHatchLvMin,
                needHatchLvMax),
            transCost,
            maxExp);
        return true;
    }

    private static bool TryParseTranscendEvos(
        SafeBinaryReader reader,
        out Dictionary<uint, IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>>? evos)
    {
        evos = null;
        if (!MapBinFormat.TryReadCount(reader, out int count))
        {
            return false;
        }

        var result = new Dictionary<uint, IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint digimonIdx) ||
                !MapBinFormat.TryReadCount(reader, out int subCount))
            {
                return false;
            }

            var byUseType = new Dictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>();
            for (int j = 0; j < subCount; j++)
            {
                if (!reader.TryReadInt32(out int itemUseType) ||
                    !MapBinFormat.TryReadCount(reader, out int listCount) ||
                    (long)listCount * 12 > reader.Remaining)
                {
                    return false;
                }

                var materials = new List<TacticsTranscendMaterialRecord>(listCount);
                for (int k = 0; k < listCount; k++)
                {
                    if (!reader.TryReadInt32(out int needCount) ||
                        !reader.TryReadUInt32(out uint itemType) ||
                        !reader.TryReadUInt32(out uint minExp))
                    {
                        return false;
                    }

                    materials.Add(new TacticsTranscendMaterialRecord(needCount, itemType, minExp));
                }

                byUseType[itemUseType] = materials;
            }

            result[digimonIdx] = byUseType;
        }

        evos = result;
        return true;
    }

    private static bool TryParseExpDigimons(
        SafeBinaryReader reader,
        out Dictionary<uint, IReadOnlyDictionary<int, uint>>? expDigimons)
    {
        expDigimons = null;
        if (!MapBinFormat.TryReadCount(reader, out int count))
        {
            return false;
        }

        var result = new Dictionary<uint, IReadOnlyDictionary<int, uint>>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint digimonIdx) ||
                !MapBinFormat.TryReadCount(reader, out int subCount) ||
                (long)subCount * 8 > reader.Remaining)
            {
                return false;
            }

            var byGrowth = new Dictionary<int, uint>();
            for (int j = 0; j < subCount; j++)
            {
                if (!reader.TryReadInt32(out int growth) ||
                    !reader.TryReadUInt32(out uint fixedFigure))
                {
                    return false;
                }

                byGrowth[growth] = fixedFigure;
            }

            result[digimonIdx] = byGrowth;
        }

        expDigimons = result;
        return true;
    }

    private static void EnsureEosmonHatchTactics(List<TacticsRecord> tactics)
    {
        UpsertSeed(tactics, 78032, 35153, 8108, 8117, 10, 10, 3, 5, 0, 3);
        UpsertSeed(tactics, 78033, 35153, 8108, 8117, 10, 10, 4, 5, 0, 4);
        UpsertSeed(tactics, 78034, 35153, 8108, 8117, 10, 10, 5, 5, 0, 5);
        UpsertSeed(tactics, 78035, 35153, 8108, 8117, 10, 10, 3, 5, 0, 3);
        UpsertSeed(tactics, 78036, 35153, 8108, 8117, 10, 10, 4, 5, 0, 4);
    }

    private static void UpsertSeed(
        List<TacticsRecord> tactics,
        uint digitamaId,
        int digimonId,
        int lowDataItem,
        int midDataItem,
        ushort lowDataCount,
        ushort midDataCount,
        byte lowLimitLevel,
        byte midLimitLevel,
        byte lowViewWarning,
        byte midViewWarning)
    {
        int existingIndex = tactics.FindIndex(record => record.DigitamaId == digitamaId);
        var seed = new TacticsRecord(
            digitamaId,
            digimonId,
            [lowDataItem, midDataItem],
            [lowDataCount, midDataCount],
            [lowLimitLevel, midLimitLevel],
            [lowViewWarning, midViewWarning]);

        if (existingIndex >= 0)
        {
            tactics[existingIndex] = seed;
        }
        else
        {
            tactics.Add(seed);
        }
    }

    private static BinFileSnapshot Snapshot(
        string path,
        int byteCount,
        IReadOnlyList<BinSectionSnapshot> sections)
    {
        return new BinFileSnapshot("CsTacticsMng", path, byteCount, sections);
    }
}
