using System.Collections.ObjectModel;
using System.IO;
using System.Text;
using System.Xml.Linq;
using ItemEditor.Models;

namespace ItemEditor.Services;

public sealed class AssetCatalogWorkspace
{
    private const string NpcRelativePath = @"data\bin\english\Npc.bin";
    private const string NpcAssetsRelativePath = @"data\bin\english\NpcAssets.bin";
    private const string QuestRelativePath = @"data\bin\english\Quest.bin";
    private const string MapNpcRelativePath = @"data\bin\english\MapNpc.bin";
    private const int QuestTitleBytes = 160;
    private const int QuestBodyBytes = 4096;
    private const int QuestSimpleBytes = 256;
    private const int QuestHelperBytes = 1024;
    private const int QuestProcessBytes = 640;
    private const int QuestCompleteBytes = 1400;
    private const int QuestExpertBytes = 640;
    private const int NpcNameChars = 32;
    private const int NpcTalkChars = 512;

    private readonly Dictionary<int, NpcEditorRow> _npcById = [];
    private readonly Dictionary<int, QuestEditorRow> _questById = [];
    private readonly Dictionary<int, string> _mapNames = [];
    private readonly Dictionary<int, (int Width, int Height)> _mapSizes = [];
    private readonly Dictionary<int, string> _mapPaths = [];
    private readonly Dictionary<int, int> _mapRegionIds = [];
    private readonly Dictionary<int, string> _mapZoneTextureNames = [];
    private ItemBinWorkspace? _items;
    private string _sourcePath = string.Empty;
    private string _binDirectory = string.Empty;
    private string _npcDirectPath = string.Empty;
    private string _npcPackBasePath = string.Empty;
    private string _npcPackRelativePath = NpcRelativePath;
    private string _npcAssetsDirectPath = string.Empty;
    private string _npcAssetsPackBasePath = string.Empty;
    private string _npcAssetsPackRelativePath = NpcAssetsRelativePath;
    private string _questDirectPath = string.Empty;
    private string _questPackBasePath = string.Empty;
    private string _questPackRelativePath = QuestRelativePath;
    private string _mapNpcDirectPath = string.Empty;
    private string _mapNpcPackBasePath = string.Empty;
    private string _mapNpcPackRelativePath = MapNpcRelativePath;
    private byte[] _npcTailRaw = [];
    private bool _npcLoaded;
    private bool _npcLoadedFromPack;
    private bool _npcAssetsLoaded;
    private bool _npcAssetsLoadedFromPack;
    private bool _questLoaded;
    private bool _questLoadedFromPack;
    private bool _mapNpcLoaded;
    private bool _mapNpcLoadedFromPack;

    public ObservableCollection<NpcEditorRow> Npcs { get; } = [];
    public ObservableCollection<QuestEditorRow> Quests { get; } = [];
    public string StatusSummary { get; private set; } = "NPC and quest bins not loaded.";

    public void Load(string sourcePath, string binDirectory, ItemBinWorkspace items)
    {
        _sourcePath = sourcePath;
        _binDirectory = binDirectory;
        _items = items;
        Npcs.Clear();
        Quests.Clear();
        _npcById.Clear();
        _questById.Clear();
        _mapNames.Clear();
        _mapSizes.Clear();
        _mapPaths.Clear();
        _mapRegionIds.Clear();
        _mapZoneTextureNames.Clear();
        _npcDirectPath = string.Empty;
        _npcPackBasePath = string.Empty;
        _npcPackRelativePath = NpcRelativePath;
        _npcAssetsDirectPath = string.Empty;
        _npcAssetsPackBasePath = string.Empty;
        _npcAssetsPackRelativePath = NpcAssetsRelativePath;
        _questDirectPath = string.Empty;
        _questPackBasePath = string.Empty;
        _questPackRelativePath = QuestRelativePath;
        _mapNpcDirectPath = string.Empty;
        _mapNpcPackBasePath = string.Empty;
        _mapNpcPackRelativePath = MapNpcRelativePath;
        _npcTailRaw = [];
        _npcLoaded = false;
        _npcLoadedFromPack = false;
        _npcAssetsLoaded = false;
        _npcAssetsLoadedFromPack = false;
        _questLoaded = false;
        _questLoadedFromPack = false;
        _mapNpcLoaded = false;
        _mapNpcLoadedFromPack = false;

        var mapCount = TryLoadMapList(ReadOptionalBin(sourcePath, binDirectory, "MapList.bin"));
        var mapNameFallbackCount = TryLoadMapNameXml(FindOptionalStringXml(sourcePath, binDirectory, "MapName_Str.xml"), ["TextA"]);
        mapNameFallbackCount += TryLoadMapNameXml(FindOptionalStringXml(sourcePath, binDirectory, "AreaMapInfo_Str.xml"), ["TextB", "TextA"]);
        var npcAssetCount = TryLoadNpcAssets(ReadOptionalBin(sourcePath, binDirectory, "NpcAssets.bin"));
        var npcBinCount = TryLoadNpcBin(ReadOptionalBin(sourcePath, binDirectory, "Npc.bin"));
        var mapNpcCount = TryLoadMapNpcs(ReadOptionalBin(sourcePath, binDirectory, "MapNpc.bin"));
        var questCount = TryLoadQuests(ReadOptionalBin(sourcePath, binDirectory, "Quest.bin"));
        ResolveQuestLinks();
        ResolveQuestUnlocks();
        AttachCraftRecipes(items);

        foreach (var npc in Npcs)
        {
            npc.RefreshAssetSummary();
            npc.MarkClean();
            npc.MarkMapPositionClean();
            foreach (var item in npc.Items)
                item.MarkClean();
            foreach (var portal in npc.Portals)
            {
                portal.MarkClean();
                foreach (var group in portal.Groups)
                {
                    group.MarkClean();
                    foreach (var resource in group.Resources)
                        resource.MarkClean();
                }
            }
        }

        foreach (var quest in Quests)
        {
            quest.MarkClean();
            foreach (var row in quest.Supplies)
                row.MarkClean();
            foreach (var row in quest.Conditions)
                row.MarkClean();
            foreach (var row in quest.Goals)
                row.MarkClean();
            foreach (var row in quest.Rewards)
                row.MarkClean();
            foreach (var row in quest.Events)
                row.MarkClean();
        }

        StatusSummary = $"Loaded {Npcs.Count:N0} NPCs ({npcAssetCount:N0} from NpcAssets.bin, {npcBinCount:N0} from Npc.bin, {mapNpcCount:N0} positions from MapNpc.bin), {mapCount:N0} maps ({mapNameFallbackCount:N0} fallback names), and {questCount:N0} quests.";
    }

    public NpcEditorRow? FindNpc(int npcId)
        => _npcById.TryGetValue(npcId, out var npc) ? npc : null;

    public string MapDisplay(int mapId)
        => _mapNames.TryGetValue(mapId, out var name) && !string.IsNullOrWhiteSpace(name)
            ? $"{mapId} - {name}"
            : $"{mapId} - Map {mapId}";

    public string MapName(int mapId)
        => _mapNames.TryGetValue(mapId, out var name) && !string.IsNullOrWhiteSpace(name)
            ? name
            : $"Map {mapId}";

    public (int Width, int Height) GetMapSize(int mapId)
        => _mapSizes.TryGetValue(mapId, out var size) && size.Width > 0 && size.Height > 0
            ? size
            : (1000, 1000);

    public IReadOnlyList<NpcEditorRow> NpcsOnMap(int mapId)
        => Npcs
            .Where(x => x.MapId == mapId && x.HasMapPosition)
            .OrderBy(x => x.NpcId)
            .ToList();

    public (double MinX, double MinY, double MaxX, double MaxY) GetMapViewport(int mapId)
    {
        var mapNpcs = Npcs.Where(x => x.MapId == mapId && x.HasMapPosition).ToList();
        if (_mapSizes.TryGetValue(mapId, out var size) && size.Width > 0 && size.Height > 0)
        {
            var minX = Math.Min(0, mapNpcs.Select(x => x.MapPositionX).DefaultIfEmpty(0).Min());
            var minY = Math.Min(0, mapNpcs.Select(x => x.MapPositionY).DefaultIfEmpty(0).Min());
            var maxX = Math.Max(size.Width, mapNpcs.Select(x => x.MapPositionX).DefaultIfEmpty(size.Width).Max());
            var maxY = Math.Max(size.Height, mapNpcs.Select(x => x.MapPositionY).DefaultIfEmpty(size.Height).Max());
            return PadViewport(minX, minY, maxX, maxY);
        }

        if (mapNpcs.Count == 0)
            return (0, 0, 1000, 1000);

        return PadViewport(
            mapNpcs.Min(x => x.MapPositionX),
            mapNpcs.Min(x => x.MapPositionY),
            mapNpcs.Max(x => x.MapPositionX),
            mapNpcs.Max(x => x.MapPositionY));
    }

