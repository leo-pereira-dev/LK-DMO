using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public enum CuidMessageType
{
    Contains = 1,
    Only = 2
}

public sealed record CuidBinData(
    IReadOnlyList<CuidIdRuleRecord> IdRules,
    IReadOnlyList<CuidChatRuleRecord> ChatRules);

public sealed record CuidIdRuleRecord(
    string Name,
    CuidMessageType MessageType);

public sealed record CuidChatRuleRecord(string Name);

public sealed class CuidBinLoader : IBinLoader
{
    private const int CuidNameLenReleaseEnglish = 32;
    private const int CuidChatLenReleaseEnglish = 32;
    private const int TCharSize = 2;
    private const int CuidIdRuleSize = CuidNameLenReleaseEnglish * TCharSize + sizeof(int);
    private const int CuidChatRuleSize = CuidChatLenReleaseEnglish * TCharSize;
    private const int MaxReasonableCount = 100000;

    public string TableName => "CsCuidMng";

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
        if (!TryParse(data, out CuidBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out CuidBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!TryReadIdRules(reader, out List<CuidIdRuleRecord>? idRules) ||
            idRules is null ||
            !TryReadChatRules(reader, out List<CuidChatRuleRecord>? chatRules) ||
            chatRules is null)
        {
            return false;
        }

        parsed = new CuidBinData(idRules, chatRules);
        return true;
    }

    private static bool TryReadIdRules(SafeBinaryReader reader, out List<CuidIdRuleRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * CuidIdRuleSize > reader.Remaining)
        {
            return false;
        }

        records = new List<CuidIdRuleRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(CuidNameLenReleaseEnglish * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
                !reader.TryReadInt32(out int messageType))
            {
                return false;
            }

            string name = reader.ReadFixedUtf16String(nameBytes.Span).ToLowerInvariant();
            if (string.IsNullOrWhiteSpace(name) ||
                !Enum.IsDefined(typeof(CuidMessageType), messageType))
            {
                continue;
            }

            records.Add(new CuidIdRuleRecord(name, (CuidMessageType)messageType));
        }

        return true;
    }

    private static bool TryReadChatRules(SafeBinaryReader reader, out List<CuidChatRuleRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * CuidChatRuleSize > reader.Remaining)
        {
            return false;
        }

        var seen = new HashSet<string>(StringComparer.Ordinal);
        records = new List<CuidChatRuleRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(CuidChatLenReleaseEnglish * TCharSize, out ReadOnlyMemory<byte> nameBytes))
            {
                return false;
            }

            string name = reader.ReadFixedUtf16String(nameBytes.Span).Trim().ToLowerInvariant();
            if (name.Length == 0 || !seen.Add(name))
            {
                continue;
            }

            records.Add(new CuidChatRuleRecord(name));
        }

        records.Sort((left, right) => right.Name.Length.CompareTo(left.Name.Length));
        return true;
    }

    private static bool TryReadCount(SafeBinaryReader reader, int maxCount, out int count)
    {
        count = 0;
        if (!reader.TryReadInt32(out int value) || value < 0 || value > maxCount)
        {
            return false;
        }

        count = value;
        return true;
    }
}
