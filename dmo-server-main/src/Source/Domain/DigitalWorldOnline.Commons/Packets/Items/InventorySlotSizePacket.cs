using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.Items
{
    public sealed class InventorySlotSizePacket : PacketWriter
    {
        private const int PacketNumber = 3924;

        public InventorySlotSizePacket(int slotSize)
        {
            Type(PacketNumber);
            WriteShort((short)slotSize);
        }
    }
}
