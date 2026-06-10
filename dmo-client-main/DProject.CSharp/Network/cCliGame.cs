using System.Buffers.Binary;
using DProject.Data.CsFileTable;
using DProject.Network.Protocol;

namespace DProject.Network;

public sealed class cCliGame : cClient
{
    private const int GameServerLibType = 1;
    private const int BattleServerLibType = 2;
    private const int TamerEquipCount = pLimit.TamerEquipCount;
    private const int CItemDataCompat487Size = pLimit.ItemDataBytesCompat487;
    private const int DigimonExtensionStatCount = 7;
    private const uint SyncConditionShop = 0x00000004;
    private const uint MonsterCastingCondition = 6;
    private const ushort MonsterSkillTypeAttachSeed = 18;
    private const bool HasTamerDigiviceSync = true;
    private const bool HasMastersMatchingSync = true;
    private const bool HasMonsterSkillGrowthSync = true;
    private const bool HasDigimonPartsSystemSync = false;

    private GameConnectionInfo? _gameConnection;
    private CsFileTableMng? _fileTableMng;

    public cCliGame() : base(nameof(cCliGame))
    {
    }

    public event Action<ChannelInfo>? ChannelInfoReceived;
    public event Action<GameInitData>? InitGameDataReceived;
    public event Action<GameSyncSummary>? SyncReceived;
    public event Action<PortalTransitionInfo>? PortalTransitionReceived;
    public event Action<LocalPortalInfo>? LocalPortalReceived;
    public event Action? ChangeServerReceived;
    public event Action<StartMessageInfo>? StartMessageReceived;
    public event Action<VersionErrorInfo>? VersionErrorReceived;
    public event Action<NetworkPacketEnvelope>? UnknownPacketReceived;

    public uint ConnectedTickCount { get; private set; }
    public uint IsChangeServer { get; private set; }
    public uint NetVersion { get; set; } = DmoVersion.UsaNetVersion;
    public string TamerName { get; set; } = string.Empty;
    public bool PortalProcessing { get; set; }
    public bool PortalRequesting { get; set; }
    public bool IsReadyCompleted { get; set; }
    public int ServerLibType { get; private set; } = GameServerLibType;

    public override void OnConnected()
    {
        SendAccessCode();
    }

    public override int OnExecute(int maxPackets = 256)
    {
        if (IsConnected)
        {
            ConnectedTickCount++;
        }

        return base.OnExecute(maxPackets);
    }

    public void ConfigureAccess(GameConnectionInfo gameConnection)
    {
        _gameConnection = gameConnection;
        if (IsConnected)
        {
            SendAccessCode();
        }
    }

    public void ConfigureFileTables(CsFileTableMng fileTableMng)
    {
        _fileTableMng = fileTableMng;
    }

    public void SendAccessCode()
    {
        if (_gameConnection is null)
        {
            return;
        }

        var packet = new cPacket();
        uint port = (uint)_gameConnection.Port;
        packet.PushUInt32((port ^ _gameConnection.AccessCode) | _gameConnection.AccountIndex);
        packet.PushUInt32(_gameConnection.AccountIndex);
        packet.PushUInt32(_gameConnection.AccessCode);
        packet.PushUInt32(NetVersion);
        Send(pSvr.AccessCode, packet);
    }

    public void SendChannelInfo()
    {
        Send(pSvr.ChannelInfo, new cPacket());
    }

    public void SendReady(int x, int y)
    {
        var packet = new cPacket();
        packet.PushInt32(x);
        packet.PushInt32(y);
        packet.PushUInt32(NetVersion);
        Send(pGame.Ready, packet);
        IsReadyCompleted = true;
    }

    public void SendSelectPortal(uint portalNo)
    {
        if (portalNo == 0 || PortalRequesting)
        {
            return;
        }

        PortalRequesting = true;
        var packet = new cPacket();
        packet.PushUInt32(portalNo);
        Send(pSvr.SelectPortal, packet);
    }

    public void SendChangeServer()
    {
        IsChangeServer++;
        Send(pSvr.Change, new cPacket());
    }

    public void SendAttendance()
    {
        Send(0, new cPacket());
    }

