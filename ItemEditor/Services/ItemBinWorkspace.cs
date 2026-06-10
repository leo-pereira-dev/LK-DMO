using System.Collections.ObjectModel;
using System.Globalization;
using System.IO;
using System.Text;
using System.Text.Json;
using ItemEditor.Models;

namespace ItemEditor.Services;

public sealed class ItemBinWorkspace
{
    private const string ItemListRelativePath = @"data\bin\english\ItemList.bin";
    private const string SkillRelativePath = @"data\bin\english\Skill.bin";
    private const string BuffRelativePath = @"data\bin\english\Buff.bin";
    private const string ContainerRelativePath = @"data\bin\english\Container.bin";
    private static readonly string[] PackBinPrefixes = [@"data\bin\english", @"bin\english", @"data\bin", "bin", ""];

    private const uint ContainerMagic = 0x4E54434C;
    private const uint ContainerVersion = 1;
    private const int SkillRecordSize = 736;
    private const int BuffRecordSize = 476;

    private readonly Dictionary<int, SkillEditorRow> _skills = [];
    private readonly Dictionary<int, BuffEditorRow> _buffs = [];
    private readonly Dictionary<int, ContainerEditorRow> _containers = [];
    private readonly List<SkillEditorRow> _skillRows = [];
    private readonly List<BuffEditorRow> _buffRows = [];
    private readonly IconAtlasService _iconAtlas = new();

    private string _itemListPath = string.Empty;
    private string _sourcePath = string.Empty;
    private string _itemListPackRelativePath = ItemListRelativePath;
    private string _skillPackRelativePath = SkillRelativePath;
    private string _buffPackRelativePath = BuffRelativePath;
    private string _containerPackRelativePath = ContainerRelativePath;
    private bool _sourceIsPack;
    private bool _skillLoaded;
    private bool _buffLoaded;
    private bool _containerLoaded;
    private byte[] _itemListTail = [];
    private byte[] _itemListTailBeforeCraft = [];
    private byte[] _itemListTailAfterCraft = [];
    private byte[] _skillTail = [];
    private bool _craftTableLoaded;
    private string _containerSqlSourcePath = string.Empty;
    private int _containerSqlLoadedCount;
    private readonly Dictionary<int, ObservableCollection<CraftRecipeEditorRow>> _craftRecipesByNpc = [];

    public ObservableCollection<ItemEditorRow> Items { get; } = [];
    public string BinDirectory { get; private set; } = string.Empty;
    public string StatusSummary { get; private set; } = "No bins loaded.";

    public void Load(string sourcePath)
    {
        _sourcePath = sourcePath;
        _sourceIsPack = PackArchive.LooksLikePack(sourcePath);
        BinDirectory = _sourceIsPack ? PackArchive.ResolveBasePath(sourcePath) : sourcePath;
        _itemListPath = _sourceIsPack ? string.Empty : Path.Combine(sourcePath, "ItemList.bin");
        _iconAtlas.Configure(sourcePath);

        Items.Clear();
        _skills.Clear();
        _buffs.Clear();
        _containers.Clear();
        _skillRows.Clear();
        _buffRows.Clear();
        _itemListTail = [];
        _itemListTailBeforeCraft = [];
        _itemListTailAfterCraft = [];
        _skillTail = [];
        _craftTableLoaded = false;
        _craftRecipesByNpc.Clear();
        _skillLoaded = false;
        _buffLoaded = false;
        _containerLoaded = false;
        _containerSqlSourcePath = string.Empty;
        _containerSqlLoadedCount = 0;

        LoadItems(ReadBinBytes("ItemList.bin", ItemListRelativePath));
        TryLoadSkills(ReadOptionalBinBytes("Skill.bin", SkillRelativePath));
        TryLoadBuffs(ReadOptionalBinBytes("Buff.bin", BuffRelativePath));
        TryLoadContainers(ReadOptionalBinBytes("Container.bin", ContainerRelativePath));
        TryLoadContainersFromSqlBackup();
        DecorateLinkedRows();

        foreach (var item in Items)
        {
            Classify(item);
            item.MarkClean();
        }

        var sourceKind = _sourceIsPack ? $"pack {Path.GetFileName(BinDirectory)}" : "bin folder";
        var sqlFallback = _containerSqlLoadedCount > 0
            ? $" ({_containerSqlLoadedCount:N0} filled from {Path.GetFileName(_containerSqlSourcePath)})"
            : string.Empty;
        StatusSummary = $"Loaded {Items.Count:N0} items, {_skills.Count:N0} skills, {_buffs.Count:N0} buffs, {_containers.Count:N0} boxes{sqlFallback} from {sourceKind}.";
    }

    public ItemEditorRow Duplicate(ItemEditorRow? source)
    {
        source ??= Items.FirstOrDefault();
        if (source == null)
            throw new InvalidOperationException("Load ItemList.bin before adding items.");

        var nextId = Math.Max(Items.Max(x => x.ItemId) + 1, 1);
        var row = Clone(source);
        row.ItemId = nextId;
        row.Name = $"New Item {nextId}";
        row.Description = string.Empty;
        row.IsNew = true;
        row.IsDirty = true;
        Items.Add(row);
        Classify(row);
        RefreshIcon(row);
        return row;
    }

    public ItemEditorRow CreateNewItem()
    {
        if (Items.Count == 0)
            throw new InvalidOperationException("Load ItemList.bin before adding items.");

        var nextId = Math.Max(Items.Max(x => x.ItemId) + 1, 1);
        var row = new ItemEditorRow
        {
            RawRecord = new byte[BinLayout.ItemInfoRecordSize],
            ItemId = nextId,
            Name = $"New Item {nextId}",
            Description = string.Empty,
            TypeComment = "Generic item",
            Icon = 0,
            Class = 0,
            TypeL = 86,
            TypeS = 0,
            TypeValue = 0,
            Section = 0,
            SellType = 1,
            UseMode = 0,
            CooldownGroup = 0,
            Overlap = 999,
            SkillCode = 0,
            ApplyValueMin = 0,
            ApplyValueMax = 0,
            ApplyElement = 0,
            BoundType = 0,
            UseTimeType = 0,
            UsageTimeMinutes = 0,
            ScanPrice = 0,
            SellPrice = 0,
            IsNew = true,
            IsDirty = true
        };

        Items.Add(row);
        Classify(row);
        RefreshIcon(row);
        return row;
    }

    public void Reclassify(ItemEditorRow row)
    {
        DecorateLinkedRows();
        Classify(row);
        RefreshIcon(row);
    }

    public SkillEditorRow? ResolveSkill(ItemEditorRow? item)
        => item?.SkillCode > 0 && _skills.TryGetValue((int)item.SkillCode, out var skill) ? skill : null;

    public BuffEditorRow? ResolveBuff(ItemEditorRow? item)
    {
        var skill = ResolveSkill(item);
        if (skill == null)
            return null;

        var buffCode = new[] { skill.Apply0.BuffCode, skill.Apply1.BuffCode, skill.Apply2.BuffCode }
            .FirstOrDefault(x => x > 0);

        return buffCode > 0 && _buffs.TryGetValue(buffCode, out var buff) ? buff : null;
    }

    public ContainerEditorRow? ResolveContainer(ItemEditorRow? item)
        => item != null && _containers.TryGetValue(item.ItemId, out var container) ? container : null;

    public IReadOnlyList<ContainerEditorRow> DirtyContainers()
        => _containers.Values
            .Where(IsContainerDirty)
            .OrderBy(x => x.ItemId)
            .ToList();

    public ItemKindOption ResolveKind(ItemEditorRow item)
        => ItemKindCatalog.Infer(item, _containers.ContainsKey(item.ItemId));

    public ItemEditorRow? FindItem(int itemId)
        => Items.FirstOrDefault(x => x.ItemId == itemId);

    public ItemEditorRow? ResolveItemDisplay(int itemId, out string itemName, out System.Windows.Media.ImageSource? icon)
    {
        var item = FindItem(itemId);
        itemName = item?.NameTitle ?? string.Empty;
        icon = item == null ? null : item.IconImage ?? _iconAtlas.ResolveItemIcon(item.Icon, item.Class).image;
        return item;
    }

    public IEnumerable<int> CraftNpcIds => _craftRecipesByNpc.Keys;

    public ObservableCollection<CraftRecipeEditorRow> GetCraftRecipes(int npcId)
    {
        if (_craftRecipesByNpc.TryGetValue(npcId, out var recipes))
            return recipes;

        recipes = [];
        _craftRecipesByNpc[npcId] = recipes;
        return recipes;
    }

