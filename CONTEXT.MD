# LK-DMO Technical Context

Last updated: 2026-05-22

This document is the working memory for the LK-DMO workspace in `C:\0-NewDmo`.
It summarizes the main fixes, migration work, build notes, and packet/protocol
map discovered while adapting the client and server.

## Non-negotiable client build target

The active test client is the English release build. Unless the user explicitly
asks for another locale, always build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Use the solution file for client builds. Do not compile
`C:\0-NewDmo\dmo-client-main\DProject\DProject.vcxproj` directly: it does not
carry the same solution configuration/dependency mapping and has repeatedly
caused false build failures around `Release_English|Win32`, PCH, and PDB locks.
If MSBuild reports `C1041` or another PDB contention issue, stop orphaned
`MSBuild`/`cl` processes if needed, then rebuild the same solution with `/FS`
in the compiler environment:

```powershell
$env:CL="/FS"
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Do not use `Release|Win32` for normal testing; that produces the Korean binary
under `C:\Release\Kor\DProject_R.exe`.

Expected English output and local test copies:

```text
C:\Release\GSP\DProject_R_ENG.exe
C:\0-NewDmo\ClientDist\DProject_R_ENG.exe
D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe
```

After a successful English build, copy `C:\Release\GSP\DProject_R_ENG.exe` to
both `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe` and
`D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`.
If either destination is locked by a running client, stop the blocking
`DProject_R_ENG.exe` process and retry the copy.

## Repository layout

- `dmo-client-main/`: C++ client source. Main game project is `DProject.sln`.
- `dmo-server-main-postgres/`: PostgreSQL server port and active Docker setup.
- `ClientDist/`: local compiled client distribution/output copy. Ignored by Git.
- `Start-DMO-Postgres-Docker.bat`: helper for the PostgreSQL stack.
- `Start-DMO-Servers.bat`: helper for local server startup.

## Current build/run targets

### Client

Primary build used during the fixes:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Important: run the command against `DProject.sln`, not
`DProject\DProject.vcxproj`.

Expected output:

```text
C:\Release\GSP\DProject_R_ENG.exe
```

Common copy targets used locally:

```text
C:\0-NewDmo\ClientDist\DProject_R_ENG.exe
D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe
```

If the destination exe is open, Windows blocks the copy. In that case close the
running `DProject_R_ENG.exe` process before replacing it.

### PostgreSQL server stack

Active compose directory:

```text
C:\0-NewDmo\dmo-server-main-postgres
```

Common command:

```powershell
docker compose up -d --build
```

Observed container/port mapping:

- `dmo-postgres`: local port `5432`, database `dso`, user `dso`, password `dso_password`.
- `dmo-pg-account`: local port `17029`.
- `dmo-pg-character`: local port `17050`.
- `dmo-pg-game`: local port `17608`.
- `dmo-pg-api`: local port `15189`.
- `dmo-pg-admin`: local port `15000`.

PostgreSQL connection string used by services:

```text
Host=postgres;Port=5432;Database=dso;Username=dso;Password=dso_password;Search Path=dso;
```

For DBeaver on the host:

```text
Host: localhost
Port: 5432
Database: dso
User: dso
Password: dso_password
Schema: dso
```

## Changelog / work completed

### 2026-05-22 archive, transcendence, D-Unit, and packet stabilization

- Digimon Archive UI was aligned back toward the official layout: five visible
  top slots with arrow paging, support for the local eight mercenary slots, and
  Storage/Academy/Transcendence tab flow without the temporary stretched slot
  layout that pushed the detail panel into the wrong position.
- Archive packet handling was hardened around client/server bin mismatches:
  - server load packet `3204` only serializes archive rows with both `DigimonId`
    and a loaded `Digimon` object;
  - server manage packet `3201` accepts the observed non-VIP and VIP payload
    shapes, validates target archive slots, and logs tamer, digivice slot,
    archive slot, NPC, partner IDs, and packet length;
  - archived digimon use `Slot = 255` (`byte.MaxValue`) and active digimon are
    filtered from that archive slot before building the active mercenary list.
- Archive persistence now avoids the broad `UpdateCharacterDigimonsOrderCommand`
  in archive move/swap paths. Individual digimon slots and archive rows are
  updated directly, and stale archive navigation references are cleared when
  moving a digimon out of the archive. This was the key fix for the Gabumon/slot
  disappearance after repeated open, close, move, and channel tests.
- Transcendence was kept packet-aware without enabling the global Korean DNS
  transcendence define in the USA client. The server fixes now match the current
  English client layouts for charge EXP `16039` and final transcendence `16040`,
  including material order, EXP fields, and processor registration.
- Added detailed archive diagnostics on the server under `ARCHIVE 3201` and
  `ARCHIVE 3204`, plus protocol audit notes in
  `PROTOCOL_AUDIT_2026-05-22.md`. Use these logs first when the archive opens
  empty, shows a wrong slot count, or a digimon visually moves but is lost after
  reload.
- Jump Booster / All Area crash was traced to a client-side message box lifetime
  issue. `cMessageBox::DelMsg()` no longer leaves a pending dismissal when no
  live box exists, `DeleteList()` clears pending dismissals, and
  `JumpBuster.cpp` now validates selected map data and falls back to a direct
  send if the confirmation box cannot be recovered.
- GM Panel was restored to the wider modern panel with side actions for Make
  Item, Debug Char, Level Up, LevelUp-Digi, AllEvo, and Bits. The main menu GM
  button/icon state was also corrected.
- The old client `UnionDUnit.*` project entries were removed from the active
  project in favor of the XML Union implementation. `XmlUnionDUnit.*` now owns
  the D-Unit test window, card population, rank/icon rendering, input capture,
  and client-side XML loading from `Data\union`.
- Server-side XML Union/D-Unit work added XML asset loading, packet processors,
  progress/reward packets, character progress DTO/model/configuration, and the
  `20260521143000_AddCharacterXmlUnionProgress` migration. Runtime XML assets
  live under `dmo-server-main-postgres\xml\union`.
- `Dcommerce/` is a large local extraction/tooling workspace, about 983 MB of
  XML dumps, DLL/runtime files, executables, and a portable tool archive. It is
  intentionally ignored by Git and should be regenerated or copied locally when
  needed, not committed as source.
- Latest known client verification for this round used the English release
  solution build and copied `DProject_R_ENG.exe` to both local test targets. The
  JumpBuster build hash observed after the crash fix was
  `F8EA7AAC5BD77F9F12ABBB3F2EAE23B962D7AFE4E6A4915B7A9B796CD4C4E59A`.

### Client/server bootstrap

- Read earlier `source-client` notes as historical reference only. The active
  project root for build/test work is `C:\0-NewDmo`.
- Confirmed that `Release_English|Win32` is the relevant client target for
  `DProject_R_ENG.exe`.
- Avoided relying on the `Give_English` target for final testing after it caused
  unnecessary library rebuild confusion.
- Added local output/copy workflow for the compiled release client.

### Docker and database environment

- Created and used a PostgreSQL server copy under `dmo-server-main-postgres`.
- Added/used Docker Compose services for account, character, game, API, admin,
  routine, and PostgreSQL.
- Configured service connection strings to use PostgreSQL with `Search Path=dso`.
- Imported/migrated the MySQL dump into PostgreSQL for active testing.
- Retired the local MySQL server tree from the active workspace so server work
  targets PostgreSQL only.

### Admin/account/secondary password

- Located the admin portal and account data flow.
- Queried/validated admin user information.
- Identified that the in-game second password is separate from the admin portal
  credentials and comes from account/game account data, not from the admin page.
- Reset/adjusted second password state enough to continue login testing.

### Login, server select, and character select

- Investigated server select disconnects after the PostgreSQL migration.
- Added targeted logs around server/channel selection and packet handoff.
- Fixed packet/enum alignment for server/channel flows so the client could pass
  server selection.
- Fixed character selection/loading issues that originally caused a white screen
  or non-responsive client after selecting a character.
- Added and later reduced diagnostics as packet formats became clear.
- Investigated an intermittent Server Select UI issue where the server card
  appeared near the upper-left of the screen instead of inside the centered
  server list window.
  - Root cause was in
    `DProject/_Interface/05.ServerSelect/SelectServer.cpp`.
  - `m_pServerListBox` was initialized against the main `CSelectServer`
    `GetRoot()` even though it is attached to `m_SelectListWindow`.
  - On wider resolutions or repeated enter/leave cycles, the list box could
    calculate its client position against the 1024x768 root instead of the
    centered list window.
  - Fix: initialize and resize the grid with
    `m_SelectListWindow.GetRoot()` as parent, and call `SetParentWindow(...)`
    during list-size refresh.
  - Added a defensive empty-list guard in `_ChangeListSize(...)` so an empty
    server list does not underflow the `size_t` width calculation.
  - A temporary `m_SelectListWindow.SetWindowPosition(...)` call inside
    `ResetServerList()` caused a `CsAssert` in `ScriptUI.cpp:173` during stress
    testing, because it could hit the UI region recalculation at a bad point in
    the screen lifecycle. That temporary call was removed; the final fix keeps
    only the list-box parent/empty-list changes.
- Added a user-facing "Remember PW" checkbox to the login box.
  - Runtime setting is stored in `dmo.ini` under `[login]`, not under `[DEBUG]`,
    because this is a user feature rather than a debug/server environment flag.
  - Expected keys:

    ```ini
    [login]
    RememberPassword=0
    id=
    pw=
    ```

  - When `RememberPassword=1`, the login screen auto-fills ID/PW.
  - New credentials are saved back to `[login]` when logging in while the
    checkbox is enabled.
  - For backward compatibility, if `[login] id/pw` are empty the client falls
    back to `[network_eng] id/pw`, then `[network] id/pw`.
  - `dmo.ini.example` and the local `ClientDist\dmo.ini` were updated with the
    new `[login]` section.
  - `cProfile` now supports writing string values, not only integers, so the
    client can persist the remembered ID/PW through the existing ini wrapper.

### PostgreSQL compatibility fixes

- Audited areas where MySQL behavior differed from PostgreSQL:
  - numeric types and byte-size assumptions,
  - nullable/default values,
  - inventory slot and item persistence,
  - seal persistence/reload,
  - packet fields serialized from database-backed models.
- Fixed several gameplay regressions caused by migration mismatches:
  - inventory item movement/desync,
  - equip item crashes,
  - character reload inconsistencies,
  - seal data not reloading after relog,
  - inventory expansion item incorrectly reporting "Inventory is full".
- Added logs in character/game server areas where crashes or state desyncs were
  hard to reproduce.

### Quest data loading

- Investigated the quest UI/NPC issue where no quests appeared on NPCs.
- Found that the client quest table manager had quest data loading disabled:
  `CsQuestMng::Init(...)` returned early with `DATAFILE_DISABLE_FOR_NOW`.
- Re-enabled quest loading in
  `dmo-client-main/LibProj/CsFileTable/QuestMng.cpp` by removing that early
  return so the client loads `Quest.bin` again.
- Updated `CsQuestMng::Reload(...)` to call `Delete()` before reloading, so
  repeated reloads clear old quest state instead of appending or asserting on
  existing entries.
- Client build verification was done with `Release_English|Win32`.
- User confirmed quests now load and are visible through NPC/Digivice UI.
- Important remaining validation: quest acceptance, progress tracking,
  completion/reward delivery, and persistence after relog have not been tested
  yet. Current confirmation covers visibility/loading only.

### GM tools / GM panel

- Added an admin-only entry from the existing option/menu flow into a `GMPanel`
  style UI.
- Implemented a "Make Item" / "Give Item" panel concept using the client's
  existing UI style.
- Extended the panel so item selection can search/filter by item name or item ID.
- Used server/client item assets so the item list is based on actual known items
  instead of a small hardcoded sample.
- Added server-side support for giving an item to an online player, including
  inventory placement/persistence and feedback messages.

### Seals

- Investigated the seal opener UI and server support.
- Temporarily explored a seal selection window change, then reverted that UI
  approach when it was not desired.
- Focus shifted to persistence:
  - reproduced seals appearing once and disappearing after relog/reselect,
  - added logs around seal load/save flow,
  - fixed/reduced the reload issue so seal values persist and reload more
    consistently.

### Inventory expansion

- Investigated expansion item behavior where the client/server always reported
  "Inventory is full".
- Verified that the user's inventory did contain free visible slots and that the
  error was logic/state related.
- Adjusted inventory expansion handling so the expansion item increases usable
  slots instead of treating existing empty slots as failure.
- User confirmed the later iteration worked.

### Usable items / consumables

- Investigated usable items after the inventory expansion fix because several
  valid items still returned incorrect client messages:
  - seal box / container-style items eventually worked,
  - HP/DS recovery items such as `Special Chicken Combo` still failed,
  - the client displayed misleading messages such as "Inventory is full" or
    "Item function is not connected".
- Important terminology: an item `SkillCode` / `SkillId` is not necessarily a
  player-cast skill. For items it usually points to a `Skill_Code` row from
  `DM_Item_List`, which describes the technical effect to apply.
  - `ItemId` identifies the item.
  - `SkillCode` identifies the effect row.
  - `SkillInfo.Apply[]` contains the actual behavior, such as HP, DS, EXP,
    movement speed, attack, buff code, percent/default value, etc.
- Client reference points:
  - `LibProj/CsFileTable/ItemMng.cpp` reads the item `Skill` field.
  - `LibProj/CsFileTable/SkillMng.cpp` loads `DM_Item_List -> Skill_Code`.
  - `DProject/_Interface/Game/ToolTip_Item.cpp` uses `s_dwSkill` to render item
    effect text.
- Server reference points:
  - `AssetsLoader.cs` maps `ItemAssetModel.SkillCode` to `SkillCodeInfo`.
  - `ItemConsumePacketProcessor.cs` applies `SkillInfo.Apply[]` when the item is
    consumed.
- Fixed the consumable flow in `ItemConsumePacketProcessor.cs`:
  - reloads runtime `ItemInfo` from assets if an inventory item is missing or has
    stale metadata,
  - resolves missing skill info from both database assets and `ItemList.bin`,
  - treats item types `61` and `71` as food/recovery consumables,
  - accepts `Percent`, `AlsoPercent`, `Unknown105`, and `Default` apply rows for
    HP/DS style recovery,
  - infers target `Both` for recovery consumables when the target field is
    missing/unavailable but the applies clearly restore HP/DS,
  - correctly applies tamer DS to the tamer instead of accidentally applying it
    to the partner in the `Tamer` target branch,
  - refreshes current HP/DS resource packets and HP rate packets after applying
    a consumable,
  - logs unsupported applies with item id, type, target, skill code, and apply
    summary for future debugging.
- Disabled the old `ItemTap` validation as a use allow-list. In this client data
  it is tab/category metadata, and using it as a gate rejected valid usable
  items such as recovery consumables, warehouse/inventory expansion, buffs, and
  containers.
- Adjusted failure packet mapping so unrelated validation failures do not show
  the inventory-full style message.
- Client diagnostics were added in `cCliGameReceive.cpp` for item-use failures,
  and `USE_FAIL_NOT_USE_TERMS` now maps to the more accurate "item function is
  not connected" style message instead of the previous misleading text.
- `ItemAssetModel.SetSkillInfo(...)` now allows replacing null/stale skill info
  with a resolved non-null skill model, while still preserving already-loaded
  valid skill info.
- Server build and Docker game publish were performed after the fix. User
  confirmed HP/DS consumables now work.

### Jump boosters / All Area jump booster

- Normal jump booster:
  - The `Release_English|Win32` client does not define
    `SDM_VIP_SYSTEM_20181105`, so it sends the jump booster request without the
    leading VIP byte.
  - The server originally always read `vip + slot + mapId`, which misaligned
    the payload for this build.
  - `JumpBoosterPacketProcessor.cs` now accepts both payload shapes:
    - `vip:u1 + slot:u2 + map:u2`,
    - `slot:u2 + map:u2`.
  - Added a log line with packet length, payload length, vip flag, slot, and map
    id.
  - User confirmed one regular jump booster path worked after this change.
- All Area / tamer jump gate:
  - The All Area flow does not use the normal item-slot jump booster packet.
  - Client path:
    - `JumpBuster.cpp` opens/selects the All Area destination.
    - `MessageBox.cpp` sends `SendTamerActiveSkill(...)` and then
      `SendTamerJumpGate(9801, mapId)`.
    - `cCliGameSend.cpp::SendTamerJumpGate` sends `pSkill::RickySkill`.
    - `common_vs2019/pSkill.h` defines `RickySkill` as packet `1113`.
  - Server did not have a processor for packet `1113`.
  - Added `GameServerPacketEnum.TamerJumpBooster = 1113`.
  - Added `TamerJumpBoosterPacketProcessor.cs`, which reads:

    ```text
    itemId:u2
    mapId:u2
    ```

  - The processor mirrors normal jump validation against
    `DMBase.bin -> JumpBusterDestinations`, flushes owner storage before map
    transition, removes the client from the current map, persists tamer and
    partner locations, marks the tamer as loading, and sends `MapSwapPacket`.
  - Added a log line:

    ```text
    Tamer jump booster request: tamer ... packetBytes ... item ... map ...
    ```

  - Built `DigitalWorldOnline.Game.csproj` successfully and published the
    `game` service with `docker compose up -d --build game`.
  - Container `dmo-pg-game` started cleanly and loaded all bins after publish.
- Final DATS / D-Terminal follow-up:
  - The initial "All Area can show DATS/D-Terminal" approach was replaced
    because it was too item-specific.
  - Investigation showed current world-map/jump data uses:
    - D-Terminal: `mapId 2`
    - DATS Center: `mapId 3`
    - old/legacy DATS id: `mapId 1`
  - Client-side data normalization now happens during base table load:
    - `CsBaseMng::_NormalizeJumpBusterDestinations()` adds DATS `3` to all
      jump booster destination sets.
    - It adds D-Terminal `2` to broad/all-area-style boosters, detected by
      destination count instead of hardcoding item `9801`.
    - `CsBaseJumpBuster::Insert()` is idempotent so normalized duplicates do
      not assert/crash.
  - Client map-open checks now treat legacy DATS `1` as DATS `3`, and treat
    DATS/D-Terminal as base opened hub maps.
  - Server-side `DMBaseBinLoader.ReadJumpBuster()` applies the same
    normalization:
    - maps destination `1 -> 3`,
    - adds DATS `3` to all jump booster lists,
    - adds D-Terminal `2` to all-area-scope destination lists.
  - The normal and tamer/all-area jump processors now validate against the
    normalized `DMBase.bin -> JumpBusterDestinations` data instead of carrying
    special-case allow logic.
  - Latest client rebuild/copy after this normalization:
    - `C:\Release\GSP\DProject_R_ENG.exe`
    - `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe`
  - Latest Docker publish after this normalization:
    - `docker compose up -d --build game`
    - `dmo-pg-game` started cleanly.

### Megaphone / shout systems

- Investigated all shout/megaphone variants because the first fix affected only
  consumption and not chat display.
- Found an important client C++ overload bug:
  - `SendMsgAllByItem(wchar *msg)` passed a mutable wide pointer.
  - The generic `push<T&>` overload could serialize the pointer address instead
    of serializing the string.
- Added explicit wide-string overloads:
  - `dmo-client-main/nlib/cClient.h`
  - `dmo-client-main/nlib/cpacket.h`

  Example:

  ```cpp
  void push(wchar_t* str)
  {
      push(static_cast<const wchar_t*>(str));
  }
  ```

- Fixed the server megaphone packet processor so it reads:
  - message string,
  - inventory slot,
  without consuming an extra terminator byte before slot.
- Confirmed megaphone broadcast displayed in chat and item consumption behavior
  stopped being incorrect.
- Added logging in:
  - `MegaphoneMessagePacketProcessor.cs`
  - `ShoutMessagePacketProcessor.cs`
  to inspect string length, reader position, payload remainder, slot, and raw
  packet during debugging.

### Chat debug/corrupted text issue

- Investigated strange cyan/garbled chat lines.
- Determined they are not the megaphone payload.
- Found client-side debug messages also appear in the chat, for example:
  - `MoveSpeed ...`
  - other `NS_CHAT::DEBUG_TEXT` lines from C++ systems.
- Changed `ChatContents::_SetDebug` so debug chat only displays for `_DEBUG`
  builds:

  ```cpp
  #if defined(_DEBUG)
      _SetText(szText, nType, nValue);
  #endif
  ```

- Added a non-debug guard in `ChatContents::Chat_Process` so direct
  `NS_CHAT::DEBUG_TEXT` events are dropped before entering the chat UI.
- Removed several monster/buff growth debug chat emissions from:
  - `DProject/network/cCliGameSync.cpp`
  - `DProject/network/cCliGameBuff.cpp`
  - `DProject/Monster.cpp`
- Found a stronger root cause for the static mojibake/cyan spam: the base
  client packet reader copied length-prefixed `char*` strings without writing a
  terminating `\0` into the destination buffer. Short system/chat strings could
  therefore display stack residue after the valid message.
- Fixed `dmo-client-main/nlib/packet.cpp` so `cPacket::pop(..., wlen=true)`
  always null-terminates the copied string, including empty/short reads.
- Also zeroed `SyncMsgSystem()`'s local `msg` buffer before each pop.
- Added final render diagnostics and a defensive mojibake gate in
  `ChatMainWindow::Notify(NS_CHAT::ADDSTRING_UPDATE)`.
  - Runtime log: `Log\ChatRenderTrace_client.txt` beside the running exe.
  - Each line records chat type, value, string length, whether it was blocked,
    and the first codepoints.
- Follow-up for the persistent cyan/green static spam:
  - User identified one visible character as `촫` (`U+CD2B`), which is a strong
    mojibake signature in the malformed chat line.
  - Extended the chat garbage detector to treat `U+CD2B` plus nearby Latin-1
    mojibake as suspicious while still allowing normal Korean/Hangul text.
  - Added a second defensive gate directly in `ChatMainWindow::StringListRender()`
    because the previous `ADDSTRING_UPDATE` trace only saw normal chat/system
    messages, meaning the spam could already be resident in the visible list or
    entering below the prior hook.
  - Added runtime log `Log\ChatVisibleTrace_client.txt`; it records blocked
    visible-list lines and Hangul lines using codepoints without logging every
    normal frame render.
  - Added a small system-chat Korean translation pass in `ChatContents` for known
    debug/system phrases, using Unicode escape literals in source to avoid
    source-file encoding corruption.
- Second follow-up after user reported the spam still appears every 5-6 seconds:
  - Existing logs showed the normal chat hook still only recorded valid English
    `NORMAL_TEXT` / `DIGIMON_TIP` messages, and `ChatVisibleTrace_client.txt`
    was not created. That means the visible garbage was not matching the earlier
    Hangul-only guard, or it was being rendered through a lower `cString` path.
  - Network logs showed packet `16032` repeating at roughly five-second cadence
    among other frequent packets, matching the user's timing clue.
  - Added a lower render guard in `DProject/_Interface/Base/String.cpp` at
    `cString::sTEXT::Render()` / `RenderLimit()`. It blocks short rendered text
    with classic mojibake signatures such as Cyrillic `Г`, `ї`, Latin-extended
    `ž`, the Hangul artifact `촫`, and comma-zero suffixes like `, 0`.
  - Added runtime log `Log\TextRenderTrace_client.txt`; it records blocked text
    render attempts and throttled comma-containing samples as codepoints.
  - User later confirmed that this last lower-level render guard stopped the
    repeated malformed chat text from being displayed.
- Latest successful client build/copy for this follow-up:
  - `C:\Release\GSP\DProject_R_ENG.exe`
  - `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe`
  - `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`
  - timestamp `2026-05-19 21:25:41`, size `10840064`.
- Latest successful client build after the lower render guard:
  - `C:\Release\GSP\DProject_R_ENG.exe`
  - `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe`
  - `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`
  - timestamp `2026-05-19 21:30:56`, size `10842624`.
- Last successful client build/copy for this fix:
  - `C:\Release\GSP\DProject_R_ENG.exe`
  - `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe`
  - `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`
  - timestamp `2026-05-19 21:13:50`, size `10835456`.

## Packet/protocol maintenance guide

This project has two packet worlds that must stay aligned:

- C++ client packet IDs and read/write order.
- C# server packet enum, processors, and packet writer/readers.

Most bugs fixed during this work came from one side reading a different byte
layout than the other side wrote.

### Client C++ protocol map

Core enum files:

- `dmo-client-main/common_vs2019/nScope.h`
  - Defines protocol number ranges, e.g. `Game = 1000`, `Svr = 1700`,
    `Item = 3900`, `Quest = 11000`.
- `dmo-client-main/common_vs2019/pGame.h`
  - Defines game packet IDs as offsets from `nScope::Game`.
  - Example: `pGame::Sync` is `1006`.
  - Example: `pGame::MsgAllByItem` is the client-side megaphone request.
- `dmo-client-main/common_vs2019/pSync.h`
  - Defines sub-packet types inside `pGame::Sync`.
  - Important chat values:
    - `MsgChat`
    - `MsgWhisper`
    - `MsgAll`
    - `MsgSystem`
    - `MsgShout`
    - `MsgAllByItem`
- `dmo-client-main/common_vs2019/pItem.h`
  - Item/inventory packet families and item slot tab definitions.

Client read/write helpers:

- `dmo-client-main/nlib/cClient.h`
- `dmo-client-main/nlib/cpacket.h`

Important behavior:

- `push(...)` writes primitives and strings.
- `pop(...)` reads primitives and strings.
- Client strings are generally:
  - 1 byte length,
  - string bytes,
  - trailing `0`.
- Wide strings must be converted to multibyte before writing. The added
  `wchar_t*` overload avoids accidentally serializing a pointer value.

Client send path:

- `dmo-client-main/DProject/network/cCliGameSend.cpp`
  - Contains methods that build outgoing game packets with `newp(...)`,
    `push(...)`, `endp(...)`.

Client receive dispatch:

- `dmo-client-main/DProject/network/cCliGame.cpp`
  - `cCliGame::OnExecute()` reads the protocol ID and routes to handlers.
  - Example: `case pGame::Sync : RecvSyncData();`

Client receive handlers:

- `dmo-client-main/DProject/network/cCliGameReceive.cpp`
  - Handles normal direct packets.
  - Example: `RecvStartMessage()` handles `pGame::StartMessage`.
- `dmo-client-main/DProject/network/cCliGameSync.cpp`
  - Handles `pGame::Sync` sub-packets.
  - `RecvSyncData()` reads a sync type byte and loops until `0`.
  - Chat handlers include:
    - `SyncMsgChat()`
    - `SyncMsgWhisper()`
    - `SyncMsgAll()`
    - `SyncMsgSystem()`
    - `SyncMsgShout()`
    - `SyncMsgAllByItem()`

### Server C# protocol map

Core enum:

- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Enums/PacketProcessor/GameServerPacketEnum.cs`
  - Maps incoming client packet numbers to named server processors.
  - Example: `MegaphoneMessage = 1074`.
  - Example: `ShoutMessage = 1056`.
  - Example: `MoveItem = 3904`.

