using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public sealed class DigimonArchiveMovePacket : PacketWriter
    {
        private const int PacketNumber = 3243;

        public DigimonArchiveMovePacket(int sourceArchiveSlot, int destinationArchiveSlot)
        {
            Type(PacketNumber);
            WriteInt(sourceArchiveSlot + 1000);
            WriteInt(destinationArchiveSlot + 1000);
        }
    }
}
