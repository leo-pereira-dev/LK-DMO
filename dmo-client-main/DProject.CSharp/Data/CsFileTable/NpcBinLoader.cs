using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record NpcBinData(
    IReadOnlyList<NpcBinRecord> Npcs,
    IReadOnlyDictionary<uint, NpcModelInfoRecord> ModelInfos,
    IReadOnlyDictionary<uint, NpcEventInfoRecord> CardGameInfos);

public sealed record NpcBinRecord(
    uint NpcId,
    uint MapId,
    int Type,
    int MoveType,
    uint DisplayFlag,
    uint ModelId,
    string PenName,
    string Name,
    string Talk,
    NpcTypePayload? TypePayload,
    IReadOnlyList<NpcQuestExtraRecord> QuestExtras);

public abstract record NpcTypePayload(int NpcType);

public sealed record NpcItemListTypePayload(
    int NpcType,
    IReadOnlyList<uint> ItemIds) : NpcTypePayload(NpcType);

public sealed record NpcPortalTypePayload(
    int PortalType,
    IReadOnlyList<NpcPortalRecord> Portals) : NpcTypePayload(NpcBinLoader.NpcTypePortal);

public sealed record NpcSpecialEventTypePayload(
    int SubType,
    IReadOnlyList<uint> ItemIds) : NpcTypePayload(NpcBinLoader.NpcTypeSpecialEvent);

public sealed record NpcPortalRecord(
    uint EventId,
    IReadOnlyList<NpcPortalRequirementRecord> Requirements);

public sealed record NpcPortalRequirementRecord(
    int EnableType,
    uint EnableId,
    uint EnableCount);

public sealed record NpcQuestExtraRecord(
    uint InitState,
    IReadOnlyList<NpcQuestActionRecord> Actions);

public sealed record NpcQuestActionRecord(
    uint ActionType,
    uint CompleteState,
    IReadOnlyList<int> QuestIds);

public sealed record NpcModelInfoRecord(
    uint ModelId,
    IReadOnlyList<short> Offsets,
    string Comment);

public sealed record NpcEventInfoRecord(
    uint NpcId,
    short TryCount,
    uint ExhaustMoney,
    uint ExhaustItem,
    uint ItemCount,
    IReadOnlyList<NpcEventItemRecord> Items);

public sealed record NpcEventItemRecord(
    uint ItemId,
    short Count);

public sealed class NpcBinLoader : IBinLoader
{
    public const int NpcTypePortal = 3;
    public const int NpcTypeSpecialEvent = 19;

    private const int NpcTypeNone = 0;
    private const int NpcTypeTrade = 1;
    private const int NpcTypeDigitamaTrade = 2;
    private const int NpcTypeMakeTactics = 4;
    private const int NpcTypeElementItem = 5;
    private const int NpcTypeWarehouse = 6;
    private const int NpcTypeTacticsHouse = 7;
    private const int NpcTypeGuild = 8;
    private const int NpcTypeDigiCore = 9;
    private const int NpcTypeCapsuleMachine = 10;
    private const int NpcTypeSkill = 11;
    private const int NpcTypeEventStore = 12;
    private const int NpcTypeDatsPortal = 13;
    private const int NpcTypePropertyStore = 14;
    private const int NpcTypeGotchaMachine = 15;
    private const int NpcTypeMastersMatching = 16;
    private const int NpcTypeMysteryMachine = 17;
    private const int NpcTypeSpiritEvolution = 18;
    private const int NpcTypeItemProduction = 20;
    private const int NpcTypeBattleRegistrant = 21;
    private const int NpcTypeInfiniteWarManager = 22;
    private const int NpcTypeInfiniteWarNoticeBoard = 23;
    private const int NpcTypeExtraEvolution = 24;
    private const int NpcTypeGdmoPortalTest = 25;

    private const int NpcSpecialEventCardGame = 1;
    private const int NpcSpecialEventPinokimon = 2;
    private const int NpcExtraQuest = 0;

    private const int NpcNameLen = 32;
    private const int NpcTalkLen = 512;
    private const int TCharSize = 2;
    private const int PortalRequirementCount = 3;
    private const int MaxCardItem = 5;

