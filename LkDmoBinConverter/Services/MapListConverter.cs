using System.IO;
using System.Text;
using System.Xml.Linq;
using LkDmoBinConverter.Models;

namespace LkDmoBinConverter.Services;

public sealed class MapListConverter
{
    private const string MapListRelativePath = @"data\bin\english\MapList.bin";
    private const uint DefaultFallbackResurrectionMapId = 1;

    public Task<MapConversionReport> ConvertMapListAsync(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        return Task.Run(() => ConvertMapList(options, applyToPack, cancellationToken), cancellationToken);
    }

    private static MapConversionReport ConvertMapList(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        cancellationToken.ThrowIfCancellationRequested();
        Directory.CreateDirectory(options.OutputDirectory);

        var report = new MapConversionReport
        {
            OutputDirectory = options.OutputDirectory,
            Success = false
        };

        var source = ResolveSource(options.GdmoBinPath);
        report.SourceFiles.Add(source.Path);

        var oldBytes = LoadCurrentMapList(options.LkdmoInputPath);
        var oldMaps = ParseLegacyMapList(oldBytes).ToDictionary(x => x.MapId);
        var gdmoMaps = source.Kind == MapSourceKind.Xml
            ? ParseGdmoMapListXml(source.Path)
            : ParseGdmoMapListBin(source.Path);

        var converted = BuildLegacyMapList(gdmoMaps, oldMaps, report);
        var outputPath = Path.Combine(options.OutputDirectory, "MapList.bin");
        var stamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
        var diffCsvPath = Path.Combine(options.OutputDirectory, $"maplist_diff_{stamp}.csv");

        File.WriteAllBytes(outputPath, converted.Bytes);
        WriteCsv(diffCsvPath, converted.Rows);

        report.MapListOutputPath = outputPath;
        report.DiffCsvPath = diffCsvPath;
        report.OldMapCount = oldMaps.Count;
        report.GdmoMapCount = gdmoMaps.Count;
        report.NewMapCount = gdmoMaps.Count;
        report.PatchedFiles.Add(outputPath);
        report.PatchedFiles.Add(diffCsvPath);

        var oldOnlyCount = oldMaps.Keys.Count(id => !gdmoMaps.ContainsKey(id));
        report.OldOnlyMaps = oldOnlyCount;
        if (oldOnlyCount > 0)
            report.Warnings.Add($"{oldOnlyCount} old LKDMO maps are not present in the GDMO MapList source and were not emitted into the generated MapList.bin.");

        if (report.FallbackDescriptions > 0)
            report.Warnings.Add($"{report.FallbackDescriptions} maps had no legacy LKDMO description; their MapName was used as a safe fallback description.");

        if (report.FallbackResurrectionMaps > 0)
            report.Warnings.Add($"{report.FallbackResurrectionMaps} maps had no legacy LKDMO resurrection map ID; fallback map {DefaultFallbackResurrectionMapId} was used.");

        if (applyToPack)
        {
            var backups = Pack03Archive.PatchFiles(options.LkdmoInputPath, new Dictionary<string, string>
            {
                [MapListRelativePath] = outputPath
            });
            report.PackBackupPf = backups.pfBackup;
            report.PackBackupHf = backups.hfBackup;
        }

        report.Success = report.Errors.Count == 0;
        return report;
    }