Server packet dispatcher:

- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/GamePacketProcessor.cs`
  - Reads incoming packet ID.
  - Finds an `IGamePacketProcessor` whose `Type` matches the enum.
  - Calls that processor.

Server packet readers/writers:

- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/GamePacketReader.cs`
  - Reads incoming payload from the client.
- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Writers/PacketWriterBase.cs`
  - Writes primitive values and strings.
- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Writers/PacketWriter.cs`
  - Adds length/checksum around server-to-client packets.

Server processors:

- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/`
  - One file per incoming packet behavior.
  - Examples:
    - `ChatMessagePacketProcessor.cs`
    - `ShoutMessagePacketProcessor.cs`
    - `MegaphoneMessagePacketProcessor.cs`
    - `ItemMovePacketProcessor.cs`
    - `ItemConsumePacketProcessor.cs`
    - `SealOpenPacketProcessor.cs`

Server response packets:

- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Packets/`
  - Packet classes sent from server to client.
  - Chat examples live under `Packets/Chat/`.

### Real example: megaphone item chat

Client open/send path:

1. UI code opens the shout/megaphone window:
   - `dmo-client-main/DProject/_Interface/Game/Shout.cpp`
2. The UI submits:
   - `net::game->SendMsgAllByItem(...)`
3. Packet is built in:
   - `dmo-client-main/DProject/network/cCliGameSend.cpp`

