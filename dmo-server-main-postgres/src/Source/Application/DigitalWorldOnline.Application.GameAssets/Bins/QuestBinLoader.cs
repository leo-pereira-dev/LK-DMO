using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Asset;

namespace DigitalWorldOnline.Application.GameAssets.Bins;

/// <summary>
/// Parses the Quest.bin layout used by the English client pack. The runtime
/// server only consumes structural quest data; localized text is skipped.
/// </summary>
public sealed class QuestBinLoader
{
    private const string FileName = "Quest.bin";
    private const int QuestTitleBytes = 160;
    private const int QuestBodyBytes = 4096;
    private const int QuestSimpleBytes = 256;
    private const int QuestHelperBytes = 1024;
    private const int QuestProcessBytes = 640;
    private const int QuestCompleteBytes = 1400;
    private const int QuestExpertBytes = 640;

    private QuestBin? _data;

    public QuestBin Data => _data ?? throw new InvalidOperationException(
        $"{nameof(QuestBinLoader)}: bin not loaded yet - call Load() first.");

    public bool IsLoaded => _data != null;

    public QuestBin Load()
    {
        if (_data != null) return _data;

        var path = Path.Combine(BinPath.ResolveDirectory(), FileName);
        using var stream = File.OpenRead(path);
        using var reader = new BinaryReader(stream);
        _data = Parse(reader);
        return _data;
    }

    public static QuestBin Parse(BinaryReader reader)
    {
        var questCount = reader.ReadInt32();
        var quests = new List<QuestAssetModel>(questCount);

        for (var i = 0; i < questCount; i++)
            quests.Add(ReadQuest(reader, i + 1));

        return new QuestBin(quests);
    }

    private static QuestAssetModel ReadQuest(BinaryReader reader, long sequence)
    {
        var questId = checked((int)reader.ReadUInt32());
        _ = reader.ReadUInt32(); // ModelIdRecv
        _ = reader.ReadUInt32(); // ModelIdComplete
        var level = reader.ReadInt16();
        _ = reader.ReadInt32(); // Tool position X
        _ = reader.ReadInt32(); // Tool position Y
        _ = reader.ReadUInt32(); // ManagedId
        _ = reader.ReadBoolean(); // Active
        _ = reader.ReadBoolean(); // Immediate
        _ = reader.ReadBoolean(); // ResetQuest

        var questType = (QuestTypeEnum)reader.ReadUInt32();
        _ = reader.ReadUInt32(); // StartTargetType
        _ = reader.ReadUInt32(); // StartTargetId
        var targetType = checked((int)reader.ReadUInt32());
        var targetValue = reader.ReadInt32();

        SkipText(reader, QuestTitleBytes);    // TitleTab
        SkipText(reader, QuestTitleBytes);    // TitleText
        SkipText(reader, QuestBodyBytes);     // Body
        SkipText(reader, QuestSimpleBytes);   // Simple
        SkipText(reader, QuestHelperBytes);   // Helper
        SkipText(reader, QuestProcessBytes);  // Process
        SkipText(reader, QuestCompleteBytes); // Complete
        SkipText(reader, QuestExpertBytes);   // Expert

        var quest = new QuestAssetModel
        {
            Id = sequence,
            QuestId = questId,
            QuestType = questType,
            TargetType = targetType,
            TargetValue = targetValue,
            unlockedLevel = level,
            QuestSupplies = ReadSupplies(reader, sequence),
            QuestConditions = ReadConditions(reader, sequence),
            QuestGoals = ReadGoals(reader, sequence),
            QuestRewards = ReadRewards(reader, sequence),
            QuestEvents = ReadEvents(reader, sequence)
        };

        AttachParents(quest);
        return quest;
    }

    private static List<QuestSupplyAssetModel> ReadSupplies(BinaryReader reader, long questSequence)
    {
        var count = reader.ReadInt32();
        var supplies = new List<QuestSupplyAssetModel>(count);

        for (var i = 0; i < count; i++)
        {
            _ = reader.ReadUInt32(); // Type
            supplies.Add(new QuestSupplyAssetModel
            {
                Id = BuildChildId(questSequence, i),
                ItemId = checked((int)reader.ReadUInt32()),
                Amount = reader.ReadInt32(),
                QuestId = questSequence
            });
        }

        return supplies;
    }

