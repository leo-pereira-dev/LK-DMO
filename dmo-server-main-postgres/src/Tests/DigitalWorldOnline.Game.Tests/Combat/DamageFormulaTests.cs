using DigitalWorldOnline.Commons.Models.Combat;
using Xunit;

namespace DigitalWorldOnline.Game.Tests.Combat;

public sealed class DamageFormulaTests
{
    [Fact]
    public void Normal_damage_applies_attribute_advantage()
    {
        var input = new DamageFormulaInput
        {
            Attack = 1000,
            AttributePercent = 100,
            HasAttributeAdvantage = true
        };

        var result = DamageFormula.CalculateDamage(input, new DamageFormulaConfig());

        Assert.Equal(1500, result.FinalDamage);
    }

    [Fact]
    public void Normal_damage_applies_attribute_and_final_damage()
    {
        var input = new DamageFormulaInput
        {
            Attack = 1000,
            AttributePercent = 100,
            HasAttributeAdvantage = true,
            FinalDamagePercent = 20
        };

        var result = DamageFormula.CalculateDamage(input, new DamageFormulaConfig());

        Assert.Equal(1800, result.FinalDamage);
    }

    [Fact]
    public void Critical_damage_keeps_extra_critical_bonus_outside_attribute_by_default()
    {
        var input = new DamageFormulaInput
        {
            Attack = 1000,
            AttributePercent = 100,
            CriticalDamageExtraPercent = 50,
            HasAttributeAdvantage = true,
            IsCritical = true
        };

        var result = DamageFormula.CalculateDamage(
            input,
            new DamageFormulaConfig
            {
                CritBaseRate = 1.0,
                ApplyAttributeToCriticalExtra = false
            });

        Assert.Equal(3500, result.FinalDamage);
    }

    [Fact]
    public void Skill_damage_applies_skill_percent_flat_attribute_and_final_damage()
    {
        var input = new DamageFormulaInput
        {
            SkillBaseDamage = 10000,
            Attack = 1000,
            SkillDamagePercent = 30,
            SkillDamageFlat = 2000,
            AttributePercent = 100,
            HasAttributeAdvantage = true,
            FinalDamagePercent = 10,
            IsSkill = true
        };

        var result = DamageFormula.CalculateDamage(input, new DamageFormulaConfig());

        Assert.Equal(25300, result.FinalDamage);
    }
}
