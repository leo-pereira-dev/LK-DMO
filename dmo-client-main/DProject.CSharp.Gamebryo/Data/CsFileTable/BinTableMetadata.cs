using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record BinSectionManifest(
    string Name,
    int? FixedRecordSize = null,
    bool StopAfterCount = false);

public sealed record BinTableManifest(
    string ManagerName,
    string FileName,
    IReadOnlyList<BinSectionManifest> Sections);

public sealed record BinSectionSnapshot(
    string Name,
    int Offset,
    int Count,
    int? FixedRecordSize,
    int BytesConsumed,
    bool IsComplete);

public sealed record BinFileSnapshot(
    string TableName,
    string Path,
    int ByteCount,
    IReadOnlyList<BinSectionSnapshot> Sections);

public static class BinTableManifestCatalog
{
    private static readonly Dictionary<string, BinTableManifest> Manifests = new(StringComparer.OrdinalIgnoreCase)
    {
        ["CDigimonEvolution"] = new("CDigimonEvolution", "DigimonEvo.bin",
        [
            new("EvolutionInfos", StopAfterCount: true)
        ]),
        ["CsAddExpMng"] = new("CsAddExpMng", "AddExp.bin",
        [
            new("AddExp", FixedRecordSize: 1164)
        ]),
        ["CsAchieveMng"] = new("CsAchieveMng", "Achieve.bin",
        [
            new("TypeTree", StopAfterCount: true),
            new("Achieves", FixedRecordSize: 796)
        ]),
        ["CsBaseMng"] = new("CsBaseMng", "DMBase.bin",
        [
            new("TamerBases", FixedRecordSize: 40),
            new("DigimonBases", FixedRecordSize: 40),
            new("MapInfos", FixedRecordSize: 12),
            new("JumpBusters", StopAfterCount: true)
        ]),
        ["CsItemMng"] = new("CsItemMng", "ItemList.bin",
        [
            new("Items", FixedRecordSize: 1596),
            new("Categories", FixedRecordSize: 66),
            new("CoolTimes", FixedRecordSize: 16),
            new("DisplayTypes", FixedRecordSize: 8),
            new("TypeNames", FixedRecordSize: 132),
            new("Ranks", FixedRecordSize: 8)
        ]),
        ["CsSkillMng"] = new("CsSkillMng", "Skill.bin",
        [
            new("Skills", FixedRecordSize: 736),
            new("TamerSkills", FixedRecordSize: 36),
            new("AreaChecks", FixedRecordSize: 64)
        ]),
        ["CsTamerMng"] = new("CsTamerMng", "TamerList.bin",
        [
            new("Tamers", FixedRecordSize: 1500),
            new("Emotions", StopAfterCount: true)
        ]),
        ["CsDigimonMng"] = new("CsDigimonMng", "Digimon_List.bin",
        [
            new("Digimons", FixedRecordSize: 572)
        ]),
        ["CsMapMng"] = new("CsMapMng", "Map.bin",
        [
            new("MapComposite", StopAfterCount: true)
        ]),
        ["CsMapListMng"] = new("CsMapListMng", "MapList.bin",
        [
            new("MapList", StopAfterCount: true)
        ]),
        ["CsMapMonsterMng"] = new("CsMapMonsterMng", "MapMonsterList.bin",
        [
            new("MonsterFileGroups", StopAfterCount: true)
        ]),
        ["CsMapNpcMng"] = new("CsMapNpcMng", "MapNpc.bin",
        [
            new("MapNpcs", FixedRecordSize: 20)
        ]),
        ["CsMapObjectMng"] = new("CsMapObjectMng", "MapObject.bin",
        [
            new("ObjectGroups", StopAfterCount: true)
        ]),
        ["CsMapTableObjMng"] = new("CsMapTableObjMng", "Data\\Map\\TableObj.bin",
        [
            new("TableObjGroups", StopAfterCount: true)
        ]),
        ["CsMapPortalMng"] = new("CsMapPortalMng", "MapPortal.bin",
        [
            new("PortalGroups", StopAfterCount: true)
        ]),
        ["CsMapRegionMng"] = new("CsMapRegionMng", "MapRegion.bin",
        [
            new("RegionGroups", StopAfterCount: true)
        ]),
        ["CsMapCharLightMng"] = new("CsMapCharLightMng", "MapCharLight.bin",
        [
            new("CharLightGroups", StopAfterCount: true)
        ]),
        ["CsWeatherMng"] = new("CsWeatherMng", "Weather.bin",
        [
            new("WeatherGroups", StopAfterCount: true)
        ]),
        ["CsMonsterMng"] = new("CsMonsterMng", "Monster.bin",
        [
            new("Monsters", FixedRecordSize: 396),
            new("MonsterHits", FixedRecordSize: 8),
            new("MonsterSkills", FixedRecordSize: 144),
            new("MonsterSkillTerms", FixedRecordSize: 12)
        ]),
        ["CsMoveObjectMng"] = new("CsMoveObjectMng", "Data\\MO.bin",
        [
            new("MoveObjects", StopAfterCount: true)
        ]),
        ["CsNatureMng"] = new("CsNatureMng", "Nature.bin",
        [
            new("Natures", FixedRecordSize: 26),
            new("BaseElements", StopAfterCount: true)
        ]),
        ["CsBuffMng"] = new("CsBuffMng", "Buff.bin",
        [
            new("Buffs", FixedRecordSize: 476)
        ]),
        ["CsCashShopMng"] = new("CsCashShopMng", "CashShop.bin",
        [
            new("CashShopTables", StopAfterCount: true),
            new("CashShopWebData", StopAfterCount: true)
        ]),
        ["CsGotchaMng"] = new("CsGotchaMng", "Gotcha.bin",
        [
            new("GotchaMachines", FixedRecordSize: 36),
            new("GotchaItems", FixedRecordSize: 64),
            new("GotchaRareItems", FixedRecordSize: 144),
            new("GotchaMysteryItems", FixedRecordSize: 12),
            new("GotchaMysteryCoins", FixedRecordSize: 20)
        ]),
        ["CsHelpMng"] = new("CsHelpMng", "Data\\Interface\\Help\\HelpEng.bin",
        [
            new("HelpRecords", StopAfterCount: true)
        ]),
        ["CsTotalExcelMng"] = new("CsTotalExcelMng", "GameData\\English\\Total.bin",
        [
            new("Sheets", StopAfterCount: true)
        ]),
        ["CsNpcMng"] = new("CsNpcMng", "Npc.bin",
        [
            new("Npcs", StopAfterCount: true)
        ]),
        ["CsQuestMng"] = new("CsQuestMng", "Quest.bin",
        [
            new("Quests", StopAfterCount: true)
        ]),
        ["CsCuidMng"] = new("CsCuidMng", "Cuid.bin",
        [
            new("IdRules", FixedRecordSize: 68),
            new("ChatRules", FixedRecordSize: 64)
        ]),
        ["CsWorldMapMng"] = new("CsWorldMapMng", "WorldMap.bin",
        [
            new("Worlds", FixedRecordSize: 616),
            new("Areas", FixedRecordSize: 632)
        ]),
        ["CsSceneDataMng"] = new("CsSceneDataMng", "Scene.bin",
        [
            new("Scenes", StopAfterCount: true)
        ]),
        ["CsTalkMng"] = new("CsTalkMng", "Talk.bin",
        [
            new("DigimonTalks", FixedRecordSize: 412),
            new("EventTalks", FixedRecordSize: 408),
            new("Messages", FixedRecordSize: 560),
            new("Tips", FixedRecordSize: 404),
            new("LoadingTips", FixedRecordSize: 408)
        ]),
        ["CsTacticsMng"] = new("CsTacticsMng", "Tactics.bin",
        [
            new("Tactics", FixedRecordSize: 24),
            new("Explains", FixedRecordSize: 1160),
            new("EnchantItems", FixedRecordSize: 16)
        ]),
        ["CsMapListMng"] = new("CsMapListMng", "MapList.bin",
        [
            new("MapList", StopAfterCount: true)
        ])
    };

