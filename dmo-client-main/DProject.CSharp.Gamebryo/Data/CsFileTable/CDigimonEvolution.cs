namespace DProject.Data.CsFileTable;

public sealed class CDigimonEvolution : IBinFileTableManager
{
    private readonly DigimonEvolutionBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, DigimonEvolutionInfoRecord> _evolutions = new();
    private readonly Dictionary<uint, DigimonEvolutionObjectRecord> _evolutionObjects = new();
    private readonly List<BattleEvolutionInfoRecord> _battleEvolutions = new();

    public CDigimonEvolution(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CDigimonEvolution";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, DigimonEvolutionInfoRecord> Map => _evolutions;
    public IReadOnlyDictionary<uint, DigimonEvolutionObjectRecord> EvolutionObjects => _evolutionObjects;
    public IReadOnlyList<BattleEvolutionInfoRecord> BattleEvolutions => _battleEvolutions;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is DigimonEvolutionBinData data)
        {
            foreach (DigimonEvolutionInfoRecord evolution in data.Evolutions)
            {
                _evolutions.TryAdd(evolution.DigimonId, evolution);

                foreach (KeyValuePair<uint, DigimonEvolutionObjectRecord> obj in evolution.Objects)
                {
                    _evolutionObjects.TryAdd(obj.Key, obj.Value);
                }
            }

            _battleEvolutions.AddRange(data.BattleEvolutions);
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _evolutions.Clear();
        _evolutionObjects.Clear();
        _battleEvolutions.Clear();
        IsLoaded = false;
    }

    public bool IsEvolveInfo(uint digimonId) => _evolutions.ContainsKey(digimonId);

    public DigimonEvolutionInfoRecord? GetEvolveInfo(uint digimonId)
    {
        return _evolutions.TryGetValue(digimonId, out DigimonEvolutionInfoRecord? info) ? info : null;
    }

    public DigimonEvolutionObjectRecord? GetEvolObj(uint digimonId)
    {
        return _evolutionObjects.TryGetValue(digimonId, out DigimonEvolutionObjectRecord? obj) ? obj : null;
    }

    public BattleEvolutionInfoRecord? GetBattleEvo(int type)
    {
        int index = type - 1;
        return (uint)index < (uint)_battleEvolutions.Count ? _battleEvolutions[index] : null;
    }
}
