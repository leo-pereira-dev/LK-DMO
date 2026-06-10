using DigitalWorldOnline.Commons.Enums;

namespace DigitalWorldOnline.Commons.Models.Asset
{
    public enum EquipmentSetBonusTrigger
    {
        None,
        NormalAttack,
        WhenHit,
        MonsterDefeated,
        DigimonSkill
    }

    public sealed record EquipmentSetBonusApply(
        int RequiredCount,
        SkillCodeApplyTypeEnum Type,
        SkillCodeApplyAttributeEnum Attribute,
        int Value,
        int AdditionalValue,
        bool IsPassive,
        long SkillCode);

    public sealed record EquipmentSetBonusEffect(
        int RequiredCount,
        long SkillCode,
        int BuffId,
        int DurationSeconds,
        int ChancePercent,
        bool IsPassive,
        EquipmentSetBonusTrigger Trigger,
        BuffInfoAssetModel? BuffInfo,
        IReadOnlyCollection<EquipmentSetBonusApply> Applies);

    public sealed class EquipmentSetBonusRule
    {
        private readonly HashSet<int> _itemIds;

        public EquipmentSetBonusRule(int setIndex, IEnumerable<int> itemIds, IEnumerable<EquipmentSetBonusEffect> effects)
        {
            SetIndex = setIndex;
            _itemIds = itemIds.ToHashSet();
            ItemIds = _itemIds.ToArray();
            Effects = effects.ToArray();
            Applies = Effects.SelectMany(x => x.Applies).ToArray();
        }

        public int SetIndex { get; }
        public IReadOnlyCollection<int> ItemIds { get; }
        public IReadOnlyCollection<EquipmentSetBonusEffect> Effects { get; }
        public IReadOnlyCollection<EquipmentSetBonusApply> Applies { get; }

        public bool ContainsItem(int itemId) => _itemIds.Contains(itemId);
    }

    public static class EquipmentSetBonusRegistry
    {
        private const long SetPieceSkillFirst = 2702087;
        private const long SetPieceSkillLast = 2702263;
        private const long SetEffectSkillFirst = 2702279;
        private const int SetItemGroupCount = 12;
        private const int SetEffectCountPerGroup = 2;

        private static readonly int[] SetPieceTypePattern = { 21, 27, 22, 24, 23, 25 };
        private static readonly int[] FallbackRequiredCounts = { 4, 6 };
        private static IReadOnlyList<EquipmentSetBonusRule> _rules = Array.Empty<EquipmentSetBonusRule>();

        public static IReadOnlyList<EquipmentSetBonusRule> Rules => _rules;

        public static void ReplaceRules(IEnumerable<EquipmentSetBonusRule> rules)
        {
            _rules = rules.ToArray();
        }

