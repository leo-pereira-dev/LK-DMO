using DigitalWorldOnline.Commons.DTOs.Assets;
using DigitalWorldOnline.Commons.Enums.ClientEnums;

namespace DigitalWorldOnline.Application.GameAssets.Bins;

/// <summary>
/// Parses the server-side NPC asset catalog. This binary replaces the old
/// Asset_Npc database tables while keeping the same DTO shape used by the
/// runtime services.
///
/// Layout:
/// int npcCount
///   long id
///   int npcId
///   int mapId
///   int itemCount
///     long id
///     int itemId
///   int portalCount
///     long id
///     int portalType
///     int portalCount
///     int portalGroupCount
///       long id
///       int resourceCount
///         long id
///         int type
///         int itemId
///         int resourceAmount
/// </summary>
public sealed class NpcBinLoader
{
    private const string FileName = "NpcAssets.bin";

    private NpcBin? _data;

    public NpcBin Data => _data ?? throw new InvalidOperationException(
        $"{nameof(NpcBinLoader)}: bin not loaded yet - call Load() first.");

    public bool IsLoaded => _data != null;

    public NpcBin Load()
    {
        if (_data != null) return _data;

        var path = Path.Combine(BinPath.ResolveDirectory(), FileName);
        using var stream = File.OpenRead(path);
        using var reader = new BinaryReader(stream);
        _data = Parse(reader);
        return _data;
    }

    public static NpcBin Parse(BinaryReader reader)
    {
        var npcCount = ReadCount(reader, "NPC");
        var npcs = new List<NpcAssetDTO>(npcCount);

        for (var i = 0; i < npcCount; i++)
            npcs.Add(ReadNpc(reader));

        return new NpcBin(npcs);
    }

    private static NpcAssetDTO ReadNpc(BinaryReader reader)
    {
        var npc = new NpcAssetDTO
        {
            Id = reader.ReadInt64(),
            NpcId = reader.ReadInt32(),
            MapId = reader.ReadInt32(),
            Items = new List<NpcItemAssetDTO>(),
            Portals = new List<NpcPortalAssetDTO>()
        };

        var itemCount = ReadCount(reader, $"items for NPC {npc.NpcId}");
        for (var i = 0; i < itemCount; i++)
            npc.Items.Add(ReadItem(reader, npc));

        var portalCount = ReadCount(reader, $"portals for NPC {npc.NpcId}");
        for (var i = 0; i < portalCount; i++)
            npc.Portals.Add(ReadPortal(reader, npc));

        return npc;
    }

    private static NpcItemAssetDTO ReadItem(BinaryReader reader, NpcAssetDTO npc)
    {
        return new NpcItemAssetDTO
        {
            Id = reader.ReadInt64(),
            ItemId = reader.ReadInt32(),
            NpcAssetId = npc.Id,
            NpcAsset = npc
        };
    }

    private static NpcPortalAssetDTO ReadPortal(BinaryReader reader, NpcAssetDTO npc)
    {
        var portal = new NpcPortalAssetDTO
        {
            Id = reader.ReadInt64(),
            PortalType = reader.ReadInt32(),
            PortalCount = reader.ReadInt32(),
            NpcAssetId = npc.Id,
            NpcAsset = npc,
            PortalsAsset = new List<NpcPortalsAmountAssetDTO>()
        };

        var groupCount = ReadCount(reader, $"portal groups for NPC {npc.NpcId}");
        for (var i = 0; i < groupCount; i++)
            portal.PortalsAsset.Add(ReadPortalGroup(reader, portal));

        return portal;
    }

    private static NpcPortalsAmountAssetDTO ReadPortalGroup(BinaryReader reader, NpcPortalAssetDTO portal)
    {
        var group = new NpcPortalsAmountAssetDTO
        {
            Id = reader.ReadInt64(),
            NpcAssetId = portal.Id,
            NpcAsset = portal,
            npcPortalsAsset = new List<NpcPortalsAssetDTO>()
        };

        var resourceCount = ReadCount(reader, $"portal resources for portal {portal.Id}");
        for (var i = 0; i < resourceCount; i++)
            group.npcPortalsAsset.Add(ReadPortalResource(reader, group));

        return group;
    }

    private static NpcPortalsAssetDTO ReadPortalResource(BinaryReader reader, NpcPortalsAmountAssetDTO group)
    {
        return new NpcPortalsAssetDTO
        {
            Id = reader.ReadInt64(),
            Type = (NpcResourceTypeEnum)reader.ReadInt32(),
            ItemId = reader.ReadInt32(),
            ResourceAmount = reader.ReadInt32(),
            NpcAssetId = group.Id,
            NpcAsset = group
        };
    }

    private static int ReadCount(BinaryReader reader, string label)
    {
        var count = reader.ReadInt32();
        if (count < 0)
            throw new InvalidDataException($"{nameof(NpcBinLoader)}: negative {label} count ({count}).");

        return count;
    }
}
