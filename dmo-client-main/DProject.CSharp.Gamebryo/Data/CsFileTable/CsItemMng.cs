namespace DProject.Data.CsFileTable;

public sealed class CsItemMng : IBinFileTableManager
{
    private readonly ItemBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, ItemBinRecord> _items = new();
    private readonly Dictionary<uint, ItemCategoryRecord> _categories = new();
    private readonly Dictionary<uint, ItemCoolTimeRecord> _coolTimes = new();
    private readonly Dictionary<uint, uint> _displayTypes = new();
    private readonly Dictionary<uint, string> _typeNames = new();
    private readonly Dictionary<int, ItemRankRecord> _ranks = new();

    public CsItemMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsItemMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<uint, ItemBinRecord> Items => _items;
    public IReadOnlyDictionary<uint, ItemCategoryRecord> Categories => _categories;
    public IReadOnlyDictionary<uint, ItemCoolTimeRecord> CoolTimes => _coolTimes;
    public IReadOnlyDictionary<uint, uint> DisplayTypes => _displayTypes;
    public IReadOnlyDictionary<uint, string> TypeNames => _typeNames;
    public IReadOnlyDictionary<int, ItemRankRecord> Ranks => _ranks;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is ItemBinData data)
        {
            foreach (ItemBinRecord item in data.Items)
            {
                _items.TryAdd(item.ItemId, item);
            }

            foreach (ItemCategoryRecord category in data.Categories)
            {
                _categories.TryAdd(category.SellClass, category);
            }

            foreach (ItemCoolTimeRecord coolTime in data.CoolTimes)
            {
                _coolTimes.TryAdd(coolTime.GroupId, coolTime);
            }

            foreach (KeyValuePair<uint, uint> displayType in data.DisplayTypes)
            {
                _displayTypes.TryAdd(displayType.Key, displayType.Value);
            }

            foreach (KeyValuePair<uint, string> typeName in data.TypeNames)
            {
                _typeNames.TryAdd(typeName.Key, typeName.Value);
            }

            foreach (KeyValuePair<int, ItemRankRecord> rank in data.Ranks)
            {
                _ranks.TryAdd(rank.Key, rank.Value);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _items.Clear();
        _categories.Clear();
        _coolTimes.Clear();
        _displayTypes.Clear();
        _typeNames.Clear();
        _ranks.Clear();
        IsLoaded = false;
    }

    public ItemBinRecord? GetItem(uint itemId)
    {
        return _items.TryGetValue(itemId, out ItemBinRecord? item) ? item : null;
    }

    public bool IsItem(uint itemId)
    {
        return _items.ContainsKey(itemId);
    }

    public string GetItemName(uint itemId)
    {
        return GetItem(itemId)?.Name ?? string.Empty;
    }

    public string? GetCategory(uint categoryId)
    {
        return _categories.TryGetValue(categoryId, out ItemCategoryRecord? category)
            ? category.Name
            : null;
    }

    public ItemCoolTimeRecord? GetCoolTime(uint groupId)
    {
        return _coolTimes.TryGetValue(groupId, out ItemCoolTimeRecord? coolTime) ? coolTime : null;
    }

    public bool IsCoolTime(uint groupId)
    {
        return _coolTimes.ContainsKey(groupId);
    }

    public int GetDropCount(int itemId)
    {
        return _ranks.TryGetValue(itemId, out ItemRankRecord? rank) ? rank.DropCount : 0;
    }

    public uint TypeT_to_Disp(uint typeT)
    {
        return _displayTypes.TryGetValue(typeT, out uint displayId) ? displayId : 0;
    }

    public uint TypeLS_to_Disp(uint typeL, uint typeS)
    {
        return TypeT_to_Disp((typeL * 100) + typeS);
    }

    public string? GetTypeName(uint typeS)
    {
        return _typeNames.TryGetValue(typeS, out string? typeName) ? typeName : null;
    }
}
