using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record QuestBinData(IReadOnlyList<QuestBinRecord> Quests);

public sealed record QuestBinRecord(
    uint QuestId,
    uint ReceiveModelId,
    uint CompleteModelId,
    ushort Level,
    int ToolPosX,
    int ToolPosY,
    uint ManageId,
    bool Active,
    bool Immediate,
    bool ResetQuest,
    uint QuestType,
    uint StartTargetType,
    uint StartTargetId,
    uint QuestTargetType,
    int QuestTargetValue1,
    string TitleTab,
    string TitleText,
    string BodyText,
    string SimpleText,
    string HelperText,
    string ProcessText,
    string CompleteText,
    string ExpertText,
    IReadOnlyList<QuestSimpleRequirementRecord> ApplyRequirements,
    IReadOnlyList<QuestSimpleRequirementRecord> PreRequirements,
    IReadOnlyList<QuestCompleteRequirementRecord> CompleteRequirements,
    IReadOnlyList<QuestRewardRecord> Rewards,
    IReadOnlyList<int> Events);

public sealed record QuestSimpleRequirementRecord(
    uint Type,
    uint TypeId,
    int TypeCount);

public sealed record QuestCompleteRequirementRecord(
    uint Type,
    uint TypeId,
    int TypeCount,
    uint TargetId,
    int SubValue1,
    int SubValue2);

public sealed record QuestRewardRecord(
    uint Method,
    uint Type,
    uint TypeId,
    int TypeCount);

public sealed class QuestBinLoader : IBinLoader
{
    private const int QuestTitleLen = 80;
    private const int QuestSimpleLen = 128;
    private const int QuestHelperLen = 512;
    private const int QuestProcessLen = 320;
    private const int QuestCompleteLen = 700;
    private const int QuestBodyLenUsa = 2048;
    private const int TCharSize = 2;
    private const int SimpleRequirementSize = 12;
    private const int CompleteRequirementSize = 24;
    private const int RewardSize = 16;
    private const int EventMaxCount = 4;
    private const int MaxReasonableQuestCount = 100000;
    private const int MaxReasonableListCount = 10000;

    public string TableName => "CsQuestMng";

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
        if (!TryParse(data, out QuestBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out QuestBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!TryReadCount(reader, MaxReasonableQuestCount, out int count))
        {
            return false;
        }

        var records = new List<QuestBinRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!TryParseQuest(reader, out QuestBinRecord? quest) || quest is null)
            {
                return false;
            }

