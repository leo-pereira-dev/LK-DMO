using System.Collections.ObjectModel;
using System.Windows.Media;

namespace ItemEditor.Models;

public sealed record LookupOption(int Id, string Name)
{
    public string DisplayName => Id < 0 ? Name : $"{Id} - {Name}";

    public override string ToString() => DisplayName;
}

public static class NpcTypeCatalog
{
    public const int DTerminalDungeonEntry = 26;
    private static readonly int[] ShopTypeIds = [1, 8, 9, 12, 14, 16, 25];

    public static IReadOnlyList<LookupOption> Options { get; } =
    [
        new(0, "None / unresolved"),
        new(1, "Trade shop"),
        new(2, "Digitama trade"),
        new(3, "Portal"),
        new(4, "Make tactics"),
        new(5, "Element item"),
        new(6, "Warehouse"),
        new(7, "Tactics house"),
        new(8, "Guild"),
        new(9, "Digicore"),
        new(10, "Capsule machine"),
        new(11, "Skill shop"),
        new(12, "Event store"),
        new(13, "DATS portal"),
        new(14, "Property store / exchange"),
        new(15, "Gotcha machine"),
        new(16, "Masters matching"),
        new(17, "Mystery machine"),
        new(18, "Spirit evolution"),
        new(19, "Special event"),
        new(20, "Item production / craft"),
        new(21, "Battle registrant"),
        new(22, "Infinite war manager"),
        new(23, "Infinite war notice board"),
        new(24, "Extra evolution"),
        new(25, "GDMO portal test"),
        new(DTerminalDungeonEntry, "D-Terminal dungeon entry")
    ];

    public static IReadOnlyList<LookupOption> FilterOptions { get; } =
    [
        new(-1, "All NPC types"),
        .. Options
    ];

    public static IReadOnlyList<LookupOption> ShopFilterOptions { get; } =
    [
        new(-1, "All shop types"),
        .. Options.Where(x => ShopTypeIds.Contains(x.Id))
    ];

    public static string NameOf(int id)
        => Options.FirstOrDefault(x => x.Id == id)?.Name ?? $"Unknown NPC type {id}";

    public static bool IsCraft(int id) => id == 20;

    public static bool IsShop(int id) => ShopTypeIds.Contains(id);
}

public static class QuestTypeCatalog
{
    public static IReadOnlyList<LookupOption> Options { get; } =
    [
        new(0, "Sub quest"),
        new(1, "Main quest"),
        new(2, "Daily quest"),
        new(3, "Repeatable quest"),
        new(4, "Repeatable event quest"),
        new(5, "Achievement quest"),
        new(6, "Combine quest"),
        new(7, "Tutorial quest"),
        new(8, "Xanti joint progress quest")
    ];

    public static IReadOnlyList<LookupOption> FilterOptions { get; } =
    [
        new(-1, "All quest types"),
        .. Options
    ];

    public static string NameOf(int id)
        => Options.FirstOrDefault(x => x.Id == id)?.Name ?? $"Unknown quest type {id}";
}

public static class QuestTargetCatalog
{
    public static IReadOnlyList<LookupOption> StartTargetOptions { get; } =
    [
        new(0, "NPC"),
        new(1, "Digivice")
    ];

    public static IReadOnlyList<LookupOption> CompletionTargetOptions { get; } =
    [
        new(0, "Self completion"),
        new(1, "NPC completion"),
        new(2, "Immediate completion")
    ];
}

public static class QuestSupplyCatalog
{
    public static IReadOnlyList<LookupOption> Options { get; } =
    [
        new(0, "Item supplied on accept")
    ];

    public static string NameOf(int id)
        => Options.FirstOrDefault(x => x.Id == id)?.Name ?? $"Unknown supply {id}";
}

public static class QuestPreconditionCatalog
{
    public static IReadOnlyList<LookupOption> Options { get; } =
    [
        new(0, "Completed quest"),
        new(1, "Tamer level"),
        new(2, "Has item"),
        new(3, "Digimon level"),
        new(4, "Digimon type"),
        new(5, "Region reached"),
        new(6, "Evolution slot enabled"),
        new(7, "Evolution slot disabled"),
        new(8, "Base digimon ID"),
        new(9, "Tamer table ID"),
        new(10, "Quest not in progress"),
        new(11, "Quest not completed")
    ];

    public static string NameOf(int id)
        => Options.FirstOrDefault(x => x.Id == id)?.Name ?? $"Unknown precondition {id}";
}

