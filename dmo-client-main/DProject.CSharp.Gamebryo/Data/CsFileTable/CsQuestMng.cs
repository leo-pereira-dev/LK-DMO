namespace DProject.Data.CsFileTable;

public sealed class CsQuestMng : IBinFileTableManager
{
    private readonly QuestBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, QuestBinRecord> _quests = new();
    private readonly Dictionary<uint, List<uint>> _questsByType = new();
    private readonly Dictionary<uint, List<uint>> _questsByStartTarget = new();

    public CsQuestMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsQuestMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, QuestBinRecord> Quests => _quests;
    public IReadOnlyDictionary<uint, List<uint>> QuestsByType => _questsByType;
    public IReadOnlyDictionary<uint, List<uint>> QuestsByStartTarget => _questsByStartTarget;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is QuestBinData data)
        {
            foreach (QuestBinRecord quest in data.Quests)
            {
                if (_quests.TryAdd(quest.QuestId, quest))
                {
                    AddToGroup(_questsByType, quest.QuestType, quest.QuestId);
                    AddToGroup(_questsByStartTarget, quest.StartTargetId, quest.QuestId);
                }
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _quests.Clear();
        _questsByType.Clear();
        _questsByStartTarget.Clear();
        IsLoaded = false;
    }

    public bool IsQuest(uint questId) => _quests.ContainsKey(questId);

    public QuestBinRecord? GetQuest(uint questId)
    {
        return _quests.TryGetValue(questId, out QuestBinRecord? quest) ? quest : null;
    }

    public uint GetLastUniqID()
    {
        return _quests.Count == 0 ? 0 : _quests.Keys.Max();
    }

    public IReadOnlyList<uint> GetQuestListFormType(uint questType)
    {
        return _questsByType.TryGetValue(questType, out List<uint>? quests) ? quests : [];
    }

    public IReadOnlyList<uint> GetQuestListFromStartTarget(uint startTargetId)
    {
        return _questsByStartTarget.TryGetValue(startTargetId, out List<uint>? quests) ? quests : [];
    }

    private static void AddToGroup(Dictionary<uint, List<uint>> groups, uint key, uint questId)
    {
        if (!groups.TryGetValue(key, out List<uint>? group))
        {
            group = [];
            groups[key] = group;
        }

        group.Add(questId);
    }
}