    protected override void OnPacket(NetworkPacketEnvelope packet)
    {
        switch (packet.PacketId)
        {
            case ushort.MaxValue:
                _ = packet.Packet.PopInt16();
                return;
            case pNet.VersionError:
                VersionErrorReceived?.Invoke(ReadVersionError(packet.Packet));
                return;
            case pGame.StartMessage:
                StartMessageReceived?.Invoke(ReadStartMessage(packet.Packet));
                return;
            case pGame.InitData:
                GameInitData initData = ReadInitGameData(packet.Packet);
                ServerLibType = initData.ServerLibType;
                InitGameDataReceived?.Invoke(initData);
                return;
            case pSvr.ChannelInfo:
                ChannelInfoReceived?.Invoke(ReadChannelInfo(packet.Packet));
                return;
            case pGame.Sync:
                SyncReceived?.Invoke(ReadSyncSummary(packet.Packet, ServerLibType));
                return;
            case pSvr.SelectPortal:
                PortalTransitionReceived?.Invoke(ReadPortalTransition(packet.Packet));
                return;
            case pSvr.SelectPortalFailure:
                PortalRequesting = false;
                PortalProcessing = false;
                UnknownPacketReceived?.Invoke(packet);
                return;
            case pSvr.LocalPortal:
                PortalRequesting = false;
                PortalProcessing = false;
                LocalPortalReceived?.Invoke(ReadLocalPortal(packet.Packet));
                return;
            case pSvr.Change:
                ChangeServerReceived?.Invoke();
                return;
            case pGame.BattleOn:
            case pGame.BattleOff:
            case pGame.AllStat:
            case pGame.HpRate:
            case pGame.DiedAlready:
            case pGame.ChangeState:
            case pGame.RefreshScreen:
            case pGameTwo.GameGuardCheck:
            case pGameTwo.XignCodeCheck:
                UnknownPacketReceived?.Invoke(packet);
                return;
            default:
                if (!IsReadyCompleted)
                {
                    UnknownPacketReceived?.Invoke(packet);
                    return;
                }

                UnknownPacketReceived?.Invoke(packet);
                return;
        }
    }

    private static ChannelInfo ReadChannelInfo(cPacket packet)
    {
        var channels = new Dictionary<byte, byte>();
        while (packet.Remaining > 0)
        {
            byte channelIndex = packet.PopByte();
            if (channelIndex == byte.MaxValue)
            {
                break;
            }

            channels[channelIndex] = packet.PopByte();
        }

        return new ChannelInfo(channels);
    }

    private static GameInitData ReadInitGameData(cPacket packet)
    {
        int serverLibType = packet.PopInt32();
        int x = packet.PopInt32();
        int y = packet.PopInt32();
        ulong tamerTypeAll = packet.PopUInt64();
        string tamerName = packet.PopString(consumeTrailingNull: true);
        return new GameInitData(serverLibType, x, y, tamerTypeAll, tamerName);
    }

    private GameSyncSummary ReadSyncSummary(cPacket packet, int serverLibType)
    {
        if (packet.Remaining == 0)
        {
            return new GameSyncSummary(
                pSync.None,
                0,
                Array.Empty<byte>(),
                Array.Empty<SyncMovement>(),
                Array.Empty<SyncRotation>(),
                Array.Empty<SyncPresence>(),
                Array.Empty<SyncWorldObjectSnapshot>(),
                Array.Empty<SyncConditionChange>(),
                Array.Empty<SyncChatMessage>(),
                Array.Empty<SyncBuffObject>(),
                false);
        }

        var syncTypes = new List<byte>();
        var movements = new List<SyncMovement>();
        var rotations = new List<SyncRotation>();
        var presenceChanges = new List<SyncPresence>();
        var worldObjects = new List<SyncWorldObjectSnapshot>();
        var conditionChanges = new List<SyncConditionChange>();
        var chatMessages = new List<SyncChatMessage>();
        var buffObjects = new List<SyncBuffObject>();
        bool stoppedAtUnparsedPayload = false;
        byte first = pSync.None;

        while (packet.Remaining > 0 && syncTypes.Count < 64)
        {
            byte syncType = packet.PopByte();
            if (syncTypes.Count == 0)
            {
                first = syncType;
            }

            syncTypes.Add(syncType);
            if (syncType == pSync.None)
            {
                break;
            }

            switch (syncType)
            {
                case pSync.Walk:
                case pSync.Move:
                    ReadMovementBlock(packet, syncType, movements);
                    break;
                case pSync.Rotation:
                    ReadRotationBlock(packet, rotations);
                    break;
                case pSync.Out:
                    ReadOutBlock(packet, presenceChanges);
                    break;
                case pSync.Delete:
                    ReadDeleteBlock(packet, presenceChanges);
                    break;
                case pSync.New:
                case pSync.In:
                    stoppedAtUnparsedPayload = ReadPresenceObjectBlock(packet, syncType, serverLibType, presenceChanges, worldObjects);
                    break;
                case pSync.Condition:
                    ReadConditionBlock(packet, conditionChanges);
                    break;
                case pSync.MsgChat:
                    ReadChatBlock(packet, chatMessages);
                    break;
                case pSync.MsgWhisper:
                    ReadWhisperBlock(packet, chatMessages);
                    break;
                case pSync.MsgAll:
                case pSync.MsgShout:
                case pSync.MsgGuildShout:
                    ReadSourceMessageBlock(packet, syncType, chatMessages, syncType == pSync.MsgGuildShout);
                    break;
                case pSync.MsgSystem:
                    ReadSystemMessageBlock(packet, chatMessages);
                    break;
                case pSync.MsgAllByItem:
                    ReadItemBroadcastBlock(packet, chatMessages);
                    break;
                case pSync.BuffSync:
                    ReadBuffSyncBlock(packet, buffObjects);
                    break;
                default:
                    stoppedAtUnparsedPayload = true;
                    break;
            }

            if (stoppedAtUnparsedPayload)
            {
                break;
            }
        }

        return new GameSyncSummary(
            first,
            packet.Remaining,
            syncTypes,
            movements,
            rotations,
            presenceChanges,
            worldObjects,
            conditionChanges,
            chatMessages,
            buffObjects,
            stoppedAtUnparsedPayload);
    }