    private static MapSource ResolveSource(string gdmoPath)
    {
        var root = gdmoPath.Trim().Trim('"');
        var candidates = new List<string>();

        if (File.Exists(root))
        {
            var fileName = Path.GetFileName(root);
            if (fileName.Equals("MapList.xml", StringComparison.OrdinalIgnoreCase))
                return new MapSource(root, MapSourceKind.Xml);
            if (fileName.Equals("MapList.bin", StringComparison.OrdinalIgnoreCase))
                return new MapSource(root, MapSourceKind.Bin);

            candidates.Add(Path.GetDirectoryName(root)!);
        }
        else
        {
            candidates.Add(root);
        }

        candidates.Add(Path.Combine(root, "Data"));
        candidates.Add(Path.Combine(root, "Table"));
        candidates.Add(Path.Combine(root, "..", "xml", "AllBins_full2", "Data"));
        candidates.Add(Path.Combine(root, "..", "xml", "AllBins_full", "Data"));
        candidates.Add(Path.Combine(root, "..", "xml", "AllBins", "Data"));
        candidates.Add(Path.Combine(root, "..", "..", "xml", "AllBins_full2", "Data"));
        candidates.Add(Path.Combine(root, "..", "..", "xml", "AllBins_full", "Data"));
        candidates.Add(Path.Combine(root, "..", "..", "xml", "AllBins", "Data"));

        foreach (var candidate in candidates.Select(Path.GetFullPath).Distinct(StringComparer.OrdinalIgnoreCase))
        {
            var xmlExport = Path.GetFullPath(Path.Combine(candidate, "..", "xml", "AllBins_full2", "Data", "MapList.xml"));
            if (File.Exists(xmlExport))
                return new MapSource(xmlExport, MapSourceKind.Xml);

            var xml = Path.Combine(candidate, "MapList.xml");
            if (File.Exists(xml))
                return new MapSource(xml, MapSourceKind.Xml);

            var bin = Path.Combine(candidate, "MapList.bin");
            if (File.Exists(bin))
                return new MapSource(bin, MapSourceKind.Bin);
        }

        throw new FileNotFoundException($"Could not find GDMO MapList.xml or MapList.bin from: {gdmoPath}.");
    }

    private static byte[] LoadCurrentMapList(string input)
    {
        var path = input.Trim().Trim('"');
        if (File.Exists(path) && Path.GetFileName(path).Equals("MapList.bin", StringComparison.OrdinalIgnoreCase))
            return File.ReadAllBytes(path);

        return Pack03Archive.ReadFile(path, MapListRelativePath);
    }

    private static IReadOnlyList<LegacyMapRecord> ParseLegacyMapList(byte[] bytes)
    {
        using var reader = NewReader(bytes);
        var count = reader.ReadInt32();
        if (count < 0 || count > 100000)
            throw new InvalidDataException($"Invalid LKDMO MapList count {count}.");

        var result = new List<LegacyMapRecord>(count);
        for (var i = 0; i < count; i++)
        {
            var record = new LegacyMapRecord
            {
                MapId = reader.ReadUInt32(),
                MapName = ReadFramedString(reader),
                MapPath = ReadFramedString(reader),
                BGSound = ReadFramedString(reader),
                Width = reader.ReadInt32(),
                Height = reader.ReadInt32(),
                Description = ReadFramedWString(reader),
                DescriptionEng = ReadFramedWString(reader),
                ResurrectionMapId = reader.ReadUInt32(),
                MapRegionId = reader.ReadUInt16(),
                FatigueType = reader.ReadUInt16(),
                FatigueDebuff = reader.ReadUInt16(),
                FatigueStartTime = reader.ReadUInt16(),
                FatigueAddTime = reader.ReadUInt16(),
                FatigueAddPoint = reader.ReadInt16(),
                CameraMaxLevel = reader.ReadInt16(),
                XgConsumeType = reader.ReadByte() != 0,
                BattleTagUse = reader.ReadByte() != 0
            };
            result.Add(record);
        }

        if (reader.BaseStream.Position != bytes.Length)
            throw new InvalidDataException($"LKDMO MapList parsed to {reader.BaseStream.Position}, length is {bytes.Length}.");

        return result;
    }

    private static Dictionary<uint, GdmoMapRecord> ParseGdmoMapListXml(string path)
    {
        var doc = XDocument.Load(path);
        var result = new Dictionary<uint, GdmoMapRecord>();

        foreach (var element in doc.Descendants())
        {
            if (DirectChild(element, "MapId") is null || DirectChild(element, "MapPath") is null)
                continue;

            var record = new GdmoMapRecord(
                RequiredUInt(element, "MapId"),
                RequiredInt(element, "Width"),
                RequiredInt(element, "Height"),
                RequiredUInt(element, "MapRegionId"),
                RequiredUShort(element, "FatigueType"),
                RequiredUShort(element, "FatigueDebuff"),
                RequiredUShort(element, "FatigueStartTime"),
                RequiredUShort(element, "FatigueAddTime"),
                RequiredShort(element, "FatigueAddPoint"),
                RequiredShort(element, "CameraMaxLevel"),
                RequiredBool(element, "XgConsumeType"),
                RequiredBool(element, "BattleTagUse"),
                RequiredText(element, "MapPath"),
                RequiredText(element, "MapName"),
                RequiredText(element, "BGSound"));

            if (!result.ContainsKey(record.MapId))
                result.Add(record.MapId, record);
        }

        return result;
    }

