using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class QuestCanceledPacket : PacketWriter
    {
        private const int PacketNumber = 11008;

        public QuestCanceledPacket()
        {
            Type(PacketNumber);
            WriteShort(0);
        }
    }
}
