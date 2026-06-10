using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace ItemEditor.Models;

public abstract class DirtyRow : INotifyPropertyChanged
{
    private bool _isDirty;

    public bool IsDirty
    {
        get => _isDirty;
        set => Set(ref _isDirty, value, markDirty: false);
    }

    public event PropertyChangedEventHandler? PropertyChanged;

    public void MarkClean() => IsDirty = false;

    protected void Set<T>(ref T field, T value, [CallerMemberName] string? propertyName = null, bool markDirty = true)
    {
        if (EqualityComparer<T>.Default.Equals(field, value))
            return;

        field = value;
        if (markDirty)
            _isDirty = true;

        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        OnValueChanged(propertyName);
        if (markDirty)
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(IsDirty)));
    }

    protected void Raise(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

    protected virtual void OnValueChanged(string? propertyName)
    {
    }
}

public sealed class SkillEditorRow : DirtyRow
{
    private int _id;
    private string _name = string.Empty;
    private string _description = string.Empty;
    private ushort _icon;
    private ushort _target;
    private ushort _attType;
    private ushort _levelupPoint;
    private ushort _maxLevel;
    private ushort _attributeType;
    private ushort _natureType;
    private ushort _familyType;
    private ushort _useHp;
    private ushort _useDs;
    private float _attRange;
    private float _minDamage;
    private float _normalDamage;
    private float _maxDamage;
    private float _attSphere;
    private float _castingTime;
    private float _damageTime;
    private ushort _damageDay;
    private float _distanceTime;
    private float _cooldownTimeMs;
    private ushort _cooldownDay;
    private float _skillVelocity;
    private float _skillAccel;
    private ushort _skillType;
    private ushort _limitLevel;
    private ushort _skillGroup;
    private ushort _skillRank;
    private ushort _memorySkill;
    private ushort _reqItem;

    public byte[] RawRecord { get; set; } = [];
    public SkillApplyEditor Apply0 { get; } = new() { SlotName = "Apply 0" };
    public SkillApplyEditor Apply1 { get; } = new() { SlotName = "Apply 1" };
    public SkillApplyEditor Apply2 { get; } = new() { SlotName = "Apply 2" };
    public IReadOnlyList<SkillApplyEditor> Applies => [Apply0, Apply1, Apply2];

    public SkillEditorRow()
    {
        Apply0.PropertyChanged += (_, _) => Raise(nameof(VisualSummary));
        Apply1.PropertyChanged += (_, _) => Raise(nameof(VisualSummary));
        Apply2.PropertyChanged += (_, _) => Raise(nameof(VisualSummary));
    }

    public int Id { get => _id; set => Set(ref _id, value); }
    public string Name { get => _name; set => Set(ref _name, value); }
    public string Description { get => _description; set => Set(ref _description, value); }
    public ushort Icon { get => _icon; set => Set(ref _icon, value); }
    public ushort Target { get => _target; set => Set(ref _target, value); }
    public ushort AttType { get => _attType; set => Set(ref _attType, value); }
    public ushort LevelupPoint { get => _levelupPoint; set => Set(ref _levelupPoint, value); }
    public ushort MaxLevel { get => _maxLevel; set => Set(ref _maxLevel, value); }
    public ushort AttributeType { get => _attributeType; set => Set(ref _attributeType, value); }
    public ushort NatureType { get => _natureType; set => Set(ref _natureType, value); }
    public ushort FamilyType { get => _familyType; set => Set(ref _familyType, value); }
    public ushort UseHP { get => _useHp; set => Set(ref _useHp, value); }
    public ushort UseDS { get => _useDs; set => Set(ref _useDs, value); }
    public float AttRange { get => _attRange; set => Set(ref _attRange, value); }
    public float MinDamage { get => _minDamage; set => Set(ref _minDamage, value); }
    public float NormalDamage { get => _normalDamage; set => Set(ref _normalDamage, value); }
    public float MaxDamage { get => _maxDamage; set => Set(ref _maxDamage, value); }
    public float AttSphere { get => _attSphere; set => Set(ref _attSphere, value); }
    public float CastingTime { get => _castingTime; set => Set(ref _castingTime, value); }
    public float DamageTime { get => _damageTime; set => Set(ref _damageTime, value); }
    public ushort DamageDay { get => _damageDay; set => Set(ref _damageDay, value); }
    public float DistanceTime { get => _distanceTime; set => Set(ref _distanceTime, value); }
    public float CooldownTimeMs { get => _cooldownTimeMs; set => Set(ref _cooldownTimeMs, value); }
    public ushort CooldownDay { get => _cooldownDay; set => Set(ref _cooldownDay, value); }
    public float SkillVelocity { get => _skillVelocity; set => Set(ref _skillVelocity, value); }
    public float SkillAccel { get => _skillAccel; set => Set(ref _skillAccel, value); }
    public ushort SkillType { get => _skillType; set => Set(ref _skillType, value); }
    public ushort LimitLevel { get => _limitLevel; set => Set(ref _limitLevel, value); }
    public ushort SkillGroup { get => _skillGroup; set => Set(ref _skillGroup, value); }
    public ushort SkillRank { get => _skillRank; set => Set(ref _skillRank, value); }
    public ushort MemorySkill { get => _memorySkill; set => Set(ref _memorySkill, value); }
    public ushort ReqItem { get => _reqItem; set => Set(ref _reqItem, value); }

