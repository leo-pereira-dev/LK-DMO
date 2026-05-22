using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class XmlUnionProgressPacket : PacketWriter
    {
        public const int PacketNumber = 11050;

        public XmlUnionProgressPacket(CharacterXmlUnionProgressModel progress)
        {
            Type(PacketNumber);
            WriteByte(progress.Level);
            WriteInt(progress.CurrentExperience);
            WriteInt(progress.RequiredExperience);
            WriteUShort(progress.ProgressPercentBasisPoints);
            WriteUShort(progress.ClaimedRewardMask);
        }
    }
}
