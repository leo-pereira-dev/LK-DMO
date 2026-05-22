using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using MediatR;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class XmlUnionRewardClaimPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.XmlUnionRewardClaim;

        private readonly AssetsLoader _assets;
        private readonly ISender _sender;

        public XmlUnionRewardClaimPacketProcessor(AssetsLoader assets, ISender sender)
        {
            _assets = assets;
            _sender = sender;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var rewardIndex = packet.ReadByte();

            client.Tamer.EnsureXmlUnionProgress();

            var rewardExists = _assets.XmlUnion.RewardTiers.Any(x => x.Index == rewardIndex);
            if (!rewardExists)
            {
                client.Send(new XmlUnionRewardClaimResultPacket(rewardIndex, 2, client.Tamer.XmlUnionProgress));
                return;
            }

            var claimed = client.Tamer.XmlUnionProgress.ClaimReward(rewardIndex);
            if (!claimed)
            {
                client.Send(new XmlUnionRewardClaimResultPacket(rewardIndex, 1, client.Tamer.XmlUnionProgress));
                return;
            }

            await _sender.Send(new UpdateCharacterXmlUnionProgressCommand(client.Tamer.XmlUnionProgress));
            client.Send(new XmlUnionRewardClaimResultPacket(rewardIndex, 0, client.Tamer.XmlUnionProgress));
            client.Send(new XmlUnionProgressPacket(client.Tamer.XmlUnionProgress));
        }
    }
}
