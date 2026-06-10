using DProject.Data.CsFilePack;

namespace DProject.Data.CsFileTable;

public enum BinLoadStatus
{
    Loaded,
    Missing,
    Placeholder
}

public sealed record BinLoadResult(
    BinLoadStatus Status,
    string Path,
    int ByteCount,
    BinFileSnapshot? Snapshot = null,
    object? ParsedTable = null);

public interface IBinLoader
{
    string TableName { get; }
    BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables);
}

public static class BinLoaderFactory
{
    public static IBinLoader Create(string tableName)
    {
        return tableName switch
        {
            "CDigimonEvolution" => new DigimonEvolutionBinLoader(),
            "CsAddExpMng" => new AddExpBinLoader(),
            "CsAchieveMng" => new AchieveBinLoader(),
            "CsBaseMng" => new BaseBinLoader(),
            "CsBuffMng" => new BuffBinLoader(),
            "CsCashShopMng" => new CashShopBinLoader(),
            "CsCuidMng" => new CuidBinLoader(),
            "CsDigimonMng" => new DigimonBinLoader(),
            "CsGotchaMng" => new GotchaBinLoader(),
            "CsHelpMng" => new HelpBinLoader(),
            "CsItemMng" => new ItemBinLoader(),
            "CsMonsterMng" => new MonsterBinLoader(),
            "CsMoveObjectMng" => new MoveObjectBinLoader(),
            "CsMapCharLightMng" => new MapCharLightBinLoader(),
            "CsMapListMng" => new MapListBinLoader(),
            "CsMapMonsterMng" => new MapMonsterBinLoader(),
            "CsMapNpcMng" => new MapNpcBinLoader(),
            "CsMapObjectMng" => new MapObjectBinLoader(),
            "CsMapPortalMng" => new MapPortalBinLoader(),
            "CsMapRegionMng" => new MapRegionBinLoader(),
            "CsMapTableObjMng" => new MapTableObjBinLoader(),
            "CsNatureMng" => new NatureBinLoader(),
            "CsNpcMng" => new NpcBinLoader(),
            "CsQuestMng" => new QuestBinLoader(),
            "CsSceneDataMng" => new SceneDataBinLoader(),
            "CsSkillMng" => new SkillBinLoader(),
            "CsTalkMng" => new TalkBinLoader(),
            "CsTacticsMng" => new TacticsBinLoader(),
            "CsTamerMng" => new TamerBinLoader(),
            "CsTotalExcelMng" => new TotalExcelBinLoader(),
            "CsWeatherMng" => new WeatherBinLoader(),
            "CsWorldMapMng" => new WorldMapBinLoader(),
            _ => new PlaceholderBinLoader(tableName)
        };
    }
}

internal static class BinDataSource
{
    public static bool TryRead(string binPath, eFILE_TYPE fileType, out byte[] data)
    {
        if (fileType == eFILE_TYPE.FT_BIN)
        {
            if (File.Exists(binPath))
            {
                data = File.ReadAllBytes(binPath);
                return true;
            }

            data = [];
            return false;
        }

        return CsFPSystem.TryGetFileData(binPath, out data);
    }
}

public sealed class PlaceholderBinLoader : IBinLoader
{
    public PlaceholderBinLoader(string tableName)
    {
        TableName = tableName;
    }

    public string TableName { get; }

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            BinFileSnapshot snapshot = CountPrefixedBinInspector.Inspect(TableName, binPath, data);
            return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot);
        }

        return allowMissingTables
            ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
            : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
    }
}

public static class TableFileNameCatalog
{
    private static readonly Dictionary<string, string> KnownBinFiles = new(StringComparer.OrdinalIgnoreCase)
    {
        ["CsBaseMng"] = "DMBase.bin",
        ["CsItemMng"] = "ItemList.bin",
        ["CsSkillMng"] = "Skill.bin",
        ["CsTamerMng"] = "TamerList.bin",
        ["CsDigimonMng"] = "Digimon_List.bin",
        ["CsMonsterMng"] = "Monster.bin",
        ["CsMoveObjectMng"] = "Data\\MO.bin",
        ["CsNpcMng"] = "Npc.bin",
        ["CsMapMng"] = "Map.bin",
        ["CsMapListMng"] = "MapList.bin",
        ["CsMapMonsterMng"] = "MapMonsterList.bin",
        ["CsMapNpcMng"] = "MapNpc.bin",
        ["CsMapObjectMng"] = "MapObject.bin",
        ["CsMapPortalMng"] = "MapPortal.bin",
        ["CsMapRegionMng"] = "MapRegion.bin",
        ["CsMapCharLightMng"] = "MapCharLight.bin",
        ["CsMapTableObjMng"] = "Data\\Map\\TableObj.bin",
        ["CsWeatherMng"] = "Weather.bin",
        ["CsQuestMng"] = "Quest.bin",
        ["CsNatureMng"] = "Nature.bin",
        ["CsCuidMng"] = "Cuid.bin",
        ["CsCashShopMng"] = "CashShop.bin",
        ["CsAchieveMng"] = "Achieve.bin",
        ["CsBuffMng"] = "Buff.bin",
        ["CsSceneDataMng"] = "Scene.bin",
        ["CsGotchaMng"] = "Gotcha.bin",
        ["CDigimonEvolution"] = "DigimonEvo.bin",
        ["CsHelpMng"] = "Data\\Interface\\Help\\HelpEng.bin",
        ["CsWorldMapMng"] = "WorldMap.bin",
        ["CsAddExpMng"] = "AddExp.bin",
        ["CsTalkMng"] = "Talk.bin",
        ["CsTacticsMng"] = "Tactics.bin",
        ["CsTotalExcelMng"] = "GameData\\English\\Total.bin"
    };

    public static string GetBinFileName(string managerName)
    {
        if (KnownBinFiles.TryGetValue(managerName, out string? fileName))
        {
            return fileName;
        }

        string trimmed = managerName;
        if (trimmed.StartsWith("Cs", StringComparison.OrdinalIgnoreCase))
        {
            trimmed = trimmed[2..];
        }

        if (trimmed.EndsWith("Mng", StringComparison.OrdinalIgnoreCase))
        {
            trimmed = trimmed[..^3];
        }

        return trimmed + ".bin";
    }
}
