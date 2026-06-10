using DProject.Data.CsFileTable;
using DProject.Data;
using DProject.GlobalData;
using DProject.Network;

namespace DProject.Runtime;

public sealed class GameStateSync
{
    private const int MaxSyncChatMessages = 100;

    private readonly GlobalDataStore _globalData;
    private readonly MainThreadDispatcher _mainThread;

    public GameStateSync(GlobalDataStore globalData, MainThreadDispatcher mainThread)
    {
        _globalData = globalData;
        _mainThread = mainThread;
    }

    public void RequestMapTransition(uint mapId, string mapName, string reason)
    {
        ApplyOnMainThread(() =>
        {
            _globalData.IsMapTransitioning = true;
            _globalData.PendingMapId = mapId;
            _globalData.PendingMapName = mapName;
            _globalData.MapTransitionReason = reason;
        });
    }

    public void CompleteMapTransition(uint mapId, string mapName)
    {
        ApplyOnMainThread(() =>
        {
            _globalData.CurrentMapId = mapId;
            _globalData.CurrentMapName = mapName;
            _globalData.PendingMapId = 0;
            _globalData.PendingMapName = string.Empty;
            _globalData.MapTransitionReason = string.Empty;
            _globalData.IsMapTransitioning = false;
        });
    }

    public void LoadMapNpcs(CsFileTableMng fileTables, uint mapId)
    {
        if (mapId == 0 || !fileTables.IsInitialized)
        {
            return;
        }

        ApplyOnMainThread(() =>
        {
            _globalData.LastMapNpcLoadMapId = mapId;
            _globalData.LastMapNpcLoadCount = _globalData.CharMng.LoadNpcsForMap(fileTables, mapId);
        });
    }

    public void EnterCombat(string source)
    {
        ApplyOnMainThread(() =>
        {
            _globalData.CombatState = GameCombatState.Active;
            _globalData.IsBattleActive = true;
            _globalData.CombatStateSource = source;
        });
    }

    public void LeaveCombat(string source)
    {
        ApplyOnMainThread(() =>
        {
            _globalData.CombatState = GameCombatState.Peace;
            _globalData.IsBattleActive = false;
            _globalData.CombatStateSource = source;
        });
    }

    public void SendServerSync(eSERVER_SYNC sync, int refConstant, params object?[] values)
    {
        ApplyOnMainThread(() =>
        {
            _globalData.ServerSync.SendServer(sync, refConstant, networkEnabled: true, values);
            RefreshServerSyncState();
        });
    }

    public void RecvServerSync(eSERVER_SYNC sync, int refConstant, params object?[] values)
    {
        ApplyOnMainThread(() =>
        {
            _globalData.ServerSync.RecvServer(sync, refConstant, values);
            RefreshServerSyncState();
        });
    }

    public void UpdateServerSync()
    {
        ApplyOnMainThread(() =>
        {
            _globalData.ServerSync.Update();
            RefreshServerSyncState();
        });
    }

    private void RefreshServerSyncState()
    {
        _globalData.IsChangingDigimon = _globalData.ServerSync.IsChageDigimon();
        _globalData.IsUsingDigimonSkill = _globalData.ServerSync.IsSyncActive(eSERVER_SYNC.USE_DIGIMON_SKILL);
        _globalData.IsRideSyncActive =
            _globalData.ServerSync.IsSyncActive(eSERVER_SYNC.RIDE_ON) ||
            _globalData.ServerSync.IsSyncActive(eSERVER_SYNC.RIDE_OFF);
        _globalData.LastServerSyncSend = _globalData.ServerSync.LastSendSync?.ToString() ?? string.Empty;
        _globalData.LastServerSyncRecv = _globalData.ServerSync.LastRecvSync?.ToString() ?? string.Empty;
    }

    public void ApplyInitGameData(GameInitData initData, string source = "pGame.InitGameData", uint serverOption = 0)
    {
        ApplyOnMainThread(() =>
        {
            string tamerName = string.IsNullOrWhiteSpace(initData.TamerName)
                ? _globalData.SelectedCharacterName
                : initData.TamerName;
            DmoObjectType objectType = DmoObjectType.FromTypeAll(initData.TamerTypeAll);

            _globalData.ServerLibType = initData.ServerLibType;
            _globalData.StartPositionX = initData.StartX;
            _globalData.StartPositionY = initData.StartY;
            _globalData.SelectedCharacterName = tamerName;
            _globalData.CharMng.SetTamerUser(
                objectType,
                new SyncPosition(initData.StartX, initData.StartY),
                tamerName,
                serverOption);

            _globalData.WorldObjects[objectType.Uid] = new WorldObjectState(
                objectType.Uid,
                objectType.Index,
                objectType.ClassId,
                objectType.TypeId,
                initData.StartX,
                initData.StartY,
                0.0f,
                true,
                source,
                tamerName,
                PayloadKind: "TamerUser");
        });
    }

