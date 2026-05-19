using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class NoOpBuddyUiSyncPacketProcessor : IGamePacketProcessor
    {
        private readonly ILogger _logger;

        public NoOpBuddyUiSyncPacketProcessor(ILogger logger)
        {
            _logger = logger;
        }

        public GameServerPacketEnum Type => GameServerPacketEnum.BuddyUiSync;

        public Task Process(GameClient client, byte[] packetData)
        {
            _logger.Verbose("Ignoring v487 buddy UI packet 2404 for tamer {TamerId}.", client.TamerId);
            return Task.CompletedTask;
        }
    }
}
