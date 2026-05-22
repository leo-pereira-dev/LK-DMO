using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public sealed class DigimonArchiveIncubatorListPacket : PacketWriter
    {
        private const int PacketNumber = 3226;
        private const int MaxIncubatorSlots = 3;

        public DigimonArchiveIncubatorListPacket()
        {
            Type(PacketNumber);

            for (var i = 0; i < MaxIncubatorSlots; i++)
            {
                WriteInt(0); // Archive slot id, 0 means empty for this client UI.
                WriteInt(0); // Registered item id.
                WriteInt(0); // Remaining seconds.
            }
        }
    }
}
