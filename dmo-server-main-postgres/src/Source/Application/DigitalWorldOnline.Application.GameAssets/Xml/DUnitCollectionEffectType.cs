using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;

namespace DigitalWorldOnline.Application.GameAssets.Xml
{
    public enum DUnitCollectionEffectType
    {
        Unknown = 0,
        HP,
        DS,
        AT,
        DE,
        HT,
        CT,
        EV,
        BL,
        EXP,
        SCD,
        Basic,
        Vaccine,
        Data,
        Virus,
        UnknownAttribute,
        Fire,
        Water,
        Ice,
        Wind,
        Thunder,
        Light,
        Dark,
        Land,
        Wood,
        Steel
    }

    public static class DUnitCollectionEffectTypeMapper
    {
        public static bool TryMap(string? value, out DUnitCollectionEffectType effectType)
        {
            effectType = value?.Trim() switch
            {
                "HP" => DUnitCollectionEffectType.HP,
                "DS" => DUnitCollectionEffectType.DS,
                "AT" => DUnitCollectionEffectType.AT,
                "DE" => DUnitCollectionEffectType.DE,
                "HT" => DUnitCollectionEffectType.HT,
                "CT" => DUnitCollectionEffectType.CT,
                "EV" => DUnitCollectionEffectType.EV,
                "BL" => DUnitCollectionEffectType.BL,
                "EXP" => DUnitCollectionEffectType.EXP,
                "SCD" => DUnitCollectionEffectType.SCD,
                "Basic" => DUnitCollectionEffectType.Basic,
                "Vaccine" => DUnitCollectionEffectType.Vaccine,
                "Data" => DUnitCollectionEffectType.Data,
                "Virus" => DUnitCollectionEffectType.Virus,
                "Unknown" => DUnitCollectionEffectType.UnknownAttribute,
                "Fire" => DUnitCollectionEffectType.Fire,
                "Water" => DUnitCollectionEffectType.Water,
                "Ice" => DUnitCollectionEffectType.Ice,
                "Wind" => DUnitCollectionEffectType.Wind,
                "Thunder" => DUnitCollectionEffectType.Thunder,
                "Light" => DUnitCollectionEffectType.Light,
                "Dark" => DUnitCollectionEffectType.Dark,
                "Land" => DUnitCollectionEffectType.Land,
                "Wood" => DUnitCollectionEffectType.Wood,
                "Steel" => DUnitCollectionEffectType.Steel,
                _ => DUnitCollectionEffectType.Unknown
            };

            return effectType != DUnitCollectionEffectType.Unknown;
        }

        public static AccessoryStatusTypeEnum? ToAccessoryType(DUnitCollectionEffectType effectType)
        {
            return effectType switch
            {
                DUnitCollectionEffectType.SCD => AccessoryStatusTypeEnum.SCD,
                DUnitCollectionEffectType.Data => AccessoryStatusTypeEnum.Data,
                DUnitCollectionEffectType.Vaccine => AccessoryStatusTypeEnum.Vacina,
                DUnitCollectionEffectType.Virus => AccessoryStatusTypeEnum.Virus,
                DUnitCollectionEffectType.UnknownAttribute => AccessoryStatusTypeEnum.Unknown,
                DUnitCollectionEffectType.Ice => AccessoryStatusTypeEnum.Ice,
                DUnitCollectionEffectType.Water => AccessoryStatusTypeEnum.Water,
                DUnitCollectionEffectType.Fire => AccessoryStatusTypeEnum.Fire,
                DUnitCollectionEffectType.Land => AccessoryStatusTypeEnum.Earth,
                DUnitCollectionEffectType.Wind => AccessoryStatusTypeEnum.Wind,
                DUnitCollectionEffectType.Wood => AccessoryStatusTypeEnum.Wood,
                DUnitCollectionEffectType.Light => AccessoryStatusTypeEnum.Light,
                DUnitCollectionEffectType.Dark => AccessoryStatusTypeEnum.Dark,
                DUnitCollectionEffectType.Thunder => AccessoryStatusTypeEnum.Thunder,
                DUnitCollectionEffectType.Steel => AccessoryStatusTypeEnum.Steel,
                _ => null
            };
        }

        public static bool MatchesPartner(
            DUnitCollectionEffectType effectType,
            DigimonAttributeEnum attribute,
            DigimonElementEnum element)
        {
            return effectType switch
            {
                DUnitCollectionEffectType.Basic => true,
                DUnitCollectionEffectType.Data => attribute == DigimonAttributeEnum.Data,
                DUnitCollectionEffectType.Vaccine => attribute == DigimonAttributeEnum.Vaccine,
                DUnitCollectionEffectType.Virus => attribute == DigimonAttributeEnum.Virus,
                DUnitCollectionEffectType.UnknownAttribute => attribute == DigimonAttributeEnum.Unknown,
                DUnitCollectionEffectType.Ice => element == DigimonElementEnum.Ice,
                DUnitCollectionEffectType.Water => element == DigimonElementEnum.Water,
                DUnitCollectionEffectType.Fire => element == DigimonElementEnum.Fire,
                DUnitCollectionEffectType.Land => element == DigimonElementEnum.Land,
                DUnitCollectionEffectType.Wind => element == DigimonElementEnum.Wind,
                DUnitCollectionEffectType.Wood => element == DigimonElementEnum.Wood,
                DUnitCollectionEffectType.Light => element == DigimonElementEnum.Light,
                DUnitCollectionEffectType.Dark => element == DigimonElementEnum.Dark,
                DUnitCollectionEffectType.Thunder => element == DigimonElementEnum.Thunder,
                DUnitCollectionEffectType.Steel => element == DigimonElementEnum.Steel,
                _ => false
            };
        }
    }
}
