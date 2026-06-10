using System.Text;

namespace LkDmoBinConverter.Models;

public sealed class MonsterConversionReport
{
    public bool Success { get; set; }
    public string OutputDirectory { get; set; } = string.Empty;
    public string MonsterOutputPath { get; set; } = string.Empty;
    public string MapMonsterOutputPath { get; set; } = string.Empty;
    public string? PackBackupPf { get; set; }
    public string? PackBackupHf { get; set; }
    public int OldMonsterCount { get; set; }
    public int OldHitCount { get; set; }
    public int GdmoMonsterCount { get; set; }
    public int GdmoSkillCount { get; set; }
    public int GdmoTermsCount { get; set; }
    public int GdmoMapCount { get; set; }
    public int GdmoMapSpawnCount { get; set; }
    public int OutputMonsterCount { get; set; }
    public int OutputSkillCount { get; set; }
    public int OutputTermsCount { get; set; }
    public int OutputMapMonsterFileTableCount { get; set; }
    public int OutputMapSpawnCount { get; set; }
    public int OldOnlyMonsterCount { get; set; }
    public int NameFallbackCount { get; set; }
    public int NameTruncatedCount { get; set; }
    public int ExpClampedCount { get; set; }
    public int IntClampedCount { get; set; }
    public int SkillFactorTruncatedCount { get; set; }
    public int InvalidMapMonsterReferenceCount { get; set; }
    public List<string> SourceFiles { get; } = [];
    public List<string> PatchedFiles { get; } = [];
    public List<string> Warnings { get; } = [];
    public List<string> Errors { get; } = [];

    public string ToDetailedText()
    {
        var sb = new StringBuilder();
        sb.AppendLine(Success ? "SUCCESS" : "FAILED");
        sb.AppendLine();
        sb.AppendLine("Monster conversion");
        sb.AppendLine($"  Output: {OutputDirectory}");
        sb.AppendLine($"  Monster output: {MonsterOutputPath}");
        sb.AppendLine($"  MapMonster output: {MapMonsterOutputPath}");
        sb.AppendLine();
        sb.AppendLine("Monster.bin");
        sb.AppendLine($"  Old LKDMO monsters: {OldMonsterCount:N0}");
        sb.AppendLine($"  Old LKDMO hit rows preserved: {OldHitCount:N0}");
        sb.AppendLine($"  GDMO monsters: {GdmoMonsterCount:N0}");
        sb.AppendLine($"  GDMO skills: {GdmoSkillCount:N0}");
        sb.AppendLine($"  GDMO skill terms: {GdmoTermsCount:N0}");
        sb.AppendLine($"  Output monsters/skills/terms: {OutputMonsterCount:N0} / {OutputSkillCount:N0} / {OutputTermsCount:N0}");
        sb.AppendLine($"  Old-only monsters not emitted: {OldOnlyMonsterCount:N0}");
        sb.AppendLine();
        sb.AppendLine("MapMonsterList.bin");
        sb.AppendLine($"  GDMO maps with monsters: {GdmoMapCount:N0}");
        sb.AppendLine($"  GDMO spawn rows: {GdmoMapSpawnCount:N0}");
        sb.AppendLine($"  Output file-table groups: {OutputMapMonsterFileTableCount:N0}");
        sb.AppendLine($"  Output spawn rows: {OutputMapSpawnCount:N0}");
        sb.AppendLine($"  Invalid map monster references skipped: {InvalidMapMonsterReferenceCount:N0}");
        sb.AppendLine();
        sb.AppendLine("Compatibility adjustments");
        sb.AppendLine($"  Name fallbacks: {NameFallbackCount:N0}");
        sb.AppendLine($"  Name/title truncations: {NameTruncatedCount:N0}");
        sb.AppendLine($"  EXP values clamped to UInt16: {ExpClampedCount:N0}");
        sb.AppendLine($"  Int32 values clamped: {IntClampedCount:N0}");
        sb.AppendLine($"  Skill factor lists truncated to 3: {SkillFactorTruncatedCount:N0}");
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