    public void UpsertWorldObject(
        uint uid,
        uint index,
        uint classId,
        uint typeId,
        int x,
        int y,
        string source)
    {
        ApplyOnMainThread(() =>
        {
            if (!_globalData.WorldObjects.TryGetValue(uid, out WorldObjectState? current))
            {
                current = new WorldObjectState(uid, index, classId, typeId, x, y, 0.0f, true, source);
            }

            _globalData.WorldObjects[uid] = current with
            {
                Index = index,
                ClassId = classId,
                TypeId = typeId == 0 ? current.TypeId : typeId,
                X = x,
                Y = y,
                IsPresent = true,
                LastSyncKind = source
            };
        });
    }

    public void UpsertWorldObject(SyncWorldObjectSnapshot snapshot, string source)
    {
        ApplyOnMainThread(() =>
        {
            uint uid = snapshot.ObjectType.Uid;
            if (!_globalData.WorldObjects.TryGetValue(uid, out WorldObjectState? current))
            {
                current = new WorldObjectState(
                    uid,
                    snapshot.ObjectType.Index,
                    snapshot.ObjectType.ClassId,
                    snapshot.ObjectType.TypeId,
                    snapshot.Position.X,
                    snapshot.Position.Y,
                    0.0f,
                    true,
                    source);
            }

            (string name, byte level, byte hpRate, uint condition, float rotation, SyncPosition position) =
                ExtractSnapshotState(snapshot, current.Rotation);
            _globalData.CharMng.ApplySnapshot(snapshot);

            _globalData.WorldObjects[uid] = current with
            {
                Index = snapshot.ObjectType.Index,
                ClassId = snapshot.ObjectType.ClassId,
                TypeId = snapshot.ObjectType.TypeId == 0 ? current.TypeId : snapshot.ObjectType.TypeId,
                X = position.X,
                Y = position.Y,
                Rotation = rotation,
                IsPresent = true,
                LastSyncKind = source,
                Name = name,
                Level = level,
                HpRate = hpRate,
                Condition = condition,
                PayloadKind = SnapshotPayloadKind(snapshot.Payload)
            };
        });
    }

    public void MoveWorldObject(uint uid, uint index, uint classId, int x, int y, string source)
    {
        ApplyOnMainThread(() =>
        {
            if (!_globalData.WorldObjects.TryGetValue(uid, out WorldObjectState? current))
            {
                current = new WorldObjectState(uid, index, classId, 0, x, y, 0.0f, true, source);
            }

            _globalData.WorldObjects[uid] = current with
            {
                Index = index,
                ClassId = classId,
                X = x,
                Y = y,
                IsPresent = true,
                LastSyncKind = source
            };
            _globalData.CharMng.MoveObject(uid, x, y);
        });
    }

    public void RotateWorldObject(uint uid, uint index, uint classId, float rotation, string source)
    {
        ApplyOnMainThread(() =>
        {
            if (!_globalData.WorldObjects.TryGetValue(uid, out WorldObjectState? current))
            {
                current = new WorldObjectState(uid, index, classId, 0, 0, 0, rotation, true, source);
            }

            _globalData.WorldObjects[uid] = current with
            {
                Index = index,
                ClassId = classId,
                Rotation = rotation,
                IsPresent = true,
                LastSyncKind = source
            };
            _globalData.CharMng.RotateObject(uid, rotation);
        });
    }

    public void RemoveWorldObject(uint uid, string source)
    {
        ApplyOnMainThread(() =>
        {
            if (_globalData.WorldObjects.TryGetValue(uid, out WorldObjectState? current))
            {
                _globalData.WorldObjects[uid] = current with
                {
                    IsPresent = false,
                    LastSyncKind = source
                };
                _globalData.CharMng.RemoveObject(uid);
                return;
            }

            _globalData.WorldObjects[uid] = new WorldObjectState(uid, 0, 0, 0, 0, 0, 0.0f, false, source);
            _globalData.CharMng.RemoveObject(uid);
        });
    }