        public static IReadOnlyList<EquipmentSetBonusRule> BuildFrom(
            IEnumerable<ItemAssetModel>? items,
            IEnumerable<SkillCodeAssetModel>? skillCodes,
            IEnumerable<SkillInfoAssetModel>? skillInfos,
            IEnumerable<BuffInfoAssetModel>? buffInfos)
        {
            if (items == null || skillCodes == null)
                return Array.Empty<EquipmentSetBonusRule>();

            var candidates = items
                .Where(IsSetPieceItem)
                .OrderBy(x => x.ItemId)
                .ToArray();

            if (candidates.Length < SetItemGroupCount)
                return Array.Empty<EquipmentSetBonusRule>();

            var skillCodeById = skillCodes
                .GroupBy(x => x.SkillCode)
                .ToDictionary(x => x.Key, x => x.First());

            var skillInfoById = (skillInfos ?? Enumerable.Empty<SkillInfoAssetModel>())
                .GroupBy(x => x.SkillId)
                .ToDictionary(x => x.Key, x => x.First());

            var buffBySkillCode = (buffInfos ?? Enumerable.Empty<BuffInfoAssetModel>())
                .Where(x => x.SkillCode > 0 && x.BuffId > 0)
                .GroupBy(x => x.SkillCode)
                .ToDictionary(x => x.Key, x => x.First().BuffId);

            var rules = new List<EquipmentSetBonusRule>();

            for (var index = 0; index + SetItemGroupCount <= candidates.Length;)
            {
                if (!IsExpectedSetChunk(candidates, index))
                {
                    index++;
                    continue;
                }

                var setIndex = rules.Count;
                var itemIds = candidates
                    .Skip(index)
                    .Take(SetItemGroupCount)
                    .Select(x => x.ItemId);

                var effects = new List<EquipmentSetBonusEffect>();
                for (var effectIndex = 0; effectIndex < SetEffectCountPerGroup; effectIndex++)
                {
                    var skillCode = SetEffectSkillFirst + (setIndex * SetEffectCountPerGroup) + effectIndex;
                    if (!skillCodeById.TryGetValue(skillCode, out var skillCodeInfo))
                        continue;

                    var requiredCount = ParseRequiredCount(skillCodeInfo.Comment, FallbackRequiredCounts[effectIndex]);
                    var duration = ResolveDurationSeconds(skillCode, skillCodeInfo.Comment, skillInfoById);
                    var trigger = ResolveTrigger(skillCodeInfo.Comment);
                    var passive = IsPassiveSetEffect(skillCodeInfo.Comment, duration, trigger);
                    var applies = new List<EquipmentSetBonusApply>();

                    foreach (var apply in skillCodeInfo.Apply ?? Enumerable.Empty<SkillCodeApplyAssetModel>())
                    {
                        if (apply.Type == SkillCodeApplyTypeEnum.None || apply.Attribute == SkillCodeApplyAttributeEnum.Unknown)
                            continue;

                        applies.Add(new EquipmentSetBonusApply(
                            requiredCount,
                            apply.Type,
                            apply.Attribute,
                            ResolveApplyValue(apply),
                            apply.AdditionalValue,
                            passive,
                            skillCode));
                    }

                    if (applies.Count == 0)
                        continue;

                    var runtimeBuffId = ResolveRuntimeBuffId(skillCode, buffBySkillCode);

                    if (!passive && trigger != EquipmentSetBonusTrigger.None && runtimeBuffId <= 0)
                        continue;

                    var buffInfo = runtimeBuffId <= 0
                        ? null
                        : BuffInfoAssetModel.CreateRuntimeEquipmentSetBuff(
                            runtimeBuffId,
                            (int)skillCode,
                            $"Equipment Set {skillCode}",
                            skillCodeInfo);

                    effects.Add(new EquipmentSetBonusEffect(
                        requiredCount,
                        skillCode,
                        runtimeBuffId,
                        duration,
                        ResolveChancePercent(skillCodeInfo, passive),
                        passive,
                        trigger,
                        buffInfo,
                        applies));
                }

                if (effects.Count > 0)
                    rules.Add(new EquipmentSetBonusRule(setIndex, itemIds, effects));

                index += SetItemGroupCount;
            }

            return rules;
        }

        private static bool IsSetPieceItem(ItemAssetModel item)
        {
            return item.SkillCode >= SetPieceSkillFirst &&
                   item.SkillCode <= SetPieceSkillLast &&
                   SetPieceTypePattern.Contains(item.Type);
        }

        private static bool IsExpectedSetChunk(ItemAssetModel[] candidates, int startIndex)
        {
            for (var offset = 0; offset < SetItemGroupCount; offset++)
            {
                if (candidates[startIndex + offset].Type != SetPieceTypePattern[offset % SetPieceTypePattern.Length])
                    return false;
            }

            return true;
        }

        private static bool IsPassiveSetEffect(
            string? comment,
            int duration,
            EquipmentSetBonusTrigger trigger)
        {
            if (!string.IsNullOrWhiteSpace(comment) &&
                comment.Contains("Passive", StringComparison.OrdinalIgnoreCase))
                return true;

            return duration <= 0 && trigger == EquipmentSetBonusTrigger.None;
        }

        private static int ResolveApplyValue(SkillCodeApplyAssetModel apply)
        {
            return apply.Value != 0 ? apply.Value : apply.AdditionalValue;
        }

