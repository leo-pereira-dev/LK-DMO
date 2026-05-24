# Last sweep notes

Date: 2026-05-23

This pass rechecked the exclusive inventory window against the decompiled dump
using the broad markers below:

- `ExtraInventory`
- `ExtraInven`
- `EXTRAINVEN`
- `EXTRA_INVEN`
- `DM_Extra_Inven`
- `EvoExInven`
- `NewInventory`
- `Invensort`
- `SEALMASTER_SHOW`
- `SealMaster`

The raw xref result is stored in:

```text
docs/LastSweep_ExtraInventoryXrefs.csv
```

## Result

No new direct `ExtraInventoryUI` window function was found missing from the
subproject.

The direct window chain is already represented by the copied functions:

- `CreateExtraInventoryUI.original.c`
- `InitializeExtraInventoryUI.original.c`
- `BuildExtraInventoryTabs.original.c`
- `BuildSealInventoryCategory.original.c`
- `BuildTicketInventoryCategory.original.c`
- `BuildEvolutionInventoryCategory.original.c`
- `BuildDigitamaInventoryCategory.original.c`
- `BuildMaterialInventoryCategory.original.c`
- `BuildSealInventoryGrid.original.c`
- `BuildSealOverviewDropdown.original.c`
- `BuildSealStatFilterDropdown.original.c`

The broad sweep produced many `Copied=NO` rows, but those rows group into
adjacent systems rather than missing exclusive-inventory window code:

- full Seal Master screen functions and its own window/background resources;
- generic inventory and preset error messages;
- macro/quick-menu labels that only open or name the exclusive inventory;
- shared `NewInventory` notification widgets used outside this window;
- DetailInfo Seal Master stat helpers already tracked in `DetailInfoFeature`;
- generic filter/tooltip/table formatters.

## Important distinction

`NewInventory\new_inventory_win.tga` is present as an official string and
matches the screenshot frame, but it still does not have a direct xref inside
the copied function set. It remains intentionally recorded in
`assets/manifest/AdditionalKnownUiResourceStrings.csv`.

The xref-proven assets remain in `assets/manifest/RequiredAssets.csv`.

## Current coverage

- copied decompiled functions: 31
- required direct asset rows: 30
- known additional UI strings: 3
- copied XML/BIN table files: 40
- copied language bins: 8

