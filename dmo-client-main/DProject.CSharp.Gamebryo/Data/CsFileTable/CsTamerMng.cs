namespace DProject.Data.CsFileTable;

public sealed class CsTamerMng : IBinFileTableManager
{
    private readonly TamerBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, TamerBinRecord> _tamers = new();
    private readonly Dictionary<uint, TamerEmotionRecord> _emotions = new();

    public CsTamerMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsTamerMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, TamerBinRecord> TamerMap => _tamers;
    public IReadOnlyDictionary<uint, TamerEmotionRecord> EmotionMap => _emotions;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is TamerBinData data)
        {
            foreach (TamerBinRecord tamer in data.Tamers)
            {
                _tamers.TryAdd(tamer.TamerId, tamer);
            }

            foreach (TamerEmotionRecord emotion in data.Emotions)
            {
                _emotions.TryAdd(emotion.Id, emotion);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _tamers.Clear();
        _emotions.Clear();
        IsLoaded = false;
    }

    public bool IsTamer(uint tamerId) => _tamers.ContainsKey(tamerId);

    public bool IsTamer(int tamerId) => tamerId >= 0 && IsTamer((uint)tamerId);

    public TamerBinRecord? GetTamer(uint tamerId)
    {
        return _tamers.TryGetValue(tamerId, out TamerBinRecord? tamer) ? tamer : null;
    }

    public TamerBinRecord? GetTamer(int tamerId)
    {
        return tamerId >= 0 ? GetTamer((uint)tamerId) : null;
    }

    public bool IsEmotion(int id) => id >= 0 && _emotions.ContainsKey((uint)id);

    public TamerEmotionRecord? GetEmotion(int id)
    {
        return id >= 0 && _emotions.TryGetValue((uint)id, out TamerEmotionRecord? emotion)
            ? emotion
            : null;
    }

    public TamerEmotionRecord? FindEmotion(string command, out int findIndex)
    {
        findIndex = -1;
        string normalizedCommand = NormalizeCommand(command);
        if (string.IsNullOrWhiteSpace(normalizedCommand))
        {
            return null;
        }

        foreach (TamerEmotionRecord emotion in _emotions.Values)
        {
            int commandCount = Math.Min(emotion.UseCommandCount, (byte)emotion.Commands.Count);
            for (int i = 0; i < commandCount; i++)
            {
                if (NormalizeCommand(emotion.Commands[i]).Equals(normalizedCommand, StringComparison.OrdinalIgnoreCase))
                {
                    findIndex = i;
                    return emotion;
                }
            }
        }

        return null;
    }

    private static string NormalizeCommand(string command)
    {
        string normalized = command.Trim();
        return normalized.StartsWith("/", StringComparison.Ordinal)
            ? normalized[1..]
            : normalized;
    }
}
