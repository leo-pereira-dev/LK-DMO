#include "../include/DigiCombineService.h"

#include <numeric>

namespace DigiCombine
{
	namespace
	{
		constexpr std::size_t RequiredMaterialCount = 4;

		std::uint16_t NormalGroupForRank(std::uint8_t rank)
		{
			return static_cast<std::uint16_t>(rank);
		}
	}

	CombineService::CombineService(const CombineTableRepository& tables)
		: tables_(tables)
	{
	}

	std::optional<CombineRatePreview> CombineService::BuildRatePreview(const CombineRequest& request) const
	{
		std::uint8_t materialRank = 0;
		if (ValidateMaterials(request, materialRank) != CombineError::None)
			return std::nullopt;

		const CombineRewardGroup* group = SelectRewardGroup(request, materialRank);
		if (group == nullptr)
			return std::nullopt;

		CombineRatePreview preview;
		preview.materialRank = materialRank;
		preview.selectedGroupKey = group->groupKey;
		preview.usesCeilingPool = group->poolKind == RewardPoolKind::Ceiling;
		preview.rewards = group->rewards;
		return preview;
	}

	CombineResult CombineService::Roll(const CombineRequest& request, std::mt19937& random) const
	{
		std::uint8_t materialRank = 0;
		const CombineError validation = ValidateMaterials(request, materialRank);
		if (validation != CombineError::None)
			return { validation, CombineRewardEntry(), false };

		const CombineRewardGroup* group = SelectRewardGroup(request, materialRank);
		if (group == nullptr)
			return { CombineError::RewardGroupNotFound, CombineRewardEntry(), false };

		const std::uint32_t totalWeight = std::accumulate(
			group->rewards.begin(),
			group->rewards.end(),
			std::uint32_t(0),
			[](std::uint32_t total, const CombineRewardEntry& entry)
			{
				return total + entry.chanceBasisPoints;
			});

		if (totalWeight == 0)
			return { CombineError::RewardRateInvalid, CombineRewardEntry(), false };

		std::uniform_int_distribution<std::uint32_t> distribution(1, totalWeight);
		const std::uint32_t roll = distribution(random);

		std::uint32_t cumulative = 0;
		for (const CombineRewardEntry& entry : group->rewards)
		{
			cumulative += entry.chanceBasisPoints;
			if (roll <= cumulative)
				return { CombineError::None, entry, group->poolKind == RewardPoolKind::Ceiling };
		}

		return { CombineError::RewardRateInvalid, CombineRewardEntry(), false };
	}

	const CombineRewardGroup* CombineService::SelectRewardGroup(const CombineRequest& request, std::uint8_t materialRank) const
	{
		const CombineRankRule* rank = tables_.FindRank(materialRank);
		if (rank != nullptr && rank->ceilingFailCount > 0 && rank->ceilingGroupKey > 0 &&
			request.currentFailCount >= rank->ceilingFailCount)
		{
			const CombineRewardGroup* ceilingGroup = tables_.FindRewardGroup(rank->ceilingGroupKey, RewardPoolKind::Ceiling);
			if (ceilingGroup != nullptr)
				return ceilingGroup;
		}

		return tables_.FindRewardGroup(NormalGroupForRank(materialRank), RewardPoolKind::Normal);
	}

	CombineError CombineService::ValidateMaterials(const CombineRequest& request, std::uint8_t& materialRank) const
	{
		if (request.materials.size() != RequiredMaterialCount)
			return CombineError::WrongMaterialCount;

		bool rankAssigned = false;
		for (const CombineMaterialSlot& slot : request.materials)
		{
			const CombineItemDefinition* item = tables_.FindItem(slot.itemId);
			if (item == nullptr)
				return CombineError::UnknownMaterialItem;

			if (!rankAssigned)
			{
				materialRank = item->rankKey;
				rankAssigned = true;
				continue;
			}

			if (item->rankKey != materialRank)
				return CombineError::MixedMaterialRanks;
		}

		return CombineError::None;
	}

	const char* ToString(CombineError error)
	{
		switch (error)
		{
		case CombineError::None:
			return "None";
		case CombineError::WrongMaterialCount:
			return "WrongMaterialCount";
		case CombineError::UnknownMaterialItem:
			return "UnknownMaterialItem";
		case CombineError::MixedMaterialRanks:
			return "MixedMaterialRanks";
		case CombineError::RewardGroupNotFound:
			return "RewardGroupNotFound";
		case CombineError::RewardRateInvalid:
			return "RewardRateInvalid";
		default:
			return "Unknown";
		}
	}
}
