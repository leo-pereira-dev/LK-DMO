# DetailInfo / Detalhes

Last updated: 2026-05-25

This file tracks the active implementation of the GDMO-style `Detalhes`
window in the LK-DMO client. The reverse-mapping source of truth remains under
`DetailInfoFeature/`, while this file records what was actually ported into the
main client and what is still pending.

## Current implementation

Main client files:

- `dmo-client-main/DProject/_Interface/14.MainFrameUI/MainFrame_DetailInfoUI.h`
- `dmo-client-main/DProject/_Interface/14.MainFrameUI/MainFrame_DetailInfoUI.cpp`
- `dmo-client-main/DProject/DProject.vcxproj`
- `dmo-client-main/DProject/DProject.vcxproj.filters`
- `dmo-client-main/DProject/_Interface/Game/_BaseWindow.cpp`
- `dmo-client-main/DProject/_Interface/Game/_BaseWindow.h`
- `dmo-client-main/DProject/_Interface/Game/_GameIF.h`
- `dmo-client-main/DProject/_Interface/14.MainFrameUI/TamerStatusUI_Tamer.cpp`

Implemented behavior:

- Added an independent, movable `WT_DETAIL_INFO` window instead of drawing the
  content inside the existing `C`/Tamer Status window.
- Added a `Detalhes` button to the Tamer/equipment page so the new window opens
  from the character equipment screen.
- Added `Tamer` and `Digimon` tabs using the official `System\\Btn_Radio1.tga`
  radio style.
- Rebuilt the Tamer tab layout with the official DetailInfo background assets,
  equipment slots, equipment icons, `Equipamento` and `Selo Mestre` buttons, and
  the right-side `Atributos Obtidos` table.
- Added a 3D tamer render over the same Tamer Status background field used by
  the normal `C` window.
- Wired the Tamer attribute table to client/server status data, including Seal
  Master bonuses and the newer added stat fields that were added to the status
  packet path.
- Added the Digimon tab skeleton with portrait/model render, skill slots,
  enchant rows, and basic Digimon status values.
- The latest build was compiled as `Release_English|Win32` through
  `DProject.sln` and copied to the two local test clients.

## Portrait rule

The Tamer portrait must use the same source as the HUD/target character portrait:

```cpp
pTamer->GetCharImage()->Render_L(...);
```

Do not use `Render_S` for this portrait. The small tamer image is square and
causes the visible box around the face. The large tamer image is already an
`84x84` TGA with the circular alpha/frame baked in, matching the HUD behavior.

Do not overlay `DetailInfo\\TamerStatus_Digimon_Tamer.tga` on top of the Tamer
face. That asset has a dark filled center and will dim or hide the portrait. It
is safe for Digimon-style framing where the source image/layout expects it, but
it was the cause of the broken/dark Tamer portrait during the last pass.

## Assets

The active UI texture paths are relative to the interface pack and must resolve
from Pack01 / `Data\\Interface` at runtime. Keep using paths in the same style
as the rest of the client code, for example:

- `DetailInfo\\TamerStatus_Win_03_bg.tga`
- `DetailInfo\\TamerStatus_Win_04_bg.tga`
- `DetailInfo\\TamerStatus_win_02_bg.tga`
- `DetailInfo\\Storage_M_field_a.tga`
- `DetailInfo\\TamerStatus_Digimon_Tamer.tga`
- `DetailInfo\\Autocrop_ON_Btn.bmp`
- `System\\Btn_Radio1.tga`
- `TamerStatus_New\\TamerStatus_Win_01_bg.tga`

Do not hardcode absolute paths and do not prefix these interface assets with an
extra `Data\\Interface` in the code.

## Backend/status notes

The right-side Tamer table is meant to show attributes obtained by the Tamer,
not Digimon stats. It should aggregate values from equipment, accessories,
seals, buffs/added status, and server-calculated status packet fields. Digimon
stats belong to the Digimon tab and must stay separate.

The status packet route was expanded during this work, but the user/other-player
DetailInfo route is not finished yet. Official references found in the
decompiled client:

- `C2GS_REQ_OTHER_TAMER_DETAIL_INFO`
- `GS2C_RES_OTHER_TAMER_DETAIL_INFO`
- `nsOtherTamerDetailInfo::stTamer`
- `nsOtherTamerDetailInfo::stDigimon`

## Pending work

- Finish the user/other-player DetailInfo flow later. The current window is
  focused on local-player testing and does not yet fully implement the official
  request/response path for inspecting another user's Tamer/Digimon details.
- Continue visual parity for the Digimon tab. The Tamer portrait is now fixed;
  the Digimon tab still needs a calmer pass against the official screenshot.
- Recheck the full Tamer attribute table once the remaining user-detail packet
  work is done, especially AT/AS/CT/HT/SCD/CD/SD/base damage formatting.
- Keep the DetailInfo mapping folder as the reference before further changes:
  `DetailInfoFeature/docs/WindowReconstructionMap.md` and
  `DetailInfoFeature/docs/FunctionChainMap.md`.

## Verification

Last successful client build:

```powershell
$env:CL="/FS"
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /nr:false /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Output copied to:

```text
C:\0-NewDmo\ClientDist\DProject_R_ENG.exe
D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe
```
