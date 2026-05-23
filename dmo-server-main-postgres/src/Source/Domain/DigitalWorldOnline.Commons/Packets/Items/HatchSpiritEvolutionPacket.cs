using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.Items
{
    public class HatchSpiritEvolutionPacket : PacketWriter
    {
        private const int PacketNumber = 3239;

        public HatchSpiritEvolutionPacket(int targetType, long currencyBits,List<ExtraEvolutionMaterialAssetModel> Material,List<ExtraEvolutionRequiredAssetModel> Required)
        {
            Type(PacketNumber);
            WriteInt(targetType);
            WriteInt64(currencyBits);

            foreach (var material in Material)
            {
                WriteByte((byte)material.Amount);
                WriteInt(material.ItemId);
  
            }

            foreach (var material in Required)
            {
                WriteByte((byte)material.Amount);
                WriteInt(material.ItemId);
            }

            WriteByte(0);
        }
    }
}