    public CraftRecipeEditorRow AddCraftRecipe(int npcId, ItemEditorRow? resultItem = null)
    {
        var recipes = GetCraftRecipes(npcId);
        var template = recipes.FirstOrDefault();
        var nextId = Math.Max(_craftRecipesByNpc.Values.SelectMany(x => x).Select(x => x.SequencialId).DefaultIfEmpty(0).Max() + 1, 1);
        var recipe = new CraftRecipeEditorRow
        {
            NpcId = npcId,
            SequencialId = nextId,
            MainCategoryId = template?.MainCategoryId ?? 1,
            MainCategoryName = template?.MainCategoryName ?? "Default",
            SubCategoryId = template?.SubCategoryId ?? 1,
            SubCategoryName = template?.SubCategoryName ?? "Default",
            ItemId = resultItem?.ItemId ?? 0,
            Amount = 1,
            SuccessRate = 100,
            Price = 0,
            PercentageAssistGroup = template?.PercentageAssistGroup ?? 0,
            ProtectAssistGroup = template?.ProtectAssistGroup ?? 0,
            IsDirty = true
        };

        ResolveCraftRecipeDisplay(recipe);
        recipes.Add(recipe);
        _craftTableLoaded = true;
        return recipe;
    }

    public CraftMaterialEditorRow AddCraftMaterial(CraftRecipeEditorRow recipe, ItemEditorRow? materialItem = null)
    {
        var material = new CraftMaterialEditorRow
        {
            ItemId = materialItem?.ItemId ?? 0,
            Amount = 1,
            IsDirty = true
        };

        ResolveCraftMaterialDisplay(material);
        recipe.Materials.Add(material);
        recipe.IsDirty = true;
        recipe.RefreshMaterialsSummary();
        return material;
    }

    public void ResolveCraftRecipeDisplay(CraftRecipeEditorRow recipe)
    {
        ResolveItem(recipe.ItemId, out var itemName, out var icon);
        recipe.ItemName = itemName;
        recipe.IconImage = icon;
        foreach (var material in recipe.Materials)
            ResolveCraftMaterialDisplay(material);
        recipe.RefreshMaterialsSummary();
    }

    public ContainerEditorRow EnsureContainer(ItemEditorRow item)
    {
        if (_containers.TryGetValue(item.ItemId, out var existing))
            return existing;

        var nextId = Math.Max(_containers.Values.Select(x => x.Id).DefaultIfEmpty(0).Max() + 1, 1);
        var container = new ContainerEditorRow
        {
            Id = nextId,
            ItemId = item.ItemId,
            ItemName = item.NameTitle,
            RewardAmount = 1,
            IsDirty = true
        };

        _containers[container.ItemId] = container;
        _containerLoaded = true;
        return container;
    }

    public ContainerRewardEditorRow AddContainerReward(ItemEditorRow boxItem, ItemEditorRow? rewardItem)
    {
        var container = EnsureContainer(boxItem);
        var nextId = Math.Max(
            _containers.Values.SelectMany(x => x.Rewards).Select(x => x.Id).DefaultIfEmpty(0).Max() + 1,
            1);

        var reward = new ContainerRewardEditorRow
        {
            Id = nextId,
            ItemId = rewardItem?.ItemId ?? 0,
            ItemName = rewardItem?.NameTitle ?? string.Empty,
            MinAmount = 1,
            MaxAmount = 1,
            Chance = 100,
            Rare = false,
            ContainerAssetId = container.Id,
            IsDirty = true
        };

        container.Rewards.Add(reward);
        container.IsDirty = true;
        container.RefreshRewardSummary();
        return reward;
    }

    public void RemoveContainerReward(ItemEditorRow boxItem, ContainerRewardEditorRow reward)
    {
        var container = EnsureContainer(boxItem);
        if (container.Rewards.Remove(reward))
        {
            container.IsDirty = true;
            container.RefreshRewardSummary();
        }
    }

    public int MergeContainersFromDatabase(IReadOnlyList<ContainerEditorRow> containers)
    {
        foreach (var container in containers)
        {
            container.MarkClean();
            foreach (var reward in container.Rewards)
                reward.MarkClean();

            _containers[container.ItemId] = container;
        }

        _containerLoaded = _containerLoaded || containers.Count > 0;

        foreach (var item in Items)
            Classify(item);

        return containers.Count;
    }

    public IReadOnlyList<ItemEditorRow> SearchRewardItems(string filter, int maxResults = 160)
    {
        var query = filter.Trim();
        IEnumerable<ItemEditorRow> items = Items;
        if (query.Length > 0)
        {
            items = items.Where(item =>
                item.ItemId.ToString().Contains(query, StringComparison.OrdinalIgnoreCase) ||
                item.Name.Contains(query, StringComparison.OrdinalIgnoreCase) ||
                item.Description.Contains(query, StringComparison.OrdinalIgnoreCase) ||
                item.TypeComment.Contains(query, StringComparison.OrdinalIgnoreCase) ||
                item.EffectKind.Contains(query, StringComparison.OrdinalIgnoreCase));
        }

        return items.OrderBy(x => x.ItemId)
            .ThenBy(x => x.Name, StringComparer.OrdinalIgnoreCase)
            .Take(maxResults)
            .ToList();
    }

    public IReadOnlyList<SkillEditorRow> SearchSkills(string filter, int maxResults = 120)
    {
        var query = filter.Trim();
        IEnumerable<SkillEditorRow> skills = _skillRows;
        if (query.Length > 0)
        {
            skills = skills.Where(skill =>
                skill.Id.ToString().Contains(query, StringComparison.OrdinalIgnoreCase) ||
                skill.Name.Contains(query, StringComparison.OrdinalIgnoreCase) ||
                skill.Description.Contains(query, StringComparison.OrdinalIgnoreCase) ||
                skill.VisualSummary.Contains(query, StringComparison.OrdinalIgnoreCase) ||
                skill.Applies.Any(apply =>
                    apply.ApplyLabel.Contains(query, StringComparison.OrdinalIgnoreCase) ||
                    apply.ApplyName.Contains(query, StringComparison.OrdinalIgnoreCase) ||
                    apply.EffectSentence.Contains(query, StringComparison.OrdinalIgnoreCase)));
        }

        return skills.OrderBy(x => x.Id)
            .ThenBy(x => x.Name, StringComparer.OrdinalIgnoreCase)
            .Take(maxResults)
            .ToList();
    }

    public SkillEditorRow DuplicateSkillForItem(ItemEditorRow item)
    {
        if (!_skillLoaded)
            throw new InvalidOperationException("Skill.bin is not loaded.");

        var source = ResolveSkill(item) ?? _skillRows.FirstOrDefault();
        if (source == null)
            throw new InvalidOperationException("Skill.bin does not contain a skill to clone.");

        var nextId = Math.Max(_skillRows.Max(x => x.Id) + 1, 1);
        var skill = CloneSkill(source);
        skill.Id = nextId;
        skill.Name = string.IsNullOrWhiteSpace(source.Name) ? $"New Skill {nextId}" : $"{source.Name} Copy";
        skill.IsDirty = true;
        _skillRows.Add(skill);
        _skills[skill.Id] = skill;
        item.SkillCode = skill.Id;
        DecorateLinkedRows();
        Classify(item);
        return skill;
    }

    public string Save()
    {
        if (string.IsNullOrWhiteSpace(_sourcePath))
            throw new InvalidOperationException("No ItemList.bin is loaded.");

        var changed = new Dictionary<string, byte[]>(StringComparer.OrdinalIgnoreCase)
        {
            [_itemListPackRelativePath] = BuildItemListBytes()
        };

        if (_skillLoaded && IsAnySkillDirty())
            changed[_skillPackRelativePath] = BuildSkillBytes();

        if (_buffLoaded && _buffRows.Any(x => x.IsDirty))
            changed[_buffPackRelativePath] = BuildBuffBytes();

        if (_containerLoaded && _containers.Values.Any(IsContainerDirty))
            changed[_containerPackRelativePath] = BuildContainerBytes();

        if (_sourceIsPack)
        {
            PackArchive.PatchBytes(_sourcePath, changed);
            MarkAllClean();
            return $"Saved {changed.Count} bin(s) back into pack.";
        }

        foreach (var pair in changed)
        {
            var fileName = Path.GetFileName(pair.Key);
            var path = Path.Combine(BinDirectory, fileName);
            if (!File.Exists(path))
                throw new FileNotFoundException($"{fileName} was not found for chained save.", path);

            var backup = path + "." + DateTime.Now.ToString("yyyyMMddHHmmss") + ".bak";
            File.Copy(path, backup, overwrite: false);
            File.WriteAllBytes(path, pair.Value);
        }

        MarkAllClean();
        return $"Saved {changed.Count} bin(s): {string.Join(", ", changed.Keys.Select(Path.GetFileName))}.";
    }

