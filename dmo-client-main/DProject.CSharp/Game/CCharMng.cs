using DProject.Data.CsFileTable;
using DProject.Network;
using DProject.Network.Protocol;
using DProject.Gamebryo;

namespace DProject.Game;

public sealed class CCharMng
{
    private readonly Dictionary<uint, CTamer> _tamers = new();
    private readonly Dictionary<uint, CDigimon> _digimons = new();
    private readonly Dictionary<uint, CMonster> _monsters = new();
    private readonly Dictionary<uint, CNpc> _npcs = new();
    private readonly Dictionary<uint, CFieldItem> _items = new();
    private readonly Dictionary<uint, CEmployment> _employments = new();
    private readonly Dictionary<uint, CsC_AvObject> _objects = new();
    private readonly Dictionary<int, CDigimonUser> _digimonUsers = new();
    private CTamerUser? _tamerUser;

    public IReadOnlyDictionary<uint, CTamer> Tamers => _tamers;
    public IReadOnlyDictionary<uint, CDigimon> Digimons => _digimons;
    public IReadOnlyDictionary<uint, CMonster> Monsters => _monsters;
    public IReadOnlyDictionary<uint, CNpc> Npcs => _npcs;
    public IReadOnlyDictionary<uint, CFieldItem> Items => _items;
    public IReadOnlyDictionary<uint, CEmployment> Employments => _employments;
    public IReadOnlyDictionary<uint, CsC_AvObject> Objects => _objects;
    public IReadOnlyDictionary<int, CDigimonUser> DigimonUsers => _digimonUsers;
    public CTamerUser? TamerUser => _tamerUser;
    public CDigimonUser? DigimonUser => GetDigimonUser(0);

    public CsC_AvObject? GetObject(uint uid)
    {
        return _objects.TryGetValue(uid, out CsC_AvObject? value) ? value : null;
    }

    public CTamer? GetTamer(uint idxOrUid)
    {
        if (_tamers.TryGetValue(idxOrUid, out CTamer? byUid))
        {
            return byUid;
        }

        return _tamers.Values.FirstOrDefault(tamer => tamer.IDX == idxOrUid);
    }

    public CDigimon? GetDigimon(uint idxOrUid)
    {
        if (_digimons.TryGetValue(idxOrUid, out CDigimon? byUid))
        {
            return byUid;
        }

        return _digimons.Values.FirstOrDefault(digimon => digimon.IDX == idxOrUid);
    }

    public CMonster? GetMonster(uint idxOrUid)
    {
        if (_monsters.TryGetValue(idxOrUid, out CMonster? byUid))
        {
            return byUid;
        }

        return _monsters.Values.FirstOrDefault(monster => monster.IDX == idxOrUid);
    }

    public CNpc? GetNpc(uint idxOrUid)
    {
        if (_npcs.TryGetValue(idxOrUid, out CNpc? byUid))
        {
            return byUid;
        }

        return _npcs.Values.FirstOrDefault(npc => npc.IDX == idxOrUid);
    }

    public CTamerUser? GetTamerUser()
    {
        return _tamerUser;
    }

    public CDigimonUser? GetDigimonUser(int userIndex)
    {
        return _digimonUsers.TryGetValue(userIndex, out CDigimonUser? digimonUser)
            ? digimonUser
            : null;
    }

    public CTamerUser SetTamerUser(GameInitData initData, uint serverOption = 0)
    {
        DmoObjectType objectType = DmoObjectType.FromTypeAll(initData.TamerTypeAll);
        return SetTamerUser(
            objectType,
            new SyncPosition(initData.StartX, initData.StartY),
            initData.TamerName,
            serverOption);
    }

    public CTamerUser SetTamerUser(
        DmoObjectType objectType,
        SyncPosition position,
        string name,
        uint serverOption = 0)
    {
        if (_tamerUser is null || _tamerUser.GetUniqID() != objectType.Uid)
        {
            if (_tamerUser is not null)
            {
                _tamers.Remove(_tamerUser.GetUniqID());
                _objects.Remove(_tamerUser.GetUniqID());
            }

            _tamerUser = new CTamerUser();
        }

        _tamerUser.InitLocal(objectType, position, name, serverOption);
        _tamers[objectType.Uid] = _tamerUser;
        _objects[objectType.Uid] = _tamerUser;
        return _tamerUser;
    }