    public IReadOnlyList<string> MapTextureCandidates(int mapId)
    {
        var candidates = new List<string>();
        if (_mapPaths.TryGetValue(mapId, out var mapPath) && !string.IsNullOrWhiteSpace(mapPath))
            AddPathDerivedMapTextureCandidates(candidates, mapPath);

        if (_mapZoneTextureNames.TryGetValue(mapId, out var zoneTexture) && !string.IsNullOrWhiteSpace(zoneTexture))
        {
            AddInterfaceMapTextureCandidates(candidates, zoneTexture);
            var miniTexture = zoneTexture.Replace("_Zone.tga", "_Mini.tga", StringComparison.OrdinalIgnoreCase);
            if (!miniTexture.Equals(zoneTexture, StringComparison.OrdinalIgnoreCase))
                AddInterfaceMapTextureCandidates(candidates, miniTexture);
        }

        if (MapName(mapId).Contains("DATS", StringComparison.OrdinalIgnoreCase) ||
            (_mapPaths.TryGetValue(mapId, out var datsPath) && datsPath.Contains("DATS", StringComparison.OrdinalIgnoreCase)))
        {
            AddInterfaceMapTextureCandidates(candidates, "DATS_IN_Zone.tga");
        }

        AddAreaMapTextureCandidates(candidates, mapId);
        if (_mapRegionIds.TryGetValue(mapId, out var regionId) && regionId != mapId)
            AddAreaMapTextureCandidates(candidates, regionId);

        AddInterfaceMapTextureCandidates(candidates, "_EmptyZone.tga");
        return candidates.Distinct(StringComparer.OrdinalIgnoreCase).ToList();
    }

    public void SetNpcMapPosition(NpcEditorRow npc, int x, int y)
    {
        npc.MapPositionX = x;
        npc.MapPositionY = y;
        npc.HasMapPosition = true;
        if (npc.MapId > 0)
            npc.MapName = ResolveMapName(npc.MapId);
    }

    public string SaveNpcPositions()
    {
        if (!_mapNpcLoaded)
            return "MapNpc.bin not loaded.";

        if (!Npcs.Any(x => x.IsMapPositionDirty))
            return "NPC positions unchanged.";

        var bytes = BuildMapNpcBytes();
        if (_mapNpcLoadedFromPack)
        {
            var packBase = string.IsNullOrWhiteSpace(_mapNpcPackBasePath) ? _sourcePath : _mapNpcPackBasePath;
            _ = PackArchive.PatchBytes(packBase, new Dictionary<string, byte[]>
            {
                [_mapNpcPackRelativePath] = bytes
            });
            MarkNpcPositionsClean();
            return $"Saved MapNpc.bin back into pack ({_mapNpcPackRelativePath}).";
        }

        if (string.IsNullOrWhiteSpace(_mapNpcDirectPath))
            throw new InvalidOperationException("MapNpc.bin source path was not captured during load.");

        var backup = _mapNpcDirectPath + ".bak_itemeditor_" + DateTime.Now.ToString("yyyyMMdd_HHmmss");
        File.Copy(_mapNpcDirectPath, backup, overwrite: false);
        File.WriteAllBytes(_mapNpcDirectPath, bytes);
        MarkNpcPositionsClean();
        return $"Saved MapNpc.bin ({Npcs.Count(x => x.HasMapPosition):N0} NPC positions).";
    }

    public NpcEditorRow CreateNewNpc(int typeId, string name, int mapId = 0)
    {
        var nextId = Npcs.Count == 0 ? 1 : Npcs.Max(x => x.NpcId) + 1;
        while (_npcById.ContainsKey(nextId))
            nextId++;

        var npc = new NpcEditorRow
        {
            NpcId = nextId,
            TypeId = typeId,
            MapId = mapId,
            MapName = ResolveMapName(mapId),
            Name = name
        };

        _npcById[npc.NpcId] = npc;
        Npcs.Add(npc);
        npc.RefreshAssetSummary();
        return npc;
    }

    private static (double MinX, double MinY, double MaxX, double MaxY) PadViewport(double minX, double minY, double maxX, double maxY)
    {
        if (Math.Abs(maxX - minX) < 1)
            maxX = minX + 1000;
        if (Math.Abs(maxY - minY) < 1)
            maxY = minY + 1000;

        var padX = Math.Max(100, (maxX - minX) * 0.05);
        var padY = Math.Max(100, (maxY - minY) * 0.05);
        return (minX - padX, minY - padY, maxX + padX, maxY + padY);
    }

    public QuestEditorRow CreateNewQuest(int questType)
    {
        var nextId = Quests.Count == 0 ? 1 : Quests.Max(x => x.QuestId) + 1;
        while (_questById.ContainsKey(nextId))
            nextId++;

        var quest = new QuestEditorRow
        {
            QuestId = nextId,
            QuestType = questType,
            Level = 1,
            Active = true,
            Title = "New Quest",
            TitleTab = "New Quest",
            Simple = "New quest draft"
        };

        _questById[quest.QuestId] = quest;
        Quests.Add(quest);
        return quest;
    }

    public QuestSupplyEditorRow AddQuestSupply(QuestEditorRow quest, ItemEditorRow? item)
    {
        var supply = new QuestSupplyEditorRow
        {
            Type = 0,
            ItemId = item?.ItemId ?? 0,
            Amount = 1,
            IsDirty = true
        };

        ResolveQuestSupply(supply);
        quest.Supplies.Add(supply);
        quest.IsDirty = true;
        return supply;
    }

    public QuestConditionEditorRow AddQuestCondition(QuestEditorRow quest)
    {
        var condition = new QuestConditionEditorRow
        {
            ConditionType = 0,
            ConditionId = 0,
            ConditionCount = 1,
            IsDirty = true
        };

        ResolveQuestCondition(condition);
        quest.Conditions.Add(condition);
        quest.IsDirty = true;
        ResolveQuestUnlocks();
        return condition;
    }

    public QuestGoalEditorRow AddQuestGoal(QuestEditorRow quest, ItemEditorRow? item)
    {
        var goal = new QuestGoalEditorRow
        {
            GoalType = item == null ? 4 : 1,
            GoalId = item?.ItemId ?? 0,
            GoalAmount = 1,
            CurTypeCount = 0,
            SubValue = 0,
            SubValueTwo = 0,
            IsDirty = true
        };

        ResolveQuestGoal(goal);
        quest.Goals.Add(goal);
        quest.IsDirty = true;
        return goal;
    }

    public QuestRewardEditorRow AddQuestReward(QuestEditorRow quest, ItemEditorRow? item)
    {
        var reward = new QuestRewardEditorRow
        {
            Method = 0,
            RewardType = item == null ? 0 : 2,
            ValueA = item?.ItemId ?? 0,
            ValueB = item == null ? 0 : 1,
            IsDirty = true
        };

        ResolveQuestReward(reward);
        quest.Rewards.Add(reward);
        quest.IsDirty = true;
        return reward;
    }

    public QuestEventEditorRow AddQuestEvent(QuestEditorRow quest)
    {
        var evt = new QuestEventEditorRow
        {
            EventSlot = quest.Events.Count,
            EventId = 0,
            IsDirty = true
        };

        quest.Events.Add(evt);
        quest.IsDirty = true;
        return evt;
    }

    public void RefreshQuestDerivedData(QuestEditorRow? quest)
    {
        if (quest == null)
            return;

        _questById.Clear();
        foreach (var candidate in Quests)
        {
            if (!_questById.ContainsKey(candidate.QuestId))
                _questById[candidate.QuestId] = candidate;
        }

        ResolveQuest(quest);
        ResolveQuestLinks();
        ResolveQuestUnlocks();
    }

    public string SaveQuests()
    {
        if (!_questLoaded)
            return "Quest.bin not loaded.";

        if (!IsAnyQuestDirty())
            return "Quest.bin unchanged.";

        RebuildQuestIndex();
        ResolveQuestLinks();
        ResolveQuestUnlocks();
        foreach (var quest in Quests)
            ResolveQuest(quest);

        var bytes = BuildQuestBinBytes();
        if (_questLoadedFromPack)
        {
            var packBase = string.IsNullOrWhiteSpace(_questPackBasePath) ? _sourcePath : _questPackBasePath;
            _ = PackArchive.PatchBytes(packBase, new Dictionary<string, byte[]>
            {
                [_questPackRelativePath] = bytes
            });
            MarkQuestsClean();
            return $"Saved Quest.bin back into pack ({_questPackRelativePath}).";
        }

        if (string.IsNullOrWhiteSpace(_questDirectPath))
            throw new InvalidOperationException("Quest.bin source path was not captured during load.");

        var backup = _questDirectPath + ".bak_itemeditor_" + DateTime.Now.ToString("yyyyMMdd_HHmmss");
        File.Copy(_questDirectPath, backup, overwrite: false);
        File.WriteAllBytes(_questDirectPath, bytes);
        MarkQuestsClean();
        return $"Saved Quest.bin ({Quests.Count:N0} quests).";
    }

    public IReadOnlyList<string> SaveAllDirty()
    {
        var messages = new List<string>();
        var npcDataDirty = IsAnyNpcDataDirty();
        if (npcDataDirty)
        {
            if (_npcAssetsLoaded)
                messages.Add(SaveNpcAssetsCore());
            else
                messages.Add("NpcAssets.bin not loaded.");

            if (_npcLoaded)
                messages.Add(SaveNpcsCore());
            else
                messages.Add("Npc.bin not loaded.");

            MarkNpcDataClean();
        }
        else
        {
            messages.Add("Npc.bin/NpcAssets.bin unchanged.");
        }

        messages.Add(SaveNpcPositions());
        messages.Add(SaveQuests());
        return messages;
    }

