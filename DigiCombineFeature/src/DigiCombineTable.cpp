#include "../include/DigiCombineTable.h"

#include <fstream>
#include <regex>
#include <sstream>

namespace DigiCombine
{
	namespace
	{
		bool ReadTextFile(const std::filesystem::path& path, std::string& outText)
		{
			std::ifstream file(path, std::ios::binary);
			if (!file)
				return false;

			std::ostringstream buffer;
			buffer << file.rdbuf();
			outText = buffer.str();
			return true;
		}

		std::optional<std::uint32_t> ReadLeafValue(const std::string& block, const char* leafName)
		{
			const std::regex leafRegex(
				std::string("<") + leafName + R"xml(\b[^>]*\bvalue="([0-9]+)"\s*/>)xml",
				std::regex::icase);

			std::smatch match;
			if (!std::regex_search(block, match, leafRegex))
				return std::nullopt;

			return static_cast<std::uint32_t>(std::stoul(match[1].str()));
		}

		std::vector<std::string> ExtractRecordBlocks(const std::string& text)
		{
			std::vector<std::string> records;
			const std::regex recordRegex(R"(<_\d+\b[^>]*kind="Record"[^>]*>([\s\S]*?)</_\d+>)");

			for (std::sregex_iterator it(text.begin(), text.end(), recordRegex), end; it != end; ++it)
				records.push_back((*it)[1].str());

			return records;
		}

		std::vector<std::string> ExtractEntryBlocks(const std::string& recordBlock)
		{
			std::vector<std::string> entries;
			const std::regex entryRegex(R"(<_\d+_\b[^>]*kind="Struct"[^>]*>([\s\S]*?)</_\d+_>)");

			for (std::sregex_iterator it(recordBlock.begin(), recordBlock.end(), entryRegex), end; it != end; ++it)
				entries.push_back((*it)[1].str());

			return entries;
		}

		const char* Prefix(CombineFamily family)
		{
			return family == CombineFamily::Digi ? "DigiCombine" : "UnionCombine";
		}
	}

	std::filesystem::path MakeTableFileName(CombineFamily family, const char* tableName)
	{
		return std::filesystem::path(std::string(Prefix(family)) + tableName + ".xml");
	}

	bool CombineTableRepository::LoadFromXmlDirectory(CombineFamily family, const std::filesystem::path& directory)
	{
		tables_ = CombineTableSet();
		tables_.family = family;

		const bool loadedItems = LoadItemList(directory / MakeTableFileName(family, "ItemList"));
		const bool loadedRanks = LoadRankList(directory / MakeTableFileName(family, "RankList"));
		const bool loadedNormalRewards = LoadRewardGroups(directory / MakeTableFileName(family, "ItemGroup"), RewardPoolKind::Normal);
		const bool loadedCeilingRewards = LoadRewardGroups(directory / MakeTableFileName(family, "CeilGroup"), RewardPoolKind::Ceiling);

		return loadedItems && loadedRanks && loadedNormalRewards && loadedCeilingRewards;
	}

	const CombineTableSet& CombineTableRepository::Tables() const
	{
		return tables_;
	}

	const CombineItemDefinition* CombineTableRepository::FindItem(std::uint32_t itemId) const
	{
		for (const CombineItemDefinition& item : tables_.items)
		{
			if (item.itemId == itemId)
				return &item;
		}
		return nullptr;
	}

	const CombineRankRule* CombineTableRepository::FindRank(std::uint8_t rankKey) const
	{
		for (const CombineRankRule& rank : tables_.ranks)
		{
			if (rank.rankKey == rankKey)
				return &rank;
		}
		return nullptr;
	}

	const CombineRewardGroup* CombineTableRepository::FindRewardGroup(std::uint16_t groupKey, RewardPoolKind poolKind) const
	{
		const std::vector<CombineRewardGroup>& groups =
			poolKind == RewardPoolKind::Normal ? tables_.normalRewardGroups : tables_.ceilingRewardGroups;

		for (const CombineRewardGroup& group : groups)
		{
			if (group.groupKey == groupKey)
				return &group;
		}
		return nullptr;
	}

