using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record AddExpBinData(IReadOnlyList<AddExpRecord> Records);

public sealed record AddExpRecord(
    uint Id,
    uint SkillId,
    int IconId,
    string Title,
    string Comment);

public sealed class AddExpBinLoader : IBinLoader
{
    private const int ItemNameLen = 64;
    private const int ItemCommentLen = 512;
    private const int TCharSize = 2;
    private const int AddExpInfoSize = 1164;

    public string TableName => "CsAddExpMng";

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
        if (!TryParse(data, out AddExpBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out AddExpBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!MapBinFormat.TryReadCount(reader, out int count) ||
            (long)count * AddExpInfoSize > reader.Remaining)
        {
            return false;
        }

        var records = new List<AddExpRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(AddExpInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseRecord(recordBytes, out AddExpRecord? record) ||
                record is null)
            {
                return false;
            }

            if (seen.Add(record.Id))
            {
                records.Add(record);
            }
        }

        parsed = new AddExpBinData(records);
        return reader.EndOfData;
    }

    private static bool TryParseRecord(ReadOnlyMemory<byte> data, out AddExpRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt32(out uint id) ||
            !reader.TryReadUInt32(out uint skillId) ||
            !reader.TryReadInt32(out int iconId) ||
            !reader.TryReadBytes(ItemNameLen * TCharSize, out ReadOnlyMemory<byte> titleBytes) ||
            !reader.TryReadBytes(ItemCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
            !reader.EndOfData)
        {
            return false;
        }

        record = new AddExpRecord(
            id,
            skillId,
            iconId,
            reader.ReadFixedUtf16String(titleBytes.Span),
            reader.ReadFixedUtf16String(commentBytes.Span));
        return true;
    }
}