    private string SaveNpcsCore()
    {
        RebuildNpcIndex();
        var bytes = BuildNpcBinBytes();
        if (_npcLoadedFromPack)
        {
            var packBase = string.IsNullOrWhiteSpace(_npcPackBasePath) ? _sourcePath : _npcPackBasePath;
            _ = PackArchive.PatchBytes(packBase, new Dictionary<string, byte[]>
            {
                [_npcPackRelativePath] = bytes
            });
            return $"Saved Npc.bin back into pack ({_npcPackRelativePath}).";
        }

        if (string.IsNullOrWhiteSpace(_npcDirectPath))
            throw new InvalidOperationException("Npc.bin source path was not captured during load.");

        WriteDirectBinWithBackup(_npcDirectPath, bytes);
        return $"Saved Npc.bin ({Npcs.Count(x => x.NpcId > 0):N0} NPCs).";
    }

    private string SaveNpcAssetsCore()
    {
        var bytes = BuildNpcAssetsBytes();
        if (_npcAssetsLoadedFromPack)
        {
            var packBase = string.IsNullOrWhiteSpace(_npcAssetsPackBasePath) ? _sourcePath : _npcAssetsPackBasePath;
            _ = PackArchive.PatchBytes(packBase, new Dictionary<string, byte[]>
            {
                [_npcAssetsPackRelativePath] = bytes
            });
            return $"Saved NpcAssets.bin back into pack ({_npcAssetsPackRelativePath}).";
        }

        if (string.IsNullOrWhiteSpace(_npcAssetsDirectPath))
            throw new InvalidOperationException("NpcAssets.bin source path was not captured during load.");

        WriteDirectBinWithBackup(_npcAssetsDirectPath, bytes);
        return $"Saved NpcAssets.bin ({Npcs.Count(x => x.AssetId > 0 || HasDirtyNpcAssetContent(x)):N0} NPC asset rows).";
    }

    private bool IsAnyNpcDataDirty()
        => Npcs.Any(npc =>
            npc.IsDirty ||
            npc.Items.Any(x => x.IsDirty) ||
            npc.Portals.Any(IsPortalDirty) ||
            npc.QuestLinks.Any(x => x.IsDirty));

    private static bool IsPortalDirty(NpcPortalEditorRow portal)
        => portal.IsDirty ||
           portal.Groups.Any(group => group.IsDirty || group.Resources.Any(resource => resource.IsDirty));

    private void MarkNpcDataClean()
    {
        foreach (var npc in Npcs)
        {
            npc.MarkClean();
            foreach (var item in npc.Items)
                item.MarkClean();
            foreach (var portal in npc.Portals)
            {
                portal.MarkClean();
                foreach (var group in portal.Groups)
                {
                    group.MarkClean();
                    foreach (var resource in group.Resources)
                        resource.MarkClean();
                }
            }
            foreach (var link in npc.QuestLinks)
                link.MarkClean();
        }
    }

    private void RebuildNpcIndex()
    {
        _npcById.Clear();
        foreach (var npc in Npcs.Where(x => x.NpcId > 0))
        {
            if (_npcById.ContainsKey(npc.NpcId))
                throw new InvalidDataException($"NPC ID {npc.NpcId} is duplicated. Fix duplicate NPC IDs before saving Npc.bin.");

            _npcById[npc.NpcId] = npc;
            npc.MapName = ResolveMapName(npc.MapId);
            npc.RefreshAssetSummary();
        }
    }

    private static void WriteDirectBinWithBackup(string path, byte[] bytes)
    {
        var backup = path + ".bak_itemeditor_" + DateTime.Now.ToString("yyyyMMdd_HHmmss");
        File.Copy(path, backup, overwrite: false);
        File.WriteAllBytes(path, bytes);
    }

    private bool IsAnyQuestDirty()
        => Quests.Any(quest =>
            quest.IsDirty ||
            quest.Supplies.Any(x => x.IsDirty) ||
            quest.Conditions.Any(x => x.IsDirty) ||
            quest.Goals.Any(x => x.IsDirty) ||
            quest.Rewards.Any(x => x.IsDirty) ||
            quest.Events.Any(x => x.IsDirty));

    private void MarkQuestsClean()
    {
        foreach (var quest in Quests)
        {
            quest.MarkClean();
            foreach (var row in quest.Supplies)
                row.MarkClean();
            foreach (var row in quest.Conditions)
                row.MarkClean();
            foreach (var row in quest.Goals)
                row.MarkClean();
            foreach (var row in quest.Rewards)
                row.MarkClean();
            foreach (var row in quest.Events)
                row.MarkClean();
        }
    }

    private void RebuildQuestIndex()
    {
        _questById.Clear();
        foreach (var quest in Quests)
        {
            if (_questById.ContainsKey(quest.QuestId))
                throw new InvalidDataException($"Quest ID {quest.QuestId} is duplicated. Fix duplicate quest IDs before saving Quest.bin.");

            _questById[quest.QuestId] = quest;
        }
    }

    private byte[] BuildQuestBinBytes()
    {
        using var stream = new MemoryStream();
        using var writer = new BinaryWriter(stream);
        writer.Write(Quests.Count);

        foreach (var quest in Quests)
            WriteQuest(writer, quest);

        writer.Flush();
        return stream.ToArray();
    }

    private static void WriteQuest(BinaryWriter writer, QuestEditorRow quest)
    {
        if (quest.Events.Count > 4)
            throw new InvalidDataException($"Quest {quest.QuestId} has {quest.Events.Count} events. The client Quest.bin layout supports at most 4 event slots.");

        writer.Write(checked((uint)quest.QuestId));
        writer.Write(checked((uint)quest.ModelIdRecv));
        writer.Write(checked((uint)quest.ModelIdComplete));
        writer.Write(checked((short)quest.Level));
        writer.Write(quest.ToolPosX);
        writer.Write(quest.ToolPosY);
        writer.Write(checked((uint)quest.ManagedId));
        writer.Write(quest.Active);
        writer.Write(quest.Immediate);
        writer.Write(quest.Reset);
        writer.Write(checked((uint)quest.QuestType));
        writer.Write(checked((uint)quest.StartTargetType));
        writer.Write(checked((uint)quest.StartTargetId));
        writer.Write(checked((uint)quest.TargetType));
        writer.Write(quest.TargetValue);

        WriteUtf16Bytes(writer, quest.TitleTab, QuestTitleBytes);
        WriteUtf16Bytes(writer, quest.Title, QuestTitleBytes);
        WriteUtf16Bytes(writer, quest.Body, QuestBodyBytes);
        WriteUtf16Bytes(writer, quest.Simple, QuestSimpleBytes);
        WriteUtf16Bytes(writer, quest.Helper, QuestHelperBytes);
        WriteUtf16Bytes(writer, quest.Process, QuestProcessBytes);
        WriteUtf16Bytes(writer, quest.Complete, QuestCompleteBytes);
        WriteUtf16Bytes(writer, quest.Expert, QuestExpertBytes);

        writer.Write(quest.Supplies.Count);
        foreach (var supply in quest.Supplies)
        {
            writer.Write(checked((uint)supply.Type));
            writer.Write(checked((uint)supply.ItemId));
            writer.Write(supply.Amount);
        }

        writer.Write(quest.Conditions.Count);
        foreach (var condition in quest.Conditions)
        {
            writer.Write(checked((uint)condition.ConditionType));
            writer.Write(checked((uint)condition.ConditionId));
            writer.Write(condition.ConditionCount);
        }

        writer.Write(quest.Goals.Count);
        foreach (var goal in quest.Goals)
        {
            writer.Write(checked((uint)goal.GoalType));
            writer.Write(checked((uint)goal.GoalId));
            writer.Write(goal.GoalAmount);
            writer.Write(checked((uint)goal.CurTypeCount));
            writer.Write(goal.SubValue);
            writer.Write(goal.SubValueTwo);
        }

        writer.Write(quest.Rewards.Count);
        foreach (var reward in quest.Rewards)
        {
            writer.Write(checked((uint)reward.Method));
            writer.Write(checked((uint)reward.RewardType));
            writer.Write(checked((uint)reward.ValueA));
            writer.Write(reward.ValueB);
        }

        var events = quest.Events.OrderBy(x => x.EventSlot).ToList();
        writer.Write(events.Count);
        foreach (var evt in events)
            writer.Write(evt.EventId);
    }

    private byte[] BuildNpcBinBytes()
    {
        var rows = Npcs
            .Where(x => x.NpcId > 0)
            .OrderBy(x => x.NpcId)
            .ToList();

        using var stream = new MemoryStream();
        using var writer = new BinaryWriter(stream);
        writer.Write(rows.Count);
        foreach (var npc in rows)
            WriteNpcBinRow(writer, npc);

        writer.Write(BuildNpcTailBytes());
        writer.Flush();
        return stream.ToArray();
    }

