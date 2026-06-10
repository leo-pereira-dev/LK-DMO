using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record TamerBinData(
    IReadOnlyList<TamerBinRecord> Tamers,
    IReadOnlyList<TamerEmotionRecord> Emotions);

public sealed record TamerBinRecord(
    uint TamerId,
    string Name,
    string SoundDirName,
    byte TamerType,
    IReadOnlyList<uint> SkillIds,
    string Comment,
    string Part,
    string Gender);

public sealed record TamerEmotionRecord(
    uint Id,
    string Name,
    IReadOnlyList<string> Commands,
    uint AnimationId,
    byte UseCommandCount);

public sealed class TamerBinLoader : IBinLoader
{
    private const int MaxFilename = 64;
    private const int TamerMaxSkillCount = 5;
    private const int TamerCommentLen = 512;
    private const int EmotionStrLen = 16;
    private const int EmotionCmdCount = 5;

    private const int TCharSize = 2;
    private const int TamerInfoSize = 1500;
    private const int EmotionInfoSize = 204;

    public string TableName => "CsTamerMng";

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
        if (!TryParse(data, out TamerBinData? parsed))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out TamerBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);

        if (!MapBinFormat.TryReadCount(reader, out int tamerCount) ||
            (long)tamerCount * TamerInfoSize > reader.Remaining)
        {
            return false;
        }

        var tamers = new List<TamerBinRecord>(tamerCount);
        for (int i = 0; i < tamerCount; i++)
        {
            if (!reader.TryReadBytes(TamerInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseTamer(recordBytes.Span, out TamerBinRecord? tamer) || tamer is null)
            {
                return false;
            }

            tamers.Add(tamer);
        }

        if (!MapBinFormat.TryReadCount(reader, out int emotionCount) ||
            (long)emotionCount * EmotionInfoSize > reader.Remaining)
        {
            return false;
        }

        var emotions = new List<TamerEmotionRecord>(emotionCount);
        for (int i = 0; i < emotionCount; i++)
        {
            if (!reader.TryReadBytes(EmotionInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseEmotion(recordBytes.Span, out TamerEmotionRecord? emotion) || emotion is null)
            {
                return false;
            }

            emotions.Add(emotion);
        }

        parsed = new TamerBinData(tamers, emotions);
        return reader.EndOfData;
    }

    private static bool TryParseTamer(ReadOnlySpan<byte> bytes, out TamerBinRecord? tamer)
    {
        tamer = null;
        if (bytes.Length != TamerInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint tamerId) ||
            !reader.TryReadBytes(MaxFilename * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(MaxFilename, out ReadOnlyMemory<byte> soundBytes) ||
            !reader.TryReadByte(out byte tamerType))
        {
            return false;
        }

        if (!reader.TrySkip(3))
        {
            return false;
        }

        var skills = new List<uint>(TamerMaxSkillCount);
        for (int i = 0; i < TamerMaxSkillCount; i++)
        {
            if (!reader.TryReadUInt32(out uint skillId))
            {
                return false;
            }

            skills.Add(skillId);
        }

        if (!reader.TryReadBytes(TamerCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
            !reader.TryReadBytes(MaxFilename * TCharSize, out ReadOnlyMemory<byte> partBytes) ||
            !reader.TryReadBytes(MaxFilename * TCharSize, out ReadOnlyMemory<byte> genderBytes))
        {
            return false;
        }

        tamer = new TamerBinRecord(
            tamerId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedAnsiString(soundBytes.Span),
            tamerType,
            skills,
            reader.ReadFixedUtf16String(commentBytes.Span),
            reader.ReadFixedUtf16String(partBytes.Span),
            reader.ReadFixedUtf16String(genderBytes.Span));
        return true;
    }

    private static bool TryParseEmotion(ReadOnlySpan<byte> bytes, out TamerEmotionRecord? emotion)
    {
        emotion = null;
        if (bytes.Length != EmotionInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint id) ||
            !reader.TryReadBytes(EmotionStrLen * TCharSize, out ReadOnlyMemory<byte> nameBytes))
        {
            return false;
        }

        var commands = new List<string>(EmotionCmdCount);
        for (int i = 0; i < EmotionCmdCount; i++)
        {
            if (!reader.TryReadBytes(EmotionStrLen * TCharSize, out ReadOnlyMemory<byte> commandBytes))
            {
                return false;
            }

            commands.Add(reader.ReadFixedUtf16String(commandBytes.Span));
        }

        if (!reader.TryReadUInt32(out uint animationId) ||
            !reader.TryReadByte(out byte useCommandCount))
        {
            return false;
        }

        emotion = new TamerEmotionRecord(
            id,
            reader.ReadFixedUtf16String(nameBytes.Span),
            commands,
            animationId,
            useCommandCount);
        return true;
    }
}
