using System.IO;
using System.Text;
using LkDmoBinConverter.Models;

namespace LkDmoBinConverter.Services;

public sealed class DungeonBinReportConverter
{
    private const string NpcRelativePath = @"data\bin\english\Npc.bin";
    private const int Type25 = 25;
    private const int DungeonPortalType = 1;
    private const int NpcPortalObjectType = 1;
    private const int NpcInfoSize = 1176;
    private const int NpcNameOffset = 88;
    private const int NpcNameBytes = 64;
    private const int NpcPortalRecordSize = 40;

    private static readonly string[] MapPortalRelativeCandidates =
    [
        @"data\bin\english\MapPortal.bin",
        @"data\bin\table\MapPortal.bin"
    ];

    private static readonly string[] MapListRelativeCandidates =
    [
        @"data\bin\english\MapList.bin",
        @"data\bin\table\MapList.bin"
    ];

    private static readonly HashSet<int> ItemPayloadTypes = [1, 8, 9, 12, 14, 16, 25];

    public Task<string> BuildReportAsync(CraftConversionOptions options, CancellationToken cancellationToken)
    {
        return Task.Run(() => BuildReport(options, cancellationToken), cancellationToken);
    }

    private static string BuildReport(CraftConversionOptions options, CancellationToken cancellationToken)
    {
        cancellationToken.ThrowIfCancellationRequested();
        Directory.CreateDirectory(options.OutputDirectory);

        var npcs = ParseNpcBin(Pack03Archive.ReadFile(options.LkdmoInputPath, NpcRelativePath));
        var type25Npcs = npcs.Where(x => x.Type == Type25).OrderBy(x => x.MapId).ThenBy(x => x.NpcId).ToList();

        var clientMapPortalPath = ResolvePackFile(options.LkdmoInputPath, MapPortalRelativeCandidates);
        var clientPortals = ParseMapPortal(clientMapPortalPath.Bytes);
        var clientDungeonNpcPortals = clientPortals
            .Where(IsDungeonNpcPortal)
            .OrderBy(x => x.SourceMapId)
            .ThenBy(x => x.UniqObjectId)
            .ThenBy(x => x.PortalKindIndex)
            .ThenBy(x => x.PortalId)
            .ToList();

        var clientMapNames = TryReadPackFile(options.LkdmoInputPath, MapListRelativeCandidates) is { } clientMapListBytes
            ? ParseMapList(clientMapListBytes)
            : new Dictionary<uint, string>();

        var serverPortals = TryReadServerMapPortal(options.ServerBinPath);
        var serverNpcAssets = TryReadServerNpcAssets(options.ServerBinPath);
        var serverMapNames = TryReadServerMapList(options.ServerBinPath) ?? new Dictionary<uint, string>();

        var npcRows = BuildNpcGateRows(type25Npcs, clientDungeonNpcPortals, serverPortals, serverNpcAssets, clientMapNames, serverMapNames);
        var portalRows = BuildPortalDiffRows(type25Npcs, clientDungeonNpcPortals, serverPortals, serverNpcAssets, clientMapNames, serverMapNames);
        var serverOnlyRows = BuildServerOnlyRows(clientDungeonNpcPortals, serverPortals, serverNpcAssets, serverMapNames);

        var npcCsv = Path.Combine(options.OutputDirectory, "dungeon_type25_npc_gate_diff.csv");
        var portalCsv = Path.Combine(options.OutputDirectory, "dungeon_mapportal_diff.csv");
        var serverOnlyCsv = Path.Combine(options.OutputDirectory, "dungeon_server_only_portals.csv");

        WriteCsv(npcCsv, NpcGateRow.Header, npcRows.Select(x => x.ToCsvFields()));
        WriteCsv(portalCsv, PortalDiffRow.Header, portalRows.Select(x => x.ToCsvFields()));
        WriteCsv(serverOnlyCsv, ServerOnlyPortalRow.Header, serverOnlyRows.Select(x => x.ToCsvFields()));

        var type25WithoutLinkedClientPortal = npcRows
            .Where(x => x.RowStatus == "NoClientDungeonNpcPortal")
            .Select(x => (x.NpcId, x.NpcMapId))
            .Distinct()
            .Count();

        var linkedPortalsMissingServer = portalRows.Count(x => !x.ServerPortalExists);
        var linkedPortalsMissingServerNpcAsset = portalRows.Count(x => !x.ServerNpcAssetExists);
        var linkedPortalsMissingServerNpcPortalIndex = portalRows.Count(x => x.ServerNpcAssetExists && !x.ServerNpcPortalIndexExists);
        var payloadExcluded = npcRows.Count(x => x.RowStatus == "PayloadExcludesClientPortal");

        var sb = new StringBuilder();
        sb.AppendLine("Dungeon Type 25 bin report");
        sb.AppendLine($"Pack input: {options.LkdmoInputPath}");
        sb.AppendLine($"Client MapPortal path: {clientMapPortalPath.RelativePath}");
        sb.AppendLine($"Server bin path: {options.ServerBinPath}");
        sb.AppendLine();
        sb.AppendLine($"Client Type 25 NPCs: {type25Npcs.Count:N0}");
        sb.AppendLine($"Client dungeon NPC portals: {clientDungeonNpcPortals.Count:N0}");
        sb.AppendLine($"Type 25 NPCs without linked client dungeon portal: {type25WithoutLinkedClientPortal:N0}");
        sb.AppendLine($"Linked client portals missing in server MapPortal.bin: {linkedPortalsMissingServer:N0}");
        sb.AppendLine($"Linked client portals missing server NpcAssets.bin NPC row: {linkedPortalsMissingServerNpcAsset:N0}");
        sb.AppendLine($"Linked client portals missing server NpcAssets.bin portal index: {linkedPortalsMissingServerNpcPortalIndex:N0}");
        sb.AppendLine($"Type 25 payload entries that exclude an otherwise linked client portal: {payloadExcluded:N0}");
        sb.AppendLine($"Server-only dungeon NPC portals: {serverOnlyRows.Count:N0}");
        sb.AppendLine();
        sb.AppendLine("How to read the mapping:");
        sb.AppendLine("  NPC Type 25 window entries come from MapPortal.bin rows where PortalType=1, ObjectType=1, SourceMapId=NPC.MapId and UniqObjectId=NPC.NpcId.");
        sb.AppendLine("  The Enter button sends PortalId. The server validates the same PortalId in its active MapPortal.bin.");
        sb.AppendLine("  Entry requirements are read by the server from NpcAssets.bin using NpcId, MapId and PortalKindIndex.");
        sb.AppendLine();
        sb.AppendLine($"NPC gate diff CSV: {npcCsv}");
        sb.AppendLine($"MapPortal diff CSV: {portalCsv}");
        sb.AppendLine($"Server-only portal CSV: {serverOnlyCsv}");
        return sb.ToString();
    }