public static class QuestGoalCatalog
{
    public static IReadOnlyList<LookupOption> Options { get; } =
    [
        new(0, "Kill monster"),
        new(1, "Loot item"),
        new(2, "Client action"),
        new(3, "Reach region"),
        new(4, "Talk to NPC"),
        new(5, "Use item"),
        new(6, "Use item in NPC"),
        new(7, "Use item in monster"),
        new(8, "Use item at region"),
        new(9, "Reach level"),
        new(10, "Acquire partner"),
        new(100, "Map marker"),
        new(101, "Employment marker")
    ];

    public static string NameOf(int id) => id switch
    {
        _ => Options.FirstOrDefault(x => x.Id == id)?.Name ?? $"Unknown goal {id}"
    };
}

public static class QuestRewardCatalog
{
    public static IReadOnlyList<LookupOption> MethodOptions { get; } =
    [
        new(0, "Give reward"),
        new(1, "Select one reward")
    ];

    public static IReadOnlyList<LookupOption> TypeOptions { get; } =
    [
        new(0, "Money"),
        new(1, "Experience"),
        new(2, "Item"),
        new(3, "Evolution slot")
    ];

    public static string MethodNameOf(int id)
        => MethodOptions.FirstOrDefault(x => x.Id == id)?.Name ?? $"Unknown method {id}";

    public static string TypeNameOf(int id)
        => TypeOptions.FirstOrDefault(x => x.Id == id)?.Name ?? $"Unknown reward {id}";
}

public static class QuestEventCatalog
{
    public static string NameOf(int index) => index switch
    {
        0 => "Start event",
        1 => "Process event",
        2 => "Complete event",
        3 => "Reward event",
        _ => $"Event slot {index}"
    };
}

public sealed class NpcEditorRow : DirtyRow
{
    private long _assetId;
    private int _npcId;
    private int _mapId;
    private int _typeId;
    private int _moveType;
    private int _displayFlag;
    private int _modelId;
    private int _questInitialState;
    private int _mapPositionX;
    private int _mapPositionY;
    private float _mapRotation;
    private bool _hasMapPosition;
    private bool _isMapPositionDirty;
    private string _mapName = string.Empty;
    private string _name = string.Empty;
    private string _penName = string.Empty;
    private string _talk = string.Empty;
    private string _modelComment = string.Empty;

    public long AssetId { get => _assetId; set => Set(ref _assetId, value); }
    public int NpcId { get => _npcId; set => Set(ref _npcId, value); }
    public int MapId { get => _mapId; set => Set(ref _mapId, value); }
    public int TypeId { get => _typeId; set => Set(ref _typeId, value); }
    public int MoveType { get => _moveType; set => Set(ref _moveType, value); }
    public int DisplayFlag { get => _displayFlag; set => Set(ref _displayFlag, value); }
    public int ModelId { get => _modelId; set => Set(ref _modelId, value); }
    public int QuestInitialState { get => _questInitialState; set => Set(ref _questInitialState, value); }
    public int MapPositionX { get => _mapPositionX; set => Set(ref _mapPositionX, value, markDirty: false); }
    public int MapPositionY { get => _mapPositionY; set => Set(ref _mapPositionY, value, markDirty: false); }
    public float MapRotation { get => _mapRotation; set => Set(ref _mapRotation, value, markDirty: false); }
    public bool HasMapPosition { get => _hasMapPosition; set => Set(ref _hasMapPosition, value, markDirty: false); }
    public bool IsMapPositionDirty { get => _isMapPositionDirty; set => Set(ref _isMapPositionDirty, value, markDirty: false); }
    public string MapName { get => _mapName; set => Set(ref _mapName, value, markDirty: false); }
    public string Name { get => _name; set => Set(ref _name, value); }
    public string PenName { get => _penName; set => Set(ref _penName, value); }
    public string Talk { get => _talk; set => Set(ref _talk, value); }
    public string ModelComment { get => _modelComment; set => Set(ref _modelComment, value); }

    public ObservableCollection<NpcItemEditorRow> Items { get; } = [];
    public ObservableCollection<CraftRecipeEditorRow> CraftRecipes { get; } = [];
    public ObservableCollection<NpcPortalEditorRow> Portals { get; } = [];
    public ObservableCollection<NpcQuestLinkEditorRow> QuestLinks { get; } = [];
    public byte[] NpcTypePayloadRaw { get; set; } = [];

