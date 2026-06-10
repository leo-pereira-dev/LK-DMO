# Patch Notes

## Current LK-DMO Workspace Update

Detailed operational documentation for the latest item/status fixes:

```text
C:\0-NewDmo\dmo-server-main-postgres\docs\2026-06-08_ITEM_STATUS_AND_ACCESSORY_FIXES.md
```

### 2026-06-08 - Sealed Accessory Unseal Fallback

- Fixed newer sealed accessories such as `1310631 - Sealed Zero Unit Necklace`
  not responding to right-click unseal when they are type `170` items without a
  `Container.bin` reward entry.
- `ItemConsumePacketProcessor` now keeps configured containers on the existing
  container path, then falls back only for sealed ring/necklace/earring/
  bracelet names that resolve to a real unsealed accessory with `AccOption`
  data.
- The fallback consumes one sealed item, creates the resolved accessory,
  applies the normal accessory identify/options flow, persists the inventory,
  and reloads inventory for the client. Stacked sealed items require one free
  inventory slot before unsealing.
- BIN coverage check resolved 103/103 sealed accessory type-170 candidates and
  left non-accessory sealed digivice/box items on their normal container/use
  paths.
- Server build passed with no errors. VPS deploy completed with archive
  `.codex-tmp\dmo-server-main-postgres-20260608-155153.tar.gz`; remote backup:
  `/home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-185210`.
- App containers were rebuilt/recreated and are Up; `postgres` was not
  recreated.

### 2026-06-08 - ChipSet Scan Second Status Fix

- Fixed scan-created double ChipSets only storing the first status in the
  server item instance. The scan processor was reading only the first
  `SkillCodeInfo.Apply`, so CT/HT or CT/EV items could show both stats in the
  client tooltip but only apply CT on the server.
- Scan now uses `AccessoryEnchantService.ApplyRandomChipsetStats`, which fills
  every valid apply slot from `Skill.bin` while preserving scan RNG.
- GM `!item` creation remains max-value only through the existing max path.
  This does not force normal scanned family ChipSets to maximum values.
- Server build passed with no errors. VPS deploy completed with archive
  `.codex-tmp\dmo-server-main-postgres-20260608-145315.tar.gz`; remote backup:
  `/home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-175334`.
- App containers were rebuilt/recreated and are Up; `postgres` was not
  recreated. Post-deploy log scan found no recent `ERR`, `Exception`, DI
  failure, duplicate-key, or item-move rejection entries in the checked app logs.

### 2026-06-08 - Digivice ItemMove Race And Chipset CT Percent Fix

- Fixed the remaining Digivice unequip bug found in live logs. Removing a
  Digivice with multiple attached chipsets makes the client send multiple
  `ChipsetToInventory` moves almost together; the game server processed those
  `3904` packets in parallel and the second save could duplicate
  `Shared_ItemStorageInstance`.
- `ItemMovePacketProcessor` now serializes item movement per tamer before swap,
  persistence, stat update, and response packet generation. This keeps
  inventory/equipment/Digivice/chipset/warehouse moves in packet order without
  changing client slot layout or packet format.
- Corrected ChipSet CT/EV contribution on the server. Chipset raw values now
  stay in the same basis-point scale the client uses instead of being multiplied
  by `100`, so a CT raw value like `436` contributes `4.36%` instead of
  clamping the Digimon CT to the unsigned-short ceiling.
- Updated the client item tooltip parser so CT/EV ChipSet effects render with
  percent text, e.g. `4.36%`, while integer stats like HT keep integer display.
- Built the x64 bridge client and copied the new runtime to
  `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`
  (`08/06/2026 14:27:23`, `13686784` bytes).
- Server build passed with no errors. VPS deploy completed with archive
  `.codex-tmp\dmo-server-main-postgres-20260608-142831.tar.gz`; remote backup:
  `/home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-172850`.
- App containers were rebuilt/recreated and are Up; `postgres` was not
  recreated. Post-deploy log scan found no recent `ERR`, `duplicate key`,
  `Exception`, or `ItemMove rejected` entries in the checked app logs.

### 2026-06-08 - Digivice Unequip Storage Persistence Fix

- Confirmed that removing a Digivice should also remove/move the attached
  chipsets; that part of the observed packet flow is expected.
- Fixed the server-side normalized storage save that could fail after the
  Digivice was already removed in memory. The failure was a duplicate-key insert
  into `Shared_ItemStorageInstance` when an item instance moved from one owner
  list to another.
- Added explicit item-move persistence mode so per-list saves preserve
  temporarily unreferenced instances during inventory/equipment/Digivice/
  chipset/warehouse moves.
- Owner-storage upsert now resolves existing instances from both current slots
  and the incoming payload ids, and replaces accessory/socket detail rows for
  seen items without duplicating them.
- Server build passed with no errors. VPS deploy completed with archive
  `.codex-tmp\dmo-server-main-postgres-20260608-135154.tar.gz`; remote backup:
  `/home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-165214`.
- App containers were rebuilt/recreated and are Up; `postgres` was not
  recreated. Post-deploy log scan found no recent duplicate-key or item-move
  persistence errors.

### 2026-06-08 - Status Tooltip Attribution, DUnit Icon, And Final Deploy

- Fixed Digimon stat tooltips so unresolved stat deltas are logged as
  `STAT_TOOLTIP_UNRESOLVED` instead of being drawn as fake red equipment rows.
- Added a server-detail equipment fallback for Digimon stat tooltips. When the
  server already sent a positive `TamerDetail*` bonus and the local legacy
  equipment formula cannot decompose it, the tooltip now shows that missing
  positive detail as an equipment row only if tamer equipment is actually
  equipped.
