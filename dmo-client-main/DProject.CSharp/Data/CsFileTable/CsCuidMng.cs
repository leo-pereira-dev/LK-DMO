namespace DProject.Data.CsFileTable;

public sealed class CsCuidMng : IBinFileTableManager
{
    private static readonly char[] ReplacementCharacters = ['!', '@', '#', '$', '%', '&', '*'];

    private readonly CuidBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly List<CuidIdRuleRecord> _idRules = [];
    private readonly List<CuidChatRuleRecord> _chatRules = [];

    public CsCuidMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsCuidMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyList<CuidIdRuleRecord> IdRules => _idRules;
    public IReadOnlyList<CuidChatRuleRecord> ChatRules => _chatRules;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is CuidBinData data)
        {
            _idRules.AddRange(data.IdRules);
            _chatRules.AddRange(data.ChatRules.OrderByDescending(rule => rule.Name.Length));
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _idRules.Clear();
        _chatRules.Clear();
        IsLoaded = false;
    }

    public bool CheckID(string? name)
    {
        if (string.IsNullOrWhiteSpace(name))
        {
            return true;
        }

        string normalized = name.ToLowerInvariant();
        foreach (CuidIdRuleRecord rule in _idRules)
        {
            switch (rule.MessageType)
            {
                case CuidMessageType.Contains:
                    if (normalized.Contains(rule.Name, StringComparison.Ordinal))
                    {
                        return false;
                    }
                    break;

                case CuidMessageType.Only:
                    if (string.Equals(normalized, rule.Name, StringComparison.Ordinal))
                    {
                        return false;
                    }
                    break;
            }
        }

        return !_chatRules.Any(rule => normalized.Contains(rule.Name, StringComparison.Ordinal));
    }

    public string CheckChat(string? text)
    {
        if (string.IsNullOrEmpty(text))
        {
            return string.Empty;
        }

        string lowered = text.ToLowerInvariant();
        var ranges = new List<(int Position, int Length)>();
        foreach (CuidChatRuleRecord rule in _chatRules)
        {
            int nextPosition = 0;
            while (nextPosition < lowered.Length)
            {
                int found = lowered.IndexOf(rule.Name, nextPosition, StringComparison.Ordinal);
                if (found < 0)
                {
                    break;
                }

                ranges.Add((found, rule.Name.Length));
                nextPosition = found + rule.Name.Length;
            }
        }

        if (ranges.Count == 0)
        {
            return text;
        }

        char[] chars = text.ToCharArray();
        foreach ((int position, int length) in ranges)
        {
            int end = Math.Min(position + length, chars.Length);
            for (int i = position; i < end; i++)
            {
                chars[i] = ReplacementCharacters[Random.Shared.Next(ReplacementCharacters.Length)];
            }
        }

        return new string(chars);
    }
}
