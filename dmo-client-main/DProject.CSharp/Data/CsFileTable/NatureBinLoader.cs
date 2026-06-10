using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record NatureBinData(
    IReadOnlyList<NatureRecord> Natures,
    IReadOnlyDictionary<int, IReadOnlyDictionary<int, IReadOnlyList<BaseElementPointRecord>>> CompareElements);

public sealed record NatureRecord(
    ushort NatureId,
    IReadOnlyList<short> Elements);

public sealed record BaseElementPointRecord(
    int TargetType,
    int Point);

public sealed class NatureBinLoader : IBinLoader
{
    private const int NatureElementCount = 12;
    private const int NatureInfoSize = 26;
    private const int BaseElementPointSize = 8;
    private const int MaxReasonableCount = 100000;
    private const int MaxReasonableSubCount = 10000;

    public string TableName => "CsNatureMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        BinFileSnapshot snapshot = CountPrefixedBinInspector.Inspect(TableName, binPath, data);
        if (!TryParse(data, out NatureBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out NatureBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!TryReadNatures(reader, out List<NatureRecord>? natures) || natures is null)
        {
            return false;
        }

        var compareElements = new Dictionary<int, IReadOnlyDictionary<int, IReadOnlyList<BaseElementPointRecord>>>();
        if (!reader.EndOfData && !TryReadBaseElements(reader, compareElements))
        {
            return false;
        }

        parsed = new NatureBinData(natures, compareElements);
        return true;
    }

    private static bool TryReadNatures(SafeBinaryReader reader, out List<NatureRecord>? natures)
    {
        natures = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * NatureInfoSize > reader.Remaining)
        {
            return false;
        }

        natures = new List<NatureRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt16(out ushort natureId))
            {
                return false;
            }

            var elements = new List<short>(NatureElementCount);
            for (int elementIndex = 0; elementIndex < NatureElementCount; elementIndex++)
            {
                if (!reader.TryReadInt16(out short elementValue))
                {
                    return false;
                }

                elements.Add(elementValue);
            }

            natures.Add(new NatureRecord(natureId, elements));
        }

        return true;
    }

    private static bool TryReadBaseElements(
        SafeBinaryReader reader,
        Dictionary<int, IReadOnlyDictionary<int, IReadOnlyList<BaseElementPointRecord>>> compareElements)
    {
        if (!TryReadCount(reader, MaxReasonableSubCount, out int compareTypeCount))
        {
            return false;
        }

        for (int i = 0; i < compareTypeCount; i++)
        {
            if (!reader.TryReadInt32(out int compareType) ||
                !TryReadCount(reader, MaxReasonableSubCount, out int baseTypeCount))
            {
                return false;
            }

            var baseElements = new Dictionary<int, IReadOnlyList<BaseElementPointRecord>>(baseTypeCount);
            for (int baseIndex = 0; baseIndex < baseTypeCount; baseIndex++)
            {
                if (!reader.TryReadInt32(out int baseType) ||
                    !TryReadCount(reader, MaxReasonableSubCount, out int pointCount) ||
                    (long)pointCount * BaseElementPointSize > reader.Remaining)
                {
                    return false;
                }

                var points = new List<BaseElementPointRecord>(pointCount);
                for (int pointIndex = 0; pointIndex < pointCount; pointIndex++)
                {
                    if (!reader.TryReadInt32(out int targetType) ||
                        !reader.TryReadInt32(out int point))
                    {
                        return false;
                    }

                    points.Add(new BaseElementPointRecord(targetType, point));
                }

                baseElements.TryAdd(baseType, points);
            }

            compareElements.TryAdd(compareType, baseElements);
        }

        return true;
    }

    private static bool TryReadCount(SafeBinaryReader reader, int maxCount, out int count)
    {
        count = 0;
        if (!reader.TryReadInt32(out int value) || value < 0 || value > maxCount)
        {
            return false;
        }

        count = value;
        return true;
    }
}
