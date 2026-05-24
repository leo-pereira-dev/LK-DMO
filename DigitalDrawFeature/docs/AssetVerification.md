# Asset Verification

Assets and resource/table strings were extracted from the copied decompiled
functions via Ghidra xrefs.

Primary manifests:

- `assets/manifest/DecompiledXrefStringReferences.csv`
- `assets/manifest/RequiredAssetsTablesAndSounds.csv`
- `assets/manifest/TextClassAndPacketReferences.csv`
- `assets/manifest/DynamicDigiSummonComparableCardAssets.csv`

## Fixed visible assets

- `Random_box\bg.png`
- `Random_box\bg_big.png`
- `Random_box\over_frame.png`
- `Random_box\button\arrow_l.tga`
- `Random_box\button\arrow_r.tga`
- `Random_box\button\definite_information.tga`
- `Random_box\button\Purchase_1.tga`
- `Random_box\button\Purchase_10.tga`
- `Random_box\button\Tap.tga`
- `Random_box\popup\rate_popup\rate_popup_`
- `Random_box\Rate_bg_5x5.png`
- `Random_box\frame.png`
- `CommonUI\CommonUI_close.tga`
- `CommonUI\digimon_rank_new.dds`
- `Random_box\sub_title_line.png`
- `Random_box\loding.png`
- `Random_box\Slot_image\`

## Scene assets

The shared scene/result flow reuses `DigiCombine\DigiCombine_Gacha_*` textures
and `System\DigiCombine_Gacha_*.wav` sounds, same visual family seen in the
other union/gacha screens.

## Missing official data

`DM_Luckydraw_Info-LuckyDrawInfo.bin` is directly referenced by the EXE at
`01a1dc40` and loaded by `016828f0`, but no matching XML/BIN file was found
under the current `Dcommerce` extraction. It is documented in
`docs/MissingOfficialTableFiles.csv`.