Simplified client-side shape:

```cpp
void cCliGame::SendMsgAllByItem(wchar* msg, n4 nItemSlotNo)
{
    newp(pGame::MsgAllByItem);
    push(msg);
    push(nItemSlotNo);
    endp(pGame::MsgAllByItem);
}
```

Server receive path:

1. Incoming packet ID `1074`.
2. `GameServerPacketEnum.MegaphoneMessage`.
3. Routed to:
   - `MegaphoneMessagePacketProcessor.cs`
4. Processor reads:

```csharp
var packet = new GamePacketReader(packetData);
var message = packet.ReadString();
var slot = packet.ReadInt();
```

5. Processor finds the inventory item in that slot.
6. Processor broadcasts a sync chat packet:

```csharp
new ChatMessagePacket(
    message,
    ChatTypeEnum.Megaphone,
    client.Tamer.Name,
    inventoryItem.ItemId,
    client.Tamer.Level)
```

Server response packet:

- `ChatMessagePacket.cs`
- Packet number `1006`, which matches client `pGame::Sync`.
- For megaphone, first byte is `ChatTypeEnum.Megaphone`.
- That maps to client `pSync::MsgAllByItem`.

Client receive path:

1. `cCliGame::OnExecute()` receives `pGame::Sync`.
2. Calls `RecvSyncData()`.
3. `RecvSyncData()` sees sync type `MsgAllByItem`.
4. Calls `SyncMsgAllByItem()`.
5. `SyncMsgAllByItem()` reads:
   - count,
   - sender name,
   - message,
   - item type/id,
   - tamer level.
