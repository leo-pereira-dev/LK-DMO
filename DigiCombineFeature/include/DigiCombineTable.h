#pragma once

#include "DigiCombineTypes.h"

#include <filesystem>
#include <optional>

namespace DigiCombine
{
	class CombineTableRepository
	{
	public:
		bool LoadFromXmlDirectory(CombineFamily family, const std::filesystem::path& directory);

		const CombineTableSet& Tables() const;
		const CombineItemDefinition* FindItem(std::uint32_t itemId) const;
		const CombineRankRule* FindRank(std::uint8_t rankKey) const;
		const CombineRewardGroup* FindRewardGroup(std::uint16_t groupKey, RewardPoolKind poolKind) const;

	private:
		bool LoadItemList(const std::filesystem::path& path);
		bool LoadRankList(const std::filesystem::path& path);
		bool LoadRewardGroups(const std::filesystem::path& path, RewardPoolKind poolKind);

		CombineTableSet tables_;
	};

	std::filesystem::path MakeTableFileName(CombineFamily family, const char* tableName);
}
