using DigitalWorldOnline.Commons.Enums;

namespace DigitalWorldOnline.Commons.Models.Asset
{
    public static class TamerEquipmentUpgradeStageRegistry
    {
        private const int FirstItemId = 47359;
        private const int LastItemId = 47394;
        private const int FirstStageSkillCode = 2702320;
        private const int SkillCodesPerItem = 16;
        private const int MaxStage = 15;

        private static readonly Dictionary<(int ItemId, byte Stage), IReadOnlyCollection<SkillCodeApplyAssetModel>> _applies = new();

        public static void ReplaceRules(IEnumerable<SkillCodeAssetModel>? skillCodes)
        {
            _applies.Clear();

            if (skillCodes == null)
                return;

            var skillByCode = skillCodes
                .GroupBy(x => x.SkillCode)
                .ToDictionary(x => x.Key, x => x.First());

            for (var itemId = FirstItemId; itemId <= LastItemId; itemId++)
            {
                for (byte stage = 1; stage <= MaxStage; stage++)
                {
                    var skillCode = GetSkillCode(itemId, stage);
                    if (!skillByCode.TryGetValue(skillCode, out var skill))
                        continue;

                    var applies = skill.Apply
                        .Where(x => x.Type != SkillCodeApplyTypeEnum.None &&
                                    x.Attribute != SkillCodeApplyAttributeEnum.Unknown)
                        .ToArray();

                    if (applies.Length > 0)
                        _applies[(itemId, stage)] = applies;
                }
            }
        }

        public static IReadOnlyCollection<SkillCodeApplyAssetModel> GetApplies(int itemId, byte stage)
        {
            if (itemId < FirstItemId || itemId > LastItemId || stage < 1 || stage > MaxStage)
                return Array.Empty<SkillCodeApplyAssetModel>();

            return _applies.TryGetValue((itemId, stage), out var applies)
                ? applies
                : Array.Empty<SkillCodeApplyAssetModel>();
        }

        private static int GetSkillCode(int itemId, byte stage)
        {
            return FirstStageSkillCode + (itemId - FirstItemId) * SkillCodesPerItem + (stage - 1);
        }
    }
}