    private static void WriteNpcBinRow(BinaryWriter writer, NpcEditorRow npc)
    {
        WriteUInt32FromInt(writer, npc.NpcId, "NPC ID");
        WriteUInt32FromInt(writer, Math.Max(0, npc.MapId), "NPC map ID");
        writer.Write(npc.TypeId);
        writer.Write(npc.MoveType);
        WriteUInt32FromInt(writer, Math.Max(0, npc.DisplayFlag), "NPC display flag");
        WriteUInt32FromInt(writer, Math.Max(0, npc.ModelId), "NPC model ID");
        WriteUtf16Bytes(writer, npc.PenName, NpcNameChars * 2);
        WriteUtf16Bytes(writer, npc.Name, NpcNameChars * 2);
        WriteUtf16Bytes(writer, npc.Talk, NpcTalkChars * 2);

        WriteNpcTypePayload(writer, npc);
        WriteNpcQuestExtras(writer, npc);
    }

    private static void WriteNpcTypePayload(BinaryWriter writer, NpcEditorRow npc)
    {
        if (NpcTypeCatalog.IsShop(npc.TypeId))
        {
            var items = npc.Items
                .Where(x => x.ItemId > 0)
                .OrderBy(x => x.ItemId)
                .ToList();

            writer.Write(items.Count);
            foreach (var item in items)
                WriteUInt32FromInt(writer, item.ItemId, "NPC shop item ID");
            return;
        }

        if (npc.TypeId == 3)
        {
            if (npc.NpcTypePayloadRaw.Length > 0)
            {
                writer.Write(npc.NpcTypePayloadRaw);
                return;
            }

            writer.Write(npc.Portals.FirstOrDefault()?.PortalType ?? 0);
            writer.Write(0);
            return;
        }

        if (npc.TypeId == 19)
        {
            if (npc.Items.Count > 0 && (npc.NpcTypePayloadRaw.Length == 0 || npc.Items.Any(x => x.IsDirty)))
            {
                var items = npc.Items
                    .Where(x => x.ItemId > 0)
                    .OrderBy(x => x.ItemId)
                    .ToList();

                writer.Write(2);
                writer.Write(items.Count);
                foreach (var item in items)
                    WriteUInt32FromInt(writer, item.ItemId, "NPC special event item ID");
                return;
            }

            if (npc.NpcTypePayloadRaw.Length > 0)
            {
                writer.Write(npc.NpcTypePayloadRaw);
                return;
            }

            writer.Write(0);
        }
    }

    private static void WriteNpcQuestExtras(BinaryWriter writer, NpcEditorRow npc)
    {
        var groups = npc.QuestLinks
            .Where(x => x.QuestId > 0)
            .GroupBy(x => new { x.ActType, x.CompleteState })
            .OrderBy(x => x.Key.ActType)
            .ThenBy(x => x.Key.CompleteState)
            .ToList();

        if (groups.Count == 0)
        {
            writer.Write(0);
            return;
        }

        writer.Write(1);
        writer.Write(0);
        WriteUInt32FromInt(writer, Math.Max(0, npc.QuestInitialState), "NPC quest initial state");
        writer.Write(groups.Count);
        foreach (var group in groups)
        {
            WriteUInt32FromInt(writer, Math.Max(0, group.Key.ActType), "NPC quest act type");
            WriteUInt32FromInt(writer, Math.Max(0, group.Key.CompleteState), "NPC quest complete state");
            var links = group.OrderBy(x => x.QuestId).ToList();
            writer.Write(links.Count);
            foreach (var link in links)
                WriteUInt32FromInt(writer, link.QuestId, "NPC linked quest ID");
        }
    }

    private byte[] BuildNpcTailBytes()
    {
        if (_npcTailRaw.Length == 0)
            return [];

        var bytes = _npcTailRaw.ToArray();
        if (!TryGetNpcModelRecordSize(bytes, out var count, out var recordSize))
            return bytes;

        for (var i = 0; i < count; i++)
        {
            var offset = 4 + (i * recordSize);
            if (offset + 138 > bytes.Length)
                break;

            var modelId = BitConverter.ToInt32(bytes, offset);
            var npc = Npcs.FirstOrDefault(x => x.ModelId == modelId);
            if (npc == null)
                continue;

            WriteUtf16IntoBuffer(bytes, offset + 10, 128, npc.ModelComment);
        }

        return bytes;
    }

    private static bool TryGetNpcModelRecordSize(byte[] bytes, out int count, out int recordSize)
    {
        count = 0;
        recordSize = 0;
        if (bytes.Length < 4)
            return false;

        count = BitConverter.ToInt32(bytes, 0);
        if (count < 0 || count > 100000)
            return false;

        foreach (var candidateSize in new[] { 140, 138 })
        {
            var end = 4L + ((long)count * candidateSize);
            if (end > bytes.Length)
                continue;

            if (end == bytes.Length)
            {
                recordSize = candidateSize;
                return true;
            }

            if (end + 4 <= bytes.Length)
            {
                var nextCount = BitConverter.ToInt32(bytes, (int)end);
                if (nextCount is >= 0 and < 100000)
                {
                    recordSize = candidateSize;
                    return true;
                }
            }
        }

        return false;
    }

    private byte[] BuildNpcAssetsBytes()
    {
        var rows = Npcs
            .Where(x => x.NpcId > 0 && (x.AssetId > 0 || HasDirtyNpcAssetContent(x)))
            .OrderBy(x => x.AssetId <= 0 ? long.MaxValue : x.AssetId)
            .ThenBy(x => x.NpcId)
            .ToList();

        EnsureNpcAssetIds(rows);

        using var stream = new MemoryStream();
        using var writer = new BinaryWriter(stream);
        writer.Write(rows.Count);
        foreach (var npc in rows)
        {
            writer.Write(npc.AssetId);
            writer.Write(npc.NpcId);
            writer.Write(Math.Max(0, npc.MapId));

            var items = npc.Items.Where(x => x.ItemId > 0).OrderBy(x => x.Id <= 0 ? long.MaxValue : x.Id).ThenBy(x => x.ItemId).ToList();
            writer.Write(items.Count);
            foreach (var item in items)
            {
                writer.Write(item.Id);
                writer.Write(item.ItemId);
            }

            writer.Write(npc.Portals.Count);
            foreach (var portal in npc.Portals)
            {
                writer.Write(portal.Id);
                writer.Write(portal.PortalType);
                writer.Write(portal.PortalCount);
                writer.Write(portal.Groups.Count);
                foreach (var group in portal.Groups)
                {
                    writer.Write(group.Id);
                    writer.Write(group.Resources.Count);
                    foreach (var resource in group.Resources)
                    {
                        writer.Write(resource.Id);
                        writer.Write(resource.ValueA);
                        writer.Write(resource.ValueB);
                        writer.Write(resource.ValueC);
                    }
                }
            }
        }

        writer.Flush();
        return stream.ToArray();
    }

    private static bool HasDirtyNpcAssetContent(NpcEditorRow npc)
        => npc.Items.Any(x => x.Id > 0 || x.IsDirty) ||
           npc.Portals.Any(x => x.Id > 0 || IsPortalDirty(x));

    private void EnsureNpcAssetIds(IReadOnlyList<NpcEditorRow> rows)
    {
        var nextAssetId = NextLongId(Npcs.Select(x => x.AssetId));
        var nextItemId = NextLongId(Npcs.SelectMany(x => x.Items).Select(x => x.Id));
        var nextPortalId = NextLongId(Npcs.SelectMany(x => x.Portals).Select(x => x.Id));
        var nextGroupId = NextLongId(Npcs.SelectMany(x => x.Portals).SelectMany(x => x.Groups).Select(x => x.Id));
        var nextResourceId = NextLongId(Npcs.SelectMany(x => x.Portals).SelectMany(x => x.Groups).SelectMany(x => x.Resources).Select(x => x.Id));

        foreach (var npc in rows)
        {
            if (npc.AssetId <= 0)
                npc.AssetId = nextAssetId++;

            foreach (var item in npc.Items.Where(x => x.ItemId > 0 && x.Id <= 0))
                item.Id = nextItemId++;

            foreach (var portal in npc.Portals)
            {
                if (portal.Id <= 0)
                    portal.Id = nextPortalId++;

                foreach (var group in portal.Groups)
                {
                    if (group.Id <= 0)
                        group.Id = nextGroupId++;

                    foreach (var resource in group.Resources.Where(x => x.Id <= 0))
                        resource.Id = nextResourceId++;
                }
            }
        }
    }

    private static long NextLongId(IEnumerable<long> values)
    {
        var max = values.DefaultIfEmpty(0).Max();
        return Math.Max(1, max + 1);
    }

    private static void WriteUInt32FromInt(BinaryWriter writer, int value, string fieldName)
    {
        if (value < 0)
            throw new InvalidDataException($"{fieldName} cannot be negative.");

        writer.Write((uint)value);
    }

    private static void WriteUtf16IntoBuffer(byte[] buffer, int offset, int byteCount, string value)
    {
        Array.Clear(buffer, offset, byteCount);
        var maxBytes = Math.Max(0, byteCount - 2);
        if (maxBytes % 2 != 0)
            maxBytes--;

        var encoded = Encoding.Unicode.GetBytes(value ?? string.Empty);
        var copyBytes = Math.Min(encoded.Length, maxBytes);
        if (copyBytes % 2 != 0)
            copyBytes--;

        Array.Copy(encoded, 0, buffer, offset, copyBytes);
    }

