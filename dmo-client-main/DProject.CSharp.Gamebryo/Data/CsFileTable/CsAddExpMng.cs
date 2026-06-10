namespace DProject.Data.CsFileTable;

public sealed class CsAddExpMng : IBinFileTableManager
{
    private readonly AddExpBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, AddExpRecord> _records = new();

    public CsAddExpMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsAddExpMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, AddExpRecord> Records => _records;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is AddExpBinData data)
        {
            foreach (AddExpRecord record in data.Records)
            {
                _records.TryAdd(record.Id, record);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _records.Clear();
        IsLoaded = false;
    }

    public bool IsData(uint id) => _records.ContainsKey(id);

    public AddExpRecord? GetData(uint id)
    {
        return _records.TryGetValue(id, out AddExpRecord? record) ? record : null;
    }

    public AddExpRecord? FindData_FromSkill(uint skillId)
    {
        return _records.Values.FirstOrDefault(record => record.SkillId == skillId);
    }
}
