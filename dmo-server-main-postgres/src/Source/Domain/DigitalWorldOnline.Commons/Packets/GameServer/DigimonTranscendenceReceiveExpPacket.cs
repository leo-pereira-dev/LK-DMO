using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class DigimonTranscendenceReceiveExpPacket : PacketWriter
    {
        private const int PacketNumber = 16039;


        public DigimonTranscendenceReceiveExpPacket(int result)
        {
            Type(PacketNumber);
            WriteInt(result);
        }

        public DigimonTranscendenceReceiveExpPacket(
            AcademyInputType inputType,
            byte targetSlot,
            short digimonCount,
            IReadOnlyCollection<short> targetDeleteSlots,
            IReadOnlyCollection<KeyValuePair<short, ItemModel>> updatedItems,
            short successRate,
            long chargeExp,
            long targetPartnerFinalExp)
        {
            Type(PacketNumber);
            WriteInt(0); // Sucess
            WriteByte((byte)inputType);
            WriteByte((byte)targetSlot);
            WriteShort(digimonCount);

            foreach (var targetToDeleteSlot in targetDeleteSlots)
            {
                WriteShort(targetToDeleteSlot);
            }
            WriteShort((short)updatedItems.Count);
            foreach (var updatedItem in updatedItems)
            {
                WriteShort(updatedItem.Key);
                WriteShort(0); // sItemInfo padding before cItemData on the v487 client.
                WriteBytes(updatedItem.Value.ToArray());
            }

            WriteShort((short)successRate);
            WriteInt64(chargeExp);
            WriteInt64(targetPartnerFinalExp);
        }
    }
}
