namespace DProject.Data.CsFileTable;

public sealed class CsMapMng : ICompositeBinFileTableManager
{
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly List<BinLoadResult> _loadResults = new();

    public CsMapMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapMng";
    public bool IsLoaded { get; private set; }
    public CsMapListMng? MapListMng { get; private set; }
    public CsMapPortalMng? MapPortalMng { get; private set; }
    public CsMapNpcMng? MapNpcMng { get; private set; }
    public CsMapMonsterMng? MapMonsterMng { get; private set; }
    public CsMapRegionMng? MapRegionMng { get; private set; }
    public CsMapCharLightMng? MapCharLightMng { get; private set; }
    public CsWeatherMng? WeatherMng { get; private set; }
    public CsMapObjectMng? MapObjectMng { get; private set; }
    public CsMapTableObjMng? MapTableObjMng { get; private set; }
    public IReadOnlyCollection<BinLoadResult> LoadResults => _loadResults;

    public bool Init()
    {
        Delete();

        MapListMng = new CsMapListMng(_fileType, _languagePath, _allowMissingTables);
        MapPortalMng = new CsMapPortalMng(_fileType, _languagePath, _allowMissingTables);
        MapNpcMng = new CsMapNpcMng(_fileType, _languagePath, _allowMissingTables);
        MapMonsterMng = new CsMapMonsterMng(_fileType, _languagePath, _allowMissingTables);
        MapRegionMng = new CsMapRegionMng(_fileType, _languagePath, _allowMissingTables);
        MapCharLightMng = new CsMapCharLightMng(_fileType, _languagePath, _allowMissingTables);
        WeatherMng = new CsWeatherMng(_fileType, _languagePath, _allowMissingTables);
        MapObjectMng = new CsMapObjectMng(_fileType, _languagePath, _allowMissingTables);
        MapTableObjMng = new CsMapTableObjMng(_fileType, _allowMissingTables);

        if (!InitSubManager(MapListMng) ||
            !InitSubManager(MapPortalMng) ||
            !InitSubManager(MapNpcMng) ||
            !InitSubManager(MapMonsterMng) ||
            !InitSubManager(MapRegionMng) ||
            !InitSubManager(MapCharLightMng) ||
            !InitSubManager(WeatherMng) ||
            !InitSubManager(MapObjectMng) ||
            !InitSubManager(MapTableObjMng))
        {
            return false;
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        MapListMng?.Delete();
        MapPortalMng?.Delete();
        MapNpcMng?.Delete();
        MapMonsterMng?.Delete();
        MapRegionMng?.Delete();
        MapCharLightMng?.Delete();
        WeatherMng?.Delete();
        MapObjectMng?.Delete();
        MapTableObjMng?.Delete();
        MapListMng = null;
        MapPortalMng = null;
        MapNpcMng = null;
        MapMonsterMng = null;
        MapRegionMng = null;
        MapCharLightMng = null;
        WeatherMng = null;
        MapObjectMng = null;
        MapTableObjMng = null;
        _loadResults.Clear();
        IsLoaded = false;
    }

    public MapListRecord? GetList(uint mapId) => MapListMng?.GetList(mapId);

    public string GetMapPath(uint mapId) => MapListMng?.GetMapPath(mapId) ?? string.Empty;

    public IReadOnlyList<MapPortalRecord> GetPortals(uint mapId) => MapPortalMng?.GetGroup(mapId) ?? [];

    public IReadOnlyList<MapNpcRecord> GetNpcs(uint mapId) => MapNpcMng?.GetGroup(mapId) ?? [];

    public IReadOnlyList<MapMonsterRecord> GetMonsters(uint mapId) => MapMonsterMng?.GetGroup(mapId) ?? [];

    public IReadOnlyList<MapRegionRecord> GetRegions(uint mapId) => MapRegionMng?.GetRegionGroup(mapId) ?? [];

    public IReadOnlyList<MapLimitEvoRecord> GetLimitEvoRegions(uint mapId) => MapRegionMng?.GetLimitEvoGroup(mapId) ?? [];

    public IReadOnlyList<MapBuffRegionRecord> GetBuffRegions(uint mapId) => MapRegionMng?.GetBuffRegionGroup(mapId) ?? [];

    public IReadOnlyList<MapCharLightRecord> GetCharLights(uint mapId) => MapCharLightMng?.GetGroup(mapId) ?? [];

    public IReadOnlyList<WeatherRecord> GetWeathers(uint mapId) => WeatherMng?.GetGroup(mapId) ?? [];

    public IReadOnlyList<MapObjectEntryRecord> GetObjects(uint mapId) => MapObjectMng?.GetGroup(mapId) ?? [];

    public MapTableObjGroupRecord? GetTableObjGroup(uint mapId) => MapTableObjMng?.GetGroup(mapId);

    private bool InitSubManager(IBinFileTableManager manager)
    {
        if (!manager.Init())
        {
            return false;
        }

        if (manager.LoadResult is not null)
        {
            _loadResults.Add(manager.LoadResult);
        }

        return true;
    }
}

public sealed class CsMapListMng : IBinFileTableManager
{
    private readonly MapListBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, MapListRecord> _maps = new();

