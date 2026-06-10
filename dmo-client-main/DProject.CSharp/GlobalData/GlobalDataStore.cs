using DProject.Data;
using DProject.Data.Loading;
using DProject.Game;
using DProject.Interface.Base;
using DProject.Resources;

namespace DProject.GlobalData;

public enum GameLanguage
{
    Korean,
    English,
    Hongkong,
    Taiwan,
    Thailand
}

public enum GameCombatState
{
    Peace,
    Active
}

public sealed record WorldObjectState(
    uint Uid,
    uint Index,
    uint ClassId,
    uint TypeId,
    int X,
    int Y,
    float Rotation,
    bool IsPresent,
    string LastSyncKind,
    string Name = "",
    byte Level = 0,
    byte HpRate = 0,
    uint Condition = 0,
    string PayloadKind = "",
    uint ExtBuff = 0);

public sealed record WorldObjectBuffState(ushort BuffCode, ushort Level, uint EndTimestamp, uint SkillCode);

public sealed record WorldSyncChatMessage(
    byte Command,
    uint TamerUid,
    byte ResultCode,
    string Source,
    string Target,
    string Message,
    int ItemType,
    byte TamerLevel,
    bool IsGuild);

public sealed record MapRenderableState(
    string Key,
    string Kind,
    int Count,
    IReadOnlyDictionary<string, string> Metadata);

public sealed class GlobalDataStore
{
    public GameLanguage Language { get; set; } = GameLanguage.English;
    public string AccountId { get; set; } = string.Empty;
    public string AccountPassword { get; set; } = string.Empty;
    public string SelectedServerName { get; set; } = string.Empty;
    public string SelectedCharacterName { get; set; } = string.Empty;
    public string PendingCharacterName { get; set; } = string.Empty;
    public string PendingPartnerName { get; set; } = string.Empty;
    public bool SecondPasswordVerified { get; set; }
    public bool IsBattleActive { get; set; }
    public string LastBattleResult { get; set; } = string.Empty;
    public GameCombatState CombatState { get; set; } = GameCombatState.Peace;
    public string CombatStateSource { get; set; } = string.Empty;
    public bool IsMapTransitioning { get; set; }
    public uint CurrentMapId { get; set; }
    public string CurrentMapName { get; set; } = string.Empty;
    public uint PendingMapId { get; set; }
    public string PendingMapName { get; set; } = string.Empty;
    public string MapTransitionReason { get; set; } = string.Empty;
    public bool MapAssetsLoaded { get; set; }
    public uint LastMapLoadId { get; set; }
    public string LastMapLoadName { get; set; } = string.Empty;
    public string LastMapLoadReason { get; set; } = string.Empty;
    public string LastMapLoadStatus { get; set; } = string.Empty;
    public string LastMapTerrainPath { get; set; } = string.Empty;
    public int LastMapAssetCount { get; set; }
    public int LastMapRenderableCount { get; set; }
    public MapLoadResult? LastMapLoadResult { get; set; }
    public string GameServerHost { get; set; } = string.Empty;
    public int GameServerPort { get; set; }
    public int StartPositionX { get; set; }
    public int StartPositionY { get; set; }
    public int ServerLibType { get; set; }
    public bool UseFilePack { get; set; }
    public bool FileTablesLoaded { get; set; }
    public string FileTableSource { get; set; } = string.Empty;
    public string FileTableLanguage { get; set; } = string.Empty;
    public string FileTablePath { get; set; } = string.Empty;
    public CCharMng CharMng { get; } = new();
    public CCharResMng CharResMng { get; } = new();
    public CPat_AI_Mng PatAIMng { get; } = new();
    public CEffectMng EffectMng { get; } = new();
    public ResourceMng ResourceMng { get; } = new();
    public cIconMng IconMng { get; }
    public cDataMng DataMng { get; } = new();
    public cData_ServerSync ServerSync => DataMng.ServerSync;
    public AssetLoadingCoordinator? AssetLoading { get; set; }
    public Dictionary<uint, WorldObjectState> WorldObjects { get; } = new();
    public Dictionary<uint, IReadOnlyList<WorldObjectBuffState>> WorldObjectBuffs { get; } = new();
    public Dictionary<string, MapRenderableState> MapRenderables { get; } = new(StringComparer.OrdinalIgnoreCase);
    public List<WorldSyncChatMessage> SyncChatMessages { get; } = new();
    public long LastMovementSyncSequence { get; set; }
    public int LastMovementSyncObjectCount { get; set; }
    public int LastMovementSyncRemainingBytes { get; set; }
    public int LastSyncConditionCount { get; set; }
    public int LastSyncBuffObjectCount { get; set; }
    public int LastSyncChatMessageCount { get; set; }
    public uint LastMapNpcLoadMapId { get; set; }
    public int LastMapNpcLoadCount { get; set; }
    public bool IsChangingDigimon { get; set; }
    public bool IsUsingDigimonSkill { get; set; }
    public bool IsRideSyncActive { get; set; }
    public string LastServerSyncSend { get; set; } = string.Empty;
    public string LastServerSyncRecv { get; set; } = string.Empty;

    public GlobalDataStore()
    {
        IconMng = new cIconMng(ResourceMng);
    }

    public void Clear2ndPass()
    {
        SecondPasswordVerified = false;
    }
}