    private int TryLoadMapList(byte[]? bytes)
    {
        if (bytes == null || bytes.Length == 0)
            return 0;

        try
        {
            using var stream = new MemoryStream(bytes);
            using var reader = new BinaryReader(stream);
            var count = reader.ReadInt32();
            for (var i = 0; i < count; i++)
            {
                var mapId = ReadUInt32AsInt(reader);
                var mapName = ReadSizedString(reader);
                var mapPath = ReadSizedString(reader);
                _ = ReadSizedString(reader); // bg sound
                var width = reader.ReadInt32();
                var height = reader.ReadInt32();
                var mapDescription = ReadSizedUnicodeString(reader); // visible map name in English bins
                var englishDescription = ReadSizedUnicodeString(reader);
                _ = ReadUInt32AsInt(reader); // resurrection map
                var regionId = reader.ReadUInt16();
                _ = reader.ReadUInt16(); // fatigue type
                _ = reader.ReadUInt16(); // fatigue debuff
                _ = reader.ReadUInt16(); // fatigue start
                _ = reader.ReadUInt16(); // fatigue add time
                _ = reader.ReadInt16(); // fatigue add point
                _ = reader.ReadInt16(); // camera max level
                _ = reader.ReadBoolean(); // xg consume type
                _ = reader.ReadBoolean(); // battle tag use

                _mapNames[mapId] = BestMapName(mapId, mapDescription, mapName, englishDescription);
                _mapSizes[mapId] = (width, height);
                _mapPaths[mapId] = mapPath;
                _mapRegionIds[mapId] = regionId;
                var zoneTexture = ZoneTextureName(mapPath);
                if (!string.IsNullOrWhiteSpace(zoneTexture))
                    _mapZoneTextureNames[mapId] = zoneTexture;
            }

            return _mapNames.Count;
        }
        catch
        {
            _mapNames.Clear();
            _mapPaths.Clear();
            _mapRegionIds.Clear();
            _mapZoneTextureNames.Clear();
            return 0;
        }
    }

    private int TryLoadMapNameXml(string? path, IReadOnlyList<string> preferredTextFields)
    {
        if (string.IsNullOrWhiteSpace(path) || !File.Exists(path))
            return 0;

        try
        {
            var loaded = 0;
            var document = XDocument.Load(path);
            foreach (var record in document.Descendants().Where(x => x.Attribute("kind")?.Value == "Record"))
            {
                var keyElement = record.Elements().FirstOrDefault(x => x.Name.LocalName == "TextKey");
                if (keyElement?.Attribute("value")?.Value is not { } keyValue || !int.TryParse(keyValue, out var mapId))
                    continue;

                foreach (var field in preferredTextFields)
                {
                    var name = CleanMapName(record.Elements().FirstOrDefault(x => x.Name.LocalName == field)?.Value);
                    if (!HasUsefulMapName(name, mapId))
                        continue;

                    if (!_mapNames.TryGetValue(mapId, out var current) || !HasUsefulMapName(current, mapId))
                    {
                        _mapNames[mapId] = name;
                        loaded++;
                    }

                    break;
                }
            }

            return loaded;
        }
        catch
        {
            return 0;
        }
    }

    private int TryLoadNpcAssets(byte[]? bytes)
    {
        if (bytes == null || bytes.Length == 0)
            return 0;

        _npcAssetsLoaded = true;
        using var stream = new MemoryStream(bytes);
        using var reader = new BinaryReader(stream);
        var count = reader.ReadInt32();

        for (var i = 0; i < count; i++)
        {
            var assetId = reader.ReadInt64();
            var npcId = reader.ReadInt32();
            var mapId = reader.ReadInt32();
            var npc = GetOrCreateNpc(npcId);
            npc.AssetId = assetId;
            if (npc.MapId == 0)
                npc.MapId = mapId;
            npc.MapName = ResolveMapName(npc.MapId);

            var itemCount = reader.ReadInt32();
            for (var itemIndex = 0; itemIndex < itemCount; itemIndex++)
            {
                var id = reader.ReadInt64();
                var itemId = reader.ReadInt32();
                AddNpcItem(npc, id, itemId);
            }

            var portalCount = reader.ReadInt32();
            for (var portalIndex = 0; portalIndex < portalCount; portalIndex++)
            {
                var portalId = reader.ReadInt64();
                var portalType = reader.ReadInt32();
                var portalEntryCount = reader.ReadInt32();
                var groupCount = reader.ReadInt32();
                var resourceCount = 0;
                var portal = new NpcPortalEditorRow
                {
                    Id = portalId,
                    PortalType = portalType,
                    PortalCount = portalEntryCount
                };

                for (var groupIndex = 0; groupIndex < groupCount; groupIndex++)
                {
                    var group = new NpcPortalGroupEditorRow
                    {
                        Id = reader.ReadInt64()
                    };
                    var resources = reader.ReadInt32();
                    resourceCount += resources;
                    for (var resourceIndex = 0; resourceIndex < resources; resourceIndex++)
                    {
                        group.Resources.Add(new NpcPortalResourceEditorRow
                        {
                            Id = reader.ReadInt64(),
                            ValueA = reader.ReadInt32(),
                            ValueB = reader.ReadInt32(),
                            ValueC = reader.ReadInt32()
                        });
                    }

                    portal.Groups.Add(group);
                }

                portal.ResourceCount = resourceCount;
                npc.Portals.Add(portal);
            }

            if (npc.TypeId == 0)
            {
                if (npc.Portals.Count > 0)
                    npc.TypeId = 3;
            }
        }

        return count;
    }

    private int TryLoadNpcBin(byte[]? bytes)
    {
        if (bytes == null || bytes.Length == 0)
            return 0;

        try
        {
            _npcLoaded = true;
            using var stream = new MemoryStream(bytes);
            using var reader = new BinaryReader(stream);
            var count = reader.ReadInt32();

            for (var i = 0; i < count; i++)
                ReadNpcBinRow(reader);

            _npcTailRaw = bytes.AsSpan((int)stream.Position).ToArray();
            using var tailStream = new MemoryStream(_npcTailRaw);
            using var tailReader = new BinaryReader(tailStream);
            var modelComments = ReadNpcModelInfo(tailReader);
            foreach (var npc in Npcs)
            {
                if (modelComments.TryGetValue(npc.ModelId, out var comment))
                    npc.ModelComment = comment;
            }

            return count;
        }
        catch
        {
            _npcLoaded = false;
            _npcTailRaw = [];
            return 0;
        }
    }

    private int TryLoadMapNpcs(byte[]? bytes)
    {
        if (bytes == null || bytes.Length == 0)
            return 0;

        using var stream = new MemoryStream(bytes);
        using var reader = new BinaryReader(stream);
        var count = reader.ReadInt32();
        if (count < 0)
            return 0;

        _mapNpcLoaded = true;
        var loaded = 0;
        for (var i = 0; i < count; i++)
        {
            if (reader.BaseStream.Position + 20 > reader.BaseStream.Length)
                break;

            var npcId = ReadUInt32AsInt(reader);
            var mapId = ReadUInt32AsInt(reader);
            var positionX = reader.ReadInt32();
            var positionY = reader.ReadInt32();
            var rotation = reader.ReadSingle();

            var npc = GetOrCreateNpc(npcId);
            if (mapId > 0)
                npc.MapId = mapId;
            npc.MapName = ResolveMapName(npc.MapId);
            npc.MapPositionX = positionX;
            npc.MapPositionY = positionY;
            npc.MapRotation = rotation;
            npc.HasMapPosition = true;
            loaded++;
        }

        return loaded;
    }

    private byte[] BuildMapNpcBytes()
    {
        var positionedNpcs = Npcs
            .Where(x => x.NpcId > 0 && x.MapId > 0 && x.HasMapPosition)
            .OrderBy(x => x.NpcId)
            .ToList();

        using var stream = new MemoryStream();
        using var writer = new BinaryWriter(stream);
        writer.Write(positionedNpcs.Count);
        foreach (var npc in positionedNpcs)
        {
            writer.Write(checked((uint)npc.NpcId));
            writer.Write(checked((uint)npc.MapId));
            writer.Write(npc.MapPositionX);
            writer.Write(npc.MapPositionY);
            writer.Write(npc.MapRotation);
        }

        writer.Flush();
        return stream.ToArray();
    }

    private void MarkNpcPositionsClean()
    {
        foreach (var npc in Npcs.Where(x => x.HasMapPosition))
            npc.MarkMapPositionClean();
    }