    public void ApplyConditionChanges(IReadOnlyList<SyncConditionChange> changes, string source)
    {
        if (changes.Count == 0)
        {
            return;
        }

        ApplyOnMainThread(() =>
        {
            foreach (SyncConditionChange change in changes)
            {
                uint uid = change.ObjectType.Uid;
                if (!_globalData.WorldObjects.TryGetValue(uid, out WorldObjectState? current))
                {
                    current = new WorldObjectState(
                        uid,
                        change.ObjectType.Index,
                        change.ObjectType.ClassId,
                        change.ObjectType.TypeId,
                        0,
                        0,
                        0.0f,
                        true,
                        source);
                }

                _globalData.WorldObjects[uid] = current with
                {
                    Index = change.ObjectType.Index,
                    ClassId = change.ObjectType.ClassId,
                    TypeId = change.ObjectType.TypeId == 0 ? current.TypeId : change.ObjectType.TypeId,
                    Condition = change.Condition,
                    ExtBuff = change.ExtBuff,
                    LastSyncKind = source
                };
                _globalData.CharMng.SetCondition(change);
            }

            _globalData.LastSyncConditionCount = changes.Count;
        });
    }

    public void ApplyBuffSync(IReadOnlyList<SyncBuffObject> buffObjects)
    {
        if (buffObjects.Count == 0)
        {
            return;
        }

        ApplyOnMainThread(() =>
        {
            foreach (SyncBuffObject buffObject in buffObjects)
            {
                _globalData.WorldObjectBuffs[buffObject.ObjectType.Uid] = buffObject.Buffs
                    .Select(static buff => new WorldObjectBuffState(
                        buff.BuffCode,
                        buff.Level,
                        buff.EndTimestamp,
                        buff.SkillCode))
                    .ToArray();
                _globalData.CharMng.ApplyBuffs(buffObject);
            }

            _globalData.LastSyncBuffObjectCount = buffObjects.Count;
        });
    }

    public void AppendSyncChatMessages(IReadOnlyList<SyncChatMessage> messages)
    {
        if (messages.Count == 0)
        {
            return;
        }

        ApplyOnMainThread(() =>
        {
            foreach (SyncChatMessage message in messages)
            {
                _globalData.SyncChatMessages.Add(new WorldSyncChatMessage(
                    message.Command,
                    message.TamerUid,
                    message.ResultCode,
                    message.Source,
                    message.Target,
                    message.Message,
                    message.ItemType,
                    message.TamerLevel,
                    message.IsGuild));
            }

            int overflow = _globalData.SyncChatMessages.Count - MaxSyncChatMessages;
            if (overflow > 0)
            {
                _globalData.SyncChatMessages.RemoveRange(0, overflow);
            }

            _globalData.LastSyncChatMessageCount = messages.Count;
        });
    }

    public void RecordMovementSync(int objectCount, int remainingBytes)
    {
        ApplyOnMainThread(() =>
        {
            _globalData.LastMovementSyncSequence++;
            _globalData.LastMovementSyncObjectCount = objectCount;
            _globalData.LastMovementSyncRemainingBytes = remainingBytes;
        });
    }

    private void ApplyOnMainThread(Action action)
    {
        if (_mainThread.IsMainThread)
        {
            action();
            return;
        }

        _mainThread.Post(action);
    }

    private static (string Name, byte Level, byte HpRate, uint Condition, float Rotation, SyncPosition Position)
        ExtractSnapshotState(SyncWorldObjectSnapshot snapshot, float currentRotation)
    {
        return snapshot.Payload switch
        {
            SyncTamerPayload tamer => (
                tamer.Name,
                tamer.Level,
                tamer.HpRate,
                tamer.Condition,
                tamer.Direction,
                snapshot.Position),
            SyncDigimonPayload digimon => (
                digimon.Name,
                digimon.Level,
                digimon.HpRate,
                digimon.Condition,
                digimon.Direction,
                snapshot.Position),
            SyncMonsterPayload monster => (
                string.Empty,
                monster.Level,
                monster.HpRate,
                monster.Condition,
                currentRotation,
                snapshot.Position),
            SyncCommissionShopPayload commissionShop => (
                commissionShop.ShopName,
                0,
                0,
                0,
                commissionShop.Direction,
                snapshot.Position),
            _ => (
                string.Empty,
                0,
                0,
                0,
                currentRotation,
                snapshot.Position)
        };
    }

    private static string SnapshotPayloadKind(SyncObjectPayload payload)
    {
        return payload switch
        {
            SyncTamerPayload => "Tamer",
            SyncDigimonPayload => "Digimon",
            SyncMonsterPayload => "Monster",
            SyncItemPayload => "Item",
            SyncCommissionShopPayload => "CommissionShop",
            SyncUnknownPayload => "Unknown",
            _ => payload.GetType().Name
        };
    }
}
