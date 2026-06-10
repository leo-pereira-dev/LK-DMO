using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.GameHost;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class PartnerRideModeStartPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.PartnerRideModeStart;

        private readonly MapServer _mapServer;
        private readonly AssetsLoader _assets;
        private readonly MapBinLoader _mapBin;
        private readonly ILogger _logger;

        public PartnerRideModeStartPacketProcessor(
            MapServer mapServer,
            AssetsLoader assets,
            MapBinLoader mapBin,
            ILogger logger)
        {
            _mapServer = mapServer;
            _assets = assets;
            _mapBin = mapBin;
            _logger = logger;
        }

        public Task Process(GameClient client, byte[] packetData)
        {
            if (LimitEvolutionRegionRule.ShouldStopRideMode(_assets, _mapBin, client))
            {
                if (client.Tamer.Riding)
                    client.Tamer.StopRideMode();

                client.Send(new UpdateMovementSpeedPacket(client.Tamer));
                client.Send(new RideModeStopPacket(client.Tamer.GeneralHandler, client.Partner.GeneralHandler));

                _logger.Information(
                    "Ride mode start rejected by limit evolution region. Tamer {TamerId}, partner {PartnerId}, currentType {CurrentType}, map {MapId}, x {X}, y {Y}.",
                    client.TamerId,
                    client.Partner.Id,
                    client.Partner.CurrentType,
                    client.Tamer.Location.MapId,
                    client.Partner.Location.X,
                    client.Partner.Location.Y);

                return Task.CompletedTask;
            }

            client.Tamer.StartRideMode();

            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId,
                new UpdateMovementSpeedPacket(client.Tamer).Serialize());

            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId,
                new RideModeStartPacket(client.Tamer.GeneralHandler, client.Partner.GeneralHandler).Serialize());

            _logger.Verbose($"Character {client.TamerId} started riding mode with " +
                $"{client.Partner.Id} ({client.Partner.CurrentType}).");

            return Task.CompletedTask;
        }
    }
}