6. UI renders it as cash/megaphone chat.

Important maintenance warning:

- Do not add or remove "terminator" bytes casually.
- The final `WriteByte(0)` in `ChatMessagePacket` is the `pSync::None` terminator
  for the sync loop, not part of the string.
- If the server consumes or writes an extra byte before a slot/int, all following
  fields shift and the client reads garbage.

### Real example: normal chat

Client send:

- `ChatContents::_SendNetwork(...)`
- `cCliGameSend.cpp`
- Sends normal chat packet to the game server.

Server receive:

- `GameServerPacketEnum.ChatMessage = 1008`
- `ChatMessagePacketProcessor.cs`
- Reads the string and broadcasts:

```csharp
new ChatMessagePacket(
    message,
    ChatTypeEnum.Normal,
    client.Tamer.GeneralHandler)
```

Client receive:

- Server sends packet `1006` (`pGame::Sync`).
- Sync type maps to `pSync::MsgChat`.
- `SyncMsgChat()` reads:
  - count,
  - tamer handler/UID,
  - message.

### Real example: start/welcome messages

Server response packet:

- `WelcomeMessagePacket.cs`
- Packet number `1047`.

Client receive:

- `pGame::StartMessage`
- `cCliGameReceive.cpp`
- `RecvStartMessage()`

Expected payload:

```text
u1 messageCount
messageCount x string
```

If welcome/system text appears corrupted:

1. Check `Config_WelcomeMessage` in PostgreSQL.
2. Check `WelcomeMessagePacket.cs` writes exactly the count and strings expected.
3. Check whether the line is actually `NORMAL_TEXT`, `DEBUG_TEXT`, or another
   chat type in `ChatContents::Chat_Process`.

### Packet debugging checklist

When a packet breaks:

1. Find the client packet ID.
   - Search in `common_vs2019/p*.h`.
2. Find the client send or receive function.
   - Sends: `cCliGameSend.cpp`
   - Direct receives: `cCliGameReceive.cpp`
   - Sync receives: `cCliGameSync.cpp`
3. Write down the exact field order and byte sizes.
   - `u1` = 1 byte.
   - `u2` = 2 bytes.
   - `u4` / `n4` = 4 bytes.
   - strings = len byte + bytes + trailing zero.
4. Match that order in C#.
   - Incoming: `GamePacketReader`.
   - Outgoing: `PacketWriterBase`.
5. Check whether the server packet is direct or a `pGame::Sync` wrapper.
6. For sync packets, make sure the final byte is `0` so the client sync loop ends.
7. Add short logs with:
   - packet id,
   - payload length,
   - reader position,
   - remaining bytes,
   - decoded key fields.
8. Remove or reduce raw hex logs after the format is confirmed.

## Recent client UI work - quick menu, main menu, and HUD polish

Completed on 2026-05-20:

- Reworked the bottom quick menu to use the newer GDMO-style `QuickMenu`
  button atlases:
  - `quickmenu_button_menu.png`
  - `quickmenu_button_box.png`
  - `quickmenu_button_community.png`
  - `quickmenu_button_inventory.png`
  - `quickmenu_button_data.png`
  - `quickmenu_button_character.png`
  - `quickmenu_button_event.png`
- Main quick menu implementation lives mainly in:
  - `dmo-client-main/DProject/_Interface/Game/MainBar.cpp`
  - `dmo-client-main/DProject/_Interface/Game/MainBar.h`
- Added two placeholder HUD buttons beside the bottom quick menu:
  - auto potion uses `autopotion\ui_simplification_on.tga`
  - auto magnetic uses `autocrop\autocrop_on_btn.tga`
  - both are currently visual-only/no gameplay function wired yet.
- Expanded the `MainBar` root height to safely contain the new buttons and keep
  them clickable/rendered inside the UI window instead of relying on negative
  child coordinates.
- Aligned the auto potion/auto magnetic buttons with the bottom quick menu and
  standardized spacing between them and the first quick menu icon.
- Rebuilt the client with `Release_English|Win32` and copied the output to
  `ClientDist\DProject_R_ENG.exe` after each visual adjustment.

Main menu refactor:

- Replaced the old vertical `Option` window with the newer large icon menu using
  `MainMenu\mainmenu_*` assets.
- The main menu now follows the modern two-row icon layout and bottom option
  row, matching the newer reference screenshots more closely.
- Current action wiring includes existing useful actions where available:
  map, Digimon, inventory, quest, seal, graphic/options, cash warehouse,
  tamer, guild, friend, mailbox, cash shop, reward/box, encyclopedia, logout,
  exit, interface, sound, shortcut.
- GM Panel access was intentionally placed on one of the otherwise unused menu
  icons so the function is still reachable.
- Main menu files:
  - `dmo-client-main/DProject/_Interface/Game/MainOption.cpp`
  - `dmo-client-main/DProject/_Interface/Game/MainOption.h`

Quick menu popup/submenu polish:

- Updated popup/submenu icon sets and labels to better match the current GDMO
  references.
- Added/adjusted entries such as Cash Shop, Cash Warehouse, Seal Master,
  Encyclopedia, Battle Statistics, Evolution Helps, Bag, Exclusive Inventory,
  skin inventory, D-Unit, and related submenu actions where client functions
  already existed.
- Popup menu files:
  - `dmo-client-main/DProject/_Interface/Game/Popup.cpp`
  - `dmo-client-main/DProject/_Interface/Game/Popup.h`

HUD near minimap/channel:

- Replaced/adjusted the event/news button to use the newer megaphone-style event
  popup asset: `EventPopup\event_popup_openbtn.png`.
- Repositioned the event/news, VIP/membership, and Season Pass icons to sit
  under the channel selector with more uniform spacing and less overlap with the
  minimap.
- Repositioned the channel selector itself slightly left/down so it no longer
  touches the minimap edge as tightly.
- Reduced Season Pass subtly from `64x64` to `60x60` and moved it slightly down
  to better match the newer HUD reference.
- HUD files touched:
  - `dmo-client-main/DProject/_Interface/Game/Channel.cpp`
  - `dmo-client-main/DProject/_Interface/Game/EventNoticeButton.cpp`
  - `dmo-client-main/DProject/_Interface/17.VipMemberShip/VipSystemWindow.cpp`
  - `dmo-client-main/DProject/_Interface/Game/SeasonPassButton.cpp`

Decompiler/reference notes:

- `quickmenu_png_functions/` contains the small extracted function references
  copied from `unpacked_exe_all` for quick menu `.png` discovery.
- `unpacked_exe_all/` is a large local decompiled dump, useful only as analysis
  input, and is intentionally ignored by Git.
- `quickmenu_png_functions/README.md` documents the copied functions and the
  asset name references found in the original executable.

## Recent client UI work - Encyclopedia and inventory renewal

Completed on 2026-05-21:

### Encyclopedia

- Reworked the encyclopedia window around the newer
  `Encyclopedia\newencyclopedia\...` visual set.
  - Main files:
    - `dmo-client-main/DProject/_Interface/Game/Encyclopedia.cpp`
    - `dmo-client-main/DProject/_Interface/Game/Encyclopedia.h`
    - `dmo-client-main/DProject/_Interface/Game/EncyclopediaContents.cpp`
  - The main window uses the same large `886x768` centered layout later reused
    by the D-Unit screen.
  - Added modern top-level tabs:
    - `Encyclopedia`
    - `Deck`
  - Added submenu buttons for:
    - `Series`
    - `All`
    - `Class`
    - `Stats`
  - The original radio/tab data is still reused internally, but the visible UI
    is driven by the new buttons.
- Added encyclopedia search UI.
  - Uses the new encyclopedia search bar/button/reset assets.
  - Stores the current keyword in `m_wsSearchKeyword`.
  - Search/reset visibility follows the active tab/submenu.
- Added card/list hover tracking.
  - Card hover data is tracked through `m_vCardHoverInfo`.
  - Deck panel hover data is tracked through `m_vDeckPanelHoverInfo`.
  - Deck favorite/bookmark visuals are tracked through
    `m_vDeckBookmarkInfo` and `m_setDeckFavoriteGroup`.
- Added the Digimon overview/detail panel opened from encyclopedia cards.
  - Uses render texture support through `cRenderTex`.
  - Renders the selected Digimon model in the overview.
  - Tracks selected/hovered skill entries and displays skill detail text.
  - Includes model view controls for zoom, rotation, arrows, and reset view.
  - Resolves visible growth/stat values through `_ResolveOverviewGrowthData`.
