using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.Items
{
    public class ReceiveItemPacket : PacketWriter
    {
        private const int PacketNumber = 3936;

        /// <summary>
        /// Receives and highlights a new item in the target inventory.
        /// </summary>
        /// <param name="item">The item received.</param>
        /// <param name="inventoryType">The target inventory enumeration.</param>
        /// <param name="deleteIndex">The gift-storage index to delete after receiving, when applicable.</param>
        public ReceiveItemPacket(ItemModel item, InventoryTypeEnum inventoryType, ushort deleteIndex = 0)
        {
            Type(PacketNumber);
            WriteByte((byte)inventoryType);
            WriteByte((byte)item.Slot);
            WriteUInt((uint)item.ItemId);
            WriteShort((short)item.Amount);
            WriteByte(item.Power);

            if (item.RemainingMinutes() == 4294967280)
            {
                WriteUInt(item.RemainingMinutes());
            }
            else
            {
                var endTime = UtilitiesFunctions.RemainingTimeMinutes((int)item.RemainingMinutes());
                WriteInt(endTime);
            }

            WriteUShort(deleteIndex);
        }
    }
}
