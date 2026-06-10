using System.IO;
using System.Text;
using System.Xml.Linq;
using LkDmoBinConverter.Models;

namespace LkDmoBinConverter.Services;

public sealed class NpcType25Converter
{
    private const string NpcRelativePath = @"data\bin\english\Npc.bin";
    private const int Type25 = 25;
    private const int InfoSize = 1176;
    private const int NpcIdOffset = 0;
    private const int MapIdOffset = 4;
    private const int TypeOffset = 8;
    private static readonly HashSet<int> ItemPayloadTypes = [1, 8, 9, 12, 14, 16, 25];

    public Task<NpcConversionReport> ConvertType25Async(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        return Task.Run(() => ConvertType25(options, applyToPack, cancellationToken), cancellationToken);
    }

    private static NpcConversionReport ConvertType25(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        cancellationToken.ThrowIfCancellationRequested();
        Directory.CreateDirectory(options.OutputDirectory);

        var report = new NpcConversionReport
        {
            OutputDirectory = options.OutputDirectory,
            Success = false
        };

        var sources = ResolveSources(options.GdmoBinPath);
        report.SourceFiles.Add(sources.NpcInfoXml);
        if (sources.MapNpcXml is not null) report.SourceFiles.Add(sources.MapNpcXml);
        if (sources.MapPortalXml is not null) report.SourceFiles.Add(sources.MapPortalXml);

        var gdmoNpcs = ParseGdmoNpcInfo(sources.NpcInfoXml);
        IReadOnlySet<uint> mapNpcIds = sources.MapNpcXml is null ? new HashSet<uint>() : ParseMapNpcIds(sources.MapNpcXml);
        IReadOnlyDictionary<uint, List<GdmoMapPortalRecord>> mapPortalByNpc = sources.MapPortalXml is null ? new Dictionary<uint, List<GdmoMapPortalRecord>>() : ParseNpcMapPortals(sources.MapPortalXml);
        var npcBin = LoadCurrentNpcBin(options.LkdmoInputPath);
        var parsed = ParseLegacyNpcBin(npcBin);
        var conversion = RebuildNpcBin(parsed, gdmoNpcs, mapNpcIds, mapPortalByNpc);

        var stamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
        var npcOutputPath = Path.Combine(options.OutputDirectory, "Npc.bin");
        var changedCsvPath = Path.Combine(options.OutputDirectory, $"npc_type25_changed_{stamp}.csv");
        var fullDiffCsvPath = Path.Combine(options.OutputDirectory, $"npc_type_diff_full_{stamp}.csv");
        var missingCsvPath = Path.Combine(options.OutputDirectory, $"gdmo_type25_missing_in_lkdmo_{stamp}.csv");

        File.WriteAllBytes(npcOutputPath, conversion.Bytes);
        WriteCsv(changedCsvPath, conversion.ChangedRows, NpcPatchRow.CsvHeader);
        WriteCsv(fullDiffCsvPath, conversion.FullDiffRows, NpcDiffRow.CsvHeader);
        WriteCsv(missingCsvPath, conversion.MissingRows, NpcMissingRow.CsvHeader);

        report.NpcBinOutputPath = npcOutputPath;
        report.ChangedCsvPath = changedCsvPath;
        report.FullDiffCsvPath = fullDiffCsvPath;
        report.MissingCsvPath = missingCsvPath;
        report.ActiveNpcCount = parsed.Count;
        report.GdmoNpcCount = gdmoNpcs.Count;
        report.GdmoType25Count = gdmoNpcs.Values.Count(n => n.Type == Type25);
        report.MatchedType25Count = conversion.MatchedType25Count;
        report.MissingType25Count = conversion.MissingRows.Count;
        report.ChangedNpcCount = conversion.ChangedRows.Count(r => r.Status == "ChangedToType25");
        report.AlreadyType25Count = conversion.ChangedRows.Count(r => r.Status == "AlreadyType25Safe");
        report.MatchedType25WithMapNpc = conversion.ChangedRows.Count(r => r.HasMapNpcPlacement);
        report.MatchedType25WithNpcPortal = conversion.ChangedRows.Count(r => r.PortalIds.Length > 0);
        report.PatchedFiles.Add(npcOutputPath);
        report.PatchedFiles.Add(changedCsvPath);
        report.PatchedFiles.Add(fullDiffCsvPath);
        report.PatchedFiles.Add(missingCsvPath);

        if (sources.MapNpcXml is null)
            report.Warnings.Add("MapNpc.xml was not found. NPC placement cross-check was skipped.");
        if (sources.MapPortalXml is null)
            report.Warnings.Add("MapPortal.xml was not found. Dungeon/portal cross-check was skipped.");
        if (report.ChangedNpcCount == 0)
            report.Warnings.Add("No binary changes were required. The matched Type 25 NPCs already use the safe LKDMO payload.");

        if (applyToPack && report.ChangedNpcCount > 0)
        {
            var backups = Pack03Archive.PatchFiles(options.LkdmoInputPath, new Dictionary<string, string>
            {
                [NpcRelativePath] = npcOutputPath
            });
            report.PackBackupPf = backups.pfBackup;
            report.PackBackupHf = backups.hfBackup;
        }
        else if (applyToPack)
        {
            report.Warnings.Add("Apply was requested, but Pack03 was not patched because there are no pending Npc.bin changes.");
        }

        report.Success = report.Errors.Count == 0;
        return report;
    }

