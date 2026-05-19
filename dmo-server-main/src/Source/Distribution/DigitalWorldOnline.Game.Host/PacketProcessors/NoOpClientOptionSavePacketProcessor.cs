using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class NoOpClientOptionSavePacketProcessor : IGamePacketProcessor
    {
        private readonly ILogger _logger;

        public NoOpClientOptionSavePacketProcessor(ILogger logger)
        {
            _logger = logger;
        }

        public GameServerPacketEnum Type => GameServerPacketEnum.ClientOptionSave;

        public Task Process(GameClient client, byte[] packetData)
        {
            _logger.Verbose("Ignoring v487 client option packet 1076 for tamer {TamerId}.", client.TamerId);
            return Task.CompletedTask;
        }
    }
}
