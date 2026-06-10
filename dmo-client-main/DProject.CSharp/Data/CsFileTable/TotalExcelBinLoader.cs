using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public enum TotalExcelType
{
    SKILL,
    TALK_TIP,
    TALK_DIGIMON,
    TALK_EVENT,
    TALK_MESSAGE,
    TACTICS_EXPLAIN,
    ITEM_NAME,
    ITEM_COMMENT,
    PARCINGWORD,
    WORLDMAP,
    REGION,
    NPC_PENNAME,
    NPC,
    QUEST,
    HELPTEXT,
    TAMER,
    MAX_COUNT
}

public sealed record TotalExcelTimeRecord(int Year, int Month, int Day, int Hour, int Minute);

public sealed record TotalExcelRecord(
    uint Id,
    string Korean,
    string English,
    TotalExcelTimeRecord CreatedAt);

public sealed record TotalExcelBinData(IReadOnlyDictionary<TotalExcelType, IReadOnlyDictionary<uint, TotalExcelRecord>> Sheets);

public sealed class TotalExcelBinLoader : IBinLoader
{
    private const int KoreanCharCount = 1024;
    private const int EnglishCharCount = 2048;
    private const int FixedRecordSize = sizeof(uint) +
                                        (KoreanCharCount * sizeof(char)) +
                                        (EnglishCharCount * sizeof(char)) +
                                        (sizeof(int) * 5);
    private const int MaxReasonableCount = 100000;

    public string TableName => "CsTotalExcelMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = ResolveExistingPath(languagePath, fileType, out byte[] data);
        if (data.Length == 0)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        if (!TryParse(data, out TotalExcelBinData? parsed, out int bytesConsumed) || parsed is null)
        {
            var failedSnapshot = new BinFileSnapshot(
                TableName,
                binPath,
                data.Length,
                [new BinSectionSnapshot("Sheets", 0, 0, FixedRecordSize, bytesConsumed, false)]);

            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, failedSnapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, failedSnapshot);
        }

        int recordCount = parsed.Sheets.Values.Sum(static sheet => sheet.Count);
        var snapshot = new BinFileSnapshot(
            TableName,
            binPath,
            data.Length,
            [new BinSectionSnapshot("Sheets", 0, recordCount, FixedRecordSize, bytesConsumed, bytesConsumed <= data.Length)]);

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static string ResolveExistingPath(string languagePath, eFILE_TYPE fileType, out byte[] data)
    {
        foreach (string candidate in BuildCandidatePaths(languagePath))
        {
            if (fileType == eFILE_TYPE.FT_FILEPACK && BinDataSource.TryRead(candidate, fileType, out data))
            {
                return candidate;
            }

            if (File.Exists(candidate))
            {
                data = File.ReadAllBytes(candidate);
                return candidate;
            }
        }

        data = [];
        return "GameData\\English\\Total.bin";
    }

    private static IEnumerable<string> BuildCandidatePaths(string languagePath)
    {
        yield return "GameData\\English\\Total.bin";
        yield return Path.Combine("GameData", "English", "Total.bin");
        yield return Path.Combine(languagePath, "Total.bin");
        yield return "Total.bin";
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out TotalExcelBinData? parsed, out int bytesConsumed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadInt32(out int sheetCount) ||
            sheetCount < 0 ||
            sheetCount > (int)TotalExcelType.MAX_COUNT)
        {
            bytesConsumed = reader.Position;
            return false;
        }

        var sheets = new Dictionary<TotalExcelType, IReadOnlyDictionary<uint, TotalExcelRecord>>();
        for (int sheet = 0; sheet < sheetCount; sheet++)
        {
            if (!reader.TryReadInt32(out int sheetId) ||
                !reader.TryReadInt32(out int dataCount) ||
                sheetId < 0 ||
                sheetId >= (int)TotalExcelType.MAX_COUNT ||
                dataCount < 0 ||
                dataCount > MaxReasonableCount ||
                (long)dataCount * FixedRecordSize > reader.Remaining)
            {
                bytesConsumed = reader.Position;
                return false;
            }

            var records = new Dictionary<uint, TotalExcelRecord>();
            for (int i = 0; i < dataCount; i++)
            {
                if (!TryReadRecord(reader, out TotalExcelRecord? record) || record is null)
                {
                    bytesConsumed = reader.Position;
                    return false;
                }

                records[record.Id] = record;
            }

            sheets[(TotalExcelType)sheetId] = records;
        }

        bytesConsumed = reader.Position;
        parsed = new TotalExcelBinData(sheets);
        return true;
    }

    private static bool TryReadRecord(SafeBinaryReader reader, out TotalExcelRecord? record)
    {
        record = null;
        if (!reader.TryReadUInt32(out uint id) ||
            !reader.TryReadBytes(KoreanCharCount * sizeof(char), out ReadOnlyMemory<byte> koreanBytes) ||
            !reader.TryReadBytes(EnglishCharCount * sizeof(char), out ReadOnlyMemory<byte> englishBytes) ||
            !reader.TryReadInt32(out int year) ||
            !reader.TryReadInt32(out int month) ||
            !reader.TryReadInt32(out int day) ||
            !reader.TryReadInt32(out int hour) ||
            !reader.TryReadInt32(out int minute))
        {
            return false;
        }

        record = new TotalExcelRecord(
            id,
            reader.ReadFixedUtf16String(koreanBytes.Span),
            reader.ReadFixedUtf16String(englishBytes.Span),
            new TotalExcelTimeRecord(year, month, day, hour, minute));
        return true;
    }
}

