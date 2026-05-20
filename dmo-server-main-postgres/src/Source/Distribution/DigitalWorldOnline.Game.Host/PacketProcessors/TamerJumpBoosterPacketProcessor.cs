using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.MapServer;
using DigitalWorldOnline.Game.Services;
using DigitalWorldOnline.GameHost;
using MediatR;
using Microsoft.Extensions.Configuration;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class TamerJumpBoosterPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.TamerJumpBooster;

        private readonly MapServer _mapServer;
        private readonly DMBaseBinLoader _dmBase;
        private readonly IConfiguration _configuration;
        private readonly ISender _sender;
        private readonly ILogger _logger;
        private readonly OwnerStorageFlushService _ownerStorageFlushService;

        private const string GamerServerPublic = "GameServer:PublicAddress";
        private const string GameServerPort = "GameServer:Port";

        public TamerJumpBoosterPacketProcessor(
            MapServer mapServer,
            DMBaseBinLoader dmBase,
            IConfiguration configuration,
            ISender sender,
            ILogger logger,
            OwnerStorageFlushService ownerStorageFlushService)
        {
            _mapServer = mapServer;
            _dmBase = dmBase;
            _configuration = configuration;
            _sender = sender;
            _logger = logger;
            _ownerStorageFlushService = ownerStorageFlushService;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var itemId = packet.ReadShort();
            var mapId = packet.ReadShort();

            if (mapId == 1)
                mapId = 3;

            _logger.Information(
                "Tamer jump booster request: tamer {TamerId} packetBytes {PacketBytes} item {ItemId} map {MapId}.",
                client.TamerId,
                packetData.Length,
                itemId,
                mapId);

            if (!_dmBase.Data.JumpBusterDestinations.TryGetValue(itemId, out var allowedMaps)
                || !allowedMaps.Contains(mapId))
            {
                client.Send(new SystemMessagePacket($"Jump booster {itemId} cannot teleport to map {mapId}."));
                _logger.Warning(
                    "Rejecting tamer jump-booster from tamer {TamerId}: item {ItemId} not allowed to map {MapId} per DMBase.bin allowlist.",
                    client.TamerId,
                    itemId,
                    mapId);
                return;
            }

            var waypoints = await _sender.Send(new MapRegionListAssetsByMapIdQuery(mapId));
            if (waypoints == null || !waypoints.Regions.Any())
            {
                client.Send(new SystemMessagePacket($"Map information not found for map Id {mapId}."));
                _logger.Warning($"Map information not found for map Id {mapId} on character {client.TamerId} tamer jump booster.");
                return;
            }

            await _ownerStorageFlushService.FlushForTransitionAsync(client);
            _mapServer.RemoveClient(client);

            var destination = waypoints.Regions.First();

            client.Tamer.NewLocation(mapId, destination.X, destination.Y);
            await _sender.Send(new UpdateCharacterLocationCommand(client.Tamer.Location));

            client.Tamer.Partner.NewLocation(mapId, destination.X, destination.Y);
            await _sender.Send(new UpdateDigimonLocationCommand(client.Tamer.Partner.Location));

            client.Tamer.UpdateState(CharacterStateEnum.Loading);
            await _sender.Send(new UpdateCharacterStateCommand(client.TamerId, CharacterStateEnum.Loading));

            client.SetGameQuit(false);

            client.Send(new MapSwapPacket(
                _configuration[GamerServerPublic],
                _configuration[GameServerPort],
                client.Tamer.Location.MapId,
                client.Tamer.Location.X,
                client.Tamer.Location.Y)
                .Serialize());
        }
    }
}