    private static void ReadMovementBlock(cPacket packet, byte syncType, List<SyncMovement> movements)
    {
        ushort count = packet.PopUInt16();
        for (int i = 0; i < count; i++)
        {
            DmoObjectType objectType = DmoObjectType.FromUid(packet.PopUInt32());
            SyncPosition position = ReadPosition(packet);
            movements.Add(new SyncMovement(syncType, objectType, position, syncType == pSync.Walk));
        }
    }

    private static void ReadRotationBlock(cPacket packet, List<SyncRotation> rotations)
    {
        ushort count = packet.PopUInt16();
        for (int i = 0; i < count; i++)
        {
            DmoObjectType objectType = DmoObjectType.FromUid(packet.PopUInt32());
            rotations.Add(new SyncRotation(objectType, packet.PopSingle()));
        }
    }

    private static void ReadOutBlock(cPacket packet, List<SyncPresence> presenceChanges)
    {
        ushort count = packet.PopUInt16();
        for (int i = 0; i < count; i++)
        {
            DmoObjectType objectType = DmoObjectType.FromUid(packet.PopUInt32());
            SyncPosition position = ReadPosition(packet);
            presenceChanges.Add(new SyncPresence(pSync.Out, objectType, position));
        }
    }

    private static void ReadDeleteBlock(cPacket packet, List<SyncPresence> presenceChanges)
    {
        ushort count = packet.PopUInt16();
        for (int i = 0; i < count; i++)
        {
            DmoObjectType objectType = DmoObjectType.FromUid(packet.PopUInt32());
            presenceChanges.Add(new SyncPresence(pSync.Delete, objectType, null));
        }
    }

    private static void ReadConditionBlock(cPacket packet, List<SyncConditionChange> conditionChanges)
    {
        ushort count = packet.PopUInt16();
        for (int i = 0; i < count; i++)
        {
            DmoObjectType objectType = DmoObjectType.FromUid(packet.PopUInt32());
            uint condition = packet.PopUInt32();
            uint extBuff = objectType.ClassId == nClass.Tamer ? packet.PopUInt32() : 0;
            conditionChanges.Add(new SyncConditionChange(objectType, condition, extBuff));
        }
    }

    private static void ReadChatBlock(cPacket packet, List<SyncChatMessage> chatMessages)
    {
        byte count = packet.PopByte();
        for (int i = 0; i < count; i++)
        {
            uint tamerUid = packet.PopUInt32();
            string message = packet.PopString(consumeTrailingNull: true);
            chatMessages.Add(new SyncChatMessage(pSync.MsgChat, tamerUid, 0, string.Empty, string.Empty, message, 0, 0, false));
        }
    }

    private static void ReadWhisperBlock(cPacket packet, List<SyncChatMessage> chatMessages)
    {
        byte count = packet.PopByte();
        for (int i = 0; i < count; i++)
        {
            byte resultCode = packet.PopByte();
            string source = packet.PopString(consumeTrailingNull: true);
            string target = packet.PopString(consumeTrailingNull: true);
            string message = packet.PopString(consumeTrailingNull: true);
            chatMessages.Add(new SyncChatMessage(pSync.MsgWhisper, 0, resultCode, source, target, message, 0, 0, false));
        }
    }

