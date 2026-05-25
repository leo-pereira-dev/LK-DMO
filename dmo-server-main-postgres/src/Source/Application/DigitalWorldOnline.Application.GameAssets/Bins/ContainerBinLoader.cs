using System.Text;
using DigitalWorldOnline.Commons.DTOs.Assets;

namespace DigitalWorldOnline.Application.GameAssets.Bins;

public sealed class ContainerBinLoader
{
    private const string ContainerFileName = "Container.bin";
    private const string RandomBoxListFileName = "RandomBoxList.bin";
    private const string RandomBoxItemFileName = "RandomBoxItem.bin";
    private const string RandomBoxRewardFileName = "RandomBoxReward.bin";
    private const uint ContainerMagic = 0x4E54434C; // LCTN
    private const uint ContainerVersion = 1;
    private const double ChanceScale = 100.0;
    private const byte RareRankThreshold = 4;

    private readonly ItemListBinLoader _itemListBinLoader;
    private ContainerBin? _data;

    public ContainerBinLoader(ItemListBinLoader itemListBinLoader)
    {
        _itemListBinLoader = itemListBinLoader;
    }

    public ContainerBin Data => _data ?? throw new InvalidOperationException(
        $"{nameof(ContainerBinLoader)}: bins not loaded yet - call Load() first.");

    public bool IsLoaded => _data != null;

    public ContainerBin Load()
    {
        if (_data != null) return _data;

        var dir = BinPath.ResolveDirectory();
        var containerPath = Path.Combine(dir, ContainerFileName);
        if (File.Exists(containerPath))
        {
            _data = ReadContainerBin(containerPath);
            return _data;
        }

        var randomBoxes = ReadRandomBoxList(Path.Combine(dir, RandomBoxListFileName));
        var itemGroups = ReadRandomBoxItems(Path.Combine(dir, RandomBoxItemFileName));
        var rewardGroups = ReadRandomBoxRewards(Path.Combine(dir, RandomBoxRewardFileName));
        var itemNames = _itemListBinLoader.Load().Items.ToDictionary(x => x.ItemId, x => x.Name);

        var containers = new List<ContainerAssetDTO>();
        var missingItemGroups = 0;
        var missingRewardGroups = 0;
        long containerId = 1;
        long rewardId = 1;

        foreach (var box in randomBoxes)
        {
            if (!itemGroups.TryGetValue(box.ItemGroupId, out var rewardEntries))
            {
                missingItemGroups++;
                if (!rewardGroups.TryGetValue(box.RewardGroupId, out rewardEntries))
                {
                    missingRewardGroups++;
                    continue;
                }
            }

            var rewards = new List<ContainerRewardAssetDTO>();
            foreach (var entry in rewardEntries)
            {
                if (entry.ItemId <= 0)
                    continue;

                rewards.Add(new ContainerRewardAssetDTO
                {
                    Id = rewardId++,
                    ItemId = entry.ItemId,
                    ItemName = itemNames.TryGetValue(entry.ItemId, out var rewardName)
                        ? rewardName
                        : entry.ItemId.ToString(),
                    MinAmount = 1,
                    MaxAmount = 1,
                    Chance = NormalizeChance(entry.ChanceRaw),
                    Rare = entry.Rank >= RareRankThreshold,
                    ContainerAssetId = containerId
                });
            }

            if (rewards.Count == 0)
                continue;

            containers.Add(new ContainerAssetDTO
            {
                Id = containerId,
                ItemId = box.BoxId,
                ItemName = itemNames.TryGetValue(box.BoxId, out var boxName)
                    ? boxName
                    : box.BoxId.ToString(),
                RewardAmount = 1,
                Rewards = rewards
            });

            containerId++;
        }

        _data = new ContainerBin(
            containers,
            randomBoxes.Count,
            itemGroups.Count,
            rewardGroups.Count,
            missingItemGroups,
            missingRewardGroups);

        return _data;
    }