	bool CombineTableRepository::LoadItemList(const std::filesystem::path& path)
	{
		std::string text;
		if (!ReadTextFile(path, text))
			return false;

		for (const std::string& record : ExtractRecordBlocks(text))
		{
			const std::optional<std::uint32_t> itemId = ReadLeafValue(record, "ItemId");
			const std::optional<std::uint32_t> rankKey = ReadLeafValue(record, "FlagOrType");
			const std::optional<std::uint32_t> catalogValue = ReadLeafValue(record, "Value");
			if (!itemId || !rankKey || !catalogValue)
				continue;

			CombineItemDefinition item;
			item.itemId = *itemId;
			item.rankKey = static_cast<std::uint8_t>(*rankKey);
			item.catalogValue = *catalogValue;
			tables_.items.push_back(item);
		}

		return !tables_.items.empty();
	}

	bool CombineTableRepository::LoadRankList(const std::filesystem::path& path)
	{
		std::string text;
		if (!ReadTextFile(path, text))
			return false;

		for (const std::string& record : ExtractRecordBlocks(text))
		{
			const std::optional<std::uint32_t> rankKey = ReadLeafValue(record, "RankKey");
			const std::optional<std::uint32_t> displayRank = ReadLeafValue(record, "FieldA");
			const std::optional<std::uint32_t> ceilingFailCount = ReadLeafValue(record, "FieldB");
			const std::optional<std::uint32_t> ceilingGroupKey = ReadLeafValue(record, "FieldC");
			if (!rankKey || !displayRank || !ceilingFailCount || !ceilingGroupKey)
				continue;

			CombineRankRule rank;
			rank.rankKey = static_cast<std::uint8_t>(*rankKey);
			rank.displayRank = static_cast<std::uint16_t>(*displayRank);
			rank.ceilingFailCount = static_cast<std::uint16_t>(*ceilingFailCount);
			rank.ceilingGroupKey = static_cast<std::uint16_t>(*ceilingGroupKey);
			tables_.ranks.push_back(rank);
		}

		return !tables_.ranks.empty();
	}

	bool CombineTableRepository::LoadRewardGroups(const std::filesystem::path& path, RewardPoolKind poolKind)
	{
		std::string text;
		if (!ReadTextFile(path, text))
			return false;

		std::vector<CombineRewardGroup>& groups =
			poolKind == RewardPoolKind::Normal ? tables_.normalRewardGroups : tables_.ceilingRewardGroups;

		for (const std::string& record : ExtractRecordBlocks(text))
		{
			const std::optional<std::uint32_t> groupKey = ReadLeafValue(record, "GroupKey");
			if (!groupKey)
				continue;

			CombineRewardGroup group;
			group.groupKey = static_cast<std::uint16_t>(*groupKey);
			group.poolKind = poolKind;

			for (const std::string& entryBlock : ExtractEntryBlocks(record))
			{
				const std::optional<std::uint32_t> rewardItemId = ReadLeafValue(entryBlock, "ItemId");
				const std::optional<std::uint32_t> rewardCount = ReadLeafValue(entryBlock, "FieldA");
				const std::optional<std::uint32_t> chanceBasisPoints = ReadLeafValue(entryBlock, "FieldB");
				const std::optional<std::uint32_t> resultGradeBand = ReadLeafValue(entryBlock, "FieldC");
				const std::optional<std::uint32_t> resultRank = ReadLeafValue(entryBlock, "FieldD");
				const std::optional<std::uint32_t> tierUpFlag = ReadLeafValue(entryBlock, "FieldE");
				if (!rewardItemId || !rewardCount || !chanceBasisPoints || !resultGradeBand || !resultRank || !tierUpFlag)
					continue;

				CombineRewardEntry entry;
				entry.rewardItemId = *rewardItemId;
				entry.rewardCount = static_cast<std::uint16_t>(*rewardCount);
				entry.chanceBasisPoints = static_cast<std::uint16_t>(*chanceBasisPoints);
				entry.resultGradeBand = static_cast<std::uint8_t>(*resultGradeBand);
				entry.resultRank = static_cast<std::uint8_t>(*resultRank);
				entry.isTierUpReward = *tierUpFlag != 0;
				group.rewards.push_back(entry);
			}

			if (!group.rewards.empty())
				groups.push_back(group);
		}

		return !groups.empty();
	}
}
