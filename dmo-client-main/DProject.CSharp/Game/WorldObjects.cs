using DProject.Data.CsFileTable;
using DProject.Gamebryo;
using DProject.Network;
using DProject.Network.Protocol;

namespace DProject.Game;

public enum eDELETE_STATE
{
    DS_NONE,
    DS_WAIT,
    DS_DELETE
}

public sealed record sCHANGE_PART_INFO(int s_nPartIndex, uint s_nFileTableID, uint s_nRemainTime);

public sealed record BuffState(ushort BuffCode, ushort Level, uint EndTimestamp, uint SkillCode);

public sealed class cCondition
{
    public uint Value { get; private set; }
    public uint ExtBuff { get; private set; }

    public void SetCondition(uint condition, uint extBuff = 0)
    {
        Value = condition;
        ExtBuff = extBuff;
    }

    public bool IsCondition(uint condition)
    {
        return (Value & condition) == condition;
    }
}

public sealed class cBuffData
{
    private readonly Dictionary<ushort, BuffState> _buffs = new();

    public IReadOnlyDictionary<ushort, BuffState> Buffs => _buffs;

    public void SetBuff(ushort buffCode, uint endTimestamp, ushort level, uint skillCode)
    {
        _buffs[buffCode] = new BuffState(buffCode, level, endTimestamp, skillCode);
    }

    public void ReleaseBuff(ushort buffCode)
    {
        _buffs.Remove(buffCode);
    }

    public void ReplaceAll(IEnumerable<SyncBuffEntry> buffs)
    {
        _buffs.Clear();
        foreach (SyncBuffEntry buff in buffs)
        {
            SetBuff(buff.BuffCode, buff.EndTimestamp, buff.Level, buff.SkillCode);
        }
    }
}

public sealed class CBaseStat
{
    public byte Level { get; private set; }
    public byte HpRate { get; private set; }
    public float MoveSpeed { get; private set; }
    public ushort AttackSpeed { get; private set; }

    public void SetLevel(byte level) => Level = level;
    public void SetHP(byte hpRate) => HpRate = hpRate;
    public void SetMoveSpeed(float moveSpeed) => MoveSpeed = moveSpeed;
    public void SetAttackSpeed(ushort attackSpeed) => AttackSpeed = attackSpeed;
}

public sealed class CCharOption
{
    public uint GuildUid { get; private set; }
    public string GuildName { get; private set; } = string.Empty;
    public ushort Achievement { get; private set; }
    public byte MatchTeam { get; private set; }

    public void SetGuildName(string guildName) => GuildName = guildName;
    public void DeleteGuildName() => GuildName = string.Empty;
    public void SetGuildUID(uint guildUid) => GuildUid = guildUid;
    public void SetAchieve(ushort achievement) => Achievement = achievement;
    public void SetMatchIcon(byte team) => MatchTeam = team;
}

public class CsC_AvObject
{
    public uint UID { get; private set; }
    public uint IDX { get; private set; }
    public uint ClassID { get; private set; }
    public uint FileTableID { get; private set; }
    public string Name { get; protected set; } = string.Empty;
    public NiPoint3 Position { get; private set; }
    public NiPoint3 Destination { get; private set; }
    public float Rotation { get; private set; }
    public bool IsPresent { get; private set; }
    public eDELETE_STATE DeleteState { get; private set; }
    public CBaseStat BaseStat { get; } = new();
    public cCondition Condition { get; } = new();
    public cBuffData Buff { get; } = new();
    public CCharOption CharOption { get; } = new();

    public virtual void Init(uint uid, uint idx, uint classId, uint fileTableId, NiPoint3 position, float rotation, string name = "")
    {
        UID = uid;
        IDX = idx;
        ClassID = classId;
        FileTableID = fileTableId;
        Position = position;
        Destination = position;
        Rotation = rotation;
        Name = name;
        IsPresent = true;
        DeleteState = eDELETE_STATE.DS_NONE;
    }

    public virtual void Delete()
    {
        IsPresent = false;
        DeleteState = eDELETE_STATE.DS_DELETE;
    }

