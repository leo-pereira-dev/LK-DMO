using DProject.Data.CsFileTable;
using DProject.GlobalData;
using DProject.Runtime;

namespace DProject.Data.Loading;

public enum MapLoadState
{
    NotStarted,
    Planning,
    LoadingAssets,
    Applying,
    Succeeded,
    Failed
}

public sealed record MapAssetRef(
    AssetKind Kind,
    string Key,
    string Path,
    bool Required,
    IReadOnlyDictionary<string, string> Metadata);

public sealed record MapRenderableRef(
    string Key,
    string Kind,
    int Count,
    IReadOnlyDictionary<string, string> Metadata);

public sealed record MapLoadPlan(
    uint MapId,
    string MapName,
    string MapPath,
    int Width,
    int Height,
    string TerrainPath,
    IReadOnlyList<MapAssetRef> Assets,
    IReadOnlyList<MapRenderableRef> Renderables,
    int PortalCount,
    int NpcCount,
    int MonsterSpawnCount,
    int RegionCount,
    int LightCount,
    int WeatherCount,
    int MapObjectCount,
    int TableObjectNodeCount,
    int TableObjectCount);

public sealed record MapLoadResult(
    bool Success,
    MapLoadPlan? Plan,
    IReadOnlyList<AssetLoadResult> AssetResults,
    string? ErrorMessage,
    DateTimeOffset CompletedAt);

public sealed class MapLoadingService
{
    private readonly GlobalDataStore _globalData;
    private readonly MainThreadDispatcher _mainThread;
    private readonly JobSystem _jobs;
    private readonly AssetLoadingCoordinator _assetLoading;
    private readonly RenderCommandQueue _renderCommands;
    private long _generation;

    public MapLoadingService(
        GlobalDataStore globalData,
        MainThreadDispatcher mainThread,
        JobSystem jobs,
        AssetLoadingCoordinator assetLoading,
        RenderCommandQueue renderCommands)
    {
        _globalData = globalData;
        _mainThread = mainThread;
        _jobs = jobs;
        _assetLoading = assetLoading;
        _renderCommands = renderCommands;
    }

    public MapLoadState State { get; private set; } = MapLoadState.NotStarted;
    public MapLoadPlan? CurrentPlan { get; private set; }
    public MapLoadResult? LastResult { get; private set; }
    public int PendingAssetCount { get; private set; }

    public void RequestLoad(
        CsFileTableMng fileTables,
        uint mapId,
        string fallbackMapName,
        string reason,
        Action<MapLoadResult>? afterApply = null)
    {
        long generation = Interlocked.Increment(ref _generation);
        ApplyOnMainThread(() =>
        {
            State = MapLoadState.Planning;
            CurrentPlan = null;
            PendingAssetCount = 0;
            _globalData.LastMapLoadStatus = "Planning";
            _globalData.LastMapLoadReason = reason;
            _globalData.MapAssetsLoaded = false;
        });

        if (_jobs.IsStarted)
        {
            _jobs.Enqueue(token => BuildPlanOnWorker(fileTables, mapId, fallbackMapName, reason, generation, afterApply, token));
            return;
        }

        ThreadPool.QueueUserWorkItem(_ =>
            BuildPlanOnWorker(fileTables, mapId, fallbackMapName, reason, generation, afterApply, CancellationToken.None));
    }

    private void BuildPlanOnWorker(
        CsFileTableMng fileTables,
        uint mapId,
        string fallbackMapName,
        string reason,
        long generation,
        Action<MapLoadResult>? afterApply,
        CancellationToken token)
    {
        try
        {
            token.ThrowIfCancellationRequested();
            MapLoadPlan plan = BuildPlan(fileTables, mapId, fallbackMapName, reason);
            _mainThread.Post(() => ApplyPlan(generation, plan, afterApply));
        }
        catch (Exception ex)
        {
            var result = new MapLoadResult(false, null, [], ex.Message, DateTimeOffset.UtcNow);
            _mainThread.Post(() => Complete(generation, result, afterApply));
        }
    }