    public void ExportJson(string path)
    {
        var payload = Items.Select(x => new
        {
            x.ItemId,
            x.Name,
            x.Description,
            x.TypeComment,
            x.Icon,
            x.Class,
            x.TypeL,
            x.TypeS,
            x.TypeValue,
            x.Section,
            x.SellType,
            x.UseMode,
            x.CooldownGroup,
            x.Overlap,
            x.SkillCode,
            x.ApplyValueMin,
            x.ApplyValueMax,
            x.ApplyElement,
            x.BoundType,
            x.UseTimeType,
            x.UsageTimeMinutes,
            x.ScanPrice,
            x.SellPrice,
            x.EffectKind,
            x.EffectSummary
        }).ToList();

        File.WriteAllText(path, JsonSerializer.Serialize(payload, new JsonSerializerOptions { WriteIndented = true }));
    }

    private byte[] ReadBinBytes(string fileName, string packRelativePath)
    {
        if (_sourceIsPack)
            return ReadPackBin(fileName, packRelativePath);

        var path = Path.Combine(BinDirectory, fileName);
        if (!File.Exists(path))
            throw new FileNotFoundException($"{fileName} was not found.", path);

        return File.ReadAllBytes(path);
    }

    private byte[]? ReadOptionalBinBytes(string fileName, string packRelativePath)
    {
        try
        {
            if (_sourceIsPack)
                return ReadPackBin(fileName, packRelativePath);

            var path = Path.Combine(BinDirectory, fileName);
            return File.Exists(path) ? File.ReadAllBytes(path) : null;
        }
        catch
        {
            return null;
        }
    }

    private byte[] ReadPackBin(string fileName, string preferredRelativePath)
    {
        var candidates = new[] { preferredRelativePath }
            .Concat(PackBinPrefixes.Select(prefix => string.IsNullOrEmpty(prefix) ? fileName : $@"{prefix}\{fileName}"))
            .Distinct(StringComparer.OrdinalIgnoreCase);

        Exception? last = null;
        foreach (var candidate in candidates)
        {
            try
            {
                var data = PackArchive.ReadFile(_sourcePath, candidate);
                if (fileName.Equals("ItemList.bin", StringComparison.OrdinalIgnoreCase)) _itemListPackRelativePath = candidate;
                if (fileName.Equals("Skill.bin", StringComparison.OrdinalIgnoreCase)) _skillPackRelativePath = candidate;
                if (fileName.Equals("Buff.bin", StringComparison.OrdinalIgnoreCase)) _buffPackRelativePath = candidate;
                if (fileName.Equals("Container.bin", StringComparison.OrdinalIgnoreCase)) _containerPackRelativePath = candidate;
                return data;
            }
            catch (FileNotFoundException ex)
            {
                last = ex;
            }
        }

        throw last ?? new FileNotFoundException($"Pack entry not found: {fileName}");
    }

    private void LoadItems(byte[] bytes)
    {
        var count = BitConverter.ToInt32(bytes, 0);
        var cursor = sizeof(int);
        var itemBytes = checked(count * BinLayout.ItemInfoRecordSize);
        if (cursor + itemBytes > bytes.Length)
            throw new InvalidDataException($"ItemList.bin does not match v487 item record size {BinLayout.ItemInfoRecordSize}.");

        var rows = new List<ItemEditorRow>(count);
        for (var i = 0; i < count; i++)
        {
            var raw = bytes.AsSpan(cursor, BinLayout.ItemInfoRecordSize).ToArray();
            cursor += BinLayout.ItemInfoRecordSize;
            rows.Add(ParseItem(raw));
        }

        foreach (var row in rows.OrderBy(x => x.ItemId).ThenBy(x => x.Name, StringComparer.OrdinalIgnoreCase))
            Items.Add(row);

        _itemListTail = bytes.AsSpan(cursor).ToArray();
        TryLoadCraftTableFromItemListTail();
    }

    private static ItemEditorRow ParseItem(byte[] raw)
    {
        return new ItemEditorRow
        {
            RawRecord = raw,
            ItemId = BinLayout.ReadInt32(raw, BinLayout.ItemId),
            Name = BinLayout.ReadUtf16(raw, BinLayout.Name, 64),
            Description = BinLayout.ReadUtf16(raw, BinLayout.Comment, 512),
            TypeComment = BinLayout.ReadUtf16(raw, BinLayout.TypeComment, 64),
            Icon = BinLayout.ReadInt32(raw, BinLayout.Icon),
            Class = BinLayout.ReadUInt16(raw, BinLayout.Class),
            TypeL = BinLayout.ReadUInt16(raw, BinLayout.TypeL),
            TypeS = BinLayout.ReadUInt16(raw, BinLayout.TypeS),
            TypeValue = BinLayout.ReadInt32(raw, BinLayout.TypeValue),
            Section = BinLayout.ReadInt32(raw, BinLayout.Section),
            SellType = BinLayout.ReadUInt16(raw, BinLayout.SellType),
            UseMode = BinLayout.ReadByte(raw, BinLayout.UseMode),
            CooldownGroup = BinLayout.ReadUInt16(raw, BinLayout.CooldownGroup),
            Overlap = unchecked((short)BinLayout.ReadUInt16(raw, BinLayout.Overlap)),
            SkillCode = BinLayout.ReadInt32(raw, BinLayout.SkillCode),
            ApplyValueMax = BinLayout.ReadByte(raw, BinLayout.ApplyValueMax),
            ApplyValueMin = BinLayout.ReadByte(raw, BinLayout.ApplyValueMin),
            ApplyElement = BinLayout.ReadByte(raw, BinLayout.ApplyElement),
            BoundType = BinLayout.ReadByte(raw, BinLayout.BoundType),
            UseTimeType = BinLayout.ReadByte(raw, BinLayout.UseTimeType),
            UsageTimeMinutes = BinLayout.ReadInt32(raw, BinLayout.UsageTimeMinutes),
            ScanPrice = BinLayout.ReadUInt32(raw, BinLayout.ScanPrice),
            SellPrice = BinLayout.ReadUInt32(raw, BinLayout.SellPrice)
        };
    }

    private static ItemEditorRow Clone(ItemEditorRow source)
    {
        return new ItemEditorRow
        {
            RawRecord = source.RawRecord.ToArray(),
            ItemId = source.ItemId,
            Name = source.Name,
            Description = source.Description,
            TypeComment = source.TypeComment,
            Icon = source.Icon,
            Class = source.Class,
            TypeL = source.TypeL,
            TypeS = source.TypeS,
            TypeValue = source.TypeValue,
            Section = source.Section,
            SellType = source.SellType,
            UseMode = source.UseMode,
            CooldownGroup = source.CooldownGroup,
            Overlap = source.Overlap,
            SkillCode = source.SkillCode,
            ApplyValueMin = source.ApplyValueMin,
            ApplyValueMax = source.ApplyValueMax,
            ApplyElement = source.ApplyElement,
            BoundType = source.BoundType,
            UseTimeType = source.UseTimeType,
            UsageTimeMinutes = source.UsageTimeMinutes,
            ScanPrice = source.ScanPrice,
            SellPrice = source.SellPrice
        };
    }

    private static void ApplyItemToRaw(ItemEditorRow item)
    {
        BinLayout.WriteInt32(item.RawRecord, BinLayout.ItemId, item.ItemId);
        BinLayout.WriteUtf16(item.RawRecord, BinLayout.Name, 64, item.Name);
        BinLayout.WriteUtf16(item.RawRecord, BinLayout.Comment, 512, item.Description);
        BinLayout.WriteUtf16(item.RawRecord, BinLayout.TypeComment, 64, item.TypeComment);
        BinLayout.WriteInt32(item.RawRecord, BinLayout.Icon, item.Icon);
        BinLayout.WriteUInt16(item.RawRecord, BinLayout.Class, item.Class);
        BinLayout.WriteUInt16(item.RawRecord, BinLayout.TypeL, item.TypeL);
        BinLayout.WriteUInt16(item.RawRecord, BinLayout.TypeS, item.TypeS);
        BinLayout.WriteInt32(item.RawRecord, BinLayout.TypeValue, item.TypeValue);
        BinLayout.WriteInt32(item.RawRecord, BinLayout.Section, item.Section);
        BinLayout.WriteUInt16(item.RawRecord, BinLayout.SellType, item.SellType);
        BinLayout.WriteByte(item.RawRecord, BinLayout.UseMode, item.UseMode);
        BinLayout.WriteUInt16(item.RawRecord, BinLayout.CooldownGroup, item.CooldownGroup);
        BinLayout.WriteUInt16(item.RawRecord, BinLayout.Overlap, unchecked((ushort)item.Overlap));
        BinLayout.WriteInt32(item.RawRecord, BinLayout.SkillCode, checked((int)item.SkillCode));
        BinLayout.WriteByte(item.RawRecord, BinLayout.ApplyValueMin, checked((byte)item.ApplyValueMin));
        BinLayout.WriteByte(item.RawRecord, BinLayout.ApplyValueMax, checked((byte)item.ApplyValueMax));
        BinLayout.WriteByte(item.RawRecord, BinLayout.ApplyElement, checked((byte)item.ApplyElement));
        BinLayout.WriteByte(item.RawRecord, BinLayout.BoundType, checked((byte)item.BoundType));
        BinLayout.WriteByte(item.RawRecord, BinLayout.UseTimeType, checked((byte)item.UseTimeType));
        BinLayout.WriteInt32(item.RawRecord, BinLayout.UsageTimeMinutes, item.UsageTimeMinutes);
        BinLayout.WriteUInt32(item.RawRecord, BinLayout.ScanPrice, item.ScanPrice);
        BinLayout.WriteUInt32(item.RawRecord, BinLayout.SellPrice, item.SellPrice);
    }