    private static IReadOnlyList<NpcGateRow> BuildNpcGateRows(
        IReadOnlyList<NpcRecord> type25Npcs,
        IReadOnlyList<MapPortalRecord> clientDungeonNpcPortals,
        IReadOnlyList<MapPortalRecord>? serverPortals,
        IReadOnlyList<NpcAssetRecord>? serverNpcAssets,
        IReadOnlyDictionary<uint, string> clientMapNames,
        IReadOnlyDictionary<uint, string> serverMapNames)
    {
        var rows = new List<NpcGateRow>();
        foreach (var npc in type25Npcs)
        {
            var linked = clientDungeonNpcPortals
                .Where(x => x.SourceMapId == npc.MapId && x.UniqObjectId == npc.NpcId)
                .OrderBy(x => x.PortalKindIndex)
                .ThenBy(x => x.PortalId)
                .ToList();

            if (linked.Count == 0)
            {
                rows.Add(NpcGateRow.NoClientPortal(npc));
                continue;
            }

            foreach (var portal in linked)
            {
                var payloadMatches = PayloadMatches(npc, portal);
                var serverPortal = serverPortals?.FirstOrDefault(x => x.PortalId == portal.PortalId);
                var serverNpcAsset = FindServerNpcAsset(serverNpcAssets, portal);
                var serverNpcPortal = FindServerNpcPortal(serverNpcAsset, portal.PortalKindIndex);
                var status = payloadMatches ? "Linked" : "PayloadExcludesClientPortal";

                rows.Add(new NpcGateRow(
                    npc.NpcId,
                    npc.MapId,
                    npc.Name,
                    npc.PayloadIds.Count,
                    string.Join(';', npc.PayloadIds),
                    linked.Count,
                    status,
                    portal.PortalId,
                    portal.PortalKindIndex,
                    portal.PortalType,
                    portal.ObjectType,
                    portal.DestMapId,
                    MapName(clientMapNames, portal.DestMapId),
                    portal.DestX,
                    portal.DestY,
                    payloadMatches,
                    serverPortal is not null,
                    SameSource(serverPortal, portal),
                    SameNpc(serverPortal, portal),
                    serverPortal?.DestMapId,
                    serverPortal is null ? string.Empty : MapName(serverMapNames, serverPortal.DestMapId),
                    serverNpcAsset is not null,
                    serverNpcPortal is not null,
                    DescribeRequirements(serverNpcPortal)));
            }
        }

        return rows;
    }

