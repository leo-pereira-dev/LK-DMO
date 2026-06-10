using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record GotchaBinData(
    IReadOnlyDictionary<int, GotchaMachineRecord> Machines,
    IReadOnlyDictionary<ushort, GotchaItemRecord> Items,
    IReadOnlyDictionary<int, GotchaRareItemRecord> RareItems,
    IReadOnlyDictionary<uint, GotchaMysteryItemRecord> MysteryItems,
    IReadOnlyDictionary<ushort, GotchaMysteryCoinRecord> MysteryCoins);

public sealed record GotchaMachineRecord(
    int Index,
    uint NpcId,
    uint UseItemCode,
    ushort UseItemCount,
    bool Limit,
    uint StartDate,
    uint EndDate,
    uint StartTime,
    uint EndTime,
    ushort MinLevel,
    ushort MaxLevel,
    int ShowCountUi);

public sealed record GotchaItemRecord(
    ushort GroupId,
    IReadOnlyList<uint> ItemCodes,
    IReadOnlyList<ushort> ItemCounts);

public sealed record GotchaRareItemRecord(
    int Index,
    uint NpcId,
    string RareItemName,
    uint RareItem,
    uint RareItemCount,
    uint RareItemGive);

public sealed record GotchaMysteryItemRecord(
    ushort Group,
    uint Item,
    ushort Effect);

public sealed record GotchaMysteryCoinRecord(
    ushort CoinOrder,
    uint CoinIdx,
    ushort CoinCount,
    uint Money,
    ushort StartGroupNo,
    ushort EndGroupNo);

public sealed class GotchaBinLoader : IBinLoader
{
    private const int MaxGroupItem = 10;
    private const int ItemNameLen = 64;
    private const int TCharSize = 2;
    private const int MachineInfoSize = 36;
    private const int ItemInfoSize = 64;
    private const int RareItemInfoSize = 144;
    private const int MysteryItemInfoSize = 12;
    private const int MysteryCoinInfoSizeWithEggPreferment = 20;

    public string TableName => "CsGotchaMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        if (!TryParse(data, binPath, out GotchaBinData? parsed, out BinFileSnapshot snapshot) ||
            parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(
        ReadOnlyMemory<byte> data,
        string binPath,
        out GotchaBinData? parsed,
        out BinFileSnapshot snapshot)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        var sections = new List<BinSectionSnapshot>();