    private byte[] BuildItemListBytes()
    {
        using var ms = new MemoryStream();
        using var w = new BinaryWriter(ms);
        w.Write(Items.Count);
        foreach (var item in Items)
        {
            ApplyItemToRaw(item);
            w.Write(item.RawRecord);
        }
        if (_craftTableLoaded)
        {
            w.Write(_itemListTailBeforeCraft);
            w.Write(BuildCraftTableBytes());
            w.Write(_itemListTailAfterCraft);
        }
        else
        {
            w.Write(_itemListTail);
        }

        w.Flush();
        return ms.ToArray();
    }

    private void TryLoadCraftTableFromItemListTail()
    {
        _craftRecipesByNpc.Clear();
        _craftTableLoaded = false;
        _itemListTailBeforeCraft = [];
        _itemListTailAfterCraft = [];

        try
        {
            using var stream = new MemoryStream(_itemListTail);
            using var reader = new BinaryReader(stream, Encoding.Unicode);
            SkipFixedItemListSection(reader, 66);  // item sell/category names
            SkipFixedItemListSection(reader, 16);  // cooldown table
            SkipFixedItemListSection(reader, 8);   // display map
            SkipFixedItemListSection(reader, 132); // type names
            SkipFixedItemListSection(reader, 8);   // rank table
            SkipFixedItemListSection(reader, 4);   // element item 1
            SkipFixedItemListSection(reader, 4);   // element item 2
            SkipFixedItemListSection(reader, 44);  // exchange table
            SkipFixedItemListSection(reader, 540); // accessory option
            SkipFixedItemListSection(reader, 12);  // accessory enchant

            var craftOffset = checked((int)stream.Position);
            _itemListTailBeforeCraft = _itemListTail.AsSpan(0, craftOffset).ToArray();

            var npcCount = reader.ReadInt32();
            if (npcCount < 0 || npcCount > 100000)
                throw new InvalidDataException($"Invalid craft NPC count {npcCount}.");

            for (var i = 0; i < npcCount; i++)
                ReadCraftNpc(reader);

            var suffixOffset = checked((int)stream.Position);
            _itemListTailAfterCraft = _itemListTail.AsSpan(suffixOffset).ToArray();
            _craftTableLoaded = true;
        }
        catch
        {
            _craftRecipesByNpc.Clear();
            _craftTableLoaded = false;
            _itemListTailBeforeCraft = [];
            _itemListTailAfterCraft = [];
        }
    }

    private static void SkipFixedItemListSection(BinaryReader reader, int recordSize)
    {
        var count = reader.ReadInt32();
        if (count < 0)
            throw new InvalidDataException($"Invalid ItemList section count {count}.");

        var bytesToSkip = checked((long)count * recordSize);
        if (reader.BaseStream.Position + bytesToSkip > reader.BaseStream.Length)
            throw new EndOfStreamException("Unexpected EOF while skipping ItemList tail section.");

        reader.BaseStream.Position += bytesToSkip;
    }

    private void ReadCraftNpc(BinaryReader reader)
    {
        var npcId = reader.ReadInt32();
        if (npcId == 0)
            return;

        var recipes = GetCraftRecipes(npcId);
        var mainCategoryCount = reader.ReadInt32();
        for (var mainIndex = 0; mainIndex < mainCategoryCount; mainIndex++)
        {
            _ = reader.ReadInt32(); // map key duplicated by the original writer
            var mainCategoryId = reader.ReadInt32();
            var mainCategoryName = ReadPackedUtf16(reader);
            var subCategoryCount = reader.ReadInt32();

            for (var subIndex = 0; subIndex < subCategoryCount; subIndex++)
            {
                _ = reader.ReadInt32(); // map key duplicated by the original writer
                var subCategoryId = reader.ReadInt32();
                var subCategoryName = ReadPackedUtf16(reader);
                var recipeCount = reader.ReadInt32();

                for (var recipeIndex = 0; recipeIndex < recipeCount; recipeIndex++)
                {
                    var recipe = new CraftRecipeEditorRow
                    {
                        NpcId = npcId,
                        MainCategoryId = mainCategoryId,
                        MainCategoryName = mainCategoryName,
                        SubCategoryId = subCategoryId,
                        SubCategoryName = subCategoryName,
                        SequencialId = reader.ReadInt32(),
                        ItemId = reader.ReadInt32(),
                        Amount = reader.ReadInt32(),
                        SuccessRate = reader.ReadInt32(),
                        PercentageAssistGroup = reader.ReadInt32(),
                        ProtectAssistGroup = reader.ReadInt32(),
                        Price = reader.ReadUInt32()
                    };

                    var materialCount = reader.ReadInt32();
                    for (var materialIndex = 0; materialIndex < materialCount; materialIndex++)
                    {
                        var material = new CraftMaterialEditorRow
                        {
                            ItemId = reader.ReadInt32(),
                            Amount = reader.ReadInt32()
                        };
                        ResolveCraftMaterialDisplay(material);
                        material.MarkClean();
                        recipe.Materials.Add(material);
                    }

                    ResolveCraftRecipeDisplay(recipe);
                    recipe.MarkClean();
                    foreach (var material in recipe.Materials)
                        material.MarkClean();
                    recipes.Add(recipe);
                }
            }
        }
    }

    private byte[] BuildCraftTableBytes()
    {
        using var stream = new MemoryStream();
        using var writer = new BinaryWriter(stream, Encoding.Unicode);
        var groups = _craftRecipesByNpc
            .Where(x => x.Value.Count > 0)
            .OrderBy(x => x.Key)
            .ToList();

        writer.Write(groups.Count);
        foreach (var group in groups)
        {
            writer.Write(group.Key);
            var mainGroups = group.Value
                .GroupBy(x => new { x.MainCategoryId, Name = x.MainCategoryName })
                .OrderBy(x => x.Key.MainCategoryId)
                .ToList();

            writer.Write(mainGroups.Count);
            foreach (var mainGroup in mainGroups)
            {
                writer.Write(mainGroup.Key.MainCategoryId);
                writer.Write(mainGroup.Key.MainCategoryId);
                WritePackedUtf16(writer, mainGroup.Key.Name);

                var subGroups = mainGroup
                    .GroupBy(x => new { x.SubCategoryId, Name = x.SubCategoryName })
                    .OrderBy(x => x.Key.SubCategoryId)
                    .ToList();

                writer.Write(subGroups.Count);
                foreach (var subGroup in subGroups)
                {
                    writer.Write(subGroup.Key.SubCategoryId);
                    writer.Write(subGroup.Key.SubCategoryId);
                    WritePackedUtf16(writer, subGroup.Key.Name);
                    var recipes = subGroup.OrderBy(x => x.SequencialId).ToList();
                    writer.Write(recipes.Count);

                    foreach (var recipe in recipes)
                    {
                        writer.Write(recipe.SequencialId);
                        writer.Write(recipe.ItemId);
                        writer.Write(recipe.Amount);
                        writer.Write(recipe.SuccessRate);
                        writer.Write(recipe.PercentageAssistGroup);
                        writer.Write(recipe.ProtectAssistGroup);
                        writer.Write(checked((uint)Math.Max(0, recipe.Price)));
                        writer.Write(recipe.Materials.Count);
                        foreach (var material in recipe.Materials)
                        {
                            writer.Write(material.ItemId);
                            writer.Write(material.Amount);
                        }
                    }
                }
            }
        }

        writer.Flush();
        return stream.ToArray();
    }