    public string TypeName => NpcTypeCatalog.NameOf(TypeId);
    public string DisplayName => $"{NpcId} - {NameTitle}";
    public string NameTitle => string.IsNullOrWhiteSpace(Name) ? $"NPC {NpcId}" : FirstLine(Name);
    public string MapDisplay => string.IsNullOrWhiteSpace(MapName) ? $"{MapId} - Map {MapId}" : $"{MapId} - {MapName}";
    public string CommerceItemsTitle => CraftRecipes.Count > 0 || TypeId == 20
        ? "Craft recipes"
        : TypeId switch
        {
            1 or 8 or 9 or 12 or 14 or 16 or 25 => "Shop item entries",
            0 when Items.Count > 0 => "Shop/craft entries, type not resolved",
            _ => "NPC item entries"
        };
    public string AssetSummary => $"{TypeName} | items {Items.Count} | recipes {CraftRecipes.Count} | portals {Portals.Count} | quest links {QuestLinks.Count}";
    public string PositionSummary => HasMapPosition
        ? $"MapNpc.bin position X {MapPositionX}, Y {MapPositionY}, rotation {MapRotation:0.##}"
        : "No MapNpc.bin position loaded. Click the minimap to place this NPC.";
    public string VisualSummary => ModelId > 0
        ? $"Model {ModelId}{(string.IsNullOrWhiteSpace(ModelComment) ? string.Empty : $" - {ModelComment}")}"
        : "No model id loaded. Load a client pack with Npc.bin to resolve the visual model.";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(NpcId) or nameof(Name))
        {
            Raise(nameof(NameTitle));
            Raise(nameof(DisplayName));
        }
        if (propertyName is nameof(MapId) or nameof(MapName))
            Raise(nameof(MapDisplay));
        if (propertyName is nameof(MapPositionX) or nameof(MapPositionY) or nameof(MapRotation) or nameof(HasMapPosition))
        {
            _isMapPositionDirty = true;
            Raise(nameof(IsMapPositionDirty));
            Raise(nameof(PositionSummary));
        }
        if (propertyName == nameof(MapId) && HasMapPosition)
        {
            _isMapPositionDirty = true;
            Raise(nameof(IsMapPositionDirty));
        }
        if (propertyName == nameof(TypeId))
        {
            Raise(nameof(TypeName));
            Raise(nameof(CommerceItemsTitle));
            Raise(nameof(AssetSummary));
        }
        if (propertyName is nameof(ModelId) or nameof(ModelComment))
            Raise(nameof(VisualSummary));
    }

    public void RefreshAssetSummary()
    {
        Raise(nameof(CommerceItemsTitle));
        Raise(nameof(AssetSummary));
    }

    public void MarkMapPositionClean()
    {
        _isMapPositionDirty = false;
        Raise(nameof(IsMapPositionDirty));
    }

    private static string FirstLine(string value)
    {
        var normalized = value.Replace("\r\n", "\n").Replace('\r', '\n');
        return normalized.Split('\n', 2)[0].Trim();
    }
}

public sealed class NpcItemEditorRow : DirtyRow
{
    private long _id;
    private int _itemId;
    private string _itemName = string.Empty;
    private ImageSource? _iconImage;
    private uint _sellPrice;
    private uint _scanPrice;
    private int _digicorePrice;
    private int _eventPriceId;
    private int _eventPriceAmount;
    private string _eventPriceName = string.Empty;

