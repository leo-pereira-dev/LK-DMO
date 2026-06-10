namespace LkDmoBinConverter.Models;

internal sealed class CreationStringMain
{
    public string Name { get; init; } = string.Empty;
    public Dictionary<int, string> Subs { get; } = [];
}

internal sealed class ConvertedCraftPayload
{
    public byte[] Bytes { get; init; } = [];
    public int NpcCount { get; init; }
    public int MainCategories { get; init; }
    public int SubCategories { get; init; }
    public int Products { get; init; }
    public int Materials { get; init; }
    public int LimitReferences { get; init; }
    public int MissingNames { get; init; }
}

internal sealed class CraftStats
{
    public int NpcCount { get; init; }
    public int Products { get; init; }
    public int Materials { get; init; }
    public int EndOffset { get; init; }
}

internal sealed class AssistStats
{
    public int Groups { get; init; }
    public int Items { get; init; }
    public int EndOffset { get; init; }
}

internal sealed class LimitGroupStats
{
    public int Records { get; init; }
    public int RecordSize { get; init; }
}
