using System.Text;

namespace LkDmoBinConverter.Models;

public sealed class NpcConversionReport
{
    public bool Success { get; set; }
    public string OutputDirectory { get; set; } = string.Empty;
    public string NpcBinOutputPath { get; set; } = string.Empty;
    public string ChangedCsvPath { get; set; } = string.Empty;
    public string FullDiffCsvPath { get; set; } = string.Empty;
    public string MissingCsvPath { get; set; } = string.Empty;
    public string? PackBackupPf { get; set; }
    public string? PackBackupHf { get; set; }
    public int ActiveNpcCount { get; set; }
    public int GdmoNpcCount { get; set; }
    public int GdmoType25Count { get; set; }
    public int MatchedType25Count { get; set; }
    public int MissingType25Count { get; set; }
    public int ChangedNpcCount { get; set; }
    public int AlreadyType25Count { get; set; }
    public int MatchedType25WithMapNpc { get; set; }
    public int MatchedType25WithNpcPortal { get; set; }
    public List<string> SourceFiles { get; } = [];
    public List<string> PatchedFiles { get; } = [];
    public List<string> Warnings { get; } = [];
    public List<string> Errors { get; } = [];

    public string ToDetailedText()
    {
        var sb = new StringBuilder();
        sb.AppendLine(Success ? "SUCCESS" : "FAILED");
        sb.AppendLine();
        sb.AppendLine("NPC Type 25 conversion");
        sb.AppendLine($"  Output: {OutputDirectory}");
        sb.AppendLine($"  Npc.bin output: {NpcBinOutputPath}");
        sb.AppendLine($"  Active LKDMO NPC count: {ActiveNpcCount:N0}");
        sb.AppendLine($"  GDMO NPC records: {GdmoNpcCount:N0}");
        sb.AppendLine($"  GDMO Type 25 records: {GdmoType25Count:N0}");
        sb.AppendLine($"  Matched Type 25 in LKDMO Npc.bin: {MatchedType25Count:N0}");
        sb.AppendLine($"  Missing Type 25 in LKDMO Npc.bin: {MissingType25Count:N0}");
        sb.AppendLine($"  Changed to Type 25: {ChangedNpcCount:N0}");
        sb.AppendLine($"  Already safe Type 25: {AlreadyType25Count:N0}");
        sb.AppendLine($"  Matched with MapNpc placement: {MatchedType25WithMapNpc:N0}");
        sb.AppendLine($"  Matched with MapPortal NPC link: {MatchedType25WithNpcPortal:N0}");
        sb.AppendLine();
        sb.AppendLine("Generated reports");
        sb.AppendLine($"  Changed CSV: {ChangedCsvPath}");
        sb.AppendLine($"  Full diff CSV: {FullDiffCsvPath}");
        sb.AppendLine($"  Missing CSV: {MissingCsvPath}");
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
