namespace DigitalWorldOnline.Application.GameAssets.Bins;

public sealed class DungeonBinLoader
{
    private DungeonBin? _data;

    public DungeonBin Data => _data ?? throw new InvalidOperationException(
        $"{nameof(DungeonBinLoader)}: bins not loaded yet - call Load() first.");

    public bool IsLoaded => _data != null;

    public DungeonBin Load()
    {
        if (_data != null) return _data;

        var dir = BinPath.ResolveDirectory();
        var records = LoadDungeonList(Path.Combine(dir, "DungeonList.bin"));
        var clearInfo = LoadDungeonClearInfo(Path.Combine(dir, "DungeonClearInfo.bin"));
        var rewards = LoadDungeonReward(Path.Combine(dir, "DungeonReward.bin"));
        var steps = LoadDungeonStepInfo(Path.Combine(dir, "DungeonStepInfo.bin"));
        var values = LoadDungeonValue(Path.Combine(dir, "DungeonValue.bin"));
        var qualification = LoadDungeonQualification(Path.Combine(dir, "DungeonQualification.bin"));
        var strings = LoadDungeonString(ResolveDungeonStringPath(dir));

        _data = new DungeonBin(records, clearInfo, rewards, steps, values, qualification, strings);
        return _data;
    }

    private static IReadOnlyList<DungeonListRecord> LoadDungeonList(string path)
    {
        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var count = r.ReadInt32();
        var records = new List<DungeonListRecord>(count);
        for (var i = 0; i < count; i++)
        {
            var dungeonId = r.ReadInt32();
            var difficulty = r.ReadUInt16();
            var sortGroup = r.ReadUInt16();
            var imagePath = ReadFramedString(r);
            var portalId = r.ReadInt32();
            var extra = r.ReadUInt16();

            records.Add(new DungeonListRecord(dungeonId, difficulty, sortGroup, imagePath, portalId, extra));
        }

        return records;
    }

    private static IReadOnlyDictionary<int, DungeonClearRecord> LoadDungeonClearInfo(string path)
    {
        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var count = r.ReadInt32();
        var records = new Dictionary<int, DungeonClearRecord>(count);
        for (var i = 0; i < count; i++)
        {
            var dungeonId = r.ReadInt32();
            var conditionCount = r.ReadInt32();
            var timeLimitSeconds = 0;
            var failLimit = 0;

            for (var c = 0; c < conditionCount; c++)
            {
                var conditionKey = r.ReadByte();
                var value = r.ReadInt32();
                _ = r.ReadByte();

                if (conditionKey == 1)
                    timeLimitSeconds = value;
                else if (conditionKey == 2)
                    failLimit = value;
            }

            records[dungeonId] = new DungeonClearRecord(dungeonId, timeLimitSeconds, failLimit);
        }

        return records;
    }

    private static IReadOnlyDictionary<int, DungeonRewardRecord> LoadDungeonReward(string path)
    {
        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var count = r.ReadInt32();
        var records = new Dictionary<int, DungeonRewardRecord>(count);
        for (var i = 0; i < count; i++)
        {
            var dungeonId = r.ReadInt32();
            var groupCount = r.ReadInt32();
            var groups = new List<DungeonRewardGroup>(groupCount);

            for (var g = 0; g < groupCount; g++)
            {
                var groupKey = r.ReadByte();
                var rewardCount = r.ReadInt32();
                var items = new List<DungeonRewardItem>(rewardCount);
                for (var item = 0; item < rewardCount; item++)
                {
                    var itemId = r.ReadInt32();
                    var amount = r.ReadInt32();
                    items.Add(new DungeonRewardItem(itemId, amount));
                }

                groups.Add(new DungeonRewardGroup(groupKey, items));
            }

            records[dungeonId] = new DungeonRewardRecord(dungeonId, groups);
        }

        return records;
    }

