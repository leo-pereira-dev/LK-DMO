namespace DProject.Network;

public sealed record AccountLoginResult(uint Result, byte SubType)
{
    public bool Succeeded => Result == 0;
}

public sealed record ServerInfo(
    int Index,
    string Name,
    byte DisableState,
    byte ClusterState,
    byte TamerNumber,
    bool IsNewServer,
    byte MaxTamerSlot,
    byte OpenTamerSlot);

public sealed record GateConnectionInfo(
    uint AccountIndex,
    uint AccessCode,
    string Host,
    int Port);

public sealed record SecondPasswordResult(ushort Protocol, uint Result);

public sealed record CharacterSummary(
    sbyte SlotNumber,
    short MapId,
    int TamerObjectTableId,
    byte TamerLevel,
    string TamerName,
    uint DigimonObjectTableId,
    byte DigimonLevel,
    string DigimonName,
    short DigimonScale,
    ushort PatLeader,
    int RelocateState);

public sealed record CharacterSelectResult(
    bool Succeeded,
    string GameHost,
    int GamePort,
    uint MapId,
    uint FailureMapId);

public sealed record GameConnectionInfo(
    uint AccountIndex,
    uint AccessCode,
    string Host,
    int Port,
    uint MapId);

public sealed record ChannelInfo(IReadOnlyDictionary<byte, byte> Channels);

public sealed record GameInitData(
    int ServerLibType,
    int StartX,
    int StartY,
    ulong TamerTypeAll,
    string TamerName);

public sealed record PortalTransitionInfo(
    string Host,
    int Port,
    uint MapId,
    int X,
    int Y);

public sealed record LocalPortalInfo(
    uint TamerUid,
    uint DigimonUid,
    int X,
    int Y);

public sealed record SyncPosition(int X, int Y);

public sealed record DmoObjectType(ulong TypeAll, uint Uid, uint Index, uint ClassId, uint TypeId)
{
    public static DmoObjectType FromTypeAll(ulong typeAll)
    {
        return new DmoObjectType(
            typeAll,
            (uint)(typeAll & 0x7FFFF),
            (uint)(typeAll & 0x3FFF),
            (uint)((typeAll >> 14) & 0x1F),
            (uint)((typeAll >> 19) & 0x7FFFF));
    }

    public static DmoObjectType FromUid(uint uid)
    {
        return new DmoObjectType(
            uid,
            uid,
            uid & 0x3FFF,
            (uid >> 14) & 0x1F,
            0);
    }
}

public sealed record SyncMovement(byte Command, DmoObjectType ObjectType, SyncPosition Position, bool IsWalk);

public sealed record SyncRotation(DmoObjectType ObjectType, float Direction);

public sealed record SyncPresence(byte Command, DmoObjectType ObjectType, SyncPosition? Position);

public sealed record SyncConditionChange(DmoObjectType ObjectType, uint Condition, uint ExtBuff);

public sealed record SyncChatMessage(
    byte Command,
    uint TamerUid,
    byte ResultCode,
    string Source,
    string Target,
    string Message,
    int ItemType,
    byte TamerLevel,
    bool IsGuild);

public sealed record SyncBuffEntry(ushort BuffCode, ushort Level, uint EndTimestamp, uint SkillCode);

public sealed record SyncBuffObject(byte ObjectClass, DmoObjectType ObjectType, IReadOnlyList<SyncBuffEntry> Buffs);

public sealed record DmoItemData(uint Type, uint Count, uint EndTime, IReadOnlyList<byte> RawBytes);

public abstract record SyncObjectPayload;

public sealed record SyncUnknownPayload(string Reason) : SyncObjectPayload;

public sealed record SyncItemPayload(uint OwnerTamerUid, byte ItemState) : SyncObjectPayload;

public sealed record SyncCommissionShopPayload(float Direction, uint ItemType, string ShopName) : SyncObjectPayload;

public sealed record SyncTamerPayload(
    SyncPosition Destination,
    string Name,
    byte Level,
    float Direction,
    ushort MoveSpeed,
    byte HpRate,
    IReadOnlyList<DmoItemData> EquipItems,
    DmoItemData? DigiviceItem,
    uint Condition,
    uint SyncFlags,
    uint PartnerUid,
    ushort Scale,
    byte GuildClass,
    uint GuildIndex,
    string GuildName,
    ushort Achievement,
    byte MatchTeam,
    ushort Card,
    string ShopTitle,
    uint ChangeCostumeNo) : SyncObjectPayload;

public sealed record SyncDigimonPayload(
    SyncPosition Destination,
    string Name,
    ushort Scale,
    byte Level,
    float Direction,
    ushort MoveSpeed,
    ushort AttackSpeed,
    uint TamerUid,
    byte HpRate,
    uint Condition,
    byte? BattleTeam,
    ushort EnchantLevel,
    IReadOnlyList<ushort> ExtensionParameterLevels,
    int CompatRemainingBytes,
    uint? PartsType) : SyncObjectPayload;

public sealed record SyncMonsterPayload(
    SyncPosition Destination,
    byte HpRate,
    byte Level,
    uint MonsterSkillIndex,
    int MonsterStack,
    uint Condition,
    uint? CastingSkillIndex,
    IReadOnlyList<uint> SeedSkillIndexes,
    IReadOnlyList<SyncMonsterSeedEffect> SeedEffects,
    bool SkillTableResolved,
    bool HasPotentialSeedPayload) : SyncObjectPayload;

public sealed record SyncMonsterSeedEffect(
    uint SkillIndex,
    int X,
    int Y,
    int EffectLifetime,
    uint Range);

public sealed record SyncWorldObjectSnapshot(
    byte Command,
    bool IsNew,
    DmoObjectType ObjectType,
    SyncPosition Position,
    SyncObjectPayload Payload,
    bool HasUnparsedTail);

public sealed record GameSyncSummary(
    byte FirstSyncType,
    int RemainingBytes,
    IReadOnlyList<byte> SyncTypes,
    IReadOnlyList<SyncMovement> Movements,
    IReadOnlyList<SyncRotation> Rotations,
    IReadOnlyList<SyncPresence> PresenceChanges,
    IReadOnlyList<SyncWorldObjectSnapshot> WorldObjects,
    IReadOnlyList<SyncConditionChange> ConditionChanges,
    IReadOnlyList<SyncChatMessage> ChatMessages,
    IReadOnlyList<SyncBuffObject> BuffObjects,
    bool StoppedAtUnparsedPayload);

public sealed record StartMessageInfo(IReadOnlyList<string> Messages);

public sealed record VersionErrorInfo(uint ServerVersion, string Message);
