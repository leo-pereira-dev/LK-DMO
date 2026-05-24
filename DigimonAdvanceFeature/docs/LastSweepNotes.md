# Last Sweep Notes

Data: 2026-05-23

## O que foi conferido

- Strings oficiais e RTTI: `EnchantTactics`, `DigiPowerEnchant`,
  `EnchantOption`, `EnchantOptionResultUI`, `DigimonTranscendenceHelp` e
  `DigimonEnchantPage`.
- Chaves de texto `ENCHANTTACTICS_*`, `DIGIMONTRANSCEND_*` e
  `DIGIMONTRANSCENDHELP_*`.
- Loader de tabelas `0161e970` e callbacks `0161f0b0` ate `0161f170`.
- Assets e sons referenciados por xrefs diretos nas funcoes copiadas.
- XML/BIN/struct presentes em `Dcommerce`.

## Resultado

- 106/106 funcoes solicitadas foram copiadas.
- 13/13 tabelas de dados selecionadas foram copiadas com XML e BIN.
- 3/3 language bins em ingles foram copiadas.
- O asset solto `Enchant_Room.mp3` foi encontrado e copiado.
- As demais texturas/efeitos continuam registrados por path oficial de pack.

## Cuidado

O trace completo inclui classes compartilhadas. Para a porta inicial, use
`CoreAndPopupAssetReferences.csv` como lista principal e mantenha
`AdjacentCopiedClusterAssetReferences.csv` apenas como auditoria do que veio
junto por reutilizacao de classe.
