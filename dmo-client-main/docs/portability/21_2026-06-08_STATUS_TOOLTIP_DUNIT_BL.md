# 2026-06-08 - Status Tooltip, DUnit Source, CT, And BL

## Goal

Fix the Digimon stat tooltip source attribution without changing the server stat
contract or the normal equipment/scan rules.

The visible symptom was a red equipment row appearing even when no equipment was
worn. CT showed a phantom equipment bonus, and BL could show negative or
otherwise impossible equipment residuals.

## Client Changes

- `ToolTip_Stat.cpp` no longer displays unresolved leftover stat deltas as
  equipment rows.
- Unresolved attribution is logged as `STAT_TOOLTIP_UNRESOLVED` for later
  investigation.
- DUnit/Unit stat source rows use `Union\Filter_icon.png`, matching the original
  pseudo-code icon reference.
- CT attribution can use the authoritative server total as the tooltip base so
  the tooltip does not invent an equipment row when no equipment is equipped.
- BL converts the authoritative total to basis points inside tooltip math only.
- Equipment attribution now has a guarded server-detail fallback: if
  `TamerDetail*` contains a positive bonus that the legacy local equipment
  formula does not show, the tooltip displays that missing detail as an
  equipment row only when tamer equipment is equipped.

Touched client files:

```text
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ToolTip_Stat.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\00.Contents\MainFrame_Contents.cpp
```

Important log markers:

```text
STAT_TOOLTIP_SERVER_DETAIL_EQUIP
STAT_TOOLTIP_UNRESOLVED
```

## Server Contract

BL stays as integer percent on the server and in combat.

Do not change the server BL model to basis points unless the main client status
display and combat comparison are changed at the same time. The current correct
state is:

```text
Server/main BL display: integer percent
Tooltip-only BL math: basis points after local conversion
Combat BL comparison: integer percent threshold
```

The intermediate server basis-point experiment was reverted before the final
VPS deploy.

## Scope Guardrails

- Normal ChipSet scan RNG was not changed by this tooltip fix.
- GM Make Item behavior can create completed ChipSets with max rolled values,
  but that must not force normal scan results to max.
- Family ChipSets must keep family binding (`NSP`, `DR`, `ALL`, etc.).
  Vaccine/Data/Virus are attributes and must not replace family binding.
- The Digivice layout remains 8 normal ChipSet slots plus 1 dedicated
  Jogress/Xros slot.

## Validation

Client build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=X64_WinDX9_Bridge /p:Platform=x64 /v:minimal
```

Runtime copied to:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Verified runtime:

```text
Built: C:\0-NewDmo\Release\GSP_x64\lk-dmo-x64.exe
Runtime: C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
Timestamp: 08/06/2026 13:17:10
Length: 13686272
```

Server build:

```powershell
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln" --no-restore
```

Final VPS deploy:

```text
Archive: .codex-tmp\dmo-server-main-postgres-20260608-130112.tar.gz
Archive size: 91.7 MB
Remote backup: /home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-160131
Services: account, character, game, routine, api, admin
Postgres: not deployed or recreated
Post-deploy grep: no ERR/FTL/Unhandled/Exception matches in app services
```