    private static IReadOnlyList<PortalDiffRow> BuildPortalDiffRows(
        IReadOnlyList<NpcRecord> type25Npcs,
        IReadOnlyList<MapPortalRecord> clientDungeonNpcPortals,
        IReadOnlyList<MapPortalRecord>? serverPortals,
        IReadOnlyList<NpcAssetRecord>? serverNpcAssets,
        IReadOnlyDictionary<uint, string> clientMapNames,
        IReadOnlyDictionary<uint, string> serverMapNames)
    {
        var rows = new List<PortalDiffRow>();
        foreach (var portal in clientDungeonNpcPortals)
        {
            var npc = type25Npcs.FirstOrDefault(x => x.MapId == portal.SourceMapId && x.NpcId == portal.UniqObjectId);
            var serverPortal = serverPortals?.FirstOrDefault(x => x.PortalId == portal.PortalId);
            var serverNpcAsset = FindServerNpcAsset(serverNpcAssets, portal);
            var serverNpcPortal = FindServerNpcPortal(serverNpcAsset, portal.PortalKindIndex);

            rows.Add(new PortalDiffRow(
                portal.PortalId,
                portal.SourceMapId,
                MapName(clientMapNames, portal.SourceMapId),
                portal.UniqObjectId,
                portal.PortalKindIndex,
                portal.DestMapId,
                MapName(clientMapNames, portal.DestMapId),
                portal.DestX,
                portal.DestY,
                npc is not null,
                npc?.Name ?? string.Empty,
                npc is null ? null : PayloadMatches(npc, portal),
                serverPortal is not null,
                SameSource(serverPortal, portal),
                SameNpc(serverPortal, portal),
                serverPortal?.DestMapId,
                serverPortal is null ? string.Empty : MapName(serverMapNames, serverPortal.DestMapId),
                serverNpcAsset is not null,
                serverNpcPortal is not null,
                DescribeRequirements(serverNpcPortal)));
        }

        return rows;
    }

    private static IReadOnlyList<ServerOnlyPortalRow> BuildServerOnlyRows(
        IReadOnlyList<MapPortalRecord> clientDungeonNpcPortals,
        IReadOnlyList<MapPortalRecord>? serverPortals,
        IReadOnlyList<NpcAssetRecord>? serverNpcAssets,
        IReadOnlyDictionary<uint, string> serverMapNames)
    {
        if (serverPortals is null)
            return [];

        var clientPortalIds = clientDungeonNpcPortals.Select(x => x.PortalId).ToHashSet();
        return serverPortals
            .Where(IsDungeonNpcPortal)
            .Where(x => !clientPortalIds.Contains(x.PortalId))
            .OrderBy(x => x.SourceMapId)
            .ThenBy(x => x.UniqObjectId)
            .ThenBy(x => x.PortalKindIndex)
            .ThenBy(x => x.PortalId)
            .Select(portal =>
            {
                var serverNpcAsset = FindServerNpcAsset(serverNpcAssets, portal);
                var serverNpcPortal = FindServerNpcPortal(serverNpcAsset, portal.PortalKindIndex);
                return new ServerOnlyPortalRow(
                    portal.PortalId,
                    portal.SourceMapId,
                    MapName(serverMapNames, portal.SourceMapId),
                    portal.UniqObjectId,
                    portal.PortalKindIndex,
                    portal.DestMapId,
                    MapName(serverMapNames, portal.DestMapId),
                    portal.DestX,
                    portal.DestY,
                    serverNpcAsset is not null,
                    serverNpcPortal is not null,
                    DescribeRequirements(serverNpcPortal));
            })
            .ToList();
    }

