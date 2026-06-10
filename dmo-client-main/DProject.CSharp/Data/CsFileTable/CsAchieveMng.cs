namespace DProject.Data.CsFileTable;

public sealed class CsAchieveMng : IBinFileTableManager
{
    private readonly AchieveBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, AchieveRecord> _achieves = new();

    public CsAchieveMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsAchieveMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public AchieveTypeNodeRecord RootType { get; private set; } = new(string.Empty, []);
    public IReadOnlyDictionary<uint, AchieveRecord> Map => _achieves;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is AchieveBinData data)
        {
            RootType = data.RootType;
            foreach (AchieveRecord record in data.Achieves)
            {
                _achieves[record.QuestId] = record;
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _achieves.Clear();
        RootType = new AchieveTypeNodeRecord(string.Empty, []);
        IsLoaded = false;
    }

    public bool IsAchieve(int questId) => _achieves.ContainsKey((uint)questId);

    public AchieveRecord? GetAchieve(int questId)
    {
        return _achieves.TryGetValue((uint)questId, out AchieveRecord? record) ? record : null;
    }
}
