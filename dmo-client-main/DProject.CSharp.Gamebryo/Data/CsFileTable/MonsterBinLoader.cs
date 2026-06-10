using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record MonsterBinData(
    IReadOnlyList<MonsterBinRecord> Monsters,
    IReadOnlyDictionary<int, int> MonsterHits,
    IReadOnlyList<MonsterSkillRecord> Skills,
    IReadOnlyList<MonsterSkillTermsRecord> SkillTerms);

public sealed record MonsterBinRecord(
    uint MonsterId,
    uint ModelId,
    string Name,
    string PenName,
    string Title,
    ushort Level,
    ushort Exp,
    ushort Battle,
    int HP,
    int DS,
    ushort DefPower,
    ushort Evasion,
    ushort MoveSpeed,
    ushort WalkSpeed,
    ushort CriticalRate,
    ushort AttPower,
    ushort AttSpeed,
    ushort AttRange,
    ushort HitRate,
    ushort Sight,
    ushort HuntRange,
    float Scale,
    ushort Class,
    IReadOnlyList<ushort> Icons,
    ushort ExpMin,
    ushort ExpMax);

public sealed record MonsterSkillRecord(
    ushort SkillIndex,
    uint MonsterId,
    uint CoolTime,
    ushort CastTime,
    ushort CastCheck,
    ushort TargetCount,
    ushort TargetMinCount,
    ushort TargetMaxCount,
    ushort UseTerms,
    ushort SkillType,
    uint EffectValueMin,
    uint EffectValueMax,
    ushort RangeIndex,
    uint SequenceId,
    ushort AnimationDelay,
    ushort Velocity,
    ushort Accel,
    IReadOnlyList<ushort> EffectFactors,
    IReadOnlyList<uint> EffectFactorValues,
    uint TalkId,
    uint ActiveType,
    float NoticeTime,
    string NoticeEffectName);

public sealed record MonsterSkillTermsRecord(
    ushort Index,
    ushort Direction,
    uint Range,
    ushort TargetingType,
    ushort RefCode);

public sealed class MonsterBinLoader : IBinLoader
{
    private const int MonsterNameLen = 64;
    private const int MonsterPenNameLen = 34;
    private const int MonsterIconNameLen = 64;
    private const int MaxFilename = 64;
    private const int MaxFactorCount = 3;
    private const int TCharSize = 2;

    private const int MonsterInfoSize = 396;
    private const int MonsterSkillInfoSize = 144;
    private const int MonsterSkillTermsInfoSize = 12;

    public string TableName => "CsMonsterMng";

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
        if (!TryParse(data, out MonsterBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out MonsterBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);

        if (!TryParseMonsters(reader, out List<MonsterBinRecord>? monsters) || monsters is null ||
            !TryParseMonsterHits(reader, out Dictionary<int, int>? monsterHits) || monsterHits is null ||
            !TryParseMonsterSkills(reader, out List<MonsterSkillRecord>? skills) || skills is null ||
            !TryParseMonsterSkillTerms(reader, out List<MonsterSkillTermsRecord>? skillTerms) || skillTerms is null)
        {
            return false;
        }

        parsed = new MonsterBinData(monsters, monsterHits, skills, skillTerms);
        return true;
    }

    private static bool TryParseMonsters(SafeBinaryReader reader, out List<MonsterBinRecord>? monsters)
    {
        monsters = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new List<MonsterBinRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(MonsterInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseMonster(recordBytes.Span, out MonsterBinRecord? monster) || monster is null)
            {
                return false;
            }

            if (seen.Add(monster.MonsterId))
            {
                records.Add(monster);
            }
        }

