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
            WriteUShort(character.Partner.CC);
            WriteFloat(character.Partner.AS / 1000f);
            WriteUShort(character.Partner.EV);
            WriteInt(character.Partner.HT);
            WriteUShort(character.Partner.AR);
            WriteUShort(character.Partner.BL);

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

            if (character.Partner.CC >= short.MaxValue ||
                character.Partner.EV >= short.MaxValue ||
                character.Partner.AR >= short.MaxValue ||
                character.Partner.BL >= short.MaxValue ||
                detailCT > short.MaxValue ||
                detailEV > short.MaxValue ||
                detailBL > short.MaxValue)
            {
                Console.WriteLine(
                    $"[UpdateStatusPacket:UnsignedStat] tamer={character.Id} partner={character.Partner.Id} partnerCT={character.Partner.CC} partnerEV={character.Partner.EV} partnerAR={character.Partner.AR} partnerBL={character.Partner.BL} detailCT={detailCT} detailEV={detailEV} detailBL={detailBL} shortMax={short.MaxValue} u2Max={ushort.MaxValue}");
                Console.WriteLine(
                    $"[UpdateStatusPacket:CTBreakdown] tamer={character.Id} tamerName={character.Name} partner={character.Partner.Id} partnerName={character.Partner.Name} {character.Partner.CriticalStatusDebugBreakdown()}");
            }

            if (partnerRawAttack != partnerAttack || partnerAttack >= 100000 || detailAT >= 100000)
            {
                Console.WriteLine(
                    $"[UpdateStatusPacket:PartnerATBreakdown] tamer={character.Id} tamerName={character.Name} partner={character.Partner.Id} partnerName={character.Partner.Name} baseType={character.Partner.BaseType} currentType={character.Partner.CurrentType} level={character.Partner.Level} size={character.Partner.Size} atDiff={(partnerRawAttack - partnerAttack)} {character.Partner.AttackStatusDebugBreakdown()}");
            }
        }
    }
}