    private static IReadOnlyList<NpcRecord> ParseNpcBin(byte[] bytes)
    {
        using var stream = new MemoryStream(bytes);
        using var reader = new BinaryReader(stream);

        var count = ReadCount(reader, "NPC");
        var records = new List<NpcRecord>(count);
        for (var i = 0; i < count; i++)
        {
            var info = reader.ReadBytes(NpcInfoSize);
            if (info.Length != NpcInfoSize)
                throw new EndOfStreamException($"Npc record {i} exceeds Npc.bin length.");

            var npcId = BitConverter.ToUInt32(info, 0);
            var mapId = BitConverter.ToUInt32(info, 4);
            var type = BitConverter.ToInt32(info, 8);
            var name = ReadFixedUtf16(info, NpcNameOffset, NpcNameBytes);
            var payloadIds = new List<uint>();

            if (ItemPayloadTypes.Contains(type))
            {
                var payloadCount = ReadCount(reader, $"item payload for NPC {npcId}");
                for (var p = 0; p < payloadCount; p++)
                    payloadIds.Add(reader.ReadUInt32());
            }
            else if (type == 3)
            {
                _ = reader.ReadInt32();
                var portalCount = ReadCount(reader, $"portal payload for NPC {npcId}");
                reader.BaseStream.Position += portalCount * NpcPortalRecordSize;
            }
            else if (type == 19)
            {
                var subType = reader.ReadInt32();
                if (subType == 2)
                {
                    var itemCount = ReadCount(reader, $"special event payload for NPC {npcId}");
                    reader.BaseStream.Position += itemCount * sizeof(uint);
                }
            }

            var extraCount = ReadCount(reader, $"extra payload for NPC {npcId}");
            for (var e = 0; e < extraCount; e++)
            {
                var extraType = reader.ReadInt32();
                if (extraType != 0)
                    throw new InvalidDataException($"Unsupported NPC extra type {extraType} for NPC {npcId}.");

                _ = reader.ReadUInt32();
                var actionCount = ReadCount(reader, $"quest extra actions for NPC {npcId}");
                for (var a = 0; a < actionCount; a++)
                {
                    _ = reader.ReadUInt32();
                    _ = reader.ReadUInt32();
                    var questCount = ReadCount(reader, $"quest extra list for NPC {npcId}");
                    reader.BaseStream.Position += questCount * sizeof(uint);
                }
            }

            records.Add(new NpcRecord(npcId, mapId, type, name, payloadIds));
        }

        return records;
    }

    private static IReadOnlyList<MapPortalRecord> ParseMapPortal(byte[] bytes)
    {
        using var stream = new MemoryStream(bytes);
        using var reader = new BinaryReader(stream);

        var groupCount = ReadCount(reader, "portal groups");
        var records = new List<MapPortalRecord>();
        for (var g = 0; g < groupCount; g++)
        {
            var portalCount = ReadCount(reader, $"portal group {g}");
            for (var p = 0; p < portalCount; p++)
            {
                records.Add(new MapPortalRecord(
                    reader.ReadUInt32(),
                    reader.ReadUInt32(),
                    reader.ReadUInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32(),
                    reader.ReadUInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32(),
                    reader.ReadUInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32()));
            }
        }

        return records;
    }