    public long Id { get => _id; set => Set(ref _id, value); }
    public int ItemId { get => _itemId; set => Set(ref _itemId, value); }
    public string ItemName { get => _itemName; set => Set(ref _itemName, value); }
    public ImageSource? IconImage { get => _iconImage; set => Set(ref _iconImage, value, markDirty: false); }
    public uint SellPrice { get => _sellPrice; set => Set(ref _sellPrice, value, markDirty: false); }
    public uint ScanPrice { get => _scanPrice; set => Set(ref _scanPrice, value, markDirty: false); }
    public int DigicorePrice { get => _digicorePrice; set => Set(ref _digicorePrice, value, markDirty: false); }
    public int EventPriceId { get => _eventPriceId; set => Set(ref _eventPriceId, value, markDirty: false); }
    public int EventPriceAmount { get => _eventPriceAmount; set => Set(ref _eventPriceAmount, value, markDirty: false); }
    public string EventPriceName { get => _eventPriceName; set => Set(ref _eventPriceName, value, markDirty: false); }
    public string DisplayName => $"{ItemId} - {ItemName}";
    public string SellPriceDisplay => SellPrice > 0 ? $"{SellPrice:N0}" : "-";
    public string ScanPriceDisplay => ScanPrice > 0 ? $"{ScanPrice:N0}" : "-";
    public string BuyCostDisplay
    {
        get
        {
            if (EventPriceId > 0 && EventPriceAmount > 0)
            {
                var name = string.IsNullOrWhiteSpace(EventPriceName) ? $"Item {EventPriceId}" : EventPriceName;
                return $"{EventPriceAmount:N0}x {name}";
            }

            if (DigicorePrice > 0)
                return $"{DigicorePrice:N0} digicore";

            return SellPrice > 0 ? $"{SellPrice:N0} bits" : "-";
        }
    }

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(ItemId) or nameof(ItemName))
            Raise(nameof(DisplayName));

        if (propertyName is nameof(SellPrice))
        {
            Raise(nameof(SellPriceDisplay));
            Raise(nameof(BuyCostDisplay));
        }

        if (propertyName is nameof(ScanPrice))
            Raise(nameof(ScanPriceDisplay));

        if (propertyName is nameof(DigicorePrice) or nameof(EventPriceId) or nameof(EventPriceAmount) or nameof(EventPriceName))
            Raise(nameof(BuyCostDisplay));
    }
}

public sealed class NpcPortalEditorRow : DirtyRow
{
    private long _id;
    private int _portalType;
    private int _portalCount;
    private int _resourceCount;

    public long Id { get => _id; set => Set(ref _id, value); }
    public int PortalType { get => _portalType; set => Set(ref _portalType, value); }
    public int PortalCount { get => _portalCount; set => Set(ref _portalCount, value); }
    public int ResourceCount { get => _resourceCount; set => Set(ref _resourceCount, value); }
    public ObservableCollection<NpcPortalGroupEditorRow> Groups { get; } = [];
    public string Summary => $"Portal type {PortalType}, count {PortalCount}, resources {ResourceCount}";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(PortalType) or nameof(PortalCount) or nameof(ResourceCount))
            Raise(nameof(Summary));
    }
}

public sealed class NpcPortalGroupEditorRow : DirtyRow
{
    private long _id;

    public long Id { get => _id; set => Set(ref _id, value); }
    public ObservableCollection<NpcPortalResourceEditorRow> Resources { get; } = [];
}

public sealed class NpcPortalResourceEditorRow : DirtyRow
{
    private long _id;
    private int _valueA;
    private int _valueB;
    private int _valueC;

    public long Id { get => _id; set => Set(ref _id, value); }
    public int ValueA { get => _valueA; set => Set(ref _valueA, value); }
    public int ValueB { get => _valueB; set => Set(ref _valueB, value); }
    public int ValueC { get => _valueC; set => Set(ref _valueC, value); }
}

public sealed class NpcQuestLinkEditorRow : DirtyRow
{
    private int _questId;
    private int _actType;
    private int _completeState;
    private string _questTitle = string.Empty;

    public int QuestId { get => _questId; set => Set(ref _questId, value); }
    public int ActType { get => _actType; set => Set(ref _actType, value); }
    public int CompleteState { get => _completeState; set => Set(ref _completeState, value); }
    public string QuestTitle { get => _questTitle; set => Set(ref _questTitle, value, markDirty: false); }
}

public sealed class CraftRecipeEditorRow : DirtyRow
{
    private int _npcId;
    private int _sequencialId;
    private int _mainCategoryId;
    private string _mainCategoryName = string.Empty;
    private int _subCategoryId;
    private string _subCategoryName = string.Empty;
    private int _itemId;
    private string _itemName = string.Empty;
    private ImageSource? _iconImage;
    private int _amount;
    private int _successRate;
    private long _price;
    private int _percentageAssistGroup;
    private int _protectAssistGroup;

