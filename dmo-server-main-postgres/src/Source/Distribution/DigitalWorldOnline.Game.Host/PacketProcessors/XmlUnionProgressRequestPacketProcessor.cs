using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Xml;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using MediatR;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class XmlUnionProgressRequestPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.XmlUnionProgressRequest;

        private readonly AssetsLoader _assets;
        private readonly ISender _sender;
        private readonly DUnitCollectionService _dUnitCollections;

        public XmlUnionProgressRequestPacketProcessor(AssetsLoader assets, ISender sender, DUnitCollectionService dUnitCollections)
        {
            _assets = assets;
            _sender = sender;
            _dUnitCollections = dUnitCollections;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            client.Tamer.EnsureXmlUnionProgress();
            client.Tamer.XmlUnionProgress.SetProgress(
                client.Tamer.XmlUnionProgress.Level,
                client.Tamer.XmlUnionProgress.CurrentExperience,
                _assets.XmlUnion.GetRequiredExperience(client.Tamer.XmlUnionProgress.Level));

            await _sender.Send(new UpdateCharacterXmlUnionProgressCommand(client.Tamer.XmlUnionProgress));
            client.Send(new XmlUnionProgressPacket(client.Tamer.XmlUnionProgress));

            var dUnitResult = _dUnitCollections.CalculateDUnitCollectionBonus(client.Tamer);
            _dUnitCollections.ApplyBonuses(client.Tamer, dUnitResult);
            client.Send(new UpdateStatusPacket(client.Tamer));
            client.Send(new XmlUnionCollectionInfoPacket(dUnitResult));
        }
    }
}
