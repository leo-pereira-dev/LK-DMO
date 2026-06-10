namespace DigitalWorldOnline.GameHost;

internal static class ExperienceRewardCalculator
{
    private const int TamerExperienceDivisor = 10;

    public static long CalculateKillExperience(int actorLevel, int mobLevel, long baseExperience)
    {
        var levelDifference = actorLevel - mobLevel;

        if (levelDifference > 30)
            return 0;

        if (levelDifference > 0)
            return (long)(baseExperience * (1.0 - levelDifference * 0.03));

        return baseExperience;
    }

    public static long CalculateTamerExperienceFromDigimon(long digimonExperience)
    {
        if (digimonExperience <= 0)
            return 0;

        return digimonExperience / TamerExperienceDivisor;
    }

    public static long CalculateTamerExperienceFromKilledDigimon(
        int tamerLevel,
        int mobLevel,
        long digimonExperience,
        double share = 1.0)
    {
        if (digimonExperience <= 0 || share <= 0)
            return 0;

        var adjustedDigimonExperience = CalculateKillExperience(tamerLevel, mobLevel, digimonExperience);

        return CalculateTamerExperienceFromDigimon((long)(adjustedDigimonExperience * share));
    }
}