        private static int ResolveDurationSeconds(
            long skillCode,
            string? comment,
            IReadOnlyDictionary<int, SkillInfoAssetModel> skillInfoById)
        {
            if (skillCode <= int.MaxValue &&
                skillInfoById.TryGetValue((int)skillCode, out var skillInfo) &&
                skillInfo.EffectDuration > 0)
                return skillInfo.EffectDuration;

            return ParseDurationSeconds(comment);
        }

        private static int ResolveChancePercent(SkillCodeAssetModel skillCodeInfo, bool passive)
        {
            if (passive)
                return 100;

            var applyChance = skillCodeInfo.Apply?
                .Where(x => x.Chance > 0)
                .Select(x => x.Chance)
                .DefaultIfEmpty(0)
                .Max() ?? 0;

            var chance = applyChance > 0 ? applyChance : ParseFirstPercent(skillCodeInfo.Comment);
            return Math.Clamp(chance, 0, 100);
        }

        private static EquipmentSetBonusTrigger ResolveTrigger(string? comment)
        {
            if (string.IsNullOrWhiteSpace(comment))
                return EquipmentSetBonusTrigger.None;

            if (comment.Contains("Normal ATK", StringComparison.OrdinalIgnoreCase) ||
                comment.Contains("Normal Attack", StringComparison.OrdinalIgnoreCase))
                return EquipmentSetBonusTrigger.NormalAttack;

            if (comment.Contains("When Hit", StringComparison.OrdinalIgnoreCase))
                return EquipmentSetBonusTrigger.WhenHit;

            if (comment.Contains("Defeating a Monster", StringComparison.OrdinalIgnoreCase) ||
                comment.Contains("Defeat a Monster", StringComparison.OrdinalIgnoreCase))
                return EquipmentSetBonusTrigger.MonsterDefeated;

            if (comment.Contains("Using a Digimon Skill", StringComparison.OrdinalIgnoreCase) ||
                comment.Contains("Digimon Skill", StringComparison.OrdinalIgnoreCase))
                return EquipmentSetBonusTrigger.DigimonSkill;

            return EquipmentSetBonusTrigger.None;
        }

        private static int ResolveRuntimeBuffId(
            long skillCode,
            IReadOnlyDictionary<int, int> buffBySkillCode)
        {
            return skillCode <= int.MaxValue &&
                   buffBySkillCode.TryGetValue((int)skillCode, out var buffId)
                ? buffId
                : 0;
        }

        private static int ParseFirstPercent(string? comment)
        {
            if (string.IsNullOrWhiteSpace(comment))
                return 0;

            var percentIndex = comment.IndexOf('%');
            if (percentIndex <= 0)
                return 0;

            var numberEnd = percentIndex;
            var numberStart = numberEnd - 1;
            while (numberStart >= 0 && char.IsDigit(comment[numberStart]))
                numberStart--;
            numberStart++;

            return numberStart < numberEnd && int.TryParse(comment[numberStart..numberEnd], out var chance)
                ? chance
                : 0;
        }

        private static int ParseDurationSeconds(string? comment)
        {
            if (string.IsNullOrWhiteSpace(comment))
                return 0;

            for (var index = 1; index < comment.Length; index++)
            {
                if (comment[index] != 's' && comment[index] != 'S')
                    continue;

                var numberEnd = index;
                var numberStart = numberEnd - 1;
                while (numberStart >= 0 && char.IsDigit(comment[numberStart]))
                    numberStart--;
                numberStart++;

                if (numberStart < numberEnd &&
                    int.TryParse(comment[numberStart..numberEnd], out var seconds) &&
                    seconds > 0)
                    return seconds;
            }

            return 0;
        }

        private static int ParseRequiredCount(string? comment, int fallback)
        {
            if (string.IsNullOrWhiteSpace(comment))
                return fallback;

            var setIndex = comment.IndexOf("Set ", StringComparison.OrdinalIgnoreCase);
            if (setIndex < 0)
                return fallback;

            var numberStart = setIndex + 4;
            var numberEnd = numberStart;
            while (numberEnd < comment.Length && char.IsDigit(comment[numberEnd]))
                numberEnd++;

            if (numberEnd == numberStart)
                return fallback;

            return int.TryParse(comment[numberStart..numberEnd], out var count) && count > 0
                ? count
                : fallback;
        }
    }
}