    public string Title => $"{Id} - {FirstLine(Name)}";
    public string DescriptionPreview => FirstLine(Description);
    public string CooldownDisplay => CooldownTimeMs <= 0 ? "0 ms" : $"{CooldownTimeMs:0.##} ms ({CooldownTimeMs / 1000:0.##} s)";
    public string TimingSummary => $"Casting {CastingTime:0.##} | Damage {DamageTime:0.##} | Distance {DistanceTime:0.##} | Cooldown {CooldownDisplay}";
    public string RequirementSummary => $"MaxLv {MaxLevel}, LimitLv {LimitLevel}, Use HP {UseHP}, Use DS {UseDS}, ReqItem {ReqItem}";
    public string RangeSummary => $"Range {AttRange:0.##}, Sphere {AttSphere:0.##}, Damage {MinDamage:0.##}/{NormalDamage:0.##}/{MaxDamage:0.##}";
    public string VisualSummary
    {
        get
        {
            var effects = Applies.Where(x => x.HasEffect).Select(x => x.EffectSentence).ToList();
            return effects.Count == 0 ? "No active apply effect" : string.Join("  |  ", effects);
        }
    }

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(Id) or nameof(Name))
            Raise(nameof(Title));
        if (propertyName is nameof(Description))
            Raise(nameof(DescriptionPreview));
        if (propertyName is nameof(CastingTime) or nameof(DamageTime) or nameof(DistanceTime) or nameof(CooldownTimeMs))
            Raise(nameof(TimingSummary));
        if (propertyName is nameof(MaxLevel) or nameof(LimitLevel) or nameof(UseHP) or nameof(UseDS) or nameof(ReqItem))
            Raise(nameof(RequirementSummary));
        if (propertyName is nameof(AttRange) or nameof(AttSphere) or nameof(MinDamage) or nameof(NormalDamage) or nameof(MaxDamage))
            Raise(nameof(RangeSummary));
    }

    private static string FirstLine(string value)
    {
        if (string.IsNullOrWhiteSpace(value))
            return string.Empty;

        var normalized = value.Replace("\r\n", "\n").Replace('\r', '\n');
        return normalized.Split('\n', 2)[0].Trim();
    }
}

public sealed class SkillApplyEditor : DirtyRow
{
    private string _slotName = string.Empty;
    private int _a;
    private int _invokeRate;
    private int _b;
    private int _c;
    private ushort _buffCode;
    private ushort _id;
    private short _increaseBPoint;
    private string _applyName = string.Empty;
    private string _methodName = string.Empty;
    private string _applyLabel = string.Empty;
    private string _methodLabel = string.Empty;
    private string _effectSentence = string.Empty;
    private string _resolvedBuffTitle = string.Empty;
    private string _resolvedBuffDescription = string.Empty;
    private string _resolvedBuffStaticTime = string.Empty;