            if (seen.Add(quest.QuestId))
            {
                records.Add(quest);
            }
        }

        parsed = new QuestBinData(records);
        return true;
    }

    private static bool TryParseQuest(SafeBinaryReader reader, out QuestBinRecord? quest)
    {
        quest = null;
        if (!reader.TryReadUInt32(out uint questId) ||
            !reader.TryReadUInt32(out uint receiveModelId) ||
            !reader.TryReadUInt32(out uint completeModelId) ||
            !reader.TryReadUInt16(out ushort level) ||
            !reader.TryReadInt32(out int toolPosX) ||
            !reader.TryReadInt32(out int toolPosY) ||
            !reader.TryReadUInt32(out uint manageId) ||
            !reader.TryReadBoolean(out bool active) ||
            !reader.TryReadBoolean(out bool immediate) ||
            !reader.TryReadBoolean(out bool resetQuest) ||
            !reader.TryReadUInt32(out uint questType) ||
            !reader.TryReadUInt32(out uint startTargetType) ||
            !reader.TryReadUInt32(out uint startTargetId) ||
            !reader.TryReadUInt32(out uint questTargetType) ||
            !reader.TryReadInt32(out int questTargetValue1) ||
            !TryReadFixedUtf16(reader, QuestTitleLen, out string titleTab) ||
            !TryReadFixedUtf16(reader, QuestTitleLen, out string titleText) ||
            !TryReadFixedUtf16(reader, QuestBodyLenUsa, out string bodyText) ||
            !TryReadFixedUtf16(reader, QuestSimpleLen, out string simpleText) ||
            !TryReadFixedUtf16(reader, QuestHelperLen, out string helperText) ||
            !TryReadFixedUtf16(reader, QuestProcessLen, out string processText) ||
            !TryReadFixedUtf16(reader, QuestCompleteLen, out string completeText) ||
            !TryReadFixedUtf16(reader, QuestProcessLen, out string expertText) ||
            !TryReadSimpleRequirements(reader, out List<QuestSimpleRequirementRecord>? applyRequirements) ||
            applyRequirements is null ||
            !TryReadSimpleRequirements(reader, out List<QuestSimpleRequirementRecord>? preRequirements) ||
            preRequirements is null ||
            !TryReadCompleteRequirements(reader, out List<QuestCompleteRequirementRecord>? completeRequirements) ||
            completeRequirements is null ||
            !TryReadRewards(reader, out List<QuestRewardRecord>? rewards) ||
            rewards is null ||
            !TryReadEvents(reader, out List<int>? events) ||
            events is null)
        {
            return false;
        }

        quest = new QuestBinRecord(
            questId,
            receiveModelId,
            completeModelId,
            level,
            toolPosX,
            toolPosY,
            manageId,
            active,
            immediate,
            resetQuest,
            questType,
            startTargetType,
            startTargetId,
            questTargetType,
            questTargetValue1,
            titleTab,
            titleText,
            bodyText,
            simpleText,
            helperText,
            processText,
            completeText,
            expertText,
            applyRequirements,
            preRequirements,
            completeRequirements,
            rewards,
            events);
        return true;
    }

    private static bool TryReadSimpleRequirements(
        SafeBinaryReader reader,
        out List<QuestSimpleRequirementRecord>? requirements)
    {
        requirements = null;
        if (!TryReadCount(reader, MaxReasonableListCount, out int count) ||
            (long)count * SimpleRequirementSize > reader.Remaining)
        {
            return false;
        }

        var records = new List<QuestSimpleRequirementRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint type) ||
                !reader.TryReadUInt32(out uint typeId) ||
                !reader.TryReadInt32(out int typeCount))
            {
                return false;
            }

            records.Add(new QuestSimpleRequirementRecord(type, typeId, typeCount));
        }

        requirements = records;
        return true;
    }

    private static bool TryReadCompleteRequirements(
        SafeBinaryReader reader,
        out List<QuestCompleteRequirementRecord>? requirements)
    {
        requirements = null;
        if (!TryReadCount(reader, MaxReasonableListCount, out int count) ||
            (long)count * CompleteRequirementSize > reader.Remaining)
        {
            return false;
        }

        var records = new List<QuestCompleteRequirementRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint type) ||
                !reader.TryReadUInt32(out uint typeId) ||
                !reader.TryReadInt32(out int typeCount) ||
                !reader.TryReadUInt32(out uint targetId) ||
                !reader.TryReadInt32(out int subValue1) ||
                !reader.TryReadInt32(out int subValue2))
            {
                return false;
            }

            records.Add(new QuestCompleteRequirementRecord(type, typeId, typeCount, targetId, subValue1, subValue2));
        }

        requirements = records;
        return true;
    }

    private static bool TryReadRewards(SafeBinaryReader reader, out List<QuestRewardRecord>? rewards)
    {
        rewards = null;
        if (!TryReadCount(reader, MaxReasonableListCount, out int count) ||
            (long)count * RewardSize > reader.Remaining)
        {
            return false;
        }

        var records = new List<QuestRewardRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint method) ||
                !reader.TryReadUInt32(out uint type) ||
                !reader.TryReadUInt32(out uint typeId) ||
                !reader.TryReadInt32(out int typeCount))
            {
                return false;
            }

            records.Add(new QuestRewardRecord(method, type, typeId, typeCount));
        }

        rewards = records;
        return true;
    }

    private static bool TryReadEvents(SafeBinaryReader reader, out List<int>? events)
    {
        events = null;
        if (!TryReadCount(reader, EventMaxCount, out int count) ||
            count > EventMaxCount ||
            (long)count * sizeof(int) > reader.Remaining)
        {
            return false;
        }

        var records = new List<int>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int eventId))
            {
                return false;
            }

            records.Add(eventId);
        }

        events = records;
        return true;
    }

    private static bool TryReadFixedUtf16(SafeBinaryReader reader, int charCount, out string value)
    {
        value = string.Empty;
        int byteCount = charCount * TCharSize;
        if (!reader.TryReadBytes(byteCount, out ReadOnlyMemory<byte> bytes))
        {
            return false;
        }

        value = reader.ReadFixedUtf16String(bytes.Span);
        return true;
    }

    private static bool TryReadCount(SafeBinaryReader reader, int maxCount, out int count)
    {
        count = 0;
        return reader.TryReadInt32(out count) &&
            count >= 0 &&
            count <= maxCount;
    }
}