    public virtual void PreDelete()
    {
        DeleteState = eDELETE_STATE.DS_WAIT;
    }

    public virtual void Update(float deltaTime, bool animation = true)
    {
    }

    public virtual void Render()
    {
    }

    public virtual void Render_PostChar()
    {
    }

    public virtual uint GetClass() => ClassID;
    public virtual uint GetUniqID() => UID;
    public virtual string GetName() => Name;
    public virtual bool IsRenderName() => IsPresent && !string.IsNullOrEmpty(Name);
    public virtual bool IsLoad() => IsPresent;
    public virtual float GetToolHeight() => 100.0f;
    public NiPoint3 GetPos() => Position;
    public CBaseStat GetBaseStat() => BaseStat;
    public cCondition GetCondition() => Condition;
    public cBuffData GetBuff() => Buff;
    public CCharOption GetCharOption() => CharOption;

    public void SetPos(NiPoint3 position)
    {
        Position = position;
        Destination = position;
    }

    public void SetPath(int x, int y)
    {
        Destination = new NiPoint3(x, y, Position.Z);
        Position = Destination;
    }

    public void SetRotation(float rotation, bool immediate = true)
    {
        Rotation = rotation;
    }

    public void SetCondition(uint condition, uint extBuff = 0)
    {
        Condition.SetCondition(condition, extBuff);
    }

    public void SetPresent(bool present)
    {
        IsPresent = present;
        DeleteState = present ? eDELETE_STATE.DS_NONE : eDELETE_STATE.DS_DELETE;
    }
}

public class CsC_PartObject : CsC_AvObject
{
    private readonly Dictionary<int, sCHANGE_PART_INFO> _parts = new();

    public IReadOnlyDictionary<int, sCHANGE_PART_INFO> Parts => _parts;

    public virtual void ThreadLoad_Part(IEnumerable<sCHANGE_PART_INFO> partInfo, bool includeBaseKfm = true)
    {
        foreach (sCHANGE_PART_INFO part in partInfo)
        {
            _parts[part.s_nPartIndex] = part;
        }
    }

    public virtual void ChangePart(sCHANGE_PART_INFO partInfo, bool compel = false)
    {
        _parts[partInfo.s_nPartIndex] = partInfo;
    }

    public virtual void DeletePart(int partIndex)
    {
        _parts.Remove(partIndex);
    }
}

public class CsC_DPartObject : CsC_AvObject
{
    public uint PartsType { get; private set; }

    public virtual void ChangeParts(uint partsType)
    {
        PartsType = partsType;
    }
}

public class CTamer : CsC_PartObject
{
    public enum eTAMER_STATE
    {
        TAMER_NORMAL,
        TAMER_BATTLE,
        TAMER_DIE
    }

    public enum eTAMER_MOVE
    {
        TM_NONE = 1000,
        TM_RIDE
    }

    public eTAMER_STATE TamerState { get; private set; } = eTAMER_STATE.TAMER_NORMAL;
    public eTAMER_MOVE TamerMove { get; private set; } = eTAMER_MOVE.TM_NONE;
    public uint SyncOption { get; private set; }
    public int DigimonLinkID { get; private set; }
    public uint PcbangCostume { get; private set; }
    public uint ChangeCostumeNo { get; private set; }
    public string ShopTitle { get; private set; } = string.Empty;

