using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record AchieveBinData(
    AchieveTypeNodeRecord RootType,
    IReadOnlyList<AchieveRecord> Achieves);

public sealed record AchieveTypeNodeRecord(
    string TypeName,
    IReadOnlyList<AchieveTypeNodeRecord> Children);

public sealed record AchieveRecord(
    uint QuestId,
    uint Icon,
    ushort Point,
    bool Display,
    string Name,
    string Comment,
    string Title,
    int Group,
    int SubGroup,
    int Type,
    int BuffCode);

public sealed class AchieveBinLoader : IBinLoader
{
    private const int TypeNameLen = 32;
    private const int AchieveNameLen = 64;
    private const int AchieveCommentLen = 256;
    private const int AchieveTitleLen = 64;
    private const int TCharSize = 2;
    private const int AchieveInfoSize = 796;
    private const int TypeInfoSize = TypeNameLen * TCharSize;
    private const int MaxTypeDepth = 32;
    private const int MaxReasonableChildren = 10000;

    public string TableName => "CsAchieveMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        if (!TryParse(data, binPath, out AchieveBinData? parsed, out BinFileSnapshot snapshot) ||
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
        out AchieveBinData? parsed,
        out BinFileSnapshot snapshot)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        var sections = new List<BinSectionSnapshot>();

        int typeOffset = reader.Position;
        if (!TryReadTypeNode(reader, 0, out AchieveTypeNodeRecord? rootType) ||
            rootType is null)
        {
            sections.Add(new BinSectionSnapshot("TypeTree", typeOffset, 0, null, reader.Position - typeOffset, false));
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        sections.Add(new BinSectionSnapshot("TypeTree", typeOffset, 1, null, reader.Position - typeOffset, true));

        int achieveOffset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int count) ||
            (long)count * AchieveInfoSize > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("Achieves", achieveOffset, 0, AchieveInfoSize, reader.Position - achieveOffset, false));
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        var achieves = new List<AchieveRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(AchieveInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseRecord(recordBytes, out AchieveRecord? record) ||
                record is null)
            {
                sections.Add(new BinSectionSnapshot("Achieves", achieveOffset, count, AchieveInfoSize, reader.Position - achieveOffset, false));
                snapshot = Snapshot(binPath, data.Length, sections);
                return false;
            }

            if (seen.Add(record.QuestId))
            {
                achieves.Add(record);
            }
        }

        bool complete = reader.EndOfData;
        sections.Add(new BinSectionSnapshot("Achieves", achieveOffset, count, AchieveInfoSize, reader.Position - achieveOffset, complete));
        snapshot = Snapshot(binPath, data.Length, sections);
        parsed = new AchieveBinData(rootType, achieves);
        return complete;
    }

    private static bool TryReadTypeNode(
        SafeBinaryReader reader,
        int depth,
        out AchieveTypeNodeRecord? node)
    {
        node = null;
        if (depth > MaxTypeDepth ||
            !reader.TryReadBytes(TypeInfoSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadInt32(out int childCount) ||
            childCount < 0 ||
            childCount > MaxReasonableChildren)
        {
            return false;
        }

        var children = new List<AchieveTypeNodeRecord>(childCount);
        for (int i = 0; i < childCount; i++)
        {
            if (!TryReadTypeNode(reader, depth + 1, out AchieveTypeNodeRecord? child) ||
                child is null)
            {
                return false;
            }

            children.Add(child);
        }

        node = new AchieveTypeNodeRecord(reader.ReadFixedUtf16String(nameBytes.Span), children);
        return true;
    }

    private static bool TryParseRecord(ReadOnlyMemory<byte> data, out AchieveRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt32(out uint questId) ||
            !reader.TryReadUInt32(out uint icon) ||
            !reader.TryReadUInt16(out ushort point) ||
            !reader.TryReadBoolean(out bool display) ||
            !reader.TrySkip(1) ||
            !reader.TryReadBytes(AchieveNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(AchieveCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
            !reader.TryReadBytes(AchieveTitleLen * TCharSize, out ReadOnlyMemory<byte> titleBytes) ||
            !reader.TryReadInt32(out int group) ||
            !reader.TryReadInt32(out int subGroup) ||
            !reader.TryReadInt32(out int type) ||
            !reader.TryReadInt32(out int buffCode) ||
            !reader.EndOfData)
        {
            return false;
        }

        record = new AchieveRecord(
            questId,
            icon,
            point,
            display,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedUtf16String(commentBytes.Span),
            reader.ReadFixedUtf16String(titleBytes.Span),
            group,
            subGroup,
            type,
            buffCode);
        return true;
    }

    private static BinFileSnapshot Snapshot(
        string path,
        int byteCount,
        IReadOnlyList<BinSectionSnapshot> sections)
    {
        return new BinFileSnapshot("CsAchieveMng", path, byteCount, sections);
    }
}
