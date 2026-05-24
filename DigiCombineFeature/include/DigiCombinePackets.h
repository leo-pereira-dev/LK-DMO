#pragma once

#include "DigiCombineTypes.h"

#include <cstdint>
#include <vector>

namespace DigiCombine
{
	namespace ClientPacketName
	{
		constexpr const char* RequestDigiCombineSync = "C2GS_REQ_DIGI_COMBINE_SYNC";
		constexpr const char* RequestDigiCombine = "C2GS_REQ_DIGI_COMBINE";
		constexpr const char* RequestDigiCombineReward = "C2GS_REQ_DIGI_COMBINE_REWARD";
		constexpr const char* RequestUnionCombineSync = "C2GS_REQ_UNION_COMBINE_SYNC";
		constexpr const char* RequestUnionCombine = "C2GS_REQ_UNION_COMBINE";
		constexpr const char* RequestUnionCombineReward = "C2GS_REQ_UNION_COMBINE_REWARD";
	}

	namespace ServerPacketName
	{
		constexpr const char* ResponseDigiCombineSync = "GS2C_RES_DIGI_COMBINE_SYNC";
		constexpr const char* ResponseDigiCombine = "GS2C_RES_DIGI_COMBINE";
		constexpr const char* ResponseDigiCombineReward = "GS2C_RES_DIGI_COMBINE_REWARD";
		constexpr const char* ResponseUnionCombine = "GS2C_RES_UNION_COMBINE";
		constexpr const char* ResponseUnionCombineReward = "GS2C_RES_UNION_COMBINE_REWARD";
	}

	struct CombinePacketSlot
	{
		std::uint16_t inventorySlot = 0;
		std::uint32_t itemId = 0;
		std::uint16_t amount = 1;
	};

	struct ClientCombineRequestPacket
	{
		CombineFamily family = CombineFamily::Digi;
		std::vector<CombinePacketSlot> materialSlots;
	};

	struct ServerCombineResultPacket
	{
		CombineFamily family = CombineFamily::Digi;
		std::uint8_t resultCode = 0;
		std::uint32_t rewardItemId = 0;
		std::uint16_t rewardCount = 0;
		std::uint8_t resultRank = 0;
	};
}