    private static IReadOnlyDictionary<uint, string> ParseMapList(byte[] bytes)
    {
        using var stream = new MemoryStream(bytes);
        using var reader = new BinaryReader(stream);

        var count = ReadCount(reader, "map list");
        var result = new Dictionary<uint, string>(count);
        for (var i = 0; i < count; i++)
        {
            var mapId = reader.ReadUInt32();
            var mapName = ReadFramedAscii(reader, 4096);
            _ = ReadFramedAscii(reader, 4096);
            _ = ReadFramedAscii(reader, 4096);
            _ = reader.ReadInt32();
            _ = reader.ReadInt32();
            var description = ReadFramedUtf16CharCount(reader, 8192);
            var descriptionEnglish = ReadFramedUtf16CharCount(reader, 8192);
            _ = reader.ReadUInt32();
            _ = reader.ReadUInt16();
            _ = reader.ReadUInt16();
            _ = reader.ReadUInt16();
            _ = reader.ReadUInt16();
            _ = reader.ReadUInt16();
            _ = reader.ReadInt16();
            _ = reader.ReadInt16();
            _ = reader.ReadBoolean();
            _ = reader.ReadBoolean();

            var displayName = FirstNonEmpty(description, descriptionEnglish, mapName, $"Map {mapId}");
            result[mapId] = displayName;
        }

        return result;
    }

    private static IReadOnlyList<NpcAssetRecord> ParseNpcAssets(byte[] bytes)
    {
        using var stream = new MemoryStream(bytes);
        using var reader = new BinaryReader(stream);

        var npcCount = ReadCount(reader, "server NPC assets");
        var npcs = new List<NpcAssetRecord>(npcCount);
        for (var n = 0; n < npcCount; n++)
        {
            _ = reader.ReadInt64();
            var npcId = reader.ReadInt32();
            var mapId = reader.ReadInt32();

            var itemCount = ReadCount(reader, $"server NPC {npcId} item assets");
            for (var i = 0; i < itemCount; i++)
            {
                _ = reader.ReadInt64();
                _ = reader.ReadInt32();
            }

            var portalCount = ReadCount(reader, $"server NPC {npcId} portal assets");
            var portals = new List<NpcPortalAssetRecord>(portalCount);
            for (var p = 0; p < portalCount; p++)
            {
                _ = reader.ReadInt64();
                var portalType = reader.ReadInt32();
                var portalEntryCount = reader.ReadInt32();
                var groupCount = ReadCount(reader, $"server NPC {npcId} portal groups");
                var requirements = new List<NpcPortalRequirementRecord>();
                for (var g = 0; g < groupCount; g++)
                {
                    _ = reader.ReadInt64();
                    var resourceCount = ReadCount(reader, $"server NPC {npcId} portal resources");
                    for (var r = 0; r < resourceCount; r++)
                    {
                        _ = reader.ReadInt64();
                        requirements.Add(new NpcPortalRequirementRecord(
                            reader.ReadInt32(),
                            reader.ReadInt32(),
                            reader.ReadInt32()));
                    }
                }

                portals.Add(new NpcPortalAssetRecord(p, portalType, portalEntryCount, requirements));
            }

            npcs.Add(new NpcAssetRecord(npcId, mapId, portals));
        }

        return npcs;
    }

    private static (string RelativePath, byte[] Bytes) ResolvePackFile(string packPath, IReadOnlyList<string> candidates)
    {
        foreach (var candidate in candidates)
        {
            var bytes = TryReadPackFile(packPath, candidate);
            if (bytes is not null)
                return (candidate, bytes);
        }

        throw new FileNotFoundException($"None of the pack entries were found: {string.Join(", ", candidates)}");
    }

    private static byte[]? TryReadPackFile(string packPath, IReadOnlyList<string> candidates)
    {
        foreach (var candidate in candidates)
        {
            var bytes = TryReadPackFile(packPath, candidate);
            if (bytes is not null)
                return bytes;
        }

        return null;
    }

    private static byte[]? TryReadPackFile(string packPath, string relativePath)
    {
        try
        {
            return Pack03Archive.ReadFile(packPath, relativePath);
        }
        catch (FileNotFoundException)
        {
            return null;
        }
    }

    private static IReadOnlyList<MapPortalRecord>? TryReadServerMapPortal(string serverBinPath)
    {
        if (string.IsNullOrWhiteSpace(serverBinPath))
            return null;

        var path = Path.Combine(serverBinPath.Trim().Trim('"'), "MapPortal.bin");
        return File.Exists(path) ? ParseMapPortal(File.ReadAllBytes(path)) : null;
    }