    public void ApplySnapshot(SyncWorldObjectSnapshot snapshot, SyncTamerPayload payload)
    {
        Init(
            snapshot.ObjectType.Uid,
            snapshot.ObjectType.Index,
            snapshot.ObjectType.ClassId,
            snapshot.ObjectType.TypeId,
            ToPoint(snapshot.Position),
            payload.Direction,
            payload.Name);
        SetPath(payload.Destination.X, payload.Destination.Y);
        BaseStat.SetLevel(payload.Level);
        BaseStat.SetMoveSpeed(payload.MoveSpeed);
        BaseStat.SetHP(payload.HpRate);
        SetCondition(payload.Condition);
        SyncOption = payload.SyncFlags;
        SetDigimonLink((int)DmoObjectType.FromUid(payload.PartnerUid).Index);
        CharOption.SetAchieve(payload.Achievement);
        CharOption.SetMatchIcon(payload.MatchTeam);
        if (payload.GuildClass == 0)
        {
            CharOption.DeleteGuildName();
        }
        else
        {
            CharOption.SetGuildUID(payload.GuildIndex);
            CharOption.SetGuildName(payload.GuildName);
        }

        ShopTitle = payload.ShopTitle;
        ChangeCostumeNo = payload.ChangeCostumeNo;
        ThreadLoad_Part(payload.EquipItems.Select((item, index) => new sCHANGE_PART_INFO(index, item.Type, item.EndTime)));
        if (payload.DigiviceItem is not null)
        {
            ChangePart(new sCHANGE_PART_INFO(14, payload.DigiviceItem.Type, payload.DigiviceItem.EndTime));
        }
    }

    public void SetBattle() => TamerState = eTAMER_STATE.TAMER_BATTLE;
    public void SetBattleEnd() => TamerState = eTAMER_STATE.TAMER_NORMAL;
    public bool IsBattle() => TamerState == eTAMER_STATE.TAMER_BATTLE;
    public void SetDigimonLink(int digimonIdx) => DigimonLinkID = digimonIdx;
    public int GetDigimonLink() => DigimonLinkID;
    public void SetPcbangCostume(uint costumeId) => PcbangCostume = costumeId;

    private static NiPoint3 ToPoint(SyncPosition position) => new(position.X, position.Y, 0.0f);
}

public class CDigimon : CsC_DPartObject
{
    public enum eDIGIMON_STATE
    {
        DIGIMON_NORMAL,
        DIGIMON_BATTLE,
        DIGIMON_EVOL,
        DIGIMON_CHANGE_PARTNER_START,
        DIGIMON_CHANGE_PARTNER_END,
        DIGIMON_DIE
    }

    public enum eDIGIMON_MOVE
    {
        DM_NONE = 1000,
        DM_RIDE
    }

    public eDIGIMON_STATE DigimonState { get; private set; } = eDIGIMON_STATE.DIGIMON_NORMAL;
    public eDIGIMON_MOVE DigimonMove { get; private set; } = eDIGIMON_MOVE.DM_NONE;
    public int TamerLinkID { get; private set; }
    public ushort Scale { get; private set; }
    public ushort EnchantLevel { get; private set; }
    public IReadOnlyList<ushort> ExtensionParameterLevels { get; private set; } = [];
    public byte? BattleTeam { get; private set; }

    public void ApplySnapshot(SyncWorldObjectSnapshot snapshot, SyncDigimonPayload payload)
    {
        Init(
            snapshot.ObjectType.Uid,
            snapshot.ObjectType.Index,
            snapshot.ObjectType.ClassId,
            snapshot.ObjectType.TypeId,
            ToPoint(snapshot.Position),
            payload.Direction,
            payload.Name);
        SetPath(payload.Destination.X, payload.Destination.Y);
        Scale = payload.Scale;
        BaseStat.SetLevel(payload.Level);
        BaseStat.SetMoveSpeed(payload.MoveSpeed);
        BaseStat.SetAttackSpeed(payload.AttackSpeed);
        BaseStat.SetHP(payload.HpRate);
        SetCondition(payload.Condition);
        SetTamerLink((int)DmoObjectType.FromUid(payload.TamerUid).Index);
        BattleTeam = payload.BattleTeam;
        EnchantLevel = payload.EnchantLevel;
        ExtensionParameterLevels = payload.ExtensionParameterLevels.ToArray();
        if (payload.PartsType is { } partsType)
        {
            ChangeParts(partsType);
        }
    }

