# Last Sweep Notes

Ultima varredura executada contra:

- `unpacked_exe_all/functions`
- `unpacked_GDMO.exe_xrefs.csv`
- `unpacked_GDMO.exe_strings.csv`

Padroes usados:

- `DetailInfo`
- `DETAIL_INFO`
- `TamerStatusUI`
- `DigimonStatusUI`
- `OTHER_TAMER_DETAIL_INFO`
- `TamerStatus_Win`
- `Digimon_Status_Win`
- `SEALMASTER_STATE`
- `MAINBAR_MEMU_SEAL_MASTER`

## Achados incorporados

A varredura mostrou que o painel `BuildSealMasterPanel` chamava funcoes que
ainda nao tinham sido copiadas. Elas foram adicionadas:

- `011da100_FUN_011da100.c` -> `BuildSealMasterCloseButton.original.c`
- `011da150_FUN_011da150.c` -> `BuildSealMasterStatRows.original.c`
- `011da4b0_FUN_011da4b0.c` -> `ResolveSealMasterStatText.original.c`
- `011da506_caseD_1.c` -> `ResolveSealMasterDsTextCase.original.c`
- `011da5e0_FUN_011da5e0.c` -> `ResolveSealMasterStatValue.original.c`
- `011da8e0_FUN_011da8e0.c` -> `BindSealMasterCloseButton.original.c`

## Achados nao incorporados como DetailInfo

O CSV `LastSweep_DetailInfoXrefs.csv` ainda mostra referencias marcadas como
`NO`. Elas foram mantidas fora do subprojeto principal porque pertencem a
sistemas vizinhos ou genericos:

- `AutocropUI` e `Autocrop/*.tga`: sistema macro/autocrop separado.
- `TamerStatus_New/*`: tela local de status antiga/nova, nao a janela
  `DetailInfo` da print.
- `FUN_00f206a0`: formatador/generico que tambem toca
  `DETAIL_INFO_ELEDMENT_DAMAGE`.
- funcoes globais de `SEALMASTER_STATE_*` fora do bloco `011da***`: pertencem
  ao sistema de selo mestre principal ou formatadores compartilhados.

Depois dos achados incorporados, o subprojeto possui 31 funcoes copiadas e 36
referencias de asset mapeadas.
