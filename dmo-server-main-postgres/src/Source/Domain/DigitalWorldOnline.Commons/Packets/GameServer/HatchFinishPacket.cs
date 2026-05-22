using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class HatchFinishPacket : PacketWriter
    {
        private const int PacketNumber = 1038;

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
            WriteInt(newDigimon.HP);
            WriteInt(newDigimon.DS);
            WriteInt(0); //28
            WriteInt(0);
            WriteInt(newDigimon.EV);
            WriteInt(newDigimon.CC);
            WriteInt(newDigimon.MS);
            WriteInt(newDigimon.AS);
            WriteInt(newDigimon.AR);
            WriteInt(newDigimon.HT);
            WriteInt(0);
            WriteInt(0);
            WriteInt(0);
            WriteInt(newDigimon.BL);
            WriteByte((byte)newDigimon.HatchGrade);
            WriteInt(newDigimon.BaseType);

            WriteByte((byte)newDigimon.Evolutions.Count);
            for (int i = 0; i < newDigimon.Evolutions.Count; i++)
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
