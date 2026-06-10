using System.IO;
using System.Text;
using System.Xml.Linq;
using LkDmoBinConverter.Models;

namespace LkDmoBinConverter.Services;

internal sealed class DungeonStringBinConverter
{
    private const string DungeonStringXml = "Dungeon_Str.xml";
    private const string MapDungeonRegionStringXml = "MapDungeonRegion_Str.xml";
    private const string DungeonStringRelativePath = @"data\bin\language\english\Dungeon_Str.bin";
    private const string MapDungeonRegionStringRelativePath = @"data\bin\language\english\MapDungeonRegion_Str.bin";

    public Task<(bool Success, string Report)> ConvertAsync(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        return Task.Run(() => Convert(options, applyToPack, cancellationToken), cancellationToken);
    }

    private static (bool Success, string Report) Convert(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        cancellationToken.ThrowIfCancellationRequested();

        Directory.CreateDirectory(options.OutputDirectory);

        var sourceDirectory = options.GdmoBinPath;
        var dungeonXml = Path.Combine(sourceDirectory, DungeonStringXml);
        var mapDungeonRegionXml = Path.Combine(sourceDirectory, MapDungeonRegionStringXml);
        var dungeonBin = Path.Combine(options.OutputDirectory, "Dungeon_Str.bin");
        var mapDungeonRegionBin = Path.Combine(options.OutputDirectory, "MapDungeonRegion_Str.bin");

        var warnings = new List<string>();
        var dungeonInfo = ConvertStringXml(dungeonXml, dungeonBin, warnings);
        var mapDungeonRegionInfo = ConvertStringXml(mapDungeonRegionXml, mapDungeonRegionBin, warnings);

        string? pfBackup = null;
        string? hfBackup = null;
        if (applyToPack)
        {
            var backups = Pack03Archive.PatchFiles(options.LkdmoInputPath, new Dictionary<string, string>
            {
                [DungeonStringRelativePath] = dungeonBin,
                [MapDungeonRegionStringRelativePath] = mapDungeonRegionBin
            });
            pfBackup = backups.pfBackup;
            hfBackup = backups.hfBackup;
        }

        var sb = new StringBuilder();
        sb.AppendLine("Dungeon string bin conversion");
        sb.AppendLine($"Source: {sourceDirectory}");
        sb.AppendLine($"Output: {options.OutputDirectory}");
        sb.AppendLine($"Apply to Pack03: {(applyToPack ? "yes" : "no")}");
        sb.AppendLine();
        AppendConvertedFile(sb, DungeonStringRelativePath, dungeonBin, dungeonInfo);
        AppendConvertedFile(sb, MapDungeonRegionStringRelativePath, mapDungeonRegionBin, mapDungeonRegionInfo);

        if (warnings.Count > 0)
        {
            sb.AppendLine("Warnings:");
            foreach (var warning in warnings)
                sb.AppendLine($"  {warning}");
            sb.AppendLine();
        }

        if (!string.IsNullOrWhiteSpace(pfBackup))
            sb.AppendLine($"Backup PF: {pfBackup}");
        if (!string.IsNullOrWhiteSpace(hfBackup))
            sb.AppendLine($"Backup HF: {hfBackup}");

        return (true, sb.ToString());
    }

    private static ConvertedStringBinInfo ConvertStringXml(string sourceXml, string outputBin, List<string> warnings)
    {
        if (!File.Exists(sourceXml))
            throw new FileNotFoundException($"Source XML not found: {sourceXml}");

        var document = XDocument.Load(sourceXml, LoadOptions.PreserveWhitespace);
        var root = document.Root ?? throw new InvalidDataException($"XML has no root: {sourceXml}");
        var entries = root.Element("Entries") ?? throw new InvalidDataException($"XML has no Entries node: {sourceXml}");
        var records = entries.Elements().Where(e => string.Equals((string?)e.Attribute("kind"), "Record", StringComparison.OrdinalIgnoreCase)).ToList();

        using var stream = File.Open(outputBin, FileMode.Create, FileAccess.Write, FileShare.Read);
        using var writer = new BinaryWriter(stream, Encoding.Unicode, leaveOpen: false);
        writer.Write(records.Count);

        foreach (var record in records)
        {
            var keyElement = record.Element("TextKey") ?? throw new InvalidDataException($"Record without TextKey in {sourceXml}");
            if (!uint.TryParse((string?)keyElement.Attribute("value"), out var key))
                throw new InvalidDataException($"Invalid TextKey value in {sourceXml}: {keyElement}");

            writer.Write(key);
            foreach (var textElement in record.Elements().Where(e => e.Name.LocalName.StartsWith("Text", StringComparison.OrdinalIgnoreCase) && e.Name.LocalName != "TextKey"))
                WriteWideString(writer, textElement.Value);
        }

        var expectedLengthAttr = (string?)root.Attribute("byteLength");
        if (int.TryParse(expectedLengthAttr, out var expectedLength) && expectedLength != stream.Length)
            warnings.Add($"{Path.GetFileName(sourceXml)} expected {expectedLength:N0} bytes from XML metadata, generated {stream.Length:N0} bytes.");

        return new ConvertedStringBinInfo(records.Count, stream.Length);
    }

    private static void WriteWideString(BinaryWriter writer, string text)
    {
        var bytes = Encoding.Unicode.GetBytes(text);
        writer.Write(bytes.Length);
        writer.Write(bytes);
    }

    private static void AppendConvertedFile(StringBuilder sb, string relativePath, string outputPath, ConvertedStringBinInfo info)
    {
        sb.AppendLine(relativePath);
        sb.AppendLine($"  Generated: {outputPath}");
        sb.AppendLine($"  Records: {info.RecordCount:N0}");
        sb.AppendLine($"  Size: {info.ByteLength:N0}");
        sb.AppendLine();
    }

    private readonly record struct ConvertedStringBinInfo(int RecordCount, long ByteLength);
}
