# Asset Inventory

Fonte primaria:

- `assets/manifest/Pack01RequiredAssets.csv`
- `assets/manifest/DecompiledXrefStringReferences.csv`

Os assets abaixo foram encontrados por xref ou referencia direta nos `.c`
copiados. As imagens de interface devem sair do Pack01 em `Data/Interface`.

## DetailInfo

- `DetailInfo/Autocrop_OFF_Btn.bmp`
- `DetailInfo/Autocrop_ON_Btn.bmp`
- `DetailInfo/Detailinfo_bg_a.tga`
- `DetailInfo/DetailInfo_bg_b.tga`
- `DetailInfo/Digimon_Status_Win.tga`
- `DetailInfo/Enchant_AT.tga`
- `DetailInfo/Enchant_bl.tga`
- `DetailInfo/Enchant_ct.tga`
- `DetailInfo/Enchant_ev.tga`
- `DetailInfo/Enchant_HP.tga`
- `DetailInfo/Infinite_Match_window_Test04.tga`
- `DetailInfo/Infinite_Match_window_Test05.tga`
- `DetailInfo/RemainTimerBG.tga`
- `DetailInfo/skin_bg_a.tga`
- `DetailInfo/Storage_M_field_a.tga`
- `DetailInfo/TamerStatus_Digimon_Tamer.tga`
- `DetailInfo/tamerstatus_win_01_bg.tga`
- `DetailInfo/TamerStatus_win_02_bg.tga`
- `DetailInfo/TamerStatus_Win_03_bg.tga`
- `DetailInfo/TamerStatus_Win_04_bg.tga`
- `DetailInfo/zoom_in_aa.tga`

## Shared/Reused

- `CommonUI/NinePatch/center_gradient2.tga`
- `CommonUI/CommonUI_close.tga`
- `DigimonStatus_New/Digimon_Status_Enchant_01.tga`
- `DMG_SKINS/Mini_Out_btn_a.tga`
- `DMG_SKINS/Mini_Out_btn_b.tga`
- `DMG_SKINS/skin_btn_a.tga`
- `DMG_SKINS/skin_btn_b.tga`
- `EquipSystem/Upgrade_Slot.tga`
- `Icon/Mask_Over.dds`
- `Mail/Mail_List_Box.tga`
- `Number.dds`
- `System/Btn_Radio1.tga`
- `TacticsHouse/CheckOnline.tga`
- `TacticsHouse/Storage_M_field.tga`

`TacticsHouse/Storage_M_field.tga` aparece em mais de uma funcao copiada
(`BuildDigimonInfoPanel` e `BuildEquippedAchievementSection`).
`System/Btn_Radio1.tga` e usado pelos botoes de aba `Tamer`/`Digimon` nas
variantes local e outro jogador da janela principal.

## Status

`TamerStatusUI`, `DigimonStatusUI` e `DigimonStatusUI_Help` aparecem como
classes oficiais relacionadas. O corpo visual da janela da print, porem, esta
no bloco `DetailInfo`, por isso os assets primarios estao sob
`DetailInfo/*`.