    public int NpcId { get => _npcId; set => Set(ref _npcId, value); }
    public int SequencialId { get => _sequencialId; set => Set(ref _sequencialId, value); }
    public int MainCategoryId { get => _mainCategoryId; set => Set(ref _mainCategoryId, value); }
    public string MainCategoryName { get => _mainCategoryName; set => Set(ref _mainCategoryName, value); }
    public int SubCategoryId { get => _subCategoryId; set => Set(ref _subCategoryId, value); }
    public string SubCategoryName { get => _subCategoryName; set => Set(ref _subCategoryName, value); }
    public int ItemId { get => _itemId; set => Set(ref _itemId, value); }
    public string ItemName { get => _itemName; set => Set(ref _itemName, value, markDirty: false); }
    public ImageSource? IconImage { get => _iconImage; set => Set(ref _iconImage, value, markDirty: false); }
    public int Amount { get => _amount; set => Set(ref _amount, value); }
    public int SuccessRate { get => _successRate; set => Set(ref _successRate, value); }
    public long Price { get => _price; set => Set(ref _price, value); }
    public int PercentageAssistGroup { get => _percentageAssistGroup; set => Set(ref _percentageAssistGroup, value); }
    public int ProtectAssistGroup { get => _protectAssistGroup; set => Set(ref _protectAssistGroup, value); }

    public ObservableCollection<CraftMaterialEditorRow> Materials { get; } = [];
    public string DisplayName => $"{SequencialId} - {ItemNameTitle}";
    public string ItemNameTitle => string.IsNullOrWhiteSpace(ItemName) ? $"Item {ItemId}" : ItemName;
    public string CategorySummary => $"{MainCategoryId} - {MainCategoryTitle} / {SubCategoryId} - {SubCategoryTitle}";
    public string MainCategoryTitle => string.IsNullOrWhiteSpace(MainCategoryName) ? "Main" : MainCategoryName;
    public string SubCategoryTitle => string.IsNullOrWhiteSpace(SubCategoryName) ? "Sub" : SubCategoryName;
    public string MaterialsSummary => Materials.Count == 0
        ? "No materials configured"
        : string.Join(", ", Materials.Select(x => $"{x.Amount}x {x.ItemNameTitle}"));
    public string Summary => $"{Amount}x {ItemNameTitle} | {SuccessRate}% | {Price:N0} bits | {MaterialsSummary}";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(SequencialId) or nameof(ItemId) or nameof(ItemName))
        {
            Raise(nameof(ItemNameTitle));
            Raise(nameof(DisplayName));
        }
        if (propertyName is nameof(MainCategoryId) or nameof(MainCategoryName) or nameof(SubCategoryId) or nameof(SubCategoryName))
            Raise(nameof(CategorySummary));
        if (propertyName is nameof(Amount) or nameof(SuccessRate) or nameof(Price) or nameof(ItemName))
            Raise(nameof(Summary));
    }

    public void RefreshMaterialsSummary()
    {
        Raise(nameof(MaterialsSummary));
        Raise(nameof(Summary));
    }
}

public sealed class CraftMaterialEditorRow : DirtyRow
{
    private int _itemId;
    private string _itemName = string.Empty;
    private ImageSource? _iconImage;
    private int _amount;

    public int ItemId { get => _itemId; set => Set(ref _itemId, value); }
    public string ItemName { get => _itemName; set => Set(ref _itemName, value, markDirty: false); }
    public ImageSource? IconImage { get => _iconImage; set => Set(ref _iconImage, value, markDirty: false); }
    public int Amount { get => _amount; set => Set(ref _amount, value); }
    public string ItemNameTitle => string.IsNullOrWhiteSpace(ItemName) ? $"Item {ItemId}" : ItemName;
    public string DisplayName => $"{Amount}x {ItemNameTitle}";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(ItemId) or nameof(ItemName) or nameof(Amount))
        {
            Raise(nameof(ItemNameTitle));
            Raise(nameof(DisplayName));
        }
    }
}

public sealed class QuestEditorRow : DirtyRow
{
    private int _questId;
    private int _questType;
    private int _level;
    private int _modelIdRecv;
    private int _modelIdComplete;
    private int _toolPosX;
    private int _toolPosY;
    private int _managedId;
    private int _startTargetType;
    private int _startTargetId;
    private int _targetType;
    private int _targetValue;
    private bool _active;
    private bool _immediate;
    private bool _reset;
    private string _title = string.Empty;
    private string _titleTab = string.Empty;
    private string _body = string.Empty;
    private string _simple = string.Empty;
    private string _helper = string.Empty;
    private string _process = string.Empty;
    private string _complete = string.Empty;
    private string _expert = string.Empty;
    private string _startNpcName = string.Empty;
    private string _targetNpcName = string.Empty;
    private string _modelRecvSummary = string.Empty;
    private string _modelCompleteSummary = string.Empty;

