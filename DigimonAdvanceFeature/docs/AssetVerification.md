# Asset Verification

Varredura feita a partir de:

- `unpacked_exe_all/unpacked_GDMO.exe_xrefs.csv`
- `unpacked_exe_all/unpacked_GDMO.exe_defined_data.csv`
- 106 arquivos `.c` copiados para este subprojeto

## Resultado

- `RequiredAssetsTablesAndSounds.csv`: 91 referencias brutas encontradas.
- `ClassifiedAssetReferences.csv`: mesmas referencias classificadas.
- `CoreAndPopupAssetReferences.csv`: subset recomendado para a tela e popups.
- `AdjacentCopiedClusterAssetReferences.csv`: assets vindos de classes
  compartilhadas, preservados para auditoria.

## Pack/arquivos fisicos

O workspace possui:

- `ClientDist\Data\Music\Enchant_Room.mp3` copiado para
  `assets/copied-from-clientdist/Enchant_Room.mp3`
- `ClientDist\Data\Pack01.hf`
- `ClientDist\Data\Pack01.pf`

As texturas de interface seguem como paths oficiais no manifesto porque o
Pack01 nao foi extraido para este subprojeto.

## Tabelas

Todos os XML/BIN/struct abaixo existem no `Dcommerce` e foram copiados:

- `EvoDigitama`
- `EnchantItemInfo`
- `EnchantStatInfo`
- `TranscendNeed`
- `TranscendMaterial`
- `TranscendCorrect`
- `TranscendCorrectEx`
- `ItemData`
- `ItemType`
- `ItemRankData`
- `ItemRankEffectData`
- `QuestItemData`
- `DEvolutionAdvice`

## Pendencias antes de portar

- Extrair os paths do `CoreAndPopupAssetReferences.csv` do Pack01.
- Conferir a arvore 3D/cenario real da sala, porque parte dela aparece como
  `Data/etcobject/DatsCenter/*.nif` na cadeia de som/cena.
- Verificar se o cliente principal ja possui wrappers equivalentes para
  `DigiPowerEnchant`, `EnchantOption` e `EnchantOptionResultUI`.
