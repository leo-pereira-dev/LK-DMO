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
            var partnerAttack = character.Partner.AT;
            var partnerRawAttack = character.Partner.RawAT;

            Type(PacketNumber);
            WriteInt(character.HP);
            WriteInt(character.DS);
            WriteInt(character.CurrentHp);
            WriteInt(character.CurrentDs);
            WriteInt(character.AT);
            WriteInt(character.DE);
            WriteShort((short)character.MS);
            WriteInt(character.Partner.HP);
            WriteInt(character.Partner.DS);
            WriteInt(character.Partner.CurrentHp);
            WriteInt(character.Partner.CurrentDs);
            WriteShort(character.Partner.FS);
            WriteInt(partnerAttack);
            WriteInt(character.Partner.DE);
            WriteShort(character.Partner.CC);
            WriteFloat(character.Partner.AS / 1000f);
            WriteShort(character.Partner.EV);
            WriteInt(character.Partner.HT);
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
                $"[UpdateStatusPacket:Main] tamer={character.Id} tamerHP={character.HP} tamerDS={character.DS} tamerCurrHP={character.CurrentHp} tamerCurrDS={character.CurrentDs} tamerAT={character.AT} tamerDE={character.DE} tamerMS={character.MS} partner={character.Partner.Id} partnerName={character.Partner.Name} partnerHP={character.Partner.HP} partnerDS={character.Partner.DS} partnerCurrHP={character.Partner.CurrentHp} partnerCurrDS={character.Partner.CurrentDs} partnerAT={partnerAttack} partnerRawAT={partnerRawAttack} partnerDE={character.Partner.DE} partnerAS={character.Partner.AS} partnerCT={character.Partner.CC} partnerHT={character.Partner.HT} partnerAR={character.Partner.AR} partnerBL={character.Partner.BL} partnerEV={character.Partner.EV} fs={character.Partner.FS} fsLayout=int32");

            Console.WriteLine(
                $"[UpdateStatusPacket:DetailInfo] tamer={character.Id} partner={character.Partner.Id} detailHP={detailHP} detailDS={detailDS} detailAT={detailAT} detailAS={detailAS} detailCT={detailCT} detailHT={detailHT} detailSCD={detailSCD} detailCD={detailCD} detailSD={detailSD} detailBaseDamage={detailBaseDamage} detailDE={detailDE} detailBL={detailBL} detailEV={detailEV}");

            if (partnerRawAttack != partnerAttack || partnerAttack >= 100000 || detailAT >= 100000)
            {
                Console.WriteLine(
                    $"[UpdateStatusPacket:PartnerATBreakdown] tamer={character.Id} tamerName={character.Name} partner={character.Partner.Id} partnerName={character.Partner.Name} baseType={character.Partner.BaseType} currentType={character.Partner.CurrentType} level={character.Partner.Level} size={character.Partner.Size} atDiff={(partnerRawAttack - partnerAttack)} {character.Partner.AttackStatusDebugBreakdown()}");
            }
        }
    }
}