    private static GdmoXmlSources ResolveSources(string gdmoPath)
    {
        var root = gdmoPath.Trim().Trim('"');
        var candidates = new List<string>();

        if (File.Exists(root))
            candidates.Add(Path.GetDirectoryName(root)!);
        else
            candidates.Add(root);

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
            var npcInfo = Path.Combine(candidate, "NpcInfo.xml");
            if (!File.Exists(npcInfo))
                continue;

            return new GdmoXmlSources(
                npcInfo,
                OptionalFile(candidate, "MapNpc.xml"),
                OptionalFile(candidate, "MapPortal.xml"));
        }

        throw new FileNotFoundException($"Could not find GDMO NpcInfo.xml from: {gdmoPath}. Export GDMO templates with BinXmlTool first.");
    }

    private static string? OptionalFile(string folder, string fileName)
    {
        var file = Path.Combine(folder, fileName);
        return File.Exists(file) ? file : null;
    }

    private static byte[] LoadCurrentNpcBin(string input)
    {
        var path = input.Trim().Trim('"');
        if (File.Exists(path) && Path.GetFileName(path).Equals("Npc.bin", StringComparison.OrdinalIgnoreCase))
            return File.ReadAllBytes(path);

        return Pack03Archive.ReadFile(path, NpcRelativePath);
    }

    private static Dictionary<uint, GdmoNpcRecord> ParseGdmoNpcInfo(string path)
    {
        var doc = XDocument.Load(path);
        var result = new Dictionary<uint, GdmoNpcRecord>();

        foreach (var element in doc.Descendants())
        {
            if (DirectChild(element, "NpcKey") is null)
                continue;

            var npcKey = FirstUInt(element, "NpcKey");
            var baseElement = DirectChild(element, "Base") ?? element;
            var npcId = FirstUInt(baseElement, "NpcId") ?? npcKey;
            var type = FirstInt(baseElement, "Type");
            if (npcId is null || type is null)
                continue;

            var values = new List<int>();
            var valuesElement = DirectChild(element, "Values");
            if (valuesElement is not null)
            {
                foreach (var valueElement in valuesElement.Descendants())
                {
                    var value = ScalarInt(valueElement);
                    if (value is not null)
                        values.Add(value.Value);
                }
            }

            var record = new GdmoNpcRecord(
                npcId.Value,
                npcKey,
                type.Value,
                FirstUInt(baseElement, "MapId"),
                FirstUInt(baseElement, "ModelId"),
                FirstInt(element, "FieldG"),
                values);

            if (!result.ContainsKey(record.NpcId) || record.Type == Type25)
                result[record.NpcId] = record;
        }

        return result;
    }

    private static HashSet<uint> ParseMapNpcIds(string path)
    {
        var doc = XDocument.Load(path);
        var result = new HashSet<uint>();
        foreach (var element in doc.Descendants())
        {
            var npcId = FirstUInt(element, "NpcId");
            if (npcId is not null)
                result.Add(npcId.Value);
        }

        return result;
    }

    private static Dictionary<uint, List<GdmoMapPortalRecord>> ParseNpcMapPortals(string path)
    {
        var doc = XDocument.Load(path);
        var result = new Dictionary<uint, List<GdmoMapPortalRecord>>();
        foreach (var element in doc.Descendants())
        {
            var uniqueObjectId = FirstUInt(element, "UniqueObjectId");
            if (uniqueObjectId is null or 0)
                continue;

            var portalType = FirstUInt(element, "PortalType");
            if (portalType != 1)
                continue;

            var record = new GdmoMapPortalRecord(
                uniqueObjectId.Value,
                FirstUInt(element, "PortalId"),
                FirstUInt(element, "SrcMapId"),
                FirstUInt(element, "DestMapId"),
                FirstInt(element, "PortalKindIndex"));

            if (!result.TryGetValue(record.UniqueObjectId, out var list))
            {
                list = [];
                result[record.UniqueObjectId] = list;
            }

            list.Add(record);
        }

        foreach (var list in result.Values)
            list.Sort((a, b) => Nullable.Compare(a.PortalKindIndex, b.PortalKindIndex));

        return result;
    }

    private static XElement? DirectChild(XElement element, string localName)
    {
        return element.Elements().FirstOrDefault(e => e.Name.LocalName == localName);
    }

    private static int? FirstInt(XElement element, string localName)
    {
        foreach (var descendant in element.DescendantsAndSelf())
        {
            if (descendant.Name.LocalName == localName)
            {
                var value = ScalarInt(descendant);
                if (value is not null)
                    return value;
            }
        }

        return null;
    }

    private static uint? FirstUInt(XElement element, string localName)
    {
        var value = FirstInt(element, localName);
        if (value is null || value < 0)
            return null;

        return unchecked((uint)value.Value);
    }

    private static int? ScalarInt(XElement element)
    {
        var text = element.Attribute("value")?.Value ?? element.Value;
        if (int.TryParse(text, out var value))
            return value;

        return null;
    }

    private static LegacyNpcBin ParseLegacyNpcBin(byte[] bytes)
    {
        var count = ReadInt32(bytes, 0);
        if (count <= 0 || count > 100000)
            throw new InvalidDataException($"Invalid LKDMO Npc.bin count {count}.");

        var offset = 4;
        var records = new List<LegacyNpcRecord>(count);
        for (var i = 0; i < count; i++)
        {
            if (offset + InfoSize > bytes.Length)
                throw new EndOfStreamException($"Npc record {i} exceeds Npc.bin length.");

            var info = bytes.AsSpan(offset, InfoSize).ToArray();
            offset += InfoSize;
            var npcId = ReadUInt32(info, NpcIdOffset);
            var mapId = ReadUInt32(info, MapIdOffset);
            var type = ReadInt32(info, TypeOffset);
            var payloadStart = offset;

            if (ItemPayloadTypes.Contains(type))
            {
                var itemCount = ReadInt32(bytes, offset);
                offset += 4;
                if (itemCount < 0 || itemCount > 100000)
                    throw new InvalidDataException($"Invalid item payload count {itemCount} for NPC {npcId}.");

                offset += itemCount * 4;
            }
            else if (type == 3)
            {
                var portalCount = ReadInt32(bytes, offset + 4);
                offset += 8;
                if (portalCount < 0 || portalCount > 10000)
                    throw new InvalidDataException($"Invalid portal count {portalCount} for NPC {npcId}.");

                offset += portalCount * 40;
            }
            else if (type == 19)
            {
                var subType = ReadInt32(bytes, offset);
                offset += 4;
                if (subType == 2)
                {
                    var itemCount = ReadInt32(bytes, offset);
                    offset += 4;
                    if (itemCount < 0 || itemCount > 100000)
                        throw new InvalidDataException($"Invalid special event item payload count {itemCount} for NPC {npcId}.");

                    offset += itemCount * 4;
                }
            }

            if (offset > bytes.Length)
                throw new EndOfStreamException($"NPC {npcId} payload exceeds Npc.bin length.");

            var payload = bytes.AsSpan(payloadStart, offset - payloadStart).ToArray();
            var extraStart = offset;
            offset = SkipExtras(bytes, offset);
            var extras = bytes.AsSpan(extraStart, offset - extraStart).ToArray();
            records.Add(new LegacyNpcRecord(i, npcId, mapId, type, info, payload, extras));
        }

        var tail = bytes.AsSpan(offset).ToArray();
        return new LegacyNpcBin(count, records, tail);
    }

    private static int SkipExtras(byte[] bytes, int offset)
    {
        var extraCount = ReadInt32(bytes, offset);
        offset += 4;
        if (extraCount < 0 || extraCount > 64)
            throw new InvalidDataException($"Invalid NPC extra count {extraCount}.");

        for (var i = 0; i < extraCount; i++)
        {
            var extraType = ReadInt32(bytes, offset);
            offset += 4;
            if (extraType != 0)
                throw new InvalidDataException($"Unsupported NPC extra type {extraType}.");

            offset += 4;
            var actionCount = ReadInt32(bytes, offset);
            offset += 4;
            if (actionCount < 0 || actionCount > 2048)
                throw new InvalidDataException($"Invalid NPC extra action count {actionCount}.");

            for (var a = 0; a < actionCount; a++)
            {
                offset += 8;
                var questCount = ReadInt32(bytes, offset);
                offset += 4;
                if (questCount < 0 || questCount > 10000)
                    throw new InvalidDataException($"Invalid NPC extra quest count {questCount}.");

                offset += questCount * 4;
                if (offset > bytes.Length)
                    throw new EndOfStreamException("NPC extra data exceeds Npc.bin length.");
            }
        }

        return offset;
    }

    private static NpcRebuildResult RebuildNpcBin(
        LegacyNpcBin npcBin,
        IReadOnlyDictionary<uint, GdmoNpcRecord> gdmoNpcs,
        IReadOnlySet<uint> mapNpcIds,
        IReadOnlyDictionary<uint, List<GdmoMapPortalRecord>> mapPortalByNpc)
    {
        using var output = new MemoryStream();
        using var writer = new BinaryWriter(output, Encoding.Unicode, leaveOpen: true);
        writer.Write(npcBin.Count);

        var activeIds = npcBin.Records.Select(r => r.NpcId).ToHashSet();
        var gdmoType25Ids = gdmoNpcs.Values.Where(n => n.Type == Type25).Select(n => n.NpcId).ToHashSet();
        var changedRows = new List<NpcPatchRow>();
        var diffRows = new List<NpcDiffRow>();
        var matchedType25Count = 0;

        foreach (var record in npcBin.Records)
        {
            var hasGdmo = gdmoNpcs.TryGetValue(record.NpcId, out var gdmo);
            var isType25Match = gdmoType25Ids.Contains(record.NpcId);
            var info = record.Info.ToArray();
            var payload = record.Payload;
            var status = !hasGdmo ? "NoGdmoMatch" : record.Type == gdmo.Type ? "Unchanged" : "TypeDiffNotPatched";

            if (isType25Match && hasGdmo)
            {
                matchedType25Count++;
                WriteInt32(info, TypeOffset, Type25);
                var alreadySafe = record.Type == Type25 && record.Payload.Length == 4 && ReadInt32(record.Payload, 0) == 0;
                payload = BitConverter.GetBytes(0);
                status = alreadySafe ? "AlreadyType25Safe" : "ChangedToType25";

                mapPortalByNpc.TryGetValue(record.NpcId, out var portals);
                portals ??= [];
                changedRows.Add(new NpcPatchRow(
                    record.NpcId,
                    record.MapId,
                    record.Type,
                    Type25,
                    gdmo.MapId,
                    gdmo.FieldG,
                    string.Join(';', gdmo.Values),
                    mapNpcIds.Contains(record.NpcId),
                    string.Join(';', portals.Select(p => p.PortalId?.ToString() ?? string.Empty)),
                    string.Join(';', portals.Select(p => p.DestMapId?.ToString() ?? string.Empty)),
                    "ItemCount=0",
                    status));
            }

            diffRows.Add(new NpcDiffRow(
                record.NpcId,
                record.MapId,
                record.Type,
                hasGdmo ? gdmo.Type : null,
                isType25Match ? Type25 : record.Type,
                status));

            writer.Write(info);
            writer.Write(payload);
            writer.Write(record.Extras);
        }

        writer.Write(npcBin.Tail);
        writer.Flush();

        var missingRows = gdmoType25Ids
            .Where(id => !activeIds.Contains(id))
            .OrderBy(id => id)
            .Select(id => new NpcMissingRow(id, Type25))
            .ToList();

        return new NpcRebuildResult(output.ToArray(), matchedType25Count, changedRows, diffRows, missingRows);
    }

    private static void WriteCsv<T>(string path, IReadOnlyList<T> rows, string[] header) where T : ICsvRow
    {
        using var writer = new StreamWriter(path, false, new UTF8Encoding(encoderShouldEmitUTF8Identifier: true));
        writer.WriteLine(string.Join(',', header.Select(EscapeCsv)));
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

    private static int ReadInt32(byte[] bytes, int offset) => BitConverter.ToInt32(bytes, offset);
    private static uint ReadUInt32(byte[] bytes, int offset) => BitConverter.ToUInt32(bytes, offset);
    private static void WriteInt32(byte[] bytes, int offset, int value) => BitConverter.GetBytes(value).CopyTo(bytes, offset);

    private readonly record struct GdmoXmlSources(string NpcInfoXml, string? MapNpcXml, string? MapPortalXml);
    private readonly record struct GdmoNpcRecord(uint NpcId, uint? NpcKey, int Type, uint? MapId, uint? ModelId, int? FieldG, IReadOnlyList<int> Values);
    private readonly record struct GdmoMapPortalRecord(uint UniqueObjectId, uint? PortalId, uint? SrcMapId, uint? DestMapId, int? PortalKindIndex);
    private readonly record struct LegacyNpcBin(int Count, IReadOnlyList<LegacyNpcRecord> Records, byte[] Tail);
    private readonly record struct LegacyNpcRecord(int Index, uint NpcId, uint MapId, int Type, byte[] Info, byte[] Payload, byte[] Extras);
    private readonly record struct NpcRebuildResult(byte[] Bytes, int MatchedType25Count, IReadOnlyList<NpcPatchRow> ChangedRows, IReadOnlyList<NpcDiffRow> FullDiffRows, IReadOnlyList<NpcMissingRow> MissingRows);

    private interface ICsvRow
    {
        string[] ToCsvFields();
    }

    private readonly record struct NpcPatchRow(
        uint NpcId,
        uint MapId,
        int OldType,
        int NewType,
        uint? GdmoMapId,
        int? GdmoFieldG,
        string GdmoValues,
        bool HasMapNpcPlacement,
        string PortalIds,
        string DestMapIds,
        string PayloadAction,
        string Status) : ICsvRow
    {
        public static string[] CsvHeader { get; } =
        [
            "NpcId", "MapId", "OldType", "NewType", "GdmoMapId", "GdmoFieldG", "GdmoValues",
            "HasMapNpcPlacement", "PortalIds", "DestMapIds", "PayloadAction", "Status"
        ];

        public string[] ToCsvFields() =>
        [
            NpcId.ToString(), MapId.ToString(), OldType.ToString(), NewType.ToString(),
            GdmoMapId?.ToString() ?? string.Empty, GdmoFieldG?.ToString() ?? string.Empty, GdmoValues,
            HasMapNpcPlacement ? "true" : "false", PortalIds, DestMapIds, PayloadAction, Status
        ];
    }

    private readonly record struct NpcDiffRow(uint NpcId, uint MapId, int ActiveTypeBefore, int? GdmoType, int ActiveTypeAfter, string Status) : ICsvRow
    {
        public static string[] CsvHeader { get; } =
        [
            "NpcId", "MapId", "ActiveTypeBefore", "GdmoType", "ActiveTypeAfter", "Status"
        ];

        public string[] ToCsvFields() =>
        [
            NpcId.ToString(), MapId.ToString(), ActiveTypeBefore.ToString(), GdmoType?.ToString() ?? string.Empty,
            ActiveTypeAfter.ToString(), Status
        ];
    }

    private readonly record struct NpcMissingRow(uint NpcId, int GdmoType) : ICsvRow
    {
        public static string[] CsvHeader { get; } = ["NpcId", "GdmoType"];
        public string[] ToCsvFields() => [NpcId.ToString(), GdmoType.ToString()];
    }
}