    private static Dictionary<uint, GdmoMapRecord> ParseGdmoMapListBin(string path)
    {
        using var reader = NewReader(File.ReadAllBytes(path));
        var count = reader.ReadInt32();
        if (count < 0 || count > 100000)
            throw new InvalidDataException($"Invalid GDMO MapList count {count}.");

        var result = new Dictionary<uint, GdmoMapRecord>(count);
        for (var i = 0; i < count; i++)
        {
            var record = new GdmoMapRecord(
                reader.ReadUInt32(),
                checked((int)reader.ReadUInt32()),
                checked((int)reader.ReadUInt32()),
                reader.ReadUInt32(),
                reader.ReadUInt16(),
                reader.ReadUInt16(),
                reader.ReadUInt16(),
                reader.ReadUInt16(),
                reader.ReadInt16(),
                reader.ReadInt16(),
                reader.ReadByte() != 0,
                reader.ReadByte() != 0,
                ReadFramedString(reader),
                ReadFramedString(reader),
                ReadFramedString(reader));

            if (!result.ContainsKey(record.MapId))
                result.Add(record.MapId, record);
        }

        if (reader.BaseStream.Position != reader.BaseStream.Length)
            throw new InvalidDataException($"GDMO MapList parsed to {reader.BaseStream.Position}, length is {reader.BaseStream.Length}.");

        return result;
    }

    private static MapBuildResult BuildLegacyMapList(
        IReadOnlyDictionary<uint, GdmoMapRecord> gdmoMaps,
        IReadOnlyDictionary<uint, LegacyMapRecord> oldMaps,
        MapConversionReport report)
    {
        using var output = new MemoryStream();
        using var writer = new BinaryWriter(output, Encoding.Unicode, leaveOpen: true);
        var rows = new List<MapDiffRow>();
        writer.Write(gdmoMaps.Count);

        foreach (var gdmo in gdmoMaps.Values.OrderBy(x => x.MapId))
        {
            oldMaps.TryGetValue(gdmo.MapId, out var old);
            var hasOld = old is not null;

            if (hasOld)
            {
                WriteLegacyMapRecord(writer, old!);
                report.PreservedDescriptions++;
                report.PreservedResurrectionMaps++;
                report.UnchangedMaps++;

                rows.Add(new MapDiffRow(
                    gdmo.MapId,
                    "ExistingPreserved",
                    old!.MapName,
                    gdmo.MapName,
                    old.MapPath,
                    gdmo.MapPath,
                    old.Width,
                    gdmo.Width,
                    old.Height,
                    gdmo.Height,
                    old.MapRegionId,
                    ToUShort(gdmo.MapRegionId),
                    old.ResurrectionMapId,
                    old.ResurrectionMapId));

                continue;
            }

            var description = gdmo.MapName;
            var descriptionEng = gdmo.MapName;
            var resurrectionMapId = DefaultFallbackResurrectionMapId;
            report.FallbackDescriptions++;
            report.FallbackResurrectionMaps++;
            report.AddedMaps++;

            writer.Write(gdmo.MapId);
            WriteFramedString(writer, gdmo.MapName);
            WriteFramedString(writer, gdmo.MapPath);
            WriteFramedString(writer, gdmo.BGSound);
            writer.Write(gdmo.Width);
            writer.Write(gdmo.Height);
            WriteFramedWString(writer, description);
            WriteFramedWString(writer, descriptionEng);
            writer.Write(resurrectionMapId);
            writer.Write(ToUShort(gdmo.MapRegionId));
            writer.Write(gdmo.FatigueType);
            writer.Write(gdmo.FatigueDebuff);
            writer.Write(gdmo.FatigueStartTime);
            writer.Write(gdmo.FatigueAddTime);
            writer.Write(gdmo.FatigueAddPoint);
            writer.Write(gdmo.CameraMaxLevel);
            writer.Write(gdmo.XgConsumeType);
            writer.Write(gdmo.BattleTagUse);

            rows.Add(new MapDiffRow(
                gdmo.MapId,
                "AddedFromGdmo",
                string.Empty,
                gdmo.MapName,
                string.Empty,
                gdmo.MapPath,
                null,
                gdmo.Width,
                null,
                gdmo.Height,
                null,
                ToUShort(gdmo.MapRegionId),
                null,
                resurrectionMapId));
        }

        foreach (var oldOnly in oldMaps.Values.Where(x => !gdmoMaps.ContainsKey(x.MapId)).OrderBy(x => x.MapId))
        {
            rows.Add(new MapDiffRow(
                oldOnly.MapId,
                "OldOnlyNotEmitted",
                oldOnly.MapName,
                string.Empty,
                oldOnly.MapPath,
                string.Empty,
                oldOnly.Width,
                null,
                oldOnly.Height,
                null,
                oldOnly.MapRegionId,
                null,
                oldOnly.ResurrectionMapId,
                null));
        }

        writer.Flush();
        return new MapBuildResult(output.ToArray(), rows);
    }