    private static IReadOnlyDictionary<int, DungeonStepRecord> LoadDungeonStepInfo(string path)
    {
        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var count = r.ReadInt32();
        var records = new Dictionary<int, DungeonStepRecord>(count);
        for (var i = 0; i < count; i++)
        {
            var dungeonId = r.ReadInt32();
            var stepGroupCount = r.ReadInt32();
            var steps = new List<DungeonStepGroup>(stepGroupCount);

            for (var s = 0; s < stepGroupCount; s++)
            {
                var stepKey = r.ReadByte();
                var objectiveCount = r.ReadInt32();
                var objectives = new List<DungeonStepObjective>(objectiveCount);

                for (var o = 0; o < objectiveCount; o++)
                {
                    var objectiveId = r.ReadInt32();
                    var fieldA = r.ReadByte();
                    var fieldB = r.ReadInt32();
                    var fieldC = r.ReadByte();
                    var targetMonsterType = r.ReadInt32();
                    var amount = r.ReadInt32();

                    objectives.Add(new DungeonStepObjective(
                        objectiveId,
                        fieldA,
                        fieldB,
                        fieldC,
                        targetMonsterType,
                        amount));
                }

                steps.Add(new DungeonStepGroup(stepKey, objectives));
            }

            records[dungeonId] = new DungeonStepRecord(dungeonId, steps);
        }

        return records;
    }

    private static IReadOnlyDictionary<byte, byte> LoadDungeonValue(string path)
    {
        if (!File.Exists(path))
            return new Dictionary<byte, byte>();

        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var count = r.ReadInt32();
        var records = new Dictionary<byte, byte>(count);
        for (var i = 0; i < count; i++)
        {
            var key = r.ReadByte();
            var value = r.ReadByte();
            records[key] = value;
        }

        return records;
    }

    private static IReadOnlyDictionary<int, DungeonQualificationRecord> LoadDungeonQualification(string path)
    {
        if (!File.Exists(path))
            return new Dictionary<int, DungeonQualificationRecord>();

        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        var count = r.ReadInt32();
        var records = new Dictionary<int, DungeonQualificationRecord>(count);
        for (var i = 0; i < count; i++)
        {
            var dungeonId = r.ReadInt32();
            _ = r.ReadInt32();
            _ = r.ReadInt32();
            var minPlayers = r.ReadUInt16();
            var maxPlayers = r.ReadUInt16();
            var requirementCount = r.ReadInt32();

            for (var req = 0; req < requirementCount; req++)
            {
                _ = r.ReadUInt16();
                var valueCount = r.ReadInt32();
                for (var v = 0; v < valueCount; v++)
                    _ = r.ReadInt32();
            }

            records[dungeonId] = new DungeonQualificationRecord(dungeonId, minPlayers, maxPlayers);
        }

        return records;
    }

    private static string? ResolveDungeonStringPath(string dir)
    {
        var candidates = new[]
        {
            Path.Combine(dir, "Dungeon_Str.bin"),
            Path.Combine(dir, "notused", "Dungeon_Str.bin")
        };

        return candidates.FirstOrDefault(File.Exists);
    }

    private static IReadOnlyDictionary<int, DungeonStringRecord> LoadDungeonString(string? path)
    {
        if (string.IsNullOrWhiteSpace(path) || !File.Exists(path))
            return new Dictionary<int, DungeonStringRecord>();

        if (TryLoadDungeonString(path, true, out var characterLengthRecords))
            return characterLengthRecords;

        return TryLoadDungeonString(path, false, out var byteLengthRecords)
            ? byteLengthRecords
            : new Dictionary<int, DungeonStringRecord>();
    }

    private static bool TryLoadDungeonString(
        string path,
        bool lengthIsCharacters,
        out IReadOnlyDictionary<int, DungeonStringRecord> records)
    {
        records = new Dictionary<int, DungeonStringRecord>();

        try
        {
            using var fs = File.OpenRead(path);
            using var r = new BinaryReader(fs);

            var count = r.ReadInt32();
            if (count < 0 || count > 10000)
                return false;

            var loaded = new Dictionary<int, DungeonStringRecord>(count);
            for (var i = 0; i < count; i++)
            {
                var dungeonId = r.ReadInt32();
                var name = ReadFramedUnicodeString(r, lengthIsCharacters);
                var description = ReadFramedUnicodeString(r, lengthIsCharacters);

                loaded[dungeonId] = new DungeonStringRecord(dungeonId, name, description);
            }

            records = loaded;
            return true;
        }
        catch (EndOfStreamException)
        {
            return false;
        }
        catch (IOException)
        {
            return false;
        }
        catch (ArgumentOutOfRangeException)
        {
            return false;
        }
    }

