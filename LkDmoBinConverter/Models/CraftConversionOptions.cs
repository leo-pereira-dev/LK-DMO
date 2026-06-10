namespace LkDmoBinConverter.Models;

public sealed class CraftConversionOptions
{
    public string RootPath { get; init; } = string.Empty;
    public string GdmoBinPath { get; init; } = string.Empty;
    public string LkdmoInputPath { get; init; } = string.Empty;
    public string ServerBinPath { get; init; } = string.Empty;
    public string OutputDirectory { get; init; } = string.Empty;
    public bool PatchEmbeddedItemListCraft { get; init; }
}
