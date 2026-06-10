using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record TalkBinData(
    IReadOnlyList<TalkDigimonRecord> DigimonTalks,
    IReadOnlyList<TalkEventRecord> EventTalks,
    IReadOnlyList<TalkMessageRecord> Messages,
    IReadOnlyList<TalkTipRecord> Tips,
    IReadOnlyList<TalkLoadingTipRecord> LoadingTips,
    IReadOnlyList<TalkTutorialRecord> Tutorials);

public sealed record TalkDigimonRecord(
    uint TalkId,
    uint Param,
    ushort TalkType,
    string Text,
    string ListText);

public sealed record TalkEventRecord(
    uint TalkId,
    uint TalkNumber,
    string Text);

public sealed record TalkMessageRecord(
    uint MessageId,
    int MessageType,
    int Type,
    string Title,
    string Message,
    uint LinkId);

public sealed record TalkTipRecord(
    uint TipId,
    string Text);

public sealed record TalkLoadingTipRecord(
    uint TipId,
    string Text,
    int Level);

public sealed record TalkTutorialRecord(
    uint TutorialId,
    string Text);

public sealed class TalkBinLoader : IBinLoader
{
    private const int TCharSize = 2;
    private const int DigimonTalkLen = 100;
    private const int EventTalkLen = 200;
    private const int MessageTitleLen = 16;
    private const int MessageBodyLen = 256;
    private const int TipLen = 200;
    private const int TutorialLen = 256;
    private const int DigimonTalkInfoSize = 408;
    private const int EventTalkInfoSize = 404;
    private const int MessageInfoSize = 560;
    private const int TipInfoSize = 400;
    private const int LoadingTipInfoSize = 404;
    private const int TutorialInfoSize = 512;
    private const int MaxReasonableCount = 100000;

    public string TableName => "CsTalkMng";

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
        if (!TryParseTalk(data, out TalkBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        string tutorialPath = Path.Combine(languagePath, "Tutorial.bin");
        if (BinDataSource.TryRead(tutorialPath, fileType, out byte[] tutorialData) &&
            TryParseTutorial(tutorialData, out List<TalkTutorialRecord>? tutorials) &&
            tutorials is not null)
        {
            parsed = parsed with { Tutorials = tutorials };
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParseTalk(ReadOnlyMemory<byte> data, out TalkBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!TryReadDigimonTalks(reader, out List<TalkDigimonRecord>? digimonTalks) ||
            digimonTalks is null ||
            !TryReadEventTalks(reader, out List<TalkEventRecord>? eventTalks) ||
            eventTalks is null ||
            !TryReadMessages(reader, out List<TalkMessageRecord>? messages) ||
            messages is null ||
            !TryReadTips(reader, out List<TalkTipRecord>? tips) ||
            tips is null ||
            !TryReadLoadingTips(reader, out List<TalkLoadingTipRecord>? loadingTips) ||
            loadingTips is null)
        {
            return false;
        }

        parsed = new TalkBinData(digimonTalks, eventTalks, messages, tips, loadingTips, []);
        return true;
    }

    private static bool TryReadDigimonTalks(SafeBinaryReader reader, out List<TalkDigimonRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * (sizeof(uint) + DigimonTalkInfoSize) > reader.Remaining)
        {
            return false;
        }

        records = new List<TalkDigimonRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint talkId) ||
                !reader.TryReadBytes(DigimonTalkInfoSize, out ReadOnlyMemory<byte> infoBytes) ||
                !TryParseDigimonTalkInfo(talkId, infoBytes, out TalkDigimonRecord? record) ||
                record is null)
            {
                return false;
            }