    private static string ReadFramedUnicodeString(BinaryReader r, bool lengthIsCharacters)
    {
        var length = r.ReadInt32();
        if (length <= 0)
            return string.Empty;

        var byteLength = lengthIsCharacters ? checked(length * 2) : length;
        if (byteLength < 0 || byteLength > 65536 || byteLength > r.BaseStream.Length - r.BaseStream.Position)
            throw new EndOfStreamException();

        var bytes = r.ReadBytes(byteLength);
        return System.Text.Encoding.Unicode.GetString(bytes).TrimEnd('\0');
    }

    private static string ReadFramedString(BinaryReader r)
    {
        var length = r.ReadInt32();
        if (length <= 0)
            return string.Empty;

        var bytes = r.ReadBytes(length);
        return System.Text.Encoding.Default.GetString(bytes).TrimEnd('\0');
    }
}

public sealed class DungeonBin
{
    private static readonly IReadOnlyDictionary<int, (int BaseMapId, ushort Difficulty)> RuntimeDUnitMap =
        new Dictionary<int, (int BaseMapId, ushort Difficulty)>
        {
            [1605] = (1600, 1),
            [1604] = (1601, 1),
            [1606] = (1602, 1),
            [1607] = (1603, 1),
            [1600] = (1600, 2),
            [1601] = (1601, 2),
            [1602] = (1602, 2),
            [1603] = (1603, 2),
            [1610] = (1600, 3),
            [1611] = (1601, 3),
            [1612] = (1602, 3),
            [1613] = (1603, 3),
            [1615] = (1614, 1),
            [1609] = (1608, 2),
            [1616] = (1616, 3)
        };

    public DungeonBin(
        IReadOnlyList<DungeonListRecord> list,
        IReadOnlyDictionary<int, DungeonClearRecord> clearInfo,
        IReadOnlyDictionary<int, DungeonRewardRecord> rewards,
        IReadOnlyDictionary<int, DungeonStepRecord> steps,
        IReadOnlyDictionary<byte, byte> values,
        IReadOnlyDictionary<int, DungeonQualificationRecord> qualification,
        IReadOnlyDictionary<int, DungeonStringRecord> strings)
    {
        List = list;
        ListByDungeonId = list.ToDictionary(x => x.DungeonId, x => x);
        ClearInfo = clearInfo;
        Rewards = rewards;
        Steps = steps;
        Values = values;
        Qualification = qualification;
        Strings = strings;
    }

    public IReadOnlyList<DungeonListRecord> List { get; }
    public IReadOnlyDictionary<int, DungeonListRecord> ListByDungeonId { get; }
    public IReadOnlyDictionary<int, DungeonClearRecord> ClearInfo { get; }
    public IReadOnlyDictionary<int, DungeonRewardRecord> Rewards { get; }
    public IReadOnlyDictionary<int, DungeonStepRecord> Steps { get; }
    public IReadOnlyDictionary<byte, byte> Values { get; }
    public IReadOnlyDictionary<int, DungeonQualificationRecord> Qualification { get; }
    public IReadOnlyDictionary<int, DungeonStringRecord> Strings { get; }

    public DungeonListRecord? ResolveByRuntimeMapId(int mapId, int entryPortalId = 0)
    {
        if (entryPortalId > 0)
        {
            var byPortal = List.FirstOrDefault(x => x.PortalId == entryPortalId);
            if (byPortal != null)
                return byPortal;
        }

        if (RuntimeDUnitMap.TryGetValue(mapId, out var mapped))
        {
            return List.FirstOrDefault(x =>
                x.BaseMapId == mapped.BaseMapId &&
                x.Difficulty == mapped.Difficulty);
        }

        var matches = List
            .Where(x => x.BaseMapId == mapId)
            .OrderBy(x => x.SortGroup)
            .ThenBy(x => x.Difficulty)
            .ToList();

        return matches.Count == 1 ? matches[0] : matches.FirstOrDefault();
    }

    public IReadOnlyList<DungeonStepGroup> GetOrderedSteps(int dungeonId)
    {
        if (!Steps.TryGetValue(dungeonId, out var step) || step.Groups.Count == 0)
            return Array.Empty<DungeonStepGroup>();

        return step.Groups
            .OrderBy(x => x.StepKey)
            .ToList();
    }

