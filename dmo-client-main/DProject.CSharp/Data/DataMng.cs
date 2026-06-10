using DProject.Gamebryo;

namespace DProject.Data;

public enum ServerDataSlotGroup
{
    Inven = cDataMng.SERVER_DATA_INVEN_CONSTANT,
    TEquip = cDataMng.SERVER_DATA_TEQUIP_CONSTANT,
    Chipset = cDataMng.SERVER_DATA_CHIPSET_CONSTANT,
    Warehouse = cDataMng.SERVER_DATA_WAREHOUSE_CONSTANT,
    Digivice = cDataMng.SERVER_DATA_DIGIVICE_CONSTANT,
    TSkill = cDataMng.SERVER_DATA_TSKILL_CONSTANT,
    Memory = cDataMng.SERVER_DATA_MEMORY_CONSTANT,
    ExtraSeal = cDataMng.SERVER_DATA_EXTRA_SEAL_CONSTANT,
    ExtraTicket = cDataMng.SERVER_DATA_EXTRA_TICKET_CONSTANT,
    ExtraEvolution = cDataMng.SERVER_DATA_EXTRA_EVOLUTION_CONSTANT,
    ExtraDigitama = cDataMng.SERVER_DATA_EXTRA_DIGITAMA_CONSTANT,
    ExtraMaterial = cDataMng.SERVER_DATA_EXTRA_MATERIAL_CONSTANT
}

public abstract class cDataComponent
{
    public bool IsInitialized { get; private set; }

    public virtual void Init()
    {
        IsInitialized = true;
    }

    public virtual void Delete()
    {
        IsInitialized = false;
    }

    public virtual void Reset()
    {
    }

    public virtual void InitNetOff()
    {
    }

    public virtual void Update(float elapsedSeconds)
    {
    }
}

public sealed class cData_Inven : cDataComponent
{
    public int SlotCount { get; private set; } = 100;

    public void SetSlotCount(int slotCount)
    {
        SlotCount = Math.Max(0, slotCount);
    }
}

public sealed class cData_PostLoad : cDataComponent { }
public sealed class cData_TEquip : cDataComponent { }
public sealed class cData_Digivice : cDataComponent { }
public sealed class cData_QuickSlot : cDataComponent { }
public sealed class cData_Quest : cDataComponent { }
public sealed class cData_QuestOwner : cDataComponent { }
public sealed class cData_Tactics : cDataComponent { }
public sealed class cData_MapRegion : cDataComponent { }
public sealed class cData_Cash : cDataComponent { }
public sealed class cData_Attendance : cDataComponent { }
public sealed class cData_PCBang : cDataComponent { }
public sealed class cData_MapObject : cDataComponent { }

public sealed class cDataMng
{
    public const int SERVER_DATA_INVEN_CONSTANT = 0;
    public const int SERVER_DATA_TEQUIP_CONSTANT = 1000;
    public const int SERVER_DATA_CHIPSET_CONSTANT = 2000;
    public const int SERVER_DATA_WAREHOUSE_CONSTANT = 3000;
    public const int SERVER_DATA_DIGIVICE_CONSTANT = 5000;
    public const int SERVER_DATA_TSKILL_CONSTANT = 6000;
    public const int SERVER_DATA_MEMORY_CONSTANT = 10000;
    public const int SERVER_DATA_EXTRAINVEN_BASE_CONSTANT = 12000;
    public const int SERVER_DATA_EXTRA_SEAL_CONSTANT = 12000;
    public const int SERVER_DATA_EXTRA_TICKET_CONSTANT = 13000;
    public const int SERVER_DATA_EXTRA_EVOLUTION_CONSTANT = 14000;
    public const int SERVER_DATA_EXTRA_DIGITAMA_CONSTANT = 15000;
    public const int SERVER_DATA_EXTRA_MATERIAL_CONSTANT = 16000;

    private readonly HashSet<int> _itemLockIndexes = [];
    private int _itemTryCount;
    private int _pendingExtraSealUseDestSrvId = -1;
    private int _pendingExtraSealUseRetry;

    public cData_Inven Inven { get; } = new();
    public cData_Inven[] ExtraInven { get; } =
    [
        new cData_Inven(),
        new cData_Inven(),
        new cData_Inven(),
        new cData_Inven(),
        new cData_Inven()
    ];
    public cData_PostLoad PostLoad { get; } = new();
    public cData_TEquip TEquip { get; } = new();
    public cData_Digivice Digivice { get; } = new();
    public cData_QuickSlot[] QuickSlots { get; } =
    [
        new cData_QuickSlot(),
        new cData_QuickSlot(),
        new cData_QuickSlot(),
        new cData_QuickSlot(),
        new cData_QuickSlot(),
        new cData_QuickSlot()
    ];
    public cData_Quest Quest { get; } = new();
    public cData_QuestOwner QuestOwner { get; } = new();
    public cData_Tactics Tactics { get; } = new();
    public cData_MapRegion MapRegion { get; } = new();
    public cData_ServerSync ServerSync { get; } = new();
    public cData_Cash Cash { get; } = new();
    public cData_Attendance Attendance { get; } = new();
    public cData_PCBang PCBang { get; } = new();
    public cData_MapObject MapObject { get; } = new();

    public bool IsInitialized { get; private set; }
    public bool IsNetOff { get; private set; }
    public int ItemTryCount => _itemTryCount;
    public int PendingExtraSealUseDestSrvId => _pendingExtraSealUseDestSrvId;
    public int PendingExtraSealUseRetry => _pendingExtraSealUseRetry;
    public IReadOnlyCollection<int> ItemLockIndexes => _itemLockIndexes;