    private static List<QuestConditionAssetModel> ReadConditions(BinaryReader reader, long questSequence)
    {
        var count = reader.ReadInt32();
        var conditions = new List<QuestConditionAssetModel>(count);

        for (var i = 0; i < count; i++)
        {
            conditions.Add(new QuestConditionAssetModel
            {
                Id = BuildChildId(questSequence, i),
                ConditionType = checked((int)reader.ReadUInt32()),
                ConditionId = checked((int)reader.ReadUInt32()),
                ConditionCount = reader.ReadInt32(),
                QuestId = questSequence
            });
        }

        return conditions;
    }

    private static List<QuestGoalAssetModel> ReadGoals(BinaryReader reader, long questSequence)
    {
        var count = reader.ReadInt32();
        var goals = new List<QuestGoalAssetModel>(count);

        for (var i = 0; i < count; i++)
        {
            goals.Add(new QuestGoalAssetModel
            {
                Id = BuildChildId(questSequence, i),
                GoalType = (QuestGoalTypeEnum)reader.ReadUInt32(),
                GoalId = checked((int)reader.ReadUInt32()),
                GoalAmount = reader.ReadInt32(),
                CurTypeCount = checked((int)reader.ReadUInt32()),
                SubValue = reader.ReadInt32(),
                SubValueTwo = reader.ReadInt32(),
                QuestId = questSequence
            });
        }

        return goals;
    }

    private static List<QuestRewardAssetModel> ReadRewards(BinaryReader reader, long questSequence)
    {
        var count = reader.ReadInt32();
        var rewards = new List<QuestRewardAssetModel>(count);

        for (var i = 0; i < count; i++)
        {
            var method = checked((int)reader.ReadUInt32());
            var type = checked((int)reader.ReadUInt32());
            var valueA = checked((int)reader.ReadUInt32());
            var valueB = reader.ReadInt32();
            var rewardId = BuildChildId(questSequence, i);
            var rewardType = MapRewardType(type);

            rewards.Add(new QuestRewardAssetModel
            {
                Id = rewardId,
                Reward = method,
                RewardType = rewardType,
                QuestId = questSequence,
                RewardObjectList = new List<QuestRewardObjectAssetModel>
                {
                    new()
                    {
                        Id = BuildChildId(rewardId, 0),
                        Reward = rewardType == QuestRewardTypeEnum.MoneyReward ? 0 : valueA,
                        Amount = rewardType == QuestRewardTypeEnum.MoneyReward ? valueA : valueB,
                        QuestRewardId = rewardId
                    }
                }
            });
        }

        return rewards;
    }

    private static List<QuestEventAssetModel> ReadEvents(BinaryReader reader, long questSequence)
    {
        var count = reader.ReadInt32();
        var events = new List<QuestEventAssetModel>(count);

        for (var i = 0; i < count; i++)
        {
            events.Add(new QuestEventAssetModel
            {
                Id = BuildChildId(questSequence, i),
                EventId = reader.ReadInt32(),
                QuestId = questSequence
            });
        }

        return events;
    }

    private static QuestRewardTypeEnum MapRewardType(int type)
    {
        return type switch
        {
            0 => QuestRewardTypeEnum.MoneyReward,
            1 => QuestRewardTypeEnum.ExperienceReward,
            2 => QuestRewardTypeEnum.ItemReward,
            _ => QuestRewardTypeEnum.ItemReward
        };
    }

    private static long BuildChildId(long parentId, int index)
    {
        return checked(parentId * 1000 + index + 1);
    }

    private static void AttachParents(QuestAssetModel quest)
    {
        quest.QuestSupplies.ForEach(x => x.Quest = quest);
        quest.QuestConditions.ForEach(x => x.Quest = quest);
        quest.QuestGoals.ForEach(x => x.Quest = quest);
        quest.QuestEvents.ForEach(x => x.Quest = quest);

        quest.QuestRewards.ForEach(reward =>
        {
            reward.Quest = quest;
            reward.RewardObjectList.ForEach(rewardObject => rewardObject.QuestReward = reward);
        });
    }

    private static void SkipText(BinaryReader reader, int byteCount)
    {
        var skipped = reader.ReadBytes(byteCount);
        if (skipped.Length != byteCount)
            throw new EndOfStreamException($"Unexpected end of {FileName} while skipping quest text.");
    }
}

public sealed record QuestBin(IReadOnlyList<QuestAssetModel> Quests);
