using System.IO;
using System.Security.Cryptography;
using System.Text;
using System.Windows;
using LkDmoBinConverter.Models;
using LkDmoBinConverter.Services;

namespace LkDmoBinConverter;

public partial class App : System.Windows.Application
{
    protected override async void OnStartup(StartupEventArgs e)
    {
        if (e.Args.Any(a => string.Equals(a, "--self-test", StringComparison.OrdinalIgnoreCase)))
        {
            await RunSelfTestAsync(e.Args);
            return;
        }

        base.OnStartup(e);
        new MainWindow().Show();
    }

    private async Task RunSelfTestAsync(string[] args)
    {
        var output = GetArg(args, "--output") ?? @"C:\0-NewDmo\build-temp\LkDmoBinConverter-selftest";
        try
        {
            var root = GetArg(args, "--root") ?? @"C:\0-NewDmo";
            var dungeonStringMode = args.Any(a => string.Equals(a, "--dungeon-strings", StringComparison.OrdinalIgnoreCase));
            var dungeonBinReportMode = args.Any(a => string.Equals(a, "--dungeon-bin-report", StringComparison.OrdinalIgnoreCase));
            var defaultGdmoPath = dungeonStringMode
                ? Path.Combine(root, "TamerBinMigrationFeature", "source_xml_language", "_str")
                : Path.Combine(root, "Dcommerce", "Bin");

            var options = new CraftConversionOptions
            {
                RootPath = root,
                GdmoBinPath = GetArg(args, "--gdmo") ?? defaultGdmoPath,
                LkdmoInputPath = GetArg(args, "--lkdmo") ?? Path.Combine(root, "ClientDist1", "Data", "Pack03"),
                ServerBinPath = GetArg(args, "--server-bin") ?? Path.Combine(root, "dmo-server-main-postgres", "Bins", "data", "bin", "english"),
                OutputDirectory = output,
                PatchEmbeddedItemListCraft = args.Any(a => string.Equals(a, "--patch-itemlist-craft", StringComparison.OrdinalIgnoreCase))
            };

            var npcMode = args.Any(a => string.Equals(a, "--npc-type25", StringComparison.OrdinalIgnoreCase));
            var mapMode = args.Any(a => string.Equals(a, "--map-list", StringComparison.OrdinalIgnoreCase));
            var monsterMode = args.Any(a => string.Equals(a, "--monster", StringComparison.OrdinalIgnoreCase));
            var applyToPack = args.Any(a => string.Equals(a, "--apply", StringComparison.OrdinalIgnoreCase));
            string reportText;
            bool success;
            if (dungeonBinReportMode)
            {
                var converter = new DungeonBinReportConverter();
                reportText = await converter.BuildReportAsync(options, CancellationToken.None);
                success = true;
            }
            else if (monsterMode)
            {
                var converter = new MonsterConverter();
                var report = await converter.ConvertMonstersAsync(options, applyToPack, CancellationToken.None);
                reportText = report.ToDetailedText();
                success = report.Success;
            }
            else if (mapMode)
            {
                var converter = new MapListConverter();
                var report = await converter.ConvertMapListAsync(options, applyToPack, CancellationToken.None);
                reportText = report.ToDetailedText();
                success = report.Success;
            }
            else if (npcMode)
            {
                var converter = new NpcType25Converter();
                var report = await converter.ConvertType25Async(options, applyToPack, CancellationToken.None);
                reportText = report.ToDetailedText();
                success = report.Success;
            }
            else if (dungeonStringMode)
            {
                var converter = new DungeonStringBinConverter();
                var report = await converter.ConvertAsync(options, applyToPack, CancellationToken.None);
                reportText = report.Report;
                success = report.Success;
            }
            else
            {
                var converter = new ItemListCraftConverter();
                var report = await converter.ConvertCraftAsync(options, applyToPack, CancellationToken.None);
                reportText = report.ToDetailedText();
                success = report.Success;
            }

            Directory.CreateDirectory(output);
            await File.WriteAllTextAsync(Path.Combine(output, "self-test-report.txt"), reportText);
            await File.WriteAllTextAsync(Path.Combine(output, "bin-compare-report.txt"), BuildPackCompareReport(options, npcMode, mapMode, monsterMode, dungeonStringMode, dungeonBinReportMode));
            Shutdown(success ? 0 : 2);
        }
        catch (Exception ex)
        {
            Directory.CreateDirectory(output);
            await File.WriteAllTextAsync(Path.Combine(output, "self-test-report.txt"), ex.ToString());
            Shutdown(1);
        }
    }

    private static string? GetArg(string[] args, string name)
    {
        for (var i = 0; i < args.Length - 1; i++)
        {
            if (string.Equals(args[i], name, StringComparison.OrdinalIgnoreCase))
                return args[i + 1];
        }

        return null;
    }

