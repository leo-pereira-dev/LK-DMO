namespace DProject.Data.CsFileTable;

public sealed class CsTalkMng : IBinFileTableManager
{
    private readonly TalkBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, TalkDigimonRecord> _digimonTalks = new();
    private readonly Dictionary<uint, TalkEventRecord> _eventTalks = new();
    private readonly Dictionary<uint, TalkMessageRecord> _messages = new();
    private readonly Dictionary<uint, TalkTipRecord> _tips = new();
    private readonly Dictionary<uint, TalkLoadingTipRecord> _loadingTips = new();
    private readonly Dictionary<uint, TalkTutorialRecord> _tutorials = new();

    public CsTalkMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsTalkMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, TalkDigimonRecord> DigimonTalks => _digimonTalks;
    public IReadOnlyDictionary<uint, TalkEventRecord> EventTalks => _eventTalks;
    public IReadOnlyDictionary<uint, TalkMessageRecord> Messages => _messages;
    public IReadOnlyDictionary<uint, TalkTipRecord> Tips => _tips;
    public IReadOnlyDictionary<uint, TalkLoadingTipRecord> LoadingTips => _loadingTips;
    public IReadOnlyDictionary<uint, TalkTutorialRecord> Tutorials => _tutorials;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is TalkBinData data)
        {
            foreach (TalkDigimonRecord talk in data.DigimonTalks)
            {
                _digimonTalks.TryAdd(talk.TalkId, talk);
            }

            foreach (TalkEventRecord talk in data.EventTalks)
            {
                _eventTalks.TryAdd(talk.TalkId, talk);
            }

            foreach (TalkMessageRecord message in data.Messages)
            {
                _messages.TryAdd(message.MessageId, message);
            }

            foreach (TalkTipRecord tip in data.Tips)
            {
                _tips.TryAdd(tip.TipId, tip);
            }

            foreach (TalkLoadingTipRecord tip in data.LoadingTips)
            {
                _loadingTips.TryAdd(tip.TipId, tip);
            }

            foreach (TalkTutorialRecord tutorial in data.Tutorials)
            {
                _tutorials.TryAdd(tutorial.TutorialId, tutorial);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _digimonTalks.Clear();
        _eventTalks.Clear();
        _messages.Clear();
        _tips.Clear();
        _loadingTips.Clear();
        _tutorials.Clear();
        IsLoaded = false;
    }

    public TalkDigimonRecord? GetTalk_Digimon(uint id)
    {
        return _digimonTalks.TryGetValue(id, out TalkDigimonRecord? talk) ? talk : null;
    }

    public bool IsExist_Digimon(uint id) => _digimonTalks.ContainsKey(id);

    public IReadOnlyDictionary<uint, TalkDigimonRecord> GetMap_Digimon() => _digimonTalks;

    public TalkEventRecord? GetTalk_Event(uint id)
    {
        return _eventTalks.TryGetValue(id, out TalkEventRecord? talk) ? talk : null;
    }

    public bool IsExist_Event(uint id) => _eventTalks.ContainsKey(id);

    public IReadOnlyDictionary<uint, TalkEventRecord> GetMap_Event() => _eventTalks;

    public void EventTalk_Reload()
    {
        // Excel reload is intentionally left as a placeholder in the C# port.
    }

    public TalkMessageRecord? GetTalk_Message(uint id)
    {
        return _messages.TryGetValue(id, out TalkMessageRecord? message) ? message : null;
    }

    public bool IsExist_Message(uint id) => _messages.ContainsKey(id);

    public IReadOnlyDictionary<uint, TalkMessageRecord> GetMap_Message() => _messages;

    public TalkTipRecord? GetRandom_Tip()
    {
        if (_tips.Count == 0)
        {
            return null;
        }

        return _tips.Values.ElementAt(Random.Shared.Next(_tips.Count));
    }

    public TalkTipRecord? GetTalk_Tip(uint id)
    {
        return _tips.TryGetValue(id, out TalkTipRecord? tip) ? tip : null;
    }

    public bool IsExist_Tip(uint id) => _tips.ContainsKey(id);

    public IReadOnlyDictionary<uint, TalkTipRecord> GetMap_Tip() => _tips;

    public TalkLoadingTipRecord? GetRandom_LoadingTip(int level)
    {
        if (_loadingTips.Count == 0)
        {
            return null;
        }

        TalkLoadingTipRecord[] eligible = _loadingTips.Values
            .Where(tip => tip.Level < level)
            .ToArray();
        return eligible.Length == 0 ? null : eligible[Random.Shared.Next(eligible.Length)];
    }

    public TalkLoadingTipRecord? GetTalk_LoadingTip(uint id)
    {
        return _loadingTips.TryGetValue(id, out TalkLoadingTipRecord? tip) ? tip : null;
    }

    public bool IsExist_LoadingTip(uint id) => _loadingTips.ContainsKey(id);

    public IReadOnlyDictionary<uint, TalkLoadingTipRecord> GetMap_LoadingTip() => _loadingTips;

    public bool Load_Tutorial()
    {
        return _tutorials.Count > 0 || LoadResult?.Status == BinLoadStatus.Loaded;
    }

    public void Delete_Tutorial()
    {
        _tutorials.Clear();
    }

    public TalkTutorialRecord? GetTalk_Tutorial(uint id)
    {
        return _tutorials.TryGetValue(id, out TalkTutorialRecord? tutorial) ? tutorial : null;
    }

    public bool IsExist_Tutorial(uint id) => _tutorials.ContainsKey(id);
}