    public void Init()
    {
        foreach (cDataComponent component in Components())
        {
            component.Init();
        }

        ServerSync.Init();
        ReleaseItemLock();
        ResetItemTryCount();
        _pendingExtraSealUseDestSrvId = -1;
        _pendingExtraSealUseRetry = 0;
        IsInitialized = true;
    }

    public void Delete()
    {
        foreach (cDataComponent component in Components())
        {
            component.Delete();
        }

        ServerSync.Delete();
        IsInitialized = false;
    }

    public void Reset()
    {
        foreach (cDataComponent component in Components())
        {
            component.Reset();
        }

        ServerSync.Reset();
        ReleaseItemLock();
        ResetItemTryCount();
        _pendingExtraSealUseDestSrvId = -1;
        _pendingExtraSealUseRetry = 0;

        if (IsNetOff)
        {
            InitNetOff();
        }
    }

    public void LogOut()
    {
        MapRegion.Reset();
        IsNetOff = true;
        InitNetOff();
    }

    public void Update(float elapsedSeconds = 1.0f / 60.0f)
    {
        QuestOwner.Update(elapsedSeconds);
        ServerSync.Update();
    }

    public void DiedAlready()
    {
        ReleaseItemLock();
    }

    public void InitNetOff()
    {
        foreach (cDataComponent component in Components())
        {
            component.InitNetOff();
        }
    }

    public NiColor GetItemColor(int itemClass)
    {
        return itemClass switch
        {
            1 => new NiColor(167 / 255.0f, 167 / 255.0f, 167 / 255.0f),
            2 => NiColor.White,
            3 => new NiColor(34 / 255.0f, 255 / 255.0f, 73 / 255.0f),
            4 => new NiColor(28 / 255.0f, 229 / 255.0f, 255 / 255.0f),
            5 => new NiColor(255 / 255.0f, 41 / 255.0f, 178 / 255.0f),
            6 => new NiColor(225 / 255.0f, 118 / 255.0f, 72 / 255.0f),
            7 => new NiColor(255 / 255.0f, 212 / 255.0f, 72 / 255.0f),
            8 => new NiColor(255 / 255.0f, 102 / 255.0f, 255 / 255.0f),
            9 => new NiColor(255 / 255.0f, 0.0f, 87 / 255.0f),
            10 => new NiColor(255 / 255.0f, 240 / 255.0f, 0.0f),
            13 => new NiColor(255 / 255.0f, 240 / 255.0f, 0.0f),
            _ => NiColor.White
        };
    }

    public void ItemAllLock()
    {
        for (int slot = 0; slot < Inven.SlotCount; slot++)
        {
            ItemLock(ToInvenSid(slot));
        }
    }

    public void ItemLock(int lockIndex)
    {
        _itemLockIndexes.Add(lockIndex);
    }

    public void ItemUnlock(int lockIndex)
    {
        _itemLockIndexes.Remove(lockIndex);
    }

    public bool IsItemLock(int lockIndex) => _itemLockIndexes.Contains(lockIndex);
    public bool IsItemAllLock() => _itemLockIndexes.Count != 0;
    public void ReleaseItemLock() => _itemLockIndexes.Clear();
    public void ResetItemTryCount() => _itemTryCount = 0;
    public void ItemTryCountOnce() => _itemTryCount = 1;

    public cData_Inven? GetExtraInven(int category)
    {
        return category >= 0 && category < ExtraInven.Length ? ExtraInven[category] : null;
    }

    public cData_QuickSlot? GetQuickSlot(int index)
    {
        return index >= 0 && index < QuickSlots.Length ? QuickSlots[index] : null;
    }

    public static int ToInvenSid(int id) => id;
    public static int ToTEquipSid(int id) => id + SERVER_DATA_TEQUIP_CONSTANT;
    public static int ToMemorySid(int id) => id + SERVER_DATA_MEMORY_CONSTANT;
    public static int ToTSkillSid(int id) => id + SERVER_DATA_TSKILL_CONSTANT;
    public static int ToChipsetSid(int id) => id + SERVER_DATA_CHIPSET_CONSTANT;
    public static int ToWarehouseSid(int id) => id + SERVER_DATA_WAREHOUSE_CONSTANT;
    public static int ToDigiviceSid(int id) => id + SERVER_DATA_DIGIVICE_CONSTANT;
    public static int ToExtraInvenSid(int category, int id) => id + SERVER_DATA_EXTRAINVEN_BASE_CONSTANT + (category * 1000);
    public static int ToId(int sid) => sid % 1000;
    public static int ToConstant(int sid) => sid / 1000 * 1000;
    public static bool IsExtraInvenConstant(int constant) =>
        constant >= SERVER_DATA_EXTRA_SEAL_CONSTANT && constant <= SERVER_DATA_EXTRA_MATERIAL_CONSTANT;
    public static int ToExtraInvenCategory(int constant) => (constant - SERVER_DATA_EXTRAINVEN_BASE_CONSTANT) / 1000;

    private IEnumerable<cDataComponent> Components()
    {
        yield return Inven;
        foreach (cData_Inven extraInven in ExtraInven)
        {
            yield return extraInven;
        }

        yield return PostLoad;
        yield return TEquip;
        yield return Digivice;
        foreach (cData_QuickSlot quickSlot in QuickSlots)
        {
            yield return quickSlot;
        }

        yield return Quest;
        yield return QuestOwner;
        yield return Tactics;
        yield return MapRegion;
        yield return Cash;
        yield return Attendance;
        yield return PCBang;
        yield return MapObject;
    }
}