    public CsMapListMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapListMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, MapListRecord> Maps => _maps;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MapListBinData data)
        {
            foreach (MapListRecord map in data.Maps)
            {
                _maps.TryAdd(map.MapId, map);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _maps.Clear();
        IsLoaded = false;
    }

    public bool IsList(uint mapId) => _maps.ContainsKey(mapId);

    public MapListRecord? GetList(uint mapId)
    {
        return _maps.TryGetValue(mapId, out MapListRecord? map) ? map : null;
    }

    public MapListRecord? GetList(string mapPath)
    {
        return _maps.Values.FirstOrDefault(map =>
            map.MapPath.Equals(mapPath, StringComparison.OrdinalIgnoreCase));
    }

    public MapListRecord? GetListIndex(int index)
    {
        return index < 0 || index >= _maps.Count
            ? null
            : _maps.Values.ElementAt(index);
    }

    public string GetMapPath(uint mapId)
    {
        return _maps.TryGetValue(mapId, out MapListRecord? map) ? map.MapPath : string.Empty;
    }

    public int MapIDToMapRegionID(uint mapId)
    {
        return _maps.TryGetValue(mapId, out MapListRecord? map) ? map.MapRegionId : 0;
    }

    public bool IsBattleTagUseMap(uint mapId)
    {
        return _maps.TryGetValue(mapId, out MapListRecord? map) && map.BattleTagUse;
    }
}

public sealed class CsMapPortalMng : IBinFileTableManager
{
    private readonly MapPortalBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, List<MapPortalRecord>> _portalGroups = new();

    public CsMapPortalMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapPortalMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, List<MapPortalRecord>> PortalGroups => _portalGroups;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MapPortalBinData data)
        {
            foreach (MapPortalRecord portal in data.Portals)
            {
                if (!_portalGroups.TryGetValue(portal.SourceMapId, out List<MapPortalRecord>? group))
                {
                    group = [];
                    _portalGroups[portal.SourceMapId] = group;
                }

                group.Add(portal);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _portalGroups.Clear();
        IsLoaded = false;
    }

    public IReadOnlyList<MapPortalRecord> GetGroup(uint mapId)
    {
        return _portalGroups.TryGetValue(mapId, out List<MapPortalRecord>? group) ? group : [];
    }
}

public sealed class CsMapNpcMng : IBinFileTableManager
{
    private readonly MapNpcBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, List<MapNpcRecord>> _npcGroups = new();

    public CsMapNpcMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapNpcMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, List<MapNpcRecord>> NpcGroups => _npcGroups;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MapNpcBinData data)
        {
            foreach (MapNpcRecord npc in data.Npcs)
            {
                if (!_npcGroups.TryGetValue(npc.MapId, out List<MapNpcRecord>? group))
                {
                    group = [];
                    _npcGroups[npc.MapId] = group;
                }

                group.Add(npc);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _npcGroups.Clear();
        IsLoaded = false;
    }

    public IReadOnlyList<MapNpcRecord> GetGroup(uint mapId)
    {
        return _npcGroups.TryGetValue(mapId, out List<MapNpcRecord>? group) ? group : [];
    }
}

public sealed class CsMapMonsterMng : IBinFileTableManager
{
    private readonly MapMonsterBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, List<MapMonsterRecord>> _monsterGroups = new();
    private readonly Dictionary<uint, List<MapMonsterRecord>> _monstersByFileTableId = new();

