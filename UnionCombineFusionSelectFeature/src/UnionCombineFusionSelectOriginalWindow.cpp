#include "../include/UnionCombineFusionSelectOriginalWindow.h"

namespace union_combine_fusion_select_original {
namespace {

constexpr MainUiLayout kMainUiLayout{
    0x144,
    0xAC,
    0xB0,
    0xB4,
    0xB8,
    0xBC,
};

constexpr RegisterLayout kRegisterLayout{
    5,
    4,
    {0x280, 0x2C0, 0x8C, 0x28},
    {0x366, 0x247, 0x55, 0x1E},
    {0x0C2, 0x2BC, 0x78, 0x32},
    {0x280, 0x26C, 0x8C, 0x28},
    {0x32A, 0x26C, 0x8C, 0x28},
    {0x32A, 0x2BC, 0x96, 0x32},
};

} // namespace

std::string_view InternalWindowName()
{
    return "UnionCombineUI";
}

std::string_view MainClassName()
{
    return "CUnionCombineMainUI";
}

const MainUiLayout& GetMainUiLayout()
{
    return kMainUiLayout;
}

const RegisterLayout& GetRegisterLayout()
{
    return kRegisterLayout;
}

std::array<std::string_view, 8> RankFilterLabels()
{
    return {"Todos", "N", "A", "A+", "S", "S+", "SS", "SS+"};
}

std::array<std::string_view, 7> RankLabels()
{
    return {"N", "A", "A+", "S", "S+", "SS", "SS+"};
}

std::array<DecompiledFunctionRef, 28> DecompiledFunctions()
{
    return {{
        {"EnterUnionCombineSceneAndPlayBgm", 0x010A78D0, "decompiled/UnionCombineUI/EnterUnionCombineSceneAndPlayBgm.010a78d0.original.c", "Adjacent UnionCombine scene entry path that starts UnionCombine.mp3."},
        {"CreateUnionCombineUI", 0x01290160, "decompiled/UnionCombineUI/CreateUnionCombineUI.01290160.original.c", "Creates/registers the UnionCombineUI runtime window."},
        {"BuildUnionCombineMainUI", 0x011F9D20, "decompiled/UnionCombineUI/BuildUnionCombineMainUI.011f9d20.original.c", "Builds the root window and background."},
        {"BuildUnionCombineContents", 0x011FB410, "decompiled/UnionCombineUI/BuildUnionCombineContents.011fb410.original.c", "Builds the main content container."},
        {"BuildUnionCombineRegisterUI", 0x011FBCD0, "decompiled/UnionCombineUI/BuildUnionCombineRegisterUI.011fbcd0.original.c", "Builds the register/content UI."},
        {"BindUnionCombineRegisterEvents", 0x011FC500, "decompiled/UnionCombineUI/BindUnionCombineRegisterEvents.011fc500.original.c", "Binds register UI callbacks and text keys."},
        {"UpdateUnionCombineRegisterState", 0x011FCED0, "decompiled/UnionCombineUI/UpdateUnionCombineRegisterState.011fced0.original.c", "Updates visible register selection state."},
        {"BuildUnionCombineMaterialRows", 0x011FD5F0, "decompiled/UnionCombineUI/BuildUnionCombineMaterialRows.011fd5f0.original.c", "Creates the five rows with four material slots, arrow, and card result."},
        {"BuildUnionCombineRegisterButtons", 0x011FDD70, "decompiled/UnionCombineUI/BuildUnionCombineRegisterButtons.011fdd70.original.c", "Creates Registro, Fusao, Cancelar registro, Saida, info, and reward buttons."},
        {"BuildUnionCombineAvailableItemGrid", 0x011FEA60, "decompiled/UnionCombineUI/BuildUnionCombineAvailableItemGrid.011fea60.original.c", "Creates the right panel, rank filters, item grid, and scrollbar."},
        {"BuildUnionCombineRateInfoUI", 0x011FF560, "decompiled/UnionCombineUI/BuildUnionCombineRateInfoUI.011ff560.original.c", "Builds probability/reward info popup."},
        {"UpdateUnionCombineSceneProgress", 0x01200EA0, "decompiled/UnionCombineUI/UpdateUnionCombineSceneProgress.01200ea0.original.c", "Updates gacha/scene progress and sound."},
        {"BuildUnionCombineSceneUI", 0x012012E0, "decompiled/UnionCombineUI/BuildUnionCombineSceneUI.012012e0.original.c", "Builds card animation scene using DigiCombine scene assets."},
        {"BindUnionCombineSceneEvents", 0x012022E0, "decompiled/UnionCombineUI/BindUnionCombineSceneEvents.012022e0.original.c", "Binds scene/card events."},
        {"SetUnionCombineSceneRank", 0x01202490, "decompiled/UnionCombineUI/SetUnionCombineSceneRank.01202490.original.c", "Sets scene rank state."},
        {"StartUnionCombineSceneResult", 0x012025C0, "decompiled/UnionCombineUI/StartUnionCombineSceneResult.012025c0.original.c", "Starts final result transition."},
        {"SelectUnionCombineResultCardByRank", 0x01202700, "decompiled/UnionCombineUI/SelectUnionCombineResultCardByRank.01202700.original.c", "Selects rank-specific final card texture."},
        {"SelectUnionCombineSceneBackgroundByRank", 0x01202880, "decompiled/UnionCombineUI/SelectUnionCombineSceneBackgroundByRank.01202880.original.c", "Selects rank-specific scene background texture."},
        {"LoadUnionCombineTables", 0x0164D930, "decompiled/UnionCombineTables/LoadUnionCombineTables.0164d930.original.c", "Loads DM_UnionCombine table family."},
        {"ParseUnionCombineItemListCallback", 0x0164DB10, "decompiled/UnionCombineTables/ParseUnionCombineItemListCallback.0164db10.original.c", "Parses ItemList XML records."},
        {"ParseUnionCombineRankListCallback", 0x0164DB30, "decompiled/UnionCombineTables/ParseUnionCombineRankListCallback.0164db30.original.c", "Parses RankList XML records."},
        {"ParseUnionCombineItemGroupCallback", 0x0164DB70, "decompiled/UnionCombineTables/ParseUnionCombineItemGroupCallback.0164db70.original.c", "Parses ItemGroup XML records."},
        {"ParseUnionCombineCeilGroupCallback", 0x0164DB50, "decompiled/UnionCombineTables/ParseUnionCombineCeilGroupCallback.0164db50.original.c", "Parses CeilGroup XML records."},
        {"RegisterUnionCombineBinParser", 0x0164C930, "decompiled/UnionCombineTables/RegisterUnionCombineBinParser.0164c930.original.c", "Registers UnionCombine bin readers."},
        {"ReadUnionCombineItemList", 0x0164D0B0, "decompiled/UnionCombineTables/ReadUnionCombineItemList.0164d0b0.original.c", "Reads ItemList bin records."},
        {"ReadUnionCombineRankList", 0x0164CC40, "decompiled/UnionCombineTables/ReadUnionCombineRankList.0164cc40.original.c", "Reads RankList bin records."},
        {"ReadUnionCombineRewardGroups", 0x0164CE60, "decompiled/UnionCombineTables/ReadUnionCombineRewardGroups.0164ce60.original.c", "Reads ItemGroup reward records."},
        {"ReadUnionCombineRewardEntries", 0x0164D340, "decompiled/UnionCombineTables/ReadUnionCombineRewardEntries.0164d340.original.c", "Reads CeilGroup reward records."},
    }};
}

std::array<TableRef, 9> RequiredTables()
{
    return {{
        {"DM_UnionCombine-ItemList", "tables/xml/DM_UnionCombine-ItemList.xml", "tables/bin/DM_UnionCombine-ItemList.bin", "Allowed source items for union combine fusion."},
        {"DM_UnionCombine-RankList", "tables/xml/DM_UnionCombine-RankList.xml", "tables/bin/DM_UnionCombine-RankList.bin", "Seven visible rank filters, N through SS+."},
        {"DM_UnionCombine-ItemGroup", "tables/xml/DM_UnionCombine-ItemGroup.xml", "tables/bin/DM_UnionCombine-ItemGroup.bin", "Reward group mapping."},
        {"DM_UnionCombine-CeilGroup", "tables/xml/DM_UnionCombine-CeilGroup.xml", "tables/bin/DM_UnionCombine-CeilGroup.bin", "Ceiling/guarantee reward group mapping."},
        {"ItemData", "tables/xml/ItemData.xml", "tables/bin/ItemData.bin", "Item icon/name/category lookup."},
        {"ItemType", "tables/xml/ItemType.xml", "tables/bin/ItemType.bin", "Item type lookup."},
        {"ItemRankData", "tables/xml/ItemRankData.xml", "tables/bin/ItemRankData.bin", "Rank visual/effect lookup."},
        {"ItemRankEffectData", "tables/xml/ItemRankEffectData.xml", "tables/bin/ItemRankEffectData.bin", "Rank effect lookup."},
        {"QuestItemData", "tables/xml/QuestItemData.xml", "tables/bin/QuestItemData.bin", "Quest/material item fallback lookup."},
    }};
}

std::array<RuntimeAssetRef, 20> VisibleCoreAssets()
{
    return {{
        {"Random_box/bg.png", "BuildUnionCombineMainUI", "Window/background panel."},
        {"Random_box/button/done.tga", "BuildUnionCombineRegisterButtons", "Registro / green confirm-style button source."},
        {"Random_box/button/Purchase_1.tga", "BuildUnionCombineRegisterButtons", "Fusao / blue action button source."},
        {"Random_box/button/Repurchase.tga", "BuildUnionCombineRegisterButtons", "Cancelar registro / red action button source."},
        {"Cash/NewCashShop/Store_btn_G1.tga", "BuildUnionCombineRegisterButtons", "Saida gray button source."},
        {"Unionmon_Ev_UI/Ev_Btn.tga", "BuildUnionCombineRegisterButtons", "Rank/category button source."},
        {"CommonUI/arrow_Right.tga", "UpdateUnionCombineRegisterState", "Blue arrow between slots and result card."},
        {"CommonUI/Digimon_rank_new.dds", "BuildUnionCombineMaterialRows", "New rank sprite sheet used by this UnionCombine variant."},
        {"CommonUI/Unionmon_rank.dds", "BuildUnionCombineAvailableItemGrid", "Union rank marks used in available-item grid."},
        {"CommonUI/Slot/Windows_Scroll2.tga", "BuildUnionCombineAvailableItemGrid", "Right panel scrollbar."},
        {"Control_G/Popup/N2Dlg5.tga", "BuildUnionCombineAvailableItemGrid", "Right panel popup/list frame texture."},
        {"DigiCombine/DigiCombine_Slot_Empty.dds", "BuildUnionCombineMaterialRows", "Empty plus slot texture."},
        {"DigiCombine/DigiCombine_Slot_rank.dds", "BuildUnionCombineMaterialRows", "Slot rank overlay texture."},
        {"Random_box/gauge.png", "BuildUnionCombineAvailableItemGrid", "Gauge texture referenced by the available-item panel path."},
        {"DigiCombine/DigiCombine_CardBack.dds", "BindUnionCombineSceneEvents", "Scene/card animation back texture."},
        {"DigiCombine/DigiCombine_Gacha_Start.dds", "BuildUnionCombineSceneUI", "Scene start effect."},
        {"DigiCombine/DigiCombine_Gacha_Change.dds", "BuildUnionCombineSceneUI", "Scene change effect."},
        {"DigiCombine/DigiCombine_Gacha_Bg_N.dds", "BuildUnionCombineSceneUI", "Default N rank scene background."},
        {"SealMaster/sealmaster_highlight.tga", "BuildUnionCombineAvailableItemGrid", "Selection/highlight sprite used by grid."},
        {"System/DigiCombine_Gacha_Progress.wav", "UpdateUnionCombineSceneProgress", "Gacha progress sound."},
    }};
}

} // namespace union_combine_fusion_select_original