    private static void ReadSourceMessageBlock(cPacket packet, byte command, List<SyncChatMessage> chatMessages, bool isGuild)
    {
        byte count = packet.PopByte();
        for (int i = 0; i < count; i++)
        {
            string source = packet.PopString(consumeTrailingNull: true);
            string message = packet.PopString(consumeTrailingNull: true);
            chatMessages.Add(new SyncChatMessage(command, 0, 0, source, string.Empty, message, 0, 0, isGuild));
        }
    }

    private static void ReadSystemMessageBlock(cPacket packet, List<SyncChatMessage> chatMessages)
    {
        byte count = packet.PopByte();
        for (int i = 0; i < count; i++)
        {
            string message = packet.PopString(consumeTrailingNull: true);
            chatMessages.Add(new SyncChatMessage(pSync.MsgSystem, 0, 0, string.Empty, string.Empty, message, 0, 0, false));
        }
    }

    private static void ReadItemBroadcastBlock(cPacket packet, List<SyncChatMessage> chatMessages)
    {
        byte count = packet.PopByte();
        for (int i = 0; i < count; i++)
        {
            string source = packet.PopString(consumeTrailingNull: true);
            string message = packet.PopString(consumeTrailingNull: true);
            int itemType = packet.PopInt32();
            byte tamerLevel = packet.PopByte();
            chatMessages.Add(new SyncChatMessage(pSync.MsgAllByItem, 0, 0, source, string.Empty, message, itemType, tamerLevel, false));
        }
    }

    private static void ReadBuffSyncBlock(cPacket packet, List<SyncBuffObject> buffObjects)
    {
        ReadBuffObjectGroup(packet, nClass.Tamer, buffObjects);
        ReadBuffObjectGroup(packet, nClass.Digimon, buffObjects);
        ReadBuffObjectGroup(packet, nClass.Monster, buffObjects);
    }

    private static void ReadBuffObjectGroup(cPacket packet, uint objectClass, List<SyncBuffObject> buffObjects)
    {
        ushort objectCount = packet.PopUInt16();
        for (int i = 0; i < objectCount; i++)
        {
            DmoObjectType objectType = DmoObjectType.FromUid(packet.PopUInt32());
            byte buffCount = packet.PopByte();
            var buffs = new List<SyncBuffEntry>(buffCount);
            for (int b = 0; b < buffCount; b++)
            {
                buffs.Add(new SyncBuffEntry(
                    packet.PopUInt16(),
                    packet.PopUInt16(),
                    packet.PopUInt32(),
                    packet.PopUInt32()));
            }

            buffObjects.Add(new SyncBuffObject((byte)objectClass, objectType, buffs));
        }
    }

    private bool ReadPresenceObjectBlock(
        cPacket packet,
        byte syncType,
        int serverLibType,
        List<SyncPresence> presenceChanges,
        List<SyncWorldObjectSnapshot> worldObjects)
    {
        ushort count = packet.PopUInt16();
        for (int i = 0; i < count; i++)
        {
            SyncPosition position = ReadPosition(packet);
            DmoObjectType objectType = DmoObjectType.FromTypeAll(packet.PopUInt64());
            presenceChanges.Add(new SyncPresence(syncType, objectType, position));

            SyncWorldObjectSnapshot snapshot = ReadWorldObjectSnapshot(packet, syncType, serverLibType, objectType, position);
            worldObjects.Add(snapshot);
            if (snapshot.HasUnparsedTail)
            {
                return true;
            }
        }

        return false;
    }

    private SyncWorldObjectSnapshot ReadWorldObjectSnapshot(
        cPacket packet,
        byte syncType,
        int serverLibType,
        DmoObjectType objectType,
        SyncPosition position)
    {
        bool isNew = syncType == pSync.New;
        return objectType.ClassId switch
        {
            nClass.Tamer => new SyncWorldObjectSnapshot(syncType, isNew, objectType, position, ReadTamerPayload(packet), false),
            nClass.Digimon => new SyncWorldObjectSnapshot(syncType, isNew, objectType, position, ReadDigimonPayload(packet, serverLibType), false),
            nClass.Monster => ReadMonsterSnapshot(packet, syncType, isNew, objectType, position),
            nClass.Item => new SyncWorldObjectSnapshot(syncType, isNew, objectType, position, ReadItemPayload(packet), false),
            nClass.CommissionShop => new SyncWorldObjectSnapshot(syncType, isNew, objectType, position, ReadCommissionShopPayload(packet), false),
            _ => new SyncWorldObjectSnapshot(
                syncType,
                isNew,
                objectType,
                position,
                new SyncUnknownPayload($"Unsupported nClass {objectType.ClassId}"),
                true)
        };
    }

