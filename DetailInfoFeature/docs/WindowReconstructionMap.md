# Window Reconstruction Map

Este mapa descreve como as duas abas da print se conectam aos arquivos
decompilados oficiais.

## Janela principal

Classe/RTTI oficial:

- `cDetailInfoMainUI`
- `cDetailInfoMainUI_Other`
- `cDetailInfoContents`
- `AdaptDetailInfo`

Pacotes oficiais encontrados:

- `C2GS_REQ_OTHER_TAMER_DETAIL_INFO`
- `GS2C_RES_OTHER_TAMER_DETAIL_INFO`
- `nsOtherTamerDetailInfo::stTamer`
- `nsOtherTamerDetailInfo::stDigimon`

## Aba Tamer

Funcao raiz:

- `FUN_011daec0` -> `BuildTamerDetailRoot.original.c`

Subfuncoes chamadas pela raiz:

- `FUN_011daf90` -> prepara controles/eventos da aba.
- `FUN_011db440` -> equipamentos do Tamer.
- `FUN_011db840` -> painel direito de atributos.
- `FUN_011dbca0` -> tabela `Atributos Obtidos`.
- `FUN_011dc210` -> preview/campo de Tamer/Digimon.
- `FUN_011dc7f0` -> `Conquista Equipada`.

Assets principais da aba:

- `DetailInfo/TamerStatus_Win_03_bg.tga`
- `DetailInfo/TamerStatus_Win_04_bg.tga`
- `DetailInfo/TamerStatus_win_02_bg.tga`
- `DetailInfo/Storage_M_field_a.tga`
- `DetailInfo/TamerStatus_Digimon_Tamer.tga`

## Aba Digimon

Funcao raiz:

- `FUN_011d2a20` -> `BuildDigimonDetailRoot.original.c`

Subfuncoes chamadas pela raiz:

- `FUN_011d2b30` -> retrato/nome/level do Digimon.
- `FUN_011d3690` -> secao de dados do Digimon.
- `FUN_011d3b00` -> slots de habilidade.
- `FUN_011d30b0` -> caixa `Informacao Digimon`.
- `FUN_011d4320` -> `Encanto Digimon`.
- `FUN_011d4bd0` -> `Avanco de Digimon`.
- `FUN_011d50f0` -> finalizacao/eventos.

Assets principais da aba:

- `DetailInfo/TamerStatus_Win_03_bg.tga`
- `DetailInfo/Digimon_Status_Win.tga`
- `DetailInfo/Storage_M_field_a.tga`
- `DetailInfo/TamerStatus_Digimon_Tamer.tga`
- `DetailInfo/RemainTimerBG.tga`
- `DetailInfo/zoom_in_aa.tga`
- `DetailInfo/Enchant_AT.tga`
- `DetailInfo/Enchant_ct.tga`
- `DetailInfo/Enchant_bl.tga`
- `DetailInfo/Enchant_HP.tga`
- `DetailInfo/Enchant_ev.tga`

## Toggle ON/OFF

Funcao:

- `FUN_011d82a0` -> `BuildVisibilityToggleButton.original.c`

Assets:

- `DetailInfo/Autocrop_ON_Btn.bmp`
- `DetailInfo/Autocrop_OFF_Btn.bmp`
- paths completos tambem existem como
  `Data/Interface/DetailInfo/Autocrop_ON_Btn.bmp` e
  `Data/Interface/DetailInfo/Autocrop_OFF_Btn.bmp`.

## Selo Mestre

Funcao raiz:

- `FUN_011da000` -> `BuildSealMasterPanel.original.c`

Dependencias encontradas na ultima varredura:

- `FUN_011da100` -> `BuildSealMasterCloseButton.original.c`
- `FUN_011da150` -> `BuildSealMasterStatRows.original.c`
- `FUN_011da4b0` -> `ResolveSealMasterStatText.original.c`
- `FUN_011da506` -> `ResolveSealMasterDsTextCase.original.c`
- `FUN_011da5e0` -> `ResolveSealMasterStatValue.original.c`
- `FUN_011da8e0` -> `BindSealMasterCloseButton.original.c`

Assets principais:

- `DetailInfo/Detailinfo_bg_a.tga`
- `CommonUI/CommonUI_close.tga`

## Stat table

`FUN_011dd050` resolve os textos da tabela `Atributos Obtidos` por indice:

| Index | Text key | Label da print |
| --- | --- | --- |
| 0 | `TOOLTIP_ENCHANT_EXPLAIN_TITLE_HP` | HP |
| 1 | `COMMON_TXT_DS` | DS |
| 2 | `TOOLTIP_ENCHANT_EXPLAIN_TITLE_AT` | AT |
| 3 | `COMMON_TXT_ATTACK_SPEED` | AS |
| 4 | `TOOLTIP_ENCHANT_EXPLAIN_TITLE_CR` | CT |
| 5 | `COMMON_TXT_STAT_HIT_RATE` | HT |
| 6 | `DETAIL_INFO_SCD` | Aumento SCD |
| 7 | `DETAIL_INFO_CRIDAMAGE` | Dano CT |
| 8 | `DETAIL_INFO_SD` | Dano SD |
| 9 | `DETAIL_INFO_ELEDMENT_DAMAGE` | Dano Base |
| 10 | `COMMON_TXT_STAT_DEFENSE` | Defesa |
| 11 | `TOOLTIP_ENCHANT_EXPLAIN_TITLE_BL` | BL |
| 12 | `TOOLTIP_ENCHANT_EXPLAIN_TITLE_EV` | EV |