    public void SetDigimonState(eDIGIMON_STATE state) => DigimonState = state;
    public eDIGIMON_STATE GetDigimonState() => DigimonState;
    public void SetBattle() => DigimonState = eDIGIMON_STATE.DIGIMON_BATTLE;
    public void SetBattleEnd() => DigimonState = eDIGIMON_STATE.DIGIMON_NORMAL;
    public void SetDie() => DigimonState = eDIGIMON_STATE.DIGIMON_DIE;
    public bool IsBattle() => DigimonState == eDIGIMON_STATE.DIGIMON_BATTLE;
    public bool IsDie() => DigimonState == eDIGIMON_STATE.DIGIMON_DIE;
    public void SetTamerLink(int tamerIdx) => TamerLinkID = tamerIdx;
    public int GetTamerLink() => TamerLinkID;

    private static NiPoint3 ToPoint(SyncPosition position) => new(position.X, position.Y, 0.0f);
}

public sealed class CMonster : CsC_AvObject
{
    public enum eMONSTER_STATE
    {
        MONSTER_CREATE,
        MONSTER_NORMAL,
        MONSTER_BATTLE,
        MONSTER_DIE
    }

    public eMONSTER_STATE MonsterState { get; private set; } = eMONSTER_STATE.MONSTER_NORMAL;
    public bool IsCastingMonster { get; private set; }
    public uint OwnerID { get; private set; }
    public IReadOnlyList<SyncMonsterSeedEffect> SeedEffects { get; private set; } = [];

    public void ApplySnapshot(SyncWorldObjectSnapshot snapshot, SyncMonsterPayload payload)
    {
        Init(
            snapshot.ObjectType.Uid,
            snapshot.ObjectType.Index,
            snapshot.ObjectType.ClassId,
            snapshot.ObjectType.TypeId,
            ToPoint(snapshot.Position),
            Rotation,
            string.Empty);
        SetPath(payload.Destination.X, payload.Destination.Y);
        BaseStat.SetLevel(payload.Level);
        BaseStat.SetHP(payload.HpRate);
        SetCondition(payload.Condition);
        IsCastingMonster = payload.CastingSkillIndex.HasValue;
        SeedEffects = payload.SeedEffects.ToArray();
        MonsterState = snapshot.IsNew ? eMONSTER_STATE.MONSTER_CREATE : eMONSTER_STATE.MONSTER_NORMAL;
    }

    public eMONSTER_STATE GetMonsterState() => MonsterState;
    public bool IsMonsterState(eMONSTER_STATE state) => MonsterState == state;
    public void SetBattle() => MonsterState = eMONSTER_STATE.MONSTER_BATTLE;
    public void SetBattleEnd() => MonsterState = eMONSTER_STATE.MONSTER_NORMAL;
    public void SetDie() => MonsterState = eMONSTER_STATE.MONSTER_DIE;
    public bool IsCasting_Monster() => IsCastingMonster;
    public void SetCasting_Monster(bool casting) => IsCastingMonster = casting;
    public void SetOwnerID(uint uid) => OwnerID = uid;
    public uint GetOwnerID() => OwnerID;

    private static NiPoint3 ToPoint(SyncPosition position) => new(position.X, position.Y, 0.0f);
}

public sealed class CTamerUser : CTamer
{
    public enum eTAMERUSER_STATE
    {
        TU_NONE,
        TU_SKILL,
        TU_WAIT_REV_SKILL,
        TU_USE_TARGET_ITEM,
        TU_TARGET_RIDE_END,
        TU_KEYBOARD_MOVE,
        TU_FOLLOW_DIGIMON
    }

    public enum eRIDE_END_STATE
    {
        RES_NONE,
        RES_STOP,
        RES_ATTACK,
        RES_SKILL
    }

    public uint ServerOption { get; private set; }
    public eTAMERUSER_STATE TamerUserState { get; private set; } = eTAMERUSER_STATE.TU_NONE;
    public eRIDE_END_STATE RideEndState { get; private set; } = eRIDE_END_STATE.RES_NONE;
    public uint LastNearPortalID { get; private set; }

    public void InitLocal(DmoObjectType objectType, SyncPosition position, string name, uint serverOption)
    {
        Init(
            objectType.Uid,
            objectType.Index,
            objectType.ClassId,
            objectType.TypeId,
            new NiPoint3(position.X, position.Y, 0.0f),
            0.0f,
            name);
        ServerOption = serverOption;
    }