    public int QuestId { get => _questId; set => Set(ref _questId, value); }
    public int QuestType { get => _questType; set => Set(ref _questType, value); }
    public int Level { get => _level; set => Set(ref _level, value); }
    public int ModelIdRecv { get => _modelIdRecv; set => Set(ref _modelIdRecv, value); }
    public int ModelIdComplete { get => _modelIdComplete; set => Set(ref _modelIdComplete, value); }
    public int ToolPosX { get => _toolPosX; set => Set(ref _toolPosX, value); }
    public int ToolPosY { get => _toolPosY; set => Set(ref _toolPosY, value); }
    public int ManagedId { get => _managedId; set => Set(ref _managedId, value); }
    public int StartTargetType { get => _startTargetType; set => Set(ref _startTargetType, value); }
    public int StartTargetId { get => _startTargetId; set => Set(ref _startTargetId, value); }
    public int TargetType { get => _targetType; set => Set(ref _targetType, value); }
    public int TargetValue { get => _targetValue; set => Set(ref _targetValue, value); }
    public bool Active { get => _active; set => Set(ref _active, value); }
    public bool Immediate { get => _immediate; set => Set(ref _immediate, value); }
    public bool Reset { get => _reset; set => Set(ref _reset, value); }
    public string Title { get => _title; set => Set(ref _title, value); }
    public string TitleTab { get => _titleTab; set => Set(ref _titleTab, value); }
    public string Body { get => _body; set => Set(ref _body, value); }
    public string Simple { get => _simple; set => Set(ref _simple, value); }
    public string Helper { get => _helper; set => Set(ref _helper, value); }
    public string Process { get => _process; set => Set(ref _process, value); }
    public string Complete { get => _complete; set => Set(ref _complete, value); }
    public string Expert { get => _expert; set => Set(ref _expert, value); }
    public string StartNpcName { get => _startNpcName; set => Set(ref _startNpcName, value, markDirty: false); }
    public string TargetNpcName { get => _targetNpcName; set => Set(ref _targetNpcName, value, markDirty: false); }
    public string ModelRecvSummary { get => _modelRecvSummary; set => Set(ref _modelRecvSummary, value, markDirty: false); }
    public string ModelCompleteSummary { get => _modelCompleteSummary; set => Set(ref _modelCompleteSummary, value, markDirty: false); }

    public ObservableCollection<QuestSupplyEditorRow> Supplies { get; } = [];
    public ObservableCollection<QuestConditionEditorRow> Conditions { get; } = [];
    public ObservableCollection<QuestGoalEditorRow> Goals { get; } = [];
    public ObservableCollection<QuestRewardEditorRow> Rewards { get; } = [];
    public ObservableCollection<QuestEventEditorRow> Events { get; } = [];
    public ObservableCollection<QuestUnlockEditorRow> Unlocks { get; } = [];

    public string QuestTypeName => QuestTypeCatalog.NameOf(QuestType);
    public string DisplayName => $"{QuestId} - {TitleTitle}";
    public string TitleTitle => string.IsNullOrWhiteSpace(Title) ? $"Quest {QuestId}" : FirstLine(Title);
    public string Summary => $"{QuestTypeName} | level {Level} | start {StartTargetSummary} | complete {TargetSummary}";
    public string StartTargetSummary => StartTargetType == 0
        ? $"NPC {StartTargetId}{FormatResolved(StartNpcName)}"
        : $"Digivice/other {StartTargetId}";
    public string TargetSummary => TargetType switch
    {
        0 => "Self completion",
        1 => $"NPC {TargetValue}{FormatResolved(TargetNpcName)}",
        2 => "Complete immediately",
        _ => $"Target type {TargetType}: {TargetValue}"
    };

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(QuestId) or nameof(Title))
        {
            Raise(nameof(TitleTitle));
            Raise(nameof(DisplayName));
        }
        if (propertyName is nameof(QuestType) or nameof(Level) or nameof(StartTargetType) or nameof(StartTargetId) or nameof(TargetType) or nameof(TargetValue) or nameof(StartNpcName) or nameof(TargetNpcName))
        {
            Raise(nameof(QuestTypeName));
            Raise(nameof(StartTargetSummary));
            Raise(nameof(TargetSummary));
            Raise(nameof(Summary));
        }
    }

    private static string FormatResolved(string value)
        => string.IsNullOrWhiteSpace(value) ? string.Empty : $" - {value}";

    private static string FirstLine(string value)
    {
        var normalized = value.Replace("\r\n", "\n").Replace('\r', '\n');
        return normalized.Split('\n', 2)[0].Trim();
    }
}

