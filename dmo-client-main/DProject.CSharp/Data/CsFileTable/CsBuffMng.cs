namespace DProject.Data.CsFileTable;

public sealed class CsBuffMng : IBinFileTableManager
{
    private readonly BuffBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<ushort, BuffRecord> _buffs = new();

    public CsBuffMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsBuffMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<ushort, BuffRecord> Buffs => _buffs;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is BuffBinData data)
        {
            foreach (BuffRecord buff in data.Buffs)
            {
                _buffs.TryAdd(buff.BuffId, buff);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _buffs.Clear();
        IsLoaded = false;
    }

    public bool IsBuff(ushort id) => _buffs.ContainsKey(id);

    public BuffRecord? GetBuff(ushort id)
    {
        return _buffs.TryGetValue(id, out BuffRecord? buff) ? buff : null;
    }

    public IReadOnlyDictionary<ushort, BuffRecord> GetBuffMap()
    {
        return _buffs;
    }
}
