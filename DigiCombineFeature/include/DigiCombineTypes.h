#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace DigiCombine
{
	enum class CombineFamily
	{
		Digi,
		Union,
	};

	enum class RewardPoolKind
	{
		Normal,
		Ceiling,
	};

	struct CombineItemDefinition
	{
		std::uint32_t itemId = 0;
		std::uint8_t rankKey = 0;
		std::uint32_t catalogValue = 0;
	};

	struct CombineRankRule
	{
		std::uint8_t rankKey = 0;
		std::uint16_t displayRank = 0;
		std::uint16_t ceilingFailCount = 0;
		std::uint16_t ceilingGroupKey = 0;
	};

	struct CombineRewardEntry
	{
		std::uint32_t rewardItemId = 0;
		std::uint16_t rewardCount = 0;
		std::uint16_t chanceBasisPoints = 0;
		std::uint8_t resultGradeBand = 0;
		std::uint8_t resultRank = 0;
		bool isTierUpReward = false;
	};

	struct CombineRewardGroup
	{
		std::uint16_t groupKey = 0;
		RewardPoolKind poolKind = RewardPoolKind::Normal;
		std::vector<CombineRewardEntry> rewards;
	};

	struct CombineTableSet
	{
		CombineFamily family = CombineFamily::Digi;
		std::vector<CombineItemDefinition> items;
		std::vector<CombineRankRule> ranks;
		std::vector<CombineRewardGroup> normalRewardGroups;
		std::vector<CombineRewardGroup> ceilingRewardGroups;
	};

	struct CombineMaterialSlot
	{
		std::uint32_t itemId = 0;
		std::uint16_t inventorySlot = 0;
		std::uint16_t amount = 1;
	};

	struct CombineRequest
	{
		CombineFamily family = CombineFamily::Digi;
		std::vector<CombineMaterialSlot> materials;
		std::uint16_t currentFailCount = 0;
	};

	struct CombineRatePreview
	{
		std::uint8_t materialRank = 0;
		std::uint16_t selectedGroupKey = 0;
		bool usesCeilingPool = false;
		std::vector<CombineRewardEntry> rewards;
	};
}
