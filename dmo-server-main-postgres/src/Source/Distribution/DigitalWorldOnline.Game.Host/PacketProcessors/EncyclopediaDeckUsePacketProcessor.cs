using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class EncyclopediaDeckUsePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.EncyclopediaDeckUse;

        private readonly ILogger _logger;

        public EncyclopediaDeckUsePacketProcessor(ILogger logger)
        {
            _logger = logger;
        }

        public Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var deckId = packet.ReadInt();

            _logger.Information(
                "Character {TamerId} requested encyclopedia deck {DeckId}. Client applies the passive deck state locally.",
                client.TamerId,
                deckId);

            return Task.CompletedTask;
        }
    }
}
