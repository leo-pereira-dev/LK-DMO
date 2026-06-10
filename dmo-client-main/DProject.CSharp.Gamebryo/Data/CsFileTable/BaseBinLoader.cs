using System.Text;
using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record BaseBinData(
    IReadOnlyList<BaseStatRecord> TamerBases,
    IReadOnlyList<BaseStatRecord> DigimonBases,
    IReadOnlyDictionary<uint, BaseMapInfoRecord> MapInfos,
    IReadOnlyDictionary<uint, IReadOnlyList<uint>> JumpBusters,
    BasePartyRecord Party,
    IReadOnlyDictionary<int, BaseGuildRecord> Guilds,
    BaseLimitRecord Limit,
    BasePersonStoreBaseRecord PersonStoreBase,
    IReadOnlyDictionary<int, BasePersonStoreObjectRecord> PersonStoreObjects,
    IReadOnlyDictionary<int, BasePlayPenaltyRecord> PlayPenalties,
    IReadOnlyDictionary<int, BaseEvolutionApplyRecord> EvolutionBaseInfos,
    IReadOnlyDictionary<int, BaseDigimonEvoMaxLevelRecord> DigimonEvoMaxLevels,
    IReadOnlyDictionary<int, BaseDskillOpenExpansionRecord> DskillOpenExpansions);

public sealed record BaseStatRecord(
    uint Id,
    ushort Level,
    long Exp,
    int Hp,
    int Ds,
    ushort MoveSpeed,
    ushort Defence,
    ushort Evasion,
    ushort Critical,
    ushort Attack,
    ushort HitRate);

public sealed record BaseMapInfoRecord(uint MapId, uint ShoutSec, bool EnableCheckMacro);

public sealed record BasePartyRecord(float PartyDist);

public sealed record BaseGuildRecord(
    int Level,
    uint Fame,
    int ItemNo1,
    int ItemCount1,
    int ItemNo2,
    int ItemCount2,
    int MasterLevel,
    int NeedPerson,
    int MaxGuildPerson,
    int IncMember,
    int MaxGuild2Master);

public sealed record BaseLimitRecord(
    short MaxTacticsHouse,
    short MaxWareHouse,
    short UnionStore,
    short MaxShareStash,
    uint ConsumeXg,
    uint ChargeXg);

public sealed record BasePersonStoreBaseRecord(
    float PersonCharge,
    float EmploymentCharge,
    float StoreDist);

public sealed record BasePersonStoreObjectRecord(
    int ItemId,
    int DigimonId,
    float Scale,
    int SlotCount,
    string FileName);

public sealed record BasePlayPenaltyRecord(
    int PenaltyLevel,
    int Exp,
    int Drop);

public sealed record BaseEvolutionApplyRecord(
    int EvolutionType,
    string EvolutionTypeName,
    int ApplyValue);

public sealed record BaseDigimonEvoMaxLevelRecord(
    int EvolutionType,
    int SkillExpStartLevel,
    IReadOnlyList<int> SkillMaxLevels);

public sealed record BaseDskillOpenExpansionRecord(
    int OpenItemSubType,
    int ExpansionRank,
    IReadOnlyList<int> DigimonEvoTypes);

public sealed class BaseBinLoader : IBinLoader
{
    private const int BaseInfoSize = 40;
    private const int MapInfoSize = 12;
    private const int GuildSize = 44;
    private const int LimitSizeReleaseEnglish = 16;
    private const int PersonStoreBaseSize = 12;
    private const int PersonStoreObjectSizeReleaseEnglish = 144;
    private const int PlayPenaltySize = 12;
    private const int MaxFilename = 64;
    private const int TCharSize = 2;
    private const int MaxReasonableCount = 100000;
    private const int MaxReasonableSubCount = 10000;
    private const int MaxCmUtilStringChars = 2048;

