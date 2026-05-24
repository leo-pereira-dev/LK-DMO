#pragma once

#include "DigiCombineTable.h"

#include <optional>
#include <random>
#include <string>

namespace DigiCombine
{
	enum class CombineError
	{
		None,
		WrongMaterialCount,
		UnknownMaterialItem,
		MixedMaterialRanks,
		RewardGroupNotFound,
		RewardRateInvalid,
	};

	struct CombineResult
	{
		CombineError error = CombineError::None;
		CombineRewardEntry reward;
		bool consumedCeiling = false;
	};

	class CombineService
	{
	public:
		explicit CombineService(const CombineTableRepository& tables);

		std::optional<CombineRatePreview> BuildRatePreview(const CombineRequest& request) const;
		CombineResult Roll(const CombineRequest& request, std::mt19937& random) const;

	private:
		const CombineRewardGroup* SelectRewardGroup(const CombineRequest& request, std::uint8_t materialRank) const;
		CombineError ValidateMaterials(const CombineRequest& request, std::uint8_t& materialRank) const;

		const CombineTableRepository& tables_;
	};

	const char* ToString(CombineError error);
}
