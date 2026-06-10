using System.Collections.Generic;

namespace DigitalWorldOnline.Commons.Constants
{
    public static class MembershipConstants
    {
        public const int SevenDayItemId = 70125;
        public const int ThirtyDayItemId = 70102;
        public const int SixtyDayItemId = 70050;

        public const int SevenDayDurationSeconds = 7 * 24 * 60 * 60;
        public const int ThirtyDayDurationSeconds = 30 * 24 * 60 * 60;
        public const int SixtyDayDurationSeconds = 60 * 24 * 60 * 60;

        public const int MoveSpeedBuffId = 50121;

        public static readonly IReadOnlyDictionary<int, int> DurationSecondsByItemId =
            new Dictionary<int, int>
            {
                { SevenDayItemId, SevenDayDurationSeconds },
                { ThirtyDayItemId, ThirtyDayDurationSeconds },
                { SixtyDayItemId, SixtyDayDurationSeconds }
            };

        public static readonly IReadOnlyList<(int BuffId, int SkillCode)> VisibleBuffs =
            new List<(int BuffId, int SkillCode)>
            {
                (50122, 2700023),
                (50123, 2700024)
            };
    }
}
