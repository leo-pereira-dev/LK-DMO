using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class DigimonTranscendenceSuccessPacket : PacketWriter
    {
        private const int PacketNumber = 16040;


        public DigimonTranscendenceSuccessPacket(int result)
        {
            Type(PacketNumber);
            WriteInt(result);
        }

        public DigimonTranscendenceSuccessPacket(int result, byte targetSlot, DigimonHatchGradeEnum scale, long transcendenceCost, long tamerMoney, long transcendenceExperience)
        {
            Type(PacketNumber);
            WriteInt(result);
            WriteByte(targetSlot);
            WriteByte((byte)scale);
            WriteInt64(transcendenceCost);
            WriteInt64(tamerMoney);
            WriteInt64(transcendenceExperience);
        }
    }
}
