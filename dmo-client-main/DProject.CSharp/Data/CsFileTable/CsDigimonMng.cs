namespace DProject.Data.CsFileTable;

public sealed class CsDigimonMng : IBinFileTableManager
{
    private readonly DigimonBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, DigimonBinRecord> _digimons = new();
    private readonly Dictionary<uint, RideBinRecord> _rides = new();
    private readonly Dictionary<uint, AttributeTypeUiRecord> _attributeTypes = new();
    private readonly Dictionary<uint, NatureTypeUiRecord> _natureTypes = new();

    public CsDigimonMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsDigimonMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, DigimonBinRecord> Digimons => _digimons;
    public IReadOnlyDictionary<uint, RideBinRecord> Rides => _rides;
    public IReadOnlyDictionary<uint, AttributeTypeUiRecord> AttributeTypes => _attributeTypes;
    public IReadOnlyDictionary<uint, NatureTypeUiRecord> NatureTypes => _natureTypes;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is DigimonBinData data)
        {
            foreach (DigimonBinRecord digimon in data.Digimons)
            {
                _digimons.TryAdd(digimon.DigimonId, digimon);
            }

            foreach (RideBinRecord ride in data.Rides)
            {
                _rides.TryAdd(ride.DigimonId, ride);
            }

            foreach (AttributeTypeUiRecord attributeType in data.AttributeTypes)
            {
                _attributeTypes.TryAdd(attributeType.AttributeId, attributeType);
            }

            foreach (NatureTypeUiRecord natureType in data.NatureTypes)
            {
                _natureTypes.TryAdd(natureType.NatureId, natureType);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _digimons.Clear();
        _rides.Clear();
        _attributeTypes.Clear();
        _natureTypes.Clear();
        IsLoaded = false;
    }

    public DigimonBinRecord? GetDigimon(uint digimonId)
    {
        return _digimons.TryGetValue(digimonId, out DigimonBinRecord? digimon) ? digimon : null;
    }

    public int GetDigimonRank(uint digimonId)
    {
        return GetDigimon(digimonId)?.DigimonRank ?? 0;
    }

    public RideBinRecord? GetRide(uint digimonId)
    {
        return _rides.TryGetValue(digimonId, out RideBinRecord? ride) ? ride : null;
    }

    public bool IsRide(uint digimonId)
    {
        return _rides.ContainsKey(digimonId);
    }

    public bool IsDigimon(uint digimonId)
    {
        return _digimons.ContainsKey(digimonId);
    }

    public IReadOnlyList<uint> FindDigimon_FromModelID(uint modelId)
    {
        return _digimons
            .Where(pair => pair.Value.ModelId == modelId)
            .Select(pair => pair.Key)
            .ToArray();
    }

    public uint GetDigimonModelIdx(uint digimonId)
    {
        return GetDigimon(digimonId)?.ModelId ?? 0;
    }

    public static bool IsStartDigimon(uint digimonId)
    {
        return digimonId is 31001 or 31002 or 31003 or 31004;
    }

    public AttributeTypeUiRecord? GetAttributeTypeUI(uint attributeTypeId)
    {
        return _attributeTypes.TryGetValue(attributeTypeId, out AttributeTypeUiRecord? attributeType)
            ? attributeType
            : null;
    }

    public bool IsAttributeTypeUI(uint attributeTypeId)
    {
        return _attributeTypes.ContainsKey(attributeTypeId);
    }

    public NatureTypeUiRecord? GetNatureTypeUI(uint natureTypeId)
    {
        return _natureTypes.TryGetValue(natureTypeId, out NatureTypeUiRecord? natureType)
            ? natureType
            : null;
    }

    public bool IsNatureTypeUI(uint natureTypeId)
    {
        return _natureTypes.ContainsKey(natureTypeId);
    }
}
