#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace DigiSummon
{
	struct ProductEntry
	{
		std::uint32_t itemOrKey = 0;
		std::uint32_t type = 0;
		std::uint32_t value = 0;
	};

	struct Product
	{
		std::uint32_t summonKey = 0;
		std::uint8_t rankOrGrade = 0;
		std::uint8_t flag = 0;
		std::uint16_t unknownA = 0;
		std::uint16_t unknownB = 0;
		std::uint32_t orderOrWeight = 0;
		std::string iconPath;
		std::vector<ProductEntry> entries;
	};

	struct TicketItem
	{
		std::uint32_t itemId = 0;
		std::uint8_t flagOrType = 0;
		std::uint32_t value = 0;
	};

	struct RewardEntry
	{
		std::uint32_t itemId = 0;
		std::uint32_t count = 0;
		std::uint32_t chanceBasisPoints = 0;
	};

	struct RewardGroup
	{
		std::uint32_t groupId = 0;
		std::vector<RewardEntry> rewards;
	};
}