public sealed class QuestSupplyEditorRow : DirtyRow
{
    private int _type;
    private int _itemId;
    private int _amount;
    private string _itemName = string.Empty;
    private ImageSource? _iconImage;

    public int Type { get => _type; set => Set(ref _type, value); }
    public int ItemId { get => _itemId; set => Set(ref _itemId, value); }
    public int Amount { get => _amount; set => Set(ref _amount, value); }
    public string ItemName { get => _itemName; set => Set(ref _itemName, value, markDirty: false); }
    public ImageSource? IconImage { get => _iconImage; set => Set(ref _iconImage, value, markDirty: false); }
    public string DisplayName => $"{ItemId} - {ItemName}";
    public string TypeName => QuestSupplyCatalog.NameOf(Type);
    public string Summary => $"{TypeName}: {Amount}x {(string.IsNullOrWhiteSpace(ItemName) ? $"Item {ItemId}" : ItemName)}";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(Type) or nameof(ItemId) or nameof(Amount) or nameof(ItemName))
        {
            Raise(nameof(TypeName));
            Raise(nameof(DisplayName));
            Raise(nameof(Summary));
        }
    }
}

public sealed class QuestConditionEditorRow : DirtyRow
{
    private int _conditionType;
    private int _conditionId;
    private int _conditionCount;
    private string _resolvedName = string.Empty;

    public int ConditionType { get => _conditionType; set => Set(ref _conditionType, value); }
    public int ConditionId { get => _conditionId; set => Set(ref _conditionId, value); }
    public int ConditionCount { get => _conditionCount; set => Set(ref _conditionCount, value); }
    public string ResolvedName { get => _resolvedName; set => Set(ref _resolvedName, value, markDirty: false); }
    public string ConditionTypeName => QuestPreconditionCatalog.NameOf(ConditionType);
    public string Summary => $"{ConditionTypeName}: {ConditionId}{(string.IsNullOrWhiteSpace(ResolvedName) ? string.Empty : $" - {ResolvedName}")} x{ConditionCount}";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(ConditionType) or nameof(ConditionId) or nameof(ConditionCount) or nameof(ResolvedName))
        {
            Raise(nameof(ConditionTypeName));
            Raise(nameof(Summary));
        }
    }
}

public sealed class QuestGoalEditorRow : DirtyRow
{
    private int _goalType;
    private int _goalId;
    private int _goalAmount;
    private int _curTypeCount;
    private int _subValue;
    private int _subValueTwo;
    private string _resolvedName = string.Empty;
    private ImageSource? _iconImage;

    public int GoalType { get => _goalType; set => Set(ref _goalType, value); }
    public int GoalId { get => _goalId; set => Set(ref _goalId, value); }
    public int GoalAmount { get => _goalAmount; set => Set(ref _goalAmount, value); }
    public int CurTypeCount { get => _curTypeCount; set => Set(ref _curTypeCount, value); }
    public int SubValue { get => _subValue; set => Set(ref _subValue, value); }
    public int SubValueTwo { get => _subValueTwo; set => Set(ref _subValueTwo, value); }
    public string ResolvedName { get => _resolvedName; set => Set(ref _resolvedName, value, markDirty: false); }
    public ImageSource? IconImage { get => _iconImage; set => Set(ref _iconImage, value, markDirty: false); }
    public string GoalTypeName => QuestGoalCatalog.NameOf(GoalType);
    public string TargetLabel => GoalType switch
    {
        0 => "Monster ID",
        1 => "Item ID",
        2 => "Client action",
        3 => "Region ID",
        4 => "NPC ID",
        5 => "Item ID",
        6 => "Item ID",
        7 => "Item ID",
        8 => "Item ID",
        9 => "Level",
        10 => "Partner ID",
        100 => "Map ID",
        101 => "Employment ID",
        _ => "Target ID"
    };
    public string TargetTitle => string.IsNullOrWhiteSpace(ResolvedName) ? $"{TargetLabel} {GoalId}" : ResolvedName;
    public string FriendlySummary => GoalType switch
    {
        0 => $"Defeat {GoalAmount}x {TargetTitle}",
        1 => $"Collect {GoalAmount}x {TargetTitle}",
        2 => $"Complete client action {GoalId}",
        3 => $"Reach region {GoalId}",
        4 => $"Talk to {TargetTitle}",
        5 => $"Use {GoalAmount}x {TargetTitle}",
        6 => $"Use {TargetTitle} at NPC target",
        7 => $"Use {TargetTitle} on monster target",
        8 => $"Use {TargetTitle} at region target",
        9 => $"Reach level {GoalId}",
        10 => $"Acquire partner {TargetTitle}",
        100 => $"Show map marker {GoalId}",
        101 => $"Employment marker {GoalId}",
        _ => $"{GoalTypeName}: {TargetLabel} {GoalId} x{GoalAmount}"
    };
    public string AdvancedSummary => $"CurType {CurTypeCount} | Sub A {SubValue} | Sub B {SubValueTwo}";
    public string Summary => $"{GoalTypeName}: {GoalId}{(string.IsNullOrWhiteSpace(ResolvedName) ? string.Empty : $" - {ResolvedName}")} x{GoalAmount}";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(GoalType) or nameof(GoalId) or nameof(GoalAmount) or nameof(ResolvedName))
        {
            Raise(nameof(GoalTypeName));
            Raise(nameof(TargetLabel));
            Raise(nameof(TargetTitle));
            Raise(nameof(FriendlySummary));
            Raise(nameof(Summary));
        }
        if (propertyName is nameof(CurTypeCount) or nameof(SubValue) or nameof(SubValueTwo))
            Raise(nameof(AdvancedSummary));
    }
}

