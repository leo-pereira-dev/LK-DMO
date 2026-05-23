using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class ItemSocketIdentifyPacket : PacketWriter
    {
        private const int PacketNumber = 3929;

        public ItemSocketIdentifyPacket(ItemModel item, long money)
        {
            Type(PacketNumber);
            WriteByte(item.Power);
            WriteInt64(money);
        }
    }
}
