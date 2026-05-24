#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace dungeon_enter
{
struct DecompiledFunction
{
    std::uint32_t address;
    std::string_view originalName;
    std::string_view mappedName;
    std::string_view copiedFile;
};

struct UiAssetReference
{
    std::uint32_t address;
    std::string_view pathOrTextKey;
    std::string_view sourceFunction;
};

struct WindowLayout
{
    int width;
    int height;
    int closeX;
    int closeY;
    int enterX;
    int enterY;
    int enterWidth;
    int enterHeight;
};

inline constexpr WindowLayout kOfficialWindowLayout{
    900, 715,
    862, 36,
    720, 650,
    166, 50,
};

inline constexpr std::array<DecompiledFunction, 17> kDungeonEnterUiFunctions{{
    {0x0128cd60, "FUN_0128cd60", "CreateDungeonEnterWindow", "CreateDungeonEnterWindow.original.c"},
    {0x010c8fa0, "FUN_010c8fa0", "BuildDungeonEnterRoot", "BuildDungeonEnterRoot.original.c"},
    {0x010c93b0, "FUN_010c93b0", "BuildDungeonListPanel", "BuildDungeonListPanel.original.c"},
    {0x010c9870, "FUN_010c9870", "BuildDungeonListButton", "BuildDungeonListButton.original.c"},
    {0x010ca260, "FUN_010ca260", "BuildRewardPanel", "BuildRewardPanel.original.c"},
    {0x010ca670, "FUN_010ca670", "BuildRewardSlot", "BuildRewardSlot.original.c"},
    {0x010ca940, "FUN_010ca940", "BuildParticipationPanel", "BuildParticipationPanel.original.c"},
    {0x010cad70, "FUN_010cad70", "RenderEmptyRequirementText", "RenderEmptyRequirementText.original.c"},
    {0x010cb260, "FUN_010cb260", "BuildRequirementTabs", "BuildRequirementTabs.original.c"},
    {0x010cb8e0, "FUN_010cb8e0", "RenderRequirementRow", "RenderRequirementRow.original.c"},
    {0x010cbd20, "FUN_010cbd20", "BuildDungeonPreviewPanel", "BuildDungeonPreviewPanel.original.c"},
    {0x010ccc80, "FUN_010ccc80", "ResolveDungeonDifficultyBadge", "ResolveDungeonDifficultyBadge.original.c"},
    {0x010cd1d0, "FUN_010cd1d0", "BuildClearRewardView", "BuildClearRewardView.original.c"},
    {0x010cd810, "FUN_010cd810", "BuildClearRewardSmallList", "BuildClearRewardSmallList.original.c"},
    {0x010cdc00, "FUN_010cdc00", "BuildClearRewardListButton", "BuildClearRewardListButton.original.c"},
    {0x010ce150, "FUN_010ce150", "BuildClearRewardLargeList", "BuildClearRewardLargeList.original.c"},
    {0x010ce810, "FUN_010ce810", "RefreshClearRewardLargeList", "RefreshClearRewardLargeList.original.c"},
}};

inline constexpr std::array<UiAssetReference, 22> kCoreWindowAssets{{
    {0x01ad49f0, "DungeonUI\\Dungeon_Bg.tga", "BuildDungeonEnterRoot"},
    {0x01ad3144, "CommonUI\\CommonUI_close.tga", "BuildDungeonEnterRoot"},
    {0x01ad438c, "DungeonUI\\Dungeon_Entrance_Btn.tga", "BuildDungeonEnterRoot"},
    {0x01ad4a24, "DungeonUI\\Dungeon_List_Frame.tga", "BuildDungeonListPanel"},
    {0x01ad4a48, "DungeonUI\\Dungeon_Line.tga", "BuildDungeonListPanel"},
    {0x01ad4a8c, "DungeonUI\\Dungeon_List_ClickBtn_Stroke.tga", "BuildDungeonListPanel"},
    {0x01ad4ab8, "DungeonUI\\Dungeon_List_Btn_Image.tga", "BuildDungeonListButton"},
    {0x01ad4ae0, "DungeonUI\\Dungeon_List_Btn.tga", "BuildDungeonListButton"},
    {0x01ad4b00, "DungeonUI\\Dungeon_Reward_Frame.tga", "BuildRewardPanel"},
    {0x01ad4b60, "DungeonUI\\Dungeon_Slot.tga", "BuildRewardSlot"},
    {0x01ad4b7c, "DungeonUI\\Dungeon_Target_Participation_Frame.tga", "BuildParticipationPanel"},
    {0x01ad4bb0, "DungeonUI\\Dungeon_Target_Participation_Unsatisfactory_Frame.tga", "BuildParticipationPanel"},
    {0x01ad4bf0, "DungeonUI\\Dungeon_User_Icon.tga", "BuildParticipationPanel"},
    {0x01ad4d48, "DungeonUI\\Dungeon_Image_Frame.tga", "BuildDungeonPreviewPanel"},
    {0x01ad4d6c, "DungeonUI\\Dungeon_name_Frame.tga", "BuildDungeonPreviewPanel"},
    {0x01ad4d90, "DungeonUI\\Dungeon_Level_Easy.tga", "ResolveDungeonDifficultyBadge"},
    {0x01ad4e54, "DungeonUI\\Dungeon_Level_Normal.tga", "ResolveDungeonDifficultyBadge"},
    {0x01ad4e78, "DungeonUI\\Dungeon_Level_Hard.tga", "ResolveDungeonDifficultyBadge"},
    {0x01ad4db4, "DungeonUI\\Dungeon_reward_btn.png", "BuildDungeonPreviewPanel"},
    {0x01ad4f84, "DungeonUI\\Dungeon_frame_new.png", "BuildClearRewardView"},
    {0x01ad501c, "DungeonUI\\Dungeon_list_S.png", "BuildClearRewardSmallList"},
    {0x01ad505c, "DungeonUI\\Dungeon_list_L.png", "BuildClearRewardLargeList"},
}};
} // namespace dungeon_enter