    private static void WriteLegacyMapRecord(BinaryWriter writer, LegacyMapRecord record)
    {
        writer.Write(record.MapId);
        WriteFramedString(writer, record.MapName);
        WriteFramedString(writer, record.MapPath);
        WriteFramedString(writer, record.BGSound);
        writer.Write(record.Width);
        writer.Write(record.Height);
        WriteFramedWString(writer, record.Description);
        WriteFramedWString(writer, record.DescriptionEng);
        writer.Write(record.ResurrectionMapId);
        writer.Write(record.MapRegionId);
        writer.Write(record.FatigueType);
        writer.Write(record.FatigueDebuff);
        writer.Write(record.FatigueStartTime);
        writer.Write(record.FatigueAddTime);
        writer.Write(record.FatigueAddPoint);
        writer.Write(record.CameraMaxLevel);
        writer.Write(record.XgConsumeType);
        writer.Write(record.BattleTagUse);
    }

    private static XElement? DirectChild(XElement element, string localName)
    {
        return element.Elements().FirstOrDefault(e => e.Name.LocalName == localName);
    }

    private static int RequiredInt(XElement element, string localName)
    {
        var text = DirectChild(element, localName)?.Attribute("value")?.Value;
        if (int.TryParse(text, out var value))
            return value;

        throw new InvalidDataException($"Missing or invalid {localName} in MapList XML record.");
    }

    private static uint RequiredUInt(XElement element, string localName)
    {
        var value = RequiredInt(element, localName);
        if (value < 0)
            throw new InvalidDataException($"{localName} cannot be negative.");

        return unchecked((uint)value);
    }

    private static ushort RequiredUShort(XElement element, string localName) => ToUShort(RequiredUInt(element, localName));

    private static short RequiredShort(XElement element, string localName)
    {
        var value = RequiredInt(element, localName);
        if (value < short.MinValue || value > short.MaxValue)
            throw new InvalidDataException($"{localName} value {value} is outside Int16 range.");

        return (short)value;
    }

    private static bool RequiredBool(XElement element, string localName) => RequiredInt(element, localName) != 0;

    private static string RequiredText(XElement element, string localName)
    {
        return DirectChild(element, localName)?.Value ?? string.Empty;
    }

    private static ushort ToUShort(uint value)
    {
        if (value > ushort.MaxValue)
            throw new InvalidDataException($"Value {value} is outside UInt16 range required by the LKDMO MapList template.");

        return (ushort)value;
    }

    private static BinaryReader NewReader(byte[] bytes) => new(new MemoryStream(bytes, writable: false), Encoding.Unicode);

    private static string ReadFramedString(BinaryReader reader)
    {
        var length = reader.ReadInt32();
        if (length < 0 || length > 4096)
            throw new InvalidDataException($"Invalid string byte length {length} at {reader.BaseStream.Position - 4}.");

        var bytes = reader.ReadBytes(length);
        if (bytes.Length != length)
            throw new EndOfStreamException("Unexpected EOF while reading framed string.");

        return Encoding.UTF8.GetString(bytes);
    }

    private static string ReadFramedWString(BinaryReader reader)
    {
        var length = reader.ReadInt32();
        if (length < 0 || length > 4096)
            throw new InvalidDataException($"Invalid UTF-16 char length {length} at {reader.BaseStream.Position - 4}.");

        var bytes = reader.ReadBytes(length * 2);
        if (bytes.Length != length * 2)
            throw new EndOfStreamException("Unexpected EOF while reading framed UTF-16 string.");

        return Encoding.Unicode.GetString(bytes);
    }