- Deck state handling was made safer.
  - `mUseDeck` now starts at `INT_MIN` instead of `0` so "no deck selected" is
    explicit.
  - Group-info lookups now guard against missing/null map entries before reading
    conditions.
  - `SetUseDeck(...)` clears previous deck usage flags before enabling a new
    deck and triggers a Digimon stat refresh event.

### Encyclopedia server packet support

- Added a server-side packet processor for encyclopedia deck selection.
  - File:
    `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/EncyclopediaDeckUsePacketProcessor.cs`
  - Packet enum:
    `GameServerPacketEnum.EncyclopediaDeckUse = 3236`.
  - The current processor reads the requested deck id and logs it. The passive
    deck state is still applied client-side by the current client flow.

### Inventory renewal

- Updated the renewed inventory UI with filtering/search support.
  - Main files:
    - `dmo-client-main/DProject/_Interface/Game/Inventory.cpp`
    - `dmo-client-main/DProject/_Interface/Game/Inventory.h`
    - `dmo-client-main/DProject/_Interface/Game/InventoryContents.cpp`
    - `dmo-client-main/DProject/_Interface/Game/InventoryContents.h`
  - Added search edit/reset controls.
  - Added filter tabs for equipment, consumables, Digimon-related items, and
    miscellaneous items.
  - Added filtered item index mapping so rendering, tooltip, and mouse handling
    point back to the real inventory slot.
  - Added inventory count text and helper tooltip support.
  - Closing the inventory clears filter/search state and resets the selected tab.
- Inventory filtering now combines:
  - item type category,
  - case-insensitive item-name search,
  - open slot count and scroll position.
- Inventory sort packet alignment was adjusted on the server side.
  - File:
    `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Packets/Items/InventorySortPacket.cs`
  - The integer after inventory type is now written as `0` instead of `1`,
    matching the current client expectation for the sort result payload.

## XML Union D-Unit window

The client D-Unit now uses the XML Union implementation as the active D-Unit
window. The previous standalone `UnionDUnit` implementation was removed from
source, project files, includes, factory registration, and hotkey handling.

- Parser: `dmo-client-main/DProject/_Interface/Game/UnionXmlTools.*`
- Window: `dmo-client-main/DProject/_Interface/Game/XmlUnionDUnit.*`
- Window type: `WT_XML_UNION`
- Hotkey: `Shift+O`
- Registration: `cBaseWindow::NewWindow()` creates `cXmlUnionDUnit` for
  `WT_XML_UNION`.
- Include/helper access: `_GameIF.h` includes `XmlUnionDUnit.h` and exposes
  `GetXmlUnionDUnit()`.
- Main title text is `D-Unit`.
- The collection stat panel title is `Efeito Total de Coleta`; display labels
  normalize XML `HP`/`DS` totals to `MaxHP`/`MaxDS`.

Input behavior while the D-Unit window is open:

- `GlobalInput` tracks a separate `m_bXmlUnionHotKeyDown` latch for `Shift+O`.
- When `WT_XML_UNION` is active, macro/mouse input is consumed by the window so
  outside camera zoom, movement, and other external commands do not leak through.
- Mouse wheel is reset after the window consumes it. This prevents the old bug
  where scrolling the Union list also zoomed or moved the game camera.

Runtime XML data location:

- The window loads from `Data\union`.
- For this workspace, the authoritative loose runtime XML folder is:

  ```text
  C:\0-NewDmo\ClientDist\Data\union
  ```

- Do not edit or test against a different `Data\union` folder unless that path
  is explicitly called out first. The user is running the local client from
  `ClientDist`, so Union XML fixes must land there.
- `UnionXmlTools::_LoadDocument()` tries loose disk XML first with
  `TiXmlDocument::LoadFile()`, then falls back to `CsFPSystem` pack lookup. This
  lets `ClientDist\Data\union` override packed files during testing.
- If the client is already open, restart it after XML changes. The XML Union
  window caches loaded groups/card data for the session.

Current Union XML staging lives in:

```text
C:\0-NewDmo\union\xml_generated
```

Source CSVs currently used:

- `C:\0-NewDmo\union\DMO Union - DigimonLines.csv`
- `C:\0-NewDmo\union\DMO Union - DigimonCollections.csv`
- `C:\0-NewDmo\union\DMO Union - Changelog.csv`
- `C:\0-NewDmo\union\DMO Union - Intro.csv`

Generated XMLs currently include line data, collection data, groups, strings,
effects, rewards, filters, source changelog/intro preservation, and an icon
validation report. Important files:

- `DM_Union-DigimonLines.xml`
- `DM_Union-DigimonCollections.xml`
- `DM_Union-Group.xml`
- `DM_Union-Group_Str.xml`
- `DM_Union-Effect.xml`
- `DM_Union-Reward.xml`
- `DM_Union-Reward_Str.xml`
- `DM_Union-Filter.xml`
- `DM_Union-Filter_Str.xml`
- `DM_Union-IconValidation.xml`
- `DM_Union-IconValidation.csv`

XML parser behavior:

- `DM_Union-DigimonLines.xml` produces `Line` groups.
- `DM_Union-DigimonCollections.xml` produces `Collection` groups. Collection IDs
  are offset by `+100000` in the test loader so they cannot collide with line
  IDs.
- `DM_Union-Filter_Str.xml` contributes filter strings, but group names are also
  added as filters so the list is usable even when the string XML is incomplete.
- `<Members>` are read as explicit card entries with `slot`, `name`,
  `iconCandidateId`, and `iconFolder`.
- `<Rewards>` are displayed in the center effects panel.
- `<TotalStat>` values are accumulated by stat name and displayed in the right
  totals panel.
- Empty member names and the `__` placeholder are ignored.

Icon IDs are inferred from unpacked pack03/data11 Digimon assets under:

```text
D:\LK-DMO TOOLS\EXTRATOR-Copiar Daqui\Decryptorv2\data11
```

The CSVs do not currently provide explicit icon IDs, so generated
`iconCandidateId` values are best-effort matches by Digimon/member name. Always
review `DM_Union-IconValidation.csv` before converting XML back to bin:

- `matched`: direct normalized asset-name match.
- `partial`: existing pack03 asset ID, but inferred by partial name; review it.
- `missing`: no matching asset found in the unpacked pack03/data11 tree.

The combine/hacking XMLs are still placeholders because the current CSVs do not
contain item group, rank, ceil, or hacking-info table fields:

- `DM_Union-HackingInfo.xml`
- `DM_UnionCombine-ItemList.xml`
- `DM_UnionCombine-RankList.xml`
- `DM_UnionCombine-ItemGroup.xml`
- `DM_UnionCombine-CeilGroup.xml`

XML Union layout:

- Window size is `896x675`.
- The right-side digivice/totals panel is intentionally shifted `10px` to the
  right through `XML_UNION_RIGHT_PANEL_SHIFT` so the digivice and text do not sit
  against the card scrollbar.
- Left list shows `15` group buttons (`IF_XML_UNION_FILTER_COUNT`).
- Card area shows `8` cards at a time (`2x4`), with up to `32` cached cards per
  selected group.
- The selected group title is drawn as a green slanted tab above the card grid.
  The tab width is calculated from the selected name and capped to avoid overlap.
- Card names are hidden by default and shown only in the hover tooltip.
- Filter rows use hover fill, yellow selected borders, cyan hover borders, and
  bookmark icons.
- Filter bookmark stars are functional favorites:
  - dim/gray star means not favorited,
  - bright yellow star means favorited,
  - clicking the star toggles only the favorite state, not the selected line,
  - favorited rows are shown at the top of the list,
  - a newly favorited row is appended after rows that were already favorited,
  - favorite state is stored in `Temp\XmlUnionDUnitFavorites_<windows-user>.cache`
    beside the running client.
- Favorite cache values are Union `groupId` values, in favorite order, not
  visible names, because displayed filter names can repeat (`Agumon`,
  `Agumon (Black)`, etc.).
- The list scrollbar uses the client assets:
  - `Encyclopedia\Scroll\ScrollUpBtn.tga`
  - `Encyclopedia\Scroll\ScrollDownBtn.tga`
- The card scrollbar appears only when a selected group has more than eight
  cards. Card scrolling moves by whole pages of `8` cards so the card grid does
  not shift by partial rows.

Key UI assets used by `XmlUnionDUnit`:

- `Union\side_bg_a.png`
- `Union\center_bg.png`
- `Union\side_bg.png`
- `Union\side_bg_b.png`
- `Union\list_bg.png`
- `Union\collection_bg_new.tga`
- `Union\collection_frame_rank1_new.png`
- `Union\collection_frame_rank2_new.png`
- `Union\collection_frame_rank3_new.png`
- `Union\digimon_hold_gauge.png`
- `Union\treasure_box_icon.png`
- `Union\filter_icon.png`
- `DungeonUI\dungeon_result_simple_ui.png`
- `System\MoveWindow.tga`
- `Encyclopedia\newencyclopedia\main\exit_btn.png`
- `Encyclopedia\newencyclopedia\main\serch bar.png`
- `Encyclopedia\newencyclopedia\main\sarch.png`
- `Encyclopedia\newencyclopedia\detail\button\top_button.png`
- `Encyclopedia\newencyclopedia\detail\button\bookmark.png`
- `Encyclopedia\newencyclopedia\collection_effect_frame01.png` through
  `collection_effect_frame07.png` for card hover animation.

Digimon portraits:

- The window resolves icon paths from `iconCandidateId`/`iconFolder` first using
  `Data\Digimon\<folder>\<id>_<folder>_small.tga`, `.png`, `.dds`, `.tga`, then
  falls back to `g_pModelDataMng->GetEvoModelIconFile()` and
  `GetSmallModelIconFile()`.
