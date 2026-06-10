namespace DProject.Data.CsFileTable;

public enum eBaseElementCompareType
{
    eAtt = 0,
    eDef = 1
}

public enum eBaseElementType
{
    eNo = 0,
    eDa = 1,
    eVa = 2,
    eVi = 3,
    eUn = 4,
    eMax = 5
}

public sealed class CsNatureMng : IBinFileTableManager
{
    public const int ElementEqual = 100;
    public const int NatureTypeIce = 16;

    private readonly NatureBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<int, NatureRecord> _natures = new();
    private readonly Dictionary<int, Dictionary<int, List<BaseElementPointRecord>>> _compareElements = new();

    public CsNatureMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsNatureMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<int, NatureRecord> Natures => _natures;
    public IReadOnlyDictionary<int, Dictionary<int, List<BaseElementPointRecord>>> CompareElements => _compareElements;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is NatureBinData data)
        {
            foreach (NatureRecord nature in data.Natures)
            {
                _natures.TryAdd(nature.NatureId, nature);
            }

            foreach (KeyValuePair<int, IReadOnlyDictionary<int, IReadOnlyList<BaseElementPointRecord>>> compareType in data.CompareElements)
            {
                var baseElements = new Dictionary<int, List<BaseElementPointRecord>>();
                foreach (KeyValuePair<int, IReadOnlyList<BaseElementPointRecord>> baseType in compareType.Value)
                {
                    baseElements[baseType.Key] = baseType.Value.ToList();
                }

                _compareElements[compareType.Key] = baseElements;
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _natures.Clear();
        _compareElements.Clear();
        IsLoaded = false;
    }

    public bool IsNature(int id) => _natures.ContainsKey(id);

    public NatureRecord? GetNature(int id)
    {
        return _natures.TryGetValue(id, out NatureRecord? nature) ? nature : null;
    }

    public int GetValue(int owner, int target)
    {
        if (!_natures.TryGetValue(owner, out NatureRecord? nature))
        {
            return 0;
        }

        int targetIndex = target - NatureTypeIce;
        return targetIndex >= 0 && targetIndex < nature.Elements.Count ? nature.Elements[targetIndex] : 0;
    }

    public bool IsSuperiority(eBaseElementCompareType compareType, eBaseElementType orgType, eBaseElementType srcType)
    {
        return IsSuperiority((int)compareType, (int)orgType, (int)srcType);
    }

    public bool IsSuperiority(int compareType, int orgType, int srcType)
    {
        BaseElementPointRecord? element = GetBaseElement(compareType, orgType, srcType);
        return element is not null && element.Point > ElementEqual;
    }

    public bool IsInferiority(eBaseElementCompareType compareType, eBaseElementType orgType, eBaseElementType srcType)
    {
        return IsInferiority((int)compareType, (int)orgType, (int)srcType);
    }

    public bool IsInferiority(int compareType, int orgType, int srcType)
    {
        BaseElementPointRecord? element = GetBaseElement(compareType, orgType, srcType);
        return element is not null && element.Point < ElementEqual;
    }

    public BaseElementPointRecord? GetBaseElement(int compareType, int orgType, int srcType)
    {
        if (!_compareElements.TryGetValue(compareType, out Dictionary<int, List<BaseElementPointRecord>>? compareMap) ||
            !compareMap.TryGetValue(orgType, out List<BaseElementPointRecord>? points) ||
            srcType < 0 ||
            srcType >= points.Count)
        {
            return null;
        }

        return points[srcType];
    }
}