    public uint GetServerOption() => ServerOption;
    public bool IsServerOption(uint flag) => (ServerOption & flag) != 0;
    public void SetServerOption(uint serverOption) => ServerOption = serverOption;
    public void AddServerOption(uint flag) => ServerOption |= flag;
    public void RemoveServerOption(uint flag) => ServerOption &= ~flag;
    public void SetTamerUserState(eTAMERUSER_STATE state) => TamerUserState = state;
    public eTAMERUSER_STATE GetTamerUserState() => TamerUserState;
    public void SetLastNearPortalID(uint portalId) => LastNearPortalID = portalId;
}

public sealed class CDigimonUser : CDigimon
{
    public enum eDIGIMONUSER_STATE
    {
        DU_WAIT,
        DU_FOLLOW,
        DU_ATTACK,
        DU_SKILL,
        DU_WAIT_REV_SKILL,
        DU_NPCFIND,
        DU_MANUAL_CONTROL,
        DU_KEYBOARD_MOVE
    }

    public uint BaseDigimonID { get; private set; }
    public int DigimonUserIndex { get; private set; }
    public eDIGIMONUSER_STATE DigimonUserState { get; private set; } = eDIGIMONUSER_STATE.DU_WAIT;
    public eDIGIMONUSER_STATE DigimonUserStateNoneSkill { get; private set; } = eDIGIMONUSER_STATE.DU_WAIT;
    public bool ServerDie { get; private set; }

    public void InitLocal(
        DmoObjectType objectType,
        uint baseDigimonId,
        SyncPosition position,
        float rotation,
        string name,
        int userIndex)
    {
        Init(
            objectType.Uid,
            objectType.Index,
            objectType.ClassId,
            objectType.TypeId,
            new NiPoint3(position.X, position.Y, 0.0f),
            rotation,
            name);
        BaseDigimonID = baseDigimonId;
        DigimonUserIndex = userIndex;
    }

    public int GetDigimonUserIndex() => DigimonUserIndex;
    public void SetDigimonUserState(eDIGIMONUSER_STATE state)
    {
        DigimonUserState = state;
        if (state != eDIGIMONUSER_STATE.DU_SKILL && state != eDIGIMONUSER_STATE.DU_WAIT_REV_SKILL)
        {
            DigimonUserStateNoneSkill = state;
        }
    }

    public eDIGIMONUSER_STATE GetDigimonUserState() => DigimonUserState;
    public void SetServerDie(bool die) => ServerDie = die;
}

public sealed class CNpc : CsC_AvObject
{
    public enum eNPC_RENDER_TYPE
    {
        NRT_NORMAL,
        NRT_DIGIVICE,
        NRT_TEMP
    }

    public enum eQUEST_VIEWTYPE
    {
        QV_NONE,
        QV_NPCTALK
    }

    public NpcBinRecord? FT_Npc { get; private set; }
    public MapNpcRecord? FT_MapNpc { get; private set; }
    public eNPC_RENDER_TYPE NpcRenderType { get; private set; } = eNPC_RENDER_TYPE.NRT_NORMAL;
    public eQUEST_VIEWTYPE QuestViewType { get; private set; } = eQUEST_VIEWTYPE.QV_NONE;
    public bool EnableUse { get; private set; } = true;
    public string PenName { get; private set; } = string.Empty;

    public void Init(NpcBinRecord npc, MapNpcRecord mapNpc, uint uid)
    {
        FT_Npc = npc;
        FT_MapNpc = mapNpc;
        NpcRenderType = eNPC_RENDER_TYPE.NRT_NORMAL;
        EnableUse = true;
        PenName = npc.PenName;
        base.Init(
            uid,
            mapNpc.NpcId,
            nClass.Npc,
            npc.NpcId,
            new NiPoint3(mapNpc.InitPosX, mapNpc.InitPosY, 0.0f),
            mapNpc.Rotation,
            npc.Name);
        GetBaseStat().SetHP(byte.MaxValue);
    }