public sealed class CsTotalExcelMng : IBinFileTableManager
{
    private readonly TotalExcelBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<TotalExcelType, Dictionary<uint, TotalExcelRecord>> _sheets = new();
    private readonly TotalExcelRecord _dummy = new(
        1,
        "translation missing (k)",
        "translation missing (e)",
        new TotalExcelTimeRecord(0, 0, 0, 0, 0));

    public CsTotalExcelMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsTotalExcelMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<TotalExcelType, Dictionary<uint, TotalExcelRecord>> Sheets => _sheets;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is TotalExcelBinData data)
        {
            foreach (KeyValuePair<TotalExcelType, IReadOnlyDictionary<uint, TotalExcelRecord>> sheet in data.Sheets)
            {
                _sheets[sheet.Key] = new Dictionary<uint, TotalExcelRecord>(sheet.Value);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _sheets.Clear();
        IsLoaded = false;
    }

    public static string? GetSheetName(TotalExcelType type)
    {
        return type switch
        {
            TotalExcelType.SKILL => "Skill",
            TotalExcelType.TALK_TIP => "Talk_Tip",
            TotalExcelType.TALK_DIGIMON => "Talk_Digimon",
            TotalExcelType.TALK_EVENT => "Talk_Event",
            TotalExcelType.TALK_MESSAGE => "Talk_Message",
            TotalExcelType.TACTICS_EXPLAIN => "Tactics_Explain",
            TotalExcelType.ITEM_NAME => "ItemName",
            TotalExcelType.ITEM_COMMENT => "ItemComment",
            TotalExcelType.PARCINGWORD => "ParcingWord",
            TotalExcelType.WORLDMAP => "WorldMap",
            TotalExcelType.REGION => "Region",
            TotalExcelType.NPC_PENNAME => "NpcPenName",
            TotalExcelType.NPC => "Npc",
            TotalExcelType.QUEST => "Quest",
            TotalExcelType.HELPTEXT => "HelpText",
            TotalExcelType.TAMER => "Tamer",
            _ => null
        };
    }

    public bool IsExist(TotalExcelType type, uint id) => IsData(type, id);

    public bool IsExistKor(TotalExcelType type, string korean)
    {
        return _sheets.TryGetValue(type, out Dictionary<uint, TotalExcelRecord>? sheet) &&
               sheet.Values.Any(record => string.Equals(record.Korean, korean, StringComparison.Ordinal));
    }

    public bool IsExistKor(TotalExcelType type, uint id, string korean)
    {
        return _sheets.TryGetValue(type, out Dictionary<uint, TotalExcelRecord>? sheet) &&
               sheet.TryGetValue(id, out TotalExcelRecord? record) &&
               string.Equals(record.Korean, korean, StringComparison.Ordinal);
    }

    public IReadOnlyDictionary<uint, TotalExcelRecord> GetMap(TotalExcelType type)
    {
        if (!_sheets.TryGetValue(type, out Dictionary<uint, TotalExcelRecord>? sheet))
        {
            sheet = new Dictionary<uint, TotalExcelRecord>();
            _sheets[type] = sheet;
        }

        return sheet;
    }

    public bool IsData(TotalExcelType type, uint key)
    {
        return _sheets.TryGetValue(type, out Dictionary<uint, TotalExcelRecord>? sheet) && sheet.ContainsKey(key);
    }

    public TotalExcelRecord GetData(TotalExcelType type, uint key)
    {
        return _sheets.TryGetValue(type, out Dictionary<uint, TotalExcelRecord>? sheet) &&
               sheet.TryGetValue(key, out TotalExcelRecord? record)
            ? record
            : _dummy;
    }
}
