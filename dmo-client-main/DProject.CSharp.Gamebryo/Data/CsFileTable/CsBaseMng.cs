namespace DProject.Data.CsFileTable;

public sealed class CsBaseMng : IBinFileTableManager
{
    public const int BaseCharTypeConstant = 1000;
    public const int BtTamerExp = 1;
    public const int BtDigimonExp = 1;

    private const uint DatsCenterMapId = 3;
    private const uint DTerminalMapId = 2;
    private const int AllAreaMinDestinations = 20;

    private readonly BaseBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, BaseStatRecord> _tamerBases = new();
    private readonly Dictionary<uint, BaseStatRecord> _digimonBases = new();
    private readonly Dictionary<uint, BaseMapInfoRecord> _mapInfos = new();
    private readonly Dictionary<uint, List<uint>> _jumpBusters = new();
    private readonly Dictionary<int, BaseGuildRecord> _guilds = new();
    private readonly Dictionary<int, BasePersonStoreObjectRecord> _personStoreObjects = new();
    private readonly Dictionary<int, BasePlayPenaltyRecord> _playPenalties = new();
    private readonly Dictionary<int, BaseEvolutionApplyRecord> _evolutionBaseInfos = new();
    private readonly Dictionary<int, BaseDigimonEvoMaxLevelRecord> _digimonEvoMaxLevels = new();
    private readonly Dictionary<int, BaseDskillOpenExpansionRecord> _dskillOpenExpansions = new();

