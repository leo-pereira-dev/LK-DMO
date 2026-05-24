# Window Reconstruction Map

Esta janela corresponde ao sistema oficial `ExtraInventoryUI`, com a aba de
selo implementada em `CExtraInvenSealUI`.

## Classes/RTTI

- `CExtraInventoryUI`
- `CExtraInventoryContents`
- `CExtraInvenBaseUI`
- `CExtraInvenSealUI`
- `CExtraInvenTicketUI`
- `CExtraInvenEvoUI`
- `CExtraInvenDigitamaUI`
- `CExtraInvenMaterialUI`
- `CExtraInvenSealData`
- `CExtraInvenTicketData`
- `CExtraInvenEvoData`
- `CExtraInvenDigitamaData`
- `CExtraInvenMaterialData`

## Janela base

- `FUN_0128ef50` -> `CreateExtraInventoryUI.original.c`
- `FUN_011a90d0` -> `InitializeExtraInventoryUI.original.c`
- `FUN_011a9f80` -> `BuildExtraInventoryTabs.original.c`
- `FUN_013a90e0` -> `ResolveExtraInventoryWindowTitle.original.c`

Texto de titulo:

- `EXTRA_INVEN_TITLE`

## Menu lateral

`FUN_01056780` resolve os textos das categorias:

| Id | Text key | Label da print |
| --- | --- | --- |
| 11 | `EXTRA_INVEN_LIST_SEAL` | Inventario de Selo |
| 12 | `EXTRA_INVEN_LIST_TICKET` | Bolsa de Bilhete |
| 13 | `EXTRA_INVEN_LIST_Evolution` | Itens de Evolucao |
| 14 | `EXTRA_INVEN_LIST_DIGITAMA` | DigiOvos |
| 15 | `EXTRA_INVEN_LIST_MATERIAL` | Materiais |

## Aba Inventario de Selo

- `FUN_011ad850` -> `BuildSealInventoryCategory.original.c`
- `FUN_011adee0` -> `BuildSealInventoryGrid.original.c`
- `FUN_011ae540` -> `BuildSealStatFilterDropdown.original.c`

O combo `Visao geral` de atributo vem da chave `SEALMASTER_STATE_COMBO_ALL`,
usada por `BuildSealStatFilterDropdown`.

Botao inferior:

- `EXTRA_INVEN_SEAL_BTN_ALLGET`

## Aba Itens de Evolucao

- `FUN_011a5270` -> `Categories/BuildEvolutionInventoryCategory.original.c`
- `FUN_011a58b0` -> `Evolution/BuildEvolutionInventoryGrid.original.c`
- `FUN_011a60d0` -> `Evolution/BuildEvolutionGradeDropdown.original.c`
- `FUN_011a7da0` -> `Evolution/BuildEvolutionSearchControls.original.c`

O grid usa slot 47x69, step 47x69, textura
`Control_G/Popup/N2Dlg5.tga` e highlight
`SealMaster/Sealmaster_highlight.tga`.

O dropdown de grade usa `SEALMASTER_SHOW_ALL` e as chaves
`ENCY_DIGIMON_GRADE_*`: N, A, A+, S, S+, SS, SS+, SSS, SSS+ e U.

A busca vem de `D_COMMERCE_BUYER_TXT_SEARCH_EMPTY_KEYWORD`, com fundo
`CommonUI/Deco_Windows_Title3.tga` e botoes
`Encyclopedia/NewEncyclopedia/main/sarch.png` e
`Encyclopedia/NewEncyclopedia/main/re.png`.

## Categorias irmas

Tambem foram copiadas porque a tela monta todas as abas:

- `FUN_011b0b60` -> `BuildTicketInventoryCategory.original.c`
- `FUN_011a5270` -> `BuildEvolutionInventoryCategory.original.c`
- `FUN_011a29c0` -> `BuildDigitamaInventoryCategory.original.c`
- `FUN_011ab290` -> `BuildMaterialInventoryCategory.original.c`
- `FUN_011b11b0` -> `Shared/BuildTicketInventoryGrid.original.c`
- `FUN_011a3020` -> `Shared/BuildDigitamaInventoryGrid.original.c`
- `FUN_011ab870` -> `Shared/BuildMaterialInventoryGrid.original.c`

## Assets principais

Refs diretas:

- `NewInventory/new_inventory_btn_tab.tga`
- `NewInventory/new_inventory_lock.tga`
- `NewInventory/extra_inventory_notification.tga`
- `NewInventory/extra_red_highlight.tga`
- `inventory/Invensort.tga`
- `CommonUI/CommonUI_btn_s.tga`
- `CommonUI/Deco_Windows_Title3.tga`
- `Control_G/ComboBox/Combo.tga`
- `Control_G/Popup/N2Dlg5.tga`
- `Encyclopedia/NewEncyclopedia/main/sarch.png`
- `Encyclopedia/NewEncyclopedia/main/re.png`
- `SealMaster/btn_combobox.tga`
- `SealMaster/sealmaster_highlight.tga`
- `SealMaster/Sealmaster_highlight.tga`
- `SealMaster/sealmaster_bg_01.tga`
- `Talk/Common_Seletbar.tga`

Strings relacionadas sem xref direto no dump copiado:

- `NewInventory/new_inventory_win.tga`
- `NewInventory/new_inventory_notification.tga`
- `SealMaster/dropdown_list_0`
