namespace DProject.Data.CsFileTable;

public sealed class CsFileTableMng
{
    private readonly Dictionary<string, IFileTableManager> _managers = new(StringComparer.OrdinalIgnoreCase);

    public string LanguagePath { get; private set; } = "Data\\Bin\\English";
    public bool IsInitialized { get; private set; }
    public eFILE_TYPE FileType { get; private set; } = eFILE_TYPE.FT_FILEPACK;

    public string GetLanguagePath(eLANGUAGE eLanguage)
    {
        return eLanguage switch
        {
            eLANGUAGE.KOREA_TRANSLATION => "Data\\Bin\\Korea_Tr",
            eLANGUAGE.KOREA_ORIGINAL => "Data\\Bin\\Korea_Org",
            eLANGUAGE.ENGLISH => "Data\\Bin\\English",
            eLANGUAGE.ENGLISH_A => "Data\\Bin\\English_A",
            eLANGUAGE.THAILAND => "Data\\Bin\\Thailand",
            eLANGUAGE.TAIWAN => "Data\\Bin\\Taiwan",
            eLANGUAGE.HONGKONG => "Data\\Bin\\HongKong",
            _ => "Data\\Bin\\English"
        };
    }

    public string GetLanguagePath(eLANGUAGE eLanguage, string binRoot)
    {
        string legacyPath = GetLanguagePath(eLanguage);
        string languageFolder = Path.GetFileName(legacyPath);
        return Path.Combine(string.IsNullOrWhiteSpace(binRoot) ? "Data\\Bin" : binRoot, languageFolder);
    }

    public void Delete()
    {
        foreach (IFileTableManager manager in _managers.Values)
        {
            manager.Delete();
        }

        _managers.Clear();
        IsInitialized = false;
    }

    public bool Init(eFILE_TYPE eFileType, eLANGUAGE eLanguage, string binRoot, bool allowMissingTables)
    {
        nsCsFileTable.g_eFileType = eFileType;
        nsCsFileTable.g_eLanguage = eLanguage;
        FileType = eFileType;
        LanguagePath = GetLanguagePath(eLanguage, binRoot);

        RegisterManager("CsTotalExcelMng", true, allowMissingTables);
        RegisterManager("CsDigimonMng", nsCsFileTable.g_bUseDigimon, allowMissingTables);
        RegisterManager("CsMonsterMng", nsCsFileTable.g_bUseMonster, allowMissingTables);
        RegisterManager("CsMapMng", nsCsFileTable.g_bUseMap, allowMissingTables);
        RegisterManager("CsNpcMng", nsCsFileTable.g_bUseNpc, allowMissingTables);
        RegisterManager("CsQuestMng", nsCsFileTable.g_bUseQuest, allowMissingTables);
        RegisterManager("CsItemMng", nsCsFileTable.g_bUseItem, allowMissingTables);
        RegisterManager("CsBaseMng", nsCsFileTable.g_bUseBase, allowMissingTables);
        RegisterManager("CsSkillMng", nsCsFileTable.g_bUseSkill, allowMissingTables);
        RegisterManager("CsNatureMng", nsCsFileTable.g_bUseNature, allowMissingTables);
        RegisterManager("CsCuidMng", nsCsFileTable.g_bUseCuid, allowMissingTables);
        RegisterManager("CsAddExpMng", true, allowMissingTables);
        RegisterManager("CsCashShopMng", nsCsFileTable.g_bUseCashShop, allowMissingTables);
        RegisterManager("CsAchieveMng", nsCsFileTable.g_bUseAchieve, allowMissingTables);
        RegisterManager("CsSceneDataMng", nsCsFileTable.g_bSceneDataMng, allowMissingTables);
        RegisterManager("CsBuffMng", nsCsFileTable.g_bBuffMng, allowMissingTables);
        RegisterManager("CsGotchaMng", nsCsFileTable.g_bUseGotcha, allowMissingTables);
        RegisterManager("CDigimonEvolution", true, allowMissingTables);
        RegisterManager("CsTalkMng", true, allowMissingTables);
        RegisterManager("CsTacticsMng", true, allowMissingTables);
        RegisterManager("CsWorldMapMng", true, allowMissingTables);
        RegisterManager("CsTamerMng", true, allowMissingTables);
        RegisterManager("CsHelpMng", nsCsFileTable.g_bUseHelp, allowMissingTables);
        RegisterManager("CsMoveObjectMng", nsCsFileTable.g_bUseMoveObject, allowMissingTables);

        IsInitialized = true;
        return true;
    }

