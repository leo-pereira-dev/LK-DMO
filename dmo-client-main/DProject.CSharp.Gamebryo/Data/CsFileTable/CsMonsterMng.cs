namespace DProject.Data.CsFileTable;

public sealed class CsMonsterMng : IBinFileTableManager
{
    private const ushort ScaleIncreaseFactor = 41;

    private readonly MonsterBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, MonsterBinRecord> _monsters = new();
    private readonly Dictionary<int, int> _monsterHits = new();
    private readonly Dictionary<ushort, MonsterSkillRecord> _skills = new();
    private readonly Dictionary<ushort, MonsterSkillTermsRecord> _skillTerms = new();

    public CsMonsterMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMonsterMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, MonsterBinRecord> Monsters => _monsters;
    public IReadOnlyDictionary<int, int> MonsterHits => _monsterHits;
    public IReadOnlyDictionary<ushort, MonsterSkillRecord> Skills => _skills;
    public IReadOnlyDictionary<ushort, MonsterSkillTermsRecord> SkillTerms => _skillTerms;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MonsterBinData data)
        {
            foreach (MonsterBinRecord monster in data.Monsters)
            {
                _monsters.TryAdd(monster.MonsterId, monster);
            }

            foreach (KeyValuePair<int, int> monsterHit in data.MonsterHits)
            {
                _monsterHits.TryAdd(monsterHit.Key, monsterHit.Value);
            }

            foreach (MonsterSkillRecord skill in data.Skills)
            {
                _skills.TryAdd(skill.SkillIndex, skill);
            }

            foreach (MonsterSkillTermsRecord terms in data.SkillTerms)
            {
                _skillTerms.TryAdd(terms.Index, terms);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _monsters.Clear();
        _monsterHits.Clear();
        _skills.Clear();
        _skillTerms.Clear();
        IsLoaded = false;
    }

    public MonsterBinRecord? GetMonster(uint monsterTableId)
    {
        return _monsters.TryGetValue(monsterTableId, out MonsterBinRecord? monster) ? monster : null;
    }

    public bool IsMonster(uint monsterId)
    {
        return _monsters.ContainsKey(monsterId);
    }

    public int GetMonsterHit(int level)
    {
        return _monsterHits.TryGetValue(level, out int hit) ? hit : 0;
    }

    public MonsterSkillRecord? GetMonsterSkill(ushort skillIndex)
    {
        return _skills.TryGetValue(skillIndex, out MonsterSkillRecord? skill) ? skill : null;
    }

    public MonsterSkillTermsRecord? GetMonsterSkillTerms(ushort index)
    {
        return index == 0
            ? null
            : _skillTerms.TryGetValue(index, out MonsterSkillTermsRecord? terms) ? terms : null;
    }

    public float GetEffectVal_Scale(int index)
    {
        if (index < ushort.MinValue || index > ushort.MaxValue)
        {
            return 1.0f;
        }

        MonsterSkillRecord? skill = GetMonsterSkill((ushort)index);
        if (skill is null)
        {
            return 1.0f;
        }

        for (int i = 0; i < skill.EffectFactors.Count && i < skill.EffectFactorValues.Count; i++)
        {
            if (skill.EffectFactors[i] == ScaleIncreaseFactor)
            {
                return skill.EffectFactorValues[i] * 0.01f;
            }
        }

        return 1.0f;
    }

    public int GetBuffStack(int index, int currentValue)
    {
        if (index < ushort.MinValue || index > ushort.MaxValue)
        {
            return 0;
        }

        MonsterSkillRecord? skill = GetMonsterSkill((ushort)index);
        if (skill is null ||
            skill.EffectFactorValues.Count == 0 ||
            skill.EffectFactorValues[0] == 0 ||
            skill.EffectFactorValues[0] > int.MaxValue)
        {
            return 0;
        }

        return currentValue / (int)skill.EffectFactorValues[0];
    }
}