        monsters = records;
        return true;
    }

    private static bool TryParseMonster(ReadOnlySpan<byte> bytes, out MonsterBinRecord? monster)
    {
        monster = null;
        if (bytes.Length != MonsterInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint monsterId) ||
            !reader.TryReadUInt32(out uint modelId) ||
            !reader.TryReadBytes(MonsterNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(MonsterPenNameLen * TCharSize, out ReadOnlyMemory<byte> penNameBytes) ||
            !reader.TryReadBytes(MonsterIconNameLen * TCharSize, out ReadOnlyMemory<byte> titleBytes) ||
            !reader.TryReadUInt16(out ushort level) ||
            !reader.TryReadUInt16(out ushort exp) ||
            !reader.TryReadUInt16(out ushort battle) ||
            !reader.TrySkip(2) ||
            !reader.TryReadInt32(out int hp) ||
            !reader.TryReadInt32(out int ds) ||
            !reader.TryReadUInt16(out ushort defPower) ||
            !reader.TryReadUInt16(out ushort evasion) ||
            !reader.TryReadUInt16(out ushort moveSpeed) ||
            !reader.TryReadUInt16(out ushort walkSpeed) ||
            !reader.TryReadUInt16(out ushort criticalRate) ||
            !reader.TryReadUInt16(out ushort attPower) ||
            !reader.TryReadUInt16(out ushort attSpeed) ||
            !reader.TryReadUInt16(out ushort attRange) ||
            !reader.TryReadUInt16(out ushort hitRate) ||
            !reader.TryReadUInt16(out ushort sight) ||
            !reader.TryReadUInt16(out ushort huntRange) ||
            !reader.TrySkip(2) ||
            !reader.TryReadSingle(out float scale) ||
            !reader.TryReadUInt16(out ushort monsterClass))
        {
            return false;
        }

        var icons = new List<ushort>(6);
        for (int i = 0; i < 6; i++)
        {
            if (!reader.TryReadUInt16(out ushort icon))
            {
                return false;
            }

            icons.Add(icon);
        }

        if (!reader.TryReadUInt16(out ushort expMin) ||
            !reader.TryReadUInt16(out ushort expMax) ||
            !reader.TrySkip(2))
        {
            return false;
        }

        monster = new MonsterBinRecord(
            monsterId,
            modelId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedUtf16String(penNameBytes.Span),
            reader.ReadFixedUtf16String(titleBytes.Span),
            level,
            exp,
            battle,
            hp,
            ds,
            defPower,
            evasion,
            moveSpeed,
            walkSpeed,
            criticalRate,
            attPower,
            attSpeed,
            attRange,
            hitRate,
            sight,
            huntRange,
            scale,
            monsterClass,
            icons,
            expMin,
            expMax);
        return reader.Position == MonsterInfoSize;
    }

    private static bool TryParseMonsterHits(SafeBinaryReader reader, out Dictionary<int, int>? monsterHits)
    {
        monsterHits = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new Dictionary<int, int>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadInt32(out int level) ||
                !reader.TryReadInt32(out int hitRate))
            {
                return false;
            }

            records.TryAdd(level, hitRate);
        }

        monsterHits = records;
        return true;
    }

    private static bool TryParseMonsterSkills(SafeBinaryReader reader, out List<MonsterSkillRecord>? skills)
    {
        skills = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new List<MonsterSkillRecord>(count);
        var seen = new HashSet<ushort>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(MonsterSkillInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseMonsterSkill(recordBytes.Span, out MonsterSkillRecord? skill) || skill is null)
            {
                return false;
            }

            if (seen.Add(skill.SkillIndex))
            {
                records.Add(skill);
            }
        }

        skills = records;
        return true;
    }

    private static bool TryParseMonsterSkill(ReadOnlySpan<byte> bytes, out MonsterSkillRecord? skill)
    {
        skill = null;
        if (bytes.Length != MonsterSkillInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt16(out ushort skillIndex) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint monsterId) ||
            !reader.TryReadUInt32(out uint coolTime) ||
            !reader.TryReadUInt16(out ushort castTime) ||
            !reader.TryReadUInt16(out ushort castCheck) ||
            !reader.TryReadUInt16(out ushort targetCount) ||
            !reader.TryReadUInt16(out ushort targetMinCount) ||
            !reader.TryReadUInt16(out ushort targetMaxCount) ||
            !reader.TryReadUInt16(out ushort useTerms) ||
            !reader.TryReadUInt16(out ushort skillType) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint effectValueMin) ||
            !reader.TryReadUInt32(out uint effectValueMax) ||
            !reader.TryReadUInt16(out ushort rangeIndex) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint sequenceId) ||
            !reader.TryReadUInt16(out ushort animationDelay) ||
            !reader.TryReadUInt16(out ushort velocity) ||
            !reader.TryReadUInt16(out ushort accel))
        {
            return false;
        }

        var effectFactors = new List<ushort>(MaxFactorCount);
        for (int i = 0; i < MaxFactorCount; i++)
        {
            if (!reader.TryReadUInt16(out ushort effectFactor))
            {
                return false;
            }

            effectFactors.Add(effectFactor);
        }

        var effectFactorValues = new List<uint>(MaxFactorCount);
        for (int i = 0; i < MaxFactorCount; i++)
        {
            if (!reader.TryReadUInt32(out uint effectFactorValue))
            {
                return false;
            }

            effectFactorValues.Add(effectFactorValue);
        }

        if (!reader.TryReadUInt32(out uint talkId) ||
            !reader.TryReadUInt32(out uint activeType) ||
            !reader.TryReadSingle(out float noticeTime) ||
            !reader.TryReadBytes(MaxFilename, out ReadOnlyMemory<byte> noticeEffectBytes))
        {
            return false;
        }

        skill = new MonsterSkillRecord(
            skillIndex,
            monsterId,
            coolTime,
            castTime,
            castCheck,
            targetCount,
            targetMinCount,
            targetMaxCount,
            useTerms,
            skillType,
            effectValueMin,
            effectValueMax,
            rangeIndex,
            sequenceId,
            animationDelay,
            velocity,
            accel,
            effectFactors,
            effectFactorValues,
            talkId,
            activeType,
            noticeTime,
            reader.ReadFixedAnsiString(noticeEffectBytes.Span));
        return reader.Position == MonsterSkillInfoSize;
    }

    private static bool TryParseMonsterSkillTerms(SafeBinaryReader reader, out List<MonsterSkillTermsRecord>? skillTerms)
    {
        skillTerms = null;
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new List<MonsterSkillTermsRecord>(count);
        var seen = new HashSet<ushort>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(MonsterSkillTermsInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            var recordReader = new SafeBinaryReader(recordBytes);
            if (!recordReader.TryReadUInt16(out ushort index) ||
                !recordReader.TryReadUInt16(out ushort direction) ||
                !recordReader.TryReadUInt32(out uint range) ||
                !recordReader.TryReadUInt16(out ushort targetingType) ||
                !recordReader.TryReadUInt16(out ushort refCode))
            {
                return false;
            }

            if (seen.Add(index))
            {
                records.Add(new MonsterSkillTermsRecord(index, direction, range, targetingType, refCode));
            }
        }

        skillTerms = records;
        return true;
    }
}
