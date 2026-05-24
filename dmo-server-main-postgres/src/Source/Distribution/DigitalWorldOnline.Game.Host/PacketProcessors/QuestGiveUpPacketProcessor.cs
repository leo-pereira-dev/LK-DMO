using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class QuestGiveUpPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.QuestGiveUp;

        private readonly ILogger _logger;
        private readonly ISender _sender;

        public QuestGiveUpPacketProcessor(
            ILogger logger,
            ISender sender)
        {
            _logger = logger;
            _sender = sender;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var payloadLength = (packet.Length - 2) - (int)packet.Packet.Position;
            if (payloadLength < 2)
            {
                _logger.Warning(
                    "Quest give up ignored: invalid payload length {Payload} for tamer {TamerId}.",
                    payloadLength,
                    client.TamerId);
                return;
            }

            var questId = packet.ReadShort();
            var activeBefore = FormatActiveQuests(client);

            _logger.Information(
                "Quest give up requested: tamer {TamerId}, quest {QuestId}, payload={Payload}, activeBefore=[{ActiveBefore}].",
                client.TamerId,
                questId,
                payloadLength,
                activeBefore);

            var id = client.Tamer.Progress.RemoveQuest(questId);

            await _sender.Send(new RemoveActiveQuestCommand(id));

            client.Send(new QuestCanceledPacket());

            _logger.Information(
                "Quest give up completed: tamer {TamerId}, quest {QuestId}, removedId={ProgressQuestId}, activeAfter=[{ActiveAfter}].",
                client.TamerId,
                questId,
                id,
                FormatActiveQuests(client));
        }

        private static string FormatActiveQuests(GameClient client)
        {
            return string.Join(", ", client.Tamer.Progress.InProgressQuestData
                .Select(x => $"{x.QuestId}:{x.FirstCondition}/{x.SecondCondition}/{x.ThirdCondition}/{x.FourthCondition}/{x.FifthCondition}"));
        }
    }
}
