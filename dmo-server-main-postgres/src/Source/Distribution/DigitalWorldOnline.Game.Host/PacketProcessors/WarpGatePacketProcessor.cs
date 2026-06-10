using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.MapServer;
using DigitalWorldOnline.Game.Configuration;
using DigitalWorldOnline.Game.Diagnostics;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.Game.Services;
using DigitalWorldOnline.GameHost;
using MediatR;
using Microsoft.Extensions.Configuration;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class WarpGatePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.WarpGate;

        private const string GamerServerPublic = "GameServer:PublicAddress";
        private const string GameServerPort = "GameServer:Port";

        private readonly PartyManager _partyManager;
        private readonly IConfiguration _configuration;
        private readonly AssetsLoader _assets;
        private readonly MapServer _mapServer;
        private readonly ISender _sender;
        private readonly ILogger _logger;
        private readonly OwnerStorageFlushService _ownerStorageFlushService;
        private readonly PortalDestinationResolver _portalDestinationResolver;

        public WarpGatePacketProcessor(
            PartyManager partyManager,
            IConfiguration configuration,
            AssetsLoader assets,
            MapServer mapServer,
            ISender sender,
            ILogger logger,
            OwnerStorageFlushService ownerStorageFlushService,
            PortalDestinationResolver portalDestinationResolver)
        {
            _partyManager = partyManager;
            _configuration = configuration;
            _assets = assets;
            _mapServer = mapServer;
            _sender = sender;
            _logger = logger;
            _ownerStorageFlushService = ownerStorageFlushService;
            _portalDestinationResolver = portalDestinationResolver;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var portalId = packet.ReadInt();
            PortalTrace.Write($"WarpGate request tamer={client.TamerId} portal={portalId} map={client.Tamer?.Location?.MapId} x={client.Tamer?.Location?.X} y={client.Tamer?.Location?.Y}");

            var portal = _assets.Portal.FirstOrDefault(x => x.Id == portalId);

            if (portal == null)
            {
                client.Send(new SystemMessagePacket($"Portal {portalId} not found."));
                client.Send(new SelectPortalFailurePacket());
                _logger.Error($"Portal id {portalId} not found.");
                return;
            }
            else
            {
                var destination = _portalDestinationResolver.Resolve(client, portal);
                if (!destination.Success)
                {
                    PortalTrace.Write($"WarpGate rejected tamer={client.TamerId} portal={portalId} map={client.Tamer.Location.MapId} reason={destination.FailureReason}");
                    client.Send(new SystemMessagePacket(destination.FailureReason ?? "Invalid portal."));
                    client.Send(new SelectPortalFailurePacket());
                    _logger.Warning(
                        "Warp rejected: tamer={TamerId} portal={PortalId} map={MapId} x={X} y={Y} reason={Reason}",
                        client.TamerId,
                        portalId,
                        client.Tamer.Location.MapId,
                        client.Tamer.Location.X,
                        client.Tamer.Location.Y,
                        destination.FailureReason);
                    return;
                }

                if (!await TryConsumePortalRequirementAsync(client, portal))
                {
                    PortalTrace.Write($"WarpGate requirement rejected tamer={client.TamerId} portal={portalId} map={client.Tamer.Location.MapId}");
                    client.Send(new SelectPortalFailurePacket());
                    return;
                }

                PortalTrace.Write($"WarpGate resolved tamer={client.TamerId} portal={portalId} sourceMap={destination.SourceMapId} destMap={destination.DestinationMapId} destX={destination.DestinationX} destY={destination.DestinationY} resolver={destination.Source}");

                if(portal.Type == PortalTypeEnum.Dungeon)
                {
                    client.Tamer.NewLocation(destination.DestinationMapId, destination.DestinationX, destination.DestinationY);
                    await _sender.Send(new UpdateCharacterLocationCommand(client.Tamer.Location));

                    client.Tamer.Partner.NewLocation(destination.DestinationMapId, destination.DestinationX, destination.DestinationY);
                    await _sender.Send(new UpdateDigimonLocationCommand(client.Tamer.Partner.Location));

                    client.Send(
                     new LocalMapSwapPacket(
                         client.Tamer.GeneralHandler,
                         client.Tamer.Partner.GeneralHandler,
                         destination.DestinationX,
                         destination.DestinationY,
                         destination.DestinationX,
                         destination.DestinationY
                     )
                 );

                    return;

                }
                await _ownerStorageFlushService.FlushForTransitionAsync(client);
                _mapServer.RemoveClient(client);

                var destMapId = destination.DestinationMapId;
                var destX = destination.DestinationX;
                var destY = destination.DestinationY;

                // Validate destination coordinates — fall back to map's default spawn if invalid
                if (destX <= 0 && destY <= 0)
                {
                    var waypoints = await _sender.Send(new MapRegionListAssetsByMapIdQuery(destMapId));
                    if (waypoints != null && waypoints.Regions.Any())
                    {
                        var fallback = waypoints.Regions.First();
                        destX = fallback.X;
                        destY = fallback.Y;
                        _logger.Warning($"Portal {portal.Id}: invalid destination (0,0). Falling back to map {destMapId} spawn ({destX},{destY}).");
                    }
                }

                client.Tamer.NewLocation(destMapId, destX, destY);
                await _sender.Send(new UpdateCharacterLocationCommand(client.Tamer.Location));

                client.Tamer.Partner.NewLocation(destMapId, destX, destY);
                await _sender.Send(new UpdateDigimonLocationCommand(client.Tamer.Partner.Location));

                client.Tamer.UpdateState(CharacterStateEnum.Loading);
                await _sender.Send(new UpdateCharacterStateCommand(client.TamerId, CharacterStateEnum.Loading));

                client.SetGameQuit(false);
                PortalTrace.Write($"MapSwap send tamer={client.TamerId} portal={portalId} destMap={client.Tamer.Location.MapId} destX={client.Tamer.Location.X} destY={client.Tamer.Location.Y}");

                _logger.Information(
                    "MapSwap: tamer={TamerId} portal={PortalId} source={SourceMapId} destMap={DestMapId} destX={DestX} destY={DestY} resolver={ResolverSource}",
                    client.TamerId,
                    portal.Id,
                    destination.SourceMapId,
                    client.Tamer.Location.MapId,
                    client.Tamer.Location.X,
                    client.Tamer.Location.Y,
                    destination.Source);

                var serverAddress = _configuration[GamerServerPublic];
                var serverPort = _configuration.GetPublicGameServerPort();
                var mapSwapPacket = new MapSwapPacket(
                    serverAddress,
                    serverPort,
                    client.Tamer.Location.MapId,
                    client.Tamer.Location.X,
                    client.Tamer.Location.Y
                );
                var mapSwapBytes = mapSwapPacket.Serialize();

                _logger.Warning(
                    "[MAPSWAP-TRACE] send tamer={TamerId} client={Client} portal={PortalId} endpoint={Address}:{Port} destMap={DestMapId} destX={DestX} destY={DestY} state={State} loading={Loading} bytes={Bytes} hex={Hex}",
                    client.TamerId,
                    client.HiddenAddress,
                    portal.Id,
                    serverAddress,
                    serverPort,
                    client.Tamer.Location.MapId,
                    client.Tamer.Location.X,
                    client.Tamer.Location.Y,
                    client.Tamer.State,
                    client.Loading,
                    mapSwapBytes.Length,
                    ToHexPreview(mapSwapBytes));

                client.Send(mapSwapBytes);
                PortalTrace.Write($"MapSwap sent tamer={client.TamerId} portal={portalId} destMap={client.Tamer.Location.MapId}");
                ScheduleMapSwapWatchdog(client, portal.Id, client.Tamer.Location.MapId, client.Tamer.Location.X, client.Tamer.Location.Y);

                var party = _partyManager.FindParty(client.TamerId);
                if (party != null)
                {
                    party.UpdateMember(party[client.TamerId]);

                    _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                        new PartyMemberWarpGatePacket(party[client.TamerId]).Serialize());
                }
            }
        }

        private void ScheduleMapSwapWatchdog(GameClient client, int portalId, int mapId, int x, int y)
        {
            var clientHash = System.Runtime.CompilerServices.RuntimeHelpers.GetHashCode(client);
            var tamerId = client.TamerId;
            var clientAddress = client.HiddenAddress;

            _ = Task.Run(async () =>
            {
                await Task.Delay(TimeSpan.FromSeconds(12));

                if (client.Tamer?.State == CharacterStateEnum.Loading || client.Loading)
                {
                    _logger.Warning(
                        "[MAPSWAP-WATCHDOG] no post-load yet tamer={TamerId} clientHash={ClientHash} client={Client} portal={PortalId} targetMap={MapId} targetX={X} targetY={Y} connected={Connected} gameQuit={GameQuit} state={State} loading={Loading}",
                        tamerId,
                        clientHash,
                        clientAddress,
                        portalId,
                        mapId,
                        x,
                        y,
                        client.IsConnected,
                        client.GameQuit,
                        client.Tamer?.State,
                        client.Loading);
                }
            });
        }

        private static string ToHexPreview(byte[] bytes)
        {
            var length = Math.Min(bytes.Length, 96);
            return Convert.ToHexString(bytes.AsSpan(0, length));
        }

        private async Task<bool> TryConsumePortalRequirementAsync(GameClient client, Commons.Models.Asset.PortalAssetModel portal)
        {
            var npcAsset = _assets.Npcs.FirstOrDefault(x =>
                x.NpcId == portal.NpcId &&
                x.MapId == client.Tamer.Location.MapId);

            if (npcAsset == null)
                return true;

            if (portal.PortalIndex < 0 || portal.PortalIndex >= npcAsset.Portals.Count)
                return true;

            var npcPortal = npcAsset.Portals[portal.PortalIndex];
            var requirements = (npcPortal.PortalsAsset ?? new List<DigitalWorldOnline.Commons.DTOs.Assets.NpcPortalsAmountAssetModel>())
                .Where(x => x?.npcPortalsAsset != null)
                .SelectMany(x => x.npcPortalsAsset)
                .Where(x => x != null && x.Type != NpcResourceTypeEnum.None)
                .ToList();

            if (!requirements.Any())
                return true;

            foreach (var requirement in requirements)
            {
                if (requirement.Type == NpcResourceTypeEnum.Item)
                {
                    var requiredItemId = requirement.ItemId;
                    var requiredAmount = requirement.ResourceAmount > 0 ? requirement.ResourceAmount : 1;
                    var available = client.Tamer.Inventory.FindItemsById(requiredItemId).Sum(x => x.Amount);
                    if (available < requiredAmount)
                    {
                        client.Send(new SystemMessagePacket("Insufficient required item amount."));
                        _logger.Warning(
                            "Warp denied: tamer {TamerId} missing NPC-portal item {ItemId} x{Amount} for portal {PortalId}.",
                            client.TamerId, requiredItemId, requiredAmount, portal.Id);
                        return false;
                    }
                }
                else if (requirement.Type == NpcResourceTypeEnum.Money)
                {
                    var requiredBits = requirement.ItemId > 0 ? requirement.ItemId : requirement.ResourceAmount;
                    if (requiredBits > 0 && client.Tamer.Inventory.Bits < requiredBits)
                    {
                        client.Send(new SystemMessagePacket("Insufficient bits amount."));
                        _logger.Warning(
                            "Warp denied: tamer {TamerId} missing NPC-portal bits {Bits} for portal {PortalId}.",
                            client.TamerId, requiredBits, portal.Id);
                        return false;
                    }
                }
            }

            var itemsChanged = false;
            var bitsChanged = false;
            foreach (var requirement in requirements)
            {
                if (requirement.Type == NpcResourceTypeEnum.Item)
                {
                    var requiredAmount = requirement.ResourceAmount > 0 ? requirement.ResourceAmount : 1;
                    if (client.Tamer.Inventory.RemoveOrReduceItemsByItemId(requirement.ItemId, requiredAmount))
                    {
                        itemsChanged = true;
                    }
                }
                else if (requirement.Type == NpcResourceTypeEnum.Money)
                {
                    var requiredBits = requirement.ItemId > 0 ? requirement.ItemId : requirement.ResourceAmount;
                    if (requiredBits > 0 && client.Tamer.Inventory.RemoveBits(requiredBits))
                    {
                        bitsChanged = true;
                    }
                }
            }

            if (itemsChanged)
                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));

            if (bitsChanged)
                await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));

            return true;
        }

    }
}