- Icon paths are cached in a static map to avoid repeated pack/file checks while
  scrolling.
- Card portraits are cropped into the circular card ring by drawing horizontal
  sprite slices. This avoids square icon bleed outside the circle.
- Card controls are pre-created once and updated by visibility/state. This was
  done to avoid lag and repeated sprite allocation while scrolling or switching
  groups.

Rank icon atlas:

- The rank badge atlas is
  `Encyclopedia\newencyclopedia\icon\encyclopedia_512.png`.
- The unpacked reference copy was found at:

  ```text
  D:\LK-DMO TOOLS\EXTRATOR-Copiar Daqui\Decryptorv2\data11\interface\encyclopedia\newencyclopedia\icon\encyclopedia_512.png
  ```

- `XmlUnionDUnit` creates a `cSprite` for each card rank badge with size `82x66`
  and changes the texture using explicit `CsRect` crops from the atlas.
- Do not use the `CommonUI\digimon_rank.tga` mapping here. That older atlas is
  used by other status UIs and has different indexing.
- `encyclopedia_512.png` is treated as a `102x102` tile atlas.
- Current critical crop: SSS+ must use tile index `9`, rect
  `CsRect(408, 102, 510, 204)`. This is the cyan/pink Korean SSS+ badge from the
  reference screenshot.
- Current XML Union mapping:
  - rank `<= 0` shows tile `1`, the `N` badge.
  - rank `1..7` maps to `rank - 1`.
  - rank `>= 8` is forced to tile `9`, the Korean `SSS+` badge.
- This mapping was tuned from in-game screenshots. Be careful before changing it:
  the earlier `cImage`/state approach selected the wrong SSS+ tile and could show
  the normal SSS/U+ style badge instead of the Korean badge.

Evolution/card population rules:

- For groups with explicit XML `<Members>`, the cards come from those members.
- For line groups without explicit members, the window resolves the selected
  group to a base Digimon using normalized names and the client evolution table
  (`g_pEvolMng`).
- Duplicate group names, such as repeated `Agumon` or `Gabumon` lines, are
  resolved by duplicate index so each duplicate can bind to a different matching
  evolution base.
- X-antibody line groups are detected by the `Line` type plus X naming, and they
  can resolve directly from `iconCandidateId`/model ID.
- Non-X line groups append related X lines when matching X groups exist in the
  XML. The X entries are treated as normal evolutions once the XML declares the
  X line.
- Collections only display Digimon/evolutions that are declared somewhere in the
  Union XML by group name, member name, or member `iconCandidateId`.
- Card data is cached by `groupId:selectedName` in `m_mapCardCache` to reduce
  the micro-delay when switching between groups.

Performance/lag notes:

- The card frame, portrait slices, hover effect, rank icon, filter row controls,
  and scrollbars are created once and reused.
- Scrolling updates existing controls instead of rebuilding the full layout.
- The hover animation only updates while visible.
- The active XML Union window consumes global input to prevent outside camera
  work from competing with the window scroll/input code.

## XML Union line maintenance notes

Updated on 2026-05-21 after the Agumon/Gabumon/Black/X line fixes.

### Files that matter

- Runtime XML to edit:
  `C:\0-NewDmo\ClientDist\Data\union\DM_Union-DigimonLines.xml`
- Client parser/window code:
  - `dmo-client-main/DProject/_Interface/Game/UnionXmlTools.*`
  - `dmo-client-main/DProject/_Interface/Game/XmlUnionDUnit.*`
- Bin references used for validation:
  - `dmo-server-main-postgres\Bins\data\bin\english\Digimon_List.bin`
  - `dmo-server-main-postgres\Bins\data\bin\english\DigimonEvo.bin`
- Server-side bin parsers that document the binary layouts:
  - `dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/Bins/DigimonListBinLoader.cs`
  - `dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/Bins/DigimonEvoBinLoader.cs`

### What `Digimon_List.bin` and `DigimonEvo.bin` mean

- `Digimon_List.bin` identifies each Digimon form:
  - `Type` / `s_dwDigimonID`: unique Digimon form id. Use this as the safest
    identity for a Union member when several forms share a model.
  - `Model` / `s_dwModelID`: visual model/icon identity. This can repeat across
    different forms. Example: `31043` is used as the model for both normal
    `Agumon(Black)` and `Agumon(Black)[BlitzGreymon]`.
  - `Name`: display/source name.
  - `Rank`: client rank value used for the badge.
- `DigimonEvo.bin` identifies evolution trees:
  - top-level key is the base `Type`.
  - each evolution form has `Type`, `EvoSlot`, `EnableSlot`, open requirements,
    and up to nine outgoing target ids.
  - `EnableSlot=0` means that form exists in the tree but is not an enabled
    progression for that line. Do not blindly add disabled forms to a Union line
    unless the line is intentionally meant to expose hidden/unused evolutions.

### Bin comparison results used for the current XML

- `31001 Agumon` is the normal DATS Agumon line. Enabled forms are:
  `Agumon -> GeoGreymon -> RizeGreymon -> ShineGreymon -> ShineGreymon:
  BurstMode`.
  - `DigimonEvo.bin` also contains disabled classic branches under this base:
    `Greymon`, `MetalGreymon`, `WarGreymon`, and `SkullGreymon` have
    `EnableSlot=0` in the `31001` tree.
  - Therefore the normal `Agumon Line 2` in Union XML must not show
    `MetalGreymon`, `SkullGreymon`, or `WarGreymon`.
- `31007 Agumon(Classic)` is the classic Agumon line. Enabled forms are:
  `Agumon(Classic) -> Greymon -> MetalGreymon -> WarGreymon`, plus
  `VictoryGreymon`, `Omegamon`, and the X branch:
  `Agumon X -> Greymon X -> MetalGreymon X -> WarGreymon X -> Omegamon X`.
- `33080 Agumon(Black)[BlitzGreymon]` uses model `31043`, but it is not the same
  base as normal `31043 Agumon(Black)`. Its enabled tree is:
  `Agumon(Black)[BlitzGreymon] -> Greymon(Black) -> MetalGreymon(Black) ->
  Blitz Greymon -> Omegamon Alter-S -> Omegamon Alter-B`.
- `31043 Agumon(Black)` is the normal black Agumon/Zwart tree:
  `Agumon(Black) -> Greymon(Black) -> MetalGreymon(Black) ->
  WarGreymon(Black) -> Omegamon Zwart -> Omegamon ZwartD`.
- `31068 Gabumon(Black)` mirrors the black Gabumon/Zwart side:
  `Gabumon(Black) -> Garurumon(Black) -> WereGarurumon(Black) ->
  MetalGarurumon(Black) -> Omegamon Zwart -> Omegamon ZwartD`.
- `33082 Gabumon[CresGarurumon]` is the Cres/Alter side:
  `Gabumon[CresGarurumon] -> Garurumon -> WereGarurumon -> Cres Garurumon ->
  Omegamon Alter-S -> Omegamon Alter-B`.
- `71109` is the Alter-B id used by the active `33080/33082` evolution trees.
  `71105` also exists in `Digimon_List.bin`, but it is not the target used by
  these trees.

### Current hand-authored line fixes

- `count` in `DM_Union-DigimonLines.xml` is `156` and must match the actual
  number of `<Line>` elements.
- First lines are intentionally ordered:
  - `Agumon Line 1`: classic Agumon with X evolutions, base icon `31007`.
  - `Agumon Line 2`: normal DATS Agumon, base icon `31001`, no classic/Skull/X
    forms.
  - `Agumon Line 3`: classic Agumon displayed as `Agumon`, same classic
    non-X members as line 1, placed third for UI order.
- Agumon Black/Gabumon Black duplicate groups were split with explicit members
  so the two list entries do not resolve to the same inferred line:
  - `Agumon Black Line 1`: ends at `Omegamon Zwart`.
  - `Agumon Black Line 2`: ends at `Omegamon ZwartD`.
  - `Gabumon Black 1`: ends at `Omegamon Zwart`.
  - `Gabumon Black 2`: ends at `Omegamon ZwartD`.
- Blitz/Cres/Alter groups are separate from the normal black Zwart groups:
  - `Agumon Black Blitz 1`: ends at `Omegamon Alter-S`.
  - `Agumon Black Blitz 2`: ends at `Omegamon Alter-B`.
- X-antibody entries were removed as standalone new Union lines. They are now
  inserted into the respective base Digimon groups:
  - `Agumon Line 1`
  - `Gabumon 1`
  - `Guilmon 1`
  - `Dorumon Doruga`
  - `Dorumon Dex`
  - `Dorumon Alpha`
  - `FanBeemon`
  - `Ryudamon`
  - `Dracmon`

### How to add a new Digimon Union line

1. Find the Digimon in `Digimon_List.bin` by name.
   - Record both `Type` and `Model`.
   - If multiple rows have similar names or the same model, prefer `Type` for
     line identity and use the exact bin name to avoid collisions.
2. Open the matching base entry in `DigimonEvo.bin`.
   - Use the base `Type`, not the model id.
   - Sort forms by `EvoSlot`.
   - Include only enabled forms (`EnableSlot != 0`) unless the desired Union
     line explicitly needs disabled/hidden forms.
3. Compare the outgoing targets from each enabled form.
   - This catches side branches such as Burst Mode, Jogress, X evolutions, and
     Alter/Zwart paths.
   - If a target belongs to a different base tree, verify it against both trees
     before adding it.