    public DungeonStepGroup? GetObjectiveStep(int dungeonId, int mobType)
    {
        return GetOrderedSteps(dungeonId)
            .FirstOrDefault(x => x.Objectives.Any(y => y.TargetMonsterType == mobType));
    }

    public DungeonStepGroup? GetNextStep(int dungeonId, byte currentStepKey)
    {
        return GetOrderedSteps(dungeonId)
            .FirstOrDefault(x => x.StepKey > currentStepKey);
    }

    public bool IsStepObjective(int dungeonId, int mobType) => GetObjectiveStep(dungeonId, mobType) != null;

    public bool HasCompletedPriorSteps(int dungeonId, byte stepKey, IReadOnlySet<byte> completedStepKeys)
    {
        return GetOrderedSteps(dungeonId)
            .Where(x => x.StepKey < stepKey)
            .All(x => completedStepKeys.Contains(x.StepKey));
    }

    public bool IsFinalObjective(int dungeonId, int mobType)
    {
        var steps = GetOrderedSteps(dungeonId);
        if (steps.Count == 0)
            return false;

        var finalStep = steps[^1];
        return finalStep.Objectives.Any(x => x.TargetMonsterType == mobType);
    }

    public byte CalculateRank(int dungeonId, int elapsedSeconds, int failCount)
    {
        ClearInfo.TryGetValue(dungeonId, out var clear);

        if (clear != null && clear.FailLimit > 0 && failCount >= clear.FailLimit)
            return 4;

        var timeLimit = clear?.TimeLimitSeconds ?? 0;
        if (timeLimit <= 0)
            return 0;

        var percent = elapsedSeconds * 100 / Math.Max(1, timeLimit);
        var sLimit = Values.TryGetValue(1, out var s) ? s : (byte)40;
        var aLimit = Values.TryGetValue(4, out var a) ? a : (byte)60;
        var bLimit = Values.TryGetValue(6, out var b) ? b : (byte)80;

        if (percent <= sLimit && failCount == 0)
            return 0;
        if (percent <= aLimit)
            return 1;
        if (percent <= bLimit)
            return 2;

        return 3;
    }

    public string GetDisplayName(DungeonListRecord record)
    {
        if (Strings.TryGetValue(record.DungeonId, out var exact) && !string.IsNullOrWhiteSpace(exact.Name))
            return exact.Name;

        var baseDungeonId = record.BaseMapId * 100;
        if (Strings.TryGetValue(baseDungeonId, out var baseString) && !string.IsNullOrWhiteSpace(baseString.Name))
            return baseString.Name;

        return $"Dungeon {record.DungeonId}";
    }

    public ushort GetMaxPlayers(int dungeonId)
    {
        return Qualification.TryGetValue(dungeonId, out var value) && value.MaxPlayers > 0
            ? value.MaxPlayers
            : (ushort)1;
    }
}

public sealed record DungeonListRecord(
    int DungeonId,
    ushort Difficulty,
    ushort SortGroup,
    string ImagePath,
    int PortalId,
    ushort Extra)
{
    public int BaseMapId => DungeonId / 100;
}

public sealed record DungeonClearRecord(int DungeonId, int TimeLimitSeconds, int FailLimit);

public sealed record DungeonRewardRecord(int DungeonId, IReadOnlyList<DungeonRewardGroup> Groups);

public sealed record DungeonRewardGroup(byte GroupKey, IReadOnlyList<DungeonRewardItem> Items);

public sealed record DungeonRewardItem(int ItemId, int Amount);

public sealed record DungeonStepRecord(int DungeonId, IReadOnlyList<DungeonStepGroup> Groups);

public sealed record DungeonStepGroup(byte StepKey, IReadOnlyList<DungeonStepObjective> Objectives);

public sealed record DungeonStepObjective(
    int ObjectiveId,
    byte FieldA,
    int FieldB,
    byte FieldC,
    int TargetMonsterType,
    int Amount);

public sealed record DungeonQualificationRecord(int DungeonId, ushort MinPlayers, ushort MaxPlayers);

public sealed record DungeonStringRecord(int DungeonId, string Name, string Description);
