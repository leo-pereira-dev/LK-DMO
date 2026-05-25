using System;
using System.Globalization;
using System.Text;

namespace DigitalWorldOnline.Commons.Models.Combat;

public sealed class DamageFormulaInput
{
    public int Attack { get; set; }
    public int ExtraAttack { get; set; }

    public int SkillBaseDamage { get; set; }
    public int SkillDamageFlat { get; set; }

    public double AttributePercent { get; set; }
    public double ElementPercent { get; set; }

    public double SkillDamagePercent { get; set; }
    public double CriticalDamageExtraPercent { get; set; }
    public double FinalDamagePercent { get; set; }
    public double TargetReductionPercent { get; set; }

    public bool HasAttributeAdvantage { get; set; }
    public bool HasElementAdvantage { get; set; }
    public bool IsCritical { get; set; }
    public bool IsSkill { get; set; }

    public int AttackerIndex { get; set; }
    public int TargetIndex { get; set; }
    public int SkillId { get; set; }
}

public sealed class DamageFormulaConfig
{
    public bool Enable { get; set; } = true;
    public double CritBaseRate { get; set; } = 1.0;

    public bool ApplyAttributeToCriticalExtra { get; set; }
    public bool ApplyAttackToSkill { get; set; } = true;
    public bool ApplyAttributeToSkillFlat { get; set; }
    public bool ApplyFinalDamageToSkill { get; set; } = true;
    public bool ApplyElementDamage { get; set; } = true;

    public bool EnableDamageFormulaLog { get; set; }
}

public sealed class DamageFormulaResult
{
    public int FinalDamage { get; set; }

    public double AttributeMultiplier { get; set; }
    public double ElementMultiplier { get; set; }
    public double FinalDamageMultiplier { get; set; }
    public double TargetReductionMultiplier { get; set; }

    public double RawBeforeFinalDamage { get; set; }
    public double RawAfterFinalDamage { get; set; }

    public bool IsCritical { get; set; }
    public bool IsSkill { get; set; }
}

public static class DamageFormula
{
    public static DamageFormulaResult CalculateNormalDamage(
        DamageFormulaInput input,
        DamageFormulaConfig config)
    {
        var result = CreateResult(input, config);
        var baseDamage = Math.Max(0, input.Attack) + Math.Max(0, input.ExtraAttack);

        result.RawBeforeFinalDamage =
            baseDamage *
            result.AttributeMultiplier *
            result.ElementMultiplier;

        result.RawAfterFinalDamage = result.RawBeforeFinalDamage * result.FinalDamageMultiplier;
        result.FinalDamage = SafeFloorDamage(result.RawAfterFinalDamage * result.TargetReductionMultiplier);

        return result;
    }

    public static DamageFormulaResult CalculateCriticalDamage(
        DamageFormulaInput input,
        DamageFormulaConfig config)
    {
        var result = CreateResult(input, config);
        var attack = Math.Max(0, input.Attack) + Math.Max(0, input.ExtraAttack);
        var critBaseExtra = attack * Math.Max(0, config.CritBaseRate);
        var critExtraFromBonus = critBaseExtra * SafePercent(input.CriticalDamageExtraPercent) / 100.0;

        if (config.ApplyAttributeToCriticalExtra)
        {
            result.RawBeforeFinalDamage =
                (attack + critBaseExtra + critExtraFromBonus) *
                result.AttributeMultiplier *
                result.ElementMultiplier;
        }
        else
        {
            result.RawBeforeFinalDamage =
                ((attack + critBaseExtra) *
                 result.AttributeMultiplier *
                 result.ElementMultiplier) +
                critExtraFromBonus;
        }

        result.RawAfterFinalDamage = result.RawBeforeFinalDamage * result.FinalDamageMultiplier;
        result.FinalDamage = SafeFloorDamage(result.RawAfterFinalDamage * result.TargetReductionMultiplier);

        return result;
    }

    public static DamageFormulaResult CalculateSkillDamage(
        DamageFormulaInput input,
        DamageFormulaConfig config)
    {
        var result = CreateResult(input, config);
        var attack = config.ApplyAttackToSkill
            ? Math.Max(0, input.Attack) + Math.Max(0, input.ExtraAttack)
            : 0;
        var skillBaseWithPercent =
            Math.Max(0, input.SkillBaseDamage) *
            (1.0 + SafePercent(input.SkillDamagePercent) / 100.0);
        var skillFlat = Math.Max(0, input.SkillDamageFlat);

        if (config.ApplyAttributeToSkillFlat)
        {
            result.RawBeforeFinalDamage =
                (skillBaseWithPercent + attack + skillFlat) *
                result.AttributeMultiplier *
                result.ElementMultiplier;
        }
        else
        {
            result.RawBeforeFinalDamage =
                ((skillBaseWithPercent + attack) *
                 result.AttributeMultiplier *
                 result.ElementMultiplier) +
                skillFlat;
        }

        var finalDamageMultiplier = config.ApplyFinalDamageToSkill
            ? result.FinalDamageMultiplier
            : 1.0;

        result.RawAfterFinalDamage = result.RawBeforeFinalDamage * finalDamageMultiplier;
        result.FinalDamage = SafeFloorDamage(result.RawAfterFinalDamage * result.TargetReductionMultiplier);

        return result;
    }

