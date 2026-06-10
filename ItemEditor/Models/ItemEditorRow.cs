using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Media;

namespace ItemEditor.Models;

public sealed class ItemEditorRow : INotifyPropertyChanged
{
    private int _itemId;
    private string _name = string.Empty;
    private string _description = string.Empty;
    private string _typeComment = string.Empty;
    private int _icon;
    private ushort _class;
    private ushort _typeL;
    private ushort _typeS;
    private int _typeValue;
    private int _section;
    private ushort _sellType;
    private byte _useMode;
    private ushort _cooldownGroup;
    private short _overlap;
    private long _skillCode;
    private short _applyValueMin;
    private short _applyValueMax;
    private short _applyElement;
    private int _boundType;
    private int _useTimeType;
    private int _usageTimeMinutes;
    private uint _scanPrice;
    private uint _sellPrice;
    private string _effectKind = "Unknown";
    private string _effectSummary = string.Empty;
    private ImageSource? _iconImage;
    private string _iconStatus = string.Empty;
    private bool _isDirty;

    public byte[] RawRecord { get; set; } = [];
    public bool IsNew { get; set; }

    public int ItemId { get => _itemId; set => Set(ref _itemId, value); }
    public string Name { get => _name; set => Set(ref _name, value); }
    public string Description { get => _description; set => Set(ref _description, value); }
    public string TypeComment { get => _typeComment; set => Set(ref _typeComment, value); }
    public int Icon { get => _icon; set => Set(ref _icon, value); }
    public ushort Class { get => _class; set => Set(ref _class, value); }
    public ushort TypeL { get => _typeL; set => Set(ref _typeL, value); }
    public ushort TypeS { get => _typeS; set => Set(ref _typeS, value); }
    public int TypeValue { get => _typeValue; set => Set(ref _typeValue, value); }
    public int Section { get => _section; set => Set(ref _section, value); }
    public ushort SellType { get => _sellType; set => Set(ref _sellType, value); }
    public byte UseMode { get => _useMode; set => Set(ref _useMode, value); }
    public ushort CooldownGroup { get => _cooldownGroup; set => Set(ref _cooldownGroup, value); }
    public short Overlap { get => _overlap; set => Set(ref _overlap, value); }
    public long SkillCode { get => _skillCode; set => Set(ref _skillCode, value); }
    public short ApplyValueMin { get => _applyValueMin; set => Set(ref _applyValueMin, value); }
    public short ApplyValueMax { get => _applyValueMax; set => Set(ref _applyValueMax, value); }
    public short ApplyElement { get => _applyElement; set => Set(ref _applyElement, value); }
    public int BoundType { get => _boundType; set => Set(ref _boundType, value); }
    public int UseTimeType { get => _useTimeType; set => Set(ref _useTimeType, value); }
    public int UsageTimeMinutes { get => _usageTimeMinutes; set => Set(ref _usageTimeMinutes, value); }
    public uint ScanPrice { get => _scanPrice; set => Set(ref _scanPrice, value); }
    public uint SellPrice { get => _sellPrice; set => Set(ref _sellPrice, value); }
    public string EffectKind { get => _effectKind; set => Set(ref _effectKind, value, markDirty: false); }
    public string EffectSummary { get => _effectSummary; set => Set(ref _effectSummary, value, markDirty: false); }
    public ImageSource? IconImage { get => _iconImage; set => Set(ref _iconImage, value, markDirty: false); }
    public string IconStatus { get => _iconStatus; set => Set(ref _iconStatus, value, markDirty: false); }
    public bool IsDirty { get => _isDirty; set => Set(ref _isDirty, value, markDirty: false); }

    public string TypeDisplay => $"{TypeL:D3}.{TypeS:D2}";
    public string NameTitle => FirstLine(Name);
    public string DisplayName => $"{ItemId} - {NameTitle}";
    public string DescriptionPreview => FirstLine(Description);
    public string DurationModeName => UseTimeType switch
    {
        0 => "Permanent item",
        1 => "Temporary item",
        2 => "Temporary item, expire action: quit/disable",
        3 => "Temporary item, expire action: remove",
        4 => "Timed effect only, no item expiration",
        _ => $"Unknown timer mode {UseTimeType}"
    };
    public string DurationSummary
    {
        get
        {
            if (UsageTimeMinutes <= 0 || UseTimeType is 0 or 4)
                return $"{DurationModeName}. No inventory/equipment expiration timer.";

            return $"{DurationModeName}. Duration {FormatMinutes(UsageTimeMinutes)} ({UsageTimeMinutes} minutes).";
        }
    }

    public event PropertyChangedEventHandler? PropertyChanged;

    public void MarkClean()
    {
        IsDirty = false;
        IsNew = false;
    }

    private void Set<T>(ref T field, T value, [CallerMemberName] string? propertyName = null, bool markDirty = true)
    {
        if (EqualityComparer<T>.Default.Equals(field, value))
            return;

        field = value;
        if (markDirty)
            _isDirty = true;

        OnPropertyChanged(propertyName);
        if (propertyName is nameof(ItemId) or nameof(Name))
        {
            OnPropertyChanged(nameof(NameTitle));
            OnPropertyChanged(nameof(DisplayName));
        }
        if (propertyName is nameof(Description))
            OnPropertyChanged(nameof(DescriptionPreview));
        if (propertyName is nameof(TypeL) or nameof(TypeS))
            OnPropertyChanged(nameof(TypeDisplay));
        if (propertyName is nameof(UseTimeType) or nameof(UsageTimeMinutes))
        {
            OnPropertyChanged(nameof(DurationModeName));
            OnPropertyChanged(nameof(DurationSummary));
        }
        if (markDirty)
            OnPropertyChanged(nameof(IsDirty));
    }

    private void OnPropertyChanged(string? propertyName)
        => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

    private static string FirstLine(string value)
    {
        if (string.IsNullOrWhiteSpace(value))
            return string.Empty;

        var normalized = value.Replace("\r\n", "\n").Replace('\r', '\n');
        return normalized.Split('\n', 2)[0].Trim();
    }

    private static string FormatMinutes(int minutes)
    {
        if (minutes <= 0)
            return "0 minutes";

        var days = minutes / 1440;
        var hours = minutes % 1440 / 60;
        var mins = minutes % 60;
        var parts = new List<string>();
        if (days > 0)
            parts.Add($"{days} day{(days == 1 ? string.Empty : "s")}");
        if (hours > 0)
            parts.Add($"{hours} hour{(hours == 1 ? string.Empty : "s")}");
        if (mins > 0 || parts.Count == 0)
            parts.Add($"{mins} minute{(mins == 1 ? string.Empty : "s")}");

        return string.Join(" ", parts);
    }
}
