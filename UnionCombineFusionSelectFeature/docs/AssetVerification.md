# Asset Verification

The asset list was generated from the exact copied decompiled functions using
Ghidra data xrefs:

- raw string xrefs: `assets/manifest/DecompiledXrefStringReferences.csv`
- filtered assets/sounds: `assets/manifest/RequiredAssetsAndSounds.csv`
- non-asset text/class keys: `assets/manifest/TextAndClassReferences.csv`

## Important visible assets

- `Random_box\bg.png`
- `Random_box\button\done.tga`
- `Random_box\button\Purchase_1.tga`
- `Random_box\button\Repurchase.tga`
- `Cash\NewCashShop\Store_btn_G1.tga`
- `Unionmon_Ev_UI\Ev_Btn.tga`
- `CommonUI\arrow_Right.tga`
- `CommonUI\Digimon_rank_new.dds`
- `CommonUI\Unionmon_rank.dds`
- `CommonUI\Slot\Windows_Scroll2.tga`
- `Control_G\Popup\N2Dlg5.tga`
- `DigiCombine\DigiCombine_Slot_Empty.dds`
- `DigiCombine\DigiCombine_Slot_rank.dds`
- `Random_box\gauge.png`
- `SealMaster\sealmaster_highlight.tga`
- `UnionCombine.mp3`

## Scene/result assets

The UnionCombine scene code reuses the `DigiCombine\DigiCombine_Gacha_*`
texture family and `System\DigiCombine_Gacha_*.wav` sounds. The adjacent
UnionCombine scene entry function `010a78d0` starts `UnionCombine.mp3`, so the
BGM is also recorded in `RequiredAssetsAndSounds.csv`. The copied selection
functions contain extra rank branches (`SSS`, `U`, etc.) even though the
official `DM_UnionCombine-RankList.xml` used by this screenshot has only seven
ranks (`N` through `SS+`). They are kept in the manifest because the decompiled
code references them directly.

## Pack note

The interface files are Pack01 runtime assets:

```text
ClientDist\Data\Pack01.hf
ClientDist\Data\Pack01.pf
```

This subproject records the official runtime paths; it does not extract or
duplicate packed textures.
