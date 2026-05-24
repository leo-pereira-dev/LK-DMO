# Original Window Map

## Identity

The screenshot is a Digital Draw/Lucky Draw variant using the already-known
`DigiSummonUI` visual runtime.

- Runtime UI name found in decompiled UI path: `DigiSummonUI`
- Exclusive table path found in EXE data: `DM_Luckydraw_Info-LuckyDrawInfo.bin`
- Exclusive table RTTI: `CLuckyDrawTable`
- User-facing title in screenshot: `Selecione o ticket que deseja utilizar.`

## Reused visual classes

- `CDigiSummonMainUI`
- `CDigiSummonContents`
- `CDigiSummonBaseUI`
- `CDigiSummonBottomUI`
- `CDigiSummonTicketSelectUI`
- `CDigiSummonRateInfoUI`
- `CDigiSummonResultUI`
- `CDigiSummonSceneUI`
- `CDigiSummonSelectUI`

## Class sizes

- Main UI: `0xD8`
- Bottom UI: `0x94`
- Select UI: `0x1E8`
- RateInfo UI: `0x80`
- Result UI: `0x11C`
- Scene UI: `0xA8`
- TicketSelect UI: `0xAC`

## Main UI pointers

```text
0xA8 -> contents
0xAC -> bottom UI
0xB0 -> active content
0xB4 -> select UI
0xB8 -> rate info UI
0xBC -> result UI
0xC0 -> scene UI
0xC4 -> ticket select UI
```

## Visible screenshot pieces

- top title text
- two gold separator lines
- centered carousel with three visible ticket cards
- left/right arrow buttons
- each card has:
  - title line
  - large product image
  - possible-rank text
  - rank icons
  - small magnifier/info button

The fixed UI textures for these pieces are in
`assets/manifest/RequiredAssetsTablesAndSounds.csv`. The per-ticket card art is
dynamic; the copied DigiSummon flow uses `Random_box\Slot_image\` plus a table
icon path. For Digital Draw, that equivalent data is expected from
`DM_Luckydraw_Info-LuckyDrawInfo.bin`, which is referenced by the EXE but absent
from the current `Dcommerce` extraction.
