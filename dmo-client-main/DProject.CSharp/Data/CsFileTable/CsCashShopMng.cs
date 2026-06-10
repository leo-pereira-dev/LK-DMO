namespace DProject.Data.CsFileTable;

public sealed class CsCashShopMng : IBinFileTableManager
{
    public const int DefaultTable = 0;
    public const int SteamTable = 1;

    private readonly CashShopBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<int, IReadOnlyDictionary<int, CashShopMainCategoryRecord>> _cashshopData = new();
    private readonly Dictionary<int, IReadOnlyList<CashShopWebDataRecord>> _webData = new();

    public CsCashShopMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsCashShopMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<int, IReadOnlyDictionary<int, CashShopMainCategoryRecord>> CashShopData => _cashshopData;
    public IReadOnlyDictionary<int, IReadOnlyList<CashShopWebDataRecord>> WebData => _webData;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is CashShopBinData data)
        {
            foreach (KeyValuePair<int, IReadOnlyDictionary<int, CashShopMainCategoryRecord>> table in data.Tables)
            {
                _cashshopData[table.Key] = table.Value;
            }

            foreach (KeyValuePair<int, IReadOnlyList<CashShopWebDataRecord>> table in data.WebData)
            {
                _webData[table.Key] = table.Value;
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _cashshopData.Clear();
        _webData.Clear();
        IsLoaded = false;
    }

    public static int GetLargeType(uint productId) => (int)(productId / 10000000);

    public static int GetSelCashType(uint productId) => (int)((productId / 1000000) % 10);

    public static int GetSmallType(uint productId) => (int)((productId / 10000) % 100);

    public IReadOnlyDictionary<int, CashShopMainCategoryRecord>? GetCashShopInfo(int tableType = DefaultTable)
    {
        return _cashshopData.TryGetValue(tableType, out IReadOnlyDictionary<int, CashShopMainCategoryRecord>? table)
            ? table
            : null;
    }

    public IReadOnlyList<CashShopWebDataRecord>? GetCashshopWebData(int tableType = DefaultTable)
    {
        return _webData.TryGetValue(tableType, out IReadOnlyList<CashShopWebDataRecord>? table)
            ? table
            : null;
    }

    public IReadOnlyList<CashShopProductGroupRecord>? GetProductGroups(
        int tableType,
        int mainType,
        int subType)
    {
        return _cashshopData.TryGetValue(tableType, out IReadOnlyDictionary<int, CashShopMainCategoryRecord>? table) &&
            table.TryGetValue(mainType, out CashShopMainCategoryRecord? mainCategory) &&
            mainCategory.SubCategories.TryGetValue(subType, out CashShopSubCategoryRecord? subCategory)
                ? subCategory.ProductGroups
                : null;
    }

    public CashShopProductRecord? GetCashShop(int tableType, uint productId)
    {
        if (!_cashshopData.TryGetValue(tableType, out IReadOnlyDictionary<int, CashShopMainCategoryRecord>? table))
        {
            return null;
        }

        foreach (CashShopMainCategoryRecord mainCategory in table.Values)
        {
            foreach (CashShopSubCategoryRecord subCategory in mainCategory.SubCategories.Values)
            {
                foreach (CashShopProductGroupRecord group in subCategory.ProductGroups)
                {
                    CashShopProductRecord? product = group.Products.FirstOrDefault(item => item.ProductId == productId);
                    if (product is not null)
                    {
                        return product;
                    }
                }
            }
        }

        return null;
    }
}