    private static string ReadPackedUtf16(BinaryReader reader)
    {
        var chars = reader.ReadInt32();
        if (chars < 0 || chars > 2048)
            throw new InvalidDataException($"Invalid UTF-16 string length {chars}.");

        var bytes = reader.ReadBytes(checked(chars * 2));
        if (bytes.Length != chars * 2)
            throw new EndOfStreamException("Unexpected EOF while reading UTF-16 string.");

        return Encoding.Unicode.GetString(bytes).TrimEnd('\0');
    }

    private static void WritePackedUtf16(BinaryWriter writer, string value)
    {
        value ??= string.Empty;
        writer.Write(value.Length);
        writer.Write(Encoding.Unicode.GetBytes(value));
    }

    private void TryLoadSkills(byte[]? bytes)
    {
        if (bytes == null)
            return;

        using var fs = new MemoryStream(bytes);
        using var r = new BinaryReader(fs);
        var count = r.ReadInt32();
        _skillLoaded = true;
        for (var i = 0; i < count; i++)
        {
            var raw = r.ReadBytes(SkillRecordSize);
            if (raw.Length != SkillRecordSize)
                throw new EndOfStreamException("Unexpected EOF in Skill.bin.");

            var skill = ParseSkill(raw);
            _skillRows.Add(skill);
            _skills.TryAdd(skill.Id, skill);
        }

        _skillTail = r.ReadBytes((int)(r.BaseStream.Length - r.BaseStream.Position));
    }

    private static SkillEditorRow ParseSkill(byte[] raw)
    {
        var skill = new SkillEditorRow
        {
            RawRecord = raw,
            Id = BinLayout.ReadInt32(raw, 0),
            Name = BinLayout.ReadUtf16(raw, 4, 32),
            Description = BinLayout.ReadUtf16(raw, 68, 256),
            LevelupPoint = BinLayout.ReadUInt16(raw, 652),
            MaxLevel = BinLayout.ReadUInt16(raw, 654),
            AttributeType = BinLayout.ReadUInt16(raw, 656),
            NatureType = BinLayout.ReadUInt16(raw, 658),
            FamilyType = BinLayout.ReadUInt16(raw, 660),
            UseHP = BinLayout.ReadUInt16(raw, 662),
            UseDS = BinLayout.ReadUInt16(raw, 664),
            Icon = BinLayout.ReadUInt16(raw, 666),
            Target = BinLayout.ReadUInt16(raw, 668),
            AttType = BinLayout.ReadUInt16(raw, 670),
            AttRange = BitConverter.ToSingle(raw, 672),
            MinDamage = BitConverter.ToSingle(raw, 676),
            NormalDamage = BitConverter.ToSingle(raw, 680),
            MaxDamage = BitConverter.ToSingle(raw, 684),
            AttSphere = BitConverter.ToSingle(raw, 688),
            CastingTime = BitConverter.ToSingle(raw, 692),
            DamageTime = BitConverter.ToSingle(raw, 696),
            DamageDay = BinLayout.ReadUInt16(raw, 700),
            DistanceTime = BitConverter.ToSingle(raw, 704),
            CooldownTimeMs = BitConverter.ToSingle(raw, 708),
            CooldownDay = BinLayout.ReadUInt16(raw, 712),
            SkillVelocity = BitConverter.ToSingle(raw, 716),
            SkillAccel = BitConverter.ToSingle(raw, 720),
            SkillType = BinLayout.ReadUInt16(raw, 724),
            LimitLevel = BinLayout.ReadUInt16(raw, 726),
            SkillGroup = BinLayout.ReadUInt16(raw, 728),
            SkillRank = BinLayout.ReadUInt16(raw, 730),
            MemorySkill = BinLayout.ReadUInt16(raw, 732),
            ReqItem = BinLayout.ReadUInt16(raw, 734)
        };
        ReadSkillApply(raw, 580, skill.Apply0);
        ReadSkillApply(raw, 604, skill.Apply1);
        ReadSkillApply(raw, 628, skill.Apply2);
        skill.Apply0.MarkClean();
        skill.Apply1.MarkClean();
        skill.Apply2.MarkClean();
        skill.MarkClean();
        return skill;
    }

    private static void ReadSkillApply(byte[] raw, int offset, SkillApplyEditor apply)
    {
        apply.A = BinLayout.ReadInt32(raw, offset + 0);
        apply.InvokeRate = BinLayout.ReadInt32(raw, offset + 4);
        apply.B = BinLayout.ReadInt32(raw, offset + 8);
        apply.C = BinLayout.ReadInt32(raw, offset + 12);
        apply.BuffCode = BinLayout.ReadUInt16(raw, offset + 16);
        apply.Id = BinLayout.ReadUInt16(raw, offset + 18);
        apply.IncreaseBPoint = unchecked((short)BinLayout.ReadUInt16(raw, offset + 20));
    }

    private static void ApplySkillToRaw(SkillEditorRow skill)
    {
        BinLayout.WriteInt32(skill.RawRecord, 0, skill.Id);
        BinLayout.WriteUtf16(skill.RawRecord, 4, 32, skill.Name);
        BinLayout.WriteUtf16(skill.RawRecord, 68, 256, skill.Description);
        WriteSkillApply(skill.RawRecord, 580, skill.Apply0);
        WriteSkillApply(skill.RawRecord, 604, skill.Apply1);
        WriteSkillApply(skill.RawRecord, 628, skill.Apply2);
        BinLayout.WriteUInt16(skill.RawRecord, 652, skill.LevelupPoint);
        BinLayout.WriteUInt16(skill.RawRecord, 654, skill.MaxLevel);
        BinLayout.WriteUInt16(skill.RawRecord, 656, skill.AttributeType);
        BinLayout.WriteUInt16(skill.RawRecord, 658, skill.NatureType);
        BinLayout.WriteUInt16(skill.RawRecord, 660, skill.FamilyType);
        BinLayout.WriteUInt16(skill.RawRecord, 662, skill.UseHP);
        BinLayout.WriteUInt16(skill.RawRecord, 664, skill.UseDS);
        BinLayout.WriteUInt16(skill.RawRecord, 666, skill.Icon);
        BinLayout.WriteUInt16(skill.RawRecord, 668, skill.Target);
        BinLayout.WriteUInt16(skill.RawRecord, 670, skill.AttType);
        BinLayout.WriteSingle(skill.RawRecord, 672, skill.AttRange);
        BinLayout.WriteSingle(skill.RawRecord, 676, skill.MinDamage);
        BinLayout.WriteSingle(skill.RawRecord, 680, skill.NormalDamage);
        BinLayout.WriteSingle(skill.RawRecord, 684, skill.MaxDamage);
        BinLayout.WriteSingle(skill.RawRecord, 688, skill.AttSphere);
        BinLayout.WriteSingle(skill.RawRecord, 692, skill.CastingTime);
        BinLayout.WriteSingle(skill.RawRecord, 696, skill.DamageTime);
        BinLayout.WriteUInt16(skill.RawRecord, 700, skill.DamageDay);
        BinLayout.WriteSingle(skill.RawRecord, 704, skill.DistanceTime);
        BinLayout.WriteSingle(skill.RawRecord, 708, skill.CooldownTimeMs);
        BinLayout.WriteUInt16(skill.RawRecord, 712, skill.CooldownDay);
        BinLayout.WriteSingle(skill.RawRecord, 716, skill.SkillVelocity);
        BinLayout.WriteSingle(skill.RawRecord, 720, skill.SkillAccel);
        BinLayout.WriteUInt16(skill.RawRecord, 724, skill.SkillType);
        BinLayout.WriteUInt16(skill.RawRecord, 726, skill.LimitLevel);
        BinLayout.WriteUInt16(skill.RawRecord, 728, skill.SkillGroup);
        BinLayout.WriteUInt16(skill.RawRecord, 730, skill.SkillRank);
        BinLayout.WriteUInt16(skill.RawRecord, 732, skill.MemorySkill);
        BinLayout.WriteUInt16(skill.RawRecord, 734, skill.ReqItem);
    }

