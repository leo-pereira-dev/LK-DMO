namespace DProject.Data.CsFileTable;

public sealed class CsSceneDataMng : IBinFileTableManager
{
    private readonly SceneDataBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, SceneRecord> _scenes = new();
    private readonly Dictionary<uint, SceneTargetRecord> _targets = new();

    public CsSceneDataMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsSceneDataMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, SceneRecord> SceneMap => _scenes;
    public IReadOnlyDictionary<uint, SceneTargetRecord> TargetMap => _targets;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is SceneDataBinData data)
        {
            foreach (SceneRecord scene in data.Scenes)
            {
                _scenes.TryAdd(scene.Info.SceneId, scene);
            }

            foreach (SceneTargetRecord target in data.Targets)
            {
                _targets.TryAdd(target.Uid, target);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _scenes.Clear();
        _targets.Clear();
        IsLoaded = false;
    }

    public bool IsTarget(uint uid) => _targets.ContainsKey(uid);

    public SceneTargetRecord? GetSceneTarget(uint uid)
    {
        return _targets.TryGetValue(uid, out SceneTargetRecord? target) ? target : null;
    }

    public bool IsData(uint sceneId) => _scenes.ContainsKey(sceneId);

    public SceneRecord? GetSceneData(uint sceneId)
    {
        return _scenes.TryGetValue(sceneId, out SceneRecord? scene) ? scene : null;
    }

    public IReadOnlyList<SceneStepRecord> GetSceneSteps(uint sceneId)
    {
        return _scenes.TryGetValue(sceneId, out SceneRecord? scene) ? scene.Steps : [];
    }
}
