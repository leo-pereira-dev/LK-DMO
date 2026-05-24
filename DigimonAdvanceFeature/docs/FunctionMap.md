# Function Map

Inventario completo: `docs/CopiedFunctionFiles.csv`.

## Janela principal `EnchantTactics`

- `01283910` -> `CreateEnchantTacticsWindowFactory`
- `01147380` -> `ResolveDigimonEnchantPageName`
- `011d2a20` -> `OpenDigimonEnchantStatusPanel`
- `011d4320` -> `BuildDigimonEnchantStatusPanel`
- `011f0c90` -> `HandleEnchantTacticsActionDispatch`
- `011f0d50` -> `BuildEnchantAndTranscendActionButtons`
- `011f51b0` -> `HandleEnchantResetFlow`
- `011f6890` -> `BuildEnchantResetButton`
- `012ce6a0` -> `BuildEnchantTacticsMaterialButtonText`
- `012ce800` -> `ShowEnchantTacticsCannotEnchantMessage`
- `012cfc40` -> `BuildEnchantTacticsEnchantLevelText`

O cluster `012ce0b0` ate `012cfb00` foi copiado porque forma a cadeia de
validacao/estado do painel inferior e chama a mensagem `Cannot Enchant`.

## Seletor superior `DigiPowerEnchant`

- `0128c990` -> `CreateDigiPowerEnchantWindow`
- `0125dde0` -> `InitializeDigiPowerEnchantWindow`

Tambem foi copiado o cluster `0125d190` ate `0125ffe0`, que contem a logica de
estado/slots do seletor e algumas funcoes compartilhadas de UI.

## Popups

`EnchantOption`:

- `0128c5d0` -> `CreateEnchantOptionWindow`
- `01371e40` -> `InitializeEnchantOptionWindow`
- cluster `01371450` ate `01371fa0`

`EnchantOptionResultUI`:

- `0128c7f0` -> `CreateEnchantOptionResultWindow`
- `01267760` -> `InitializeEnchantOptionResultWindow`
- cluster `01267070` ate `01267ef0`

`DigimonTranscendenceHelp`:

- `0128bfa0` -> `CreateDigimonTranscendenceHelpWindow`
- `01118d40` -> `InitializeDigimonTranscendenceHelpWindow`
- `01119160` -> `BuildDigimonTranscendenceHelpTabs`
- `01119590` -> `BuildDigimonTranscendenceHelpBodyText`
- cluster `01118720` ate `01118ff0`

## Cena, som e efeitos

- `00f808f0` -> `LoadEnchantRoomMusic`
- `00f81940` -> `LoadEnchantTacticsCardAndAnimationAssets`
- `01087760` -> `PlayEnchantRoomMusicVariantA`
- `010ed3b0` -> `PlayEnchantRoomMusicVariantB`
- `011ee3c0` -> `PlayEnchantRoomMusicVariantC`
- `013121c0` -> `ResolveEnchantRoomMusicPath`
- `014484b0` -> `LoadEnchantLevelEffects`
- `0111c800` -> `PlayTranscendenceSoundEffect`

## Tabelas

- `0161e970` -> `RegisterDigimonAdvanceTables`
- `0161ed10` -> `RegisterDigimonAdvanceTableEvents`
- `0161f0f0` -> `ParseEvoDigitamaTableCallback`
- `0161f0b0` -> `ParseEnchantItemInfoTableCallback`
- `0161f0d0` -> `ParseEnchantStatInfoTableCallback`
- `0161f170` -> `ParseTranscendNeedTableCallback`
- `0161f150` -> `ParseTranscendMaterialTableCallback`
- `0161f110` -> `ParseTranscendCorrectTableCallback`
- `0161f130` -> `ParseTranscendCorrectExTableCallback`

`0161e740` tambem foi copiada porque aparece na validacao de enchant.
