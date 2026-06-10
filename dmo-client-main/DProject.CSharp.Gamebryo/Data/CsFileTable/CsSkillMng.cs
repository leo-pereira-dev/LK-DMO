namespace DProject.Data.CsFileTable;

public sealed class CsSkillMng : IBinFileTableManager
{
    private readonly SkillBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, SkillBinRecord> _skills = new();
    private readonly Dictionary<ushort, TamerSkillBinRecord> _tamerSkills = new();
    private readonly Dictionary<uint, SkillAreaCheckRecord> _areaChecks = new();

    public CsSkillMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsSkillMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, SkillBinRecord> Skills => _skills;
    public IReadOnlyDictionary<ushort, TamerSkillBinRecord> TamerSkills => _tamerSkills;
    public IReadOnlyDictionary<uint, SkillAreaCheckRecord> AreaChecks => _areaChecks;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is SkillBinData data)
        {
            foreach (SkillBinRecord skill in data.Skills)
            {
                _skills.TryAdd(skill.SkillId, skill);
            }

            foreach (TamerSkillBinRecord tamerSkill in data.TamerSkills)
            {
                _tamerSkills.TryAdd(tamerSkill.Index, tamerSkill);
            }

            foreach (SkillAreaCheckRecord areaCheck in data.AreaChecks)
            {
                _areaChecks.TryAdd(areaCheck.Index, areaCheck);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _skills.Clear();
        _tamerSkills.Clear();
        _areaChecks.Clear();
        IsLoaded = false;
    }

    public bool IsSkill(uint skillId)
    {
        return _skills.ContainsKey(skillId);
    }

    public SkillBinRecord? GetSkill(uint skillId)
    {
        return _skills.TryGetValue(skillId, out SkillBinRecord? skill) ? skill : null;
    }

    public TamerSkillBinRecord? GetTamer_Skill(int index, int skillCode)
    {
        if (index < ushort.MinValue || index > ushort.MaxValue || skillCode < 0)
        {
            return null;
        }

        if (!_tamerSkills.TryGetValue((ushort)index, out TamerSkillBinRecord? tamerSkill))
        {
            return null;
        }

        return tamerSkill.SkillCode == (uint)skillCode ? tamerSkill : null;
    }

    public bool IsTamer_Skill(uint skillCode)
    {
        return _tamerSkills.Values.Any(tamerSkill => tamerSkill.SkillCode == skillCode);
    }

    public SkillAreaCheckRecord? GetArea_Check(uint index)
    {
        return _areaChecks.TryGetValue(index, out SkillAreaCheckRecord? areaCheck) ? areaCheck : null;
    }

    public bool FindArea_Check(uint index, ushort mapId)
    {
        return GetArea_Check(index)?.Areas.Contains(mapId) == true;
    }
}
