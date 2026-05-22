namespace DigitalWorldOnline.Application.GameAssets.Xml
{
    public sealed record DUnitCollectionRequirement(
        int MembersRequired,
        int TotalLevelRequirement1,
        int TotalLevelRequirement2,
        bool RequireAllTranscended);

    public sealed record DUnitRequiredDigimon(
        int DigimonId,
        string DigimonName,
        byte RequiredLevel,
        bool RequiredTranscendence,
        ushort RequiredEvolutionStage,
        bool RequiredUnlocked,
        byte Slot);

    public sealed record DUnitCollectionEffect(
        DUnitCollectionEffectType EffectType,
        int EffectValue,
        string? AttributeType,
        int? RequiredConditionIndex,
        string Description,
        bool Percent);

    public sealed record DUnitCollectionGroup(
        int GroupId,
        string GroupName,
        string Description,
        IReadOnlyList<DUnitRequiredDigimon> RequiredDigimons,
        IReadOnlyList<DUnitCollectionEffect> Effects,
        DUnitCollectionRequirement Requirements,
        bool Enabled,
        string SourceType);

    public sealed record DUnitPlayerProgress(
        long CharacterId,
        int GroupId,
        bool IsCompleted,
        int CompletedConditionCount,
        DateTime UpdatedAt);

    public sealed record DUnitRequiredDigimonProgress(
        DUnitRequiredDigimon Requirement,
        bool Owned,
        ushort OwnedLevel,
        bool EvolutionUnlocked,
        bool Transcended,
        bool MeetsLevel,
        bool MeetsEvolution,
        bool MeetsTranscendence);

    public sealed record DUnitCollectionEffectProgress(
        DUnitCollectionEffect Effect,
        bool Active);

    public sealed record DUnitCollectionEvaluation(
        DUnitCollectionGroup Group,
        bool IsCompleted,
        int OwnedDigimonCount,
        int TotalLevel,
        bool AllRequiredTranscended,
        IReadOnlyList<DUnitRequiredDigimonProgress> RequiredDigimons,
        IReadOnlyList<DUnitCollectionEffectProgress> Effects);

    public sealed class DUnitCollectionBonusResult
    {
        private readonly Dictionary<DUnitCollectionEffectType, int> _bonuses = new();

        public IReadOnlyDictionary<DUnitCollectionEffectType, int> Bonuses => _bonuses;

        public IReadOnlyList<DUnitCollectionEvaluation> Groups { get; }

        public int CompletedGroups => Groups.Count(x => x.IsCompleted);

        public DUnitCollectionBonusResult(IReadOnlyList<DUnitCollectionEvaluation> groups)
        {
            Groups = groups;
        }

        public void Add(DUnitCollectionEffectType effectType, int value)
        {
            if (_bonuses.TryGetValue(effectType, out var current))
                _bonuses[effectType] = current + value;
            else
                _bonuses[effectType] = value;
        }

        public int Get(DUnitCollectionEffectType effectType)
            => _bonuses.TryGetValue(effectType, out var value) ? value : 0;
    }
}