public sealed class QuestRewardEditorRow : DirtyRow
{
    private int _method;
    private int _rewardType;
    private int _valueA;
    private int _valueB;
    private string _resolvedName = string.Empty;
    private ImageSource? _iconImage;

    public int Method { get => _method; set => Set(ref _method, value); }
    public int RewardType { get => _rewardType; set => Set(ref _rewardType, value); }
    public int ValueA { get => _valueA; set => Set(ref _valueA, value); }
    public int ValueB { get => _valueB; set => Set(ref _valueB, value); }
    public string ResolvedName { get => _resolvedName; set => Set(ref _resolvedName, value, markDirty: false); }
    public ImageSource? IconImage { get => _iconImage; set => Set(ref _iconImage, value, markDirty: false); }
    public string MethodName => QuestRewardCatalog.MethodNameOf(Method);
    public string RewardTypeName => QuestRewardCatalog.TypeNameOf(RewardType);
    public string ValueALabel => RewardType switch
    {
        0 => "Bits",
        1 => "EXP",
        2 => "Item ID",
        3 => "Evo slot",
        _ => "Value A"
    };
    public string ValueBLabel => RewardType switch
    {
        0 => "Unused",
        1 => "Unused",
        2 => "Amount",
        3 => "Value",
        _ => "Value B"
    };
    public string Summary => RewardType switch
    {
        0 => $"Money {ValueA:N0}",
        1 => $"EXP {ValueA:N0}",
        2 => $"{ValueA} - {ResolvedName} x{ValueB}",
        3 => $"Evolution slot {ValueA}: {ValueB}",
        _ => $"Type {RewardType}: A {ValueA}, B {ValueB}"
    };

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(Method))
            Raise(nameof(MethodName));

        if (propertyName is nameof(RewardType) or nameof(ValueA) or nameof(ValueB) or nameof(ResolvedName))
        {
            Raise(nameof(RewardTypeName));
            Raise(nameof(ValueALabel));
            Raise(nameof(ValueBLabel));
            Raise(nameof(Summary));
        }
    }
}

public sealed class QuestEventEditorRow : DirtyRow
{
    private int _eventSlot;
    private int _eventId;

    public int EventSlot { get => _eventSlot; set => Set(ref _eventSlot, value); }
    public int EventId { get => _eventId; set => Set(ref _eventId, value); }
    public string EventName => QuestEventCatalog.NameOf(EventSlot);

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName == nameof(EventSlot))
            Raise(nameof(EventName));
    }
}

public sealed class QuestUnlockEditorRow : DirtyRow
{
    private int _questId;
    private string _title = string.Empty;
    private string _questTypeName = string.Empty;
    private int _level;

    public int QuestId { get => _questId; set => Set(ref _questId, value, markDirty: false); }
    public string Title { get => _title; set => Set(ref _title, value, markDirty: false); }
    public string QuestTypeName { get => _questTypeName; set => Set(ref _questTypeName, value, markDirty: false); }
    public int Level { get => _level; set => Set(ref _level, value, markDirty: false); }
}
