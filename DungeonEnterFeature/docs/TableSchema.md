# Dungeon Table Map

## Loaders

- `LoadDungeonTables.original.c` (`01607560`) carrega:
  - `DungeonList.bin`
  - `DungeonQualification.bin`
  - `DungeonObtainable.bin`
  - `DungeonValue.bin`
  - `DungeonReward.bin`
  - `DungeonStepInfo.bin`
  - `DungeonClearInfo.bin`
  - `Dungeon_Old.bin`
- `LoadMapDungeonRegionTables.original.c` (`0160bb10`) carrega:
  - `MapDungeonRegion.bin`

## XML/BIN copiados

- `DungeonList`: ids, campos de dificuldade/grupo e caminho da imagem de preview.
- `DungeonQualification`: requisitos de entrada.
- `DungeonObtainable`: ids dos itens exibidos em recompensa monstro.
- `DungeonValue`: valores globais da feature.
- `DungeonReward`: grupos de recompensa de clear.
- `DungeonStepInfo`: passos/progresso da dungeon.
- `DungeonClearInfo`: condicoes de clear.
- `Dungeon_Old`: tabela legada.
- `MapDungeonRegion`: vinculo com regioes/mapas.

O resumo quantitativo esta em `TableSummary.csv`.

## Textos

- `bin/Language/English/Dungeon_Str.bin`
- `bin/Language/English/MapDungeonRegion_Str.bin`

Esses bins carregam nomes/descricoes localizadas. Por isso o texto em portugues da screenshot nao aparece como XML de texto nesta pasta.
