# Item bin migration feature

Last updated: 2026-05-24

Purpose: adapt updated global/GDMO item data into the older LK-DMO client and
server template.

Applied fixes:

- Merge `SkillCodeItem.bin` effects into legacy `Skill.bin`.
- This covers consumable effects, item buffs, food, EXP items, and many
  item-driven behaviors that failed when the item `SkillCode` was absent from
  `Skill.bin`.
- Convert modern split `ItemData.bin + Item_Str.bin` into the legacy client
  `ItemList.bin` first section, while preserving the old template tail
  sections.
- Load `RandomBoxList.bin`, `RandomBoxItem.bin`, and `RandomBoxReward.bin` as
  server container data while keeping database containers as fallback.

Important state:

- Server already has updated split files from `Dcommerce\Bin\Table`:
  `ItemData.bin`, `Item_Str.bin`, `AccOption.bin`, `AccEnchant.bin`,
  `SkillCodeItem.bin`, `RandomBox*.bin`, etc.
- Client Pack03 uses legacy `ItemList.bin`, now regenerated with 27320 item
  records.
- RandomBox bins exist, but server box opening currently uses `_assets.Container`
  from DB/config, not `RandomBoxList/Item/Reward` directly.

Generated files:

```text
C:\0-NewDmo\ItemBinMigrationFeature\generated\converted\Skill.bin
C:\0-NewDmo\ItemBinMigrationFeature\generated\converted\ItemList.bin
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\skillcodeitem-merge.csv
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\itemlist-modern-to-legacy.csv
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\randombox-container-audit.csv
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\random-box-usable-test-ids.csv
```

Tools:

```text
C:\0-NewDmo\ItemBinMigrationFeature\tools\Merge-SkillCodeItemIntoSkillBin.ps1
C:\0-NewDmo\ItemBinMigrationFeature\tools\Convert-ModernItemDataToLegacyItemList.ps1
```

Server loader:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\Bins\ContainerBinLoader.cs
```

Boot check:

```text
Loaded RandomBox container bins: Boxes=335, ItemGroups=173, RewardGroups=101,
Containers=335, MissingItemGroups=0, MissingRewardGroups=0
```

See:

```text
C:\0-NewDmo\AddItems.md
```