    public void SaveBin(eLANGUAGE eLanguage)
    {
        nsCsFileTable.g_eLanguage = eLanguage;
    }

    public void StringChange(eLANGUAGE eLanguage)
    {
        nsCsFileTable.g_eLanguage = eLanguage;
    }

    public IReadOnlyCollection<string> GetLoadedManagerNames() => _managers.Keys.ToArray();

    public IReadOnlyCollection<BinFileSnapshot> GetLoadedSnapshots()
    {
        var snapshots = new List<BinFileSnapshot>();
        foreach (IFileTableManager manager in _managers.Values)
        {
            if (manager is ICompositeBinFileTableManager composite)
            {
                snapshots.AddRange(composite.LoadResults
                    .Select(result => result.Snapshot)
                    .Where(snapshot => snapshot is not null)
                    .Cast<BinFileSnapshot>());
                continue;
            }

            if (manager is IBinFileTableManager binManager &&
                binManager.LoadResult?.Snapshot is BinFileSnapshot snapshot)
            {
                snapshots.Add(snapshot);
            }
        }

        return snapshots;
    }

    public CsSkillMng? SkillMng => _managers.TryGetValue("CsSkillMng", out IFileTableManager? manager)
        ? manager as CsSkillMng
        : null;

    public CsDigimonMng? DigimonMng => _managers.TryGetValue("CsDigimonMng", out IFileTableManager? manager)
        ? manager as CsDigimonMng
        : null;

    public CsItemMng? ItemMng => _managers.TryGetValue("CsItemMng", out IFileTableManager? manager)
        ? manager as CsItemMng
        : null;

    public CsMonsterMng? MonsterMng => _managers.TryGetValue("CsMonsterMng", out IFileTableManager? manager)
        ? manager as CsMonsterMng
        : null;

    public CsMapMng? MapMng => _managers.TryGetValue("CsMapMng", out IFileTableManager? manager)
        ? manager as CsMapMng
        : null;

    public CsNpcMng? NpcMng => _managers.TryGetValue("CsNpcMng", out IFileTableManager? manager)
        ? manager as CsNpcMng
        : null;

    public CsQuestMng? QuestMng => _managers.TryGetValue("CsQuestMng", out IFileTableManager? manager)
        ? manager as CsQuestMng
        : null;

    public CsBaseMng? BaseMng => _managers.TryGetValue("CsBaseMng", out IFileTableManager? manager)
        ? manager as CsBaseMng
        : null;

    public CsNatureMng? NatureMng => _managers.TryGetValue("CsNatureMng", out IFileTableManager? manager)
        ? manager as CsNatureMng
        : null;

    public CsCuidMng? CuidMng => _managers.TryGetValue("CsCuidMng", out IFileTableManager? manager)
        ? manager as CsCuidMng
        : null;

    public CsWorldMapMng? WorldMapMng => _managers.TryGetValue("CsWorldMapMng", out IFileTableManager? manager)
        ? manager as CsWorldMapMng
        : null;

    public CsBuffMng? BuffMng => _managers.TryGetValue("CsBuffMng", out IFileTableManager? manager)
        ? manager as CsBuffMng
        : null;

    public CsCashShopMng? CashShopMng => _managers.TryGetValue("CsCashShopMng", out IFileTableManager? manager)
        ? manager as CsCashShopMng
        : null;

    public CsTalkMng? TalkMng => _managers.TryGetValue("CsTalkMng", out IFileTableManager? manager)
        ? manager as CsTalkMng
        : null;

    public CsSceneDataMng? SceneDataMng => _managers.TryGetValue("CsSceneDataMng", out IFileTableManager? manager)
        ? manager as CsSceneDataMng
        : null;

    public CsMoveObjectMng? MoveObjectMng => _managers.TryGetValue("CsMoveObjectMng", out IFileTableManager? manager)
        ? manager as CsMoveObjectMng
        : null;

    public CsHelpMng? HelpMng => _managers.TryGetValue("CsHelpMng", out IFileTableManager? manager)
        ? manager as CsHelpMng
        : null;

    public CsTotalExcelMng? TotalExcelMng => _managers.TryGetValue("CsTotalExcelMng", out IFileTableManager? manager)
        ? manager as CsTotalExcelMng
        : null;

    public CsAddExpMng? AddExpMng => _managers.TryGetValue("CsAddExpMng", out IFileTableManager? manager)
        ? manager as CsAddExpMng
        : null;

    public CsAchieveMng? AchieveMng => _managers.TryGetValue("CsAchieveMng", out IFileTableManager? manager)
        ? manager as CsAchieveMng
        : null;

