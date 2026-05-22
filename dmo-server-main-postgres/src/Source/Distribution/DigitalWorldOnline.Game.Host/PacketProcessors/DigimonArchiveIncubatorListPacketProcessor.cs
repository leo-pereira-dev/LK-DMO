using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class DigimonArchiveIncubatorListPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.DigimonArchiveIncubatorList;

        private readonly ILogger _logger;

        public DigimonArchiveIncubatorListPacketProcessor(ILogger logger)
        {
            _logger = logger;
        }

        public Task Process(GameClient client, byte[] packetData)
        {
            _logger.Verbose("Loading digimon archive academy slots for tamer {TamerId}.", client.TamerId);
            client.Send(new DigimonArchiveIncubatorListPacket());

            return Task.CompletedTask;
        }
    }
}
