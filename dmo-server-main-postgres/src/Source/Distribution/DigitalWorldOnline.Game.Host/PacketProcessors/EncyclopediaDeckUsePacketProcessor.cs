using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class EncyclopediaDeckUsePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.EncyclopediaDeckUse;

        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly DigimonBookBinLoader _digimonBook;

        public EncyclopediaDeckUsePacketProcessor(ILogger logger, ISender sender, DigimonBookBinLoader digimonBook)
        {
            _logger = logger;
            _sender = sender;
            _digimonBook = digimonBook;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var deckId = Math.Max(0, packet.ReadInt());

            client.Tamer.SetEncyclopediaDeck(deckId);
            await _sender.Send(new UpdateCharacterEncyclopediaDeckCommand(client.TamerId, deckId));
            client.Send(new EncyclopediaDeckStatusPacket(client.Tamer));
            client.Send(new UpdateStatusPacket(client.Tamer));

            var deckEffects = "none";
            if (deckId > 0 && _digimonBook.Data.DecksByGroupId.TryGetValue(deckId, out var deck))
            {
                deckEffects = string.Join(", ", deck.Effects.Select(effect =>
                    $"cond={effect.Condition}/atk={effect.AttackType}/opt={effect.Option}/val={effect.Value}/prob={effect.Probability}/time={effect.Time}"));
            }

            _logger.Information(
                "[EncyclopediaDeckUse] Character {TamerId} persisted encyclopedia deck {DeckId} enabled={Enabled}; effects={Effects}.",
                client.TamerId,
                deckId,
                deckId != 0,
                deckEffects);
        }
    }
}