    private const int NpcInfoSize = 1176;
    private const int PortalRecordSize = 40;
    private const int ModelInfoSize = 140;
    private const int EventInfoSize = 60;
    private const int EventItemInfoSize = 8;
    private const int MaxReasonableVariableCount = 100000;

    public string TableName => "CsNpcMng";

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
        if (!TryParse(data, out NpcBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out NpcBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);

        if (!TryParseNpcs(reader, out List<NpcBinRecord>? npcs) || npcs is null ||
            !TryParseModelInfos(reader, out Dictionary<uint, NpcModelInfoRecord>? modelInfos) || modelInfos is null ||
            !TryParseNpcEvents(reader, out Dictionary<uint, NpcEventInfoRecord>? cardGameInfos) || cardGameInfos is null)
        {
            return false;
        }

        parsed = new NpcBinData(npcs, modelInfos, cardGameInfos);
        return true;
    }

    private static bool TryParseNpcs(SafeBinaryReader reader, out List<NpcBinRecord>? npcs)
    {
        npcs = null;
        if (!TryReadCount(reader, NpcInfoSize, out int count))
        {
            return false;
        }

        var records = new List<NpcBinRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(NpcInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseNpcInfo(recordBytes.Span, out NpcInfoFields info))
            {
                return false;
            }

            if (!TryParseTypePayload(reader, info.Type, out NpcTypePayload? typePayload) ||
                !TryParseQuestExtras(reader, out List<NpcQuestExtraRecord>? questExtras) ||
                questExtras is null)
            {
                return false;
            }

            if (seen.Add(info.NpcId))
            {
                records.Add(new NpcBinRecord(
                    info.NpcId,
                    info.MapId,
                    info.Type,
                    info.MoveType,
                    info.DisplayFlag,
                    info.ModelId,
                    info.PenName,
                    info.Name,
                    info.Talk,
                    typePayload,
                    questExtras));
            }
        }

