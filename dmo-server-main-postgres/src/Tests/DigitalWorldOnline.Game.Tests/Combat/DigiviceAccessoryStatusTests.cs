using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Game.Tests.Support;
using Xunit;

namespace DigitalWorldOnline.Game.Tests.Combat;

public sealed class DigiviceAccessoryStatusTests
{
    [Fact]
    public void Digivice_ratio_statuses_apply_to_partner_hp_ds_skill_and_final_damage()
    {
        var character = TestDigimonFactory.CharacterWithSlots(1);
        character.SetLevelStatus(new CharacterLevelStatusAssetModel());

        var partner = TestDigimonFactory.Digimon(1, 0);
        var baseStatus = new StatusAssetModel();
        baseStatus.SetHP(1000);
        baseStatus.SetDS(500);
        partner.SetBaseStatus(baseStatus);
        partner.SetBaseInfo(new DigimonBaseInfoAssetModel());
        partner.SetTamer(character);
        character.AddDigimon(partner);

        var digivice = character.Digivice.Items[0];
        digivice.SetItemId(151400);
        digivice.SetAmount(1);
        digivice.SetPower(100);
        digivice.SetItemInfo(new ItemAssetModel());

        var baseHp = partner.HP;
        var baseDs = partner.DS;
        var baseSkillDamage = partner.SkillDamagePercent;
        var baseSkillDamageValue = partner.SkillDamagePercentValue;
        var baseFinalDamage = partner.FinalDamageBasisPoints;

        SetAccessoryStatus(digivice, 0, AccessoryStatusTypeEnum.HPRate, 1000);
        SetAccessoryStatus(digivice, 1, AccessoryStatusTypeEnum.DSRate, 500);
        SetAccessoryStatus(digivice, 2, AccessoryStatusTypeEnum.SCDRate, 300);
        SetAccessoryStatus(digivice, 3, AccessoryStatusTypeEnum.FinalDamageRate, 200);

        Assert.Equal(baseHp + 100, partner.HP);
        Assert.Equal(baseDs + 25, partner.DS);
        Assert.Equal(baseSkillDamage + 3, partner.SkillDamagePercent);
        Assert.Equal(baseSkillDamageValue + 3.0, partner.SkillDamagePercentValue, 2);
        Assert.Equal(baseFinalDamage + 200, partner.FinalDamageBasisPoints);
    }

    [Fact]
    public void Accessory_percent_statuses_are_exposed_as_basis_points_for_detail_info()
    {
        var character = TestDigimonFactory.CharacterWithSlots(1);
        character.SetLevelStatus(new CharacterLevelStatusAssetModel());

        var partner = TestDigimonFactory.Digimon(1, 0);
        partner.SetBaseStatus(new StatusAssetModel());
        partner.SetBaseInfo(new DigimonBaseInfoAssetModel());
        partner.SetTamer(character);
        character.AddDigimon(partner);

        var accessory = character.Equipment.Items[0];
        accessory.SetItemId(47420);
        accessory.SetAmount(1);
        accessory.SetPower(100);
        accessory.SetItemInfo(new ItemAssetModel());

        SetAccessoryStatus(accessory, 0, AccessoryStatusTypeEnum.CD, 3645);
        SetAccessoryStatus(accessory, 1, AccessoryStatusTypeEnum.SCDRate, 639);
        SetAccessoryStatus(accessory, 2, AccessoryStatusTypeEnum.FinalDamageRate, 285);

        Assert.Equal(3645, partner.TamerDetailCD);
        Assert.Equal(639, partner.TamerDetailSCD);
        Assert.Equal(285, partner.TamerDetailBaseDamage);
        Assert.Equal(36.45, partner.CriticalDamagePercent, 2);
        Assert.Equal(6.39, partner.SkillDamagePercentValue, 2);
    }

