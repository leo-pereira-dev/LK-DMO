using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class SpiritCraftPacket : PacketWriter
    {
        private const int PacketNumber = 3240;

        /// <summary>
        /// Deletes the target partner.
        /// </summary>
        /// <param name="slot">Target digivice slot</param>
        public SpiritCraftPacket(int slot, long remainingBits, int itemId, int amount = 1)
        {
            Type(PacketNumber);
            WriteByte((byte)slot);
            WriteInt64(remainingBits);
            WriteByte(0);
            WriteByte((byte)amount);
            WriteInt(itemId);
            WriteByte(0);
        }
    }
}
