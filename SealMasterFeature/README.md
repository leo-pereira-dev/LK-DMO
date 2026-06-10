# SealMasterFeature

Isolated reverse-map notes for updating the LK-DMO Seal Master window toward the
newer Global DMO layout shown in the reference screenshot.

## Scope

- Active client window: `dmo-client-main/DProject/_Interface/Game/SealMaster.*`
- Runtime content model: `SealMasterContents.*`
- Table loader: `dmo-client-main/LibProj/CsFileTable/Master_CardMng.*`
- Local reverse-map notes and generated data under this feature folder are the
  active references for Seal Master work.

## Findings

- The current client already calculates a per-seal grade/tier in
  `cSealMasterContents::CalculationGrade`.
- The official table family uses `SealPlateImage` rows to map each grade to a
  different card back image under `Data\Interface\SealMaster\SealBack`.
- The current active UI has no search edit box, while the newer visual target
  has a search box in the top-left of the card panel.
- The stat dropdown already exists and uses the official
  `SealMaster\btn_combobox.tga` and `SealMaster\dropdown_list_0` assets.

## Applied Direction

- Keep the existing Seal Master packet/data flow intact.
- Add a client-side search filter by seal name.
- Force the card back image through the official grade-to-asset mapping when
  rendering cards, so the color changes with tier progression.
- Apply the same grade-to-card visual mapping to the extra seal inventory and
  show a detailed seal tooltip for inventory hover.
- Leave the existing map/favorite/active/stat filters in place.
