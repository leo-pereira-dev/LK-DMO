using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class HatchFinishPacket : PacketWriter
    {
        private const int PacketNumber = 1038;
        private const int MaxClientEvolutionUnits = 17;

        /// <summary>
        /// Finishes the hatch of a new partner.
        /// </summary>
        /// <param name="newDigimon">Partner info</param>
        /// <param name="slot">Digivice slot</param>
        public HatchFinishPacket(DigimonModel newDigimon, uint temporaryHandler, int slot)
        {
            Type(PacketNumber);
            WriteInt(slot);
            WriteUInt(temporaryHandler);
            WriteInt(newDigimon.BaseType);
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
            WriteShort(0); // AS
            WriteShort(newDigimon.Digiclone.EVValue);
            WriteShort(0); // HT
            WriteShort(newDigimon.Digiclone.HPValue);
            WriteShort(newDigimon.Digiclone.ATLevel);
            WriteShort(newDigimon.Digiclone.BLLevel);
            WriteShort(newDigimon.Digiclone.CTLevel);
            WriteShort(0); // AS
            WriteShort(newDigimon.Digiclone.EVLevel);
            WriteShort(0); // HT
            WriteShort(newDigimon.Digiclone.HPLevel);
        }
    }
}
