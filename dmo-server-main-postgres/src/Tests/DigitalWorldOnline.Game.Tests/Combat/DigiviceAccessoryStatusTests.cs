using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Models;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
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
        var baseFinalDamage = partner.FinalDamageBasisPoints;

        SetAccessoryStatus(digivice, 0, AccessoryStatusTypeEnum.HPRate, 1000);
        SetAccessoryStatus(digivice, 1, AccessoryStatusTypeEnum.DSRate, 500);
        SetAccessoryStatus(digivice, 2, AccessoryStatusTypeEnum.SCDRate, 300);
        SetAccessoryStatus(digivice, 3, AccessoryStatusTypeEnum.FinalDamageRate, 200);

        Assert.Equal(baseHp + 100, partner.HP);
        Assert.Equal(baseDs + 25, partner.DS);
        Assert.Equal(baseSkillDamage + 3, partner.SkillDamagePercent);
        Assert.Equal(baseFinalDamage + 200, partner.FinalDamageBasisPoints);
    }

    private static void SetAccessoryStatus(ItemModel item, byte slot, AccessoryStatusTypeEnum type, short value)
    {
        item.AccessoryStatus[slot].SetType(type);
        item.AccessoryStatus[slot].SetValue(value);
    }
}
