using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record CashShopBinData(
    IReadOnlyDictionary<int, IReadOnlyDictionary<int, CashShopMainCategoryRecord>> Tables,
    IReadOnlyDictionary<int, IReadOnlyList<CashShopWebDataRecord>> WebData);

public sealed record CashShopMainCategoryRecord(
    int MainType,
    string CategoryName,
    IReadOnlyDictionary<int, CashShopSubCategoryRecord> SubCategories);

public sealed record CashShopSubCategoryRecord(
    int SubType,
    string CategoryName,
    IReadOnlyList<CashShopProductGroupRecord> ProductGroups);

public sealed record CashShopProductGroupRecord(
    uint ItemGroupId,
    IReadOnlyList<CashShopProductRecord> Products);

public sealed record CashShopProductRecord(
    string Name,
    string Comment,
    bool Active,
    uint ProductId,
    string StartTime,
    string EndTime,
    int PurchaseCashType,
    int StandardSellingPrice,
    int RealSellingPrice,
    int SalePercent,
    int IconId,
    int MaskType,
    int DisplayType,
    int DisplayCount,
    IReadOnlyList<CashShopPackageItemRecord> PackageItems);

public sealed record CashShopPackageItemRecord(uint ItemId, int ItemCount);

public sealed record CashShopWebDataRecord(string WebImageFile, string WebLinkUrl);

public sealed class CashShopBinLoader : IBinLoader
{
    private const int FixedDateStringLength = 64;

    public string TableName => "CsCashShopMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        if (!TryParse(data, binPath, out CashShopBinData? parsed, out BinFileSnapshot snapshot) ||
            parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(
        ReadOnlyMemory<byte> data,
        string binPath,
        out CashShopBinData? parsed,
        out BinFileSnapshot snapshot)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        var sections = new List<BinSectionSnapshot>();

        if (!TryReadCashShopTables(reader, sections, out Dictionary<int, IReadOnlyDictionary<int, CashShopMainCategoryRecord>>? tables) ||
            tables is null ||
            !TryReadWebData(reader, sections, out Dictionary<int, IReadOnlyList<CashShopWebDataRecord>>? webData) ||
            webData is null)
        {
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        bool complete = reader.EndOfData;
        if (!complete)
        {
            sections.Add(new BinSectionSnapshot("TrailingBytes", reader.Position, 0, null, reader.Remaining, false));
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        snapshot = Snapshot(binPath, data.Length, sections);
        parsed = new CashShopBinData(tables, webData);
        return true;
    }

    private static bool TryReadCashShopTables(
        SafeBinaryReader reader,
        List<BinSectionSnapshot> sections,
        out Dictionary<int, IReadOnlyDictionary<int, CashShopMainCategoryRecord>>? tables)
    {
        tables = null;
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int tableCount))
        {
            sections.Add(new BinSectionSnapshot("CashShopTables", offset, 0, null, reader.Position - offset, false));
            return false;
        }

        tables = new Dictionary<int, IReadOnlyDictionary<int, CashShopMainCategoryRecord>>(tableCount);
        for (int i = 0; i < tableCount; i++)
        {
            if (!reader.TryReadInt32(out int tableType) ||
                !MapBinFormat.TryReadCount(reader, out int mainCategoryCount))
            {
                sections.Add(new BinSectionSnapshot("CashShopTables", offset, tableCount, null, reader.Position - offset, false));
                return false;
            }

            var mainCategories = new Dictionary<int, CashShopMainCategoryRecord>(mainCategoryCount);
            for (int j = 0; j < mainCategoryCount; j++)
            {
                if (!TryReadMainCategory(reader, out CashShopMainCategoryRecord? mainCategory) ||
                    mainCategory is null)
                {
                    sections.Add(new BinSectionSnapshot("CashShopTables", offset, tableCount, null, reader.Position - offset, false));
                    return false;
                }

                mainCategories[mainCategory.MainType] = mainCategory;
            }

            tables[tableType] = mainCategories;
        }

