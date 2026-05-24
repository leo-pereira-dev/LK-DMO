# Function Map

## UI functions

The UI files in `decompiled/DigitalDrawUI` are copied from the same official
visual family used by `DigiSummonFeature`. They were copied into this isolated
folder because Digital Draw should be ported as a separate feature and not
silently merged with the summon screen.

Key files:

- `0128f070` -> `CreateDigiSummonUiFactoryForDigitalDraw`
- `011b6020` -> `BuildDigitalDrawMainUi`
- `011bb4b0` -> `BuildDigitalDrawTicketCarousel`
- `011bbb20` -> `BuildDigitalDrawTicketCard`
- `011bc4e0` -> `BuildDigitalDrawBottomActions`
- `011b71a0` -> `BuildDigitalDrawRateInfoPopup`
- `011b6e50` -> `BuildDigitalDrawRateInfoWindow`
- `011b7390` -> `BuildDigitalDrawRateInfoScrollList`
- `011b7530` -> `BuildDigitalDrawRateInfoRewardRow`
- `011b7bb0` -> `PopulateDigitalDrawRateInfoRows`
- `011b6d30` -> `HandleDigitalDrawRateInfoPacket`
- `011b8e70` -> `BuildDigitalDrawSceneUi`
- `011bab00` -> `SelectDigitalDrawResultCardByRank`
- `011bac80` -> `SelectDigitalDrawSceneBackgroundByRank`

The full copied-function inventory is in `docs/CopiedFunctionFiles.csv`.

## LuckyDraw table functions

- `016828f0` -> `RegisterLuckyDrawInfoTable`
- `01682ab0` -> `ParseLuckyDrawInfoTableCallback`
- `01682430` -> `ReadLuckyDrawInfoRecords`
- `01682ad0` -> `ReadLuckyDrawInfoRecordFields`
- `016827f0` -> `DestroyLuckyDrawInfoRecordStrings`
- `01682880` -> `DestroyLuckyDrawInfoTable`

These are the functions that make this screen different from a plain
`DigiSummonFeature` clone.

## Rate info subwindow

The `info. de prob.` subwindow from the screenshot is covered by the copied
rate-info chain:

- `011b6e50`: popup shell, title, close button, `rate_popup` frame, and calls
  into the tab/list builders.
- `011b71a0`: visible tab button and random-list tab text.
- `011b7390`: `Rate_bg_5x5` list body and scrollbar area.
- `011b7bb0`: populates row data.
- `011b7530`: renders each row with icon frame, rank/class icon, item text,
  and percentage text.
