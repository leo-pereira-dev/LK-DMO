using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.Items
{
    public class DirectHatchPacket : PacketWriter
    {
        private const int PacketNumber = 3955;
        private const int MaxClientEvolutionUnits = 17;

        public DirectHatchPacket()
        {
            Type(PacketNumber);
            WriteByte(0);
        }

        public DirectHatchPacket(short itemSlot, DigimonModel newDigimon, uint temporaryHandler, int slot)
        {
            Type(PacketNumber);
            WriteByte(1);
            WriteInt(itemSlot);
            WriteUInt((uint)slot);
            WriteUInt64(BuildClientType(temporaryHandler, newDigimon.BaseType));
            WriteString(newDigimon.Name);
            WriteShort(newDigimon.Size);
            WriteInt64(newDigimon.CurrentExperience * 100);
            WriteInt64(newDigimon.TranscendenceExperience);
            WriteShort(newDigimon.Level);

            WriteInt(newDigimon.HP);
            WriteInt(newDigimon.DS);
            WriteInt(newDigimon.DE);
            WriteInt(newDigimon.AT);
            WriteInt(newDigimon.CurrentHp);
            WriteInt(newDigimon.CurrentDs);
            WriteInt(newDigimon.FS);
            WriteInt(0);
            WriteInt(newDigimon.EV);
            WriteInt(newDigimon.CC);
            WriteInt(newDigimon.MS);
            WriteInt(newDigimon.AS);
            WriteInt(0);
            WriteInt(newDigimon.HT);
            WriteInt(0);
            WriteInt(0);
            WriteInt(0);
            WriteInt(newDigimon.BL);
            WriteByte((byte)newDigimon.HatchGrade);
            WriteInt(newDigimon.BaseType);

            var evolutionCount = System.Math.Min(newDigimon.Evolutions.Count, MaxClientEvolutionUnits);
            WriteByte((byte)evolutionCount);
            for (int i = 0; i < evolutionCount; i++)
            {
                var form = newDigimon.Evolutions[i];
                WriteBytes(form.ToArray());
            }

            WriteShort(newDigimon.Digiclone.CloneLevel);
            WriteShort(newDigimon.Digiclone.ATValue);
            WriteShort(newDigimon.Digiclone.BLValue);
            WriteShort(newDigimon.Digiclone.CTValue);
            WriteShort(0);
            WriteShort(newDigimon.Digiclone.EVValue);
            WriteShort(0);
            WriteShort(newDigimon.Digiclone.HPValue);
            WriteShort(newDigimon.Digiclone.ATLevel);
            WriteShort(newDigimon.Digiclone.BLLevel);
            WriteShort(newDigimon.Digiclone.CTLevel);
            WriteShort(0);
            WriteShort(newDigimon.Digiclone.EVLevel);
            WriteShort(0);
            WriteShort(newDigimon.Digiclone.HPLevel);
        }

        private static ulong BuildClientType(uint handler, int digimonType)
        {
            const int clientTypeShift = 19;
            const ulong clientTypeLowMask = (1UL << clientTypeShift) - 1;

            return ((ulong)digimonType << clientTypeShift) | (handler & clientTypeLowMask);
        }
    }
}
