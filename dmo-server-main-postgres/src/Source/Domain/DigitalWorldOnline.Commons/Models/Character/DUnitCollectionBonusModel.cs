using DigitalWorldOnline.Commons.Enums.ClientEnums;

namespace DigitalWorldOnline.Commons.Models.Character
{
    public sealed class DUnitCollectionBonusModel
    {
        public int HP { get; private set; }
        public int DS { get; private set; }
        public int AT { get; private set; }
        public int DE { get; private set; }
        public int HT { get; private set; }
        public int CT { get; private set; }
        public int EV { get; private set; }
        public int BL { get; private set; }
        public int EXP { get; private set; }
        public int SCD { get; private set; }
        public int Basic { get; private set; }
        public int Vaccine { get; private set; }
        public int Data { get; private set; }
        public int Virus { get; private set; }
        public int Unknown { get; private set; }
        public int Fire { get; private set; }
        public int Water { get; private set; }
        public int Ice { get; private set; }
        public int Wind { get; private set; }
        public int Thunder { get; private set; }
        public int Light { get; private set; }
        public int Dark { get; private set; }
        public int Land { get; private set; }
        public int Wood { get; private set; }
        public int Steel { get; private set; }

        public void Reset()
        {
            HP = DS = AT = DE = HT = CT = EV = BL = EXP = SCD = Basic = 0;
            Vaccine = Data = Virus = Unknown = Fire = Water = Ice = Wind = 0;
            Thunder = Light = Dark = Land = Wood = Steel = 0;
        }

        public void Set(
            int hp,
            int ds,
            int at,
            int de,
            int ht,
            int ct,
            int ev,
            int bl,
            int exp,
            int scd,
            int basic,
            int vaccine,
            int data,
            int virus,
            int unknown,
            int fire,
            int water,
            int ice,
            int wind,
            int thunder,
            int light,
            int dark,
            int land,
            int wood,
            int steel)
        {
            HP = hp;
            DS = ds;
            AT = at;
            DE = de;
            HT = ht;
            CT = ct;
            EV = ev;
            BL = bl;
            EXP = exp;
            SCD = scd;
            Basic = basic;
            Vaccine = vaccine;
            Data = data;
            Virus = virus;
            Unknown = unknown;
            Fire = fire;
            Water = water;
            Ice = ice;
            Wind = wind;
            Thunder = thunder;
            Light = light;
            Dark = dark;
            Land = land;
            Wood = wood;
            Steel = steel;
        }

        public int AttributeSkillDamage(DigimonAttributeEnum attribute, DigimonElementEnum element)
        {
            var value = Basic;

            value += attribute switch
            {
                DigimonAttributeEnum.Vaccine => Vaccine,
                DigimonAttributeEnum.Data => Data,
                DigimonAttributeEnum.Virus => Virus,
                DigimonAttributeEnum.Unknown => Unknown,
                _ => 0
            };

            value += element switch
            {
                DigimonElementEnum.Fire => Fire,
                DigimonElementEnum.Water => Water,
                DigimonElementEnum.Ice => Ice,
                DigimonElementEnum.Wind => Wind,
                DigimonElementEnum.Thunder => Thunder,
                DigimonElementEnum.Light => Light,
                DigimonElementEnum.Dark => Dark,
                DigimonElementEnum.Land => Land,
                DigimonElementEnum.Wood => Wood,
                DigimonElementEnum.Steel => Steel,
                _ => 0
            };

            return value;
        }
    }
}
