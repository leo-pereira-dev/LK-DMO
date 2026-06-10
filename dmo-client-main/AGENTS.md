# AGENTS.md - DMO Client Build & Development Guide

## Scope

Work from this client repository only:

```text
C:\0-NewDmo\dmo-client-main
```

Do not use historical external paths as active references. If an old note points
outside `C:\0-NewDmo`, treat it as obsolete unless the user explicitly provides
a current local replacement inside this workspace.

## Build

Default client build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=X64_WinDX9_Bridge /p:Platform=x64 /v:minimal
```

Expected output:

```text
C:\0-NewDmo\Release\GSP_x64\lk-dmo-x64.exe
```

Copy the built executable to the local runtime that is being tested:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Build through `DProject.sln` only. Do not build `DProject.vcxproj` directly for
normal testing. Do not use `Release_English|x64` as the x64 default because the
solution maps several projects back to Win32; use `X64_WinDX9_Bridge|x64` for a
real x64 build.

## Active Runtime Data

Use local runtime data only:

```text
C:\0-NewDmo\ClientDist\Data
C:\0-NewDmo\ClientDist1\Data
C:\0-NewDmo\ClientDist1_x64\Data
```

For BIN sync, compare the server BIN folder with the active local client pack or
generated local source folder:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english
C:\0-NewDmo\TamerBinMigrationFeature\generated\pack03-source
C:\0-NewDmo\ClientDist1\Data\Pack03
C:\0-NewDmo\ClientDist1_x64\Data\Pack03
```

Patch only the active local `Pack03.pf/.hf` pair when the client pack needs the
same files.

## Current Status Tooltip Notes

Recent stat-tooltip work fixed source attribution for Digimon stats without
changing the server stat contract.

- Do not synthesize a red equipment row for leftover/unresolved stat
  differences. Current client code logs `STAT_TOOLTIP_UNRESOLVED` instead.
- DUnit/Unit tooltip rows use `Union\Filter_icon.png`, matching the original
  pseudo-code icon reference for Union/DUnit source rows.
- CT tooltip base can resolve from the authoritative server total to avoid a
  false equipment residual row when no equipment is worn.
- BL remains an integer-percent stat in the server and in the main client
  status display. The tooltip converts the authoritative BL total to basis
  points only for internal tooltip math. Do not change server BL/combat to basis
  points unless the main client BL display and combat comparison are changed at
  the same time.

## Current Live Incident Notes

Recent local work focused on players in the same channel/location not seeing
each other, map/combat freezes, channel-change failures, bad stats, and crashes
around packets `3969`, `1006`, and `1308`.

Client-side packet/object findings:

- `common_vs2019/pType.h` and `common_vs2019/pType.cpp`: `cType::SetTypeAll`
  and `GetTypeAll` must use `u8`. `pop<cType>` reads/writes `u8`; truncating to
  `u4` dropped high object/model bits and caused object sync/creation failures.
- `DProject/network/cCliGameSync.cpp`: sync/crash logs exist around
  `RecvSyncData`, `SyncNewObject`, `SyncInObject`, `SyncInTamer`,
  `SyncInDigimon`, and `SyncInMonster`.
- `DProject/network/cCliGateReceive.cpp`: packet 1308 /
  `RecvTamerSelectSuccess` parse logging exists.
- Current server `UpdateStatusPacket` writes partner AT as `int32`; keep the
  client `RecvAllStat` reader and server writer aligned before changing stats.

## Runtime Dependencies

The local runtime directory used for launch must contain:

| File | Purpose |
|------|---------|
| `mss64.dll` | x64 Miles Sound System runtime/stub used by the x64 bridge |
| `Data\NP\Sound\*.asi, *.flt` | Miles audio codecs and filters |
| `Data\Pack01.hf / .pf` | Game data pack |
| `Data\Pack03.hf / .pf` | English tables/data pack |
| `d3dx9_43.dll` | DirectX 9 extensions |
| `d3d9.dll` | DirectX 9 runtime |
| `PathEngine.dll` | x64 PathEngine runtime loaded by the client |
| `dmo.ini` | Client configuration |

## Critical Build Notes

### MSVC Toolset

The solution uses v143. If you change the toolset, verify:

- `DProject.vcxproj` retains `ImageHasSafeExceptionHandlers=false` for
  `Release_English|Win32`.
- `/OPT:ICF` does not strip `NiMilesAudioSDM::Init`.

### Miles Import Library

Do not regenerate `mss32.lib` with `dlltool` or third-party tools. The MSVC
linker requires import libraries in MSVC short import format.

If it must be regenerated from the local runtime DLL:

```cmd
dumpbin /exports mss32.dll > exports.txt
lib.exe /def:mss32_ordinal.def /machine:ix86 /out:mss32.lib
```

Verify the built EXE imports `mss32.dll`:

```cmd
dumpbin /imports DProject_R_ENG.exe | findstr mss32
```

### UITextSafe

The stub header exists at:

```text
DProject/_Interface/Base/UITextSafe.h
```

If a real implementation is added later, replace the stub carefully.

## Known Client Issues

### Tamer.bin Model Table

Older client data only had models through `80010`. Current local migration work
adds later tamers through local BIN conversion; verify the active pack and
server BINs before assuming a model is missing.

### Magenta Icons

Magenta UI icons usually mean the active client data files are missing or
incomplete for that feature. Check local `Pack03`/asset data before changing
server logic.

## Bug Investigation Checklist

For every DMO bug, explicitly check:

- Packet mismatches
- Race conditions
- Null checks
- Game logic
- Pipelines
- Overflow

When the issue involves client/server contracts, packets, loading, map
transitions, UI rendering, movement, mobs, or crashes, compare local client
source, local server source, active local BIN data, and local logs before
deciding root cause.
