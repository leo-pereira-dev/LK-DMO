namespace DProject.Data.CsFileTable;

public sealed class CsWorldMapMng : IBinFileTableManager
{
    private readonly WorldMapBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<int, WorldMapRecord> _worlds = new();
    private readonly Dictionary<int, AreaMapRecord> _areas = new();
    private readonly Dictionary<int, List<AreaMapRecord>> _areasByWorld = new();
    private readonly List<WorldMapRecord> _realWorlds = [];
    private readonly List<WorldMapRecord> _digitalWorlds = [];

    public CsWorldMapMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsWorldMapMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<int, WorldMapRecord> Worlds => _worlds;
    public IReadOnlyDictionary<int, AreaMapRecord> Areas => _areas;
    public IReadOnlyDictionary<int, List<AreaMapRecord>> AreasByWorld => _areasByWorld;
    public IReadOnlyList<WorldMapRecord> RealWorlds => _realWorlds;
    public IReadOnlyList<WorldMapRecord> DigitalWorlds => _digitalWorlds;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is WorldMapBinData data)
        {
            foreach (WorldMapRecord world in data.Worlds)
            {
                _worlds.TryAdd(world.WorldId, world);
            }

            foreach (AreaMapRecord area in data.Areas)
            {
                _areas.TryAdd(area.MapId, area);
            }

            ConnectArea();
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _worlds.Clear();
        _areas.Clear();
        _areasByWorld.Clear();
        _realWorlds.Clear();
        _digitalWorlds.Clear();
        IsLoaded = false;
    }

    public bool IsWorld(int id) => _worlds.ContainsKey(id);

    public bool IsArea(int id) => _areas.ContainsKey(id);

    public WorldMapRecord? GetWorld(int id)
    {
        return _worlds.TryGetValue(id, out WorldMapRecord? world) ? world : null;
    }

    public AreaMapRecord? GetArea(int id)
    {
        return _areas.TryGetValue(id, out AreaMapRecord? area) ? area : null;
    }

    public IReadOnlyList<AreaMapRecord> GetAreaList(int worldId)
    {
        return _areasByWorld.TryGetValue(worldId, out List<AreaMapRecord>? areas) ? areas : [];
    }

    public IReadOnlyDictionary<int, AreaMapRecord> GetAreaList()
    {
        return _areas;
    }

    public IReadOnlyDictionary<int, WorldMapRecord> GetWorldList()
    {
        return _worlds;
    }

    public string GetWorldName(uint worldId)
    {
        return worldId <= int.MaxValue && _worlds.TryGetValue((int)worldId, out WorldMapRecord? world)
            ? world.Name
            : string.Empty;
    }

    public string GetAreaName(uint areaId)
    {
        return areaId <= int.MaxValue && _areas.TryGetValue((int)areaId, out AreaMapRecord? area)
            ? area.Name
            : string.Empty;
    }

    public void ReloadGaussianBlur()
    {
        // Excel reload is intentionally left as a placeholder in the C# port.
    }

    private void ConnectArea()
    {
        foreach (AreaMapRecord area in _areas.Values)
        {
            if (!_worlds.ContainsKey(area.AreaType))
            {
                continue;
            }

            if (!_areasByWorld.TryGetValue(area.AreaType, out List<AreaMapRecord>? worldAreas))
            {
                worldAreas = [];
                _areasByWorld[area.AreaType] = worldAreas;
            }

            worldAreas.Add(area);
        }

        foreach (WorldMapRecord world in _worlds.Values)
        {
            if (world.WorldType == 1)
            {
                _realWorlds.Add(world);
            }
            else if (world.WorldType == 2)
            {
                _digitalWorlds.Add(world);
            }
        }
    }
}