4. Add a `<Line>` in `ClientDist\Data\union\DM_Union-DigimonLines.xml`.
   - Choose a unique `id`.
   - Keep `name` user-facing and searchable.
   - Set `iconCandidateId` to the base form identity that resolves the right
     portrait. If the form shares model with another form, use explicit members
     and exact names.
   - Add a `<Members>` block with one `<Member>` per visible card:

     ```xml
     <Member slot="1" name="Agumon" iconCandidateId="31001" iconStatus="matched" iconFolder="" />
     ```

5. Update the root `count` attribute if the number of `<Line>` elements changed.
6. Restart the client before validating. The XML window may reuse cached card
   data until the process reloads.

### How to add evolutions to an existing Union line

1. Verify the evolution belongs to that exact base in `DigimonEvo.bin`.
   - Do not rely on the displayed name alone.
   - Do not rely on `Model` alone; repeated models caused the Agumon Black Blitz
     bug.
2. Insert the new `<Member>` into the existing line's `<Members>` block.
   - Keep slots sequential and in UI order.
   - For X forms, add them inside the original base line unless the target data
     really has a separate Union group.
3. Adjust line metadata if needed:
   - `unlocks`, `myUnlocks`, and `levelReq*` are display/progress fields.
   - Keep them consistent with the intended number of cards.
4. If two Union entries have the same visible name, force both lines to use
   explicit `<Members>`. The fallback duplicate-name resolver is useful, but it
   should not be trusted for ambiguous families like Agumon/Gabumon Black.

### Quick validation commands

Check the first Agumon/Black lines:

```powershell
rg -n "Agumon Line|Agumon Black|Gabumon Black|Omegamon Alter|Omegamon Zwart|ShineGreymon" `
  "C:\0-NewDmo\ClientDist\Data\union\DM_Union-DigimonLines.xml"
```

Build and copy the client after C++ layout/parser changes:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /p:Configuration=Release_English /p:Platform=Win32 /m:1 /nologo /verbosity:minimal

Copy-Item -LiteralPath "C:\Release\GSP\DProject_R_ENG.exe" `
  -Destination "C:\0-NewDmo\ClientDist\DProject_R_ENG.exe" -Force
```

## Known risky areas

- PostgreSQL migration can change behavior where old MySQL code relied on:
  - unsigned/signed width,
  - implicit defaults,
  - zero dates/nulls,
  - case-insensitive comparisons,
  - byte-array or blob conversion.
- The client is very sensitive to packet byte order. One extra byte can cause:
  - disconnects on server select,
  - white screen on character select,
  - inventory desync,
  - crashes when equipping or moving items.
- Build outputs are large and should not be committed:
  - `.pch`
  - `.pdb`
  - `.obj`
  - generated `.exe`
  - temporary `Release_English/`, `Give_English/`, `.vs/`.

## Useful commands

Build client:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Start PostgreSQL stack:

```powershell
cd C:\0-NewDmo\dmo-server-main-postgres
docker compose up -d --build
```

Tail game logs:

```powershell
docker logs --tail 200 dmo-pg-game
```

Query PostgreSQL:

```powershell
docker exec dmo-postgres psql -U dso -d dso
```

Check active containers:

```powershell
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}"
```

## XML Union backend foundation

The backend now has a lightweight persistence layer for D-Unit/XML Union
progress. Keep the large Digimon/collection definitions in XML files and send
only compact progress fields through packets.

Database state:

- Table: `Character_XmlUnionProgress`.
- One row per character, linked by `CharacterId` to `Character_Tamer`.
- Stored fields:
  - `Level`: current XML Union level.
  - `CurrentExperience`: current progress inside the level.
  - `RequiredExperience`: target amount for the current level.
  - `ClaimedRewardMask`: bit mask for claimed reward tiers.
  - `CreatedAt` / `UpdatedAt`.
- Migration: `20260521143000_AddCharacterXmlUnionProgress`.
- The game schema validator now requires this table before server startup, so
  apply the migration before enabling the feature on a live database.

Server model/files:

- DTO: `CharacterXmlUnionProgressDTO`.
- Domain model: `CharacterXmlUnionProgressModel`.
- EF mapping: `CharacterXmlUnionProgressConfiguration`.
- Repository update method:
  `ICharacterCommandsRepository.UpdateCharacterXmlUnionProgressAsync`.
- MediatR command:
  `UpdateCharacterXmlUnionProgressCommand`.
- `CharacterModel.EnsureXmlUnionProgress()` creates/defaults the model when an
  old character has no row yet.
- `CharacterQueriesRepository.GetCharacterByIdAsync()` includes
  `XmlUnionProgress`, so login loads the current state with the character.

Backend XML data:

- Preferred folder: `dmo-server-main-postgres\xml\union`.
- Loader: `UnionXmlAssetLoader`.
- Search fallback, in order:
  - `xml\union`
  - `Data\union`
  - `ClientDist\Data\union`
- Parsed files:
  - `DM_Union-DigimonLines.xml`
  - `DM_Union-DigimonCollections.xml`
  - `DM_Union-Effect.xml`
  - optional `DM_Union-Level.xml`
- The loader reads `<TotalStat type="" value="">` from lines/collections and
  stores totals as `UnionXmlStatusBonus`. This prepares the status reward data
  without sending the whole XML to the client.
- The loader also builds validated D-Unit collection groups for server-side
  bonus calculation. Invalid groups are skipped with `[DUnit]` warnings instead
  of blocking server startup.
- Optional level format:

  ```xml
  <UnionLevels>
    <Level id="0" requiredExperience="10000" />
    <Level id="1" requiredExperience="15000" />
  </UnionLevels>
  ```

Lightweight packet design:

- Client request opcode: `XmlUnionProgressRequest = 11050`.
- Client claim opcode: `XmlUnionRewardClaim = 11051`.
- Server collection state response packet: `XmlUnionCollectionInfoPacket`,
  packet number `11053`.
- Server progress response packet: `XmlUnionProgressPacket`, packet number
  `11050`.
- Server claim result packet: `XmlUnionRewardClaimResultPacket`, packet number
  `11052`.
- Progress response payload is intentionally small:
  - `byte Level`
  - `int CurrentExperience`
  - `int RequiredExperience`
  - `ushort ProgressPercentBasisPoints` (`10000` = `100.00%`)
  - `ushort ClaimedRewardMask`
- Do not send XML line/member definitions through these packets. The client
  still loads the collection structure from `ClientDist\Data\union`; packet
  `11053` only overlays server-owned state and calculated bonuses:
  - group id using the same client id space (`Collection` ids are sent as
    `id + 100000`, matching `UnionXmlTools`);
  - group completed flag, owned/valid member count, total owned level, and
    transcendence summary;
  - per required Digimon: Digimon id, XML slot, owned flag, owned level,
    evolution unlocked flag, transcended flag, and requirement pass flags;
  - per effect: condition index, active flag, effect type, and value;
  - aggregate bonus values in the display order `HP`, `DS`, `AT`, `DE`, `EV`,
    `HT`, `CT`, `EXP`, `BL`, `SCD`, `Basic`, followed by attribute/element
    bonus totals for future UI expansion.

Important client/server safety note:

- The C++ client now handles server packets `11050`, `11052`, and `11053`.
- `cXmlUnionDUnit::Create()` sends `SendXmlUnionProgressRequest()` when the
  D-Unit opens.
- Clicking a reward button sends `SendXmlUnionRewardClaim(rewardIndex)` for
  opcode `11051`.
- The receive handlers are in `network/cCliGameReceive.cpp` and dispatch into
  `cXmlUnionDUnit::RecvServerProgress()` /
  `cXmlUnionDUnit::RecvRewardClaimResult()` /
  `cXmlUnionDUnit::RecvCollectionInfo()`.
- `XmlUnionDUnit` no longer trusts local XML totals/currentLevel values for
  visible progress: cards are always drawn from XML, but the Digimon icon and
  level only light up from server state. Missing/locked Digimon remain as a
  dim empty frame with the normal name/tier context available from the local
  XML/card data.
- The active docker compose mounts `dmo-server-main-postgres\xml` as
  `/app/xml` for `game` and `routine`, so backend XML files should be added
  under `dmo-server-main-postgres\xml\union`.
- On 2026-05-21 the PostgreSQL database `dso` was updated manually with the SQL
  equivalent of migration `20260521143000_AddCharacterXmlUnionProgress`, and
  `dmo-pg-game` / `dmo-pg-routine` were rebuilt/recreated.

Collection bonus behavior:

- Existing status systems, such as seal/title/buff logic, recalculate model
  status server-side and then send `UpdateStatusPacket`.
- D-Unit collection bonuses are calculated server-side by
  `DUnitCollectionService`.
- Runtime bonus storage lives in `CharacterModel.DUnitCollectionBonus`.
- Partner final stats now include D-Unit bonuses for `HP`, `DS`, `AT`, `DE`,
  `HT`, `CT`, `EV`, and `BL`.
- `SCD` is applied through the partner SCD path using the same scale as the
  existing SCD bonuses.
- `EXP` is added to the character EXP bonus path.
- Attribute/element skill damage bonuses are kept separate from raw stats and
  only apply when the partner base attribute/element matches the effect type.
- Recalculation currently runs:
  - during initial character load/login, after archive Digimon are hydrated;
  - when the Digimon Archive packet loads archive Digimon, followed by
    `UpdateStatusPacket`;
  - when the D-Unit window requests progress (`11050`), which recalculates
    once, clears/reapplies runtime bonuses through `ApplyBonuses(...)`, sends
    `UpdateStatusPacket`, then sends `XmlUnionCollectionInfoPacket` (`11053`).
- Still add future event hooks for level up, transcendence, evolution unlock,
  new Digimon acquisition, deletion/removal, and config reload.

D-Unit collection XML analysis from 2026-05-21:

- Backend XML source copied from `ClientDist\Data\union` to
  `dmo-server-main-postgres\xml\union`, which Docker mounts as `/app/xml/union`.
- `DM_Union-DigimonLines.xml` contains `156` line groups, `126` explicit member
  entries, `155` rewards blocks, `620` reward entries, and `25` `TotalStat`
  entries.
- Only `17` line groups currently have explicit `<Members>`. The remaining
  line groups are not safe for server bonus validation because there is no
  authoritative required-Digimon list.
- `Line id=156` / `Agumon Line 3` has members but no rewards, so the backend
  skips it for collection bonuses until effects are added.
- `DM_Union-DigimonCollections.xml` contains `61` collection groups, `343`
  member entries, `244` rewards, and `25` `TotalStat` entries.
- Collection member `iconCandidateId` values are often icon/model-style ids,
  not `Digimon_List.bin` type ids. The backend resolves a member by valid
  `Digimon_List.bin` type first, then by exact normalized Digimon name if unique.
- Many collection names are shorthand or inconsistent with `Digimon_List.bin`
  names, so unresolved members are logged as `DigimonId not found`.
- `DM_Union-Effect.xml` contains `66` effects. Stats/effect types currently
  seen: `HP`, `DS`, `AT`, `DE`, `HT`, `CT`, `EV`, `BL`, `EXP`, `SCD`, `Basic`,
  `Vaccine`, `Data`, `Virus`, `Unknown`, `Fire`, `Water`, `Ice`, `Wind`,
  `Thunder`, `Light`, `Dark`, `Land`, `Wood`, and `Steel`.
- `DM_Union-Group.xml` contains `216` groups with no duplicate group ids.
- `DM_Union-Reward.xml` contains `864` rewards, matching `216` groups with
  `4` rewards each.
- Current Docker load result after validation:
  `[DUnit] XML summary: files=17, groupsTotal=217, groupsValid=73, groupsInvalid=144, members=469, effects=66, unknownEffects=0, missingDigimonIds=102, source=/app/xml/union`.
- The loader still emits the shorter compatibility line:
  `[DUnit] Loaded groups: 73. Effects: 66. Invalid groups: 144. Source: /app/xml/union`.

Persistence decision:

- No new database table was added for completed D-Unit collection groups or
  active collection bonus cache in this step.
- The current server calculation is dynamic from real owned Digimon state:
  active/inventory Digimon plus hydrated Digimon Archive entries.
- Recalculation is not cumulative: `DUnitCollectionService.ApplyBonuses()` builds
  a fresh aggregate result, then calls `ClearDUnitCollectionBonus()` before
  `SetDUnitCollectionBonus()`.
- `DUnitCollectionService.CalculateDUnitCollectionBonus()` also guards applied
  completed groups by `SourceType:GroupId`; if a duplicate complete group ever
  slips past XML validation, it logs a warning and ignores the duplicate in the
  same calculation.
- This matches the desired rule that a card only lights up when the player
  actually owns that Digimon, and the displayed level should come from the owned
  Digimon in active inventory or archive/warehouse data.
- If the calculation becomes heavy later, add cache tables such as
  `DUnitCollectionProgress` and `DUnitCollectionBonusCache`, but keep the server
  able to rebuild them from real Digimon ownership.

Manual validation commands:

```powershell
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj" -v:minimal
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Routine.Host\DigitalWorldOnline.Routine\DigitalWorldOnline.Routine.csproj" -v:minimal
docker compose -f "C:\0-NewDmo\dmo-server-main-postgres\docker-compose.yml" up -d --build game routine
docker logs --tail 300 dmo-pg-game | Select-String "DUnit|Loaded Digimon_List|Loaded DigimonEvo|GameServer started"
```

## Maintenance notes

- Treat `dmo-server-main-postgres` as the only active server tree.
- Prefer fixing packet alignment at the source instead of masking symptoms in the
  UI.
- When adding a new feature:
  - document the client packet ID,
  - document the server enum,
  - document the processor,
  - include one known-good payload shape.
- Do not commit local compiled client executables. Rebuild them from source.

## GM panel, hatch, and archive status from 2026-05-22

Current PR:

- Open PR: https://github.com/leo-pereira-dev/LK-DMO/pull/1
- Branch: `Branch_dunit-union-system`
- Base: `main`

GM panel:

- The client GM panel work is in progress in the `DProject/_Interface/Game`
  UI code.
- The panel was expanded from a single Make Item screen toward a broader
  administration panel with tabbed actions such as Make Item, debug character,
  level actions, AllEvo, and Bits.
- Keep future GM panel additions aligned with existing client UI assets and
  avoid introducing runtime-only assets into source control.

Hatch system:

- Hatch packet handling was adjusted server-side so successful hatches refresh
  the client state without requiring a channel swap.
- Client/server validation used:

  ```powershell
  dotnet build "C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj"
  docker compose -f "C:\0-NewDmo\dmo-server-main-postgres\docker-compose.yml" up -d --build account character game
  & "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
    "C:\0-NewDmo\dmo-client-main\DProject.sln" `
    /p:Configuration=Release_English /p:Platform=Win32 /m
  ```

