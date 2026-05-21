namespace DigitalWorldOnline.Commons.Models.Character
{
    public sealed partial class CharacterXmlUnionProgressModel
    {
        public ushort ProgressPercentBasisPoints
        {
            get
            {
                if (RequiredExperience <= 0)
                    return 0;

                var value = CurrentExperience * 10000L / RequiredExperience;
                if (value < 0)
                    return 0;

                return value > 10000 ? (ushort)10000 : (ushort)value;
            }
        }

        public void SetCharacter(long characterId)
        {
            CharacterId = characterId;
            Touch();
        }

        public void SetProgress(byte level, int currentExperience, int requiredExperience)
        {
            Level = level;
            CurrentExperience = Math.Max(0, currentExperience);
            RequiredExperience = Math.Max(1, requiredExperience);

            if (CurrentExperience > RequiredExperience)
                CurrentExperience = RequiredExperience;

            Touch();
        }

        public bool AddExperience(int amount, Func<byte, int> requiredExperienceProvider)
        {
            if (amount <= 0)
                return false;

            CurrentExperience += amount;
            RequiredExperience = Math.Max(1, requiredExperienceProvider(Level));

            while (CurrentExperience >= RequiredExperience)
            {
                CurrentExperience -= RequiredExperience;
                if (Level < byte.MaxValue)
                    Level++;

                RequiredExperience = Math.Max(1, requiredExperienceProvider(Level));
            }

            Touch();
            return true;
        }

        public bool HasClaimedReward(byte rewardIndex)
        {
            if (rewardIndex >= 16)
                return false;

            return (ClaimedRewardMask & (1 << rewardIndex)) != 0;
        }

        public bool ClaimReward(byte rewardIndex)
        {
            if (rewardIndex >= 16 || HasClaimedReward(rewardIndex))
                return false;

            ClaimedRewardMask = (ushort)(ClaimedRewardMask | (1 << rewardIndex));
            Touch();
            return true;
        }

        private void Touch()
        {
            UpdatedAt = DateTime.UtcNow;
            if (CreatedAt == default)
                CreatedAt = UpdatedAt;
        }
    }
}
