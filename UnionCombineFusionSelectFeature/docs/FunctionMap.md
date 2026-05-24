# Function Map

This subproject keeps the copied `.c` files byte-for-byte close to the
decompiled dump and gives them readable names through filenames, manifests, and
`include/UnionCombineFusionSelectOriginalWindow.h`.

## UI family

The selected window is `UnionCombineUI`, not the older `DigiCombineUI` window.
The strongest screenshot match is the rank filter set:

- screenshot: `Todos`, `N`, `A`, `A+`, `S`, `S+`, `SS`, `SS+`
- `DM_UnionCombine-RankList.xml`: 7 records
- `DM_DigiCombine-RankList.xml`: 8 records

## Core UI functions

- `010a78d0` -> `EnterUnionCombineSceneAndPlayBgm`
- `01290160` -> `CreateUnionCombineUI`
- `011f9d20` -> `BuildUnionCombineMainUI`
- `011fb410` -> `BuildUnionCombineContents`
- `011fbcd0` -> `BuildUnionCombineRegisterUI`
- `011fc500` -> `BindUnionCombineRegisterEvents`
- `011fced0` -> `UpdateUnionCombineRegisterState`
- `011fd5f0` -> `BuildUnionCombineMaterialRows`
- `011fdd70` -> `BuildUnionCombineRegisterButtons`
- `011fea60` -> `BuildUnionCombineAvailableItemGrid`
- `011ff560` -> `BuildUnionCombineRateInfoUI`
- `01200ea0` -> `UpdateUnionCombineSceneProgress`
- `012012e0` -> `BuildUnionCombineSceneUI`
- `012022e0` -> `BindUnionCombineSceneEvents`
- `01202490` -> `SetUnionCombineSceneRank`
- `012025c0` -> `StartUnionCombineSceneResult`
- `01202700` -> `SelectUnionCombineResultCardByRank`
- `01202880` -> `SelectUnionCombineSceneBackgroundByRank`

## Table functions

- `0164d930` -> `LoadUnionCombineTables`
- `0164db10` -> `ParseUnionCombineItemListCallback`
- `0164db30` -> `ParseUnionCombineRankListCallback`
- `0164db70` -> `ParseUnionCombineItemGroupCallback`
- `0164db50` -> `ParseUnionCombineCeilGroupCallback`
- `0164c930` -> `RegisterUnionCombineBinParser`
- `0164d0b0` -> `ReadUnionCombineItemList`
- `0164cc40` -> `ReadUnionCombineRankList`
- `0164ce60` -> `ReadUnionCombineRewardGroups`
- `0164d340` -> `ReadUnionCombineRewardEntries`

The full source/copy path inventory is in `docs/CopiedFunctionFiles.csv`.
