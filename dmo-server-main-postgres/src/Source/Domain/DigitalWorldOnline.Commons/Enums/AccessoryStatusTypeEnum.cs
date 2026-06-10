namespace DigitalWorldOnline.Commons.Enums
{
    public enum AccessoryStatusTypeEnum
    {
        AT = 1,
        DE = 2,
        HP = 3,
        DS = 4,
        SCD = 5,
        ATT = 6,
        CT = 7,
        CD = 8,
        AS = 9,
        EV = 10,
        BL = 11,
        HT = 12,
        ATRate = 13,
        DERate = 14,
        HPRate = 15,
        DSRate = 16,
        SCDRate = 17,
        FinalDamageRate = 18,

        Data = 101,
        Vacina = 102,
        Virus = 103,
        Unknown = 104,
        Ice = 105,
        Water = 106,
        Fire = 107,
        Earth = 108,
        Wind = 109,
        Wood = 110,
        Light = 111,
        Dark = 112,
        Thunder = 113,
        Steel = 114,

        ATVariant1 = 1001,
        DEVariant1 = 1002,
        HPVariant1 = 1003,
        DSVariant1 = 1004,
        SCDVariant1 = 1005,
        ATTVariant1 = 1006,
        CTVariant1 = 1007,
        CDVariant1 = 1008,
        EVVariant1 = 1010,
        BLVariant1 = 1011,
        HTVariant1 = 1012,
        ATRateVariant1 = 1013,
        DERateVariant1 = 1014,
        HPRateVariant1 = 1015,
        DSRateVariant1 = 1016,
        SCDRateVariant1 = 1017,
        FinalDamageRateVariant1 = 1018,

        ATVariant2 = 2001,
        DEVariant2 = 2002,
        HPVariant2 = 2003,
        DSVariant2 = 2004,
        SCDVariant2 = 2005,
        ATTVariant2 = 2006,
        CDVariant2 = 2008,
        EVVariant2 = 2010,
        BLVariant2 = 2011,
        ATRateVariant2 = 2013,
        DERateVariant2 = 2014,
        HPRateVariant2 = 2015,
        DSRateVariant2 = 2016,
        SCDRateVariant2 = 2017,
        FinalDamageRateVariant2 = 2018,

        ATVariant3 = 3001,
        DEVariant3 = 3002,
        HPVariant3 = 3003,
        DSVariant3 = 3004,
        SCDVariant3 = 3005,
        ATTVariant3 = 3006,
        CDVariant3 = 3008,
        EVVariant3 = 3010,
        BLVariant3 = 3011,
        ATRateVariant3 = 3013,
        DERateVariant3 = 3014,
        HPRateVariant3 = 3015,
        DSRateVariant3 = 3016,
        SCDRateVariant3 = 3017,
        FinalDamageRateVariant3 = 3018,
    }

    public static class AccessoryStatusTypeEnumExtensions
    {
        public static AccessoryStatusTypeEnum NormalizeAccessoryStatus(this AccessoryStatusTypeEnum type)
        {
            var rawType = (int)type;

            if (rawType >= 1000 && rawType < 4000)
            {
                var baseType = rawType % 1000;
                if ((baseType >= (int)AccessoryStatusTypeEnum.AT && baseType <= (int)AccessoryStatusTypeEnum.FinalDamageRate) ||
                    (baseType >= (int)AccessoryStatusTypeEnum.Data && baseType <= (int)AccessoryStatusTypeEnum.Steel))
                    return (AccessoryStatusTypeEnum)baseType;
            }

            return type;
        }

        public static bool IsAttributeOrElementDamage(this AccessoryStatusTypeEnum type)
        {
            var baseType = type.NormalizeAccessoryStatus();
            return baseType >= AccessoryStatusTypeEnum.Data && baseType <= AccessoryStatusTypeEnum.Steel;
        }

        public static bool IsRatioStatus(this AccessoryStatusTypeEnum type)
        {
            var baseType = type.NormalizeAccessoryStatus();
            return baseType >= AccessoryStatusTypeEnum.ATRate && baseType <= AccessoryStatusTypeEnum.FinalDamageRate;
        }

        public static bool UsesBaseValueScaling(this AccessoryStatusTypeEnum type)
        {
            var baseType = type.NormalizeAccessoryStatus();
            return baseType == AccessoryStatusTypeEnum.AS || baseType.IsRatioStatus();
        }
    }
}
