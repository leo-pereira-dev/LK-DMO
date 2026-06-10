namespace DProject.Data;

public enum eSERVER_SYNC
{
    DIGIMON_EVOLUTION_OPEN,
    DIGIMON_RIDE_OPEN,
    ELEMENT_ITEM_CHECK,
    TACTICSHOUSE_LIST,
    CASH_REFRESH,
    CASH_EXTRA_INVEN,
    CASH_EXTRA_WAREHOUSE,
    CASH_EXTRA_TACTICSHOUSE,
    CASH_CHANGE_TNAME,
    CASH_CHANGE_TAMER,
    CASH_CAPSULE_COIN_CLIENT,
    CASH_CAPSULE_COIN_SERVER,
    CASH_CAPSULE_COIN_WAIT,
    CASH_CAPSULE_COIN_COMPLEATE,
    CASH_ITEM_BUY,
    CASH_ITEM_PRESENT,
    POSTAL_REFRESH,
    GUILD_INVATE,
    GUILD_CREATE,
    GUILD_DESTROY,
    GUILD_LEAVE,
    GUILD_BAN,
    GUILD_AP_MASTER,
    GUILD_AP_2MASTER,
    GUILD_AP_DAT,
    GUILD_AP_NORMAL,
    GUILD_AP_NEWBI,
    FRIEND_LIST,
    SHUTOUT_LIST,
    CHANGE_KFM,
    CHANGE_KFM_ING,
    CHANGE_PARTNER,
    CHANGE_PARTNER_ING,
    CROSSCHANGE_PARTNER,
    CROSSCHANGE_PARTNER_ING,
    CROSSRECOVER_PARTNER,
    CROSSRECOVER_PARTNER_ING,
    CHANGE_PARTNER_WITHOUTSCENE,
    USE_DIGIMON_SKILL,
    DELETE_TACTICS,
    RIDE_ON,
    RIDE_OFF,
    TEMP_FRIEND_LOCAL_TO_SERVER,
    CASH_EXTRA_CROSSINVEN,
    REWARD_REFRESH,
    GET_REWARD,
    CASH_EXTRA_CARDINVEN,
    CASH_IDENTIFY_CARD,
    CASH_CARD_RESEAL,
    TACTICS_OPEN,
    MEMORY_SKILL_DELETE
}

public sealed record sCHANGE_PARTNER(
    uint UID,
    byte NextArrIDX,
    int NextType,
    byte NextLevel,
    ushort NextScale,
    string NextDigimonName);

public sealed record ServerSyncCommand(
    eSERVER_SYNC Sync,
    int RefConstant,
    IReadOnlyList<object?> Values,
    bool NetworkEnabled,
    DateTimeOffset CreatedAt);

public sealed class sSYNC
{
    private readonly HashSet<int> _refs = new();

    public IReadOnlyCollection<int> Refs => _refs;
    public int RefCount { get; private set; }
    public int EtcValue1 { get; private set; }
    public int EtcValue2 { get; private set; }

    public void Reset()
    {
        _refs.Clear();
        RefCount = 0;
        EtcValue1 = 0;
        EtcValue2 = 0;
    }

    public void Increase(int refConstant)
    {
        RefCount++;
        _refs.Add(refConstant);
    }

    public void Decrease(int refConstant)
    {
        if (RefCount <= 0)
        {
            return;
        }

        RefCount--;
        _refs.Remove(refConstant);
    }

    public bool HasRef(int refConstant) => _refs.Contains(refConstant);
    public void SetEtcValue1(int value) => EtcValue1 = value;
    public void SetEtcValue2(int value) => EtcValue2 = value;
}