    public string SlotName { get => _slotName; set => Set(ref _slotName, value, markDirty: false); }
    public int A { get => _a; set => Set(ref _a, value); }
    public int InvokeRate { get => _invokeRate; set => Set(ref _invokeRate, value); }
    public int B { get => _b; set => Set(ref _b, value); }
    public int C { get => _c; set => Set(ref _c, value); }
    public ushort BuffCode { get => _buffCode; set => Set(ref _buffCode, value); }
    public ushort Id { get => _id; set => Set(ref _id, value); }
    public short IncreaseBPoint { get => _increaseBPoint; set => Set(ref _increaseBPoint, value); }
    public string ApplyName { get => _applyName; set => Set(ref _applyName, value, markDirty: false); }
    public string MethodName { get => _methodName; set => Set(ref _methodName, value, markDirty: false); }
    public string ApplyLabel { get => _applyLabel; set => Set(ref _applyLabel, value, markDirty: false); }
    public string MethodLabel { get => _methodLabel; set => Set(ref _methodLabel, value, markDirty: false); }
    public string EffectSentence { get => _effectSentence; set => Set(ref _effectSentence, value, markDirty: false); }
    public string ResolvedBuffTitle { get => _resolvedBuffTitle; set => Set(ref _resolvedBuffTitle, value, markDirty: false); }
    public string ResolvedBuffDescription { get => _resolvedBuffDescription; set => Set(ref _resolvedBuffDescription, value, markDirty: false); }
    public string ResolvedBuffStaticTime { get => _resolvedBuffStaticTime; set => Set(ref _resolvedBuffStaticTime, value, markDirty: false); }

    public string Title => $"{SlotName}: {ApplyName}";
    public string MethodSummary => $"{MethodName} | Invoke {InvokeRate} | B {B} | C {C} | IncB {IncreaseBPoint}";
    public bool HasEffect => Id != 0 || A != 0 || B != 0 || C != 0 || BuffCode != 0;

    public void RefreshVisual()
    {
        ApplyName = SkillEffectCatalog.ApplyName(A);
        ApplyLabel = SkillEffectCatalog.ApplyLabel(A);
        MethodName = SkillEffectCatalog.MethodName(Id);
        MethodLabel = SkillEffectCatalog.MethodLabel(Id);
        EffectSentence = SkillEffectCatalog.BuildEffectSentence(this);
    }

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(A) or nameof(Id) or nameof(B) or nameof(C) or nameof(IncreaseBPoint) or nameof(BuffCode))
            RefreshVisual();
        if (propertyName is nameof(SlotName) or nameof(ApplyName))
            Raise(nameof(Title));
        if (propertyName is nameof(MethodName) or nameof(InvokeRate) or nameof(B) or nameof(C) or nameof(IncreaseBPoint))
            Raise(nameof(MethodSummary));
        if (propertyName is nameof(Id) or nameof(A) or nameof(B) or nameof(C) or nameof(BuffCode))
            Raise(nameof(HasEffect));
    }
}

public sealed class BuffEditorRow : DirtyRow
{
    private ushort _id;
    private string _name = string.Empty;
    private string _description = string.Empty;
    private ushort _icon;
    private ushort _buffType;
    private ushort _lifeType;
    private ushort _timeType;
    private ushort _minLevel;
    private ushort _buffClass;
    private uint _skillCode;
    private uint _digimonSkillCode;
    private ushort _conditionLevel;
    private bool _isDeleted;
    private string _effectFile = string.Empty;
    private string _linkedSkillTitle = string.Empty;

    public byte[] RawRecord { get; set; } = [];

    public ushort Id { get => _id; set => Set(ref _id, value); }
    public string Name { get => _name; set => Set(ref _name, value); }
    public string Description { get => _description; set => Set(ref _description, value); }
    public ushort Icon { get => _icon; set => Set(ref _icon, value); }
    public ushort BuffType { get => _buffType; set => Set(ref _buffType, value); }
    public ushort LifeType { get => _lifeType; set => Set(ref _lifeType, value); }
    public ushort TimeType { get => _timeType; set => Set(ref _timeType, value); }
    public ushort MinLevel { get => _minLevel; set => Set(ref _minLevel, value); }
    public ushort BuffClass { get => _buffClass; set => Set(ref _buffClass, value); }
    public uint SkillCode { get => _skillCode; set => Set(ref _skillCode, value); }
    public uint DigimonSkillCode { get => _digimonSkillCode; set => Set(ref _digimonSkillCode, value); }
    public ushort ConditionLevel { get => _conditionLevel; set => Set(ref _conditionLevel, value); }
    public bool IsDeleted { get => _isDeleted; set => Set(ref _isDeleted, value); }
    public string EffectFile { get => _effectFile; set => Set(ref _effectFile, value); }
    public string LinkedSkillTitle { get => _linkedSkillTitle; set => Set(ref _linkedSkillTitle, value, markDirty: false); }

