#include "../include/DigitalDrawOriginalWindow.h"

namespace digital_draw_original {
namespace {

constexpr MainUiLayout kMainUiLayout{
    0xD8,
    0xA8,
    0xAC,
    0xB0,
    0xB4,
    0xB8,
    0xBC,
    0xC0,
    0xC4,
};

} // namespace

std::string_view ReusedRuntimeWindowName()
{
    return "DigiSummonUI";
}

std::string_view ExclusiveTableName()
{
    return "CLuckyDrawTable";
}

std::string_view MissingOfficialTableBin()
{
    return "DM_Luckydraw_Info-LuckyDrawInfo.bin";
}

const MainUiLayout& GetMainUiLayout()
{
    return kMainUiLayout;
}

std::array<ClassSize, 7> ReusedClassSizes()
{
    return {{
        {"CDigiSummonMainUI", 0xD8},
        {"CDigiSummonBottomUI", 0x94},
        {"CDigiSummonSelectUI", 0x1E8},
        {"CDigiSummonRateInfoUI", 0x80},
        {"CDigiSummonResultUI", 0x11C},
        {"CDigiSummonSceneUI", 0xA8},
        {"CDigiSummonTicketSelectUI", 0xAC},
    }};
}

std::array<DecompiledFunctionRef, 19> KeyFunctions()
{
    return {{
        {"CreateDigiSummonUiFactoryForDigitalDraw", 0x0128F070, "decompiled/DigitalDrawUI/CreateDigiSummonUiFactoryForDigitalDraw.0128f070.original.c", "Creates the reused DigiSummonUI runtime window."},
        {"BuildDigitalDrawMainUi", 0x011B6020, "decompiled/DigitalDrawUI/BuildDigitalDrawMainUi.011b6020.original.c", "Builds the Random_box root window and child UIs."},
        {"BuildDigitalDrawTicketCarousel", 0x011BB4B0, "decompiled/DigitalDrawUI/BuildDigitalDrawTicketCarousel.011bb4b0.original.c", "Builds the ticket/card carousel and arrow buttons."},
        {"BuildDigitalDrawTicketCard", 0x011BBB20, "decompiled/DigitalDrawUI/BuildDigitalDrawTicketCard.011bbb20.original.c", "Builds one ticket/card tile."},
        {"BuildDigitalDrawBottomActions", 0x011BC4E0, "decompiled/DigitalDrawUI/BuildDigitalDrawBottomActions.011bc4e0.original.c", "Builds purchase/rate-info bottom actions."},
        {"BuildDigitalDrawRateInfoPopup", 0x011B71A0, "decompiled/DigitalDrawUI/BuildDigitalDrawRateInfoPopup.011b71a0.original.c", "Builds probability/reward popup."},
        {"HandleDigitalDrawRateInfoPacket", 0x011B6D30, "decompiled/DigitalDrawUI/HandleDigitalDrawRateInfoPacket.011b6d30.original.c", "Parses rate-info payload and forwards rows to the popup."},
        {"BuildDigitalDrawRateInfoWindow", 0x011B6E50, "decompiled/DigitalDrawUI/BuildDigitalDrawRateInfoWindow.011b6e50.original.c", "Builds the info. de prob. popup shell, title, close button, and list container."},
        {"BuildDigitalDrawRateInfoScrollList", 0x011B7390, "decompiled/DigitalDrawUI/BuildDigitalDrawRateInfoScrollList.011b7390.original.c", "Builds Rate_bg_5x5 list body and scrollbar."},
        {"BuildDigitalDrawRateInfoRewardRow", 0x011B7530, "decompiled/DigitalDrawUI/BuildDigitalDrawRateInfoRewardRow.011b7530.original.c", "Builds each probability row with icon, text, rank and percent."},
        {"PopulateDigitalDrawRateInfoRows", 0x011B7BB0, "decompiled/DigitalDrawUI/PopulateDigitalDrawRateInfoRows.011b7bb0.original.c", "Populates the probability rows into the rate-info list."},
        {"BuildDigitalDrawSceneUi", 0x011B8E70, "decompiled/DigitalDrawUI/BuildDigitalDrawSceneUi.011b8e70.original.c", "Builds draw/gacha scene animation."},
        {"SelectDigitalDrawResultCardByRank", 0x011BAB00, "decompiled/DigitalDrawUI/SelectDigitalDrawResultCardByRank.011bab00.original.c", "Selects result texture by rank."},
        {"SelectDigitalDrawSceneBackgroundByRank", 0x011BAC80, "decompiled/DigitalDrawUI/SelectDigitalDrawSceneBackgroundByRank.011bac80.original.c", "Selects scene background by rank."},
        {"RegisterLuckyDrawInfoTable", 0x016828F0, "decompiled/LuckyDrawTable/RegisterLuckyDrawInfoTable.016828f0.original.c", "Registers DM_Luckydraw_Info-LuckyDrawInfo.bin."},
        {"ParseLuckyDrawInfoTableCallback", 0x01682AB0, "decompiled/LuckyDrawTable/ParseLuckyDrawInfoTableCallback.01682ab0.original.c", "LuckyDraw table event callback."},
        {"ReadLuckyDrawInfoRecords", 0x01682430, "decompiled/LuckyDrawTable/ReadLuckyDrawInfoRecords.01682430.original.c", "Reads LuckyDrawInfo records."},
        {"ReadLuckyDrawInfoRecordFields", 0x01682AD0, "decompiled/LuckyDrawTable/ReadLuckyDrawInfoRecordFields.01682ad0.original.c", "Reads one LuckyDrawInfo record."},
        {"DestroyLuckyDrawInfoTable", 0x01682880, "decompiled/LuckyDrawTable/DestroyLuckyDrawInfoTable.01682880.original.c", "CLuckyDrawTable destructor path."},
    }};
}

std::array<RuntimeAssetRef, 17> VisibleCoreAssets()
{
    return {{
        {"Random_box/bg.png", "BuildDigitalDrawMainUi", "Main background used by the ticket selection window."},
        {"Random_box/bg_big.png", "BuildDigitalDrawTicketCard", "Large card/ticket panel background."},
        {"Random_box/over_frame.png", "BuildDigitalDrawTicketCarousel", "Card hover/selection overlay frame."},
        {"Random_box/button/arrow_l.tga", "BuildDigitalDrawTicketCarousel", "Left carousel arrow."},
        {"Random_box/button/arrow_r.tga", "BuildDigitalDrawTicketCarousel", "Right carousel arrow."},
        {"Random_box/button/definite_information.tga", "BuildDigitalDrawTicketCard", "Small probability/info button on card."},
        {"Random_box/button/Purchase_1.tga", "BuildDigitalDrawBottomActions", "Primary one-draw/purchase button."},
        {"Random_box/button/Tap.tga", "BuildDigitalDrawRateInfoPopup", "Rate popup tab button."},
        {"Random_box/popup/rate_popup/rate_popup_", "BuildDigitalDrawRateInfoWindow", "Rate-info popup frame prefix used by the official UI."},
        {"Random_box/Rate_bg_5x5.png", "BuildDigitalDrawRateInfoScrollList", "Probability list background."},
        {"Random_box/frame.png", "BuildDigitalDrawRateInfoRewardRow", "Reward row item frame."},
        {"CommonUI/CommonUI_close.tga", "BuildDigitalDrawRateInfoWindow", "Popup close button."},
        {"CommonUI/digimon_rank_new.dds", "BuildDigitalDrawRateInfoRewardRow", "Rank icon sheet for probability rows."},
        {"Random_box/sub_title_line.png", "BuildDigitalDrawTitleLines", "Gold title separator line."},
        {"Random_box/loding.png", "BuildDigitalDrawTicketCard", "Card loading placeholder."},
        {"DigiCombine/DigiCombine_Gacha_Start.dds", "BuildDigitalDrawSceneUi", "Draw scene start effect reused from combine gacha."},
        {"System/DigiCombine_Gacha_Progress.wav", "BuildDigitalDrawSceneUi", "Draw scene progress sound."},
    }};
}

} // namespace digital_draw_original
