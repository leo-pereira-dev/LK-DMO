namespace DProject.Data.CsFileTable;

public sealed class CsTacticsMng : IBinFileTableManager
{
    private readonly TacticsBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, TacticsRecord> _tactics = new();
    private readonly Dictionary<uint, TacticsExplainRecord> _explains = new();
    private readonly Dictionary<uint, TacticsEnchantItemRecord> _enchantItems = new();
    private readonly Dictionary<int, IReadOnlyList<TacticsEnchantStatRecord>> _enchantStats = new();
    private readonly Dictionary<int, IReadOnlyDictionary<int, TacticsTranscendInfoRecord>> _transcendInfos = new();
    private readonly Dictionary<uint, IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>> _transcendEvos = new();
    private readonly Dictionary<uint, IReadOnlyDictionary<int, uint>> _expDigimons = new();

    public CsTacticsMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsTacticsMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, TacticsRecord> TacticsMap => _tactics;
    public IReadOnlyDictionary<uint, TacticsExplainRecord> ExplainMap => _explains;
    public IReadOnlyDictionary<uint, TacticsEnchantItemRecord> EnchantItemData => _enchantItems;
    public TacticsTranscendCorrectRecord Correct { get; private set; } = new(0, new Dictionary<int, IReadOnlyDictionary<int, float>>());

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is TacticsBinData data)
        {
            foreach (TacticsRecord record in data.Tactics)
            {
                _tactics[record.DigitamaId] = record;
            }

            foreach (TacticsExplainRecord record in data.Explains)
            {
                _explains[record.TacticsMonId] = record;
            }

            foreach (KeyValuePair<uint, TacticsEnchantItemRecord> item in data.EnchantItems)
            {
                _enchantItems[item.Key] = item.Value;
            }

            foreach (KeyValuePair<int, IReadOnlyList<TacticsEnchantStatRecord>> stat in data.EnchantStats)
            {
                _enchantStats[stat.Key] = stat.Value;
            }

            foreach (KeyValuePair<int, IReadOnlyDictionary<int, TacticsTranscendInfoRecord>> info in data.TranscendInfos)
            {
                _transcendInfos[info.Key] = info.Value;
            }

            foreach (KeyValuePair<uint, IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>> evo in data.TranscendEvos)
            {
                _transcendEvos[evo.Key] = evo.Value;
            }

            foreach (KeyValuePair<uint, IReadOnlyDictionary<int, uint>> expDigimon in data.ExpDigimonGroups)
            {
                _expDigimons[expDigimon.Key] = expDigimon.Value;
            }

            Correct = data.Correct;
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _tactics.Clear();
        _explains.Clear();
        _enchantItems.Clear();
        _enchantStats.Clear();
        _transcendInfos.Clear();
        _transcendEvos.Clear();
        _expDigimons.Clear();
        Correct = new TacticsTranscendCorrectRecord(0, new Dictionary<int, IReadOnlyDictionary<int, float>>());
        IsLoaded = false;
    }

    public bool IsTactics(uint digitamaId) => _tactics.ContainsKey(digitamaId);

    public TacticsRecord? GetTactics(uint digitamaId)
    {
        return _tactics.TryGetValue(digitamaId, out TacticsRecord? record) ? record : null;
    }

    public bool IsTacticsExplain(uint digimonId) => _explains.ContainsKey(digimonId);

    public TacticsExplainRecord? GetTacticsExplain(uint digimonId)
    {
        return _explains.TryGetValue(digimonId, out TacticsExplainRecord? record) ? record : null;
    }

    public TacticsTranscendInfoRecord? GetTranscendInfo(int currentEvoIdx, int currentGrowth)
    {
        return _transcendInfos.TryGetValue(currentEvoIdx, out IReadOnlyDictionary<int, TacticsTranscendInfoRecord>? group) &&
            group.TryGetValue(currentGrowth, out TacticsTranscendInfoRecord? info)
                ? info
                : null;
    }

    public IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>? GetTranscendEvo(uint digimonIdx)
    {
        return _transcendEvos.TryGetValue(digimonIdx, out IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>? group)
            ? group
            : null;
    }

    public IReadOnlyList<TacticsTranscendMaterialRecord>? GetTranscendEvo(uint digimonIdx, int itemUseType)
    {
        return _transcendEvos.TryGetValue(digimonIdx, out IReadOnlyDictionary<int, IReadOnlyList<TacticsTranscendMaterialRecord>>? group) &&
            group.TryGetValue(itemUseType, out IReadOnlyList<TacticsTranscendMaterialRecord>? materials)
                ? materials
                : null;
    }

    public TacticsTranscendCorrectRecord GetTranscend_Correct() => Correct;

    public float GetSameCorrectValue(bool same, int growth)
    {
        int sameType = same ? 1 : 0;
        return Correct.SameTypeCorrect.TryGetValue(sameType, out IReadOnlyDictionary<int, float>? group) &&
            group.TryGetValue(growth, out float value)
                ? value
                : 0;
    }

    public bool IsEnchantData(uint itemType) => _enchantItems.ContainsKey(itemType);

    public TacticsEnchantItemRecord? GetEnchantItemInfo(uint itemType)
    {
        return _enchantItems.TryGetValue(itemType, out TacticsEnchantItemRecord? item) ? item : null;
    }

    public IReadOnlyDictionary<uint, TacticsEnchantItemRecord> GetEnchantItemData() => _enchantItems;

    public TacticsEnchantStatRecord? GetEnchantStatInfo(int statIdx, int enchantLevel)
    {
        if (!_enchantStats.TryGetValue(statIdx, out IReadOnlyList<TacticsEnchantStatRecord>? list))
        {
            return null;
        }

        return list.FirstOrDefault(info =>
            info.LowEnchantLevel <= enchantLevel &&
            info.HighEnchantLevel >= enchantLevel);
    }

    public bool IsEnableEnchantHatchLv(int statIdx, int enchantLevel, int hatchLevel)
    {
        TacticsEnchantStatRecord? info = GetEnchantStatInfo(statIdx, enchantLevel);
        return info is not null &&
            info.DigimonGrowMinLevel <= hatchLevel &&
            info.DigimonGrowMaxLevel >= hatchLevel;
    }

    public bool IsExpDigimon(uint digimonIdx, int currentGrowth)
    {
        return _expDigimons.TryGetValue(digimonIdx, out IReadOnlyDictionary<int, uint>? group) &&
            group.ContainsKey(currentGrowth);
    }

    public uint GetExpDigimonFixedFigure(uint digimonIdx, int currentGrowth)
    {
        return _expDigimons.TryGetValue(digimonIdx, out IReadOnlyDictionary<int, uint>? group) &&
            group.TryGetValue(currentGrowth, out uint fixedFigure)
                ? fixedFigure
                : 0;
    }
}