    private void ReadNpcBinRow(BinaryReader reader)
    {
        var npcId = ReadUInt32AsInt(reader);
        var mapId = ReadUInt32AsInt(reader);
        var typeId = reader.ReadInt32();
        var moveType = reader.ReadInt32();
        var displayFlag = ReadUInt32AsInt(reader);
        var modelId = ReadUInt32AsInt(reader);
        var penName = ReadUtf16Fixed(reader, NpcNameChars);
        var name = ReadUtf16Fixed(reader, NpcNameChars);
        var talk = ReadUtf16Fixed(reader, NpcTalkChars);

        var npc = GetOrCreateNpc(npcId);
        npc.MapId = mapId;
        npc.MapName = ResolveMapName(mapId);
        npc.TypeId = typeId;
        npc.MoveType = moveType;
        npc.DisplayFlag = displayFlag;
        npc.ModelId = modelId;
        npc.PenName = penName;
        npc.Name = name;
        npc.Talk = talk;

        var payloadStart = reader.BaseStream.Position;
        switch (typeId)
        {
            case 1:
            case 8:
            case 9:
            case 12:
            case 14:
            case 25:
            case 16:
                ReadNpcItemList(reader, npc);
                break;
            case 3:
                ReadClientPortalList(reader, npc);
                break;
            case 19:
                ReadSpecialEventPayload(reader, npc);
                break;
            default:
                break;
        }
        npc.NpcTypePayloadRaw = CopyStreamRange(reader.BaseStream, payloadStart, reader.BaseStream.Position);

        var extraCount = reader.ReadInt32();
        for (var extraIndex = 0; extraIndex < extraCount; extraIndex++)
        {
            var extraType = reader.ReadInt32();
            if (extraType != 0)
                throw new InvalidDataException($"Unsupported NPC extra type {extraType}.");

            npc.QuestInitialState = ReadUInt32AsInt(reader);
            var actCount = reader.ReadInt32();
            for (var actIndex = 0; actIndex < actCount; actIndex++)
            {
                var actType = ReadUInt32AsInt(reader);
                var completeState = ReadUInt32AsInt(reader);
                var questCount = reader.ReadInt32();
                for (var questIndex = 0; questIndex < questCount; questIndex++)
                {
                    npc.QuestLinks.Add(new NpcQuestLinkEditorRow
                    {
                        ActType = actType,
                        CompleteState = completeState,
                        QuestId = reader.ReadInt32()
                    });
                }
            }
        }
    }

    private void ReadNpcItemList(BinaryReader reader, NpcEditorRow npc)
    {
        var count = reader.ReadInt32();
        for (var i = 0; i < count; i++)
            AddNpcItem(npc, 0, ReadUInt32AsInt(reader));
    }

    private void ReadClientPortalList(BinaryReader reader, NpcEditorRow npc)
    {
        var portalType = reader.ReadInt32();
        var portalCount = reader.ReadInt32();
        for (var i = 0; i < portalCount; i++)
        {
            _ = ReadUInt32AsInt(reader); // event id
            for (var requirement = 0; requirement < 3; requirement++)
            {
                _ = reader.ReadInt32();
                _ = ReadUInt32AsInt(reader);
                _ = ReadUInt32AsInt(reader);
            }
        }

        if (npc.Portals.Count == 0)
        {
            npc.Portals.Add(new NpcPortalEditorRow
            {
                PortalType = portalType,
                PortalCount = portalCount,
                ResourceCount = 0
            });
        }
    }

    private void ReadSpecialEventPayload(BinaryReader reader, NpcEditorRow npc)
    {
        var eventType = reader.ReadInt32();
        if (eventType != 2)
            return;

        var itemCount = reader.ReadInt32();
        for (var i = 0; i < itemCount; i++)
            AddNpcItem(npc, 0, ReadUInt32AsInt(reader));
    }

    private Dictionary<int, string> ReadNpcModelInfo(BinaryReader reader)
    {
        if (reader.BaseStream.Position + 4 > reader.BaseStream.Length)
            return [];

        var count = reader.ReadInt32();
        var dataStart = reader.BaseStream.Position;
        foreach (var recordSize in new[] { 140, 138 })
        {
            var end = dataStart + (long)count * recordSize;
            if (count < 0 || end > reader.BaseStream.Length)
                continue;

            reader.BaseStream.Position = dataStart;
            var result = new Dictionary<int, string>();
            for (var i = 0; i < count; i++)
            {
                var record = reader.ReadBytes(recordSize);
                if (record.Length != recordSize)
                    break;

                var modelId = BitConverter.ToInt32(record, 0);
                var commentBytes = record.Skip(10).Take(128).ToArray();
                result[modelId] = DecodeUtf16(commentBytes);
            }

            if (reader.BaseStream.Position + 4 <= reader.BaseStream.Length)
            {
                var next = reader.ReadInt32();
                if (next is >= 0 and < 100000)
                    return result;
            }
        }

        reader.BaseStream.Position = dataStart;
        return [];
    }

    private int TryLoadQuests(byte[]? bytes)
    {
        if (bytes == null || bytes.Length == 0)
            return 0;

        _questLoaded = true;
        using var stream = new MemoryStream(bytes);
        using var reader = new BinaryReader(stream);
        var count = reader.ReadInt32();

        for (var i = 0; i < count; i++)
        {
            var quest = ReadQuest(reader);
            Quests.Add(quest);
            _questById[quest.QuestId] = quest;
        }

        foreach (var quest in Quests)
            ResolveQuest(quest);

        return count;
    }

    private QuestEditorRow ReadQuest(BinaryReader reader)
    {
        var quest = new QuestEditorRow
        {
            QuestId = ReadUInt32AsInt(reader),
            ModelIdRecv = ReadUInt32AsInt(reader),
            ModelIdComplete = ReadUInt32AsInt(reader),
            Level = reader.ReadInt16()
        };

        quest.ToolPosX = reader.ReadInt32();
        quest.ToolPosY = reader.ReadInt32();
        quest.ManagedId = ReadUInt32AsInt(reader);
        quest.Active = reader.ReadBoolean();
        quest.Immediate = reader.ReadBoolean();
        quest.Reset = reader.ReadBoolean();
        quest.QuestType = ReadUInt32AsInt(reader);
        quest.StartTargetType = ReadUInt32AsInt(reader);
        quest.StartTargetId = ReadUInt32AsInt(reader);
        quest.TargetType = ReadUInt32AsInt(reader);
        quest.TargetValue = reader.ReadInt32();

        quest.TitleTab = ReadUtf16Bytes(reader, QuestTitleBytes);
        quest.Title = ReadUtf16Bytes(reader, QuestTitleBytes);
        quest.Body = ReadUtf16Bytes(reader, QuestBodyBytes);
        quest.Simple = ReadUtf16Bytes(reader, QuestSimpleBytes);
        quest.Helper = ReadUtf16Bytes(reader, QuestHelperBytes);
        quest.Process = ReadUtf16Bytes(reader, QuestProcessBytes);
        quest.Complete = ReadUtf16Bytes(reader, QuestCompleteBytes);
        quest.Expert = ReadUtf16Bytes(reader, QuestExpertBytes);

        var supplyCount = reader.ReadInt32();
        for (var i = 0; i < supplyCount; i++)
        {
            var supply = new QuestSupplyEditorRow
            {
                Type = ReadUInt32AsInt(reader),
                ItemId = ReadUInt32AsInt(reader),
                Amount = reader.ReadInt32()
            };
            ResolveQuestSupply(supply);
            quest.Supplies.Add(supply);
        }

        var conditionCount = reader.ReadInt32();
        for (var i = 0; i < conditionCount; i++)
        {
            quest.Conditions.Add(new QuestConditionEditorRow
            {
                ConditionType = ReadUInt32AsInt(reader),
                ConditionId = ReadUInt32AsInt(reader),
                ConditionCount = reader.ReadInt32()
            });
        }

        var goalCount = reader.ReadInt32();
        for (var i = 0; i < goalCount; i++)
        {
            quest.Goals.Add(new QuestGoalEditorRow
            {
                GoalType = ReadUInt32AsInt(reader),
                GoalId = ReadUInt32AsInt(reader),
                GoalAmount = reader.ReadInt32(),
                CurTypeCount = ReadUInt32AsInt(reader),
                SubValue = reader.ReadInt32(),
                SubValueTwo = reader.ReadInt32()
            });
        }

        var rewardCount = reader.ReadInt32();
        for (var i = 0; i < rewardCount; i++)
        {
            var reward = new QuestRewardEditorRow
            {
                Method = ReadUInt32AsInt(reader),
                RewardType = ReadUInt32AsInt(reader),
                ValueA = ReadUInt32AsInt(reader),
                ValueB = reader.ReadInt32()
            };

            if (reward.RewardType == 2)
                ResolveQuestReward(reward);

            quest.Rewards.Add(reward);
        }

        var eventCount = reader.ReadInt32();
        for (var i = 0; i < eventCount; i++)
            quest.Events.Add(new QuestEventEditorRow { EventSlot = i, EventId = reader.ReadInt32() });

        return quest;
    }

    private void ResolveQuest(QuestEditorRow quest)
    {
        if (quest.StartTargetType == 0 && _npcById.TryGetValue(quest.StartTargetId, out var startNpc))
        {
            quest.StartNpcName = startNpc.NameTitle;
            quest.ModelRecvSummary = startNpc.VisualSummary;
        }

        if (quest.TargetType == 1 && _npcById.TryGetValue(quest.TargetValue, out var targetNpc))
        {
            quest.TargetNpcName = targetNpc.NameTitle;
            quest.ModelCompleteSummary = targetNpc.VisualSummary;
        }

        foreach (var goal in quest.Goals)
        {
            ResolveQuestGoal(goal);
        }

        foreach (var condition in quest.Conditions)
            ResolveQuestCondition(condition);

        foreach (var supply in quest.Supplies)
            ResolveQuestSupply(supply);

        foreach (var reward in quest.Rewards)
            ResolveQuestReward(reward);
    }

