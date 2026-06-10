using System.Text;

namespace LkDmoBinConverter.Models;

public sealed class MapConversionReport
{
    public bool Success { get; set; }
    public string OutputDirectory { get; set; } = string.Empty;
    public string MapListOutputPath { get; set; } = string.Empty;
    public string DiffCsvPath { get; set; } = string.Empty;
    public string? PackBackupPf { get; set; }
    public string? PackBackupHf { get; set; }
    public int OldMapCount { get; set; }
    public int GdmoMapCount { get; set; }
    public int NewMapCount { get; set; }
    public int AddedMaps { get; set; }
    public int UpdatedMaps { get; set; }
    public int UnchangedMaps { get; set; }
    public int OldOnlyMaps { get; set; }
    public int PreservedDescriptions { get; set; }
    public int FallbackDescriptions { get; set; }
    public int PreservedResurrectionMaps { get; set; }
    public int FallbackResurrectionMaps { get; set; }
    public List<string> SourceFiles { get; } = [];
    public List<string> PatchedFiles { get; } = [];
    public List<string> Warnings { get; } = [];
    public List<string> Errors { get; } = [];

    public string ToDetailedText()
    {
        var sb = new StringBuilder();
        sb.AppendLine(Success ? "SUCCESS" : "FAILED");
        sb.AppendLine();
        sb.AppendLine("MapList conversion");
        sb.AppendLine($"  Output: {OutputDirectory}");
        sb.AppendLine($"  MapList output: {MapListOutputPath}");
        sb.AppendLine($"  Diff CSV: {DiffCsvPath}");
        sb.AppendLine($"  Old LKDMO maps: {OldMapCount:N0}");
        sb.AppendLine($"  GDMO maps: {GdmoMapCount:N0}");
        sb.AppendLine($"  New LKDMO maps: {NewMapCount:N0}");
        sb.AppendLine($"  Added/updated/unchanged: {AddedMaps:N0} / {UpdatedMaps:N0} / {UnchangedMaps:N0}");
        sb.AppendLine($"  Old-only maps not emitted: {OldOnlyMaps:N0}");
        sb.AppendLine();
        sb.AppendLine("Compatibility");
        sb.AppendLine($"  Preserved descriptions: {PreservedDescriptions:N0}");
        sb.AppendLine($"  Fallback descriptions: {FallbackDescriptions:N0}");
        sb.AppendLine($"  Preserved resurrection map IDs: {PreservedResurrectionMaps:N0}");
        sb.AppendLine($"  Fallback resurrection map IDs: {FallbackResurrectionMaps:N0}");
        sb.AppendLine();
        sb.AppendLine("Sources");
        foreach (var file in SourceFiles) sb.AppendLine($"  {file}");
        sb.AppendLine();
        sb.AppendLine("Staged or patched files");
        foreach (var file in PatchedFiles) sb.AppendLine($"  {file}");
        if (!string.IsNullOrWhiteSpace(PackBackupPf)) sb.AppendLine($"  Backup PF: {PackBackupPf}");
        if (!string.IsNullOrWhiteSpace(PackBackupHf)) sb.AppendLine($"  Backup HF: {PackBackupHf}");
        sb.AppendLine();

        if (Warnings.Count > 0)
        {
            sb.AppendLine("Warnings");
            foreach (var warning in Warnings) sb.AppendLine($"  - {warning}");
            sb.AppendLine();
        }

        if (Errors.Count > 0)
        {
            sb.AppendLine("Errors");
            foreach (var error in Errors) sb.AppendLine($"  - {error}");
            sb.AppendLine();
        }

        return sb.ToString();
    }
}