    public string TableName => "CsBaseMng";

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
        if (!TryParse(data, out BaseBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out BaseBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!TryReadBaseStats(reader, out List<BaseStatRecord>? tamerBases) ||
            tamerBases is null ||
            !TryReadBaseStats(reader, out List<BaseStatRecord>? digimonBases) ||
            digimonBases is null ||
            !TryReadMapInfos(reader, out Dictionary<uint, BaseMapInfoRecord>? mapInfos) ||
            mapInfos is null ||
            !TryReadJumpBusters(reader, out Dictionary<uint, IReadOnlyList<uint>>? jumpBusters) ||
            jumpBusters is null ||
            !TryReadParty(reader, out BasePartyRecord? party) ||
            party is null ||
            !TryReadGuilds(reader, out Dictionary<int, BaseGuildRecord>? guilds) ||
            guilds is null ||
            !TryReadLimit(reader, out BaseLimitRecord? limit) ||
            limit is null ||
            !TryReadPersonStore(reader, out BasePersonStoreBaseRecord? personStoreBase, out Dictionary<int, BasePersonStoreObjectRecord>? personStoreObjects) ||
            personStoreBase is null ||
            personStoreObjects is null ||
            !TryReadPlayPenalties(reader, out Dictionary<int, BasePlayPenaltyRecord>? playPenalties) ||
            playPenalties is null)
        {
            return false;
        }

        Dictionary<int, BaseEvolutionApplyRecord> evolutionBaseInfos = [];
        Dictionary<int, BaseDigimonEvoMaxLevelRecord> digimonEvoMaxLevels = [];
        Dictionary<int, BaseDskillOpenExpansionRecord> dskillOpenExpansions = [];

        if (!reader.EndOfData &&
            (!TryReadEvolutionBaseInfos(reader, evolutionBaseInfos) ||
             !TryReadDigimonEvoMaxLevels(reader, digimonEvoMaxLevels) ||
             !TryReadDskillOpenExpansions(reader, dskillOpenExpansions)))
        {
            return false;
        }

        parsed = new BaseBinData(
            tamerBases,
            digimonBases,
            mapInfos,
            jumpBusters,
            party,
            guilds,
            limit,
            personStoreBase,
            personStoreObjects,
            playPenalties,
            evolutionBaseInfos,
            digimonEvoMaxLevels,
            dskillOpenExpansions);
        return true;
    }

    private static bool TryReadBaseStats(SafeBinaryReader reader, out List<BaseStatRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * BaseInfoSize > reader.Remaining)
        {
            return false;
        }

        records = new List<BaseStatRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(BaseInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseBaseStat(recordBytes, out BaseStatRecord? record) ||
                record is null)
            {
                return false;
            }