    public static BinTableManifest GetManifest(string managerName)
    {
        if (Manifests.TryGetValue(managerName, out BinTableManifest? manifest))
        {
            return manifest;
        }

        return new BinTableManifest(
            managerName,
            TableFileNameCatalog.GetBinFileName(managerName),
            [new("Primary", StopAfterCount: true)]);
    }
}

public static class CountPrefixedBinInspector
{
    public static BinFileSnapshot Inspect(string tableName, string path, ReadOnlyMemory<byte> data)
    {
        BinTableManifest manifest = BinTableManifestCatalog.GetManifest(tableName);
        var reader = new SafeBinaryReader(data);
        var sections = new List<BinSectionSnapshot>();

        foreach (BinSectionManifest section in manifest.Sections)
        {
            int offset = reader.Position;
            if (!reader.TryReadInt32(out int count) || count < 0)
            {
                sections.Add(new BinSectionSnapshot(section.Name, offset, 0, section.FixedRecordSize, 0, false));
                break;
            }

            int bytesConsumed = sizeof(int);
            bool complete = true;
            if (section.FixedRecordSize is int recordSize)
            {
                long sectionBytes = (long)recordSize * count;
                complete = sectionBytes <= reader.Remaining && reader.TrySkip((int)sectionBytes);
                bytesConsumed += complete ? checked((int)sectionBytes) : 0;
            }

            sections.Add(new BinSectionSnapshot(
                section.Name,
                offset,
                count,
                section.FixedRecordSize,
                bytesConsumed,
                complete));

            if (!complete || section.StopAfterCount)
            {
                break;
            }
        }

        return new BinFileSnapshot(tableName, path, data.Length, sections);
    }
}
