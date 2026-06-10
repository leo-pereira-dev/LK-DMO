namespace ItemEditor.Models;

public sealed record SkillRecord(
    int Id,
    SkillApply Apply0,
    SkillApply Apply1,
    SkillApply Apply2,
    ushort Icon,
    ushort Target,
    ushort AttType,
    float CooldownTimeMs,
    ushort SkillType,
    ushort SkillGroup,
    ushort SkillRank,
    ushort MemorySkill);

public sealed record SkillApply(int A, int InvokeRate, int B, int C, ushort BuffCode, ushort Id, short IncreaseBPoint);

public sealed record BuffRecord(
    ushort Id,
    ushort BuffType,
    ushort LifeType,
    ushort TimeType,
    ushort MinLevel,
    ushort BuffClass,
    uint SkillCode,
    uint DigimonSkillCode,
    ushort ConditionLevel,
    bool IsDeleted);

public sealed record ContainerRecord(int ItemId, int RewardAmount, IReadOnlyList<ContainerRewardRecord> Rewards);

public sealed record ContainerRewardRecord(int ItemId, int MinAmount, int MaxAmount, double Chance, bool Rare, string ItemName);