    public static DamageFormulaResult CalculateDamage(
        DamageFormulaInput input,
        DamageFormulaConfig config)
    {
        if (input.IsSkill)
            return CalculateSkillDamage(input, config);

        return input.IsCritical
            ? CalculateCriticalDamage(input, config)
            : CalculateNormalDamage(input, config);
    }

    public static string CreateLogMessage(DamageFormulaInput input, DamageFormulaResult result)
    {
        var invariant = CultureInfo.InvariantCulture;
        var builder = new StringBuilder();

        builder.AppendLine("[DamageFormula]");
        builder.AppendFormat(invariant, "Attacker={0} Target={1} Skill={2}", input.AttackerIndex, input.TargetIndex, input.SkillId).AppendLine();
        builder.AppendFormat(invariant, "IsSkill={0} IsCritical={1}", Bool(input.IsSkill), Bool(input.IsCritical)).AppendLine();
        builder.AppendFormat(invariant, "Attack={0} ExtraAttack={1} SkillBase={2} SkillDamageFlat={3}", input.Attack, input.ExtraAttack, input.SkillBaseDamage, input.SkillDamageFlat).AppendLine();
        builder.AppendFormat(invariant, "AttrPercent={0:F2} AttrAdv={1} AttrMul={2:F4}", input.AttributePercent, Bool(input.HasAttributeAdvantage), result.AttributeMultiplier).AppendLine();
        builder.AppendFormat(invariant, "ElementPercent={0:F2} ElementAdv={1} ElementMul={2:F4}", input.ElementPercent, Bool(input.HasElementAdvantage), result.ElementMultiplier).AppendLine();
        builder.AppendFormat(invariant, "SkillDamagePercent={0:F2} CriticalDamageExtraPercent={1:F2} FinalDamagePercent={2:F2}", input.SkillDamagePercent, input.CriticalDamageExtraPercent, input.FinalDamagePercent).AppendLine();
        builder.AppendFormat(invariant, "FinalDamageMul={0:F4} TargetReduction={1:F2} TargetMul={2:F4}", result.FinalDamageMultiplier, input.TargetReductionPercent, result.TargetReductionMultiplier).AppendLine();
        builder.AppendFormat(invariant, "RawBeforeFD={0:F2} RawAfterFD={1:F2} FinalDamage={2}", result.RawBeforeFinalDamage, result.RawAfterFinalDamage, result.FinalDamage);

        return builder.ToString();
    }

    private static DamageFormulaResult CreateResult(DamageFormulaInput input, DamageFormulaConfig config)
    {
        return new DamageFormulaResult
        {
            AttributeMultiplier = GetAttributeMultiplier(input),
            ElementMultiplier = GetElementMultiplier(input, config),
            FinalDamageMultiplier = GetFinalDamageMultiplier(input),
            TargetReductionMultiplier = GetTargetReductionMultiplier(input),
            IsCritical = input.IsCritical,
            IsSkill = input.IsSkill
        };
    }

    private static double GetAttributeMultiplier(DamageFormulaInput input)
    {
        if (!input.HasAttributeAdvantage)
            return 1.0;

        return 1.0 + SafePercent(input.AttributePercent) / 200.0;
    }

    private static double GetElementMultiplier(DamageFormulaInput input, DamageFormulaConfig config)
    {
        if (!config.ApplyElementDamage || !input.HasElementAdvantage)
            return 1.0;

        return 1.0 + SafePercent(input.ElementPercent) / 100.0;
    }

    private static double GetFinalDamageMultiplier(DamageFormulaInput input)
    {
        return 1.0 + SafePercent(input.FinalDamagePercent) / 100.0;
    }

    private static double GetTargetReductionMultiplier(DamageFormulaInput input)
    {
        return Math.Max(0, 1.0 - SafePercent(input.TargetReductionPercent) / 100.0);
    }

    private static int SafeFloorDamage(double value)
    {
        if (double.IsNaN(value) || double.IsInfinity(value) || value <= 0)
            return 1;

        var floored = Math.Floor(value);
        if (floored >= int.MaxValue)
            return int.MaxValue;

        return (int)floored;
    }

    private static double SafePercent(double value)
    {
        if (double.IsNaN(value) || double.IsInfinity(value) || value < 0)
            return 0;

        return value;
    }

    private static int Bool(bool value) => value ? 1 : 0;
}
