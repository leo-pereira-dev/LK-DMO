using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public class UpdateStatusPacket : PacketWriter
    {
        private const int PacketNumber = 1043;

        /// <summary>
        /// Updates the tamer and partner current status.
        /// </summary>
        /// <param name="character">The tamer to be updated</param>
        public UpdateStatusPacket(CharacterModel character)
        {
            Type(PacketNumber);
            WriteInt(character.HP);
            WriteInt(character.DS);
            WriteInt(character.CurrentHp);
            WriteInt(character.CurrentDs);
            WriteShort(character.AT);
            WriteShort(character.DE);
            WriteShort((short)character.MS);
            WriteInt(character.Partner.HP);
            WriteInt(character.Partner.DS);
            WriteInt(character.Partner.CurrentHp);
            WriteInt(character.Partner.CurrentDs);
            WriteShort(character.Partner.FS);
            WriteShort(character.Partner.AT);
            WriteShort(character.Partner.DE);
            WriteShort(character.Partner.CC);
            WriteShort((short)character.Partner.AS);
            WriteShort(character.Partner.EV);
            WriteShort(character.Partner.HT);
            WriteShort(character.Partner.AR);
            WriteShort(character.Partner.BL);

            WriteShort(character.Partner.Digiclone.CloneLevel);
            WriteShort(character.Partner.Digiclone.ATValue);
            WriteShort(character.Partner.Digiclone.BLValue);
            WriteShort(character.Partner.Digiclone.CTValue);
            WriteShort(0); //AS
            WriteShort(character.Partner.Digiclone.EVValue);
            WriteShort(0); //HT
            WriteShort(character.Partner.Digiclone.HPValue);

            WriteShort(character.Partner.Digiclone.ATLevel);
            WriteShort(character.Partner.Digiclone.BLLevel);
            WriteShort(character.Partner.Digiclone.CTLevel);
            WriteShort(0); //AS
            WriteShort(character.Partner.Digiclone.EVLevel);
            WriteShort(0); //HT
            WriteShort(character.Partner.Digiclone.HPLevel);

            var detailHP = character.Partner.TamerDetailHP;
            var detailDS = character.Partner.TamerDetailDS;
            var detailAT = character.Partner.TamerDetailAT;
            var detailAS = character.Partner.TamerDetailAS;
            var detailCT = character.Partner.TamerDetailCT;
            var detailHT = character.Partner.TamerDetailHT;
            var detailSCD = character.Partner.TamerDetailSCD;
            var detailCD = character.Partner.TamerDetailCD;
            var detailSD = character.Partner.TamerDetailSD;
            var detailBaseDamage = character.Partner.TamerDetailBaseDamage;
            var detailDE = character.Partner.TamerDetailDE;
            var detailBL = character.Partner.TamerDetailBL;
            var detailEV = character.Partner.TamerDetailEV;

            WriteInt(detailHP);
            WriteInt(detailDS);
            WriteInt(detailAT);
            WriteInt(detailAS);
            WriteInt(detailCT);
            WriteInt(detailHT);
            WriteInt(detailSCD);
            WriteInt(detailCD);
            WriteInt(detailSD);
            WriteInt(detailBaseDamage);
            WriteInt(detailDE);
            WriteInt(detailBL);
            WriteInt(detailEV);

            Console.WriteLine(
                $"[UpdateStatusPacket:DetailInfo] tamer={character.Id} partner={character.Partner.Id} HP={detailHP} DS={detailDS} AT={detailAT} AS={detailAS} CT={detailCT} HT={detailHT} SCD={detailSCD} CD={detailCD} SD={detailSD} BASE={detailBaseDamage} DE={detailDE} BL={detailBL} EV={detailEV}");
        }
    }
}
