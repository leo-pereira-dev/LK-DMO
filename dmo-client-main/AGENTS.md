# AGENTS.md — DMO Client Build & Development Guide

**Project:** DMO Client (Release_English, v487)  
**Repository:** `F:\Juegos\DMO\client-repo`  
**Runtime:** `F:\Juegos\DMO\Client v487`  
**Last Updated:** 2026-05-13

---

## Quick Start

### Build the Client
```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" `
  "F:\Juegos\DMO\client-repo\DProject.sln" `
  /p:Configuration=Release_English `
  /p:Platform=Win32 `
  /m /verbosity:minimal
```

### Copy to Runtime Directory
```powershell
Copy-Item "F:\Juegos\Release\GSP\DProject_R_ENG.exe" `
  "F:\Juegos\DMO\Client v487\DProject_R_ENG.exe" -Force
```

### Launch
Run `DProject_R_ENG.exe` from `F:\Juegos\DMO\Client v487\` (NOT from the build output directory). The client expects `Data\`, `mss32.dll`, and other runtime assets in its working directory.

---

## Critical Build Notes

### 1. MSVC Toolset Version
The solution uses **v143** (VS2022). If you upgrade the toolset, verify that:
- `DProject.vcxproj` retains `ImageHasSafeExceptionHandlers=false` for Release_English|Win32
- The `/OPT:ICF` linker optimization does NOT strip `NiMilesAudioSDM::Init`

### 2. The `mss32.lib` Import Library
**DO NOT** regenerate `mss32.lib` with `dlltool` or third-party tools. The MSVC linker (`link.exe`) requires import libraries in **MSVC short import format** to generate proper PE Import Address Table (IAT) entries.

If you must regenerate it (e.g., `mss32.dll` changed):
```cmd
dumpbin /exports mss32.dll > exports.txt
# Parse exports.txt into a .def with ordinal hints
lib.exe /def:mss32_ordinal.def /machine:ix86 /out:mss32.lib
```

**Always verify with:**
```cmd
dumpbin /imports DProject_R_ENG.exe | findstr mss32
```
The output MUST show `mss32.dll` with its imports. If absent, the client will crash during audio initialization.

### 3. Missing `UITextSafe.h`
Commit `8224493` references `../Base/UITextSafe.h` which was never committed. A stub exists at:
```
DProject/_Interface/Base/UITextSafe.h
```
This is a **minimal passthrough implementation**. If the upstream author provides a real implementation, replace the stub.

### 4. Forced Windowed Mode
`Resist.cpp` forces `s_bFullScreen = false` unconditionally. The fullscreen resolution assert was also removed to support non-standard desktop resolutions.

---

## Runtime Dependencies

The following MUST be present in `F:\Juegos\DMO\Client v487\`:

| File | Purpose |
|------|---------|
| `mss32.dll` | Miles Sound System audio engine |
| `Data\NP\Sound\*.asi, *.flt` | Miles audio codecs and filters |
| `Data\Pack01.hf / .pf` | Game data pack (headers + content) |
| `Data\Pack03.hf / .pf` | Additional game data |
| `d3dx9_43.dll` | DirectX 9 extensions |
| `d3d9.dll` | DirectX 9 runtime (system) |
| `dmo.ini` | Client configuration (IP, resolution, etc.) |

**Note:** `mss32.dll` in `Client v487\` and `dmobrasil\client\` are **identical** (same MD5: `D560E68431D937CB4681612C741D639D`).

---

## Audio Subsystem Initialization Order

1. `wWinMain` → `App::CWin32App::Run`
2. `App::CGameApp::OnInitialize`
3. `CMngCollector::ShotInit`
4. `CSoundMgr::GlobalInit` → `CSoundMgr::Init`
5. `CSoundMgr::_InitMiles`
6. `NiMilesAudioSystem::Startup("Data\\NP\\Sound")`
7. `AIL_set_redist_directory` → `AIL_quick_startup`

**Crash points:**
- Before fix: `NiMilesAudioSystem::Startup+0xf` (null `this` / missing IAT)
- After fix: Works if `Data\NP\Sound\` exists and `mss32.lib` is correct

---

## Project Structure

```
client-repo/
├── DProject.sln              # Main solution
├── DProject/
│   ├── DProject.vcxproj      # Main project
│   ├── main.cpp              # Entry point + NiMilesAudioSDM linker pragma
│   ├── Resist.cpp            # Resolution/windowed settings
│   ├── CSoundMgr.cpp         # Audio manager
│   └── _Interface/           # UI code
├── common_vs2019/            # Shared static library (protocol, data parsers)
│   ├── common.sln
│   └── Protocol/             # 42+ protocol headers (C++)
├── nlib/                     # Networking library (ASIO-based)
│   ├── client.cpp            # TCP client with ASIO
│   └── packet.cpp            # Packet read/write (NO bounds checking!)
├── Gamebryo2.3/              # Gamebryo engine libs
│   └── GB_Lib/ReleaseLib/    # NiSystem, NiMain, NiMilesAudio, etc.
└── LibProj/
    └── MilesSound/
        ├── lib/
        │   └── mss32.lib     # IMPORT LIBRARY — see critical notes above
        └── include/
            └── mss.h         # Miles SDK headers
