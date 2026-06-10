using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Map;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.MapServer;
using DigitalWorldOnline.Game.Configuration;
using DigitalWorldOnline.Game.Services;
using DigitalWorldOnline.GameHost;
using MediatR;
using Microsoft.Extensions.Configuration;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class DungeonReEnterPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.DungeonReEnter;

        private const string GamerServerPublic = "GameServer:PublicAddress";

        private readonly DungeonsServer _dungeonServer;
        private readonly IConfiguration _configuration;
        private readonly ISender _sender;
        private readonly ILogger _logger;
        private readonly OwnerStorageFlushService _ownerStorageFlushService;
        private readonly MapBinLoader _mapBins;

        public DungeonReEnterPacketProcessor(
            DungeonsServer dungeonServer,
            IConfiguration configuration,
            ISender sender,
            ILogger logger,
            OwnerStorageFlushService ownerStorageFlushService,
            MapBinLoader mapBins)
        {
            _dungeonServer = dungeonServer;
            _configuration = configuration;
            _sender = sender;
            _logger = logger;
            _ownerStorageFlushService = ownerStorageFlushService;
            _mapBins = mapBins;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var instance = _dungeonServer.FindMapByTamer(client.TamerId);
            if (instance == null)
            {
                client.Send(new SystemMessagePacket("Dungeon instance not found."));
                return;
            }

            var destination = ResolveFirstRegion(instance.MapId);
            if (destination == null)
            {
                client.Send(new SystemMessagePacket($"Map information not found for dungeon {instance.MapId}."));
                _logger.Warning("Dungeon re-enter failed: no start region for map {MapId}, tamer {TamerId}.", instance.MapId, client.TamerId);
                return;
            }

            var entryPortalId = instance.DungeonEntryPortalId;
            var entranceMapId = instance.DungeonEntranceMapId;
            var instanceClients = ResolveInstanceClients(instance);

            // Re-enter must create a fresh dungeon instance. If the old clear
            // instance stays registered while party members reconnect, they can
            // be routed back into the already-cleared copy.
            await _dungeonServer.CleanMap((int)instance.DungeonId);

            foreach (var targetClient in instanceClients)
                await MoveClientAsync(targetClient, destination.Value.MapId, destination.Value.X, destination.Value.Y, entryPortalId, entranceMapId);
        }

        private (int MapId, int X, int Y)? ResolveFirstRegion(int mapId)
        {
            if (!_mapBins.Data.RegionsByMapId.TryGetValue(mapId, out var regions) || regions.Count == 0)
                return null;

            var destination = regions[0];
            return (mapId, destination.CenterX, destination.CenterY);
        }

        private static IReadOnlyList<GameClient> ResolveInstanceClients(MapInstance instance)
        {
            return instance.Clients
                .GroupBy(x => x.TamerId)
                .Select(x => x.First())
                .ToList();
        }

        private async Task MoveClientAsync(GameClient client, int mapId, int x, int y, int entryPortalId, int entranceMapId)
        {
            await _ownerStorageFlushService.FlushForTransitionAsync(client);
            _dungeonServer.RemoveClient(client);
            client.SetLastDungeonEntry(entryPortalId, entranceMapId);

            client.Tamer.NewLocation(mapId, x, y);
            await _sender.Send(new UpdateCharacterLocationCommand(client.Tamer.Location));

            client.Tamer.Partner.NewLocation(mapId, x, y);
            await _sender.Send(new UpdateDigimonLocationCommand(client.Tamer.Partner.Location));

            client.Tamer.UpdateState(CharacterStateEnum.Loading);
            await _sender.Send(new UpdateCharacterStateCommand(client.TamerId, CharacterStateEnum.Loading));

            client.SetGameQuit(false);
            client.Send(new MapSwapPacket(
                _configuration[GamerServerPublic],
                _configuration.GetPublicGameServerPort(),
                client.Tamer.Location.MapId,
                client.Tamer.Location.X,
                client.Tamer.Location.Y).Serialize());
        }
    }
}
