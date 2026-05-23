using DigitalWorldOnline.Commons.Writers;
using System.Net.Sockets;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class TradeRemoveItemPacket : PacketWriter
    {
        private const int PacketNumber = 1531;

        /// <summary>
        /// Party request sent.
        /// </summary>
        /// <param name="TargetHandle">Target tamer handle accept</param>
        /// <param name="tradeSlot"> trade slot</param>
        public TradeRemoveItemPacket(int targetHandle,  byte tradeSlot)
        {
            Type(PacketNumber);
            WriteUShort((ushort)targetHandle);
            WriteByte(tradeSlot);
        }
    }
}
