using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.Items
{
    public sealed class WarehouseSlotSizePacket : PacketWriter
    {
        private const int PacketNumber = 3925;

        public WarehouseSlotSizePacket(int slotSize)
        {
            Type(PacketNumber);
            WriteShort((short)slotSize);
        }
    }
}