```

---

## Known Issues (Not Yet Fixed)

### Security / Stability
These are documented in detail in `BUILD_FIXES.md` and should be addressed before production deployment:

| Priority | Issue | File |
|----------|-------|------|
| CRITICAL | `cPacket::pop()` has no bounds checking | `nlib/packet.cpp:70-80` |
| CRITICAL | nProtect/Xigncode buffer overflows | `cNetwork.cpp`, `cCliGameSend.cpp` |
| CRITICAL | Cash Shop stack overflow | `cCliGameShop.cpp:415-421` |
| HIGH | Channel info OOB array write | `cCliGameReceive.cpp:53-65` |
| HIGH | `cClient` destructor race (detach vs join) | `nlib/client.cpp:170-202` |

### Protocol Mismatch
The client and server use manually-implemented protocols (no protobuf). Any change to packet structures in `common_vs2019/Protocol/` must be mirrored in the server C# codebase at `server/src/Source/Domain/DigitalWorldOnline.Commons/Packets/`.

---

## Git Workflow

### Before Building
```bash
git status                    # Check for local changes
git stash push -u -m "wip"   # Stash if needed
git pull                      # Get latest
git stash pop                 # Restore local changes
```

### Files That Should NOT Be Committed
- `LibProj/MilesSound/lib/*.exp`
- `LibProj/MilesSound/lib/*.def` (except `mss32_ordinal.def` as reference)
- `LibProj/MilesSound/lib/mss32.dll` (runtime DLL, not source)
- Build outputs: `Release/`, `Lib/`, `Release_DLL/`

### Files That SHOULD Be Committed (if modified)
- `DProject/DProject.vcxproj`
- `DProject/Resist.cpp`
- `DProject/main.cpp`
- `DProject/_Interface/Base/UITextSafe.h`
- `LibProj/MilesSound/lib/mss32.lib` (if regenerated)

---

## Troubleshooting

### "The procedure entry point AIL_xxx could not be located in mss32.dll"
The import library `mss32.lib` does not match the runtime `mss32.dll`. Regenerate `mss32.lib` from the exact DLL in `Client v487\`.

### Client crashes immediately with no window
Check `logs/crash.log` in the runtime directory. If it shows `NiMilesAudioSystem::Startup`, the IAT is broken (see Critical Build Notes).

### Client asserts on resolution
The assert at `Resist.cpp:106` was removed. If it reappears, the code was reverted.

### Build fails with `UITextSafe.h` not found
The stub header is at `DProject/_Interface/Base/UITextSafe.h`. If missing, recreate it.

### Linker errors for `__imp__AIL_*`
`mss32.lib` is corrupted or has the wrong symbol format. Use the pre-built `mss32_final.lib` or regenerate with `lib.exe` using ordinal-based `.def`.

---

## Contact / Context

This client is being adapted to run with custom server files (the `server/` directory at `F:\Juegos\DMO\server`). The original `dmobrasil/` client works with its own server; this repo is the development branch for cross-compatibility.

## Server Startup & Restart (CRITICAL)

**ALWAYS** restart servers using `start_servers.bat` at `F:\Juegos\DMO\server\`.  
**DO NOT** kill or start individual server processes directly — the .bat handles cleanup of stale processes, rebuilds from source, and starts all 6 servers in the correct order:

Run `F:\Juegos\DMO\server\start_servers.bat` to start all 6 servers:
- Account (7030), Character (7050), Game (7608), Routine, Api, Admin

The .bat kills existing processes before starting new ones. All servers must be running for the client to function.

## Known Server Issues (Fixed)

### Async Fire-and-Forget Bug
All three TCP servers (`AuthenticationServer`, `CharacterServer`, `GameServer`) had `OnDataReceivedEvent` calling `ProcessPacketAsync()` without `await`. This swallowed any exception after the first `await`, making the server appear to hang. Fixed in all three by introducing `ProcessPacketSafeAsync`.

### MediatR Assembly Registration
`Account.Host` and `Character.Host` registered `AddMediatR` with the wrong assembly, causing handlers from `DigitalWorldOnline.Application` to not be found. Fixed by scanning `typeof(AccountByUsernameQuery).Assembly` and `typeof(CharactersByAccountIdQuery).Assembly` respectively, plus adding missing `*BinLoader` singletons.

## Known Client Issues (Unfixed)

### Tamer.bin Model Table Incomplete
The client's `Data\Tamer.bin` only contains models up to **80010** (Sora). Characters with Tamers-season models (Takato=80011, Rika=80012, Henry=80013, etc.) cause assertion failures at `Tamer.cpp:83` because `IsTamer(nFileTableID)` returns false for those IDs.

**Workaround:** Only create characters using models 80001–80010 (Adventure 01 cast).
**Proper fix:** Source or generate a `Tamer.bin` that includes models 80011–80020.

### Magenta Icons (Event Notice, Season Pass)
UI elements render as magenta squares — the client's data files for these features are missing or incomplete. Not a server issue.

## Known Server Issues (Gameplay)

### Melee Attack Animations
`PartnerAttackPacketProcessor` lacks handler masking (`&= 0x7FFFF`). Mob lookups fail because the client sends handlers with type bits encoded. Fix: add masking matching `PartnerSkillPacketProcessor`.

### Digivolution Requirements
Boolean logic bug in `PartnerEvolutionPacketProcessor`: uses `&&` instead of `||`, allowing evolutions to bypass level/DS/item requirements.

### Skill Crash
`PartnerSkillPacketProcessor` has multiple NRE paths from unguarded `FirstOrDefault` chains. Fix: add null-checks and try-catch.

### Cash Shop Delivery
Purchased items go to the Account Cash Warehouse, not the player's inventory. Items must be manually withdrawn. By design, but may be confusing.

For full technical details, see `BUILD_FIXES.md` in the workspace root.
---

## DMO Bug Investigation Rules (CRITICAL)

For every DMO bug, explicitly use this checklist before proposing or implementing a fix:

- Packet mismatches
- Race conditions
- Null checks
- Game logic
- Pipelines
- Overflow

When the issue involves client/server contracts, packets, loading, map transitions, UI rendering, movement, mobs, or crashes, the unpacked/decompiled client dump at `F:\Juegos\DMO\unpacked_exe_all` is a required source of truth. Compare the dump's client behavior against client source, server source, database/bin assets, and live logs before deciding root cause.

Do not treat the dump as optional context. For packet and lifecycle bugs, start by finding the relevant pseudocode/functions in the dump and mapping them to the server flow.
