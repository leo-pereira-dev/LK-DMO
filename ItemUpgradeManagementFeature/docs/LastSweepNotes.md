# Last asset sweep notes

Date: 2026-05-23

This sweep rechecked the `EquipmentUpgradeSystem` / `DataTradeDigimonUpgrade`
asset set against official decompiled strings and xrefs.

## Result

The direct xref-proven manifest is complete for the copied function set:

- required unique asset/sound paths: 27
- required copied-function xref rows: 81
- required assets also referenced by adjacent functions: 78
- required assets without xref: 0

Two visible screenshot assets were missing from the first manifest because they
are official strings in the dump but have no direct xref row inside the copied
function set:

- `EquipSystem\ArrowBtn_Down.tga`
- `EquipSystem\ArrowBtn_Up.tga`

They are now recorded in:

```text
assets/manifest/AdditionalKnownAssetStrings.csv
```

The raw sweep table is:

```text
docs/LastSweep_AssetXrefs.csv
```

## Excluded adjacent assets

The sweep also saw nearby `EquipSystem` strings that belong to adjacent systems,
mostly `cElementManagement` / normal charge, not to the screenshot window:

- `EquipSystem\Recharge_slot.tga`
- `EquipSystem\Recharge_gauge.tga`
- `EquipSystem\Recharge_win.tga`
- `System\Btn_Nor13_R.tga`
- `Item_Make\Make_ani_gauge_bg.tga`
- `Item_Make\Make_ani_Sequence.dds`

Those are intentionally separated in:

```text
assets/manifest/AdjacentOrExcludedAssetStrings.csv
```

