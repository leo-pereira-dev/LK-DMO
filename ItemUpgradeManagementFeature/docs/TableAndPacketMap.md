# Table and packet map

## Official tables copied

The core table family is:

- `DM_ItemUpgrade-Data`
- `DM_ItemUpgrade-EtcValue`
- `DM_ItemUpgrade-Transfer`
- `DM_ItemUpgrade-Restore`
- `DM_ItemUpgrade-MaterialGroup`

Support tables copied for item names, icons, ranks, and item-type display:

- `ItemData`
- `ItemType`
- `ItemRankData`
- `ItemRankEffectData`
- `QuestItemData`

Both XML and BIN copies are stored under `tables/xml` and `tables/bin`.

## Official table loader/parser functions

- `LoadItemUpgradeTables.original.c`
- `LoadItemUpgradeDataTable.original.c`
- `ParseItemUpgradeDataGroups.original.c`
- `ParseItemUpgradeDataEntries.original.c`
- `ParseItemUpgradeDataMaterialEntry.original.c`
- `LoadItemUpgradeTransferTable.original.c`
- `ParseItemUpgradeTransferGroups.original.c`
- `ParseItemUpgradeTransferEntries.original.c`
- `ParseItemUpgradeTransferEntry.original.c`
- `LoadItemUpgradeRestoreTable.original.c`
- `ParseItemUpgradeRestoreGroups.original.c`
- `ParseItemUpgradeRestoreEntries.original.c`
- `ParseItemUpgradeRestoreEntry.original.c`
- `LoadItemUpgradeMaterialGroupTable.original.c`
- `ParseItemUpgradeMaterialGroups.original.c`

## Packet/result strings found

Direct packet class/string references are recorded in
`assets/manifest/KnownPacketAndRttiStrings.csv`.

Related official strings without direct xrefs in the copied UI set are recorded
in `assets/manifest/AdditionalKnownOfficialStrings.csv`.

Important strings:

- `C2GS_REQ_ITEM_UPGRADE`
- `C2GS_REQ_ITEM_UPGRADE_DATA_TRANSFER`
- `C2GS_REQ_ITEM_UPGRADE_DATA_RESTORE`
- `GS2C_RES_ITEM_UPGRADE_DATA_TRANSFER`
- `GS2C_RES_ITEM_UPGRADE_DATA_RESTORE`
- `stProcessItemUpgradeResult`

Some of these appear as official RTTI/type strings rather than direct xrefs
inside the copied UI functions, so they are intentionally kept in a separate
manifest.
