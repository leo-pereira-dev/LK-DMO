#include "../include/ExtraInventoryOriginalWindow.h"

namespace extra_inventory_feature {

const std::array<OriginalFunctionRef, 31>& GetOriginalFunctionMap()
{
    static constexpr std::array<OriginalFunctionRef, 31> functions = {{
        {"0128ef50", "FUN_0128ef50", "CreateExtraInventoryUI", "decompiled/ExtraInventoryUI/CreateExtraInventoryUI.original.c", "Creates and registers ExtraInventoryUI."},
        {"011a90d0", "FUN_011a90d0", "InitializeExtraInventoryUI", "decompiled/ExtraInventoryUI/InitializeExtraInventoryUI.original.c", "Initializes ExtraInventoryUI against contents subject."},
        {"011a9f80", "FUN_011a9f80", "BuildExtraInventoryTabs", "decompiled/ExtraInventoryUI/BuildExtraInventoryTabs.original.c", "Creates category sub UIs for seal, ticket, evolution, digitama, and material."},
        {"013a90e0", "FUN_013a90e0", "ResolveExtraInventoryWindowTitle", "decompiled/ExtraInventoryUI/ResolveExtraInventoryWindowTitle.original.c", "Contains EXTRA_INVEN_TITLE text lookup."},
        {"01056780", "FUN_01056780", "ResolveExtraInventoryCategoryText", "decompiled/ExtraInventoryUI/ResolveExtraInventoryCategoryText.original.c", "Maps category ids to side menu text keys."},
        {"010567d8", "switchD_01056788::caseD_c", "ResolveExtraInventoryTicketTextCase", "decompiled/ExtraInventoryUI/ResolveExtraInventoryTicketTextCase.original.c", "Decompiler split case for ticket category text."},
        {"011a3290", "FUN_011a3290", "CreateExtraInventoryBaseSlot", "decompiled/ExtraInventoryUI/CreateExtraInventoryBaseSlot.original.c", "Creates a base item slot entry."},
        {"011a36a0", "FUN_011a36a0", "BuildExtraInventoryItemSlot", "decompiled/ExtraInventoryUI/BuildExtraInventoryItemSlot.original.c", "Builds item icon/count/lock slot visuals."},
        {"011a5b40", "FUN_011a5b40", "CreateExtraInventoryEvoSlot", "decompiled/ExtraInventoryUI/CreateExtraInventoryEvoSlot.original.c", "Creates an evolution extra inventory slot entry."},
        {"012faae0", "FUN_012faae0", "BuildNewInventoryTabButton", "decompiled/ExtraInventoryUI/BuildNewInventoryTabButton.original.c", "Builds tab button from NewInventory button texture."},
        {"012faf80", "FUN_012faf80", "BuildNewInventoryTabButtonEx", "decompiled/ExtraInventoryUI/BuildNewInventoryTabButtonEx.original.c", "Builds variant tab button from NewInventory button texture."},
        {"011ad850", "FUN_011ad850", "BuildSealInventoryCategory", "decompiled/ExtraInventoryUI/Categories/BuildSealInventoryCategory.original.c", "Builds the seal inventory category shown in the screenshot."},
        {"011b0b60", "FUN_011b0b60", "BuildTicketInventoryCategory", "decompiled/ExtraInventoryUI/Categories/BuildTicketInventoryCategory.original.c", "Builds ticket category."},
        {"011a5270", "FUN_011a5270", "BuildEvolutionInventoryCategory", "decompiled/ExtraInventoryUI/Categories/BuildEvolutionInventoryCategory.original.c", "Builds evolution item category."},
        {"011a29c0", "FUN_011a29c0", "BuildDigitamaInventoryCategory", "decompiled/ExtraInventoryUI/Categories/BuildDigitamaInventoryCategory.original.c", "Builds DigiOvos category."},
        {"011ab290", "FUN_011ab290", "BuildMaterialInventoryCategory", "decompiled/ExtraInventoryUI/Categories/BuildMaterialInventoryCategory.original.c", "Builds material category."},
        {"011adee0", "FUN_011adee0", "BuildSealInventoryGrid", "decompiled/ExtraInventoryUI/Seal/BuildSealInventoryGrid.original.c", "Builds grid and scrollbar for seal items."},
        {"011ae540", "FUN_011ae540", "BuildSealStatFilterDropdown", "decompiled/ExtraInventoryUI/Seal/BuildSealStatFilterDropdown.original.c", "Builds stat filter dropdown for Seal Master states."},
        {"011a60d0", "FUN_011a60d0", "BuildSealOverviewDropdown", "decompiled/ExtraInventoryUI/Seal/BuildSealOverviewDropdown.original.c", "Builds overview/grade dropdown containing SEALMASTER_SHOW_ALL."},
        {"011a72c0", "FUN_011a72c0", "BuildExtraInventoryNotificationSlot", "decompiled/ExtraInventoryUI/Seal/BuildExtraInventoryNotificationSlot.original.c", "Builds notification/card slot overlay pieces."},
        {"011a7b60", "FUN_011a7b60", "BuildExtraInventoryNotificationFront", "decompiled/ExtraInventoryUI/Seal/BuildExtraInventoryNotificationFront.original.c", "Builds notification/card front visuals."},
        {"011ac950", "FUN_011ac950", "HandleSealAlreadyMasteredMessage", "decompiled/ExtraInventoryUI/Seal/HandleSealAlreadyMasteredMessage.original.c", "Uses EXTRA_INVEN_SEAL_ALREADY_MASTERED."},
        {"01054c10", "FUN_01054c10", "HandleExtraInventoryFailure", "decompiled/Shared/HandleExtraInventoryFailure.original.c", "Shared failure message path."},
        {"01055f10", "FUN_01055f10", "HandleExtraInventoryAlreadySorted", "decompiled/Shared/HandleExtraInventoryAlreadySorted.original.c", "Shared sorted message path."},
        {"01057640", "FUN_01057640", "ResolveExtraInventoryAllGetMessage", "decompiled/Shared/ResolveExtraInventoryAllGetMessage.original.c", "Resolves lack/no item messages for all-get action."},
        {"010578a0", "FUN_010578a0", "HandleExtraInventoryWrongMove", "decompiled/Shared/HandleExtraInventoryWrongMove.original.c", "Shared wrong move message path."},
        {"01057d70", "FUN_01057d70", "HandleExtraInventoryFilterFail", "decompiled/Shared/HandleExtraInventoryFilterFail.original.c", "Shared filter fail message path."},
        {"01643ff0", "FUN_01643ff0", "LoadExtraInventoryEvoDataTable", "decompiled/Shared/LoadExtraInventoryEvoDataTable.original.c", "Loads DM_Extra_Inven-Evo_Data.bin."},
        {"0167c050", "FUN_0167c050", "LoadExtraInventoryTextTable", "decompiled/Shared/LoadExtraInventoryTextTable.original.c", "Loads DM_Extra_Inven-EvoExInven_Str.bin."},
        {"01624c60", "FUN_01624c60", "LoadSealMasterTables", "decompiled/Shared/LoadSealMasterTables.original.c", "Loads SealCard/SealLeader/Seal image/filter tables."},
        {"016760d0", "FUN_016760d0", "LoadSealMasterNameText", "decompiled/Shared/LoadSealMasterNameText.original.c", "Loads SealMasterName_Str.bin."},
    }};
    return functions;
}

const std::array<OriginalAssetRef, 17>& GetOriginalAssetMap()
{
    static constexpr std::array<OriginalAssetRef, 17> assets = {{
        {"NewInventory/new_inventory_win.tga", "ExtraInventoryUI", "Known string at 01aedb8c; likely root window frame."},
        {"NewInventory/new_inventory_notification.tga", "ExtraInventoryUI", "Known string at 01aedbb0; related notification frame."},
        {"NewInventory/new_inventory_btn_tab.tga", "BuildNewInventoryTabButton", "Direct xref."},
        {"NewInventory/new_inventory_lock.tga", "BuildExtraInventoryItemSlot", "Direct xref."},
        {"NewInventory/extra_inventory_notification.tga", "BuildExtraInventoryNotificationSlot", "Direct xref."},
        {"NewInventory/extra_red_highlight.tga", "BuildExtraInventoryNotificationSlot", "Direct xref."},
        {"inventory/Invensort.tga", "Build*InventoryCategory", "Direct xref in every category."},
        {"CommonUI/CommonUI_btn_s.tga", "Build*InventoryCategory", "All-get button background."},
        {"Control_G/ComboBox/Combo.tga", "BuildSeal*Dropdown", "Direct xref."},
        {"Control_G/Popup/N2Dlg5.tga", "BuildExtraInventoryItemSlot", "Slot/popup background."},
        {"Talk/Common_Seletbar.tga", "BuildSeal*Dropdown", "Dropdown selected bar."},
        {"SealMaster/btn_combobox.tga", "BuildSeal*Dropdown", "Direct xref."},
        {"SealMaster/dropdown_list_0", "BuildSeal*Dropdown", "Dropdown resource prefix string."},
        {"SealMaster/sealmaster_highlight.tga", "BuildSealInventoryGrid", "Selected/highlight cell overlay."},
        {"SealMaster/sealmaster_bg_01.tga", "BuildExtraInventoryNotificationFront", "Seal card background reuse."},
        {"DigiCombine/DigiCombine_CardFront.dds", "BuildExtraInventoryNotificationSlot", "Card front reuse."},
        {"Mail/Mail_Item_Icon.tga", "BuildExtraInventoryItemSlot", "Item icon frame reuse."},
    }};
    return assets;
}

const std::array<CategoryTextRef, 5>& GetCategoryTextMap()
{
    static constexpr std::array<CategoryTextRef, 5> categories = {{
        {11, "EXTRA_INVEN_LIST_SEAL", "Inventario de Selo"},
        {12, "EXTRA_INVEN_LIST_TICKET", "Bolsa de Bilhete"},
        {13, "EXTRA_INVEN_LIST_Evolution", "Itens de Evolucao"},
        {14, "EXTRA_INVEN_LIST_DIGITAMA", "DigiOvos"},
        {15, "EXTRA_INVEN_LIST_MATERIAL", "Materiais"},
    }};
    return categories;
}

} // namespace extra_inventory_feature