    public CsGotchaMng? GotchaMng => _managers.TryGetValue("CsGotchaMng", out IFileTableManager? manager)
        ? manager as CsGotchaMng
        : null;

    public CDigimonEvolution? DigimonEvolution => _managers.TryGetValue("CDigimonEvolution", out IFileTableManager? manager)
        ? manager as CDigimonEvolution
        : null;

    public CsTacticsMng? TacticsMng => _managers.TryGetValue("CsTacticsMng", out IFileTableManager? manager)
        ? manager as CsTacticsMng
        : null;

    public CsTamerMng? TamerMng => _managers.TryGetValue("CsTamerMng", out IFileTableManager? manager)
        ? manager as CsTamerMng
        : null;

    private void RegisterManager(string name, bool enabled, bool allowMissingTables)
    {
        if (!enabled || _managers.ContainsKey(name))
        {
            return;
        }

        IFileTableManager manager = CreateManager(name, allowMissingTables);
        if (!manager.Init())
        {
            throw new InvalidOperationException($"FileTable manager failed: {name}");
        }

        _managers[name] = manager;
    }

    private IFileTableManager CreateManager(string name, bool allowMissingTables)
    {
        return name switch
        {
            "CDigimonEvolution" => new CDigimonEvolution(FileType, LanguagePath, allowMissingTables),
            "CsAddExpMng" => new CsAddExpMng(FileType, LanguagePath, allowMissingTables),
            "CsAchieveMng" => new CsAchieveMng(FileType, LanguagePath, allowMissingTables),
            "CsBaseMng" => new CsBaseMng(FileType, LanguagePath, allowMissingTables),
            "CsBuffMng" => new CsBuffMng(FileType, LanguagePath, allowMissingTables),
            "CsCashShopMng" => new CsCashShopMng(FileType, LanguagePath, allowMissingTables),
            "CsCuidMng" => new CsCuidMng(FileType, LanguagePath, allowMissingTables),
            "CsDigimonMng" => new CsDigimonMng(FileType, LanguagePath, allowMissingTables),
            "CsGotchaMng" => new CsGotchaMng(FileType, LanguagePath, allowMissingTables),
            "CsHelpMng" => new CsHelpMng(FileType, LanguagePath, allowMissingTables),
            "CsItemMng" => new CsItemMng(FileType, LanguagePath, allowMissingTables),
            "CsMapMng" => new CsMapMng(FileType, LanguagePath, allowMissingTables),
            "CsMonsterMng" => new CsMonsterMng(FileType, LanguagePath, allowMissingTables),
            "CsMoveObjectMng" => new CsMoveObjectMng(FileType, LanguagePath, allowMissingTables),
            "CsNatureMng" => new CsNatureMng(FileType, LanguagePath, allowMissingTables),
            "CsNpcMng" => new CsNpcMng(FileType, LanguagePath, allowMissingTables),
            "CsQuestMng" => new CsQuestMng(FileType, LanguagePath, allowMissingTables),
            "CsSceneDataMng" => new CsSceneDataMng(FileType, LanguagePath, allowMissingTables),
            "CsSkillMng" => new CsSkillMng(FileType, LanguagePath, allowMissingTables),
            "CsTalkMng" => new CsTalkMng(FileType, LanguagePath, allowMissingTables),
            "CsTacticsMng" => new CsTacticsMng(FileType, LanguagePath, allowMissingTables),
            "CsTamerMng" => new CsTamerMng(FileType, LanguagePath, allowMissingTables),
            "CsTotalExcelMng" => new CsTotalExcelMng(FileType, LanguagePath, allowMissingTables),
            "CsWorldMapMng" => new CsWorldMapMng(FileType, LanguagePath, allowMissingTables),
            _ => new PlaceholderFileTableManager(name, FileType, LanguagePath, allowMissingTables)
        };
    }
}

public interface IFileTableManager
{
    string Name { get; }
    bool Init();
    void Delete();
}

public interface IBinFileTableManager : IFileTableManager
{
    BinLoadResult? LoadResult { get; }
}

public interface ICompositeBinFileTableManager : IFileTableManager
{
    IReadOnlyCollection<BinLoadResult> LoadResults { get; }
}

public sealed class PlaceholderFileTableManager : IBinFileTableManager
{
    private readonly IBinLoader _binLoader;
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;

    public PlaceholderFileTableManager(string name, eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        Name = name;
        _binLoader = BinLoaderFactory.Create(name);
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name { get; }
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }

    public bool Init()
    {
        LoadResult = _binLoader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        IsLoaded = false;
    }
}