    private static IReadOnlyDictionary<uint, string>? TryReadServerMapList(string serverBinPath)
    {
        if (string.IsNullOrWhiteSpace(serverBinPath))
            return null;

        var path = Path.Combine(serverBinPath.Trim().Trim('"'), "MapList.bin");
        return File.Exists(path) ? ParseMapList(File.ReadAllBytes(path)) : null;
    }

    private static IReadOnlyList<NpcAssetRecord>? TryReadServerNpcAssets(string serverBinPath)
    {
        if (string.IsNullOrWhiteSpace(serverBinPath))
            return null;

        var path = Path.Combine(serverBinPath.Trim().Trim('"'), "NpcAssets.bin");
        return File.Exists(path) ? ParseNpcAssets(File.ReadAllBytes(path)) : null;
    }

    private static bool IsDungeonNpcPortal(MapPortalRecord portal)
    {
        return portal.PortalType == DungeonPortalType && portal.ObjectType == NpcPortalObjectType;
    }

    private static bool PayloadMatches(NpcRecord npc, MapPortalRecord portal)
    {
        return npc.PayloadIds.Count == 0 ||
               npc.PayloadIds.Contains(portal.PortalId) ||
               npc.PayloadIds.Contains(portal.DestMapId);
    }

    private static bool SameSource(MapPortalRecord? serverPortal, MapPortalRecord clientPortal)
    {
        return serverPortal is not null && serverPortal.SourceMapId == clientPortal.SourceMapId;
    }

    private static bool SameNpc(MapPortalRecord? serverPortal, MapPortalRecord clientPortal)
    {
        return serverPortal is not null &&
               serverPortal.UniqObjectId == clientPortal.UniqObjectId &&
               serverPortal.ObjectType == clientPortal.ObjectType;
    }

    private static NpcAssetRecord? FindServerNpcAsset(IReadOnlyList<NpcAssetRecord>? serverNpcAssets, MapPortalRecord portal)
    {
        return serverNpcAssets?.FirstOrDefault(x => x.NpcId == portal.UniqObjectId && x.MapId == portal.SourceMapId);
    }

    private static NpcPortalAssetRecord? FindServerNpcPortal(NpcAssetRecord? npcAsset, int portalKindIndex)
    {
        return npcAsset?.Portals.FirstOrDefault(x => x.Index == portalKindIndex);
    }

    private static string DescribeRequirements(NpcPortalAssetRecord? portal)
    {
        if (portal is null)
            return string.Empty;

        var active = portal.Requirements.Where(x => x.Type != 0).ToList();
        if (active.Count == 0)
            return "none";

        return string.Join(';', active.Select(x =>
        {
            var typeName = x.Type switch
            {
                1 => "Item",
                2 => "Money",
                _ => $"Type{x.Type}"
            };
            return $"{typeName}:{x.ItemId}x{x.Amount}";
        }));
    }

    private static string MapName(IReadOnlyDictionary<uint, string> maps, uint mapId)
    {
        return maps.TryGetValue(mapId, out var name) && !string.IsNullOrWhiteSpace(name)
            ? name
            : $"Map {mapId}";
    }

    private static string FirstNonEmpty(params string[] values)
    {
        foreach (var value in values)
        {
            if (!string.IsNullOrWhiteSpace(value))
                return value;
        }

        return string.Empty;
    }

    private static int ReadCount(BinaryReader reader, string context)
    {
        var value = reader.ReadInt32();
        if (value < 0 || value > 100000)
            throw new InvalidDataException($"Invalid {context} count {value}.");
        return value;
    }

    private static string ReadFramedAscii(BinaryReader reader, int maxBytes)
    {
        var length = reader.ReadInt32();
        if (length < 0 || length > maxBytes)
            throw new InvalidDataException($"Invalid ASCII string length {length}.");
        return length == 0 ? string.Empty : Encoding.Default.GetString(reader.ReadBytes(length)).TrimEnd('\0');
    }

    private static string ReadFramedUtf16CharCount(BinaryReader reader, int maxChars)
    {
        var chars = reader.ReadInt32();
        if (chars < 0 || chars > maxChars)
            throw new InvalidDataException($"Invalid UTF-16 string length {chars}.");
        return chars == 0 ? string.Empty : Encoding.Unicode.GetString(reader.ReadBytes(chars * 2)).TrimEnd('\0');
    }

