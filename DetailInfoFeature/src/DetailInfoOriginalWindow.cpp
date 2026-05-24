#include "../include/DetailInfoOriginalWindow.h"

namespace detail_info_feature {

const std::array<OriginalFunctionRef, 31>& GetOriginalFunctionMap()
{
    static constexpr std::array<OriginalFunctionRef, 31> functions = {{
        {"0128bab0", "FUN_0128bab0", "CreateTamerStatusUI", "decompiled/StatusUI/CreateTamerStatusUI.original.c", "Creates and registers TamerStatusUI."},
        {"0128bcc0", "FUN_0128bcc0", "CreateDigimonStatusUI", "decompiled/StatusUI/CreateDigimonStatusUI.original.c", "Creates and registers DigimonStatusUI."},
        {"0128bd60", "FUN_0128bd60", "CreateDigimonStatusHelpUI", "decompiled/StatusUI/CreateDigimonStatusHelpUI.original.c", "Creates DigimonStatusUI_Help."},
        {"011d12b0", "FUN_011d12b0", "BuildAdditionalStatPanel", "decompiled/DetailInfoUI/BuildAdditionalStatPanel.original.c", "Builds the additional/attribute stat popup panel."},
        {"011d2a20", "FUN_011d2a20", "BuildDigimonDetailRoot", "decompiled/DetailInfoUI/BuildDigimonDetailRoot.original.c", "Builds Digimon tab root background and subpanels."},
        {"011d2b30", "FUN_011d2b30", "BuildDigimonInfoPanel", "decompiled/DetailInfoUI/BuildDigimonInfoPanel.original.c", "Builds Digimon portrait/name/level area."},
        {"011d30b0", "FUN_011d30b0", "BuildDigimonInfoBox", "decompiled/DetailInfoUI/BuildDigimonInfoBox.original.c", "Builds the Digimon information text box."},
        {"011d3690", "FUN_011d3690", "RenderDigimonDataSection", "decompiled/DetailInfoUI/RenderDigimonDataSection.original.c", "Renders Digimon data section labels."},
        {"011d3b00", "FUN_011d3b00", "BuildDigimonSkillSection", "decompiled/DetailInfoUI/BuildDigimonSkillSection.original.c", "Builds the Digimon skill slots."},
        {"011d4320", "FUN_011d4320", "BuildDigimonEnchantPanel", "decompiled/DetailInfoUI/BuildDigimonEnchantPanel.original.c", "Builds enchant stat rows and icons."},
        {"011d4bd0", "FUN_011d4bd0", "BuildDigimonAdvancePanel", "decompiled/DetailInfoUI/BuildDigimonAdvancePanel.original.c", "Builds Digimon breakthrough/advance area."},
        {"011d50f0", "FUN_011d50f0", "FinalizeDigimonDetailPanel", "decompiled/DetailInfoUI/FinalizeDigimonDetailPanel.original.c", "Finishes Digimon tab button/event wiring."},
        {"011d61d0", "FUN_011d61d0", "BuildEquipmentPanel", "decompiled/DetailInfoUI/BuildEquipmentPanel.original.c", "Builds equipment detail popup shell."},
        {"011d65d0", "FUN_011d65d0", "BuildEquipmentDetailList", "decompiled/DetailInfoUI/BuildEquipmentDetailList.original.c", "Builds equipment stat/detail list."},
        {"011d82a0", "FUN_011d82a0", "BuildVisibilityToggleButton", "decompiled/DetailInfoUI/BuildVisibilityToggleButton.original.c", "Selects Autocrop ON/OFF button sprite."},
        {"011da000", "FUN_011da000", "BuildSealMasterPanel", "decompiled/DetailInfoUI/BuildSealMasterPanel.original.c", "Builds Seal Master panel shell."},
        {"011da100", "FUN_011da100", "BuildSealMasterCloseButton", "decompiled/DetailInfoUI/BuildSealMasterCloseButton.original.c", "Builds Seal Master close button."},
        {"011da150", "FUN_011da150", "BuildSealMasterStatRows", "decompiled/DetailInfoUI/BuildSealMasterStatRows.original.c", "Builds Seal Master stat rows."},
        {"011da4b0", "FUN_011da4b0", "ResolveSealMasterStatText", "decompiled/DetailInfoUI/ResolveSealMasterStatText.original.c", "Maps Seal Master stat indexes to official text keys."},
        {"011da506", "switchD_011da4c0::caseD_1", "ResolveSealMasterDsTextCase", "decompiled/DetailInfoUI/ResolveSealMasterDsTextCase.original.c", "Decompiler split case for Seal Master DS text."},
        {"011da5e0", "FUN_011da5e0", "ResolveSealMasterStatValue", "decompiled/DetailInfoUI/ResolveSealMasterStatValue.original.c", "Formats Seal Master stat values."},
        {"011da8e0", "FUN_011da8e0", "BindSealMasterCloseButton", "decompiled/DetailInfoUI/BindSealMasterCloseButton.original.c", "Binds Seal Master close button event."},
        {"011daec0", "FUN_011daec0", "BuildTamerDetailRoot", "decompiled/DetailInfoUI/BuildTamerDetailRoot.original.c", "Builds Tamer tab root background and subpanels."},
        {"011daf90", "FUN_011daf90", "PrepareTamerDetailControls", "decompiled/DetailInfoUI/PrepareTamerDetailControls.original.c", "Prepares Tamer tab controls/events."},
        {"011db440", "FUN_011db440", "BuildTamerEquipmentSection", "decompiled/DetailInfoUI/BuildTamerEquipmentSection.original.c", "Builds equipped item slots and equipment button."},
        {"011db840", "FUN_011db840", "BuildTamerAttributePanel", "decompiled/DetailInfoUI/BuildTamerAttributePanel.original.c", "Builds right-side Tamer attribute panel background."},
        {"011dbca0", "FUN_011dbca0", "BuildTamerObtainedAttributes", "decompiled/DetailInfoUI/BuildTamerObtainedAttributes.original.c", "Builds obtained attribute stat table."},
        {"011dc210", "FUN_011dc210", "BuildTamerDigimonPreviewSection", "decompiled/DetailInfoUI/BuildTamerDigimonPreviewSection.original.c", "Builds Tamer/Digimon preview field."},
        {"011dc7f0", "FUN_011dc7f0", "BuildEquippedAchievementSection", "decompiled/DetailInfoUI/BuildEquippedAchievementSection.original.c", "Builds equipped achievement panel."},
        {"011dd050", "FUN_011dd050", "ResolveDetailInfoStatText", "decompiled/DetailInfoUI/ResolveDetailInfoStatText.original.c", "Maps stat indexes to official text keys."},
        {"011ed0a0", "FUN_011ed0a0", "BuildSharedDetailRewardPanel", "decompiled/Shared/BuildSharedDetailRewardPanel.original.c", "Shared reward panel using DetailInfo_bg_b."},
    }};
    return functions;
}

const std::array<OriginalAssetRef, 36>& GetOriginalAssetMap()
{
    static constexpr std::array<OriginalAssetRef, 36> assets = {{
        {"CommonUI/NinePatch/center_gradient2.tga", "BuildEquipmentDetailList", "Gradient row background used in equipment details."},
        {"CommonUI/CommonUI_close.tga", "BuildSealMasterCloseButton", "Close button used by Seal Master detail panel."},
        {"DetailInfo/Autocrop_OFF_Btn.bmp", "BuildVisibilityToggleButton", "Top-left visibility toggle OFF sprite."},
        {"DetailInfo/Autocrop_ON_Btn.bmp", "BuildVisibilityToggleButton", "Top-left visibility toggle ON sprite."},
        {"DetailInfo/Detailinfo_bg_a.tga", "BuildSealMasterPanel", "Seal Master panel background."},
        {"DetailInfo/DetailInfo_bg_b.tga", "BuildEquipmentPanel", "Equipment/detail popup background."},
        {"DetailInfo/DetailInfo_bg_b.tga", "BuildSharedDetailRewardPanel", "Shared DetailInfo reward popup background."},
        {"DetailInfo/Digimon_Status_Win.tga", "BuildDigimonDetailRoot", "Digimon tab main frame."},
        {"DetailInfo/Enchant_AT.tga", "BuildDigimonEnchantPanel", "AT enchant badge."},
        {"DetailInfo/Enchant_bl.tga", "BuildDigimonEnchantPanel", "BL enchant badge."},
        {"DetailInfo/Enchant_ct.tga", "BuildDigimonEnchantPanel", "CT enchant badge."},
        {"DetailInfo/Enchant_ev.tga", "BuildDigimonEnchantPanel", "EV enchant badge."},
        {"DetailInfo/Enchant_HP.tga", "BuildDigimonEnchantPanel", "HP enchant badge."},
        {"DetailInfo/Infinite_Match_window_Test04.tga", "BuildEquipmentDetailList", "Equipment detail row/window part."},
        {"DetailInfo/Infinite_Match_window_Test05.tga", "BuildEquipmentDetailList", "Equipment detail row/window part."},
        {"DetailInfo/RemainTimerBG.tga", "BuildDigimonInfoBox", "Digimon information field background."},
        {"DetailInfo/skin_bg_a.tga", "BuildAdditionalStatPanel", "Additional stat popup background."},
        {"DetailInfo/Storage_M_field_a.tga", "BuildDigimonInfoPanel", "Digimon portrait field background."},
        {"DetailInfo/Storage_M_field_a.tga", "BuildTamerDigimonPreviewSection", "Tamer tab preview field background."},
        {"DetailInfo/TamerStatus_Digimon_Tamer.tga", "BuildDigimonInfoPanel", "Circular portrait frame."},
        {"DetailInfo/TamerStatus_Digimon_Tamer.tga", "BuildTamerDigimonPreviewSection", "Circular preview frame."},
        {"DetailInfo/tamerstatus_win_01_bg.tga", "BuildEquipmentDetailList", "Equipment detail internal background."},
        {"DetailInfo/TamerStatus_win_02_bg.tga", "BuildTamerAttributePanel", "Right-side Tamer attribute panel background."},
        {"DetailInfo/TamerStatus_Win_03_bg.tga", "BuildDigimonDetailRoot", "Shared DetailInfo main background."},
        {"DetailInfo/TamerStatus_Win_03_bg.tga", "BuildTamerDetailRoot", "Shared DetailInfo main background."},
        {"DetailInfo/TamerStatus_Win_04_bg.tga", "BuildTamerDetailRoot", "Tamer tab frame."},
        {"DetailInfo/zoom_in_aa.tga", "BuildDigimonDetailRoot", "Zoom/help button beside Digimon data."},
        {"DigimonStatus_New/Digimon_Status_Enchant_01.tga", "BuildDigimonEnchantPanel", "Enchant section background."},
        {"DMG_SKINS/Mini_Out_btn_a.tga", "BuildTamerEquipmentSection", "Mini exit/button asset reused by Tamer equipment panel."},
        {"EquipSystem/Upgrade_Slot.tga", "BuildDigimonSkillSection", "Skill slot background."},
        {"Icon/Mask_Over.dds", "BuildTamerAttributePanel", "Icon mask overlay."},
        {"Mail/Mail_List_Box.tga", "BuildEquipmentDetailList", "List box background reused by equipment details."},
        {"TacticsHouse/CheckOnline.tga", "BuildDigimonAdvancePanel", "Check marker reused by advance area."},
        {"TacticsHouse/CheckOnline.tga", "BuildDigimonEnchantPanel", "Check marker reused by enchant area."},
        {"TacticsHouse/Storage_M_field.tga", "BuildDigimonInfoPanel", "Field background reused by Digimon info section."},
        {"TacticsHouse/Storage_M_field.tga", "BuildEquippedAchievementSection", "Field background reused by achievement section."},
    }};
    return assets;
}

const std::array<OriginalStatTextRef, 13>& GetObtainedAttributeStatTextMap()
{
    static constexpr std::array<OriginalStatTextRef, 13> stats = {{
        {0, "TOOLTIP_ENCHANT_EXPLAIN_TITLE_HP", "HP"},
        {1, "COMMON_TXT_DS", "DS"},
        {2, "TOOLTIP_ENCHANT_EXPLAIN_TITLE_AT", "AT"},
        {3, "COMMON_TXT_ATTACK_SPEED", "AS"},
        {4, "TOOLTIP_ENCHANT_EXPLAIN_TITLE_CR", "CT"},
        {5, "COMMON_TXT_STAT_HIT_RATE", "HT"},
        {6, "DETAIL_INFO_SCD", "Aumento SCD"},
        {7, "DETAIL_INFO_CRIDAMAGE", "Dano CT"},
        {8, "DETAIL_INFO_SD", "Dano SD"},
        {9, "DETAIL_INFO_ELEDMENT_DAMAGE", "Dano Base"},
        {10, "COMMON_TXT_STAT_DEFENSE", "Defesa"},
        {11, "TOOLTIP_ENCHANT_EXPLAIN_TITLE_BL", "BL"},
        {12, "TOOLTIP_ENCHANT_EXPLAIN_TITLE_EV", "EV"},
    }};
    return stats;
}

} // namespace detail_info_feature
