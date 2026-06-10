namespace DProject.Data.CsFileTable;

public sealed class CsNpcMng : IBinFileTableManager
{
    private readonly NpcBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, NpcBinRecord> _npcs = new();
    private readonly Dictionary<uint, List<NpcBinRecord>> _npcGroups = new();
    private readonly Dictionary<uint, NpcModelInfoRecord> _modelInfos = new();
    private readonly Dictionary<uint, NpcEventInfoRecord> _cardGameInfos = new();

    public CsNpcMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsNpcMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, NpcBinRecord> Npcs => _npcs;
    public IReadOnlyDictionary<uint, List<NpcBinRecord>> NpcGroups => _npcGroups;
    public IReadOnlyDictionary<uint, NpcModelInfoRecord> ModelInfos => _modelInfos;
    public IReadOnlyDictionary<uint, NpcEventInfoRecord> CardGameInfos => _cardGameInfos;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is NpcBinData data)
        {
            foreach (NpcBinRecord npc in data.Npcs)
            {
                if (_npcs.TryAdd(npc.NpcId, npc))
                {
                    if (!_npcGroups.TryGetValue(npc.MapId, out List<NpcBinRecord>? group))
                    {
                        group = [];
                        _npcGroups[npc.MapId] = group;
                    }

                    group.Add(npc);
                }
            }

            foreach (KeyValuePair<uint, NpcModelInfoRecord> modelInfo in data.ModelInfos)
            {
                _modelInfos.TryAdd(modelInfo.Key, modelInfo.Value);
            }

            foreach (KeyValuePair<uint, NpcEventInfoRecord> cardGameInfo in data.CardGameInfos)
            {
                _cardGameInfos.TryAdd(cardGameInfo.Key, cardGameInfo.Value);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _npcs.Clear();
        _npcGroups.Clear();
        _modelInfos.Clear();
        _cardGameInfos.Clear();
        IsLoaded = false;
    }

    public NpcBinRecord? GetNpc(uint npcTableId)
    {
        return _npcs.TryGetValue(npcTableId, out NpcBinRecord? npc) ? npc : null;
    }

    public NpcBinRecord? FindNpc(uint modelId)
    {
        return _npcs.Values.FirstOrDefault(npc => npc.ModelId == modelId);
    }

    public IReadOnlyList<uint> FindNpc_FromModelID(uint modelId)
    {
        return _npcs.Values
            .Where(npc => npc.ModelId == modelId)
            .Select(npc => npc.NpcId)
            .ToArray();
    }

    public bool IsNpc(uint npcId)
    {
        return _npcs.ContainsKey(npcId);
    }

    public bool IsGroup(uint mapId)
    {
        return _npcGroups.ContainsKey(mapId);
    }

    public IReadOnlyList<NpcBinRecord> GetGroup(uint mapId)
    {
        return _npcGroups.TryGetValue(mapId, out List<NpcBinRecord>? group)
            ? group
            : [];
    }

    public NpcModelInfoRecord? GetModelInfo(uint modelId)
    {
        return _modelInfos.TryGetValue(modelId, out NpcModelInfoRecord? modelInfo) ? modelInfo : null;
    }

    public NpcEventInfoRecord? GetCardGameInfo(uint npcId)
    {
        return _cardGameInfos.TryGetValue(npcId, out NpcEventInfoRecord? cardGameInfo) ? cardGameInfo : null;
    }
}