    public CsBaseMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsBaseMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, BaseStatRecord> TamerBases => _tamerBases;
    public IReadOnlyDictionary<uint, BaseStatRecord> DigimonBases => _digimonBases;
    public IReadOnlyDictionary<uint, BaseMapInfoRecord> MapInfos => _mapInfos;
    public IReadOnlyDictionary<uint, List<uint>> JumpBusters => _jumpBusters;
    public IReadOnlyDictionary<int, BaseGuildRecord> Guilds => _guilds;
    public BasePartyRecord Party { get; private set; } = new(0);
    public BaseLimitRecord Limit { get; private set; } = new(0, 0, 0, 0, 0, 0);
    public BasePersonStoreBaseRecord PersonStoreBase { get; private set; } = new(0, 0, 0);
    public IReadOnlyDictionary<int, BasePersonStoreObjectRecord> PersonStoreObjects => _personStoreObjects;
    public IReadOnlyDictionary<int, BasePlayPenaltyRecord> PlayPenalties => _playPenalties;
    public IReadOnlyDictionary<int, BaseEvolutionApplyRecord> BaseEvolutionInfos => _evolutionBaseInfos;
    public IReadOnlyDictionary<int, BaseDigimonEvoMaxLevelRecord> DigimonEvoMaxLevels => _digimonEvoMaxLevels;
    public IReadOnlyDictionary<int, BaseDskillOpenExpansionRecord> DskillOpenExpansions => _dskillOpenExpansions;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is BaseBinData data)
        {
            foreach (BaseStatRecord tamerBase in data.TamerBases)
            {
                _tamerBases.TryAdd(tamerBase.Id, tamerBase);
            }

            foreach (BaseStatRecord digimonBase in data.DigimonBases)
            {
                _digimonBases.TryAdd(digimonBase.Id, digimonBase);
            }

            foreach (KeyValuePair<uint, BaseMapInfoRecord> mapInfo in data.MapInfos)
            {
                _mapInfos.TryAdd(mapInfo.Key, mapInfo.Value);
            }

            foreach (KeyValuePair<uint, IReadOnlyList<uint>> jumpBuster in data.JumpBusters)
            {
                _jumpBusters[jumpBuster.Key] = NormalizeJumpBusterDestinations(jumpBuster.Value);
            }

            Party = data.Party;
            foreach (KeyValuePair<int, BaseGuildRecord> guild in data.Guilds)
            {
                _guilds.TryAdd(guild.Key, guild.Value);
            }

            Limit = data.Limit;
            PersonStoreBase = data.PersonStoreBase;
            foreach (KeyValuePair<int, BasePersonStoreObjectRecord> personStoreObject in data.PersonStoreObjects)
            {
                _personStoreObjects.TryAdd(personStoreObject.Key, personStoreObject.Value);
            }

            foreach (KeyValuePair<int, BasePlayPenaltyRecord> playPenalty in data.PlayPenalties)
            {
                _playPenalties.TryAdd(playPenalty.Key, playPenalty.Value);
            }

            foreach (KeyValuePair<int, BaseEvolutionApplyRecord> evolutionInfo in data.EvolutionBaseInfos)
            {
                _evolutionBaseInfos.TryAdd(evolutionInfo.Key, evolutionInfo.Value);
            }

            foreach (KeyValuePair<int, BaseDigimonEvoMaxLevelRecord> maxLevel in data.DigimonEvoMaxLevels)
            {
                _digimonEvoMaxLevels.TryAdd(maxLevel.Key, maxLevel.Value);
            }

            foreach (KeyValuePair<int, BaseDskillOpenExpansionRecord> expansion in data.DskillOpenExpansions)
            {
                _dskillOpenExpansions.TryAdd(expansion.Key, expansion.Value);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _tamerBases.Clear();
        _digimonBases.Clear();
        _mapInfos.Clear();
        _jumpBusters.Clear();
        _guilds.Clear();
        _personStoreObjects.Clear();
        _playPenalties.Clear();
        _evolutionBaseInfos.Clear();
        _digimonEvoMaxLevels.Clear();
        _dskillOpenExpansions.Clear();
        Party = new BasePartyRecord(0);
        Limit = new BaseLimitRecord(0, 0, 0, 0, 0, 0);
        PersonStoreBase = new BasePersonStoreBaseRecord(0, 0, 0);
        IsLoaded = false;
    }

    public bool IsTamerBase(uint id) => _tamerBases.ContainsKey(id);

    public BaseStatRecord? GetTamerBase(int level, int tamerType)
    {
        return GetBaseWithLevelFallback(_tamerBases, level, tamerType);
    }

    public bool IsDigimonBase(uint id) => _digimonBases.ContainsKey(id);

    public BaseStatRecord? GetDigimonBase(int level, int digimonType)
    {
        return GetBaseWithLevelFallback(_digimonBases, level, digimonType);
    }

    public BaseMapInfoRecord? GetMapInfo(uint mapId)
    {
        return _mapInfos.TryGetValue(mapId, out BaseMapInfoRecord? mapInfo) ? mapInfo : null;
    }

    public uint GetShoutSec(uint mapId)
    {
        return GetMapInfo(mapId)?.ShoutSec ?? 0;
    }

    public bool IsEnableCheckMacro(uint mapId)
    {
        return GetMapInfo(mapId)?.EnableCheckMacro == true;
    }

    public bool IsUseJumpBuster(int jumpBusterItemId, int destinationMapId)
    {
        if (jumpBusterItemId < 0 || destinationMapId < 0)
        {
            return false;
        }

        return _jumpBusters.TryGetValue((uint)jumpBusterItemId, out List<uint>? destinations) &&
            destinations.Contains((uint)destinationMapId);
    }

    public IReadOnlyList<uint> GetJumpBusterItemID(int destinationMapId)
    {
        if (destinationMapId < 0)
        {
            return [];
        }

        uint mapId = (uint)destinationMapId;
        var items = new List<uint>();
        foreach (KeyValuePair<uint, List<uint>> jumpBuster in _jumpBusters)
        {
            if (jumpBuster.Value.Contains(mapId))
            {
                items.Add(jumpBuster.Key);
            }
        }

        return items;
    }

    public BaseGuildRecord? GetGuildInfo(uint level)
    {
        return level <= int.MaxValue && _guilds.TryGetValue((int)level, out BaseGuildRecord? guild)
            ? guild
            : null;
    }

    public BasePlayPenaltyRecord? GetPlayPanelty(int level)
    {
        return _playPenalties.TryGetValue(level, out BasePlayPenaltyRecord? penalty) ? penalty : null;
    }

    public string? IsBallonNewType(int targetId)
    {
        foreach (BasePersonStoreObjectRecord personStoreObject in _personStoreObjects.Values)
        {
            if (personStoreObject.DigimonId == targetId &&
                !string.Equals(personStoreObject.FileName, "Base", StringComparison.OrdinalIgnoreCase))
            {
                return personStoreObject.FileName;
            }
        }

        return null;
    }

    public int GetEvoApplyValue(int evolutionType)
    {
        return _evolutionBaseInfos.TryGetValue(evolutionType, out BaseEvolutionApplyRecord? info)
            ? info.ApplyValue
            : 0;
    }

    public string GetEvoTypeName(int evolutionType)
    {
        return _evolutionBaseInfos.TryGetValue(evolutionType, out BaseEvolutionApplyRecord? info)
            ? info.EvolutionTypeName
            : "Unknown";
    }

    public bool IsEnableSkillMaxLvUpFromEvoType(int evolutionType, int useItemSubType)
    {
        return _dskillOpenExpansions.TryGetValue(useItemSubType, out BaseDskillOpenExpansionRecord? expansion) &&
            expansion.DigimonEvoTypes.Contains(evolutionType);
    }

    public int GetOpenSkillRankFromItemSubType(int useItemSubType)
    {
        return _dskillOpenExpansions.TryGetValue(useItemSubType, out BaseDskillOpenExpansionRecord? expansion)
            ? expansion.ExpansionRank
            : 0;
    }

    public int GetBeforOpenSkillRankeFromItemSubType(int useItemSubType)
    {
        return GetOpenSkillRankFromItemSubType(useItemSubType) - 1;
    }

    public int GetSkillRankFromSkillMaxLv(int evolutionType, int skillMaxLevel)
    {
        if (!_digimonEvoMaxLevels.TryGetValue(evolutionType, out BaseDigimonEvoMaxLevelRecord? maxLevel))
        {
            return 0;
        }

        for (int rank = 0; rank < maxLevel.SkillMaxLevels.Count; rank++)
        {
            if (skillMaxLevel <= maxLevel.SkillMaxLevels[rank])
            {
                return rank;
            }
        }

        return 0;
    }

    public int GetSkillMaxLvFromItemSubType(int evolutionType, int useItemSubType)
    {
        if (!_digimonEvoMaxLevels.TryGetValue(evolutionType, out BaseDigimonEvoMaxLevelRecord? maxLevel))
        {
            return 0;
        }

        int skillRank = GetOpenSkillRankFromItemSubType(useItemSubType);
        return skillRank >= 0 && skillRank < maxLevel.SkillMaxLevels.Count ? maxLevel.SkillMaxLevels[skillRank] : 0;
    }

    public long GetDigimonSkillMaxExp(int evolutionType, int skillExpLevel)
    {
        if (!_digimonEvoMaxLevels.TryGetValue(evolutionType, out BaseDigimonEvoMaxLevelRecord? maxLevel))
        {
            return 0;
        }

        int skillTableLevel = maxLevel.SkillExpStartLevel + skillExpLevel;
        return GetDigimonBase(skillTableLevel, BtDigimonExp)?.Exp ?? 0;
    }

    private static BaseStatRecord? GetBaseWithLevelFallback(
        IReadOnlyDictionary<uint, BaseStatRecord> bases,
        int level,
        int type)
    {
        if (level < 1)
        {
            level = 1;
        }

        while (level >= 1)
        {
            uint id = (uint)(level + type * BaseCharTypeConstant);
            if (bases.TryGetValue(id, out BaseStatRecord? baseStat))
            {
                return baseStat;
            }

            level--;
        }

        return null;
    }

    private static List<uint> NormalizeJumpBusterDestinations(IReadOnlyList<uint> rawDestinations)
    {
        bool allAreaScope = rawDestinations.Count >= AllAreaMinDestinations;
        var destinations = new List<uint>(rawDestinations.Count + 2);
        foreach (uint mapId in rawDestinations)
        {
            AddUnique(destinations, mapId);
        }

        AddUnique(destinations, DatsCenterMapId);
        if (allAreaScope)
        {
            AddUnique(destinations, DTerminalMapId);
        }

        return destinations;
    }

    private static void AddUnique(List<uint> values, uint value)
    {
        if (!values.Contains(value))
        {
            values.Add(value);
        }
    }
}