        if (!TryReadMachines(reader, sections, out Dictionary<int, GotchaMachineRecord>? machines) ||
            machines is null ||
            !TryReadItems(reader, sections, out Dictionary<ushort, GotchaItemRecord>? items) ||
            items is null ||
            !TryReadRareItems(reader, sections, out Dictionary<int, GotchaRareItemRecord>? rareItems) ||
            rareItems is null ||
            !TryReadMysteryItems(reader, sections, out Dictionary<uint, GotchaMysteryItemRecord>? mysteryItems) ||
            mysteryItems is null ||
            !TryReadMysteryCoins(reader, sections, out Dictionary<ushort, GotchaMysteryCoinRecord>? mysteryCoins) ||
            mysteryCoins is null)
        {
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        bool complete = reader.EndOfData;
        if (!complete)
        {
            sections.Add(new BinSectionSnapshot("TrailingBytes", reader.Position, 0, null, reader.Remaining, false));
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        snapshot = Snapshot(binPath, data.Length, sections);
        parsed = new GotchaBinData(machines, items, rareItems, mysteryItems, mysteryCoins);
        return true;
    }

    private static bool TryReadMachines(
        SafeBinaryReader reader,
        List<BinSectionSnapshot> sections,
        out Dictionary<int, GotchaMachineRecord>? records)
    {
        records = null;
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int count) ||
            (long)count * MachineInfoSize > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("GotchaMachines", offset, 0, MachineInfoSize, reader.Position - offset, false));
            return false;
        }

        records = new Dictionary<int, GotchaMachineRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(MachineInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseMachine(i, recordBytes, out GotchaMachineRecord? record) ||
                record is null)
            {
                sections.Add(new BinSectionSnapshot("GotchaMachines", offset, count, MachineInfoSize, reader.Position - offset, false));
                return false;
            }

            records[i] = record;
        }

        sections.Add(new BinSectionSnapshot("GotchaMachines", offset, count, MachineInfoSize, reader.Position - offset, true));
        return true;
    }

    private static bool TryReadItems(
        SafeBinaryReader reader,
        List<BinSectionSnapshot> sections,
        out Dictionary<ushort, GotchaItemRecord>? records)
    {
        records = null;
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int count) ||
            (long)count * ItemInfoSize > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("GotchaItems", offset, 0, ItemInfoSize, reader.Position - offset, false));
            return false;
        }

        records = new Dictionary<ushort, GotchaItemRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(ItemInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseItem(recordBytes, out GotchaItemRecord? record) ||
                record is null)
            {
                sections.Add(new BinSectionSnapshot("GotchaItems", offset, count, ItemInfoSize, reader.Position - offset, false));
                return false;
            }

            records[record.GroupId] = record;
        }

        sections.Add(new BinSectionSnapshot("GotchaItems", offset, count, ItemInfoSize, reader.Position - offset, true));
        return true;
    }

    private static bool TryReadRareItems(
        SafeBinaryReader reader,
        List<BinSectionSnapshot> sections,
        out Dictionary<int, GotchaRareItemRecord>? records)
    {
        records = null;
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int count) ||
            (long)count * RareItemInfoSize > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("GotchaRareItems", offset, 0, RareItemInfoSize, reader.Position - offset, false));
            return false;
        }

        records = new Dictionary<int, GotchaRareItemRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(RareItemInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseRareItem(i, recordBytes, out GotchaRareItemRecord? record) ||
                record is null)
            {
                sections.Add(new BinSectionSnapshot("GotchaRareItems", offset, count, RareItemInfoSize, reader.Position - offset, false));
                return false;
            }

            records[i] = record;
        }

        sections.Add(new BinSectionSnapshot("GotchaRareItems", offset, count, RareItemInfoSize, reader.Position - offset, true));
        return true;
    }

    private static bool TryReadMysteryItems(
        SafeBinaryReader reader,
        List<BinSectionSnapshot> sections,
        out Dictionary<uint, GotchaMysteryItemRecord>? records)
    {
        records = null;
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int count) ||
            (long)count * MysteryItemInfoSize > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("GotchaMysteryItems", offset, 0, MysteryItemInfoSize, reader.Position - offset, false));
            return false;
        }

        records = new Dictionary<uint, GotchaMysteryItemRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(MysteryItemInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseMysteryItem(recordBytes, out GotchaMysteryItemRecord? record) ||
                record is null)
            {
                sections.Add(new BinSectionSnapshot("GotchaMysteryItems", offset, count, MysteryItemInfoSize, reader.Position - offset, false));
                return false;
            }

            records[record.Item] = record;
        }

        sections.Add(new BinSectionSnapshot("GotchaMysteryItems", offset, count, MysteryItemInfoSize, reader.Position - offset, true));
        return true;
    }

    private static bool TryReadMysteryCoins(
        SafeBinaryReader reader,
        List<BinSectionSnapshot> sections,
        out Dictionary<ushort, GotchaMysteryCoinRecord>? records)
    {
        records = null;
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int count) ||
            (long)count * MysteryCoinInfoSizeWithEggPreferment > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("GotchaMysteryCoins", offset, 0, MysteryCoinInfoSizeWithEggPreferment, reader.Position - offset, false));
            return false;
        }

        records = new Dictionary<ushort, GotchaMysteryCoinRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(MysteryCoinInfoSizeWithEggPreferment, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseMysteryCoin(recordBytes, out GotchaMysteryCoinRecord? record) ||
                record is null)
            {
                sections.Add(new BinSectionSnapshot("GotchaMysteryCoins", offset, count, MysteryCoinInfoSizeWithEggPreferment, reader.Position - offset, false));
                return false;
            }

            records[record.CoinOrder] = record;
        }

        sections.Add(new BinSectionSnapshot("GotchaMysteryCoins", offset, count, MysteryCoinInfoSizeWithEggPreferment, reader.Position - offset, true));
        return true;
    }

    private static bool TryParseMachine(
        int index,
        ReadOnlyMemory<byte> data,
        out GotchaMachineRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt32(out uint npcId) ||
            !reader.TryReadUInt32(out uint useItemCode) ||
            !reader.TryReadUInt16(out ushort useItemCount) ||
            !reader.TryReadBoolean(out bool limit) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt32(out uint startDate) ||
            !reader.TryReadUInt32(out uint endDate) ||
            !reader.TryReadUInt32(out uint startTime) ||
            !reader.TryReadUInt32(out uint endTime) ||
            !reader.TryReadUInt16(out ushort minLevel) ||
            !reader.TryReadUInt16(out ushort maxLevel) ||
            !reader.TryReadInt32(out int showCountUi) ||
            !reader.EndOfData)
        {
            return false;
        }

        record = new GotchaMachineRecord(index, npcId, useItemCode, useItemCount, limit, startDate, endDate, startTime, endTime, minLevel, maxLevel, showCountUi);
        return true;
    }

    private static bool TryParseItem(ReadOnlyMemory<byte> data, out GotchaItemRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt16(out ushort groupId) ||
            !reader.TrySkip(2))
        {
            return false;
        }

        var itemCodes = new List<uint>(MaxGroupItem);
        for (int i = 0; i < MaxGroupItem; i++)
        {
            if (!reader.TryReadUInt32(out uint itemCode))
            {
                return false;
            }

            itemCodes.Add(itemCode);
        }

        var itemCounts = new List<ushort>(MaxGroupItem);
        for (int i = 0; i < MaxGroupItem; i++)
        {
            if (!reader.TryReadUInt16(out ushort count))
            {
                return false;
            }

            itemCounts.Add(count);
        }

        if (!reader.EndOfData)
        {
            return false;
        }

        record = new GotchaItemRecord(groupId, itemCodes, itemCounts);
        return true;
    }

    private static bool TryParseRareItem(
        int index,
        ReadOnlyMemory<byte> data,
        out GotchaRareItemRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt32(out uint npcId) ||
            !reader.TryReadBytes(ItemNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadUInt32(out uint rareItem) ||
            !reader.TryReadUInt32(out uint rareItemCount) ||
            !reader.TryReadUInt32(out uint rareItemGive) ||
            !reader.EndOfData)
        {
            return false;
        }

        record = new GotchaRareItemRecord(index, npcId, reader.ReadFixedUtf16String(nameBytes.Span), rareItem, rareItemCount, rareItemGive);
        return true;
    }

    private static bool TryParseMysteryItem(ReadOnlyMemory<byte> data, out GotchaMysteryItemRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt16(out ushort group) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint item) ||
            !reader.TryReadUInt16(out ushort effect) ||
            !reader.TrySkip(2) ||
            !reader.EndOfData)
        {
            return false;
        }

        record = new GotchaMysteryItemRecord(group, item, effect);
        return true;
    }

    private static bool TryParseMysteryCoin(ReadOnlyMemory<byte> data, out GotchaMysteryCoinRecord? record)
    {
        record = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt16(out ushort coinOrder) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint coinIdx) ||
            !reader.TryReadUInt16(out ushort coinCount) ||
            !reader.TrySkip(2) ||
            !reader.TryReadUInt32(out uint money) ||
            !reader.TryReadUInt16(out ushort startGroupNo) ||
            !reader.TryReadUInt16(out ushort endGroupNo) ||
            !reader.EndOfData)
        {
            return false;
        }

        record = new GotchaMysteryCoinRecord(coinOrder, coinIdx, coinCount, money, startGroupNo, endGroupNo);
        return true;
    }

    private static BinFileSnapshot Snapshot(
        string path,
        int byteCount,
        IReadOnlyList<BinSectionSnapshot> sections)
    {
        return new BinFileSnapshot("CsGotchaMng", path, byteCount, sections);
    }
}