    public CsMapMonsterMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapMonsterMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, List<MapMonsterRecord>> MonsterGroups => _monsterGroups;
    public IReadOnlyDictionary<uint, List<MapMonsterRecord>> MonstersByFileTableId => _monstersByFileTableId;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MapMonsterBinData data)
        {
            foreach (MapMonsterRecord monster in data.Monsters)
            {
                AddToGroup(_monsterGroups, monster.MapId, monster);
                AddToGroup(_monstersByFileTableId, monster.FileTableId, monster);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _monsterGroups.Clear();
        _monstersByFileTableId.Clear();
        IsLoaded = false;
    }

    public IReadOnlyList<MapMonsterRecord> GetGroup(uint mapId)
    {
        return _monsterGroups.TryGetValue(mapId, out List<MapMonsterRecord>? group) ? group : [];
    }

    public IReadOnlyList<MapMonsterRecord> GetMonsterList(uint fileTableId)
    {
        return _monstersByFileTableId.TryGetValue(fileTableId, out List<MapMonsterRecord>? list) ? list : [];
    }

    private static void AddToGroup<TKey, TValue>(
        Dictionary<TKey, List<TValue>> groups,
        TKey key,
        TValue value)
        where TKey : notnull
    {
        if (!groups.TryGetValue(key, out List<TValue>? group))
        {
            group = [];
            groups[key] = group;
        }

        group.Add(value);
    }
}

public sealed class CsMapRegionMng : IBinFileTableManager
{
    private readonly MapRegionBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, List<MapRegionRecord>> _regionGroups = new();
    private readonly Dictionary<uint, List<MapLimitEvoRecord>> _limitEvoGroups = new();
    private readonly Dictionary<uint, List<MapBuffRegionRecord>> _buffRegionGroups = new();

    public CsMapRegionMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapRegionMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, List<MapRegionRecord>> RegionGroups => _regionGroups;
    public IReadOnlyDictionary<uint, List<MapLimitEvoRecord>> LimitEvoGroups => _limitEvoGroups;
    public IReadOnlyDictionary<uint, List<MapBuffRegionRecord>> BuffRegionGroups => _buffRegionGroups;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MapRegionBinData data)
        {
            foreach (MapRegionRecord region in data.Regions)
            {
                AddToGroup(_regionGroups, region.MapId, region);
            }

            foreach (MapLimitEvoRecord region in data.LimitEvoRegions)
            {
                AddToGroup(_limitEvoGroups, region.MapId, region);
            }

            foreach (MapBuffRegionRecord region in data.BuffRegions)
            {
                AddToGroup(_buffRegionGroups, region.MapId, region);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _regionGroups.Clear();
        _limitEvoGroups.Clear();
        _buffRegionGroups.Clear();
        IsLoaded = false;
    }

    public bool IsRegionGroup(uint mapId) => _regionGroups.ContainsKey(mapId);

    public bool IsLimitEvoGroup(uint mapId) => _limitEvoGroups.ContainsKey(mapId);

    public bool IsBuffRegionGroup(uint mapId) => _buffRegionGroups.ContainsKey(mapId);

    public IReadOnlyList<MapRegionRecord> GetRegionGroup(uint mapId)
    {
        return _regionGroups.TryGetValue(mapId, out List<MapRegionRecord>? group) ? group : [];
    }

    public IReadOnlyList<MapLimitEvoRecord> GetLimitEvoGroup(uint mapId)
    {
        return _limitEvoGroups.TryGetValue(mapId, out List<MapLimitEvoRecord>? group) ? group : [];
    }

    public IReadOnlyList<MapBuffRegionRecord> GetBuffRegionGroup(uint mapId)
    {
        return _buffRegionGroups.TryGetValue(mapId, out List<MapBuffRegionRecord>? group) ? group : [];
    }

    private static void AddToGroup<TKey, TValue>(
        Dictionary<TKey, List<TValue>> groups,
        TKey key,
        TValue value)
        where TKey : notnull
    {
        if (!groups.TryGetValue(key, out List<TValue>? group))
        {
            group = [];
            groups[key] = group;
        }

        group.Add(value);
    }
}

public sealed class CsMapCharLightMng : IBinFileTableManager
{
    private readonly MapCharLightBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, List<MapCharLightRecord>> _lightGroups = new();

    public CsMapCharLightMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapCharLightMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, List<MapCharLightRecord>> LightGroups => _lightGroups;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MapCharLightBinData data)
        {
            foreach (MapCharLightRecord light in data.Lights)
            {
                if (!_lightGroups.TryGetValue(light.MapId, out List<MapCharLightRecord>? group))
                {
                    group = [];
                    _lightGroups[light.MapId] = group;
                }

                group.Add(light);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _lightGroups.Clear();
        IsLoaded = false;
    }

    public bool IsGroup(uint mapId) => _lightGroups.ContainsKey(mapId);

    public IReadOnlyList<MapCharLightRecord> GetGroup(uint mapId)
    {
        return _lightGroups.TryGetValue(mapId, out List<MapCharLightRecord>? group) ? group : [];
    }
}

public sealed class CsWeatherMng : IBinFileTableManager
{
    private readonly WeatherBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, List<WeatherRecord>> _weatherGroups = new();