    public CDigimonUser SetDigimonUser(
        int userIndex,
        DmoObjectType objectType,
        uint baseDigimonId,
        SyncPosition position,
        float rotation,
        string name)
    {
        if (!_digimonUsers.TryGetValue(userIndex, out CDigimonUser? digimonUser)
            || digimonUser.GetUniqID() != objectType.Uid)
        {
            if (digimonUser is not null)
            {
                _digimons.Remove(digimonUser.GetUniqID());
                _objects.Remove(digimonUser.GetUniqID());
            }

            digimonUser = new CDigimonUser();
            _digimonUsers[userIndex] = digimonUser;
        }

        digimonUser.InitLocal(objectType, baseDigimonId, position, rotation, name, userIndex);
        _digimons[objectType.Uid] = digimonUser;
        _objects[objectType.Uid] = digimonUser;
        return digimonUser;
    }

    public void ApplySnapshot(SyncWorldObjectSnapshot snapshot)
    {
        switch (snapshot.Payload)
        {
            case SyncTamerPayload tamerPayload:
                AddTamer(snapshot, tamerPayload);
                break;
            case SyncDigimonPayload digimonPayload:
                AddDigimon(snapshot, digimonPayload);
                break;
            case SyncMonsterPayload monsterPayload:
                AddMonster(snapshot, monsterPayload);
                break;
            case SyncItemPayload itemPayload:
                AddItem(snapshot, itemPayload);
                break;
            case SyncCommissionShopPayload commissionShopPayload:
                AddEmployment(snapshot, commissionShopPayload);
                break;
        }
    }

    public CTamer AddTamer(SyncWorldObjectSnapshot snapshot, SyncTamerPayload payload)
    {
        CTamer tamer = GetOrCreate(_tamers, snapshot.ObjectType.Uid, static () => new CTamer());
        tamer.ApplySnapshot(snapshot, payload);
        _objects[snapshot.ObjectType.Uid] = tamer;
        return tamer;
    }

    public CDigimon AddDigimon(SyncWorldObjectSnapshot snapshot, SyncDigimonPayload payload)
    {
        CDigimon digimon = GetOrCreate(_digimons, snapshot.ObjectType.Uid, static () => new CDigimon());
        digimon.ApplySnapshot(snapshot, payload);
        _objects[snapshot.ObjectType.Uid] = digimon;
        return digimon;
    }

    public CMonster AddMonster(SyncWorldObjectSnapshot snapshot, SyncMonsterPayload payload)
    {
        CMonster monster = GetOrCreate(_monsters, snapshot.ObjectType.Uid, static () => new CMonster());
        monster.ApplySnapshot(snapshot, payload);
        _objects[snapshot.ObjectType.Uid] = monster;
        return monster;
    }

    public CNpc AddNpc(NpcBinRecord npcRecord, MapNpcRecord mapNpcRecord)
    {
        uint uid = MakeUid(nClass.Npc, mapNpcRecord.NpcId);
        CNpc npc = GetOrCreate(_npcs, uid, static () => new CNpc());
        npc.Init(npcRecord, mapNpcRecord, uid);
        _objects[uid] = npc;
        return npc;
    }

    public CNpc AddNpc(
        uint idx,
        uint fileTableId,
        SyncPosition position,
        float rotation,
        string name = "")
    {
        uint uid = MakeUid(nClass.Npc, idx);
        CNpc npc = GetOrCreate(_npcs, uid, static () => new CNpc());
        npc.InitManual(uid, fileTableId, position, rotation, name);
        _objects[uid] = npc;
        return npc;
    }

    public int LoadNpcsForMap(CsFileTableMng fileTables, uint mapId, bool clearExisting = true)
    {
        if (fileTables.MapMng is null || fileTables.NpcMng is null)
        {
            return 0;
        }

        return LoadNpcsForMap(fileTables.MapMng, fileTables.NpcMng, mapId, clearExisting);
    }

