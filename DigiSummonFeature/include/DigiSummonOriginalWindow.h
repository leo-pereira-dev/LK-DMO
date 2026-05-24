#pragma once

#include <cstdint>

namespace DigiSummon::OriginalWindow
{
	enum class WindowMode
	{
		ProductSelect,
		TicketSelect,
		RateInfo,
		Result,
		Scene,
	};

	struct Rect
	{
		std::int32_t x = 0;
		std::int32_t y = 0;
		std::int32_t width = 0;
		std::int32_t height = 0;
	};

	struct DecompiledClassSize
	{
		static constexpr std::uint32_t MainUi = 0xD8;
		static constexpr std::uint32_t BottomUi = 0x94;
		static constexpr std::uint32_t SelectUi = 0x1E8;
		static constexpr std::uint32_t RateInfoUi = 0x80;
		static constexpr std::uint32_t ResultUi = 0x11C;
		static constexpr std::uint32_t SceneUi = 0xA8;
		static constexpr std::uint32_t TicketSelectUi = 0xAC;
	};

	struct MainUiOffsets
	{
		static constexpr std::uint32_t Contents = 0xA8;
		static constexpr std::uint32_t BottomUi = 0xAC;
		static constexpr std::uint32_t ActiveContent = 0xB0;
		static constexpr std::uint32_t SelectUi = 0xB4;
		static constexpr std::uint32_t RateInfoUi = 0xB8;
		static constexpr std::uint32_t ResultUi = 0xBC;
		static constexpr std::uint32_t SceneUi = 0xC0;
		static constexpr std::uint32_t TicketSelectUi = 0xC4;
	};

	struct SelectUiButtonRects
	{
		static constexpr Rect Prev = { 0x00F, 0x14B, 0x49, 0x2E };
		static constexpr Rect Next = { 0x3A8, 0x14B, 0x49, 0x2E };
	};

	struct BottomUiButtonRects
	{
		static constexpr Rect Buy = { 0x1C0, 0x24C, 0x80, 0x20 };
		static constexpr Rect RateInfo = { 0x1BE, 0x27E, 0x84, 0x26 };
	};

	namespace Asset
	{
		constexpr const char* WindowBackground = "Random_box\\bg.png";
		constexpr const char* ProductFrame = "Random_box\\over_frame.png";
		constexpr const char* PrevButton = "Random_box\\button\\arrow_l.tga";
		constexpr const char* NextButton = "Random_box\\button\\arrow_r.tga";
		constexpr const char* BuyButton = "Random_box\\button\\Purchase_1.tga";
		constexpr const char* RateInfoButton = "Cash\\NewCashShop\\Store_btn_G1.tga";
		constexpr const char* InfoButton = "Random_box\\button\\definite_information.tga";
		constexpr const char* TabButton = "Random_box\\button\\Tap.tga";
		constexpr const char* RankAtlas = "CommonUI\\digimon_rank_new.dds";
	}
}
