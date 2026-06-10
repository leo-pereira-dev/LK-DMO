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
    public sealed class DungeonClearExitPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.DungeonClearExit;

        private const string GamerServerPublic = "GameServer:PublicAddress";

        private readonly DungeonsServer _dungeonServer;
        private readonly IConfiguration _configuration;
        private readonly ISender _sender;
        private readonly ILogger _logger;
        private readonly OwnerStorageFlushService _ownerStorageFlushService;
        private readonly DungeonExitDestinationResolver _dungeonExitDestinationResolver;

        public DungeonClearExitPacketProcessor(
            DungeonsServer dungeonServer,
            IConfiguration configuration,
            ISender sender,
            ILogger logger,
            OwnerStorageFlushService ownerStorageFlushService,
            DungeonExitDestinationResolver dungeonExitDestinationResolver)
        {
            _dungeonServer = dungeonServer;
            _configuration = configuration;
            _sender = sender;
            _logger = logger;
            _ownerStorageFlushService = ownerStorageFlushService;
            _dungeonExitDestinationResolver = dungeonExitDestinationResolver;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var instance = _dungeonServer.FindMapByTamer(client.TamerId);
            if (instance == null)
            {
                client.Send(new SystemMessagePacket("Dungeon instance not found."));
                return;
            }

            var destination = _dungeonExitDestinationResolver.Resolve(instance, instance.MapId);
            if (destination == null)
            {
                client.Send(new SystemMessagePacket($"Map information not found for dungeon {instance.MapId}."));
                _logger.Warning("Dungeon clear exit failed: no exit region for map {MapId}, tamer {TamerId}.", instance.MapId, client.TamerId);
                return;
            }

            foreach (var targetClient in ResolveInstanceClients(instance))
                await MoveClientAsync(targetClient, destination.MapId, destination.X, destination.Y);
        }

        private static IReadOnlyList<GameClient> ResolveInstanceClients(MapInstance instance)
        {
            return instance.Clients
                .GroupBy(x => x.TamerId)
                .Select(x => x.First())
                .ToList();
        }

        private async Task MoveClientAsync(GameClient client, int mapId, int x, int y)
        {
            await _ownerStorageFlushService.FlushForTransitionAsync(client);
            _dungeonServer.RemoveClient(client);
            client.ClearLastDungeonEntry();

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