    private static string BuildPackCompareReport(CraftConversionOptions options, bool npcMode, bool mapMode, bool monsterMode, bool dungeonStringMode, bool dungeonBinReportMode)
    {
        if (dungeonBinReportMode)
        {
            return "Dungeon bin report mode does not stage replacement pack files; see dungeon_bin_npc_type25.csv and dungeon_bin_portals.csv.\r\n";
        }

        var pairs = monsterMode
            ? new[]
            {
                new PackComparePair(@"data\bin\english\Monster.bin", Path.Combine(options.OutputDirectory, "Monster.bin")),
                new PackComparePair(@"data\bin\english\MapMonsterList.bin", Path.Combine(options.OutputDirectory, "MapMonsterList.bin"))
            }
            : dungeonStringMode
            ? new[]
            {
                new PackComparePair(@"data\bin\language\english\Dungeon_Str.bin", Path.Combine(options.OutputDirectory, "Dungeon_Str.bin")),
                new PackComparePair(@"data\bin\language\english\MapDungeonRegion_Str.bin", Path.Combine(options.OutputDirectory, "MapDungeonRegion_Str.bin"))
            }
            : mapMode
            ? new[]
            {
                new PackComparePair(@"data\bin\english\MapList.bin", Path.Combine(options.OutputDirectory, "MapList.bin"))
            }
            : npcMode
            ? new[]
            {
                new PackComparePair(@"data\bin\english\Npc.bin", Path.Combine(options.OutputDirectory, "Npc.bin"))
            }
            : new[]
            {
                new PackComparePair(@"data\bin\english\ItemList.bin", Path.Combine(options.OutputDirectory, "ItemList.bin")),
                new PackComparePair(@"data\bin\table\ItemCreation.bin", Path.Combine(options.OutputDirectory, "ItemCreation.bin")),
                new PackComparePair(@"data\bin\table\ItemCreationGroup.bin", Path.Combine(options.OutputDirectory, "ItemCreationGroup.bin")),
                new PackComparePair(@"data\bin\table\ItemCreationLimitGroup.bin", Path.Combine(options.OutputDirectory, "ItemCreationLimitGroup.bin")),
                new PackComparePair(@"data\bin\english\ItemCreation_Str.bin", Path.Combine(options.OutputDirectory, "ItemCreation_Str.bin"))
            };

        var sb = new StringBuilder();
        sb.AppendLine(monsterMode ? "Monster Pack03 comparison" : dungeonStringMode ? "Dungeon string Pack03 comparison" : mapMode ? "MapList Pack03 comparison" : npcMode ? "NPC Type 25 Pack03 comparison" : "Craft Pack03 comparison");
        sb.AppendLine($"Pack input: {options.LkdmoInputPath}");
        sb.AppendLine($"Output: {options.OutputDirectory}");
        sb.AppendLine();

        foreach (var pair in pairs)
        {
            sb.AppendLine(pair.RelativePath);
            if (!File.Exists(pair.GeneratedPath))
            {
                sb.AppendLine($"  Generated file missing: {pair.GeneratedPath}");
                sb.AppendLine();
                continue;
            }

            try
            {
                var generated = File.ReadAllBytes(pair.GeneratedPath);
                var packed = Pack03Archive.ReadFile(options.LkdmoInputPath, pair.RelativePath);
                var identical = generated.SequenceEqual(packed);
                sb.AppendLine($"  Generated: {pair.GeneratedPath}");
                sb.AppendLine($"  Generated size: {generated.Length:N0}");
                sb.AppendLine($"  Pack size: {packed.Length:N0}");
                sb.AppendLine($"  Generated SHA256: {Sha256(generated)}");
                sb.AppendLine($"  Pack SHA256:      {Sha256(packed)}");
                sb.AppendLine($"  Byte identical: {identical}");
                if (!identical)
                    sb.AppendLine($"  First difference offset: {FirstDifferenceOffset(generated, packed):N0}");
            }
            catch (Exception ex)
            {
                sb.AppendLine($"  Pack comparison skipped: {ex.Message}");
            }

            sb.AppendLine();
        }

        return sb.ToString();
    }

    private static string Sha256(byte[] bytes) => Convert.ToHexString(SHA256.HashData(bytes)).ToLowerInvariant();

    private static int FirstDifferenceOffset(byte[] left, byte[] right)
    {
        var count = Math.Min(left.Length, right.Length);
        for (var i = 0; i < count; i++)
        {
            if (left[i] != right[i])
                return i;
        }

        return count;
    }

    private readonly record struct PackComparePair(string RelativePath, string GeneratedPath);
}