public sealed class cData_ServerSync
{
    private static readonly HashSet<eSERVER_SYNC> SingleFlightSyncs =
    [
        eSERVER_SYNC.DIGIMON_EVOLUTION_OPEN,
        eSERVER_SYNC.DIGIMON_RIDE_OPEN,
        eSERVER_SYNC.ELEMENT_ITEM_CHECK,
        eSERVER_SYNC.TACTICSHOUSE_LIST,
        eSERVER_SYNC.CASH_REFRESH,
        eSERVER_SYNC.CASH_EXTRA_INVEN,
        eSERVER_SYNC.CASH_EXTRA_WAREHOUSE,
        eSERVER_SYNC.CASH_EXTRA_TACTICSHOUSE,
        eSERVER_SYNC.CASH_CHANGE_TNAME,
        eSERVER_SYNC.CASH_CHANGE_TAMER,
        eSERVER_SYNC.CASH_CAPSULE_COIN_CLIENT,
        eSERVER_SYNC.CASH_CAPSULE_COIN_SERVER,
        eSERVER_SYNC.CASH_CAPSULE_COIN_WAIT,
        eSERVER_SYNC.CASH_CAPSULE_COIN_COMPLEATE,
        eSERVER_SYNC.CASH_ITEM_BUY,
        eSERVER_SYNC.CASH_ITEM_PRESENT,
        eSERVER_SYNC.POSTAL_REFRESH,
        eSERVER_SYNC.GUILD_INVATE,
        eSERVER_SYNC.GUILD_CREATE,
        eSERVER_SYNC.GUILD_DESTROY,
        eSERVER_SYNC.GUILD_LEAVE,
        eSERVER_SYNC.GUILD_BAN,
        eSERVER_SYNC.GUILD_AP_MASTER,
        eSERVER_SYNC.GUILD_AP_2MASTER,
        eSERVER_SYNC.GUILD_AP_DAT,
        eSERVER_SYNC.GUILD_AP_NORMAL,
        eSERVER_SYNC.GUILD_AP_NEWBI,
        eSERVER_SYNC.FRIEND_LIST,
        eSERVER_SYNC.SHUTOUT_LIST,
        eSERVER_SYNC.CHANGE_KFM,
        eSERVER_SYNC.CHANGE_KFM_ING,
        eSERVER_SYNC.CHANGE_PARTNER,
        eSERVER_SYNC.CHANGE_PARTNER_ING,
        eSERVER_SYNC.CROSSCHANGE_PARTNER,
        eSERVER_SYNC.CROSSCHANGE_PARTNER_ING,
        eSERVER_SYNC.CROSSRECOVER_PARTNER,
        eSERVER_SYNC.CROSSRECOVER_PARTNER_ING,
        eSERVER_SYNC.CHANGE_PARTNER_WITHOUTSCENE,
        eSERVER_SYNC.USE_DIGIMON_SKILL,
        eSERVER_SYNC.DELETE_TACTICS,
        eSERVER_SYNC.RIDE_ON,
        eSERVER_SYNC.RIDE_OFF,
        eSERVER_SYNC.REWARD_REFRESH,
        eSERVER_SYNC.CASH_EXTRA_CARDINVEN,
        eSERVER_SYNC.CASH_IDENTIFY_CARD,
        eSERVER_SYNC.CASH_CARD_RESEAL,
        eSERVER_SYNC.TACTICS_OPEN,
        eSERVER_SYNC.MEMORY_SKILL_DELETE
    ];

    private readonly Dictionary<eSERVER_SYNC, sSYNC> _syncs = new();
    private readonly List<ServerSyncCommand> _pendingCommands = new();

    public cData_ServerSync()
    {
        Init();
    }

    public IReadOnlyList<ServerSyncCommand> PendingCommands => _pendingCommands;
    public eSERVER_SYNC? LastSendSync { get; private set; }
    public eSERVER_SYNC? LastRecvSync { get; private set; }
    public DateTimeOffset LastUpdatedAt { get; private set; }

    public void Delete()
    {
        Reset();
    }

    public void Init()
    {
        foreach (eSERVER_SYNC sync in Enum.GetValues<eSERVER_SYNC>())
        {
            _syncs.TryAdd(sync, new sSYNC());
        }

        Reset();
    }

    public void Reset()
    {
        foreach (sSYNC sync in _syncs.Values)
        {
            sync.Reset();
        }

        _pendingCommands.Clear();
        LastSendSync = null;
        LastRecvSync = null;
        LastUpdatedAt = DateTimeOffset.UtcNow;
    }