- Added the DUnit/Unit tooltip row with the original Union icon reference:
  `Union\Filter_icon.png`.
- CT tooltip attribution now uses the authoritative server total as the base
  when needed, removing the phantom equipment CT row seen with no equipment.
- BL remains integer-percent in server/main status/combat. The client tooltip
  converts BL to basis points only inside tooltip attribution math.
- The intermediate server BL basis-point experiment was reverted before the
  final deploy.
- Final x64 client runtime verified at:
  `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`
  (`08/06/2026 13:17:10`, `13686272` bytes).
- Final VPS deploy archive:
  `.codex-tmp\dmo-server-main-postgres-20260608-130112.tar.gz`
  (`91.7 MB`).
- Final remote backup:
  `/home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-160131`.
- Post-deploy grep found no `ERR`, `FTL`, `Unhandled`, or `Exception` matches
  in app services. Known DUnit XML content warnings may still appear in game
  logs.

### 2026-06-08 - Digivice Jogress/Xros Slot And VPS Deploy

- Added the GDMO-style dedicated Digivice Jogress/Xros chip slot flow:
  normal ChipSet remains in client/server container `4000`, while the
  Jogress/Xros chip uses the dedicated EvoChip container `3000`.
- Added client storage for one EvoChip entry on `Data_Digivice`, packet parsing
  after the 8 normal ChipSet slots, and UI rendering/drag-drop handling for the
  dedicated slot on the Digivice tab.
- Added server initial-packet serialization for `JogressChipSet` immediately
  after normal `ChipSets`, so the Tamer Information Digivice tab mirrors the
  real equipped data.
- Added server move validation so normal chips accept only
  `Type=52/SkillCode!=2500245`, while Jogress/Xros chips go only to the
  dedicated EvoChip slot (`Type=52/SkillCode=2500245`).
- `CI_EVOCHIP` is only the internal drag/drop item type used by the client to
  route slot `3000`; it does not change the visual mouse cursor.
- Client x64 build was produced and copied to:
  `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`.
- Server deploy completed on the VPS with the password wrapper:
  `Publish-DmoServerToVpsDockerComposeWithPassword.ps1`.
- Deployed app services only:
  `account`, `character`, `game`, `routine`, `api`, and `admin`.
- `postgres` was not deployed or recreated. Post-deploy check showed the
  existing `dmo-postgres` container was created on 2026-05-25, still healthy,
  and had restart count `0`.
- Deploy archive: `.codex-tmp\dmo-server-main-postgres-20260608-130112.tar.gz`
  (`91.7 MB`).
- Remote backup:
  `/home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-160131`.
- Post-deploy log scan showed no `ERR`, `FTL`, `Unhandled`, or `Exception`
  matches in app services. Known DUnit XML content warnings may still appear in
  game logs.

### VPS Docker Compose Deploy Automation

- Added root deploy wrappers for the PostgreSQL server:
  - `Publish-DmoServerToVpsDockerComposeWithPassword.ps1`
  - `Publish-DmoServerToVpsDockerComposeWithPassword.py`
- The deploy wrapper uses bundled Python + Paramiko with the local-only password
  file `.codex-tmp\dmo-vps-password.txt`; it strips UTF-8 BOM to avoid SSH
  authentication failures.
- Default deploy target is app services only:
  `account`, `character`, `game`, `routine`, `api`, and `admin`.
- `postgres` is intentionally not deployed or recreated because the production
  database already exists on the VPS.
- Successful test deploy on 2026-06-07:
  - command: `Publish-DmoServerToVpsDockerComposeWithPassword.ps1 -ReuseLatestArchive`
  - archive size: `50.6 MB`
  - total time: `78.9 seconds`
  - backup: `/home/ubuntu/deploy-backups/dmo-server-main-postgres-20260607-201242`

### Server

- The workspace is now focused on the PostgreSQL server tree:
  `dmo-server-main-postgres/`.
- The old local MySQL server tree was retired from the active workspace so new
  server fixes target the PostgreSQL stack only.
- PostgreSQL server notes should stay in the active local server docs under
  `dmo-server-main-postgres/`.

### Client Chat And Mojibake Fixes

- Fixed length-prefixed `char*` packet reads in `dmo-client-main/nlib/packet.cpp`
  so copied strings are always null-terminated.
- Cleared the local system-message buffer in `SyncMsgSystem()` before reading
  packet text.
- Blocked release-build debug chat from entering the visible chat UI.
- Removed several client debug chat emissions from monster, buff, and sync paths.
- Added chat diagnostics:
  - `Log/ChatRenderTrace_client.txt`
  - `Log/ChatVisibleTrace_client.txt`
  - `Log/TextRenderTrace_client.txt`
- Added a final low-level guard in `cString::sTEXT::Render()` and
  `RenderLimit()` for short mojibake strings. This catches patterns such as
  Cyrillic/Latin-extended mojibake, the Hangul artifact `U+CD2B`, and repeated
  comma-zero suffixes like `, 0`.
- The repeated malformed cyan/green chat text was confirmed stopped after the
  low-level render guard.

### Chat Korean/System Text Handling

- Added a small translation pass for known Korean system/debug chat phrases
  before they enter the chat UI.
- The source uses Unicode escape literals for those known phrases so the mixed
  legacy source encodings do not corrupt them at compile time.

### Packet/Protocol Notes

- Added `tutorial/packetprocessor.md` with a real client/server packet flow
  using megaphone chat as the example.
- The most important packet maintenance rule is unchanged: client write/read
  order and server read/write order must match byte-for-byte.
