# Last Sweep Notes

Final verification for the new fusion selection screenshot:

- Confirmed the UI identity as `UnionCombineUI`.
- Confirmed the table family as `DM_UnionCombine-*`.
- Confirmed `DM_UnionCombine-RankList.xml` has 7 records, matching the visible
  filters `N`, `A`, `A+`, `S`, `S+`, `SS`, `SS+`.
- Confirmed `DM_DigiCombine-RankList.xml` has 8 records, so the screenshot is
  not the older `DigiCombineUI` variant.
- Copied 28 decompiled functions:
  - 17 direct UnionCombine UI functions
  - 10 UnionCombine table/bin reader functions
  - 1 adjacent UnionCombine BGM/scene entry function for `UnionCombine.mp3`
- Copied the official UnionCombine XML/BIN table family and supporting item
  lookup XML/BIN files.
- Generated `RequiredAssetsAndSounds.csv` from copied-function xrefs plus the
  direct `UnionCombine.mp3` BGM xref.
- Checked every copied file listed in the CSV inventories exists in this
  subproject.

Important correction from the sweep: this UnionCombine variant directly xrefs
`CommonUI\Digimon_rank_new.dds` and `CommonUI\Unionmon_rank.dds`; it does not
use the older `CommonUI\digimon_rank.dds` path from the previous map.