    private void ResolveQuestSupply(QuestSupplyEditorRow supply)
    {
        ResolveItem(supply.ItemId, out var name, out var icon);
        supply.ItemName = name;
        supply.IconImage = icon;
    }

    private void ResolveQuestCondition(QuestConditionEditorRow condition)
    {
        condition.ResolvedName = string.Empty;
        if (condition.ConditionType is 0 or 10 or 11 && _questById.TryGetValue(condition.ConditionId, out var quest))
            condition.ResolvedName = quest.TitleTitle;
        else if (condition.ConditionType == 2 && _items?.FindItem(condition.ConditionId) is { } item)
            condition.ResolvedName = item.NameTitle;
    }

    private void ResolveQuestGoal(QuestGoalEditorRow goal)
    {
        goal.ResolvedName = string.Empty;
        goal.IconImage = null;

        if (goal.GoalType is 1 or 5 or 6 or 7 or 8 && _items?.ResolveItemDisplay(goal.GoalId, out var itemName, out var icon) is { })
        {
            goal.ResolvedName = itemName;
            goal.IconImage = icon;
            return;
        }

        if (goal.GoalType is 4 or 6 && _npcById.TryGetValue(goal.GoalId, out var npc))
        {
            goal.ResolvedName = npc.NameTitle;
            return;
        }
    }

    private void ResolveQuestReward(QuestRewardEditorRow reward)
    {
        reward.ResolvedName = string.Empty;
        reward.IconImage = null;

        if (reward.RewardType == 2 && _items?.ResolveItemDisplay(reward.ValueA, out var itemName, out var icon) is { })
        {
            reward.ResolvedName = itemName;
            reward.IconImage = icon;
        }
    }

    private void ResolveQuestUnlocks()
    {
        foreach (var quest in Quests)
            quest.Unlocks.Clear();

        foreach (var candidate in Quests)
        {
            foreach (var condition in candidate.Conditions.Where(x => x.ConditionType == 0))
            {
                if (!_questById.TryGetValue(condition.ConditionId, out var completedQuest))
                    continue;

                completedQuest.Unlocks.Add(new QuestUnlockEditorRow
                {
                    QuestId = candidate.QuestId,
                    Title = candidate.TitleTitle,
                    QuestTypeName = candidate.QuestTypeName,
                    Level = candidate.Level
                });
            }
        }
    }

    private void ResolveQuestLinks()
    {
        foreach (var npc in Npcs)
        {
            foreach (var link in npc.QuestLinks)
            {
                if (_questById.TryGetValue(link.QuestId, out var quest))
                    link.QuestTitle = quest.TitleTitle;
            }
        }
    }

    private void AttachCraftRecipes(ItemBinWorkspace items)
    {
        foreach (var npcId in items.CraftNpcIds)
        {
            var npc = GetOrCreateNpc(npcId);
            if (npc.TypeId == 0)
                npc.TypeId = 20;
        }

        foreach (var npc in Npcs)
        {
            npc.CraftRecipes.Clear();
            foreach (var recipe in items.GetCraftRecipes(npc.NpcId))
                npc.CraftRecipes.Add(recipe);

            if (npc.CraftRecipes.Count > 0 && npc.TypeId == 0)
                npc.TypeId = 20;
        }
    }

    private NpcEditorRow GetOrCreateNpc(int npcId)
    {
        if (_npcById.TryGetValue(npcId, out var npc))
            return npc;

        npc = new NpcEditorRow
        {
            NpcId = npcId,
            Name = $"NPC {npcId}"
        };
        _npcById[npcId] = npc;
        Npcs.Add(npc);
        return npc;
    }

    private string ResolveMapName(int mapId)
        => _mapNames.TryGetValue(mapId, out var name) ? name : string.Empty;

    private void AddNpcItem(NpcEditorRow npc, long id, int itemId)
    {
        if (npc.Items.Any(x => x.ItemId == itemId))
            return;

        var item = ResolveItem(itemId, out var itemName, out var icon);
        var eventPriceId = item == null ? 0 : BinLayout.ReadInt32(item.RawRecord, BinLayout.EventPriceId);
        var eventPriceAmount = item == null ? 0 : BinLayout.ReadUInt16(item.RawRecord, BinLayout.EventPriceAmount);
        var digicorePrice = item == null ? 0 : BinLayout.ReadUInt16(item.RawRecord, BinLayout.DigicorePrice);
        var eventPriceName = string.Empty;
        if (eventPriceId > 0)
            ResolveItem(eventPriceId, out eventPriceName, out _);

        npc.Items.Add(new NpcItemEditorRow
        {
            Id = id,
            ItemId = itemId,
            ItemName = itemName,
            IconImage = icon,
            SellPrice = item?.SellPrice ?? 0,
            ScanPrice = item?.ScanPrice ?? 0,
            DigicorePrice = digicorePrice,
            EventPriceId = eventPriceId,
            EventPriceAmount = eventPriceAmount,
            EventPriceName = eventPriceName
        });
    }

    private ItemEditorRow? ResolveItem(int itemId, out string itemName, out System.Windows.Media.ImageSource? icon)
    {
        if (_items == null)
        {
            itemName = string.Empty;
            icon = null;
            return null;
        }

        return _items.ResolveItemDisplay(itemId, out itemName, out icon);
    }

    private byte[]? ReadOptionalBin(string sourcePath, string binDirectory, string fileName)
    {
        var direct = Path.Combine(binDirectory, fileName);
        if (File.Exists(direct))
        {
            CaptureNpcDirect(fileName, direct);
            CaptureNpcAssetsDirect(fileName, direct);
            CaptureQuestDirect(fileName, direct);
            CaptureMapNpcDirect(fileName, direct);
            return File.ReadAllBytes(direct);
        }

        var notUsedDirect = Path.Combine(binDirectory, "notused", fileName);
        if (File.Exists(notUsedDirect))
        {
            CaptureNpcDirect(fileName, notUsedDirect);
            CaptureNpcAssetsDirect(fileName, notUsedDirect);
            CaptureQuestDirect(fileName, notUsedDirect);
            CaptureMapNpcDirect(fileName, notUsedDirect);
            return File.ReadAllBytes(notUsedDirect);
        }

        foreach (var packBase in PackCandidates(sourcePath))
        {
            foreach (var relative in CandidateRelativePaths(fileName))
            {
                try
                {
                    var bytes = PackArchive.ReadFile(packBase, relative);
                    CaptureNpcPack(fileName, packBase, relative);
                    CaptureNpcAssetsPack(fileName, packBase, relative);
                    CaptureQuestPack(fileName, packBase, relative);
                    CaptureMapNpcPack(fileName, packBase, relative);
                    return bytes;
                }
                catch
                {
                }
            }
        }

        return null;
    }

    private void CaptureNpcDirect(string fileName, string path)
    {
        if (!fileName.Equals("Npc.bin", StringComparison.OrdinalIgnoreCase))
            return;

        _npcDirectPath = path;
        _npcPackBasePath = string.Empty;
        _npcPackRelativePath = NpcRelativePath;
        _npcLoadedFromPack = false;
    }

    private void CaptureNpcPack(string fileName, string packBase, string relative)
    {
        if (!fileName.Equals("Npc.bin", StringComparison.OrdinalIgnoreCase))
            return;

        _npcDirectPath = string.Empty;
        _npcPackBasePath = packBase;
        _npcPackRelativePath = relative;
        _npcLoadedFromPack = true;
    }

    private void CaptureNpcAssetsDirect(string fileName, string path)
    {
        if (!fileName.Equals("NpcAssets.bin", StringComparison.OrdinalIgnoreCase))
            return;

        _npcAssetsDirectPath = path;
        _npcAssetsPackBasePath = string.Empty;
        _npcAssetsPackRelativePath = NpcAssetsRelativePath;
        _npcAssetsLoadedFromPack = false;
    }

    private void CaptureNpcAssetsPack(string fileName, string packBase, string relative)
    {
        if (!fileName.Equals("NpcAssets.bin", StringComparison.OrdinalIgnoreCase))
            return;

        _npcAssetsDirectPath = string.Empty;
        _npcAssetsPackBasePath = packBase;
        _npcAssetsPackRelativePath = relative;
        _npcAssetsLoadedFromPack = true;
    }

    private void CaptureQuestDirect(string fileName, string path)
    {
        if (!fileName.Equals("Quest.bin", StringComparison.OrdinalIgnoreCase))
            return;

        _questDirectPath = path;
        _questPackBasePath = string.Empty;
        _questPackRelativePath = QuestRelativePath;
        _questLoadedFromPack = false;
    }

    private void CaptureQuestPack(string fileName, string packBase, string relative)
    {
        if (!fileName.Equals("Quest.bin", StringComparison.OrdinalIgnoreCase))
            return;

        _questDirectPath = string.Empty;
        _questPackBasePath = packBase;
        _questPackRelativePath = relative;
        _questLoadedFromPack = true;
    }