    [Fact]
    public void Update_status_packet_sends_detail_percent_statuses_as_basis_points()
    {
        var character = TestDigimonFactory.CharacterWithSlots(1);
        character.SetLevelStatus(new CharacterLevelStatusAssetModel());

        var partner = TestDigimonFactory.Digimon(1, 0);
        partner.SetBaseStatus(new StatusAssetModel());
        partner.SetBaseInfo(new DigimonBaseInfoAssetModel());
        partner.SetTamer(character);
        character.AddDigimon(partner);

        var accessory = character.Equipment.Items[0];
        accessory.SetItemId(47420);
        accessory.SetAmount(1);
        accessory.SetPower(100);
        accessory.SetItemInfo(new ItemAssetModel());

        SetAccessoryStatus(accessory, 0, AccessoryStatusTypeEnum.CD, 3645);
        SetAccessoryStatus(accessory, 1, AccessoryStatusTypeEnum.SCDRate, 639);
        SetAccessoryStatus(accessory, 2, AccessoryStatusTypeEnum.FinalDamageRate, 285);

        var packet = new UpdateStatusPacket(character).Serialize();
        var detailTailOffset = packet.Length - 2 - (13 * sizeof(int));

        var detailSCD = BitConverter.ToInt32(packet, detailTailOffset + (6 * sizeof(int)));
        var detailCD = BitConverter.ToInt32(packet, detailTailOffset + (7 * sizeof(int)));
        var detailBaseDamage = BitConverter.ToInt32(packet, detailTailOffset + (9 * sizeof(int)));

        Assert.Equal(639, detailSCD);
        Assert.Equal(3645, detailCD);
        Assert.Equal(285, detailBaseDamage);
    }

    [Fact]
    public void Attribute_and_element_skill_damage_are_situational_not_detail_scd()
    {
        var character = TestDigimonFactory.CharacterWithSlots(1);
        character.SetLevelStatus(new CharacterLevelStatusAssetModel());

        var partner = TestDigimonFactory.Digimon(1, 0);
        partner.SetBaseStatus(new StatusAssetModel());
        partner.SetBaseInfo(BaseInfo(DigimonAttributeEnum.Vaccine, DigimonElementEnum.Light));
        partner.SetTamer(character);
        character.AddDigimon(partner);

        var accessory = character.Equipment.Items[0];
        accessory.SetItemId(47420);
        accessory.SetAmount(1);
        accessory.SetPower(100);
        accessory.SetItemInfo(new ItemAssetModel());

        SetAccessoryStatus(accessory, 0, AccessoryStatusTypeEnum.SCDRate, 639);
        SetAccessoryStatus(accessory, 1, AccessoryStatusTypeEnum.ATT, 5);

        var bonus = new DUnitCollectionBonusModel();
        bonus.Set(
            hp: 0,
            ds: 0,
            at: 0,
            de: 0,
            ht: 0,
            ct: 0,
            ev: 0,
            bl: 0,
            exp: 0,
            scd: 1,
            basic: 5,
            vaccine: 2,
            data: 0,
            virus: 0,
            unknown: 0,
            fire: 0,
            water: 0,
            ice: 0,
            wind: 0,
            thunder: 0,
            light: 3,
            dark: 0,
            land: 0,
            wood: 0,
            steel: 0);
        character.SetDUnitCollectionBonus(bonus);

        Assert.Equal(739, partner.TamerDetailSCD);
        Assert.Equal(7.39, partner.SkillDamagePercentValue, 2);
        Assert.Equal(7.39, partner.SkillDamagePercentValueForAdvantage(false, false), 2);
        Assert.Equal(14.39, partner.SkillDamagePercentValueForAdvantage(true, false), 2);
        Assert.Equal(10.39, partner.SkillDamagePercentValueForAdvantage(false, true), 2);
        Assert.Equal(17.39, partner.SkillDamagePercentValueForAdvantage(true, true), 2);
        Assert.Equal(5, partner.ATT);
    }

    private static void SetAccessoryStatus(ItemModel item, byte slot, AccessoryStatusTypeEnum type, short value)
    {
        item.AccessoryStatus[slot].SetType(type);
        item.AccessoryStatus[slot].SetValue(value);
    }

    private static DigimonBaseInfoAssetModel BaseInfo(
        DigimonAttributeEnum attribute,
        DigimonElementEnum element)
    {
        var info = new DigimonBaseInfoAssetModel();
        SetPrivateProperty(info, nameof(DigimonBaseInfoAssetModel.Attribute), attribute);
        SetPrivateProperty(info, nameof(DigimonBaseInfoAssetModel.Element), element);
        return info;
    }

    private static void SetPrivateProperty<T>(object target, string propertyName, T value)
    {
        target.GetType()
            .GetProperty(propertyName)!
            .SetValue(target, value);
    }
}