    private static SyncTamerPayload ReadTamerPayload(cPacket packet)
    {
        SyncPosition destination = ReadPosition(packet);
        string name = packet.PopString(consumeTrailingNull: true);
        byte level = packet.PopByte();
        uint directRaw = packet.PopUInt32();
        float direction = BitConverter.Int32BitsToSingle(unchecked((int)directRaw));
        ushort moveSpeed = packet.PopUInt16();
        byte hpRate = packet.PopByte();

        var equipItems = new List<DmoItemData>(TamerEquipCount);
        for (int i = 0; i < TamerEquipCount; i++)
        {
            equipItems.Add(ReadItemData(packet));
        }

        DmoItemData? digiviceItem = HasTamerDigiviceSync ? ReadItemData(packet) : null;
        uint condition = packet.PopUInt32();
        uint syncFlags = packet.PopUInt32();
        uint partnerUid = packet.PopUInt32();
        ushort scale = packet.PopUInt16();

        byte guildClass = packet.PopByte();
        uint guildIndex = 0;
        string guildName = string.Empty;
        if (guildClass != 0)
        {
            guildIndex = packet.PopUInt32();
            guildName = packet.PopString(consumeTrailingNull: true);
        }

        ushort achievement = packet.PopUInt16();
        byte matchTeam = HasMastersMatchingSync ? packet.PopByte() : (byte)0;
        ushort card = packet.PopUInt16();
        string shopTitle = (condition & SyncConditionShop) != 0
            ? packet.PopString(consumeTrailingNull: true)
            : string.Empty;
        uint changeCostumeNo = packet.PopUInt32();

        return new SyncTamerPayload(
            destination,
            name,
            level,
            direction,
            moveSpeed,
            hpRate,
            equipItems,
            digiviceItem,
            condition,
            syncFlags,
            partnerUid,
            scale,
            guildClass,
            guildIndex,
            guildName,
            achievement,
            matchTeam,
            card,
            shopTitle,
            changeCostumeNo);
    }

    private static SyncDigimonPayload ReadDigimonPayload(cPacket packet, int serverLibType)
    {
        SyncPosition destination = ReadPosition(packet);
        string name = packet.PopString(consumeTrailingNull: true);
        ushort scale = packet.PopUInt16();
        byte level = packet.PopByte();
        float direction = packet.PopSingle();
        ushort moveSpeed = packet.PopUInt16();
        ushort attackSpeed = packet.PopUInt16();
        uint tamerUid = packet.PopUInt32();
        byte hpRate = packet.PopByte();
        uint condition = packet.PopUInt32();
        byte? battleTeam = serverLibType == BattleServerLibType ? packet.PopByte() : null;
        ushort enchantLevel = packet.PopUInt16();

        var extensionParameterLevels = new List<ushort>(DigimonExtensionStatCount);
        for (int i = 0; i < DigimonExtensionStatCount; i++)
        {
            extensionParameterLevels.Add(packet.PopUInt16());
        }

        int compatRemainingBytes = packet.PopInt32();
        uint? partsType = HasDigimonPartsSystemSync ? packet.PopUInt32() : null;

        return new SyncDigimonPayload(
            destination,
            name,
            scale,
            level,
            direction,
            moveSpeed,
            attackSpeed,
            tamerUid,
            hpRate,
            condition,
            battleTeam,
            enchantLevel,
            extensionParameterLevels,
            compatRemainingBytes,
            partsType);
    }