    private static ContainerBin ReadContainerBin(string path)
    {
        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var magic = r.ReadUInt32();
        if (magic != ContainerMagic)
            throw new InvalidDataException($"{ContainerFileName}: invalid magic 0x{magic:X8}.");

        var version = r.ReadUInt32();
        if (version != ContainerVersion)
            throw new InvalidDataException($"{ContainerFileName}: unsupported version {version}.");

        var containerCount = checked((int)r.ReadUInt32());
        var containers = new List<ContainerAssetDTO>(containerCount);
        for (var i = 0; i < containerCount; i++)
        {
            var container = new ContainerAssetDTO
            {
                Id = r.ReadInt64(),
                ItemId = r.ReadInt32(),
                RewardAmount = r.ReadInt32(),
                ItemName = ReadPackedString(r),
                Rewards = new List<ContainerRewardAssetDTO>()
            };

            var rewardCount = checked((int)r.ReadUInt32());
            for (var rewardIndex = 0; rewardIndex < rewardCount; rewardIndex++)
            {
                container.Rewards.Add(new ContainerRewardAssetDTO
                {
                    Id = r.ReadInt64(),
                    ItemId = r.ReadInt32(),
                    MinAmount = r.ReadInt32(),
                    MaxAmount = r.ReadInt32(),
                    Chance = r.ReadDouble(),
                    Rare = r.ReadByte() != 0,
                    ItemName = ReadPackedString(r),
                    ContainerAssetId = container.Id,
                    ContainerAsset = container
                });
            }

            containers.Add(container);
        }

        return new ContainerBin(
            containers,
            containers.Count,
            containers.Sum(x => x.Rewards?.Count ?? 0),
            0,
            0,
            0);
    }

    private static double NormalizeChance(ushort raw)
    {
        if (raw == 0)
            return 0.01;

        return Math.Clamp(raw / ChanceScale, 0.01, 100.0);
    }

    private static IReadOnlyList<RandomBoxRow> ReadRandomBoxList(string path)
    {
        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var count = checked((int)r.ReadUInt32());
        var rows = new List<RandomBoxRow>(count);
        for (var i = 0; i < count; i++)
        {
            var boxId = checked((int)r.ReadUInt32());
            _ = r.ReadByte();

            var consumeCount = checked((int)r.ReadUInt32());
            for (var c = 0; c < consumeCount; c++)
            {
                _ = r.ReadUInt32();
                _ = r.ReadUInt16();
            }

            var itemGroupId = checked((int)r.ReadUInt32());
            var rewardGroupId = checked((int)r.ReadUInt32());
            _ = r.ReadUInt32();
            _ = r.ReadUInt16();
            _ = ReadString(r);

            rows.Add(new RandomBoxRow(boxId, itemGroupId, rewardGroupId));
        }

        return rows;
    }

    private static IReadOnlyDictionary<int, IReadOnlyList<RandomBoxRewardEntry>> ReadRandomBoxItems(string path)
        => ReadRewardGroups(path);

    private static IReadOnlyDictionary<int, IReadOnlyList<RandomBoxRewardEntry>> ReadRandomBoxRewards(string path)
        => ReadRewardGroups(path);

    private static IReadOnlyDictionary<int, IReadOnlyList<RandomBoxRewardEntry>> ReadRewardGroups(string path)
    {
        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var count = checked((int)r.ReadUInt32());
        var groups = new Dictionary<int, IReadOnlyList<RandomBoxRewardEntry>>(count);
        for (var i = 0; i < count; i++)
        {
            var groupId = checked((int)r.ReadUInt32());
            var itemCount = checked((int)r.ReadUInt32());
            var entries = new List<RandomBoxRewardEntry>(itemCount);
            for (var item = 0; item < itemCount; item++)
            {
                entries.Add(new RandomBoxRewardEntry(
                    checked((int)r.ReadUInt32()),
                    r.ReadUInt16(),
                    r.ReadByte(),
                    r.ReadUInt32()));
            }

            groups[groupId] = entries;
        }

        return groups;
    }

    private static string ReadString(BinaryReader r)
    {
        var byteCount = checked((int)r.ReadUInt32());
        if (byteCount <= 0)
            return string.Empty;

        return Encoding.UTF8.GetString(r.ReadBytes(byteCount));
    }

    private static string ReadPackedString(BinaryReader r)
    {
        var byteCount = checked((int)r.ReadUInt16());
        if (byteCount <= 0)
            return string.Empty;

        return Encoding.UTF8.GetString(r.ReadBytes(byteCount));
    }

    private sealed record RandomBoxRow(int BoxId, int ItemGroupId, int RewardGroupId);

    private sealed record RandomBoxRewardEntry(int ItemId, ushort ChanceRaw, byte Rank, uint Unknown);
}