    private static void WriteFramedString(BinaryWriter writer, string value)
    {
        value ??= string.Empty;
        var bytes = Encoding.UTF8.GetBytes(value);
        writer.Write(bytes.Length);
        writer.Write(bytes);
    }

    private static void WriteFramedWString(BinaryWriter writer, string value)
    {
        value ??= string.Empty;
        writer.Write(value.Length);
        if (value.Length > 0)
            writer.Write(Encoding.Unicode.GetBytes(value));
    }

    private static void WriteCsv(string path, IReadOnlyList<MapDiffRow> rows)
    {
        using var writer = new StreamWriter(path, false, new UTF8Encoding(encoderShouldEmitUTF8Identifier: true));
        writer.WriteLine(string.Join(',', MapDiffRow.CsvHeader.Select(EscapeCsv)));
        foreach (var row in rows)
            writer.WriteLine(string.Join(',', row.ToCsvFields().Select(EscapeCsv)));
    }

    private static string EscapeCsv(string? value)
    {
        value ??= string.Empty;
        return value.Contains('"') || value.Contains(',') || value.Contains('\n') || value.Contains('\r')
            ? $"\"{value.Replace("\"", "\"\"")}\""
            : value;
    }

    private enum MapSourceKind
    {
        Xml,
        Bin
    }

    private readonly record struct MapSource(string Path, MapSourceKind Kind);

    private sealed class LegacyMapRecord
    {
        public uint MapId { get; init; }
        public string MapName { get; init; } = string.Empty;
        public string MapPath { get; init; } = string.Empty;
        public string BGSound { get; init; } = string.Empty;
        public int Width { get; init; }
        public int Height { get; init; }
        public string Description { get; init; } = string.Empty;
        public string DescriptionEng { get; init; } = string.Empty;
        public uint ResurrectionMapId { get; init; }
        public ushort MapRegionId { get; init; }
        public ushort FatigueType { get; init; }
        public ushort FatigueDebuff { get; init; }
        public ushort FatigueStartTime { get; init; }
        public ushort FatigueAddTime { get; init; }
        public short FatigueAddPoint { get; init; }
        public short CameraMaxLevel { get; init; }
        public bool XgConsumeType { get; init; }
        public bool BattleTagUse { get; init; }
    }

    private readonly record struct GdmoMapRecord(
        uint MapId,
        int Width,
        int Height,
        uint MapRegionId,
        ushort FatigueType,
        ushort FatigueDebuff,
        ushort FatigueStartTime,
        ushort FatigueAddTime,
        short FatigueAddPoint,
        short CameraMaxLevel,
        bool XgConsumeType,
        bool BattleTagUse,
        string MapPath,
        string MapName,
        string BGSound);

    private readonly record struct MapBuildResult(byte[] Bytes, IReadOnlyList<MapDiffRow> Rows);

    private readonly record struct MapDiffRow(
        uint MapId,
        string Status,
        string OldName,
        string NewName,
        string OldPath,
        string NewPath,
        int? OldWidth,
        int? NewWidth,
        int? OldHeight,
        int? NewHeight,
        ushort? OldRegion,
        ushort? NewRegion,
        uint? OldResurrectionMapId,
        uint? NewResurrectionMapId)
    {
        public static string[] CsvHeader { get; } =
        [
            "MapId", "Status", "OldName", "NewName", "OldPath", "NewPath", "OldWidth", "NewWidth",
            "OldHeight", "NewHeight", "OldRegion", "NewRegion", "OldResurrectionMapId", "NewResurrectionMapId"
        ];

        public string[] ToCsvFields() =>
        [
            MapId.ToString(), Status, OldName, NewName, OldPath, NewPath, OldWidth?.ToString() ?? string.Empty,
            NewWidth?.ToString() ?? string.Empty, OldHeight?.ToString() ?? string.Empty, NewHeight?.ToString() ?? string.Empty,
            OldRegion?.ToString() ?? string.Empty, NewRegion?.ToString() ?? string.Empty,
            OldResurrectionMapId?.ToString() ?? string.Empty, NewResurrectionMapId?.ToString() ?? string.Empty
        ];
    }
}
