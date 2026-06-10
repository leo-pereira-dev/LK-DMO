using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record SkillBinData(
    IReadOnlyList<SkillBinRecord> Skills,
    IReadOnlyList<TamerSkillBinRecord> TamerSkills,
    IReadOnlyList<SkillAreaCheckRecord> AreaChecks);

public sealed record SkillApplyRecord(
    int A,
    int InvokeRate,
    int B,
    int C,
    ushort BuffCode,
    ushort Id,
    short IncreaseBPoint);

public sealed record SkillBinRecord(
    uint SkillId,
    string Name,
    string Comment,
    IReadOnlyList<SkillApplyRecord> Applies,
    ushort LevelupPoint,
    ushort MaxLevel,
    ushort AttributeType,
    ushort NatureType,
    ushort FamilyType,
    ushort UseHP,
    ushort UseDS,
    ushort Icon,
    ushort Target,
    ushort AttType,
    float AttRange,
    float AttRangeMinDmg,
    float AttRangeNorDmg,
    float AttRangeMaxDmg,
    ushort AttSphere,
    float CastingTime,
    float DamageTime,
    ushort DamageDay,
    float DistanceTime,
    float CooldownTime,
    ushort CooldownDay,
    float SkillVelocity,
    float SkillAccel,
    ushort SkillType,
    ushort LimitLevel,
    ushort SkillGroup,
    ushort SkillRank,
    ushort MemorySkill,
    ushort ReqItem);

public sealed record TamerSkillBinRecord(
    ushort Index,
    uint SkillCode,
    ushort Type,
    uint Factor1,
    uint Factor2,
    uint TamerSeqId,
    uint DigimonSeqId,
    ushort UseState,
    ushort UseAreaCheck,
    ushort Available);

public sealed record SkillAreaCheckRecord(
    uint Index,
    IReadOnlyList<ushort> Areas);

public sealed class SkillBinLoader : IBinLoader
{
    private const int SkillNameLen = 32;
    private const int SkillCommentLen = 256;
    private const int SkillApplyMaxCount = 3;
    private const int AreaCheckCount = 30;

    private const int TCharSize = 2;
    private const int SkillApplySize = 24;
    private const int SkillInfoSize = 736;
    private const int TamerSkillInfoSize = 36;
    private const int AreaCheckInfoSize = 64;

