# Asset Inventory

Fonte primaria:

- `assets/manifest/RequiredAssets.csv`
- `assets/manifest/DecompiledXrefStringReferences.csv`

Fonte secundaria:

- `assets/manifest/AdditionalKnownUiResourceStrings.csv`

## NewInventory

- `NewInventory/new_inventory_win.tga`
- `NewInventory/new_inventory_notification.tga`
- `NewInventory/new_inventory_btn_tab.tga`
- `NewInventory/new_inventory_lock.tga`
- `NewInventory/extra_inventory_notification.tga`
- `NewInventory/extra_red_highlight.tga`

## Category/UI Reuse

- `inventory/Invensort.tga`
- `CommonUI/CommonUI_btn_s.tga`
- `CommonUI/Deco_Windows_Title3.tga`
- `Control_G/ComboBox/Combo.tga`
- `Control_G/Popup/N2Dlg5.tga`
- `Talk/Common_Seletbar.tga`
- `Mail/Mail_Item_Icon.tga`
- `Encyclopedia/NewEncyclopedia/main/sarch.png`
- `Encyclopedia/NewEncyclopedia/main/re.png`

## SealMaster Reuse

- `SealMaster/btn_combobox.tga`
- `SealMaster/dropdown_list_0`
- `SealMaster/sealmaster_highlight.tga`
- `SealMaster/Sealmaster_highlight.tga`
- `SealMaster/sealmaster_bg_01.tga`

## Card Reuse

- `DigiCombine/DigiCombine_CardFront.dds`

## Pack01 Hash Check

Verificacao feita contra `ClientDist/Data/Pack01.hf`:

- `Data/Interface/Control_G/Popup/N2Dlg5.tga`: existe.
- `Data/Interface/SealMaster/sealmaster_highlight.tga`: existe.
- `Data/Interface/SealMaster/Sealmaster_highlight.tga`: existe pelo mesmo hash
  case-insensitive.
- `Data/Interface/SealMaster/sealmaster_belt_s.dds`: existe.
- `Data/Interface/NewInventory/new_inventory_lock.tga`: existe.
- `Data/Interface/Encyclopedia/NewEncyclopedia/main/sarch.png`: existe.
- `Data/Interface/Encyclopedia/NewEncyclopedia/main/re.png`: existe.

Importante: `Control_G/Popup/N2Dlg5.tga` sem o prefixo
`Data/Interface/` nao existe no hash do pack. No cliente, `cSprite::Init`
adiciona o working folder automaticamente; ao chamar `ChangeTexture`, mantenha
`bUseWorkingFolder=true` para nao cair na textura rosa de fallback.