    private static SkillEditorRow CloneSkill(SkillEditorRow source)
    {
        var clone = ParseSkill(source.RawRecord.ToArray());
        clone.Id = source.Id;
        clone.Name = source.Name;
        clone.Description = source.Description;
        clone.Icon = source.Icon;
        clone.Target = source.Target;
        clone.AttType = source.AttType;
        clone.LevelupPoint = source.LevelupPoint;
        clone.MaxLevel = source.MaxLevel;
        clone.AttributeType = source.AttributeType;
        clone.NatureType = source.NatureType;
        clone.FamilyType = source.FamilyType;
        clone.UseHP = source.UseHP;
        clone.UseDS = source.UseDS;
        clone.AttRange = source.AttRange;
        clone.MinDamage = source.MinDamage;
        clone.NormalDamage = source.NormalDamage;
        clone.MaxDamage = source.MaxDamage;
        clone.AttSphere = source.AttSphere;
        clone.CastingTime = source.CastingTime;
        clone.DamageTime = source.DamageTime;
        clone.DamageDay = source.DamageDay;
        clone.DistanceTime = source.DistanceTime;
        clone.CooldownTimeMs = source.CooldownTimeMs;
        clone.CooldownDay = source.CooldownDay;
        clone.SkillVelocity = source.SkillVelocity;
        clone.SkillAccel = source.SkillAccel;
        clone.SkillType = source.SkillType;
        clone.LimitLevel = source.LimitLevel;
        clone.SkillGroup = source.SkillGroup;
        clone.SkillRank = source.SkillRank;
        clone.MemorySkill = source.MemorySkill;
        clone.ReqItem = source.ReqItem;
        CopyApply(source.Apply0, clone.Apply0);
        CopyApply(source.Apply1, clone.Apply1);
        CopyApply(source.Apply2, clone.Apply2);
        clone.MarkClean();
        clone.Apply0.MarkClean();
        clone.Apply1.MarkClean();
        clone.Apply2.MarkClean();
        return clone;
    }

    private static void CopyApply(SkillApplyEditor source, SkillApplyEditor target)
    {
        target.A = source.A;
        target.InvokeRate = source.InvokeRate;
        target.B = source.B;
        target.C = source.C;
        target.BuffCode = source.BuffCode;
        target.Id = source.Id;
        target.IncreaseBPoint = source.IncreaseBPoint;
    }

    private static void WriteSkillApply(byte[] raw, int offset, SkillApplyEditor apply)
    {
        BinLayout.WriteInt32(raw, offset + 0, apply.A);
        BinLayout.WriteInt32(raw, offset + 4, apply.InvokeRate);
        BinLayout.WriteInt32(raw, offset + 8, apply.B);
        BinLayout.WriteInt32(raw, offset + 12, apply.C);
        BinLayout.WriteUInt16(raw, offset + 16, apply.BuffCode);
        BinLayout.WriteUInt16(raw, offset + 18, apply.Id);
        BinLayout.WriteUInt16(raw, offset + 20, unchecked((ushort)apply.IncreaseBPoint));
    }

    private byte[] BuildSkillBytes()
    {
        using var ms = new MemoryStream();
        using var w = new BinaryWriter(ms);
        w.Write(_skillRows.Count);
        foreach (var skill in _skillRows)
        {
            ApplySkillToRaw(skill);
            w.Write(skill.RawRecord);
        }
        w.Write(_skillTail);
        w.Flush();
        return ms.ToArray();
    }

    private void TryLoadBuffs(byte[]? bytes)
    {
        if (bytes == null)
            return;

        using var fs = new MemoryStream(bytes);
        using var r = new BinaryReader(fs);
        var count = r.ReadInt32();
        _buffLoaded = true;
        for (var i = 0; i < count; i++)
        {
            var raw = r.ReadBytes(BuffRecordSize);
            if (raw.Length != BuffRecordSize)
                throw new EndOfStreamException("Unexpected EOF in Buff.bin.");

            var buff = ParseBuff(raw);
            _buffRows.Add(buff);
            _buffs[buff.Id] = buff;
        }
    }

    private static BuffEditorRow ParseBuff(byte[] raw)
    {
        var buff = new BuffEditorRow
        {
            RawRecord = raw,
            Id = BinLayout.ReadUInt16(raw, 0),
            Name = BinLayout.ReadUtf16(raw, 2, 64),
            Description = BinLayout.ReadUtf16(raw, 130, 128),
            Icon = BinLayout.ReadUInt16(raw, 386),
            BuffType = BinLayout.ReadUInt16(raw, 388),
            LifeType = BinLayout.ReadUInt16(raw, 390),
            TimeType = BinLayout.ReadUInt16(raw, 392),
            MinLevel = BinLayout.ReadUInt16(raw, 394),
            BuffClass = BinLayout.ReadUInt16(raw, 396),
            SkillCode = BinLayout.ReadUInt32(raw, 400),
            DigimonSkillCode = BinLayout.ReadUInt32(raw, 404),
            IsDeleted = BinLayout.ReadByte(raw, 408) != 0,
            EffectFile = BinLayout.ReadAscii(raw, 409, 64),
            ConditionLevel = BinLayout.ReadUInt16(raw, 474)
        };
        buff.MarkClean();
        return buff;
    }

    private static void ApplyBuffToRaw(BuffEditorRow buff)
    {
        BinLayout.WriteUInt16(buff.RawRecord, 0, buff.Id);
        BinLayout.WriteUtf16(buff.RawRecord, 2, 64, buff.Name);
        BinLayout.WriteUtf16(buff.RawRecord, 130, 128, buff.Description);
        BinLayout.WriteUInt16(buff.RawRecord, 386, buff.Icon);
        BinLayout.WriteUInt16(buff.RawRecord, 388, buff.BuffType);
        BinLayout.WriteUInt16(buff.RawRecord, 390, buff.LifeType);
        BinLayout.WriteUInt16(buff.RawRecord, 392, buff.TimeType);
        BinLayout.WriteUInt16(buff.RawRecord, 394, buff.MinLevel);
        BinLayout.WriteUInt16(buff.RawRecord, 396, buff.BuffClass);
        BinLayout.WriteUInt32(buff.RawRecord, 400, buff.SkillCode);
        BinLayout.WriteUInt32(buff.RawRecord, 404, buff.DigimonSkillCode);
        BinLayout.WriteByte(buff.RawRecord, 408, (byte)(buff.IsDeleted ? 1 : 0));
        BinLayout.WriteAscii(buff.RawRecord, 409, 64, buff.EffectFile);
        BinLayout.WriteUInt16(buff.RawRecord, 474, buff.ConditionLevel);
    }

    private byte[] BuildBuffBytes()
    {
        using var ms = new MemoryStream();
        using var w = new BinaryWriter(ms);
        w.Write(_buffRows.Count);
        foreach (var buff in _buffRows)
        {
            ApplyBuffToRaw(buff);
            w.Write(buff.RawRecord);
        }
        w.Flush();
        return ms.ToArray();
    }

    private void TryLoadContainers(byte[]? bytes)
    {
        if (bytes == null)
            return;

        using var fs = new MemoryStream(bytes);
        using var r = new BinaryReader(fs);
        if (r.ReadUInt32() != ContainerMagic || r.ReadUInt32() != ContainerVersion)
            return;

        _containerLoaded = true;
        var count = checked((int)r.ReadUInt32());
        for (var i = 0; i < count; i++)
        {
            var container = new ContainerEditorRow
            {
                Id = r.ReadInt64(),
                ItemId = r.ReadInt32(),
                RewardAmount = r.ReadInt32(),
                ItemName = ReadPackedString(r)
            };

            var rewardCount = checked((int)r.ReadUInt32());
            for (var reward = 0; reward < rewardCount; reward++)
            {
                var rewardRow = new ContainerRewardEditorRow
                {
                    Id = r.ReadInt64(),
                    ItemId = r.ReadInt32(),
                    MinAmount = r.ReadInt32(),
                    MaxAmount = r.ReadInt32(),
                    Chance = r.ReadDouble(),
                    Rare = r.ReadByte() != 0,
                    ItemName = ReadPackedString(r),
                    ContainerAssetId = container.Id
                };
                rewardRow.MarkClean();
                container.Rewards.Add(rewardRow);
            }

            container.MarkClean();
            _containers[container.ItemId] = container;
        }
    }

    private void TryLoadContainersFromSqlBackup()
    {
        var sqlPath = FindSqlBackupPath();
        if (sqlPath == null)
            return;

        var snapshot = LoadPostgresContainerSnapshot(sqlPath);
        var added = 0;
        foreach (var container in snapshot.Values.OrderBy(x => x.ItemId))
        {
            if (_containers.ContainsKey(container.ItemId))
                continue;

            container.MarkClean();
            foreach (var reward in container.Rewards)
                reward.MarkClean();

            _containers[container.ItemId] = container;
            added++;
        }

        if (added == 0)
            return;

        _containerLoaded = true;
        _containerSqlLoadedCount = added;
        _containerSqlSourcePath = sqlPath;
    }

    private string? FindSqlBackupPath()
    {
        var roots = new List<string>();
        AddSearchRoot(roots, BinDirectory);
        AddSearchRoot(roots, _sourcePath);
        AddSearchRoot(roots, AppContext.BaseDirectory);

        var seen = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
        foreach (var root in roots)
        {
            var dir = Directory.Exists(root)
                ? new DirectoryInfo(root)
                : new FileInfo(root).Directory;

            while (dir != null)
            {
                var direct = Path.Combine(dir.FullName, "db", "dso.sql");
                if (seen.Add(direct) && File.Exists(direct))
                    return direct;

                var sibling = Path.Combine(dir.FullName, "dmo-server-main-postgres", "db", "dso.sql");
                if (seen.Add(sibling) && File.Exists(sibling))
                    return sibling;

                dir = dir.Parent;
            }
        }

        return null;
    }