    public int LoadNpcsForMap(CsMapMng mapMng, CsNpcMng npcMng, uint mapId, bool clearExisting = true)
    {
        if (clearExisting)
        {
            ClearNpcs();
        }

        int loaded = 0;
        foreach (MapNpcRecord mapNpc in mapMng.GetNpcs(mapId))
        {
            NpcBinRecord? npc = npcMng.GetNpc(mapNpc.NpcId);
            if (npc is null)
            {
                continue;
            }

            AddNpc(npc, mapNpc);
            loaded++;
        }

        return loaded;
    }

    public void ClearNpcs()
    {
        foreach (CNpc npc in _npcs.Values)
        {
            npc.Delete();
            _objects.Remove(npc.GetUniqID());
        }

        _npcs.Clear();
    }

    public CFieldItem AddItem(SyncWorldObjectSnapshot snapshot, SyncItemPayload payload)
    {
        CFieldItem item = GetOrCreate(_items, snapshot.ObjectType.Uid, static () => new CFieldItem());
        item.ApplySnapshot(snapshot, payload);
        _objects[snapshot.ObjectType.Uid] = item;
        return item;
    }

    public CEmployment AddEmployment(SyncWorldObjectSnapshot snapshot, SyncCommissionShopPayload payload)
    {
        CEmployment employment = GetOrCreate(_employments, snapshot.ObjectType.Uid, static () => new CEmployment());
        employment.ApplySnapshot(snapshot, payload);
        _objects[snapshot.ObjectType.Uid] = employment;
        return employment;
    }

    public void MoveObject(uint uid, int x, int y)
    {
        GetObject(uid)?.SetPath(x, y);
    }

    public void RotateObject(uint uid, float rotation)
    {
        GetObject(uid)?.SetRotation(rotation);
    }

    public void SetCondition(SyncConditionChange change)
    {
        CsC_AvObject? obj = GetObject(change.ObjectType.Uid);
        if (obj is null)
        {
            obj = CreatePlaceholder(change.ObjectType);
        }

        obj.SetCondition(change.Condition, change.ExtBuff);
    }

    public void ApplyBuffs(SyncBuffObject buffObject)
    {
        CsC_AvObject? obj = GetObject(buffObject.ObjectType.Uid);
        if (obj is null)
        {
            obj = CreatePlaceholder(buffObject.ObjectType);
        }

        obj.GetBuff().ReplaceAll(buffObject.Buffs);
    }

    public void RemoveObject(uint uid)
    {
        if (_objects.TryGetValue(uid, out CsC_AvObject? obj))
        {
            obj.Delete();
        }
    }

    private CsC_AvObject CreatePlaceholder(DmoObjectType objectType)
    {
        CsC_AvObject obj = objectType.ClassId switch
        {
            nClass.Tamer => GetOrCreate(_tamers, objectType.Uid, static () => new CTamer()),
            nClass.Digimon => GetOrCreate(_digimons, objectType.Uid, static () => new CDigimon()),
            nClass.Monster => GetOrCreate(_monsters, objectType.Uid, static () => new CMonster()),
            nClass.Npc => GetOrCreate(_npcs, objectType.Uid, static () => new CNpc()),
            nClass.Item => GetOrCreate(_items, objectType.Uid, static () => new CFieldItem()),
            nClass.CommissionShop => GetOrCreate(_employments, objectType.Uid, static () => new CEmployment()),
            _ => new CsC_AvObject()
        };

        obj.Init(objectType.Uid, objectType.Index, objectType.ClassId, objectType.TypeId, NiPoint3.Zero, 0.0f);
        _objects[objectType.Uid] = obj;
        return obj;
    }

    private static uint MakeUid(uint classId, uint idx)
    {
        return ((classId & 0x1F) << 14) | (idx & 0x3FFF);
    }

    private static TObject GetOrCreate<TObject>(
        Dictionary<uint, TObject> objects,
        uint uid,
        Func<TObject> factory)
        where TObject : CsC_AvObject
    {
        if (!objects.TryGetValue(uid, out TObject? value))
        {
            value = factory();
            objects[uid] = value;
        }

        return value;
    }
}