            records.Add(record);
        }

        return true;
    }

    private static bool TryParseBaseStat(ReadOnlyMemory<byte> bytes, out BaseStatRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(bytes);
        if (!reader.TryReadUInt32(out uint id) ||
            !reader.TryReadUInt16(out ushort level) ||
            !reader.TrySkip(2) ||
            !reader.TryReadInt64(out long exp) ||
            !reader.TryReadInt32(out int hp) ||
            !reader.TryReadInt32(out int ds) ||
            !reader.TryReadUInt16(out ushort moveSpeed) ||
            !reader.TryReadUInt16(out ushort defence) ||
            !reader.TryReadUInt16(out ushort evasion) ||
            !reader.TryReadUInt16(out ushort critical) ||
            !reader.TryReadUInt16(out ushort attack) ||
            !reader.TryReadUInt16(out ushort hitRate) ||
            !reader.TrySkip(4))
        {
            return false;
        }

        record = new BaseStatRecord(id, level, exp, hp, ds, moveSpeed, defence, evasion, critical, attack, hitRate);
        return true;
    }

    private static bool TryReadMapInfos(SafeBinaryReader reader, out Dictionary<uint, BaseMapInfoRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * MapInfoSize > reader.Remaining)
        {
            return false;
        }

        records = new Dictionary<uint, BaseMapInfoRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(MapInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            var recordReader = new SafeBinaryReader(recordBytes);
            if (!recordReader.TryReadUInt32(out uint mapId) ||
                !recordReader.TryReadUInt32(out uint shoutSec) ||
                !recordReader.TryReadBoolean(out bool enableCheckMacro) ||
                !recordReader.TrySkip(3))
            {
                return false;
            }

            records.TryAdd(mapId, new BaseMapInfoRecord(mapId, shoutSec, enableCheckMacro));
        }

        return true;
    }

    private static bool TryReadJumpBusters(
        SafeBinaryReader reader,
        out Dictionary<uint, IReadOnlyList<uint>>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count))
        {
            return false;
        }

        records = new Dictionary<uint, IReadOnlyList<uint>>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint itemId) ||
                !TryReadCount(reader, MaxReasonableSubCount, out int destinationCount) ||
                (long)destinationCount * sizeof(uint) > reader.Remaining)
            {
                return false;
            }

            var destinations = new List<uint>(destinationCount);
            for (int destinationIndex = 0; destinationIndex < destinationCount; destinationIndex++)
            {
                if (!reader.TryReadUInt32(out uint mapId))
                {
                    return false;
                }

                if (!destinations.Contains(mapId))
                {
                    destinations.Add(mapId);
                }
            }

            records.TryAdd(itemId, destinations);
        }

        return true;
    }

    private static bool TryReadParty(SafeBinaryReader reader, out BasePartyRecord? party)
    {
        party = null;
        if (!reader.TryReadSingle(out float partyDist))
        {
            return false;
        }

        party = new BasePartyRecord(partyDist);
        return true;
    }

    private static bool TryReadGuilds(SafeBinaryReader reader, out Dictionary<int, BaseGuildRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * GuildSize > reader.Remaining)
        {
            return false;
        }

        records = new Dictionary<int, BaseGuildRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int level) ||
                !reader.TryReadUInt32(out uint fame) ||
                !reader.TryReadInt32(out int itemNo1) ||
                !reader.TryReadInt32(out int itemCount1) ||
                !reader.TryReadInt32(out int itemNo2) ||
                !reader.TryReadInt32(out int itemCount2) ||
                !reader.TryReadInt32(out int masterLevel) ||
                !reader.TryReadInt32(out int needPerson) ||
                !reader.TryReadInt32(out int maxGuildPerson) ||
                !reader.TryReadInt32(out int incMember) ||
                !reader.TryReadInt32(out int maxGuild2Master))
            {
                return false;
            }

            records.TryAdd(level, new BaseGuildRecord(
                level,
                fame,
                itemNo1,
                itemCount1,
                itemNo2,
                itemCount2,
                masterLevel,
                needPerson,
                maxGuildPerson,
                incMember,
                maxGuild2Master));
        }

        return true;
    }

    private static bool TryReadLimit(SafeBinaryReader reader, out BaseLimitRecord? limit)
    {
        limit = null;
        if (reader.Remaining < LimitSizeReleaseEnglish ||
            !reader.TryReadInt16(out short maxTacticsHouse) ||
            !reader.TryReadInt16(out short maxWareHouse) ||
            !reader.TryReadInt16(out short unionStore) ||
            !reader.TryReadInt16(out short maxShareStash) ||
            !reader.TryReadUInt32(out uint consumeXg) ||
            !reader.TryReadUInt32(out uint chargeXg))
        {
            return false;
        }

        limit = new BaseLimitRecord(maxTacticsHouse, maxWareHouse, unionStore, maxShareStash, consumeXg, chargeXg);
        return true;
    }

    private static bool TryReadPersonStore(
        SafeBinaryReader reader,
        out BasePersonStoreBaseRecord? baseInfo,
        out Dictionary<int, BasePersonStoreObjectRecord>? objects)
    {
        baseInfo = null;
        objects = null;
        if (reader.Remaining < PersonStoreBaseSize ||
            !reader.TryReadSingle(out float personCharge) ||
            !reader.TryReadSingle(out float employmentCharge) ||
            !reader.TryReadSingle(out float storeDist) ||
            !TryReadCount(reader, MaxReasonableCount, out int objectCount) ||
            (long)objectCount * PersonStoreObjectSizeReleaseEnglish > reader.Remaining)
        {
            return false;
        }

        baseInfo = new BasePersonStoreBaseRecord(personCharge, employmentCharge, storeDist);
        objects = new Dictionary<int, BasePersonStoreObjectRecord>(objectCount);
        for (int i = 0; i < objectCount; i++)
        {
            if (!reader.TryReadBytes(PersonStoreObjectSizeReleaseEnglish, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            var recordReader = new SafeBinaryReader(recordBytes);
            if (!recordReader.TryReadInt32(out int itemId) ||
                !recordReader.TryReadInt32(out int digimonId) ||
                !recordReader.TryReadSingle(out float scale) ||
                !recordReader.TryReadInt32(out int slotCount) ||
                !recordReader.TryReadBytes(MaxFilename * TCharSize, out ReadOnlyMemory<byte> fileNameBytes))
            {
                return false;
            }

            string fileName = recordReader.ReadFixedUtf16String(fileNameBytes.Span);
            objects.TryAdd(itemId, new BasePersonStoreObjectRecord(itemId, digimonId, scale, slotCount, fileName));
        }

        return true;
    }

    private static bool TryReadPlayPenalties(SafeBinaryReader reader, out Dictionary<int, BasePlayPenaltyRecord>? records)
    {
        records = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * PlayPenaltySize > reader.Remaining)
        {
            return false;
        }

        records = new Dictionary<int, BasePlayPenaltyRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int penaltyLevel) ||
                !reader.TryReadInt32(out int exp) ||
                !reader.TryReadInt32(out int drop))
            {
                return false;
            }

            records.TryAdd(penaltyLevel, new BasePlayPenaltyRecord(penaltyLevel, exp, drop));
        }

        return true;
    }

    private static bool TryReadEvolutionBaseInfos(
        SafeBinaryReader reader,
        Dictionary<int, BaseEvolutionApplyRecord> records)
    {
        if (!TryReadCount(reader, MaxReasonableCount, out int count))
        {
            return false;
        }

        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int evolutionType))
            {
                return false;
            }

            if (evolutionType == 0)
            {
                break;
            }

            if (!TryReadCmUtilUtf16String(reader, out string name) ||
                !reader.TryReadInt32(out int applyValue))
            {
                return false;
            }

            records.TryAdd(evolutionType, new BaseEvolutionApplyRecord(evolutionType, name, applyValue));
        }

        return true;
    }

    private static bool TryReadDigimonEvoMaxLevels(
        SafeBinaryReader reader,
        Dictionary<int, BaseDigimonEvoMaxLevelRecord> records)
    {
        if (!TryReadCount(reader, MaxReasonableCount, out int count))
        {
            return false;
        }

        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int evolutionType))
            {
                return false;
            }

            if (evolutionType == 0)
            {
                break;
            }

            if (!reader.TryReadInt32(out int skillExpStartLevel) ||
                !TryReadIntList(reader, out List<int>? skillMaxLevels) ||
                skillMaxLevels is null)
            {
                return false;
            }

            records.TryAdd(evolutionType, new BaseDigimonEvoMaxLevelRecord(evolutionType, skillExpStartLevel, skillMaxLevels));
        }

        return true;
    }

    private static bool TryReadDskillOpenExpansions(
        SafeBinaryReader reader,
        Dictionary<int, BaseDskillOpenExpansionRecord> records)
    {
        if (!TryReadCount(reader, MaxReasonableCount, out int count))
        {
            return false;
        }

        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int openItemSubType))
            {
                return false;
            }

            if (openItemSubType == 0)
            {
                break;
            }

            if (!reader.TryReadInt32(out int expansionRank) ||
                !TryReadIntList(reader, out List<int>? digimonEvoTypes) ||
                digimonEvoTypes is null)
            {
                return false;
            }

            records.TryAdd(openItemSubType, new BaseDskillOpenExpansionRecord(openItemSubType, expansionRank, digimonEvoTypes));
        }

        return true;
    }

    private static bool TryReadIntList(SafeBinaryReader reader, out List<int>? values)
    {
        values = null;
        if (!TryReadCount(reader, MaxReasonableSubCount, out int count) ||
            (long)count * sizeof(int) > reader.Remaining)
        {
            return false;
        }

        values = new List<int>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int value))
            {
                return false;
            }

            if (value != 0)
            {
                values.Add(value);
            }
        }

        return true;
    }

    private static bool TryReadCmUtilUtf16String(SafeBinaryReader reader, out string value)
    {
        value = string.Empty;
        if (!reader.TryReadInt32(out int charCount) ||
            charCount < 0 ||
            charCount > MaxCmUtilStringChars ||
            !reader.TryReadBytes(charCount * TCharSize, out ReadOnlyMemory<byte> bytes))
        {
            return false;
        }

        value = Encoding.Unicode.GetString(bytes.Span);
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
