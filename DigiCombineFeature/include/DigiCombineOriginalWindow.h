#pragma once

#include "DigiCombineTypes.h"

#include <array>
#include <cstdint>

namespace DigiCombine::OriginalWindow
{
	enum class WindowMode
	{
		Register,
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
		static constexpr std::uint32_t MainUi = 0x144;
		static constexpr std::uint32_t RegisterUi = 0x140;
		static constexpr std::uint32_t RateInfoUi = 0x80;
		static constexpr std::uint32_t ResultUi = 0x114;
		static constexpr std::uint32_t SceneUi = 0x88;
	};

	struct MainUiOffsets
	{
		static constexpr std::uint32_t ActiveContent = 0xAC;
		static constexpr std::uint32_t RegisterUi = 0xB0;
		static constexpr std::uint32_t RateInfoUi = 0xB4;
		static constexpr std::uint32_t ResultUi = 0xB8;
		static constexpr std::uint32_t SceneUi = 0xBC;
	};

	struct RegisterUiButtonRects
	{
		static constexpr Rect AddCancel = { 0x280, 0x2C0, 0x8C, 0x28 };
		static constexpr Rect Reward = { 0x366, 0x247, 0x55, 0x1E };
		static constexpr Rect RateInfo = { 0x0C2, 0x2BC, 0x78, 0x32 };
		static constexpr Rect AutoRegister = { 0x280, 0x26C, 0x8C, 0x28 };
		static constexpr Rect Combine = { 0x32A, 0x26C, 0x8C, 0x28 };
		static constexpr Rect Exit = { 0x32A, 0x2BC, 0x96, 0x32 };
	};

	struct RegisterUiOffsets
	{
		static constexpr std::uint32_t AutoRegisterButton = 0xA4;
		static constexpr std::uint32_t AddCancelButton = 0xA8;
		static constexpr std::uint32_t RateInfoButton = 0xAC;
		static constexpr std::uint32_t CombineButton = 0xB0;
		static constexpr std::uint32_t RewardButton = 0xB4;
		static constexpr std::uint32_t ExitButton = 0xB8;
	};

	namespace Asset
	{
		constexpr const char* WindowBackground = "Random_box_bg.png";
		constexpr const char* DigiRewardButton = "Digimon_Ev_UI_Ev_Btn.tga";
		constexpr const char* UnionRewardButton = "Unionmon_Ev_UI_Ev_Btn.tga";
		constexpr const char* AddCancelButton = "Random_box_button_Repurchase.tga";
		constexpr const char* AutoRegisterButton = "Random_box_button_Purchase_1.tga";
		constexpr const char* CombineButton = "Random_box_button_done.tga";
		constexpr const char* RateInfoButton = "Cash_NewCashShop_Store_btn_G1.tga";

		constexpr const char* CardFront = "DigiCombine\\DigiCombine_CardFront.dds";
		constexpr const char* CardBack = "DigiCombine\\DigiCombine_CardBack.dds";
		constexpr const char* EmptySlot = "DigiCombine\\DigiCombine_Slot_Empty.dds";
		constexpr const char* SlotRank = "DigiCombine\\DigiCombine_Slot_rank.dds";
		constexpr const char* GachaStart = "DigiCombine\\DigiCombine_Gacha_Start.dds";
		constexpr const char* GachaChange = "DigiCombine\\DigiCombine_Gacha_Change.dds";

		constexpr const char* GachaProgressSound = "System\\DigiCombine_Gacha_Progress.wav";
		constexpr const char* GachaRareSound = "System\\DigiCombine_Gacha_Rare.wav";
		constexpr const char* GachaChangeSound = "System\\DigiCombine_Gacha_Change.wav";
		constexpr const char* GachaDefaultSound = "System\\DigiCombine_Gacha_Default.wav";
	}

	struct GachaRankAssetSet
	{
		const char* endTexture;
		const char* backgroundTexture;
	};

	constexpr std::array<GachaRankAssetSet, 11> GachaRankAssets =
	{{
		{ "DigiCombine\\DigiCombine_Gacha_End_A.dds", "DigiCombine\\DigiCombine_Gacha_Bg_A.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_Aplus.dds", "DigiCombine\\DigiCombine_Gacha_Bg_Aplus.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_S.dds", "DigiCombine\\DigiCombine_Gacha_Bg_S.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_Splus.dds", "DigiCombine\\DigiCombine_Gacha_Bg_Splus.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_SS.dds", "DigiCombine\\DigiCombine_Gacha_Bg_SS.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_SSplus.dds", "DigiCombine\\DigiCombine_Gacha_Bg_SSplus.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_SSS.dds", "DigiCombine\\DigiCombine_Gacha_Bg_SSS.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_SSSplus.dds", "DigiCombine\\DigiCombine_Gacha_Bg_SSSplus.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_U.dds", "DigiCombine\\DigiCombine_Gacha_Bg_U.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_Uplus.dds", "DigiCombine\\DigiCombine_Gacha_Bg_Uplus.dds" },
		{ "DigiCombine\\DigiCombine_Gacha_End_N.dds", "DigiCombine\\DigiCombine_Gacha_Bg_N.dds" },
	}};

	class MainUi
	{
	public:
		explicit MainUi(CombineFamily family);

		CombineFamily Family() const;
		WindowMode Mode() const;
		void SetMode(WindowMode mode);

	private:
		CombineFamily family_;
		WindowMode mode_ = WindowMode::Register;
	};
}
