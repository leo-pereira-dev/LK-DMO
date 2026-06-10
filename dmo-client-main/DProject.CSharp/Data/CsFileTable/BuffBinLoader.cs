using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record BuffBinData(IReadOnlyList<BuffRecord> Buffs);

public sealed record BuffRecord(
    ushort BuffId,
    string Name,
    string Comment,
    ushort BuffIcon,
    ushort BuffType,
    ushort BuffLifeType,
    ushort BuffTimeType,
    ushort MinLevel,
    ushort BuffClass,
    uint SkillCode,
    uint DigimonSkillCode,
    bool Delete,
    string EffectFile,
    ushort ConditionLevel);

public sealed class BuffBinLoader : IBinLoader
{
    private const int BuffNameLenReleaseEnglish = 64;
    private const int BuffCommentLen = 128;
    private const int BuffEffectLen = 64;
    private const int TCharSize = 2;
    private const int BuffInfoSizeReleaseEnglish = 476;
    private const int MaxReasonableCount = 100000;

    public string TableName => "CsBuffMng";

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
        if (!TryParse(data, out BuffBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out BuffBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * BuffInfoSizeReleaseEnglish > reader.Remaining)
        {
            return false;
        }

        var records = new List<BuffRecord>(count);
        var seen = new HashSet<ushort>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(BuffInfoSizeReleaseEnglish, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseBuff(recordBytes, out BuffRecord? buff) ||
                buff is null)
            {
                return false;
            }

            if (seen.Add(buff.BuffId))
            {
                records.Add(buff);
            }
        }

        parsed = new BuffBinData(records);
        return true;
    }

    private static bool TryParseBuff(ReadOnlyMemory<byte> bytes, out BuffRecord? buff)
    {
        buff = null;
        var reader = new SafeBinaryReader(bytes);
        if (!reader.TryReadUInt16(out ushort buffId) ||
            !reader.TryReadBytes(BuffNameLenReleaseEnglish * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(BuffCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
            !reader.TryReadUInt16(out ushort buffIcon) ||
            !reader.TryReadUInt16(out ushort buffType) ||
            !reader.TryReadUInt16(out ushort buffLifeType) ||
            !reader.TryReadUInt16(out ushort buffTimeType) ||
            !reader.TryReadUInt16(out ushort minLevel) ||
            !reader.TryReadUInt16(out ushort buffClass) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint skillCode) ||
            !reader.TryReadUInt32(out uint digimonSkillCode) ||
            !reader.TryReadBoolean(out bool delete) ||
            !reader.TryReadBytes(BuffEffectLen, out ReadOnlyMemory<byte> effectBytes) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt16(out ushort conditionLevel))
        {
            return false;
        }

        buff = new BuffRecord(
            buffId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedUtf16String(commentBytes.Span),
            buffIcon,
            buffType,
            buffLifeType,
            buffTimeType,
            minLevel,
            buffClass,
            skillCode,
            digimonSkillCode,
            delete,
            reader.ReadFixedAnsiString(effectBytes.Span),
            conditionLevel);
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
