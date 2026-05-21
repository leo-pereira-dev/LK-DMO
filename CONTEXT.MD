# LK-DMO Technical Context

Last updated: 2026-05-21

This document is the working memory for the LK-DMO workspace in `C:\0-NewDmo`.
It summarizes the main fixes, migration work, build notes, and packet/protocol
map discovered while adapting the client and server.

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

### Client/server bootstrap

- Read `source-client` context and used the compiling source under `D:\OpenDmo`
  as a reference for client library/build behavior.
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
  - timestamp `2026-05-19 21:30:56`, size `10842624`.
  - Copy to `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`
    failed because the running client process held the file open and could not be
    stopped from the current PowerShell session (`Access denied`).
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

### D-Unit hotkey

- Added a keyboard toggle for the D-Unit/Union window in `GlobalInput`.
  - `Shift + U` toggles `WT_UNION`.
  - `m_bUnionHotKeyDown` prevents repeated toggles while the key is held.

## Recent client UI work - D-Unit / Union window

Completed on 2026-05-21:

- Added a new D-Unit/Union window registered as `WT_UNION`.
  - Window entry point is `cUnionDUnit`.
  - Main files:
    - `dmo-client-main/DProject/_Interface/Game/UnionDUnit.cpp`
    - `dmo-client-main/DProject/_Interface/Game/UnionDUnit.h`
  - The window is included through `_GameIF.h`, created from
    `_BaseWindow.cpp`, and added to `DProject.vcxproj`.
- Added `CsUnionTableMng` for reading Union/D-Unit bin data from the client
  packs.
  - Main files:
    - `dmo-client-main/LibProj/CsFileTable/UnionTableMng.cpp`
    - `dmo-client-main/LibProj/CsFileTable/UnionTableMng.h`
  - The manager is wired through `FileTableMng.cpp`, `nsCsFileTable.*`,
    `stdafx.h`, and the `CsFileTable` project files.
  - It loads the Union table bins from `Data\Bin\Table` and string bins from
    the active language path.
  - It detects zlib-compressed payloads, extracts the internal payload/key area,
    and exposes parsed groups, effects, rewards, filters, and localized names.
- The D-Unit screen uses the same large encyclopedia-style window sizing:
  `886x768`, centered on screen, built for `Release_English|Win32`.
- Visual assets currently referenced by the D-Unit screen include:
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
  - `DungeonUI\dungeon_result_simple_ui.png`
  - encyclopedia search/list/close/bookmark button assets reused for matching
    UI style.
- The right-side digivice panel was aligned and scaled using the available
  extracted assets.
  - `DungeonUI\dungeon_result_simple_ui.png` provides the blue digivice body.
  - `Union\digimon_hold_gauge.png` provides the inner gauge ring.
  - `Union\treasure_box_icon.png` uses a `56x61` frame crop; earlier `40x44`
    cropping cut the icon.
- Card Digimon portraits are rendered behind the card frame.
  - The portrait is cropped into a circle by drawing horizontal sprite slices
    whose width follows the circle radius.
  - This avoids the original square icon bleeding outside the card's circular
    portrait ring.
  - Final portrait center was tuned downward to sit inside the ring.
- The D-Unit title is rendered slightly above the card area and uses a two-pass
  text draw with a 1px horizontal offset, because the built-in bold level made
  this font look thinner instead of heavier.
- Verified build command:

  ```powershell
  & "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
    "C:\0-NewDmo\dmo-client-main\DProject.sln" `
    /p:Configuration=Release_English /p:Platform=Win32 /m:1 /nologo /verbosity:minimal
  ```

  Expected output:

  ```text
  C:\Release\GSP\DProject_R_ENG.exe
  ```

Notes for future cleanup:

- Some visible D-Unit values are still fallback/mock display values while the
  parse/layout work is stabilized.
- The local root `union/` folder is an extracted/decompiled analysis folder and
  is ignored by Git. Keep source changes in the client tree instead.

## XML Union D-Unit test window

The client now has a separate XML-only Union/D-Unit window for testing the new
Union data without touching the old Union implementation:

- Parser: `dmo-client-main/DProject/_Interface/Game/UnionXmlTools.*`
- Window: `dmo-client-main/DProject/_Interface/Game/XmlUnionDUnit.*`
- Window type: `WT_XML_UNION`
- Hotkey: `Ctrl+O`
- Registration: `cBaseWindow::NewWindow()` creates `cXmlUnionDUnit` for
  `WT_XML_UNION`.
- Include/helper access: `_GameIF.h` includes `XmlUnionDUnit.h` and exposes
  `GetXmlUnionDUnit()`.
- Important: keep this path independent from `cUnionDUnit`/`WT_UNION`. The old
  Union window remains available separately for comparison.

Input behavior while the XML Union window is open:

- `GlobalInput` tracks a separate `m_bXmlUnionHotKeyDown` latch for `Ctrl+O`.
- When `WT_XML_UNION` is active, macro/mouse input is consumed by the window so
  outside camera zoom, movement, and other external commands do not leak through.
- Mouse wheel is reset after the window consumes it. This prevents the old bug
  where scrolling the Union list also zoomed or moved the game camera.

Runtime XML data location:

- The window loads from `Data\union`.
- `UnionXmlTools::_LoadDocument()` tries loose disk XML first with
  `TiXmlDocument::LoadFile()`, then falls back to `CsFPSystem` pack lookup. This
  lets `C:\Release\GSP\Data\union` override packed files during testing.
- Runtime XML copies were placed under:
  - `C:\0-NewDmo\ClientDist\Data\union`
  - `C:\Release\GSP\Data\union`

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

- Window size is `886x675`.
- Left list shows `15` group buttons (`IF_XML_UNION_FILTER_COUNT`).
- Card area shows `8` cards at a time (`2x4`), with up to `32` cached cards per
  selected group.
- The selected group title is drawn as a green slanted tab above the card grid.
  The tab width is calculated from the selected name and capped to avoid overlap.
- Card names are hidden by default and shown only in the hover tooltip.
- Filter rows use hover fill, yellow selected borders, cyan hover borders, and
  bookmark icons.
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

- `XmlUnionDUnit` creates a `cImage` for each card rank badge with size `82x66`
  and `SetTexToken(CsPoint(102, 102))`.
- Do not use the `CommonUI\digimon_rank.tga` mapping here. That older atlas is
  used by other status UIs and has different indexing.
- Runtime-observed `encyclopedia_512.png` render indexes in this client:
  - `9`: `SSS`
  - `10`: `SSS+` Korean badge
  - `11`: `U+`
- Current XML Union mapping:
  - rank `<= 0` shows the `N` badge state.
  - rank `>= 10` is forced to state `10`, the Korean `SSS+` badge.
  - rank `1..9` maps to `rank - 1`.
- This mapping was tuned from in-game screenshots. Be careful before changing it:
  switching to the Encyclopedia helper-style `rank + 1` made rank `10` display
  as `U+`, and switching to state `9` made it display as normal `SSS`.

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
