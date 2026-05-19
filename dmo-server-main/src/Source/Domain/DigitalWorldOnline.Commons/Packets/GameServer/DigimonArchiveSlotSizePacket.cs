using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public sealed class DigimonArchiveSlotSizePacket : PacketWriter
    {
        private const int PacketNumber = 3205;

        public DigimonArchiveSlotSizePacket(int slotSize)
        {
            Type(PacketNumber);
            WriteInt(slotSize);
        }
    }
}
