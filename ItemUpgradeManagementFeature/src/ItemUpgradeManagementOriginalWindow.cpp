#include "../include/ItemUpgradeManagementOriginalWindow.h"

namespace item_upgrade_management_original {

std::string_view ItemUpgradeManagementOriginalWindow::InternalWindowName()
{
    return "EquipmentUpgradeSystem";
}

std::string_view ItemUpgradeManagementOriginalWindow::AdjacentTradeWindowName()
{
    return "DataTradeDigimonUpgrade";
}

const std::array<OriginalFunctionRef, 15>& ItemUpgradeManagementOriginalWindow::CoreFunctions()
{
    static constexpr std::array<OriginalFunctionRef, 15> refs = {{
        {"0128c370", "CreateEquipmentUpgradeSystemWindow", "decompiled/Launcher/CreateEquipmentUpgradeSystemWindow.original.c", "registered official viewer"},
        {"0128ac10", "CreateDataTradeDigimonUpgradeWindow", "decompiled/Launcher/CreateDataTradeDigimonUpgradeWindow.original.c", "adjacent official data-trade window"},
        {"01278ad0", "BuildUpgradeManagementWindow", "decompiled/MainWindow/BuildUpgradeManagementWindow.original.c", "main frame, title, close button, tabs"},
        {"01278990", "AttachUpgradeViewerToParent", "decompiled/MainWindow/AttachUpgradeViewerToParent.original.c", "parent/viewer attachment"},
        {"01271e80", "CreateStrengthenTabPanel", "decompiled/StrengthenTab/CreateStrengthenTabPanel.original.c", "Fortalecimento panel constructor"},
        {"01276140", "BuildStrengthenSlotNetwork", "decompiled/StrengthenTab/BuildStrengthenSlotNetwork.original.c", "slot network visible in screenshot"},
        {"01277870", "HandleStrengthenResultMessage", "decompiled/StrengthenTab/HandleStrengthenResultMessage.original.c", "strengthen result text/sound"},
        {"0126cb80", "CreateTransferTabPanel", "decompiled/TransferTab/CreateTransferTabPanel.original.c", "Transferencia panel constructor"},
        {"0126d080", "BuildTransferTabLayout", "decompiled/TransferTab/BuildTransferTabLayout.original.c", "transfer layout and response handling"},
        {"01271060", "HandleTransferResultMessage", "decompiled/TransferTab/HandleTransferResultMessage.original.c", "transfer result text/sound"},
        {"01268580", "CreateRestoreTabPanel", "decompiled/RestoreTab/CreateRestoreTabPanel.original.c", "Recuperacao panel constructor"},
        {"01268e20", "BuildRestoreTabLayout", "decompiled/RestoreTab/BuildRestoreTabLayout.original.c", "restore layout and button"},
        {"0126c260", "HandleRestoreResultMessage", "decompiled/RestoreTab/HandleRestoreResultMessage.original.c", "restore result text/sound"},
        {"012b5fb0", "ValidateUpgradeMaterialsAndMoney", "decompiled/ValidationAndPackets/ValidateUpgradeMaterialsAndMoney.original.c", "material/money validation"},
        {"015ec9e0", "LoadItemUpgradeTables", "decompiled/Tables/LoadItemUpgradeTables.original.c", "loads DM_ItemUpgrade table family"},
    }};
    return refs;
}

const std::array<OriginalAssetRef, 14>& ItemUpgradeManagementOriginalWindow::CoreAssets()
{
    static constexpr std::array<OriginalAssetRef, 14> refs = {{
        {"EquipSystem/Recharge_bg.tga", "main blue frame", "xref in 01278ad0"},
        {"CommonUI/CommonUI_close.tga", "close button", "xref in 01278ad0"},
        {"CommonUI/Menu_tap_btn.tga", "three top tabs", "xref in 01278ad0"},
        {"EquipSystem/Upgrade_Slot.tga", "item/data slots", "xref in tab slot functions"},
        {"EquipSystem/Upgrade_IC_BG.tga", "central icon background", "xref in 0126a270"},
        {"EquipSystem/Upgrade_Line_horizontal.tga", "slot connector lines", "xref in 01276140"},
        {"EquipSystem/Upgrade_Line_vertical.tga", "vertical connector line", "xref in 01276140"},
        {"Item_Make/Make_ani_gauge_pixel.tga", "animated gauge/stripe", "xref in tab gauge builders"},
        {"EquipSystem/Upgrade_Progress_Effect.dds", "process effect", "xref in result effect builders"},
        {"EquipSystem/Upgrade_Success_Effect.dds", "success effect", "xref in result effect builders"},
        {"EquipSystem/Upgrade_Fail_Effect.dds", "fail effect", "xref in result effect builders"},
        {"Random_box/bar_arrow.png", "arrow decoration in restore layout", "xref in 01268e20"},
        {"EquipSystem/ArrowBtn_Up.tga", "yellow counter up arrow", "official string at 01ae8cf0, visible in screenshot"},
        {"EquipSystem/ArrowBtn_Down.tga", "yellow counter down arrow", "official string at 01ae8cd0, visible in screenshot"},
    }};
    return refs;
}

const std::array<OriginalTextKeyRef, 4>& ItemUpgradeManagementOriginalWindow::CoreTextKeys()
{
    static constexpr std::array<OriginalTextKeyRef, 4> refs = {{
        {"UPGRADESYSTEM_TITLE", "Gerenciamento", "window title"},
        {"EQUIPSYSTEM_UPGRADE", "Fortalecimento", "first tab and main action"},
        {"UPGRADESYSTEM_TAB_UPGRADE_DATA_TRANSFER", "Transferencia", "second tab"},
        {"UPGRADESYSTEM_TAB_UPGRADE_DATA_RESTORE", "Recuperacao", "third tab"},
    }};
    return refs;
}

} // namespace item_upgrade_management_original
