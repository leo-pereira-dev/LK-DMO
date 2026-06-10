using System.Text;

namespace LkDmoBinConverter.Models;

public sealed class CraftConversionReport
{
    public bool Success { get; set; }
    public string OutputDirectory { get; set; } = string.Empty;
    public string ItemListOutputPath { get; set; } = string.Empty;
    public string? PackBackupPf { get; set; }
    public string? PackBackupHf { get; set; }
    public int PrefixOffset { get; set; }
    public long OldItemListSize { get; set; }
    public long NewItemListSize { get; set; }
    public int OldCraftNpcs { get; set; }
    public int NewCraftNpcs { get; set; }
    public int OldCraftProducts { get; set; }
    public int NewCraftProducts { get; set; }
    public int NewMainCategories { get; set; }
    public int NewSubCategories { get; set; }
    public int NewMaterials { get; set; }
    public int OldAssistGroups { get; set; }
    public int NewAssistGroups { get; set; }
    public int NewAssistItems { get; set; }
    public int LimitGroupRecords { get; set; }
    public int LimitGroupRecordSize { get; set; }
    public int ProductsWithLimitGroup { get; set; }
    public int MissingCategoryNames { get; set; }
    public int LegacyBelongingFixes { get; set; }
    public bool EmbeddedItemListPatchEnabled { get; set; }
    public List<string> SourceFiles { get; } = [];
    public List<string> PatchedFiles { get; } = [];
    public List<string> Warnings { get; } = [];
    public List<string> Errors { get; } = [];
    public Dictionary<int, int> LegacyBelongingFixValues { get; } = [];

    public string ToDetailedText()
    {
        var sb = new StringBuilder();
        sb.AppendLine(Success ? "SUCCESS" : "FAILED");
        sb.AppendLine();
        sb.AppendLine("Summary");
        sb.AppendLine($"  Output: {OutputDirectory}");
        sb.AppendLine($"  ItemList output: {ItemListOutputPath}");
        sb.AppendLine($"  Prefix offset: {PrefixOffset:N0}");
        sb.AppendLine($"  Old ItemList size: {OldItemListSize:N0}");
        sb.AppendLine($"  New ItemList size: {NewItemListSize:N0}");
        sb.AppendLine($"  Embedded ItemList patch: {(EmbeddedItemListPatchEnabled ? "enabled" : "disabled/safe")}");
        sb.AppendLine();
        sb.AppendLine("Craft");
        sb.AppendLine($"  Old NPCs/products: {OldCraftNpcs:N0} / {OldCraftProducts:N0}");
        sb.AppendLine($"  New NPCs/products: {NewCraftNpcs:N0} / {NewCraftProducts:N0}");
        sb.AppendLine($"  New categories: {NewMainCategories:N0} main / {NewSubCategories:N0} sub");
        sb.AppendLine($"  New materials: {NewMaterials:N0}");
        sb.AppendLine();
        sb.AppendLine("Assist and limits");
        sb.AppendLine($"  Old assist groups: {OldAssistGroups:N0}");
        sb.AppendLine($"  New assist groups/items: {NewAssistGroups:N0} / {NewAssistItems:N0}");
        sb.AppendLine($"  LimitGroup records/record size: {LimitGroupRecords:N0} / {LimitGroupRecordSize:N0}");
        sb.AppendLine($"  Products referencing LimitGroup: {ProductsWithLimitGroup:N0}");
        sb.AppendLine($"  Missing category names: {MissingCategoryNames:N0}");
        sb.AppendLine($"  Legacy belonging/bound fixes: {LegacyBelongingFixes:N0}");
        if (LegacyBelongingFixValues.Count > 0)
        {
            var values = string.Join(", ", LegacyBelongingFixValues.OrderBy(x => x.Key).Select(x => $"{x.Key}:{x.Value}"));
            sb.AppendLine($"  Original invalid values: {values}");
        }
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