    private MapLoadPlan BuildPlan(CsFileTableMng fileTables, uint mapId, string fallbackMapName, string reason)
    {
        CsMapMng? mapMng = fileTables.MapMng;
        MapListRecord? map = mapMng?.GetList(mapId);
        string mapPath = map?.MapPath ?? string.Empty;
        string mapName = !string.IsNullOrWhiteSpace(map?.MapName)
            ? map.MapName
            : !string.IsNullOrWhiteSpace(fallbackMapName)
                ? fallbackMapName
                : $"Map {mapId}";
        string terrainPath = NormalizeMapPath(mapPath);
        string bgSound = map?.BgSound ?? string.Empty;

        var assets = new List<MapAssetRef>();
        if (!string.IsNullOrWhiteSpace(terrainPath))
        {
            assets.Add(new MapAssetRef(
                AssetKind.Map,
                $"map:{mapId}:terrain",
                terrainPath,
                Required: false,
                new Dictionary<string, string>
                {
                    ["MapId"] = mapId.ToString(),
                    ["MapName"] = mapName,
                    ["Reason"] = reason
                }));
        }

        if (!string.IsNullOrWhiteSpace(bgSound))
        {
            assets.Add(new MapAssetRef(
                AssetKind.Sound,
                $"map:{mapId}:bgm",
                NormalizeSoundPath(bgSound),
                Required: false,
                new Dictionary<string, string>
                {
                    ["MapId"] = mapId.ToString(),
                    ["MapName"] = mapName
                }));
        }

        IReadOnlyList<MapPortalRecord> portals = mapMng?.GetPortals(mapId) ?? [];
        IReadOnlyList<MapNpcRecord> npcs = mapMng?.GetNpcs(mapId) ?? [];
        IReadOnlyList<MapMonsterRecord> monsters = mapMng?.GetMonsters(mapId) ?? [];
        IReadOnlyList<MapRegionRecord> regions = mapMng?.GetRegions(mapId) ?? [];
        IReadOnlyList<MapCharLightRecord> lights = mapMng?.GetCharLights(mapId) ?? [];
        IReadOnlyList<WeatherRecord> weathers = mapMng?.GetWeathers(mapId) ?? [];
        IReadOnlyList<MapObjectEntryRecord> objects = mapMng?.GetObjects(mapId) ?? [];
        MapTableObjGroupRecord? tableObj = mapMng?.GetTableObjGroup(mapId);
        int tableObjectCount = tableObj?.Nodes.Sum(static node => node.Objects.Count) ?? 0;

        var renderables = new List<MapRenderableRef>
        {
            BuildRenderable(mapId, "terrain", 1, ("Path", terrainPath)),
            BuildRenderable(mapId, "portals", portals.Count),
            BuildRenderable(mapId, "npcs", npcs.Count),
            BuildRenderable(mapId, "monster-spawns", monsters.Count),
            BuildRenderable(mapId, "regions", regions.Count),
            BuildRenderable(mapId, "lights", lights.Count),
            BuildRenderable(mapId, "weather", weathers.Count),
            BuildRenderable(mapId, "map-objects", objects.Count),
            BuildRenderable(mapId, "table-objects", tableObjectCount, ("NodeCount", (tableObj?.Nodes.Count ?? 0).ToString()))
        };

        return new MapLoadPlan(
            mapId,
            mapName,
            mapPath,
            map?.Width ?? 0,
            map?.Height ?? 0,
            terrainPath,
            assets,
            renderables,
            portals.Count,
            npcs.Count,
            monsters.Count,
            regions.Count,
            lights.Count,
            weathers.Count,
            objects.Count,
            tableObj?.Nodes.Count ?? 0,
            tableObjectCount);
    }

