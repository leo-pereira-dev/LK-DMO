using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class NoOpEventUiSyncPacketProcessor : IGamePacketProcessor
    {
        private readonly ILogger _logger;

        public NoOpEventUiSyncPacketProcessor(ILogger logger)
        {
            _logger = logger;
        }

        public GameServerPacketEnum Type => GameServerPacketEnum.EventUiSync;

        public Task Process(GameClient client, byte[] packetData)
        {
            _logger.Verbose("Ignoring v487 event UI packet 3132 for tamer {TamerId}.", client.TamerId);
            return Task.CompletedTask;
        }
    }
}
