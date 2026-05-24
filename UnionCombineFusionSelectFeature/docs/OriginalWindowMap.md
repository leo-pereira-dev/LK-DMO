# Original Window Map

## Runtime identity

- Runtime window name: `UnionCombineUI`
- Main class: `CUnionCombineMainUI`
- Register class: `CUnionCombineRegisterUI`
- Rate info class: `CUnionCombineRateInfoUI`
- Result class: `CUnionCombineResultUI`
- Scene class: `CUnionCombineSceneUI`

## Main UI composition

Known offsets copied from the decompiled map:

- class size: `0x144`
- active content pointer: `0xAC`
- register UI pointer: `0xB0`
- rate info UI pointer: `0xB4`
- result UI pointer: `0xB8`
- scene UI pointer: `0xBC`

## Visible register layout

- material rows: `5`
- material slots per row: `4`
- total material slots visible: `20`
- each row uses a right arrow and one card-front result texture
- right item panel title text key: `DIGICOMBINE_POSSIBLE_ITEM`
- title text key: `DIGICOMBINE_TITLE`

Button rectangles from the Union register function:

| Button | Rect |
| --- | --- |
| add/cancel | `x=0x280 y=0x2C0 w=0x8C h=0x28` |
| reward | `x=0x366 y=0x247 w=0x55 h=0x1E` |
| rate info | `x=0x0C2 y=0x2BC w=0x78 h=0x32` |
| auto register | `x=0x280 y=0x26C w=0x8C h=0x28` |
| combine | `x=0x32A y=0x26C w=0x8C h=0x28` |
| exit | `x=0x32A y=0x2BC w=0x96 h=0x32` |

## Rank filters

The visible filter list is driven by `DM_UnionCombine-RankList`:

| Button | Meaning |
| --- | --- |
| `Todos` | all ranks |
| `N` | rank key `1` |
| `A` | rank key `2` |
| `A+` | rank key `3` |
| `S` | rank key `4` |
| `S+` | rank key `5` |
| `SS` | rank key `6` |
| `SS+` | rank key `7` |

This rank set matches the new screenshot and differs from the older
`DigiCombineFeature` map.
