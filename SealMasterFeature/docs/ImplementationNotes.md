# Implementation Notes

## Current Client

- `cSealMasterContents::CalculationGrade()` maps seal amount to `nCardGrade`
  using the per-seal `s_stGradeInfo[*].s_nMax` thresholds.
- `cSealMaster::sSealCardControl::SetSealGrade()` changes the card-back sprite
  texture, but it trusts whatever path the current table returns.
- `SetFilter()` combines map/favorite/active-count/stat filters. The update adds
  name search as one more client-side visibility condition.
- `cExtraInventory` has a dedicated seal-card renderer for the extra seal bag.
  It now combines activated Seal Master count with the stack count and uses the
  same full `Data\Interface\SealMaster\SealBack\...` tier backgrounds.
- Seal inventory hover now routes to `cTooltip::SEALMASTER` with a detailed
  inventory-specific layout: item kind/class/quantity, tier progress, seal stack,
  activated count, total count, item description, and current tier effect.

## Official Table Paths

`SealPlateImage.xml` confirms this grade visual family:

- `sealmaster_bg_normal.dds`
- `sealmaster_bg_bronze.dds`
- `sealmaster_bg_silver.dds`
- `sealmaster_bg_master.dds`
- `sealmaster_bg_platinum.dds`
- `sealmaster_bg_master2.dds`

The active patch now uses the full official interface paths
`Data\Interface\SealMaster\SealBack\...` because `ChangeTexture(..., false)`
resolves these card backs from the client data path instead of the shorter
relative folder.