    private static void AddSearchRoot(List<string> roots, string path)
    {
        if (!string.IsNullOrWhiteSpace(path))
            roots.Add(path);
    }

    private static Dictionary<int, ContainerEditorRow> LoadPostgresContainerSnapshot(string sqlPath)
    {
        var containersById = new Dictionary<long, ContainerEditorRow>();
        var rewardsByContainerId = new Dictionary<long, List<ContainerRewardEditorRow>>();
        using var reader = new StreamReader(sqlPath, Encoding.UTF8, detectEncodingFromByteOrderMarks: true);

        while (reader.ReadLine() is { } line)
        {
            if (line.StartsWith("COPY ", StringComparison.Ordinal) &&
                line.Contains("\"Asset_Container\"", StringComparison.Ordinal))
            {
                ReadCopySection(reader, fields =>
                {
                    if (fields.Length < 4)
                        return;

                    var id = ParseLong(fields[0]);
                    var itemId = ParseInt(fields[1]);
                    if (itemId <= 0)
                        return;

                    containersById[id] = new ContainerEditorRow
                    {
                        Id = id,
                        ItemId = itemId,
                        ItemName = DecodePostgresCopyValue(fields[2]),
                        RewardAmount = ParseInt(fields[3])
                    };
                });
            }
            else if (line.StartsWith("COPY ", StringComparison.Ordinal) &&
                     line.Contains("\"Asset_ContainerReward\"", StringComparison.Ordinal))
            {
                ReadCopySection(reader, fields =>
                {
                    if (fields.Length < 8)
                        return;

                    var containerAssetId = ParseLong(fields[7]);
                    var reward = new ContainerRewardEditorRow
                    {
                        Id = ParseLong(fields[0]),
                        ItemId = ParseInt(fields[1]),
                        ItemName = DecodePostgresCopyValue(fields[2]),
                        MinAmount = ParseInt(fields[3]),
                        MaxAmount = ParseInt(fields[4]),
                        Chance = ParseDouble(fields[5]),
                        Rare = fields[6].Equals("t", StringComparison.OrdinalIgnoreCase) ||
                               fields[6].Equals("true", StringComparison.OrdinalIgnoreCase),
                        ContainerAssetId = containerAssetId
                    };

                    if (!rewardsByContainerId.TryGetValue(containerAssetId, out var rewards))
                    {
                        rewards = [];
                        rewardsByContainerId[containerAssetId] = rewards;
                    }

                    rewards.Add(reward);
                });
            }
        }

        var result = new Dictionary<int, ContainerEditorRow>();
        foreach (var container in containersById.Values)
        {
            if (rewardsByContainerId.TryGetValue(container.Id, out var rewards))
            {
                foreach (var reward in rewards.OrderBy(x => x.Id))
                    container.Rewards.Add(reward);
            }

            result[container.ItemId] = container;
        }

        return result;
    }

    private static void ReadCopySection(StreamReader reader, Action<string[]> onFields)
    {
        while (reader.ReadLine() is { } line)
        {
            if (line == @"\.")
                return;

            onFields(line.Split('\t'));
        }
    }

    private static int ParseInt(string value)
        => int.TryParse(value, NumberStyles.Integer, CultureInfo.InvariantCulture, out var parsed) ? parsed : 0;

    private static long ParseLong(string value)
        => long.TryParse(value, NumberStyles.Integer, CultureInfo.InvariantCulture, out var parsed) ? parsed : 0;

    private static double ParseDouble(string value)
        => double.TryParse(value, NumberStyles.Float, CultureInfo.InvariantCulture, out var parsed) ? parsed : 0;

    private static string DecodePostgresCopyValue(string value)
    {
        if (value == @"\N")
            return string.Empty;

        var builder = new StringBuilder(value.Length);
        for (var i = 0; i < value.Length; i++)
        {
            var c = value[i];
            if (c != '\\' || i + 1 >= value.Length)
            {
                builder.Append(c);
                continue;
            }

            var escaped = value[++i];
            builder.Append(escaped switch
            {
                'b' => '\b',
                'f' => '\f',
                'n' => '\n',
                'r' => '\r',
                't' => '\t',
                'v' => '\v',
                '\\' => '\\',
                _ => escaped
            });
        }

        return builder.ToString();
    }

    private byte[] BuildContainerBytes()
    {
        using var ms = new MemoryStream();
        using var w = new BinaryWriter(ms);
        w.Write(ContainerMagic);
        w.Write(ContainerVersion);
        w.Write((uint)_containers.Count);

        foreach (var container in _containers.Values.OrderBy(x => x.Id))
        {
            w.Write(container.Id);
            w.Write(container.ItemId);
            w.Write(container.RewardAmount);
            WritePackedString(w, container.ItemName);
            w.Write((uint)container.Rewards.Count);

            foreach (var reward in container.Rewards.OrderBy(x => x.Id))
            {
                w.Write(reward.Id);
                w.Write(reward.ItemId);
                w.Write(reward.MinAmount);
                w.Write(reward.MaxAmount);
                w.Write(reward.Chance);
                w.Write((byte)(reward.Rare ? 1 : 0));
                WritePackedString(w, reward.ItemName);
            }
        }

        w.Flush();
        return ms.ToArray();
    }

    private void DecorateLinkedRows()
    {
        foreach (var skill in _skillRows)
        {
            DecorateApply(skill.Apply0);
            DecorateApply(skill.Apply1);
            DecorateApply(skill.Apply2);
        }

        foreach (var buff in _buffRows)
        {
            buff.LinkedSkillTitle = buff.SkillCode > 0 && _skills.TryGetValue((int)buff.SkillCode, out var skill)
                ? skill.Title
                : string.Empty;
        }
    }

    private void DecorateApply(SkillApplyEditor apply)
    {
        apply.RefreshVisual();

        if (apply.BuffCode > 0 && _buffs.TryGetValue(apply.BuffCode, out var buff))
        {
            apply.ResolvedBuffTitle = buff.Title;
            apply.ResolvedBuffDescription = buff.DescriptionPreview;
            apply.ResolvedBuffStaticTime = buff.StaticTimeSummary;
        }
        else
        {
            apply.ResolvedBuffTitle = apply.BuffCode > 0 ? $"Buff {apply.BuffCode} not found in Buff.bin" : string.Empty;
            apply.ResolvedBuffDescription = string.Empty;
            apply.ResolvedBuffStaticTime = string.Empty;
        }
    }

