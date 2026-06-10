using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class TamerWalkPacket : PacketWriter
    {
        private const int PacketNumber = 1006;
        private const byte RotationSyncType = 0x7F;

        /// <summary>
        /// Default tamer movimentation packet.
        /// </summary>
        /// <param name="tamer">The tamer that is moving</param>
        public TamerWalkPacket(CharacterModel tamer)
        {
            Type(PacketNumber);
            WriteByte(5);
            WriteShort(1);
            WriteUInt(tamer.GeneralHandler);
            WriteInt(tamer.Location.X);
            WriteInt(tamer.Location.Y);
            WriteByte(RotationSyncType);
            WriteShort(1);
            WriteUInt(tamer.GeneralHandler);
            WriteFloat(tamer.Location.Z);
            WriteByte(0);
        }
        public TamerWalkPacket(int X,int Y, uint Handle, float rotation = 0)
        {
            Type(PacketNumber);
            WriteByte(5);
            WriteShort(1);
            WriteUInt(Handle);
            WriteInt(X);
            WriteInt(Y);
            WriteByte(RotationSyncType);
            WriteShort(1);
            WriteUInt(Handle);
            WriteFloat(rotation);
            WriteByte(0);
        }
    }
}