    public CsWeatherMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsWeatherMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, List<WeatherRecord>> WeatherGroups => _weatherGroups;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is WeatherBinData data)
        {
            foreach (WeatherRecord weather in data.Weathers)
            {
                if (!_weatherGroups.TryGetValue(weather.MapId, out List<WeatherRecord>? group))
                {
                    group = [];
                    _weatherGroups[weather.MapId] = group;
                }

                group.Add(weather);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _weatherGroups.Clear();
        IsLoaded = false;
    }

    public bool IsGroup(uint mapId) => _weatherGroups.ContainsKey(mapId);

    public IReadOnlyList<WeatherRecord> GetGroup(uint mapId)
    {
        return _weatherGroups.TryGetValue(mapId, out List<WeatherRecord>? group) ? group : [];
    }
}

public sealed class CsMapObjectMng : IBinFileTableManager
{
    private readonly MapObjectBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, List<MapObjectEntryRecord>> _objectGroups = new();
    private readonly Dictionary<(uint MapId, uint ObjectId), IReadOnlyList<MapObjectInstanceRecord>> _objectsById = new();

    public CsMapObjectMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapObjectMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, List<MapObjectEntryRecord>> ObjectGroups => _objectGroups;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MapObjectBinData data)
        {
            foreach (MapObjectGroupRecord group in data.Groups)
            {
                _objectGroups[group.MapId] = group.Objects.ToList();
                foreach (MapObjectEntryRecord obj in group.Objects)
                {
                    _objectsById[(group.MapId, obj.ObjectId)] = obj.Instances;
                }
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _objectGroups.Clear();
        _objectsById.Clear();
        IsLoaded = false;
    }

    public bool IsGroup(uint mapId) => _objectGroups.ContainsKey(mapId);

    public bool IsGroup(uint mapId, uint objectId) => _objectsById.ContainsKey((mapId, objectId));

    public IReadOnlyList<MapObjectEntryRecord> GetGroup(uint mapId)
    {
        return _objectGroups.TryGetValue(mapId, out List<MapObjectEntryRecord>? group) ? group : [];
    }

    public IReadOnlyList<MapObjectInstanceRecord> GetFactorList(uint mapId, uint objectId)
    {
        return _objectsById.TryGetValue((mapId, objectId), out IReadOnlyList<MapObjectInstanceRecord>? instances)
            ? instances
            : [];
    }

    public MapObjectFactorRecord? GetFactorInfo(uint mapId, uint objectId, byte factor1, byte factor2)
    {
        IReadOnlyList<MapObjectInstanceRecord> instances = GetFactorList(mapId, objectId);
        if (factor1 >= instances.Count)
        {
            return null;
        }

        IReadOnlyList<MapObjectFactorRecord> factors = instances[factor1].Factors;
        return factor2 < factors.Count ? factors[factor2] : null;
    }
}

public sealed class CsMapTableObjMng : IBinFileTableManager
{
    private readonly MapTableObjBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, MapTableObjGroupRecord> _groups = new();

    public CsMapTableObjMng(eFILE_TYPE fileType, bool allowMissingTables)
    {
        _fileType = fileType;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMapTableObjMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, MapTableObjGroupRecord> Groups => _groups;

    public bool Init()
    {
        LoadResult = _loader.Load(string.Empty, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MapTableObjBinData data)
        {
            foreach (MapTableObjGroupRecord group in data.Groups)
            {
                _groups.TryAdd(group.MapId, group);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _groups.Clear();
        IsLoaded = false;
    }

    public bool IsGroup(uint mapId) => _groups.ContainsKey(mapId);

    public MapTableObjGroupRecord? GetGroup(uint mapId)
    {
        return _groups.TryGetValue(mapId, out MapTableObjGroupRecord? group) ? group : null;
    }

    public MapTableObjNodeRecord? GetNode(uint mapId, int nodeIndex)
    {
        return _groups.TryGetValue(mapId, out MapTableObjGroupRecord? group)
            ? group.Nodes.FirstOrDefault(node => node.NodeIndex == nodeIndex)
            : null;
    }
}
