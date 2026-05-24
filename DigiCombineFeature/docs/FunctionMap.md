# Function Map

Mapa das funcoes decompiladas relevantes. Os arquivos foram copiados com nomes
por responsabilidade em `DigiCombineFeature/decompiled`.

## DigiCombine

| Original | Nome organizado | Responsabilidade |
| --- | --- | --- |
| `01630020_FUN_01630020.c` | `LoadDigiCombineTables.original.c` | Carrega `DM_DigiCombine-ItemList.bin`, `RankList`, `ItemGroup`, `CeilGroup` e registra callbacks. |
| `01630200_FUN_01630200.c` | `ParseDigiCombineItemListCallback.original.c` | Callback para `ItemList`. |
| `01630220_FUN_01630220.c` | `ParseDigiCombineRankListCallback.original.c` | Callback para `RankList`. |
| `01630260_FUN_01630260.c` | `ParseDigiCombineItemGroupCallback.original.c` | Callback para `ItemGroup`. |
| `01630240_FUN_01630240.c` | `ParseDigiCombineCeilGroupCallback.original.c` | Callback para `CeilGroup`. |
| `0162e3a0_FUN_0162e3a0.c` | `RegisterDigiCombineBinParser.original.c` | Cria um parser de bin e adiciona na lista de parsers do table manager. |
| `0162eb70_FUN_0162eb70.c` | `ReadDigiCombineItemList.original.c` | Le entradas `ItemId`, `rankKey`, `catalogValue`. |
| `0162e700_FUN_0162e700.c` | `ReadDigiCombineRankList.original.c` | Le entradas `rankKey`, `displayRank`, `ceilingFailCount`, `ceilingGroupKey`. |
| `0162e920_FUN_0162e920.c` | `ReadDigiCombineRewardGroups.original.c` | Le grupos normais e ceiling. |
| `0162ee00_FUN_0162ee00.c` | `ReadDigiCombineRewardEntries.original.c` | Le recompensas dentro de cada grupo. |

## UnionCombine

| Original | Nome organizado | Responsabilidade |
| --- | --- | --- |
| `0164d930_FUN_0164d930.c` | `LoadUnionCombineTables.original.c` | Carrega `DM_UnionCombine-ItemList.bin`, `RankList`, `ItemGroup`, `CeilGroup`. |
| `0164db10_FUN_0164db10.c` | `ParseUnionCombineItemListCallback.original.c` | Callback para `ItemList`. |
| `0164db30_FUN_0164db30.c` | `ParseUnionCombineRankListCallback.original.c` | Callback para `RankList`. |
| `0164db70_FUN_0164db70.c` | `ParseUnionCombineItemGroupCallback.original.c` | Callback para `ItemGroup`. |
| `0164db50_FUN_0164db50.c` | `ParseUnionCombineCeilGroupCallback.original.c` | Callback para `CeilGroup`. |
| `0164c930_FUN_0164c930.c` | `RegisterUnionCombineBinParser.original.c` | Cria um parser de bin e adiciona na lista de parsers do table manager. |
| `0164d0b0_FUN_0164d0b0.c` | `ReadUnionCombineItemList.original.c` | Le entradas `ItemId`, `rankKey`, `catalogValue`. |
| `0164cc40_FUN_0164cc40.c` | `ReadUnionCombineRankList.original.c` | Le entradas `rankKey`, `displayRank`, `ceilingFailCount`, `ceilingGroupKey`. |
| `0164ce60_FUN_0164ce60.c` | `ReadUnionCombineRewardGroups.original.c` | Le grupos normais e ceiling. |
| `0164d340_FUN_0164d340.c` | `ReadUnionCombineRewardEntries.original.c` | Le recompensas dentro de cada grupo. |

## Loader table order

O loader original chama os arquivos nesta ordem:

1. `ItemList`
2. `RankList`
3. `ItemGroup`
4. `CeilGroup`

Essa ordem foi preservada em `CombineTableRepository::LoadFromXmlDirectory`.
