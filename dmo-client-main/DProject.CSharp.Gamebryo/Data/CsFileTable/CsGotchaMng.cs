namespace DProject.Data.CsFileTable;

public sealed class CsGotchaMng : IBinFileTableManager
{
    private readonly GotchaBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<int, GotchaMachineRecord> _machines = new();
    private readonly Dictionary<ushort, GotchaItemRecord> _items = new();
    private readonly Dictionary<int, GotchaRareItemRecord> _rareItems = new();
    private readonly Dictionary<uint, GotchaMysteryItemRecord> _mysteryItems = new();
    private readonly Dictionary<ushort, GotchaMysteryCoinRecord> _mysteryCoins = new();

    public CsGotchaMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsGotchaMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<int, GotchaMachineRecord> Machines => _machines;
    public IReadOnlyDictionary<ushort, GotchaItemRecord> Items => _items;
    public IReadOnlyDictionary<int, GotchaRareItemRecord> RareItems => _rareItems;
    public IReadOnlyDictionary<uint, GotchaMysteryItemRecord> MysteryItems => _mysteryItems;
    public IReadOnlyDictionary<ushort, GotchaMysteryCoinRecord> MysteryCoins => _mysteryCoins;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is GotchaBinData data)
        {
            Copy(data.Machines, _machines);
            Copy(data.Items, _items);
            Copy(data.RareItems, _rareItems);
            Copy(data.MysteryItems, _mysteryItems);
            Copy(data.MysteryCoins, _mysteryCoins);
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _machines.Clear();
        _items.Clear();
        _rareItems.Clear();
        _mysteryItems.Clear();
        _mysteryCoins.Clear();
        IsLoaded = false;
    }

    public bool IsGotcha_NPC(int id)
    {
        return _machines.Values.Any(machine => machine.NpcId == (uint)id);
    }

    public bool IsGotcha_Item(int groupId)
    {
        return _items.ContainsKey((ushort)groupId);
    }

    public GotchaMachineRecord? GetGotcha_Machine(int npcId, int index)
    {
        return _machines.TryGetValue(index, out GotchaMachineRecord? machine) &&
            machine.NpcId == (uint)npcId
                ? machine
                : null;
    }

    public ushort GetGotcha_Item(int itemId)
    {
        foreach (GotchaItemRecord item in _items.Values)
        {
            for (int i = 0; i < item.ItemCodes.Count && i < item.ItemCounts.Count; i++)
            {
                if (item.ItemCodes[i] == (uint)itemId)
                {
                    return item.ItemCounts[i];
                }
            }
        }

        return 0;
    }

    public GotchaRareItemRecord? GetGotcha_RareItem(int index)
    {
        return _rareItems.TryGetValue(index, out GotchaRareItemRecord? record) ? record : null;
    }

    public int GetRareItemIndex(int npcId, int itemId)
    {
        foreach (KeyValuePair<int, GotchaRareItemRecord> item in _rareItems)
        {
            if (item.Value.NpcId == (uint)npcId &&
                item.Value.RareItem == (uint)itemId)
            {
                return item.Key;
            }
        }

        return -1;
    }

    public uint GetGotchaMachineFIDX(uint npcId)
    {
        foreach (KeyValuePair<int, GotchaMachineRecord> item in _machines)
        {
            if (item.Value.NpcId == npcId)
            {
                return (uint)item.Key;
            }
        }

        return uint.MaxValue;
    }

    public bool IsEnableGotchaMachine(uint fidx)
    {
        return fidx <= int.MaxValue &&
            _machines.TryGetValue((int)fidx, out GotchaMachineRecord? machine) &&
            machine.Limit;
    }

    public bool IsEnableLevel(uint fidx, int currentLevel)
    {
        return fidx <= int.MaxValue &&
            _machines.TryGetValue((int)fidx, out GotchaMachineRecord? machine) &&
            currentLevel >= machine.MinLevel &&
            currentLevel <= machine.MaxLevel;
    }

    public bool GetGotcha_Mystery_IsEffect(int item)
    {
        return _mysteryItems.TryGetValue((uint)item, out GotchaMysteryItemRecord? record) &&
            record.Effect == 1;
    }

    public GotchaMysteryCoinRecord? GetGotcha_Mystery_Coin(int order)
    {
        return _mysteryCoins.TryGetValue((ushort)order, out GotchaMysteryCoinRecord? record) ? record : null;
    }

    public uint Get_Req_Money(int machineNo)
    {
        int groupSection = (machineNo * 100) - 200;
        foreach (GotchaMysteryCoinRecord coin in _mysteryCoins.Values)
        {
            if (coin.StartGroupNo == groupSection)
            {
                return coin.Money;
            }
        }

        return 0;
    }

    private static void Copy<TKey, TValue>(
        IReadOnlyDictionary<TKey, TValue> source,
        Dictionary<TKey, TValue> destination)
        where TKey : notnull
    {
        foreach (KeyValuePair<TKey, TValue> item in source)
        {
            destination[item.Key] = item.Value;
        }
    }
}