    private void CaptureMapNpcDirect(string fileName, string path)
    {
        if (!fileName.Equals("MapNpc.bin", StringComparison.OrdinalIgnoreCase))
            return;

        _mapNpcDirectPath = path;
        _mapNpcPackBasePath = string.Empty;
        _mapNpcPackRelativePath = MapNpcRelativePath;
        _mapNpcLoadedFromPack = false;
    }

    private void CaptureMapNpcPack(string fileName, string packBase, string relative)
    {
        if (!fileName.Equals("MapNpc.bin", StringComparison.OrdinalIgnoreCase))
            return;

        _mapNpcDirectPath = string.Empty;
        _mapNpcPackBasePath = packBase;
        _mapNpcPackRelativePath = relative;
        _mapNpcLoadedFromPack = true;
    }

    private static IEnumerable<string> CandidateRelativePaths(string fileName)
    {
        yield return fileName;
        yield return $@"notused\{fileName}";
        yield return $@"data\bin\english\{fileName}";
        yield return $@"data\bin\english\notused\{fileName}";
        yield return $@"bin\english\{fileName}";
        yield return $@"bin\english\notused\{fileName}";
        yield return $@"data\bin\{fileName}";
        yield return $@"data\bin\notused\{fileName}";
        yield return $@"bin\{fileName}";
        yield return $@"bin\notused\{fileName}";
    }

    private static string? FindOptionalStringXml(string sourcePath, string binDirectory, string fileName)
    {
        foreach (var path in CandidateStringXmlPaths(sourcePath, binDirectory, fileName))
        {
            if (File.Exists(path))
                return path;
        }

        return null;
    }

    private static IEnumerable<string> CandidateStringXmlPaths(string sourcePath, string binDirectory, string fileName)
    {
        yield return Path.Combine(binDirectory, fileName);
        yield return Path.Combine(binDirectory, "notused", fileName);
        yield return Path.Combine(binDirectory, "_str", fileName);

        var sourceDirectory = Directory.Exists(sourcePath) ? sourcePath : Path.GetDirectoryName(sourcePath);
        if (!string.IsNullOrWhiteSpace(sourceDirectory))
        {
            yield return Path.Combine(sourceDirectory, fileName);
            yield return Path.Combine(sourceDirectory, "_str", fileName);
            yield return Path.Combine(sourceDirectory, "source_xml_language", "_str", fileName);
        }

        yield return Path.Combine("C:\\0-NewDmo", "TamerBinMigrationFeature", "source_xml_language", "_str", fileName);
    }

    private static string BestMapName(int mapId, params string[] candidates)
    {
        foreach (var candidate in candidates)
        {
            var name = CleanMapName(candidate);
            if (HasUsefulMapName(name, mapId))
                return name;
        }

        return CleanMapName(candidates.FirstOrDefault()) ?? string.Empty;
    }

    private static bool HasUsefulMapName(string? value, int mapId)
    {
        var name = CleanMapName(value);
        if (string.IsNullOrWhiteSpace(name))
            return false;

        return !name.Equals(mapId.ToString(), StringComparison.OrdinalIgnoreCase) &&
               !name.Equals($"Map {mapId}", StringComparison.OrdinalIgnoreCase);
    }

    private static string CleanMapName(string? value)
        => (value ?? string.Empty)
            .Replace("\\n", " ")
            .Replace("\r\n", " ")
            .Replace('\r', ' ')
            .Replace('\n', ' ')
            .Trim();

    private static string ZoneTextureName(string? mapPath)
    {
        if (string.IsNullOrWhiteSpace(mapPath))
            return string.Empty;

        var normalized = mapPath.Replace('/', '\\');
        var fileName = Path.GetFileName(normalized);
        if (string.IsNullOrWhiteSpace(fileName))
            return string.Empty;

        var stem = Path.GetFileNameWithoutExtension(fileName);
        return string.IsNullOrWhiteSpace(stem) ? string.Empty : $"{stem}_Zone.tga";
    }

    private static void AddPathDerivedMapTextureCandidates(List<string> candidates, string mapPath)
    {
        var normalized = mapPath.Replace('/', '\\');
        var directory = Path.GetDirectoryName(normalized)?.Replace('/', '\\') ?? string.Empty;
        var stem = Path.GetFileNameWithoutExtension(normalized);
        if (string.IsNullOrWhiteSpace(stem))
            return;

        foreach (var suffix in new[] { "_Zone.tga", "_Mini.tga" })
        {
            var texture = $"{stem}{suffix}";
            AddInterfaceMapTextureCandidates(candidates, texture);

            if (!string.IsNullOrWhiteSpace(directory))
            {
                candidates.Add($@"{directory}\{texture}");
                if (directory.StartsWith(@"Data\", StringComparison.OrdinalIgnoreCase))
                    candidates.Add($@"{directory[5..]}\{texture}");
            }
        }
    }

    private static void AddInterfaceMapTextureCandidates(List<string> candidates, string texture)
    {
        if (string.IsNullOrWhiteSpace(texture))
            return;

        candidates.Add($@"data\interface\Map\__Map_Eng\{texture}");
        candidates.Add($@"data\interface\Map\{texture}");
        candidates.Add($@"Map\__Map_Eng\{texture}");
        candidates.Add($@"Map\{texture}");
        candidates.Add(texture);
    }

    private static void AddAreaMapTextureCandidates(List<string> candidates, int areaId)
    {
        if (areaId <= 0)
            return;

        var texture = $"Area{areaId}.tga";
        candidates.Add($@"data\interface\Map\__Map_Eng\AreaMap\{texture}");
        candidates.Add($@"data\interface\Map\AreaMap\{texture}");
        candidates.Add($@"Map\__Map_Eng\AreaMap\{texture}");
        candidates.Add($@"Map\AreaMap\{texture}");
    }

    private static IEnumerable<string> PackCandidates(string sourcePath)
    {
        yield return sourcePath;
        yield return Path.Combine("C:\\0-NewDmo", "N\u00e3oUsarEsseClient", "Data", "Pack01");
        yield return Path.Combine("C:\\0-NewDmo", "N\u00e3oUsarEsseClient", "Data", "Pack03");
        yield return Path.Combine("C:\\0-NewDmo", "NãoUsarEsseClient", "Data", "Pack01");
        yield return Path.Combine("C:\\0-NewDmo", "NãoUsarEsseClient", "Data", "Pack03");
        yield return Path.Combine("C:\\0-NewDmo", "NãoUsarEsseClient", "Data", "Pack01");
        yield return Path.Combine("C:\\0-NewDmo", "NãoUsarEsseClient", "Data", "Pack03");
        yield return Path.Combine("C:\\0-NewDmo", "ClientDist", "Data", "Pack01");
        yield return Path.Combine("C:\\0-NewDmo", "ClientDist", "Data", "Pack03");
    }

    private static byte[] CopyStreamRange(Stream stream, long start, long end)
    {
        var length = checked((int)(end - start));
        if (length <= 0)
            return [];

        var originalPosition = stream.Position;
        stream.Position = start;
        var bytes = new byte[length];
        var read = stream.Read(bytes, 0, bytes.Length);
        stream.Position = originalPosition;
        if (read != bytes.Length)
            throw new EndOfStreamException("Could not copy NPC payload bytes.");

        return bytes;
    }

    private static int ReadUInt32AsInt(BinaryReader reader)
        => checked((int)reader.ReadUInt32());

    private static string ReadUtf16Fixed(BinaryReader reader, int charCount)
        => DecodeUtf16(reader.ReadBytes(charCount * 2));

    private static string ReadUtf16Bytes(BinaryReader reader, int byteCount)
        => DecodeUtf16(reader.ReadBytes(byteCount));

    private static void WriteUtf16Bytes(BinaryWriter writer, string value, int byteCount)
    {
        var buffer = new byte[byteCount];
        var maxBytes = Math.Max(0, byteCount - 2);
        if (maxBytes % 2 != 0)
            maxBytes--;

        var encoded = Encoding.Unicode.GetBytes(value ?? string.Empty);
        var copyBytes = Math.Min(encoded.Length, maxBytes);
        if (copyBytes % 2 != 0)
            copyBytes--;

        Array.Copy(encoded, 0, buffer, 0, copyBytes);
        writer.Write(buffer);
    }

    private static string ReadSizedString(BinaryReader reader)
    {
        var length = reader.ReadInt32();
        if (length <= 0)
            return string.Empty;

        var bytes = reader.ReadBytes(length);
        return Encoding.UTF8.GetString(bytes).TrimEnd('\0').Trim();
    }

    private static string ReadSizedUnicodeString(BinaryReader reader)
    {
        var length = reader.ReadInt32();
        if (length <= 0)
            return string.Empty;

        return DecodeUtf16(reader.ReadBytes(length * 2));
    }

    private static string DecodeUtf16(byte[] bytes)
    {
        var value = Encoding.Unicode.GetString(bytes);
        var terminator = value.IndexOf('\0');
        if (terminator >= 0)
            value = value[..terminator];

        return value.Trim();
    }
}