            if (seen.Add(talkId))
            {
                records.Add(record);
            }
        }

        return true;
    }

    private static bool TryParseDigimonTalkInfo(uint talkId, ReadOnlyMemory<byte> bytes, out TalkDigimonRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(bytes);
        if (!reader.TryReadUInt32(out uint param) ||
            !reader.TryReadUInt16(out ushort talkType) ||
            !reader.TryReadBytes(DigimonTalkLen * TCharSize, out ReadOnlyMemory<byte> textBytes) ||
            !reader.TryReadBytes(DigimonTalkLen * TCharSize, out ReadOnlyMemory<byte> listBytes))
        {
            return false;
        }

        record = new TalkDigimonRecord(
            talkId,
            param,
            talkType,
            reader.ReadFixedUtf16String(textBytes.Span),
            reader.ReadFixedUtf16String(listBytes.Span));
        return true;
    }

    private static bool TryReadEventTalks(SafeBinaryReader reader, out List<TalkEventRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * (sizeof(uint) + EventTalkInfoSize) > reader.Remaining)
        {
            return false;
        }

        records = new List<TalkEventRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint talkId) ||
                !reader.TryReadBytes(EventTalkInfoSize, out ReadOnlyMemory<byte> infoBytes) ||
                !TryParseEventTalkInfo(talkId, infoBytes, out TalkEventRecord? record) ||
                record is null)
            {
                return false;
            }

            if (seen.Add(talkId))
            {
                records.Add(record);
            }
        }

        return true;
    }

    private static bool TryParseEventTalkInfo(uint talkId, ReadOnlyMemory<byte> bytes, out TalkEventRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(bytes);
        if (!reader.TryReadUInt32(out uint talkNumber) ||
            !reader.TryReadBytes(EventTalkLen * TCharSize, out ReadOnlyMemory<byte> textBytes))
        {
            return false;
        }

        record = new TalkEventRecord(talkId, talkNumber, reader.ReadFixedUtf16String(textBytes.Span));
        return true;
    }

    private static bool TryReadMessages(SafeBinaryReader reader, out List<TalkMessageRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * MessageInfoSize > reader.Remaining)
        {
            return false;
        }

        records = new List<TalkMessageRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(MessageInfoSize, out ReadOnlyMemory<byte> infoBytes) ||
                !TryParseMessageInfo(infoBytes, out TalkMessageRecord? record) ||
                record is null)
            {
                return false;
            }

            if (seen.Add(record.MessageId))
            {
                records.Add(record);
            }
        }

        return true;
    }

    private static bool TryParseMessageInfo(ReadOnlyMemory<byte> bytes, out TalkMessageRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(bytes);
        if (!reader.TryReadUInt32(out uint messageId) ||
            !reader.TryReadInt32(out int messageType) ||
            !reader.TryReadInt32(out int type) ||
            !reader.TryReadBytes(MessageTitleLen * TCharSize, out ReadOnlyMemory<byte> titleBytes) ||
            !reader.TryReadBytes(MessageBodyLen * TCharSize, out ReadOnlyMemory<byte> messageBytes) ||
            !reader.TryReadUInt32(out uint linkId))
        {
            return false;
        }

        record = new TalkMessageRecord(
            messageId,
            messageType,
            type,
            reader.ReadFixedUtf16String(titleBytes.Span),
            reader.ReadFixedUtf16String(messageBytes.Span),
            linkId);
        return true;
    }

    private static bool TryReadTips(SafeBinaryReader reader, out List<TalkTipRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * (sizeof(uint) + TipInfoSize) > reader.Remaining)
        {
            return false;
        }

        records = new List<TalkTipRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint tipId) ||
                !reader.TryReadBytes(TipInfoSize, out ReadOnlyMemory<byte> tipBytes))
            {
                return false;
            }

            if (seen.Add(tipId))
            {
                records.Add(new TalkTipRecord(tipId, reader.ReadFixedUtf16String(tipBytes.Span)));
            }
        }

        return true;
    }

    private static bool TryReadLoadingTips(SafeBinaryReader reader, out List<TalkLoadingTipRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * (sizeof(uint) + LoadingTipInfoSize) > reader.Remaining)
        {
            return false;
        }

        records = new List<TalkLoadingTipRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint tipId) ||
                !reader.TryReadBytes(LoadingTipInfoSize, out ReadOnlyMemory<byte> infoBytes) ||
                !TryParseLoadingTipInfo(tipId, infoBytes, out TalkLoadingTipRecord? record) ||
                record is null)
            {
                return false;
            }

            if (seen.Add(tipId))
            {
                records.Add(record);
            }
        }

        return true;
    }

    private static bool TryParseLoadingTipInfo(uint tipId, ReadOnlyMemory<byte> bytes, out TalkLoadingTipRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(bytes);
        if (!reader.TryReadBytes(TipLen * TCharSize, out ReadOnlyMemory<byte> textBytes) ||
            !reader.TryReadInt32(out int level))
        {
            return false;
        }

        record = new TalkLoadingTipRecord(tipId, reader.ReadFixedUtf16String(textBytes.Span), level);
        return true;
    }

    private static bool TryParseTutorial(ReadOnlyMemory<byte> data, out List<TalkTutorialRecord>? records)
    {
        records = null;
        var reader = new SafeBinaryReader(data);
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * TutorialInfoSize > reader.Remaining)
        {
            return false;
        }

        records = new List<TalkTutorialRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(TutorialLen * TCharSize, out ReadOnlyMemory<byte> textBytes))
            {
                return false;
            }

            records.Add(new TalkTutorialRecord((uint)i, reader.ReadFixedUtf16String(textBytes.Span)));
        }

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
