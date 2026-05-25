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

A primeira varredura mostrou que o painel `BuildSealMasterPanel` chamava
funcoes que ainda nao tinham sido copiadas. Elas foram adicionadas:

- `011da100_FUN_011da100.c` -> `BuildSealMasterCloseButton.original.c`
- `011da150_FUN_011da150.c` -> `BuildSealMasterStatRows.original.c`
- `011da4b0_FUN_011da4b0.c` -> `ResolveSealMasterStatText.original.c`
- `011da506_caseD_1.c` -> `ResolveSealMasterDsTextCase.original.c`
- `011da5e0_FUN_011da5e0.c` -> `ResolveSealMasterStatValue.original.c`
- `011da8e0_FUN_011da8e0.c` -> `BindSealMasterCloseButton.original.c`

Uma varredura profunda posterior percorreu a familia `011d****`, strings de UI,
enderecos de xref e chamadas diretas entre funcoes. Ela gerou:

- `DeepSweep_DetailInfoCandidateFunctions.csv`: 125 funcoes candidatas.
- `DeepSweep_DetailInfoCallEdges.csv`: 85 ligacoes de chamada.
- `DeepSweepCopiedFunctionFiles.csv`: 39 funcoes oficiais adicionais copiadas.

Os novos arquivos copiados cobrem quatro areas que faltavam:

- `DetailInfoMainUI`: shell da janela, abas `Tamer`/`Digimon`, estado local e
  outro jogador.
- `DetailInfoUI`: construtores/renderizadores auxiliares das abas `Tamer` e
  `Digimon`.
- `DetailInfoAdditionalStat`: popup de atributos adicionais.
- popup de equipamento e botoes `Equipamento`/`Selo Mestre`.

Novas referencias de asset incorporadas:

- `System\Btn_Radio1.tga`
- `DMG_SKINS\skin_btn_a.tga`
- `DMG_SKINS\skin_btn_b.tga`
- `DMG_SKINS\Mini_Out_btn_b.tga`
- `Number.dds`

## Achados nao incorporados como DetailInfo

Os CSVs `LastSweep_DetailInfoXrefs.csv` e
`DeepSweep_DetailInfoCandidateFunctions.csv` ainda mostram referencias de baixa
confianca ou marcadas como `NO`. Elas foram mantidas fora do subprojeto
principal porque pertencem a sistemas vizinhos ou genericos:

- `AutocropUI` e `Autocrop/*.tga`: sistema macro/autocrop separado.
- `TamerStatus_New/*`: tela local de status antiga/nova, nao a janela
  `DetailInfo` da print.
- `FUN_00f206a0`: formatador/generico que tambem toca
  `DETAIL_INFO_ELEDMENT_DAMAGE`.
- funcoes globais de `SEALMASTER_STATE_*` fora do bloco `011da***`: pertencem
  ao sistema de selo mestre principal ou formatadores compartilhados.
- funcoes de Union/Digi-event em `011dd910+`: ficam coladas no dump, mas nao
  fazem parte da tela `Detalhes` das prints sem uma xref mais forte.

Depois dos achados incorporados, o subprojeto possui 70 funcoes mapeadas e 42
referencias de asset mapeadas.