    public void Init_NoFT(uint uid, eNPC_RENDER_TYPE renderType)
    {
        FT_Npc = null;
        FT_MapNpc = null;
        NpcRenderType = renderType;
        EnableUse = true;
        PenName = string.Empty;
        base.Init(uid, uid & 0x3FFF, nClass.Npc, 0, NiPoint3.Zero, 0.0f);
        GetBaseStat().SetHP(byte.MaxValue);
    }

    public void Init_NoMapFT(uint uid, uint fileTableId, NpcBinRecord? npc, eNPC_RENDER_TYPE renderType)
    {
        FT_Npc = npc;
        FT_MapNpc = null;
        NpcRenderType = renderType;
        EnableUse = true;
        PenName = npc?.PenName ?? string.Empty;
        base.Init(uid, uid & 0x3FFF, nClass.Npc, fileTableId, NiPoint3.Zero, 0.0f, npc?.Name ?? string.Empty);
        GetBaseStat().SetHP(byte.MaxValue);
    }

    public void InitManual(uint uid, uint fileTableId, SyncPosition position, float rotation, string name)
    {
        FT_Npc = null;
        FT_MapNpc = null;
        NpcRenderType = eNPC_RENDER_TYPE.NRT_NORMAL;
        EnableUse = true;
        PenName = string.Empty;
        base.Init(uid, uid & 0x3FFF, nClass.Npc, fileTableId, new NiPoint3(position.X, position.Y, 0.0f), rotation, name);
        GetBaseStat().SetHP(byte.MaxValue);
    }

    public override uint GetClass() => nClass.Npc;
    public uint GetModelID() => FT_Npc?.ModelId ?? FileTableID;
    public bool IsDisplayType(uint displayType) => (FT_Npc?.DisplayFlag & displayType) != 0;
    public bool IsPickEnable() => IsPresent && EnableUse;
    public void SetEnableUse(bool enable) => EnableUse = enable;
    public bool IsEnableUse() => EnableUse;
    public void SetQuestViewType(eQUEST_VIEWTYPE questViewType) => QuestViewType = questViewType;
    public eQUEST_VIEWTYPE GetQuestViewType() => QuestViewType;
    public int GetNPCType() => FT_Npc?.TypePayload?.NpcType ?? FT_Npc?.Type ?? 0;
    public bool CheckTamerDist(float dist = 0.0f) => EnableUse;
    public void Try_Talk() { }
    public void Try_Yes() { }
    public void Try_No() { }
    public void SetOpenAnimation() { }
    public void SetCloseAnimation() { }
    public object? PlaySound(string soundPath) => null;
}

public sealed class CFieldItem : CsC_AvObject
{
    public uint OwnerTamerUID { get; private set; }
    public byte ItemState { get; private set; }

    public void ApplySnapshot(SyncWorldObjectSnapshot snapshot, SyncItemPayload payload)
    {
        Init(
            snapshot.ObjectType.Uid,
            snapshot.ObjectType.Index,
            snapshot.ObjectType.ClassId,
            snapshot.ObjectType.TypeId,
            new NiPoint3(snapshot.Position.X, snapshot.Position.Y, 0.0f),
            0.0f);
        OwnerTamerUID = payload.OwnerTamerUid;
        ItemState = payload.ItemState;
    }
}

public sealed class CEmployment : CsC_AvObject
{
    public uint ItemType { get; private set; }
    public string ShopName { get; private set; } = string.Empty;

    public void ApplySnapshot(SyncWorldObjectSnapshot snapshot, SyncCommissionShopPayload payload)
    {
        Init(
            snapshot.ObjectType.Uid,
            snapshot.ObjectType.Index,
            snapshot.ObjectType.ClassId,
            snapshot.ObjectType.TypeId,
            new NiPoint3(snapshot.Position.X, snapshot.Position.Y, 0.0f),
            payload.Direction,
            payload.ShopName);
        ItemType = payload.ItemType;
        ShopName = string.IsNullOrWhiteSpace(payload.ShopName) ? "???" : payload.ShopName;
    }
}