    private static string ApplyName(int value)
        => value switch
        {
            0 => "0 - FMCOMMON_EVENT_ITEM",
            1 => "1 - APPLY_HP",
            2 => "2 - APPLY_DS",
            3 => "3 - APPLY_MAXHP",
            4 => "4 - APPLY_MAXDS",
            5 => "5 - APPLY_AP",
            6 => "6 - APPLY_CA",
            7 => "7 - APPLY_DP",
            8 => "8 - APPLY_EV",
            9 => "9 - APPLY_MS",
            10 => "10 - APPLY_AS",
            11 => "11 - APPLY_AR",
            12 => "12 - APPLY_HT",
            13 => "13 - APPLY_FP",
            14 => "14 - APPLY_FS",
            15 => "15 - APPLY_EXP",
            16 => "16 - APPLY_POWERAPPLYRATE",
            17 => "17 - APPLY_BL",
            18 => "18 - APPLY_DA",
            19 => "19 - APPLY_ER",
            20 => "20 - APPLY_AllParam",
            21 => "21 - APPLY_SER",
            22 => "22 - APPLY_SDR",
            23 => "23 - APPLY_SRR",
            24 => "24 - APPLY_SCD",
            25 => "25 - APPLY_SCR",
            26 => "26 - APPLY_HRR",
            27 => "27 - APPLY_DRR",
            28 => "28 - APPLY_MDA",
            29 => "29 - APPLY_HR",
            30 => "30 - APPLY_DSN",
            31 => "31 - APPLY_HPN",
            32 => "32 - APPLY_STA",
            33 => "33 - APPLY_UB",
            34 => "34 - APPLY_ATTRIBUTTE",
            35 => "35 - APPLY_CC",
            36 => "36 - APPLY_CR",
            37 => "37 - APPLY_DOT",
            38 => "38 - APPLY_DOT2",
            39 => "39 - APPLY_STUN",
            40 => "40 - APPLY_DR",
            41 => "41 - APPLY_AB",
            42 => "42 - APPLY_HPDMG",
            43 => "43 - APPLY_ATDMG",
            44 => "44 - APPLY_HPDEF",
            45 => "45 - APPLY_ATDEF",
            46 => "46 - APPLY_PROVOKE",
            47 => "47 - APPLY_INSURANCE",
            48 => "48 - APPLY_CAT",
            49 => "49 - APPLY_RDD",
            51 => "51 - GUILD_SHOUT",
            52 => "52 - GUILD_MEMBERUP",
            53 => "53 - GUILD_SUMMON",
            54 => "54 - GUILD_NAMECOLOR",
            61 => "61 - GUILD_NATUREUP",
            62 => "62 - GUILD_SCANREDUCE",
            63 => "63 - GUILD_SCANBITUP",
            64 => "64 - GUILD_HATCHUP",
            65 => "65 - GUILD_CROPBITUP",
            66 => "66 - GUILD_MOVEUP",
            67 => "67 - GUILD_INCHAPAYREDUCE",
            68 => "68 - GUILD_INCHARATEUP",
            71 => "71 - APPLY_WEAKNESS",
            72 => "72 - APPLY_ATTRIBUTEDAMAGE",
            73 => "73 - APPLY_ATTRIBUTEDR",
            74 => "74 - APPLY_ATTRIBUTEDS",
            75 => "75 - APPLY_SILENCE",
            76 => "76 - APPLY_AllSkillDamageDecrease",
            77 => "77 - APPLY_SpecificSkillDamageDecrease",
            78 => "78 - APPLY_SpecificSkillDamageIncrease",
            79 => "79 - APPLY_NatureShield",
            80 => "80 - APPLY_XGuage",
            81 => "81 - APPLY_XCrystal",
            82 => "82 - APPLY_RegionBuffDummy",
            83 => "83 - APPLY_EvoTypeDamageIncrease",
            84 => "84 - APPLY_NatureTypeDamageIncrease",
            85 => "85 - APPLY_FixMoveSpeed",
            _ => $"{value} - unknown APPLY"
        };

    private static string MethodName(ushort value)
        => value switch
        {
            0 => "0 - none",
            1 => "Me_001 - damage calculation",
            2 => "Me_002 - skill damage calculation",
            10 => "Me_010 - tamer skill damage calculation",
            101 => "Me_101 - A=A+B",
            102 => "Me_102 - A=A+(A*B/100)",
            103 => "Me_103 - A=A*B/100",
            104 => "Me_104 - A=B",
            105 => "Me_105 - A=A+(A*B/100)",
            106 => "Me_106 - A=A+(A*B/100)",
            107 => "Me_107 - abnormal status",
            108 => "Me_108 - A=A",
            200 => "Me_200 - no formula apply",
            201 => "Me_201 - B sec, A=A+C",
            202 => "Me_202 - B sec, A=A+(A*C/100)",
            203 => "Me_203 - B sec, A=A*C/100",
            204 => "Me_204 - B sec, A=C",
            205 => "Me_205 - DOT formula",
            206 => "Me_206 - skill damage value buff",
            207 => "Me_207 - skill damage rate buff",
            208 => "Me_208 - Time=Time+(SkillLv*Apply)",
            209 => "Me_209",
            215 => "Me_215",
            301 => "Me_301 - A=A-B",
            302 => "Me_302",
            303 => "Me_303",
            401 => "Me_401 - event sync",
            402 => "Me_402 - temporary scale change",
            403 => "Me_403",
            404 => "Me_404",
            501 => "Me_501",
            _ => $"{value} - unknown method"
        };

    private static string ReadPackedString(BinaryReader r)
    {
        var size = r.ReadUInt16();
        return size == 0 ? string.Empty : Encoding.UTF8.GetString(r.ReadBytes(size));
    }

    private static void WritePackedString(BinaryWriter writer, string value)
    {
        var bytes = Encoding.UTF8.GetBytes(value ?? string.Empty);
        if (bytes.Length > ushort.MaxValue)
            throw new InvalidDataException("Container string is too long.");

        writer.Write((ushort)bytes.Length);
        writer.Write(bytes);
    }

    private void Classify(ItemEditorRow item)
    {
        var kind = ResolveKind(item);
        if (_containers.TryGetValue(item.ItemId, out var box))
        {
            item.EffectKind = kind.Name;
            item.EffectSummary = $"{box.RewardAmount} roll(s), {box.Rewards.Count} rewards: " +
                                 string.Join(", ", box.Rewards.Take(5).Select(x => $"{x.ItemId} x{x.MinAmount}-{x.MaxAmount} {x.Chance:0.##}%"));
            return;
        }

        item.EffectKind = kind.Name;
        if (kind.Key == ItemKindCatalog.Megaphone)
        {
            item.EffectSummary = $"{ItemKindCatalog.MegaphoneVariant(item.TypeL)} | {kind.Detail} | type {item.TypeL}.{item.TypeS}";
        }
        else if (kind.Key != ItemKindCatalog.Generic)
        {
            item.EffectSummary = $"{kind.Detail} | type {item.TypeL}.{item.TypeS}";
        }
        else
        {
            item.EffectSummary = item.TypeL switch
            {
                60 or 78 => "EXP / notice style item",
                62 => "Summon/Achievement item",
                72 => "Teleport item",
                89 => "Fruit / rescale item",
                155 => "Inventory expansion",
                156 => "Warehouse expansion",
                159 => "Digimon slot expansion",
                160 => "Archive expansion",
                180 => "Cash tamer skill",
                202 => "D-Skill expansion",
                _ => "Generic ItemList.bin item"
            } + $" | type {item.TypeL}.{item.TypeS}";
        }

        var parts = new List<string> { item.EffectSummary };
        if (item.UseTimeType != 0 || item.UsageTimeMinutes > 0)
            parts.Add(item.DurationSummary);

        if (item.SkillCode > 0 && _skills.TryGetValue((int)item.SkillCode, out var skill))
        {
            parts.Add($"skill {skill.Title}");
            parts.Add($"target {skill.Target}");
            parts.Add($"cooldown {skill.CooldownDisplay}");
            var applies = new[] { skill.Apply0, skill.Apply1, skill.Apply2 }
                .Where(x => x.Id != 0 || x.A != 0 || x.BuffCode != 0)
                .Select(x => x.EffectSentence);
            parts.AddRange(applies);

            var buffCode = new[] { skill.Apply0.BuffCode, skill.Apply1.BuffCode, skill.Apply2.BuffCode }.FirstOrDefault(x => x > 0);
            if (buffCode > 0 && _buffs.TryGetValue(buffCode, out var buff))
                parts.Add($"buff {buff.Title}: type={buff.BuffTypeName}, life={buff.LifeTypeName}, time={buff.TimeTypeName}, class={buff.BuffClass}, minLv={buff.MinLevel}");
        }
        else if (item.SkillCode > 0)
        {
            parts.Add($"skill {item.SkillCode} not found in Skill.bin");
        }

        item.EffectSummary = string.Join(" | ", parts);
    }

    private void RefreshIcon(ItemEditorRow item)
    {
        var result = _iconAtlas.ResolveItemIcon(item.Icon, item.Class);
        item.IconImage = result.image;
        item.IconStatus = result.status;
    }

    private void ResolveCraftMaterialDisplay(CraftMaterialEditorRow material)
    {
        ResolveItem(material.ItemId, out var itemName, out var icon);
        material.ItemName = itemName;
        material.IconImage = icon;
    }

    private void ResolveItem(int itemId, out string itemName, out System.Windows.Media.ImageSource? icon)
    {
        _ = ResolveItemDisplay(itemId, out itemName, out icon);
    }

    private bool IsAnySkillDirty()
        => _skillRows.Any(x => x.IsDirty || x.Apply0.IsDirty || x.Apply1.IsDirty || x.Apply2.IsDirty);

    private static bool IsContainerDirty(ContainerEditorRow container)
        => container.IsDirty || container.Rewards.Any(x => x.IsDirty);

    private void MarkAllClean()
    {
        foreach (var item in Items)
            item.MarkClean();

        foreach (var skill in _skillRows)
        {
            skill.MarkClean();
            skill.Apply0.MarkClean();
            skill.Apply1.MarkClean();
            skill.Apply2.MarkClean();
        }

        foreach (var buff in _buffRows)
            buff.MarkClean();

        foreach (var container in _containers.Values)
        {
            container.MarkClean();
            foreach (var reward in container.Rewards)
                reward.MarkClean();
        }

        foreach (var recipe in _craftRecipesByNpc.Values.SelectMany(x => x))
        {
            recipe.MarkClean();
            foreach (var material in recipe.Materials)
                material.MarkClean();
        }
    }
}