    public string Title => $"{Id} - {FirstLine(Name)}";
    public string DescriptionPreview => FirstLine(Description);
    public string BuffTypeName => BuffType switch
    {
        1 => "1 - Buff",
        2 => "2 - Debuff",
        3 => "3 - Client system buff",
        _ => $"{BuffType}"
    };
    public string LifeTypeName => LifeType switch
    {
        1 => "1 - Die clear",
        2 => "2 - Die none",
        _ => $"{LifeType}"
    };
    public string TimeTypeName => TimeType switch
    {
        1 => "1 - Flat_Time",
        2 => "2 - Elapse_Time",
        3 => "3 - LogIn_Time",
        4 => "4 - NotCoreSave_Time",
        5 => "5 - VIPAutoPayment_Time",
        _ => $"{TimeType}"
    };
    public string StaticTimeSummary => $"TimeType {TimeTypeName}; LifeType {LifeTypeName}; runtime end timestamp is sent by the server packet.";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(Id) or nameof(Name))
            Raise(nameof(Title));
        if (propertyName is nameof(Description))
            Raise(nameof(DescriptionPreview));
        if (propertyName is nameof(BuffType))
            Raise(nameof(BuffTypeName));
        if (propertyName is nameof(LifeType))
        {
            Raise(nameof(LifeTypeName));
            Raise(nameof(StaticTimeSummary));
        }
        if (propertyName is nameof(TimeType))
        {
            Raise(nameof(TimeTypeName));
            Raise(nameof(StaticTimeSummary));
        }
    }

    private static string FirstLine(string value)
    {
        if (string.IsNullOrWhiteSpace(value))
            return string.Empty;

        var normalized = value.Replace("\r\n", "\n").Replace('\r', '\n');
        return normalized.Split('\n', 2)[0].Trim();
    }
}

public sealed class ContainerEditorRow : DirtyRow
{
    private long _id;
    private int _itemId;
    private string _itemName = string.Empty;
    private int _rewardAmount;

    public long Id { get => _id; set => Set(ref _id, value); }
    public int ItemId { get => _itemId; set => Set(ref _itemId, value); }
    public string ItemName { get => _itemName; set => Set(ref _itemName, value); }
    public int RewardAmount { get => _rewardAmount; set => Set(ref _rewardAmount, value); }
    public List<ContainerRewardEditorRow> Rewards { get; } = [];

    public string DropBehaviorSummary
    {
        get
        {
            if (Rewards.Count == 0)
                return "No rewards configured for this box.";

            if (RewardAmount <= 0)
                return "Reward Amount is 0, so the server will not finish a normal box reward roll.";

            if (RewardAmount >= Rewards.Count)
                return $"Drops all {Rewards.Count} configured reward row(s). Chance is ignored by the server in this mode.";

            return $"Drops {RewardAmount} random reward(s) from {Rewards.Count} possible row(s). Each row uses its own Chance %, then Min/Max decides stack amount.";
        }
    }

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName == nameof(RewardAmount))
            Raise(nameof(DropBehaviorSummary));
    }

    public void RefreshRewardSummary()
        => Raise(nameof(DropBehaviorSummary));
}

public sealed class ContainerRewardEditorRow : DirtyRow
{
    private long _id;
    private int _itemId;
    private int _minAmount;
    private int _maxAmount;
    private double _chance;
    private bool _rare;
    private string _itemName = string.Empty;
    private long _containerAssetId;

    public long Id { get => _id; set => Set(ref _id, value); }
    public int ItemId { get => _itemId; set => Set(ref _itemId, value); }
    public int MinAmount { get => _minAmount; set => Set(ref _minAmount, value); }
    public int MaxAmount { get => _maxAmount; set => Set(ref _maxAmount, value); }
    public double Chance { get => _chance; set => Set(ref _chance, value); }
    public bool Rare { get => _rare; set => Set(ref _rare, value); }
    public string ItemName { get => _itemName; set => Set(ref _itemName, value); }
    public long ContainerAssetId { get => _containerAssetId; set => Set(ref _containerAssetId, value); }
    public string DisplayName => $"{ItemId} - {ItemName}";

    protected override void OnValueChanged(string? propertyName)
    {
        if (propertyName is nameof(ItemId) or nameof(ItemName))
            Raise(nameof(DisplayName));
    }
}