        sections.Add(new BinSectionSnapshot("CashShopTables", offset, tableCount, null, reader.Position - offset, true));
        return true;
    }

    private static bool TryReadMainCategory(
        SafeBinaryReader reader,
        out CashShopMainCategoryRecord? category)
    {
        category = null;
        if (!reader.TryReadInt32(out int mainType) ||
            !MapBinFormat.TryReadUtf16String(reader, out string categoryName) ||
            !MapBinFormat.TryReadCount(reader, out int subCategoryCount))
        {
            return false;
        }

        var subCategories = new Dictionary<int, CashShopSubCategoryRecord>(subCategoryCount);
        for (int i = 0; i < subCategoryCount; i++)
        {
            if (!TryReadSubCategory(reader, out CashShopSubCategoryRecord? subCategory) ||
                subCategory is null)
            {
                return false;
            }

            subCategories[subCategory.SubType] = subCategory;
        }

        category = new CashShopMainCategoryRecord(mainType, categoryName, subCategories);
        return true;
    }

    private static bool TryReadSubCategory(
        SafeBinaryReader reader,
        out CashShopSubCategoryRecord? category)
    {
        category = null;
        if (!reader.TryReadInt32(out int subType) ||
            !MapBinFormat.TryReadUtf16String(reader, out string categoryName) ||
            !MapBinFormat.TryReadCount(reader, out int productGroupCount))
        {
            return false;
        }

        var productGroups = new List<CashShopProductGroupRecord>(productGroupCount);
        for (int i = 0; i < productGroupCount; i++)
        {
            if (!TryReadProductGroup(reader, out CashShopProductGroupRecord? productGroup) ||
                productGroup is null)
            {
                return false;
            }

            productGroups.Add(productGroup);
        }

        category = new CashShopSubCategoryRecord(subType, categoryName, productGroups);
        return true;
    }

    private static bool TryReadProductGroup(
        SafeBinaryReader reader,
        out CashShopProductGroupRecord? group)
    {
        group = null;
        if (!reader.TryReadUInt32(out uint itemGroupId) ||
            !MapBinFormat.TryReadCount(reader, out int productCount))
        {
            return false;
        }

        var products = new List<CashShopProductRecord>(productCount);
        for (int i = 0; i < productCount; i++)
        {
            if (!TryReadProduct(reader, out CashShopProductRecord? product) ||
                product is null)
            {
                return false;
            }

            products.Add(product);
        }

        group = new CashShopProductGroupRecord(itemGroupId, products);
        return true;
    }

    private static bool TryReadProduct(
        SafeBinaryReader reader,
        out CashShopProductRecord? product)
    {
        product = null;
        if (!MapBinFormat.TryReadUtf16String(reader, out string name) ||
            !MapBinFormat.TryReadUtf16String(reader, out string comment) ||
            !reader.TryReadBoolean(out bool active) ||
            !reader.TryReadUInt32(out uint productId) ||
            !reader.TryReadBytes(FixedDateStringLength, out ReadOnlyMemory<byte> startTimeBytes) ||
            !reader.TryReadBytes(FixedDateStringLength, out ReadOnlyMemory<byte> endTimeBytes) ||
            !reader.TryReadInt32(out int purchaseCashType) ||
            !reader.TryReadInt32(out int standardSellingPrice) ||
            !reader.TryReadInt32(out int realSellingPrice) ||
            !reader.TryReadInt32(out int salePercent) ||
            !reader.TryReadInt32(out int iconId) ||
            !reader.TryReadInt32(out int maskType) ||
            !reader.TryReadInt32(out int displayType) ||
            !reader.TryReadInt32(out int displayCount) ||
            !MapBinFormat.TryReadCount(reader, out int packageItemCount))
        {
            return false;
        }

        var packageItems = new List<CashShopPackageItemRecord>(packageItemCount);
        for (int i = 0; i < packageItemCount; i++)
        {
            if (!reader.TryReadUInt32(out uint itemId) ||
                !reader.TryReadInt32(out int itemCount))
            {
                return false;
            }

            packageItems.Add(new CashShopPackageItemRecord(itemId, itemCount));
        }

        product = new CashShopProductRecord(
            name,
            comment,
            active,
            productId,
            reader.ReadFixedAnsiString(startTimeBytes.Span),
            reader.ReadFixedAnsiString(endTimeBytes.Span),
            purchaseCashType,
            standardSellingPrice,
            realSellingPrice,
            salePercent,
            iconId,
            maskType,
            displayType,
            displayCount,
            packageItems);
        return true;
    }

    private static bool TryReadWebData(
        SafeBinaryReader reader,
        List<BinSectionSnapshot> sections,
        out Dictionary<int, IReadOnlyList<CashShopWebDataRecord>>? webData)
    {
        webData = null;
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int tableCount))
        {
            sections.Add(new BinSectionSnapshot("CashShopWebData", offset, 0, null, reader.Position - offset, false));
            return false;
        }

        webData = new Dictionary<int, IReadOnlyList<CashShopWebDataRecord>>(tableCount);
        for (int i = 0; i < tableCount; i++)
        {
            if (!reader.TryReadInt32(out int tableType) ||
                !MapBinFormat.TryReadCount(reader, out int listCount))
            {
                sections.Add(new BinSectionSnapshot("CashShopWebData", offset, tableCount, null, reader.Position - offset, false));
                return false;
            }

            var records = new List<CashShopWebDataRecord>(listCount);
            for (int j = 0; j < listCount; j++)
            {
                if (!MapBinFormat.TryReadAnsiString(reader, out string imageFile) ||
                    !MapBinFormat.TryReadAnsiString(reader, out string linkUrl))
                {
                    sections.Add(new BinSectionSnapshot("CashShopWebData", offset, tableCount, null, reader.Position - offset, false));
                    return false;
                }

                records.Add(new CashShopWebDataRecord(imageFile, linkUrl));
            }

            webData[tableType] = records;
        }

        sections.Add(new BinSectionSnapshot("CashShopWebData", offset, tableCount, null, reader.Position - offset, true));
        return true;
    }

    private static BinFileSnapshot Snapshot(
        string path,
        int byteCount,
        IReadOnlyList<BinSectionSnapshot> sections)
    {
        return new BinFileSnapshot("CsCashShopMng", path, byteCount, sections);
    }
}