    public void Update()
    {
        if (!IsEmptyRefCount(eSERVER_SYNC.CASH_CAPSULE_COIN_WAIT) &&
            IsEmptyRefCount(eSERVER_SYNC.CASH_CAPSULE_COIN_CLIENT) &&
            IsEmptyRefCount(eSERVER_SYNC.CASH_CAPSULE_COIN_SERVER))
        {
            RecvServer(eSERVER_SYNC.CASH_CAPSULE_COIN_WAIT, 0);
        }
    }

    public bool SendServer(
        eSERVER_SYNC sync,
        int refConstant,
        bool networkEnabled = true,
        params object?[] values)
    {
        if (SingleFlightSyncs.Contains(sync) && !IsEmptyRefCount(sync))
        {
            return false;
        }

        sSYNC syncData = GetSyncData(sync);
        syncData.Increase(refConstant);
        CaptureEtcValues(syncData, values);
        _pendingCommands.Add(new ServerSyncCommand(sync, refConstant, values, networkEnabled, DateTimeOffset.UtcNow));
        LastSendSync = sync;
        LastUpdatedAt = DateTimeOffset.UtcNow;
        return true;
    }

    public bool RecvServer(eSERVER_SYNC sync, int refConstant, params object?[] values)
    {
        if (IsEmptyRefCount(sync))
        {
            return false;
        }

        GetSyncData(sync).Decrease(refConstant);
        LastRecvSync = sync;
        LastUpdatedAt = DateTimeOffset.UtcNow;
        return true;
    }

    public bool IsEmptyRefCount(eSERVER_SYNC sync)
    {
        return GetSyncData(sync).RefCount == 0;
    }

    public bool IsEmptyRefCount(eSERVER_SYNC sync, int refConstant)
    {
        sSYNC syncData = GetSyncData(sync);
        return syncData.RefCount == 0 || syncData.HasRef(refConstant);
    }

    public sSYNC GetSyncData(eSERVER_SYNC sync)
    {
        if (!_syncs.TryGetValue(sync, out sSYNC? value))
        {
            value = new sSYNC();
            _syncs[sync] = value;
        }

        return value;
    }

    public bool IsChangeParter()
    {
        return !IsEmptyRefCount(eSERVER_SYNC.CHANGE_PARTNER) ||
            !IsEmptyRefCount(eSERVER_SYNC.CHANGE_PARTNER_ING);
    }

    public bool IsChangeKFM()
    {
        return !IsEmptyRefCount(eSERVER_SYNC.CHANGE_KFM) ||
            !IsEmptyRefCount(eSERVER_SYNC.CHANGE_KFM_ING);
    }

    public bool IsCrossChangeParter()
    {
        return !IsEmptyRefCount(eSERVER_SYNC.CROSSCHANGE_PARTNER) ||
            !IsEmptyRefCount(eSERVER_SYNC.CROSSCHANGE_PARTNER_ING);
    }

    public bool IsCrossRecover()
    {
        return !IsEmptyRefCount(eSERVER_SYNC.CROSSRECOVER_PARTNER) ||
            !IsEmptyRefCount(eSERVER_SYNC.CROSSRECOVER_PARTNER_ING);
    }

    public bool IsChageDigimon()
    {
        return IsChangeParter() || IsChangeKFM() || IsCrossChangeParter() || IsCrossRecover();
    }

    public bool IsSyncActive(eSERVER_SYNC sync) => !IsEmptyRefCount(sync);

    private static void CaptureEtcValues(sSYNC syncData, IReadOnlyList<object?> values)
    {
        if (values.Count > 0 && TryToInt(values[0], out int first))
        {
            syncData.SetEtcValue1(first);
        }

        if (values.Count > 1 && TryToInt(values[1], out int second))
        {
            syncData.SetEtcValue2(second);
        }
    }

    private static bool TryToInt(object? value, out int result)
    {
        switch (value)
        {
            case byte byteValue:
                result = byteValue;
                return true;
            case short shortValue:
                result = shortValue;
                return true;
            case ushort ushortValue:
                result = ushortValue;
                return true;
            case int intValue:
                result = intValue;
                return true;
            case uint uintValue when uintValue <= int.MaxValue:
                result = (int)uintValue;
                return true;
            case long longValue when longValue is >= int.MinValue and <= int.MaxValue:
                result = (int)longValue;
                return true;
            default:
                result = 0;
                return false;
        }
    }
}