    private SyncWorldObjectSnapshot ReadMonsterSnapshot(
        cPacket packet,
        byte syncType,
        bool isNew,
        DmoObjectType objectType,
        SyncPosition position)
    {
        SyncPosition destination = ReadPosition(packet);
        byte hpRate = packet.PopByte();
        byte level = packet.PopByte();
        uint monsterSkillIndex = HasMonsterSkillGrowthSync ? packet.PopUInt32() : 0;
        int monsterStack = HasMonsterSkillGrowthSync ? packet.PopInt32() : 0;
        uint condition = packet.PopUInt32();
        uint? castingSkillIndex = condition == MonsterCastingCondition ? packet.PopUInt32() : null;
        uint seedCount = packet.PopUInt32();

        var seedSkillIndexes = new List<uint>((int)Math.Min(seedCount, 64));
        var seedEffects = new List<SyncMonsterSeedEffect>();
        bool skillTableResolved = _fileTableMng?.MonsterMng?.IsLoaded == true;
        bool hasPotentialSeedPayload = false;
        for (uint i = 0; i < seedCount; i++)
        {
            uint seedSkillIndex = packet.PopUInt32();
            seedSkillIndexes.Add(seedSkillIndex);

            if (!TryGetMonsterSkill(seedSkillIndex, out MonsterSkillRecord? skill) || skill is null)
            {
                hasPotentialSeedPayload = !skillTableResolved || i + 1 < seedCount;
                break;
            }

            if (skill.SkillType != MonsterSkillTypeAttachSeed)
            {
                continue;
            }

            int x = packet.PopInt32();
            int y = packet.PopInt32();
            int effectLifetime = packet.PopInt32();
            uint range = _fileTableMng?.MonsterMng?.GetMonsterSkillTerms(skill.RangeIndex)?.Range ?? 0;
            seedEffects.Add(new SyncMonsterSeedEffect(seedSkillIndex, x, y, effectLifetime, range));
        }

        var payload = new SyncMonsterPayload(
            destination,
            hpRate,
            level,
            monsterSkillIndex,
            monsterStack,
            condition,
            castingSkillIndex,
            seedSkillIndexes,
            seedEffects,
            skillTableResolved,
            hasPotentialSeedPayload);

        return new SyncWorldObjectSnapshot(syncType, isNew, objectType, position, payload, hasPotentialSeedPayload);
    }

    private bool TryGetMonsterSkill(uint skillIndex, out MonsterSkillRecord? skill)
    {
        skill = null;
        CsMonsterMng? monsterMng = _fileTableMng?.MonsterMng;
        if (monsterMng?.IsLoaded != true)
        {
            return false;
        }

        skill = monsterMng.GetMonsterSkill((ushort)skillIndex);
        return skill is not null;
    }

    private static SyncItemPayload ReadItemPayload(cPacket packet)
    {
        return new SyncItemPayload(packet.PopUInt32(), packet.PopByte());
    }

    private static SyncCommissionShopPayload ReadCommissionShopPayload(cPacket packet)
    {
        float direction = packet.PopSingle();
        uint itemType = packet.PopUInt32();
        string shopName = packet.PopString(consumeTrailingNull: true);
        return new SyncCommissionShopPayload(direction, itemType, shopName);
    }

    private static DmoItemData ReadItemData(cPacket packet)
    {
        byte[] rawBytes = packet.PopBytes(CItemDataCompat487Size);
        ReadOnlySpan<byte> raw = rawBytes;
        uint all = BinaryPrimitives.ReadUInt32LittleEndian(raw[..4]);
        uint type = all & 0x1FFFF;
        uint count = (all >> 17) & 0x7FFF;
        uint endTime = BinaryPrimitives.ReadUInt32LittleEndian(raw.Slice(52, 4));
        return new DmoItemData(type, count, endTime, rawBytes);
    }

    private static SyncPosition ReadPosition(cPacket packet)
    {
        return new SyncPosition(packet.PopInt32(), packet.PopInt32());
    }

    private static PortalTransitionInfo ReadPortalTransition(cPacket packet)
    {
        string host = packet.PopString(consumeTrailingNull: true);
        int port = packet.PopInt32();
        uint mapId = packet.PopUInt32();
        int x = packet.PopInt32();
        int y = packet.PopInt32();
        return new PortalTransitionInfo(host, port, mapId, x, y);
    }

    private static LocalPortalInfo ReadLocalPortal(cPacket packet)
    {
        uint tamerUid = packet.PopUInt32();
        uint digimonUid = packet.PopUInt32();
        int x = packet.PopInt32();
        int y = packet.PopInt32();
        return new LocalPortalInfo(tamerUid, digimonUid, x, y);
    }

    private static StartMessageInfo ReadStartMessage(cPacket packet)
    {
        byte count = packet.PopByte();
        var messages = new List<string>(count);
        for (int i = 0; i < count; i++)
        {
            messages.Add(packet.PopString(consumeTrailingNull: true));
        }

        return new StartMessageInfo(messages);
    }

    private static VersionErrorInfo ReadVersionError(cPacket packet)
    {
        uint serverVersion = packet.PopUInt32();
        string message = packet.PopString(consumeTrailingNull: true);
        return new VersionErrorInfo(serverVersion, message);
    }
}