        npcs = records;
        return true;
    }

    private static bool TryParseNpcInfo(ReadOnlySpan<byte> bytes, out NpcInfoFields info)
    {
        info = default;
        if (bytes.Length != NpcInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint npcId) ||
            !reader.TryReadUInt32(out uint mapId) ||
            !reader.TryReadInt32(out int type) ||
            !reader.TryReadInt32(out int moveType) ||
            !reader.TryReadUInt32(out uint displayFlag) ||
            !reader.TryReadUInt32(out uint modelId) ||
            !reader.TryReadBytes(NpcNameLen * TCharSize, out ReadOnlyMemory<byte> penNameBytes) ||
            !reader.TryReadBytes(NpcNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(NpcTalkLen * TCharSize, out ReadOnlyMemory<byte> talkBytes))
        {
            return false;
        }

        info = new NpcInfoFields(
            npcId,
            mapId,
            type,
            moveType,
            displayFlag,
            modelId,
            reader.ReadFixedUtf16String(penNameBytes.Span),
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedUtf16String(talkBytes.Span));
        return reader.Position == NpcInfoSize;
    }

    private static bool TryParseTypePayload(SafeBinaryReader reader, int npcType, out NpcTypePayload? payload)
    {
        payload = null;
        switch (npcType)
        {
            case NpcTypeNone:
            case NpcTypeDigitamaTrade:
            case NpcTypeMakeTactics:
            case NpcTypeElementItem:
            case NpcTypeWarehouse:
            case NpcTypeTacticsHouse:
            case NpcTypeCapsuleMachine:
            case NpcTypeSkill:
            case NpcTypeDatsPortal:
            case NpcTypeItemProduction:
            case NpcTypeBattleRegistrant:
            case NpcTypeInfiniteWarManager:
            case NpcTypeInfiniteWarNoticeBoard:
            case NpcTypeExtraEvolution:
            case NpcTypeGotchaMachine:
            case NpcTypeMysteryMachine:
            case NpcTypeSpiritEvolution:
                return true;

            case NpcTypeTrade:
            case NpcTypeGuild:
            case NpcTypeDigiCore:
            case NpcTypeEventStore:
            case NpcTypeGdmoPortalTest:
            case NpcTypePropertyStore:
            case NpcTypeMastersMatching:
                if (!TryReadItemIds(reader, out List<uint>? itemIds) || itemIds is null)
                {
                    return false;
                }

                payload = new NpcItemListTypePayload(npcType, itemIds);
                return true;

            case NpcTypePortal:
                return TryParsePortalPayload(reader, out payload);

            case NpcTypeSpecialEvent:
                return TryParseSpecialEventPayload(reader, out payload);

            default:
                return false;
        }
    }

    private static bool TryParsePortalPayload(SafeBinaryReader reader, out NpcTypePayload? payload)
    {
        payload = null;
        if (!reader.TryReadInt32(out int portalType) ||
            !TryReadCount(reader, PortalRecordSize, out int count))
        {
            return false;
        }

        var portals = new List<NpcPortalRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(PortalRecordSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            var recordReader = new SafeBinaryReader(recordBytes);
            if (!recordReader.TryReadUInt32(out uint eventId))
            {
                return false;
            }

            var requirements = new List<NpcPortalRequirementRecord>(PortalRequirementCount);
            for (int req = 0; req < PortalRequirementCount; req++)
            {
                if (!recordReader.TryReadInt32(out int enableType) ||
                    !recordReader.TryReadUInt32(out uint enableId) ||
                    !recordReader.TryReadUInt32(out uint enableCount))
                {
                    return false;
                }

                requirements.Add(new NpcPortalRequirementRecord(enableType, enableId, enableCount));
            }

            if (recordReader.Position != PortalRecordSize)
            {
                return false;
            }

            portals.Add(new NpcPortalRecord(eventId, requirements));
        }

        payload = new NpcPortalTypePayload(portalType, portals);
        return true;
    }

    private static bool TryParseSpecialEventPayload(SafeBinaryReader reader, out NpcTypePayload? payload)
    {
        payload = null;
        if (!reader.TryReadInt32(out int subtype))
        {
            return false;
        }

        List<uint> itemIds = [];
        if (subtype == NpcSpecialEventPinokimon)
        {
            if (!TryReadItemIds(reader, out List<uint>? parsedItemIds) || parsedItemIds is null)
            {
                return false;
            }

            itemIds = parsedItemIds;
        }

        if (subtype != NpcSpecialEventCardGame &&
            subtype != NpcSpecialEventPinokimon)
        {
            itemIds = [];
        }

        payload = new NpcSpecialEventTypePayload(subtype, itemIds);
        return true;
    }

    private static bool TryParseQuestExtras(SafeBinaryReader reader, out List<NpcQuestExtraRecord>? extras)
    {
        extras = null;
        if (!TryReadCount(reader, sizeof(int), out int count))
        {
            return false;
        }

        var records = new List<NpcQuestExtraRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int extraType))
            {
                return false;
            }

            if (extraType != NpcExtraQuest ||
                !TryParseQuestExtra(reader, out NpcQuestExtraRecord? questExtra) ||
                questExtra is null)
            {
                return false;
            }

            records.Add(questExtra);
        }

        extras = records;
        return true;
    }

    private static bool TryParseQuestExtra(SafeBinaryReader reader, out NpcQuestExtraRecord? questExtra)
    {
        questExtra = null;
        if (!reader.TryReadUInt32(out uint initState) ||
            !TryReadCount(reader, sizeof(uint) + sizeof(uint) + sizeof(int), out int actionCount))
        {
            return false;
        }

        var actions = new List<NpcQuestActionRecord>(actionCount);
        for (int i = 0; i < actionCount; i++)
        {
            if (!reader.TryReadUInt32(out uint actionType) ||
                !reader.TryReadUInt32(out uint completeState) ||
                !TryReadCount(reader, sizeof(int), out int questCount))
            {
                return false;
            }

            var questIds = new List<int>(questCount);
            for (int quest = 0; quest < questCount; quest++)
            {
                if (!reader.TryReadInt32(out int questId))
                {
                    return false;
                }

                questIds.Add(questId);
            }

            actions.Add(new NpcQuestActionRecord(actionType, completeState, questIds));
        }

        questExtra = new NpcQuestExtraRecord(initState, actions);
        return true;
    }

    private static bool TryParseModelInfos(SafeBinaryReader reader, out Dictionary<uint, NpcModelInfoRecord>? modelInfos)
    {
        modelInfos = null;
        if (!TryReadCount(reader, ModelInfoSize, out int count))
        {
            return false;
        }

        var records = new Dictionary<uint, NpcModelInfoRecord>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(ModelInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            var recordReader = new SafeBinaryReader(recordBytes);
            if (!recordReader.TryReadUInt32(out uint modelId))
            {
                return false;
            }

            var offsets = new List<short>(3);
            for (int offset = 0; offset < 3; offset++)
            {
                if (!recordReader.TryReadInt16(out short value))
                {
                    return false;
                }

                offsets.Add(value);
            }

            if (!recordReader.TryReadBytes(64 * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
                !recordReader.TrySkip(2) ||
                recordReader.Position != ModelInfoSize)
            {
                return false;
            }

            records.TryAdd(modelId, new NpcModelInfoRecord(
                modelId,
                offsets,
                recordReader.ReadFixedUtf16String(commentBytes.Span)));
        }

        modelInfos = records;
        return true;
    }

    private static bool TryParseNpcEvents(SafeBinaryReader reader, out Dictionary<uint, NpcEventInfoRecord>? events)
    {
        events = null;
        if (!TryReadCount(reader, EventInfoSize, out int count))
        {
            return false;
        }

        var records = new Dictionary<uint, NpcEventInfoRecord>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(EventInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseNpcEvent(recordBytes.Span, out NpcEventInfoRecord? npcEvent) ||
                npcEvent is null)
            {
                return false;
            }

            records.TryAdd(npcEvent.NpcId, npcEvent);
        }

        events = records;
        return true;
    }

    private static bool TryParseNpcEvent(ReadOnlySpan<byte> bytes, out NpcEventInfoRecord? npcEvent)
    {
        npcEvent = null;
        if (bytes.Length != EventInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint npcId) ||
            !reader.TryReadInt16(out short tryCount) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint exhaustMoney) ||
            !reader.TryReadUInt32(out uint exhaustItem) ||
            !reader.TryReadUInt32(out uint itemCount))
        {
            return false;
        }

        var items = new List<NpcEventItemRecord>(MaxCardItem);
        for (int i = 0; i < MaxCardItem; i++)
        {
            if (!reader.TryReadBytes(EventItemInfoSize, out ReadOnlyMemory<byte> itemBytes))
            {
                return false;
            }

            var itemReader = new SafeBinaryReader(itemBytes);
            if (!itemReader.TryReadUInt32(out uint itemId) ||
                !itemReader.TryReadInt16(out short count) ||
                !itemReader.TrySkip(2) ||
                itemReader.Position != EventItemInfoSize)
            {
                return false;
            }

            items.Add(new NpcEventItemRecord(itemId, count));
        }

        npcEvent = new NpcEventInfoRecord(npcId, tryCount, exhaustMoney, exhaustItem, itemCount, items);
        return reader.Position == EventInfoSize;
    }

    private static bool TryReadItemIds(SafeBinaryReader reader, out List<uint>? itemIds)
    {
        itemIds = null;
        if (!TryReadCount(reader, sizeof(uint), out int count))
        {
            return false;
        }

        var records = new List<uint>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint itemId))
            {
                return false;
            }

            records.Add(itemId);
        }

        itemIds = records;
        return true;
    }

    private static bool TryReadCount(SafeBinaryReader reader, int minimumRecordSize, out int count)
    {
        count = 0;
        if (!reader.TryReadInt32(out count) ||
            count < 0 ||
            count > MaxReasonableVariableCount)
        {
            return false;
        }

        return minimumRecordSize <= 0 ||
            (long)minimumRecordSize * count <= reader.Remaining;
    }

    private readonly record struct NpcInfoFields(
        uint NpcId,
        uint MapId,
        int Type,
        int MoveType,
        uint DisplayFlag,
        uint ModelId,
        string PenName,
        string Name,
        string Talk);
}
