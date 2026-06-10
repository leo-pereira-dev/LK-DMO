using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class DigimonWalkPacket : PacketWriter
    {
        private const int PacketNumber = 1006;
        private const byte RotationSyncType = 0x7F;

        /// <summary>
        /// Default digimon movimentation packet.
        /// </summary>
        /// <param name="digimon">The digimon that is moving</param>
        public DigimonWalkPacket(DigimonModel digimon)
        {
            Type(PacketNumber);
            WriteByte(6);
            WriteShort(1);
            WriteUInt(digimon.GeneralHandler);
            WriteInt(digimon.Location.X);
            WriteInt(digimon.Location.Y);
            WriteByte(RotationSyncType);
            WriteShort(1);
            WriteUInt(digimon.GeneralHandler);
            WriteFloat(digimon.Location.Z);
            WriteByte(0);
        }
        public DigimonWalkPacket(int X, int Y,uint Handler, float rotation = 0)
        {
            Type(PacketNumber);
            WriteByte(6);
            WriteShort(1);
            WriteUInt(Handler);
            WriteInt(X);
            WriteInt(Y);
            WriteByte(RotationSyncType);
            WriteShort(1);
            WriteUInt(Handler);
            WriteFloat(rotation);
            WriteByte(0);
        }
    }
}