    private static string ReadFixedUtf16(byte[] bytes, int offset, int byteLength)
    {
        var text = Encoding.Unicode.GetString(bytes, offset, byteLength);
        var nullIndex = text.IndexOf('\0');
        return nullIndex >= 0 ? text[..nullIndex] : text.TrimEnd('\0');
    }

    private static void WriteCsv(string path, IReadOnlyList<string> header, IEnumerable<IReadOnlyList<string>> rows)
    {
        using var writer = new StreamWriter(path, false, new UTF8Encoding(encoderShouldEmitUTF8Identifier: true));
        writer.WriteLine(string.Join(',', header.Select(EscapeCsv)));
        foreach (var row in rows)
            writer.WriteLine(string.Join(',', row.Select(EscapeCsv)));
    }

    private static string EscapeCsv(string? value)
    {
        value ??= string.Empty;
        return value.Contains('"') || value.Contains(',') || value.Contains('\n') || value.Contains('\r')
            ? $"\"{value.Replace("\"", "\"\"")}\""
            : value;
    }

    private sealed record NpcRecord(uint NpcId, uint MapId, int Type, string Name, IReadOnlyList<uint> PayloadIds);

    private sealed record MapPortalRecord(
        uint PortalId,
        uint PortalType,
        uint SourceMapId,
        int SourceX,
        int SourceY,
        int SourceRadius,
        uint DestMapId,
        int DestX,
        int DestY,
        int DestRadius,
        int ObjectType,
        uint UniqObjectId,
        int PortalKindIndex,
        int ViewX,
        int ViewY);

    private sealed record NpcAssetRecord(int NpcId, int MapId, IReadOnlyList<NpcPortalAssetRecord> Portals);
    private sealed record NpcPortalAssetRecord(int Index, int PortalType, int PortalCount, IReadOnlyList<NpcPortalRequirementRecord> Requirements);
    private sealed record NpcPortalRequirementRecord(int Type, int ItemId, int Amount);

    private sealed record NpcGateRow(
        uint NpcId,
        uint NpcMapId,
        string NpcName,
        int PayloadCount,
        string PayloadIds,
        int ClientLinkedPortalCount,
        string RowStatus,
        uint? PortalId,
        int? PortalKindIndex,
        uint? ClientPortalType,
        int? ClientPortalObjectType,
        uint? ClientDestMapId,
        string ClientDestMapName,
        int? ClientDestX,
        int? ClientDestY,
        bool? PayloadMatches,
        bool ServerPortalExists,
        bool ServerPortalSameSource,
        bool ServerPortalSameNpc,
        uint? ServerDestMapId,
        string ServerDestMapName,
        bool ServerNpcAssetExists,
        bool ServerNpcPortalIndexExists,
        string ServerRequirementSummary)
    {
        public static string[] Header { get; } =
        [
            "NpcId", "NpcMapId", "NpcName", "PayloadCount", "PayloadIds", "ClientLinkedPortalCount",
            "RowStatus", "PortalId", "PortalKindIndex", "ClientPortalType", "ClientPortalObjectType",
            "ClientDestMapId", "ClientDestMapName", "ClientDestX", "ClientDestY", "PayloadMatches",
            "ServerPortalExists", "ServerPortalSameSource", "ServerPortalSameNpc", "ServerDestMapId",
            "ServerDestMapName", "ServerNpcAssetExists", "ServerNpcPortalIndexExists", "ServerRequirementSummary"
        ];

        public static NpcGateRow NoClientPortal(NpcRecord npc) =>
            new(
                npc.NpcId,
                npc.MapId,
                npc.Name,
                npc.PayloadIds.Count,
                string.Join(';', npc.PayloadIds),
                0,
                "NoClientDungeonNpcPortal",
                null,
                null,
                null,
                null,
                null,
                string.Empty,
                null,
                null,
                null,
                false,
                false,
                false,
                null,
                string.Empty,
                false,
                false,
                string.Empty);

        public string[] ToCsvFields() =>
        [
            NpcId.ToString(), NpcMapId.ToString(), NpcName, PayloadCount.ToString(), PayloadIds,
            ClientLinkedPortalCount.ToString(), RowStatus, PortalId?.ToString() ?? string.Empty,
            PortalKindIndex?.ToString() ?? string.Empty, ClientPortalType?.ToString() ?? string.Empty,
            ClientPortalObjectType?.ToString() ?? string.Empty, ClientDestMapId?.ToString() ?? string.Empty,
            ClientDestMapName, ClientDestX?.ToString() ?? string.Empty, ClientDestY?.ToString() ?? string.Empty,
            PayloadMatches?.ToString().ToLowerInvariant() ?? string.Empty, ServerPortalExists ? "true" : "false",
            ServerPortalSameSource ? "true" : "false", ServerPortalSameNpc ? "true" : "false",
            ServerDestMapId?.ToString() ?? string.Empty, ServerDestMapName, ServerNpcAssetExists ? "true" : "false",
            ServerNpcPortalIndexExists ? "true" : "false", ServerRequirementSummary
        ];
    }

    private sealed record PortalDiffRow(
        uint PortalId,
        uint SourceMapId,
        string SourceMapName,
        uint NpcObjectId,
        int PortalKindIndex,
        uint DestMapId,
        string DestMapName,
        int DestX,
        int DestY,
        bool ClientHasType25Npc,
        string Type25NpcName,
        bool? PayloadMatches,
        bool ServerPortalExists,
        bool ServerPortalSameSource,
        bool ServerPortalSameNpc,
        uint? ServerDestMapId,
        string ServerDestMapName,
        bool ServerNpcAssetExists,
        bool ServerNpcPortalIndexExists,
        string ServerRequirementSummary)
    {
        public static string[] Header { get; } =
        [
            "PortalId", "SourceMapId", "SourceMapName", "NpcObjectId", "PortalKindIndex", "DestMapId",
            "DestMapName", "DestX", "DestY", "ClientHasType25Npc", "Type25NpcName", "PayloadMatches",
            "ServerPortalExists", "ServerPortalSameSource", "ServerPortalSameNpc", "ServerDestMapId",
            "ServerDestMapName", "ServerNpcAssetExists", "ServerNpcPortalIndexExists", "ServerRequirementSummary"
        ];

        public string[] ToCsvFields() =>
        [
            PortalId.ToString(), SourceMapId.ToString(), SourceMapName, NpcObjectId.ToString(),
            PortalKindIndex.ToString(), DestMapId.ToString(), DestMapName, DestX.ToString(), DestY.ToString(),
            ClientHasType25Npc ? "true" : "false", Type25NpcName,
            PayloadMatches?.ToString().ToLowerInvariant() ?? string.Empty, ServerPortalExists ? "true" : "false",
            ServerPortalSameSource ? "true" : "false", ServerPortalSameNpc ? "true" : "false",
            ServerDestMapId?.ToString() ?? string.Empty, ServerDestMapName, ServerNpcAssetExists ? "true" : "false",
            ServerNpcPortalIndexExists ? "true" : "false", ServerRequirementSummary
        ];
    }

    private sealed record ServerOnlyPortalRow(
        uint PortalId,
        uint SourceMapId,
        string SourceMapName,
        uint NpcObjectId,
        int PortalKindIndex,
        uint DestMapId,
        string DestMapName,
        int DestX,
        int DestY,
        bool ServerNpcAssetExists,
        bool ServerNpcPortalIndexExists,
        string ServerRequirementSummary)
    {
        public static string[] Header { get; } =
        [
            "PortalId", "SourceMapId", "SourceMapName", "NpcObjectId", "PortalKindIndex", "DestMapId",
            "DestMapName", "DestX", "DestY", "ServerNpcAssetExists", "ServerNpcPortalIndexExists",
            "ServerRequirementSummary"
        ];

        public string[] ToCsvFields() =>
        [
            PortalId.ToString(), SourceMapId.ToString(), SourceMapName, NpcObjectId.ToString(),
            PortalKindIndex.ToString(), DestMapId.ToString(), DestMapName, DestX.ToString(), DestY.ToString(),
            ServerNpcAssetExists ? "true" : "false", ServerNpcPortalIndexExists ? "true" : "false",
            ServerRequirementSummary
        ];
    }
}
