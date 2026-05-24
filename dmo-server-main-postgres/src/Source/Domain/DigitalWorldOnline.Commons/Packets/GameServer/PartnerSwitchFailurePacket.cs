using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class PartnerSwitchFailurePacket : PacketWriter
    {
        private const int PacketNumber = 3210;

        /// <summary>
        /// Releases the client partner-switch sync state when the server rejects the switch.
        /// </summary>
        public PartnerSwitchFailurePacket()
        {
            Type(PacketNumber);
        }
    }
}
