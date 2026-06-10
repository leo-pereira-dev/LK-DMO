# 2026-06-08 - Digivice Jogress/Xros Dedicated Slot

## Goal

Match the GDMO Digivice behavior where normal ChipSet slots and the
Jogress/Xros chip slot are separate internal groups.

The legacy client already had the normal ChipSet grid. The missing part was a
dedicated EvoChip/Jogress slot, so Jogress/Xros chips were being treated like
normal chips or were not mirrored correctly between the real inventory and the
Tamer Information Digivice tab.

## Contract

```text
Normal ChipSet container: 4000
Jogress/Xros EvoChip container: 3000
Normal chip rule: Type=52 and SkillCode!=2500245
Jogress/Xros rule: Type=52 and SkillCode=2500245
```

The active `ClientDist1\Data\Pack03` BINs keep Jogress/Xros chips as
`Type=52/TypeN=0`; the old `TypeN!=0` split does not match the live Pack03
data.

`CI_EVOCHIP` is only an internal client drag/drop type used to route container
`3000`. It does not change the visual mouse cursor.

## Client Changes

- Added one EvoChip entry to `Data_Digivice`.
- Read EvoChip data from the initial info packet after the 8 normal ChipSet
  entries and before the equipped Digivice payload.
- Added a dedicated Digivice-tab UI slot at `CsPoint(182,450)` using:
  `TamerStatus_New\\Tamer_C_slot_Jogress.tga`.
- Kept normal ChipSet rendering and movement on container `4000`.
- Routed Jogress/Xros chip drag/drop through container `3000`, separate from the
  normal ChipSet grid.

Touched client files:

```text
C:\0-NewDmo\dmo-client-main\DProject\CCursor.h
C:\0-NewDmo\dmo-client-main\DProject\CCursor.cpp
C:\0-NewDmo\dmo-client-main\DProject\DataMng.h
C:\0-NewDmo\dmo-client-main\DProject\DataMng.cpp
C:\0-NewDmo\dmo-client-main\DProject\Data_Digivice.h
C:\0-NewDmo\dmo-client-main\DProject\Data_Digivice.cpp
C:\0-NewDmo\dmo-client-main\DProject\DigimonUser.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\00.Contents\MainFrame_Contents.h
C:\0-NewDmo\dmo-client-main\DProject\_Interface\00.Contents\MainFrame_Contents.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\14.MainFrameUI\TamerStatusUI_Digivice.h
C:\0-NewDmo\dmo-client-main\DProject\_Interface\14.MainFrameUI\TamerStatusUI_Digivice.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\Inventory.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\QuickEvol.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ToolTip_Evol.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameReceive.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameSend.cpp
```

## Server Changes

- `InitialInfoPacket` now serializes `JogressChipSet.ToArray()` immediately
  after `ChipSets.ToArray()`.
- `ItemMovePacketProcessor` now validates normal ChipSet vs Jogress/Xros chip
  separately:
  - inventory to normal ChipSet accepts only `Type=52/SkillCode!=2500245`;
  - inventory to EvoChip accepts only `Type=52/SkillCode=2500245`;
  - normal ChipSet and EvoChip moves do not share the same target slot.

Touched server files:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\GameServer\InitialInfoPacket.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\ItemMovePacketProcessor.cs
```

## Build And Deploy

Client x64 build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=X64_WinDX9_Bridge /p:Platform=x64 /v:minimal
```

Runtime copied to:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Server build:

```powershell
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln" --no-restore
```

VPS deploy:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1
```

Deploy result:

```text
Date: 2026-06-08
Services: account, character, game, routine, api, admin
Archive: .codex-tmp\dmo-server-main-postgres-20260608-130112.tar.gz
Archive size: 91.7 MB
Remote backup: /home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-160131
Postgres: not deployed or recreated
```

Post-deploy check:

```text
account, character, game, routine, api, admin: Up
dmo-postgres: created 2026-05-25, healthy, restart count 0
Recent logs: no ERR/FTL/Unhandled/Exception matches in app services; known DUnit XML content warnings may still appear in game logs
```

## Same-Day Follow-Up

- The Digivice layout must remain the global-style layout: 8 normal ChipSet
  slots plus 1 dedicated Jogress/Xros slot. Do not add extra normal slots while
  investigating item equip failures.
- R17/R18 normal ChipSet equip debugging later in the session was a tooltip and
  stat-attribution issue, not a reason to change the Digivice slot count.
- ChipSet scan RNG remains separate from GM Make Item behavior. Normal scan
  output should keep RNG values; GM-created completed ChipSets can be forced to
  their max rolled values without changing the scan flow.
- Family ChipSets must keep the Digimon family binding (`NSP`, `DR`, `ALL`,
  etc.). Attributes such as Vaccine/Data/Virus are separate metadata and should
  not replace family binding in tooltip or equip validation.

## Test Focus

- Open Tamer Information with `C`.
- Check that the Digivice tab has a dedicated Jogress/Xros chip slot.
- Confirm normal ChipSet items still occupy only the normal ChipSet grid.
- Confirm Jogress/Xros chips can only occupy the dedicated slot.
- Reopen the Tamer Information window and compare it with the real inventory to
  verify the mirrored item state.