    public string TableName => "CsSkillMng";

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
        if (!TryParse(data, out SkillBinData? parsed))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out SkillBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);

        if (!reader.TryReadInt32(out int skillCount) || skillCount < 0)
        {
            return false;
        }

        var skills = new List<SkillBinRecord>(skillCount);
        var seenSkills = new HashSet<uint>();
        for (int i = 0; i < skillCount; i++)
        {
            if (!reader.TryReadBytes(SkillInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseSkill(recordBytes.Span, out SkillBinRecord? skill) || skill is null)
            {
                return false;
            }

            if (seenSkills.Add(skill.SkillId))
            {
                skills.Add(skill);
            }
        }

        if (!reader.TryReadInt32(out int tamerSkillCount) || tamerSkillCount < 0)
        {
            return false;
        }

        var tamerSkills = new List<TamerSkillBinRecord>(tamerSkillCount);
        var seenTamerSkills = new HashSet<ushort>();
        for (int i = 0; i < tamerSkillCount; i++)
        {
            if (!reader.TryReadBytes(TamerSkillInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseTamerSkill(recordBytes.Span, out TamerSkillBinRecord? tamerSkill) || tamerSkill is null)
            {
                return false;
            }

            if (seenTamerSkills.Add(tamerSkill.Index))
            {
                tamerSkills.Add(tamerSkill);
            }
        }

        if (!reader.TryReadInt32(out int areaCheckCount) || areaCheckCount < 0)
        {
            return false;
        }

        var areaChecks = new List<SkillAreaCheckRecord>(areaCheckCount);
        var seenAreas = new HashSet<uint>();
        for (int i = 0; i < areaCheckCount; i++)
        {
            if (!reader.TryReadBytes(AreaCheckInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseAreaCheck(recordBytes.Span, out SkillAreaCheckRecord? areaCheck) || areaCheck is null)
            {
                return false;
            }

            if (seenAreas.Add(areaCheck.Index))
            {
                areaChecks.Add(areaCheck);
            }
        }

        parsed = new SkillBinData(skills, tamerSkills, areaChecks);
        return true;
    }

    private static bool TryParseSkill(ReadOnlySpan<byte> bytes, out SkillBinRecord? skill)
    {
        skill = null;
        if (bytes.Length != SkillInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint skillId) ||
            !reader.TryReadBytes(SkillNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(SkillCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes))
        {
            return false;
        }

        var applies = new List<SkillApplyRecord>(SkillApplyMaxCount);
        for (int i = 0; i < SkillApplyMaxCount; i++)
        {
            if (!TryParseApply(reader, out SkillApplyRecord? apply) || apply is null)
            {
                return false;
            }

            applies.Add(apply);
        }

        if (!reader.TryReadUInt16(out ushort levelupPoint) ||
            !reader.TryReadUInt16(out ushort maxLevel) ||
            !reader.TryReadUInt16(out ushort attributeType) ||
            !reader.TryReadUInt16(out ushort natureType) ||
            !reader.TryReadUInt16(out ushort familyType) ||
            !reader.TryReadUInt16(out ushort useHP) ||
            !reader.TryReadUInt16(out ushort useDS) ||
            !reader.TryReadUInt16(out ushort icon) ||
            !reader.TryReadUInt16(out ushort target) ||
            !reader.TryReadUInt16(out ushort attType) ||
            !reader.TryReadSingle(out float attRange) ||
            !reader.TryReadSingle(out float attRangeMinDmg) ||
            !reader.TryReadSingle(out float attRangeNorDmg) ||
            !reader.TryReadSingle(out float attRangeMaxDmg) ||
            !reader.TryReadUInt16(out ushort attSphere) ||
            !reader.TrySkip(2) ||
            !reader.TryReadSingle(out float castingTime) ||
            !reader.TryReadSingle(out float damageTime) ||
            !reader.TryReadUInt16(out ushort damageDay) ||
            !reader.TrySkip(2) ||
            !reader.TryReadSingle(out float distanceTime) ||
            !reader.TryReadSingle(out float cooldownTime) ||
            !reader.TryReadUInt16(out ushort cooldownDay) ||
            !reader.TrySkip(2) ||
            !reader.TryReadSingle(out float skillVelocity) ||
            !reader.TryReadSingle(out float skillAccel) ||
            !reader.TryReadUInt16(out ushort skillType) ||
            !reader.TryReadUInt16(out ushort limitLevel) ||
            !reader.TryReadUInt16(out ushort skillGroup) ||
            !reader.TryReadUInt16(out ushort skillRank) ||
            !reader.TryReadUInt16(out ushort memorySkill) ||
            !reader.TryReadUInt16(out ushort reqItem))
        {
            return false;
        }

        skill = new SkillBinRecord(
            skillId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedUtf16String(commentBytes.Span),
            applies,
            levelupPoint,
            maxLevel,
            attributeType,
            natureType,
            familyType,
            useHP,
            useDS,
            icon,
            target,
            attType,
            attRange,
            attRangeMinDmg,
            attRangeNorDmg,
            attRangeMaxDmg,
            attSphere,
            castingTime,
            damageTime,
            damageDay,
            distanceTime,
            cooldownTime,
            cooldownDay,
            skillVelocity,
            skillAccel,
            skillType,
            limitLevel,
            skillGroup,
            skillRank,
            memorySkill,
            reqItem);
        return reader.Position == SkillInfoSize;
    }

    private static bool TryParseApply(SafeBinaryReader reader, out SkillApplyRecord? apply)
    {
        apply = null;
        int startPosition = reader.Position;
        if (!reader.TryReadInt32(out int a) ||
            !reader.TryReadInt32(out int invokeRate) ||
            !reader.TryReadInt32(out int b) ||
            !reader.TryReadInt32(out int c) ||
            !reader.TryReadUInt16(out ushort buffCode) ||
            !reader.TryReadUInt16(out ushort id) ||
            !reader.TryReadInt16(out short increaseBPoint) ||
            !reader.TrySkip(SkillApplySize - (reader.Position - startPosition)))
        {
            return false;
        }

        apply = new SkillApplyRecord(a, invokeRate, b, c, buffCode, id, increaseBPoint);
        return true;
    }

    private static bool TryParseTamerSkill(ReadOnlySpan<byte> bytes, out TamerSkillBinRecord? tamerSkill)
    {
        tamerSkill = null;
        if (bytes.Length != TamerSkillInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt16(out ushort index) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint skillCode) ||
            !reader.TryReadUInt16(out ushort type) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint factor1) ||
            !reader.TryReadUInt32(out uint factor2) ||
            !reader.TryReadUInt32(out uint tamerSeqId) ||
            !reader.TryReadUInt32(out uint digimonSeqId) ||
            !reader.TryReadUInt16(out ushort useState) ||
            !reader.TryReadUInt16(out ushort useAreaCheck) ||
            !reader.TryReadUInt16(out ushort available) ||
            !reader.TrySkip(2))
        {
            return false;
        }

        tamerSkill = new TamerSkillBinRecord(
            index,
            skillCode,
            type,
            factor1,
            factor2,
            tamerSeqId,
            digimonSeqId,
            useState,
            useAreaCheck,
            available);
        return reader.Position == TamerSkillInfoSize;
    }

    private static bool TryParseAreaCheck(ReadOnlySpan<byte> bytes, out SkillAreaCheckRecord? areaCheck)
    {
        areaCheck = null;
        if (bytes.Length != AreaCheckInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint index))
        {
            return false;
        }

        var areas = new List<ushort>(AreaCheckCount);
        for (int i = 0; i < AreaCheckCount; i++)
        {
            if (!reader.TryReadUInt16(out ushort area))
            {
                return false;
            }

            areas.Add(area);
        }

        areaCheck = new SkillAreaCheckRecord(index, areas);
        return reader.Position == AreaCheckInfoSize;
    }
}