Digimon Archive / Warehouse:

- The local test account `test2` had an archived test Digimon removed from the
  local PostgreSQL database on 2026-05-22:
  - `Digimon_Digimon.Id = 3`
  - `Name = test2`
  - `BaseType = 31001`
  - archive row removed from `Character_DigimonArchiveItem`
  - dependent evolution, attribute, clone, location, and buff rows removed.
- The archive capacity row remains; the local archive has no occupied Digimon
  row after cleanup.
- The Transcend/Academy training tab is temporarily disabled in
  `DProject/_Interface/Game/DigimonArchiveViewer.cpp`.
- The tab button is disabled, and any accidental call to
  `OnClickShowTranscendViewer()` returns to the normal Storage view instead of
  opening training.
- This is intentionally temporary. Re-enable training only after the archive
  packet/data flow is stable end-to-end.

Build state after the temporary training disable:

- Client build: `Release_English|Win32` passed with `0` errors.
- The built exe was copied locally to
  `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe`.
- Server Docker rebuild of `account`, `character`, and `game` passed and all
  three containers were observed `Up`:
  - `dmo-pg-account` on host port `17029`
  - `dmo-pg-character` on host port `17050`
  - `dmo-pg-game` on host port `17608`

## D-Unit follow-up fixes from 2026-05-21

Agumon Line 2 test requirements:

- The non-classic `Agumon Line 2` is the normal Agumon route with
  `Agumon -> GeoGreymon -> RizeGreymon -> ShineGreymon ->
  ShineGreymon: BurstMode`.
- Runtime XMLs were synchronized in both places:
  - client: `ClientDist\Data\union`
  - server: `dmo-server-main-postgres\xml\union`
- Updated files:
  - `DM_Union-DigimonLines.xml`
  - `DM_Union-Group.xml`
- The line now uses:

  ```xml
  unlocks="4"
  membersRequired="4"
  levelReq1="480"
  levelReq2="480"
  ```

- `unlocks` is kept for the client text/goal display.
- `membersRequired` is required server-side because
  `UnionXmlAssetLoader` reads `membersRequired`; if it is missing, the server
  falls back to the full required member count.
- `myUnlocks` was not changed because it is display/source sample data, not the
  server-side requirement.

Important D-Unit evolution-display fix:

- A bug appeared where evolving Agumon into one form made another required form
  disappear from D-Unit.
- Root cause: the server calculation accepted `CurrentType` as proof of a form,
  but starter partners can evolve through normal starter routes without every
  normal form being persisted as `Unlocked > 0` in the same way as item-unlocked
  evolutions.
- Correct behavior matches the Digimon Information `Evolution & Riding` screen:
  the D-Unit should care whether the evolution slot/route is available for the
  owned Digimon, not whether that form is the current evolved body.
- `DUnitCollectionService.IsEvolutionUnlocked(...)` now:
  - returns true for base/current type as before;
  - returns true for persisted unlocked evolutions as before;
  - additionally treats normal starter evolutions for base types
    `31001`, `31002`, `31003`, and `31004` as available when the target form is
    in the starter's evolution tree and the Digimon level meets the tree
    `UnlockLevel`.
- The starter fallback is intentionally limited to normal ranks
  `Rookie` through `Mega`; it does not auto-unlock `BurstMode`, `Jogress`,
  `X`, `Capsule`, `Spirit`, or other special routes.

Server packet refresh after evolution:

- `PartnerEvolutionPacketProcessor` now injects `DUnitCollectionService`.
- After a successful evolve/devolve flow and after status is recalculated, the
  server recalculates D-Unit collection bonuses, clears/reapplies runtime bonus
  state through `ApplyBonuses(...)`, sends `UpdateStatusPacket`, and also sends
  `XmlUnionCollectionInfoPacket`.
- This keeps the D-Unit UI synchronized immediately after changing evolution
  form, without relying on closing/reopening the window.
- This also protects against duplicated D-Unit bonuses because the normal
  `ApplyBonuses(...)` path always clears old D-Unit bonus state before applying
  the fresh aggregate.

Validation done after the follow-up:

```powershell
dotnet build .\DigitalWorldOnline.sln -c Release
docker compose up -d --build game routine
docker compose logs --tail=80 game | Select-String "DUnit|error|fail|exception|Applied bonuses|XML summary|Loaded groups"
```

- Server solution build passed in `Release`.
- `game` and `routine` containers rebuilt and started.
- Boot logs showed D-Unit XML loading successfully with the known validation
  summary:
  `[DUnit] XML summary: files=17, groupsTotal=217, groupsValid=73, groupsInvalid=144, members=469, effects=66, unknownEffects=0, missingDigimonIds=102, source=/app/xml/union`.
