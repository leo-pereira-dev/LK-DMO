using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class XmlUnionRewardClaimResultPacket : PacketWriter
    {
        public const int PacketNumber = 11052;

        public XmlUnionRewardClaimResultPacket(byte rewardIndex, byte result, CharacterXmlUnionProgressModel progress)
        {
            Type(PacketNumber);
            WriteByte(result);
            WriteByte(rewardIndex);
            WriteUShort(progress.ClaimedRewardMask);
            WriteByte(progress.Level);
            WriteUShort(progress.ProgressPercentBasisPoints);
        }
    }
}
