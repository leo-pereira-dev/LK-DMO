using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Commons.Writers;
using DigitalWorldOnline.Commons.Enums.ClientEnums;

namespace DigitalWorldOnline.Commons.Packets.GameServer.Combat
{
    public class AccountWarehouseItemRetrievePacket : PacketWriter
    {
        private const int PacketNumber = 3931;

        /// </summary>
        /// <param name="giftStorage">The list of Gift Storage</param>
        public AccountWarehouseItemRetrievePacket(ItemModel item,int wareSlot)
        {
            Type(PacketNumber);
            WriteInt(100);
            WriteInt(0);
            WriteUShort((ushort)wareSlot);
            WriteByte((byte)InventorySlotTypeEnum.TabInven);
            WriteByte((byte)item.Slot);
            WriteInt(item.ItemId);
            WriteInt(item.Amount);
            WriteByte(item.Power);
            if (item.RemainingMinutes() == 4294967280)
            {
                WriteUInt(item.RemainingMinutes());
            }
            else
            {
                WriteInt(UtilitiesFunctions.RemainingTimeMinutes((int)item.RemainingMinutes()));
            }
            WriteInt(0);

        }
    }
}
