using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public sealed class EncyclopediaDeckStatusPacket : PacketWriter
    {
        private const int PacketNumber = 3236;

        public EncyclopediaDeckStatusPacket(CharacterModel character)
        {
            Type(PacketNumber);
            WriteInt(character.Partner.HP);
            WriteUShort((ushort)character.Partner.AS);
        }
    }

    public sealed class EncyclopediaDeckEffectPacket : PacketWriter
    {
        private const int PacketNumber = 3237;

        public EncyclopediaDeckEffectPacket(int effectIndex, int endTimestamp)
        {
            Type(PacketNumber);
            WriteUShort((ushort)effectIndex);
            WriteInt(endTimestamp);
        }
    }
}