    private void ApplyPlan(long generation, MapLoadPlan plan, Action<MapLoadResult>? afterApply)
    {
        if (generation != Volatile.Read(ref _generation))
        {
            return;
        }

        State = MapLoadState.LoadingAssets;
        CurrentPlan = plan;
        PendingAssetCount = plan.Assets.Count;
        _globalData.LastMapLoadStatus = "LoadingAssets";
        _globalData.LastMapLoadId = plan.MapId;
        _globalData.LastMapLoadName = plan.MapName;
        _globalData.LastMapTerrainPath = plan.TerrainPath;
        _globalData.LastMapAssetCount = plan.Assets.Count;
        _globalData.LastMapRenderableCount = plan.Renderables.Count;

        foreach (MapRenderableRef renderable in plan.Renderables)
        {
            _globalData.MapRenderables[renderable.Key] = new MapRenderableState(
                renderable.Key,
                renderable.Kind,
                renderable.Count,
                renderable.Metadata);
            _renderCommands.Enqueue(() => { });
        }

        if (plan.Assets.Count == 0)
        {
            var result = new MapLoadResult(true, plan, [], null, DateTimeOffset.UtcNow);
            Complete(generation, result, afterApply);
            return;
        }

        var assetResults = new List<AssetLoadResult>(plan.Assets.Count);
        foreach (MapAssetRef asset in plan.Assets)
        {
            AssetLoadRequest request = _assetLoading.CreateRequest(
                asset.Kind,
                asset.Key,
                asset.Path,
                allowMissing: !asset.Required,
                asset.Metadata);
            _assetLoading.Enqueue(request, assetResult =>
            {
                if (generation != Volatile.Read(ref _generation))
                {
                    return;
                }

                assetResults.Add(assetResult);
                PendingAssetCount = Math.Max(0, plan.Assets.Count - assetResults.Count);
                if (assetResults.Count == plan.Assets.Count)
                {
                    bool success = assetResults.All(static result => result.Status is AssetLoadStatus.Loaded or AssetLoadStatus.Missing);
                    var result = new MapLoadResult(
                        success,
                        plan,
                        assetResults.ToArray(),
                        success ? null : "One or more map assets failed to load.",
                        DateTimeOffset.UtcNow);
                    Complete(generation, result, afterApply);
                }
            });
        }
    }

    private void Complete(long generation, MapLoadResult result, Action<MapLoadResult>? afterApply)
    {
        if (generation != Volatile.Read(ref _generation))
        {
            return;
        }

        State = result.Success ? MapLoadState.Succeeded : MapLoadState.Failed;
        LastResult = result;
        PendingAssetCount = 0;
        _globalData.LastMapLoadResult = result;
        _globalData.LastMapLoadStatus = State.ToString();
        _globalData.MapAssetsLoaded = result.Success;
        if (result.Plan is not null)
        {
            _globalData.LastMapLoadId = result.Plan.MapId;
            _globalData.LastMapLoadName = result.Plan.MapName;
            _globalData.LastMapTerrainPath = result.Plan.TerrainPath;
            _globalData.LastMapAssetCount = result.Plan.Assets.Count;
            _globalData.LastMapRenderableCount = result.Plan.Renderables.Count;
        }

        afterApply?.Invoke(result);
    }

    private void ApplyOnMainThread(Action action)
    {
        if (_mainThread.IsMainThread)
        {
            action();
            return;
        }

        _mainThread.Post(action);
    }

    private static MapRenderableRef BuildRenderable(
        uint mapId,
        string kind,
        int count,
        params (string Key, string Value)[] metadata)
    {
        var values = new Dictionary<string, string>
        {
            ["MapId"] = mapId.ToString()
        };

        foreach ((string key, string value) in metadata)
        {
            values[key] = value;
        }

        return new MapRenderableRef($"map:{mapId}:{kind}", kind, count, values);
    }

    private static string NormalizeMapPath(string mapPath)
    {
        if (string.IsNullOrWhiteSpace(mapPath))
        {
            return string.Empty;
        }

        if (Path.IsPathRooted(mapPath) ||
            mapPath.StartsWith("Data\\", StringComparison.OrdinalIgnoreCase) ||
            mapPath.StartsWith("Data/", StringComparison.OrdinalIgnoreCase))
        {
            return mapPath;
        }

        return Path.Combine("Data", "Map", mapPath);
    }

    private static string NormalizeSoundPath(string soundPath)
    {
        if (string.IsNullOrWhiteSpace(soundPath))
        {
            return string.Empty;
        }

        if (Path.IsPathRooted(soundPath) ||
            soundPath.StartsWith("Data\\", StringComparison.OrdinalIgnoreCase) ||
            soundPath.StartsWith("Data/", StringComparison.OrdinalIgnoreCase))
        {
            return soundPath;
        }

        return Path.Combine("Data", "Sound", soundPath);
    }
}
