# LK-DMO Technical Context

Last updated: 2026-05-26

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

## Production VPS is the priority for live bugs

For current player-facing issues, inspect and fix the production VPS first
unless the user explicitly says the problem is local-only.

Production target:

```text
VPS: ubuntu@104.234.41.94
Remote root: /home/ubuntu/dmo-server-main-postgres
Compose env: .env.production
Local source: C:\0-NewDmo\dmo-server-main-postgres
```

Start with:

```bash
cd /home/ubuntu/dmo-server-main-postgres
sudo docker compose --env-file .env.production ps
sudo docker compose --env-file .env.production logs --tail=200 account character game
```

Use these deploy/restart patterns:

```bash
sudo docker compose --env-file .env.production up -d --build game
sudo docker compose --env-file .env.production up -d --build character game
sudo docker compose --env-file .env.production restart account character game
```

Secrets are stored only in ignored local helpers such as
`C:\0-NewDmo\vps-game-logs.local.ps1`; never reveal or commit them. Before
overwriting production files, create a timestamped backup under:

```text
/home/ubuntu/dmo-server-main-postgres/deploy-backups/
```

## 2026-05-25/26 live incident: sync, crashes, stats, and login

Detailed standalone handoff for the player visibility, trade, and party work:

```text
C:\0-NewDmo\LiveSyncTradePartyContext.md
```

### Status packet / equipment / tooltip resolved on 2026-05-26

The main Digimon status discrepancy is currently resolved.

- Production `game` was rebuilt on the VPS after correcting the status packet
  and equipment total paths.
- `UpdateStatusPacket` now sends large partner/tamer values using `int32` where
  needed, and sends partner AS as `float`.
- The client `RecvAllStat` parser supports the extended layout with `AT int32`,
  `DE int32`, `AS float`, `HT int32`, and the 13-value `DetailInfo` tail.
- Server equipment/accessory/chipset/digivice helpers now return `int` totals
  instead of clamping totals to `short`.
- The Digimon status tooltip now reconciles its local breakdown with the
  authoritative value from the server, so `ALL` should match the visible status
  even when the client cannot split a newer server-side source into a specific
  local line.
- Handoffs updated:
  - `C:\0-NewDmo\HANDOFF_DIGIMON_STATUS_AS_PARSE.md`
  - `C:\0-NewDmo\HANDOFF_AT_NEGATIVO_STATUS_TOOLTIP.md`
  - `C:\0-NewDmo\DetailInfo.md`

Current state from the latest pass:

- player visibility/sync was fixed enough for the user to confirm both clients
  rendered each other in the same place;
- sensitive movement/facing sync was added on 2026-05-26: client sends
  facing-only updates with a 0.03 rad threshold, server echoes
  `pSync::Rotation = 0x7F` in packet `1006`, and `game` was rebuilt on the VPS;
- precise movement target sync was tried on 2026-05-26 04:11 BRT and rolled
  back on 2026-05-26 04:22 BRT after the client froze when another player
  entered the same channel/field. The rollback keeps rotation sync but removes
  float `X/Y` from `1004` and removes `pSync::PrecisePosition = 0x7E` from
  `1006`;
- trade open and item display were fixed;
- trade final completion crash on packet `1504` was patched in the client and
  the client was rebuilt/copied at `2026-05-26 02:29`, but still needs the next
  live retest;
- party is still bugged and must be treated as active development.

This is the current high-priority production context for the next agent. The
user was testing with friends on the live VPS. When players approached each
other in the same channel/location, they could not see each other, mobs became
unattackable, channel changes failed, and the server appeared to lag. Multiple
clients also crashed while network packets were being executed.

### Crash/log symptoms reported by users

Observed client crash contexts:

```text
packet=3969 body_bytes=41 total_len=43
packet=1006 body_bytes=23 total_len=25
packet=1308 body_bytes=27 total_len=29
EXCEPTION_ACCESS_VIOLATION 0xc0000005
```

Important examples:

- `2026-05-25 22:08:26`: crash while executing packet `3969`, right after
  repeated accessory/item use attempts and sync packets.
- `2026-05-25 22:20:51` and `2026-05-25 23:02:12`: crashes around packet
  `1006` execute-end.
- `2026-05-25 23:11:42`: crash around packet `1308` execute-end while testing
  character selection/login flow.

The user also reported a player leveling/upgrading an item very quickly before
one crash. Do not assume that item speed alone is the root cause; keep checking
packet layout, object sync, race conditions, null checks, game logic, pipelines,
and overflow.

### Client object type / sync root cause found

The C++ client `cType` contract was inconsistent:

- `pop<cType>` reads/writes an 8-byte object type.
- `cType::SetTypeAll` and `GetTypeAll` were using `u4`.

This dropped the high 32 bits of object type/model information and could break
object creation/sync when tamers, digimons, mobs, or other objects entered view.

Files changed:

```text
C:\0-NewDmo\dmo-client-main\common_vs2019\pType.h
C:\0-NewDmo\dmo-client-main\common_vs2019\pType.cpp
```

Required behavior:

- `SetTypeAll(u8)`
- `GetTypeAll()` returns `u8`

Keep this aligned with every packet/object reader that uses `cType`.

### Client crash/sync logs added

Logging was added so the next live test can show exactly where object sync
breaks:

```text
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameSync.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGateReceive.cpp
```

Instrumented areas:

- `RecvSyncData`
- `SyncNewObject`
- `SyncInObject`
- `SyncInTamer`
- `SyncInDigimon`
- `SyncInMonster`
- most `SyncInBuff` debug chat spam was moved to `CrashLogger::LogMessage`
- `RecvTamerSelectSuccess` / packet `1308` parse logging

When testing, collect the latest client `message.txt` and crash dump path under
`Log\CrashDumps\`.

### Server visibility / map sync guard

Server-side map visibility was hardened:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Models\Maps\MapInstanceTamerBehavior.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\MapServers\MapServerTamerOperation.cs
```

Behavior to preserve:

- `ShowTamer` creates `TamersView[tamerKey]` if missing.
- Duplicate view entries are avoided.
- `LoadTamerPacket` and `LoadBuffsPacket` are serialized before send.
- `PKT-TRACE ShowTamer...` logs include packet size/prefix data before sending.

This was deployed to the VPS with:

```bash
docker compose --env-file .env.production up -d --build game
```

Remote backup from that pass:

```text
/home/ubuntu/dmo-server-main-postgres/deploy-backups/20260525-232841-sync-pkt-trace/files.tgz
```

### Sensitive movement / facing sync

After the visibility fix, tiny turns and small movements could still drift
because the client deduped movement only by integer `uid/X/Y`, and the server
discarded the `fDirect` value it received in `pGame::MoveTo` (`1004`) when
broadcasting `pGame::Sync` (`1006`).

Files changed:

```text
C:\0-NewDmo\dmo-client-main\DProject\UserServerMove.cpp
C:\0-NewDmo\dmo-client-main\DProject\UserServerMove.h
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameSend.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameSync.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGame.h
C:\0-NewDmo\dmo-client-main\common_vs2019\pSync.h
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\GameServer\TamerWalkPacket.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\GameServer\DigimonWalkPacket.cs
```

Behavior to preserve:

- game/dungeon movement throttle is 150 ms; battle throttle is 120 ms;
- client dedupe allows rotation-only sends when facing changes by at least
  `0.03` radians;
- coordinate sends round to nearest integer instead of truncating;
- server sends `pSync::Walk`/`Move`, then `pSync::Rotation (0x7F)`, then `0`;
- server and client must be updated together because older clients do not know
  the new `pSync::Rotation` sub-block.

2026-05-26 04:11/04:22 BRT precision experiment and rollback:

- User confirmed turn/rotation felt correct, but walking positions were still
  slightly offset between two clients standing together.
- Root cause: movement endpoints were still integer `nSync::Pos` on the wire.
- A test build appended float target `X/Y` after `fDirect` in `1004`, had the
  server echo `pSync::PrecisePosition (0x7E): uid + float X/Y` before
  `pSync::Rotation`, and had the client apply those floats with
  `SetPath(float X, float Y)`.
- The test was rolled back after live logs showed heavy `1004 bytes=34`
  movement spam and the user clarified they manually closed the client only
  because it froze after entering the same channel as another player.
- Both local test clients also had `network_pgsql.log` around 90 MB from
  verbose `SYNC1006` tracing. `cCliGameSync.cpp::SyncTrace` is now disabled
  unless `LKDMO_VERBOSE_SYNC_TRACE` is explicitly defined.
- Current active behavior: no `PrecisePosition` / `0x7E`; keep rounded integer
  position plus `pSync::Rotation (0x7F)`.
- VPS backups:
  `/home/ubuntu/dmo-server-main-postgres/deploy-backups/20260526-041101-precise-position-sync`.
  `/home/ubuntu/dmo-server-main-postgres/deploy-backups/20260526-042216-rollback-precise-position`.
- Validation after rollback: server `DigitalWorldOnline.Game.csproj -c Release`
  and client `Release_English|Win32` builds succeeded; `dmo-pg-game` was
  recreated and logs reached `GameServer started`.

Client-only rotate control added after the sync fix:

- `Shift + WASD` rotates the controlled tamer/digimon toward the keyboard
  direction instead of walking.
- `Shift + left click` rotates toward the clicked terrain/object instead of
  starting click movement.
- The path is canceled, `ANI::MOVE_RUN` is forced back to `ANI::IDLE_NORMAL`,
  and smooth turn-in-place uses existing `SetRotation_AniDir` /
  `_UpdateRotation`; no separate turn animation was found.
- The facing change continues to sync via the `1004`/`1006 pSync::Rotation`
  path above.

Production deploy:

```text
/home/ubuntu/dmo-server-main-postgres/deploy-backups/20260526-025903-sensitive-move-rotation
docker compose --env-file .env.production up -d --build game
dmo-pg-game Up on 17608; logs reached "GameServer started."
```

### Server/client BIN and Pack03 sync

The user asked to verify whether server BINs and client `Pack03` BINs were in
sync, then to synchronize the client pack data with the server version.

Server BIN root:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english
```

Extracted client Pack03 BIN root:

```text
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english
```

Core gameplay bins that already matched by SHA256:

```text
Buff.bin
CashShop.bin
CharCreateTable.bin
Digimon_Book.bin
Digimon_List.bin
DigimonEvo.bin
DMBase.bin
Event.bin
ItemList.bin
MapList.bin
MapMonsterList.bin
MapPortal.bin
MapRegion.bin
Monster.bin
Nature.bin
Quest.bin
Skill.bin
```

Files copied from server BINs into extracted Pack03:

```text
Achieve.bin
DM_UnionCombine-CeilGroup.bin
DM_UnionCombine-ItemGroup.bin
DM_UnionCombine-ItemList.bin
DM_UnionCombine-RankList.bin
DM_Union-Effect.bin
DM_Union-Filter.bin
DM_Union-Filter_Str.bin
DM_Union-Group.bin
DM_Union-Group_Str.bin
DM_Union-HackingInfo.bin
DM_Union-Reward.bin
DM_Union-Reward_Str.bin
MapCharLight.bin
MapNpc.bin
MapObject.bin
skillcodeunion.bin
Weather.bin
WorldMap.bin
```

Backup of replaced Pack03 files:

```text
C:\0-NewDmo\DigiCombineFeature\generated\backups\sync_pack03_from_server_20260525_234021
```

Post-sync validation result: all 19 copied files matched server SHA256,
`mismatches=0`.

If the active client still uses packed data, patch `ClientDist\Data\Pack03.pf`
and `.hf` with the same files after changing the extracted Pack03 tree.

### Level cap 150 context

The user requested raising both server and client level cap to `150`.
Production logs showed level 150 tamers/digimons active during the incident
(`Leleo`, `Noah`, `tatu`). Treat level 150 as part of the current live test
surface when checking stat math, packet widths, EXP/leveling assets, and
overflow.

Do not assume a stat anomaly is caused by the level cap alone. The Noah AT bug
below was a packet layout mismatch.

### Abnormal AT value on Noah

User screenshot showed:

```text
Lv. 150 Noah
AT 761680497
HP 19667
DS 20754
```

Server logs showed normal partner AT values for Noah/Leleo, around `20766`,
`30766`, or item snapshot `37201`, so the database/stat calculation was not
producing `761680497`.

Root cause:

- The current client reads partner AT in `RecvAllStat` as `n4` / 4 bytes.
- Production server `UpdateStatusPacket.cs` was still writing partner AT as
  `short` / 2 bytes.
- The client combined partner AT low bytes with the next stat bytes, producing
  huge values such as `0x2d665271 = 761680497`.

Fixed file:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\GameServer\UpdateStatusPacket.cs
```

Required behavior:

- write partner AT with `WriteInt(character.Partner.AT)`
- keep the log marker:
  `[UpdateStatusPacket:Main] ... partnerATLayout=int32 ... bytesBeforeSerialize=...`

Local validation passed for `DigitalWorldOnline.Game.csproj` with `0` errors
(pre-existing warnings remained). Production backup:

```text
/home/ubuntu/dmo-server-main-postgres/deploy-backups/20260525-234740-update-status-at-int32
```

Production was rebuilt/restarted with:

```bash
docker compose --env-file .env.production up -d --build game
```

### Tatubas account disconnect on login

User reported that account `Tatubas` was taking DC when trying to log in.

Account service accepted the login:

```text
username=Tatubas
accountId=12
```

Then the character service logged:

```text
Process packet error
System.InvalidOperationException: Sequence contains no elements
CharacterModel.get_Partner()
CharacterListPacket..ctor
CharacterPacketProcessor.ProcessPacketAsync
```

Database investigation on production:

```text
Account: Tatubas / accountId=12
Tamer: tatu / characterId=10 / position=0 / level=150 / DigimonCount=0
Tamer: testee / characterId=13 / position=1 / level=150 / DigimonCount=2
```

Existing backup table:

```text
dso_backup.reset_tatu_20260525_234346_digimons
```

Restored 5 digimons for character `10` / tamer `tatu`:

```text
id 44 pretaao BaseType=31044 CurrentType=31044 level=150 slot=0
id 36 gaogao  BaseType=31003 CurrentType=31003 level=120 slot=1
id 38 sakuyaa BaseType=31006 CurrentType=31006 level=120 slot=2
id 43 mile    BaseType=33072 CurrentType=33072 level=150 slot=3
id 37 aaa     BaseType=31029 CurrentType=31029 level=1   slot=4
```

Minimal missing child rows were created for:

```text
Digimon_Digiclone
Digimon_DigicloneHistory
Digimon_AttributeExperience
Digimon_Location
Digimon_BuffList
```

Pre-restore backup tables were created with names like:

```text
dso_backup.restore_tatubas_tatu_YYYYMMDD_HHMMSS_pre_digimons
dso_backup.restore_tatubas_tatu_YYYYMMDD_HHMMSS_pre_digiclone
dso_backup.restore_tatubas_tatu_YYYYMMDD_HHMMSS_pre_attr
dso_backup.restore_tatubas_tatu_YYYYMMDD_HHMMSS_pre_location
dso_backup.restore_tatubas_tatu_YYYYMMDD_HHMMSS_pre_bufflist
```

After restore, `tatu` had `DigimonCount=5`; all five had clone, attribute,
location, and buff-list rows. Game has `EnsureMissingEvolutionRows` and should
recreate missing evolution rows on first login, logging `[EVO-REPAIR]`.

Code guards applied:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\CharacterServer\CharacterListPacket.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\InitialInformationPacketProcessor.cs
```

Required behavior:

- filter character list with `character.Digimons.Any()` before accessing
  `character.Partner`
- cache `var partner = character.Partner`
- do not call `character.Partner` for partnerless tamers
- initial information guard checks `character == null || !character.Digimons.Any()`

Local builds passed for both `DigitalWorldOnline.Character.csproj` and
`DigitalWorldOnline.Game.csproj` with `0` errors. Production backup:

```text
/home/ubuntu/dmo-server-main-postgres/deploy-backups/20260525-235743-partnerless-login-guard
```

Production deploy command:

```bash
docker compose --env-file .env.production up -d --build character game
```

Post-deploy logs showed `CharacterServer started.` and `GameServer started.`;
there was no fresh `Sequence contains no elements` after restart in the checked
log window.

### Production restart context

The user asked to restart the full main VPS instance set. This was done with:

```bash
docker compose --env-file .env.production restart account character game
```

Confirmed markers after restart:

```text
AuthenticationServer started.
CharacterServer started.
GameServer started.
```

## Changelog / work completed

### 2026-05-24 configurable GameServer damage formula

Detailed notes for the new damage formula module were added to
`DamageFormulaSystem.md`. Read that file before continuing combat, attribute,
element, Skill Damage, Critical Damage, or Final Damage work.

Implemented server-side formula isolation:

- New central module:
  - `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Combat/DamageFormula.cs`
- New tests:
  - `dmo-server-main-postgres/src/Tests/DigitalWorldOnline.Game.Tests/Combat/DamageFormulaTests.cs`
- GameServer config:
  - `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/appsettings.json`
  - `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/appsettings.Development.Example.json`

The formula is intentionally configurable because public GAMEKING/DMO
references confirm battle attribute/element interactions and separate options
such as Skill Damage, Critical Damage, and Final Damage, but do not publish a
complete final damage formula. External reference links and the exact formulas
are recorded in `DamageFormulaSystem.md`.

Current config block:

```json
"DamageFormula": {
  "Enable": true,
  "EnableLog": false,
  "CritBaseRate": 1.0,
  "ApplyAttributeToCriticalExtra": false,
  "ApplyAttackToSkill": true,
  "ApplyAttributeToSkillFlat": false,
  "ApplyFinalDamageToSkill": true,
  "ApplyElementDamage": true
}
```

Integrated paths:

- `PartnerAttackPacketProcessor.cs`
  - normal attack vs configured mob;
  - normal attack vs summon mob;
  - PvP partner target;
  - critical route through `DamageFormula.CalculateDamage(...)`.
- `PartnerSkillPacketProcessor.cs`
  - F1/F2/F3 damage vs configured mob;
  - F1/F2/F3 damage vs summon mob.

Current formula behavior:

- Normal damage:
  `floor((Attack + ExtraAttack) * M_attr * M_element * M_fd * M_target)`.
- Critical damage:
  `CritBaseExtra = Attack * CritBaseRate`;
  `CritExtraFromBonus = CritBaseExtra * CriticalDamageExtraPercent / 100`;
  by default, critical extra is added after the attribute/element-multiplied
  base critical portion.
- Skill damage:
  `SkillBaseDamage * (1 + SkillDamagePercent / 100)`, optional AT, attribute
  and element multiplier, flat Skill Damage, optional Final Damage.
- Block is currently represented as `TargetReductionPercent=50` when the new
  formula path is active.
- Existing miss, aggro, cooldown, DS/HP consumption, debuff reduction, damage
  packets, kill handling, EXP/drop flow, and combat state remain outside the
  formula and in the existing battle flow.

Validation:

```powershell
dotnet test src\Tests\DigitalWorldOnline.Game.Tests\DigitalWorldOnline.Game.Tests.csproj -c Debug --no-build
dotnet build src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj -c Debug --no-restore
```

Result:

- Damage formula tests passed with the existing test suite: `14/14`.
- `Game.Host` build passed with `0` errors.
- Existing warnings remain: `net7.0` support warning, package vulnerability
  warnings, and pre-existing nullable warnings.

Known remaining calibration/work:

- Calibrate `CritBaseRate` (`1.0` default, `0.8` candidate).
- Validate whether Critical Damage extra should receive attribute/element
  scaling (`ApplyAttributeToCriticalExtra`).
- Validate whether current `Partner.ATT`, `Partner.SCD`, `Partner.SKD`,
  `Partner.SkillDamagePercent`, and `Partner.FinalDamageBasisPoints` map exactly
  to the desired official-like buckets.
- Confirm element option behavior separately from `Nature.bin` element delta if
  item-provided element damage is later exposed.
- Remaining parallel damage paths still calculate separately and should be
  reviewed in a later pass before declaring all server damage unified:
  `MemorySkillUsePacketProcessor`, `MapServerTamerOperation`,
  `DungeonsServerTamerOperation`, `EventServerTamerOperation`,
  `PvpServerTamerOperation`, and `MapInstanceMobBehavior`.

### 2026-05-24 recent Seal, Digimon, skill, and GM Panel work

Detailed notes for the latest chat-driven pass were consolidated in
`GMPanelAndRecentChanges.md`. Read that file before continuing work around:

- Seal Master card colors, seal inventory tooltip, and `!openseals`.
- Partial Digimon/evolution import from newer global bins into the older LK-DMO
  template.
- New skill names, buff/debuff conversion, tooltip rendering, and OMM skill
  behavior fixes.
- GM Panel scrollbar/menu fixes, admin commands, full clone, and the new
  Summon Monster page with presets, boss ranking, custom stats, client-side
  clamps, and server-side validation.

Latest GM Panel verification:

- Client built successfully with `Release_English|Win32` through
  `DProject.sln`.
- `C:\Release\GSP\DProject_R_ENG.exe` was copied to both local test targets.
- `dmo-pg-game` was rebuilt and restarted through Docker Compose.

### 2026-05-24 item bin migration pass

Detailed item workflow notes were added to `AddItems.md`. Read that file before
continuing item, box, consumable, or `ItemList.bin` work.

Safe changes applied:

- `SkillCodeItem.bin` was merged into legacy `Skill.bin` so item effects exist
  in the same table format used by the client/server template.
- Resulting `Skill.bin`: 10161 `CsSkill` rows, with all
  `SkillCodeDigimon.bin`, `SkillCodeTamer.bin`, and `SkillCodeItem.bin` IDs
  present.
- `ItemList.bin` is generated append-only from modern split
  `ItemData.bin + Item_Str.bin`: existing legacy `ItemId`s are preserved
  byte-a-byte, only missing modern `ItemId`s are converted into the old
  fixed `CsItem::sINFO` layout, and legacy-only IDs remain appended.
- Converted `ItemList.bin`: 28507 item rows. Counts: 14907 legacy input,
  27320 modern input, 13720 preserved legacy rows, 13600 converted new rows,
  and 1187 legacy-only rows.
- `Item_Str.bin` detail: the first UTF-16 string is the real item name and
  the second is a category/type label. Using the second field caused new items
  to appear as generic names like `Container item`, `Ring`, or `Earring`.
- `Zero Unit` items are confirmed in the generated add report and pack, for
  example `45054 Zero Unit Earring`, `45057 Zero Unit Bracelet`,
  `1310630 Zero Unit Necklace`, and `1310632 Zero Unit Ring`.
- The client-side accessory option pool was updated from `MAX_OPT_COUNT=16`
  to `MAX_OPT_COUNT=44` in
  `C:\0-NewDmo\dmo-client-main\LibProj\CsFileTable\Item.h`, matching the
  largest current `AccOption.bin` pool. This is separate from the runtime
  item add slots, which already support 8.
- The generated `ItemList.bin` now replaces the old accessory tail with
  modern `AccOption.bin`/`AccEnchant.bin` converted into the legacy monolithic
  layout: `AccessoryOption=284`, `AccessoryEnchant=36`, pool slots `44`.
- GM Panel item/monster search now stores a normalized alphanumeric search key,
  so names with spaces or punctuation can be found with compact input such as
  `zerounity`.
- Updated files were copied to:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Skill.bin
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english\Skill.bin
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english\ItemList.bin
```

- `ClientDist\Data\Pack03.pf/.hf` was patched with:

```text
data\bin\english\ItemList.bin
data\bin\english\Skill.bin
```

- `dmo-pg-game` was rebuilt/restarted and logged:

```text
Loaded Skill.bin: 10161 CsSkill + 116 CsTamerSkill + 93 CsAreaCheck rows
Loaded ItemList.bin static sections: Items=27320, AccessoryOption=284, AccessoryEnchant=36
```

Important limitation:

- `RandomBox*.bin` is not the same direct contract as the current
  `Asset_Container` database tables. Current audit found no direct match from
  `RandomBoxList.BoxId` or `ConsumeItems.ItemId` to `Asset_Container.ItemId`.
  Do not import it blindly; use `AddItems.md` and the generated audit report
  before changing box rewards.

### 2026-05-25 deck bin migration pass

The GDMO deck/encyclopedia data migration is tracked in:

```text
C:\0-NewDmo\DeckBinMigrationFeature
```

New converter:

```text
C:\0-NewDmo\DeckBinMigrationFeature\tools\Convert-ModernDBookToLegacyDigimonBook.ps1
```

What it does:

- Reads modern DBook XML from `C:\0-NewDmo\Dcommerce\xml\AllBins\Data`.
- Reads language XML from
  `C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str`.
- Converts the modern split DBook data into the legacy client template
  `Digimon_Book.bin`, used by
  `C:\0-NewDmo\dmo-client-main\LibProj\CsFileTable\DigimonMng.cpp`.
- Preserves the current legacy structural limit `ENCY_MAX_OPTION = 3`; any
  fourth modern deck effect is reported instead of silently overflowing the old
  binary layout.
- Writes generated output to:

```text
C:\0-NewDmo\DeckBinMigrationFeature\generated\converted\Digimon_Book.bin
```

Latest conversion result:

```text
OutputBytes=189344
OptionCount=11
ExceptionCount=14
DeckCount=57
CompositionGroupCount=58
CompositionEntryCount=395
TruncatedDeckOptions=9
UnsupportedLegacyDeckOptions=23
MissingDigimonNames=0
StringTruncations=1
ValidationExactRead=True
```

Generated reports:

```text
C:\0-NewDmo\DeckBinMigrationFeature\generated\reports\digimon-book-summary.csv
C:\0-NewDmo\DeckBinMigrationFeature\generated\reports\deck-conversion.csv
C:\0-NewDmo\DeckBinMigrationFeature\generated\reports\deck-truncated-options.csv
C:\0-NewDmo\DeckBinMigrationFeature\generated\reports\deck-unsupported-options.csv
C:\0-NewDmo\DeckBinMigrationFeature\generated\reports\missing-digimon-names.csv
C:\0-NewDmo\DeckBinMigrationFeature\generated\reports\string-truncations.csv
```

Applied files:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Digimon_Book.bin
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english\Digimon_Book.bin
```

The server split DBook bins already matched the `Dcommerce` source by hash for
`DBookDeckData.bin` and `DBookCompositionData.bin`.

Pack03 active patch status:

- `ClientDist\Data\Pack03.pf/.hf` was patched on 2026-05-25 after the client
  process released the pack.
- Patch command:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File "C:\0-NewDmo\SealMasterFeature\tools\Patch-Pack03Files.ps1" `
  -RelativeFiles @("data\bin\english\Digimon_Book.bin")
```

- Patched entry:

```text
data\bin\english\Digimon_Book.bin
Hash=0x6DB4D9AC
Size=189344
```

Functional limitation:

- Modern deck option IDs 7 to 11 are present in the converted data:
  Hit Rate, Attack Amplification, Attribute Damage Increase, Attack Increase,
  and Final Damage Increase.
- The old client effect enum/application code only has built-in handling for
  deck options 1 to 6. Treat `deck-unsupported-options.csv` as the implementation
  checklist before expecting those new effects to apply in combat/stat flows.

Runtime follow-up on 2026-05-25:

- Full runtime notes were added to `EncyclopediaDeckSystem.md`.
- The deck converter now writes the required 2-byte C++ alignment pad between
  `USHORT s_nVal[3]` and `UINT s_nProb[3]`. Missing this pad was the cause of
  tooltip values such as `1310720.0% chance` and `458752 seconds`.
- The C++ client loader in `LibProj/CsFileTable/DigimonMng.cpp` normalizes both
  padded and older no-pad converted deck records so existing packs do not show
  shifted `Probability/Time` values.
- Deck tooltips in `ToolTip.cpp` were widened and now join the attack trigger
  and chance text with explicit spacing, avoiding label/value collisions.
- Server `DigimonBookBinLoader` now loads `Digimon_Book.bin` at Game.Host boot
  and registers deck effects through `UtilitiesFunctions`.
- `Character_Tamer.EncyclopediaDeckId` remains the persisted active deck id.
  `EncyclopediaDeckUsePacketProcessor` now persists the id, sends
  `UpdateStatusPacket`, and logs all loaded effects for the selected deck.
- Current server-side functional deck passives:
  - option `6` attack speed: subtracts `floor(baseAS * percent / 100)` from the
    partner's real AS;
  - option `5` max HP: adds `floor(baseHP * percent / 100)` to the partner's
    real HP.
- Verified reference deck: group `1030`
  `White Wings : Gurei Tou of Courage` loads as
  `Condition=1,3,3`, `AttackType=0,1,1`, `Option=6,3,1`,
  `Value=22,125,30`, `Probability=0,1000,2000`, `Time=0,10,7`.
- Validation on 2026-05-25:
  - client `Release_English|Win32` build passed;
  - server `DigitalWorldOnline.Game.csproj` release build passed;
  - Docker `game` image rebuilt and `dmo-pg-game` recreated;
  - boot log confirmed `Loaded Digimon_Book.bin: 11 options, 57 decks, 58 composition groups`.

Follow-up container-bin change:

- Added server `ContainerBinLoader` for `RandomBoxList.bin`,
  `RandomBoxItem.bin`, and `RandomBoxReward.bin`.
- `AssetsLoader` now merges database containers with bin containers; bin data
  wins on duplicate `ItemId`, database remains fallback.
- `ItemConsumePacketProcessor` routes any item with `_assets.Container` config
  through `ContainerItem(...)`, even if `TypeL` is not `170`.
- Boot verification:

```text
Loaded RandomBox container bins: Boxes=335, ItemGroups=173, RewardGroups=101,
Containers=335, MissingItemGroups=0, MissingRewardGroups=0
```

- Usable low-ID RandomBox test candidates are recorded in:

```text
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\random-box-usable-test-ids.csv
```

### 2026-05-23 working tree diff consolidated before main commit

Diff base for this commit:

- Current branch: `main`.
- Remote/base commit: `origin/main` / `HEAD` at `0c23f2e`
  (`Integra archive, transcendencia e XML Union`).
- The branch was already aligned with `origin/main`; the comparison is the
  current working tree against that commit, not a separate feature branch.
- Tracked-file diff before staging: 88 modified tracked files,
  4555 insertions, 880 deletions.
- New untracked content before staging was intentionally included because the
  user asked to commit everything from the current work pass.

New isolated reverse-map subprojects added at the repository root:

- `DetailInfoFeature/` - 96 files. Official `Detalhes` / tamer and Digimon
  detail window map, copied decompiled functions, Pack01 asset manifests,
  language bins, status/enchant/Seal Master support tables, and sweep notes.
- `DigiCombineFeature/` - 152 files. Official DigiCombine and UnionCombine
  fusion maps, copied XML/BIN tables, decompiled UI/table functions, packet
  skeleton headers, service/table source files, asset manifests, and original
  window notes.
- `DigiSummonFeature/` - 67 files. Official data invocation/gacha selection
  map for `DigiSummonUI`, copied summon XML/BIN files, decompiled UI/table
  functions, packet map, and dynamic card image manifest.
- `DigitalDrawFeature/` - 95 files. Digital Draw / Lucky Draw map, reused
  `DigiSummonUI`/`Random_box` visual family, LuckyDraw table references,
  internal probability popup functions, and notes that
  `DM_Luckydraw_Info-LuckyDrawInfo.xml/bin` was not found in current
  extraction.
- `DungeonEnterFeature/` - 54 files. Dungeon entry window map, official
  dungeon XML/BIN family, copied decompiled UI/table functions, image path
  inventory, and the `Treinamento Gankoomon` candidate notes.
- `DigimonAdvanceFeature/` - 175 files. Digimon advance/enchant room map,
  copied decompiled functions, scene/sound/Pack01 references, table/XML/BIN
  dependencies, and candidate/xref CSVs for the `EnchantTactics` family.
- `ExtraInventoryFeature/` - 103 files. Official exclusive inventory reverse
  map with copied functions for `ExtraInventoryUI`, category UIs, Seal Master
  shared functions, table/language bins, XML copies, and texture-pack
  pitfalls recorded from the UI work.
- `ItemUpgradeManagementFeature/` - 120 files. Official `Gerenciamento`
  window map for strengthening, transfer, and recovery tabs, with copied
  item-upgrade tables, UI functions, packet/result strings, assets, sounds,
  and last-sweep documentation.
- `UnionCombineFusionSelectFeature/` - 60 files. Newer Union Combine fusion
  selection screen map, official `DM_UnionCombine-*` table copies, item lookup
  bins/XML, decompiled UI/table functions, asset verification, and rank-list
  comparison.
- Root CSVs `DigimonAdvance_candidate_xrefs.csv`,
  `DigimonAdvance_candidates.csv`, and `DigimonAdvance_seed_call_edges.csv`
  record the wider candidate trace used before the dedicated
  `DigimonAdvanceFeature` folder was organized.

Extra Inventory client implementation added:

- New active files:
  - `dmo-client-main/DProject/_Interface/Game/ExtraInventory.cpp`
  - `dmo-client-main/DProject/_Interface/Game/ExtraInventory.h`
  - `dmo-client-main/DProject/ExtraInventoryDebugLog.h`
- Registered the window in the Visual Studio project and filters.
- Added `WT_EXTRAINVENTORY` to the base window type registry and wired
  `cBaseWindow::NewWindow(...)` to construct `cExtraInventory`.
- Added `_GameIF` open/close/drop-routing diagnostics for
  `WT_EXTRAINVENTORY`, including cursor drop traces for `CI_INVEN` and
  `CI_EXTRAINVEN`.
- Added `Ctrl+I` menu handling in `GlobalInput.cpp` / `HotKey.cpp` so the
  window opens and closes as a normal game UI window.
- Implemented draggable header behavior so the exclusive inventory window can
  be repositioned like the normal inventory.
- Added UI layout matching the latest accepted test pass:
  - title `Inventario Exclusivo`;
  - left category tabs: `Inventario de Selo`, `Bolsa de Bilhete`,
    `Itens de Evolucao`, `DigiOvos`, `Materiais`;
  - official root/background/tab/close/scroll/button paths from Pack01;
  - permanent scrollbar using the Seal Master scrollbar visual family;
  - five-column item grid with category-specific item display;
  - normal inventory-style hover highlight on empty and filled slots;
  - Seal Master card slot background cropped from
    `SealMaster\Sealmaster_bg_00.tga` to avoid the earlier repeated/small
    texture problem;
  - Seal Master combo/dropdown assets via `SealMaster\btn_combobox.tga` and
    `SealMaster\dropdown_list_0`;
  - search icons from
    `Encyclopedia\NewEncyclopedia\main\sarch.png` and
    `Encyclopedia\NewEncyclopedia\main\re.png`;
  - search text rendered with an overlay `cText` because the local `cEditBox`
    captured input but did not visibly draw text in this custom window.
- Category filters implemented in the client:
  - Seal inventory: overview/stat dropdown only.
  - Ticket bag: search input only.
  - Evolution items: rank dropdown plus search input.
  - DigiOvos: search input only.
  - Materials: search input only.
- Client item interactions implemented:
  - drag normal inventory item into the correct exclusive inventory category;
  - drag exclusive inventory item out to normal inventory;
  - drag item within the same exclusive category;
  - right-click Seal item can call the normal item-use flow so seals can be
    opened/used from the exclusive inventory;
  - Shift + right-click routes to split/move behavior matching normal
    inventory intent;
  - collect-all button calls the new extra inventory collect-all packet.
- Client debug logging was added around keyboard open, window creation,
  category switches, collect-all clicks, drag/drop, right-click, filtering,
  hover, and packet send/receive paths. These logs were useful during testing
  but should be reduced later if they become noisy.

Extra Inventory client data/protocol changes:

- `Inventory_Protocol.h` gained inventory flags for:
  - `eEXTRA_SEAL`
  - `eEXTRA_TICKET`
  - `eEXTRA_EVOLUTION`
  - `eEXTRA_DIGITAMA`
  - `eEXTRA_MATERIAL`
- `pItem.h`, `CCursor.cpp/.h`, `DataMng.cpp/.h`, `cCliGame.h`,
  `cCliGameItem.cpp`, `cCliGameReceive.cpp`, `cCliGameSend.cpp`, and
  `cCliGameSync.cpp` were extended so the client can store, receive, send,
  drag, and refresh the five exclusive inventory lists.
- `DataMng` now owns/initializes category inventories for the exclusive
  inventory and dispatches load/update packets into the correct list.
- The item movement send path maps exclusive inventory slots into server slot
  ranges so existing move/split/use packet flows can be reused.

Extra Inventory server implementation added:

- New packet processor:
  - `ExtraInventoryCollectAllPacketProcessor.cs`
- New/extended enums and slot contracts:
  - `InventoryTypeEnum` adds `ExtraSeal`, `ExtraTicket`, `ExtraEvolution`,
    `ExtraDigitama`, `ExtraMaterial`.
  - `ItemListEnum`, `ItemListMovimentationEnum`, and
    `GeneralSizeEnum` add exclusive inventory list/movement/range constants.
  - `GameServerPacketEnum` adds the collect-all route.
- `CharacterModel` / behavior now carry the five extra item lists:
  - `ExtraInventorySeal`
  - `ExtraInventoryTicket`
  - `ExtraInventoryEvolution`
  - `ExtraInventoryDigitama`
  - `ExtraInventoryMaterial`
- `DatabaseContext`, character command/query repositories, and create-character
  flow were updated so the five extra lists are created, loaded, tracked, and
  persisted with the character.
- `InitialInformationPacketProcessor` now sends the five exclusive inventories
  to the client during initial game load.
- `ItemMovePacketProcessor` now handles:
  - inventory -> extra category;
  - extra category -> inventory;
  - extra category -> same extra category;
  - refresh packets for both normal inventory and the affected extra list.
- `ExtraInventoryCollectAllPacketProcessor` scans the normal inventory,
  filters by category type sets, moves eligible stacks into the target extra
  list, persists both lists, and sends both refreshed inventory packets.
- Extra-category validation uses type sets for Seal, Ticket, Evolution,
  Digitama, and Material categories so the server rejects wrong-category drops.
- Extra inventory logs include collect-all request/finish/reject details,
  manual move details, empty/no-match diagnostics, and inventory samples when
  no stacks are moved.

Other client/server changes included in the same diff:

- Digimon Archive and Transcendence stabilization continued:
  - shared slot resolver added on the server;
  - archive move packet/processor added;
  - archive load packet and archive UI parsing adjusted;
  - transcendence receive-exp and success packets/processors adjusted;
  - client archive/transcendence logs added around slot parsing, material
    charging, EXP packet send/receive, final transcendence, and UI refresh.
- Hatching, partner switch, partner evolution, Growth Fruit, item consume,
  item scan, item return, item split, inventory sort, quest give-up, and spirit
  craft processors received fixes/logging so item and Digimon slot side effects
  are easier to trace.
- `PartnerSwitchFailurePacket.cs`, `QuestCanceledPacket.cs`, and
  `DigimonArchiveMovePacket.cs` were added for explicit failure/cancel/move
  server responses.
- `ClientDigimonSlotResolver.cs` was added to centralize client-visible
  digimon slot resolution for archive/transcendence style packets.
- Authentication/account/character host processors and startup files received
  logging/DI adjustments used during the login and packet-debug pass.
- `CrashLogger.cpp/.h` and `nlib/client.cpp` gained extra diagnostic support.
- `QuestBinLoader.cs` and `AssetsLoader.cs` were updated so the server-side
  asset/bin loader can cover quest/table data needed by later features.
- Two infrastructure migrations were added:
  - `20260523173500_ApplyInitialTemporalDefaults.cs`
  - `20260523181000_AlignRequiredTemporalEndDefaults.cs`
- A new server test project was added under
  `dmo-server-main-postgres/src/Tests/DigitalWorldOnline.Game.Tests`, covering
  archive slot contracts and transcendence receive-exp packets.

Documentation changes included in the diff:

- `CONTEXT.MD` was expanded with the active build target, isolated reverse-map
  notes, Digimon Archive/Transcendence debug state, Extra Inventory mapping,
  Item Upgrade mapping, and this consolidated diff summary.
- Each isolated subproject has its own README/docs with function maps, asset
  inventories, table/language inventories, last-sweep notes, and texture-pack
  pitfalls. These notes explicitly call out issues hit during implementation,
  including Pack01 paths, missing extracted assets, pink placeholder texture
  failures, repeated/cropped texture mistakes, and category-specific dropdown /
  search differences.
- `quickmenu_png_functions/README.md` and `imagegenerator.md` were updated or
  added as auxiliary notes from the broader reverse-map work.
- `.gitattributes` was added with Git LFS tracking for the large copied table
  XML files `**/tables/xml/QuestItemData.xml` and
  `**/tables/xml/ItemData.xml`. These repeated official XML references are
  larger than GitHub's normal 100 MB object limit, so they must remain LFS
  objects if they stay in the reverse-map subprojects.

Latest local verification before this commit:

- The client was built with `DProject.sln` using `Release_English|Win32`.
- The last accepted client binary was copied to:
  - `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe`
  - `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`
- User confirmed the final Extra Inventory UI test state was good enough to
  leave as-is for now.

Known follow-up risks:

- Extra Inventory still has temporary debug logs in both client and server.
  Keep them while validating item movement, then trim once the flow is stable.
- Extra Inventory category type sets are server-side approximations based on
  the current mapped data and observed item types; verify against the official
  table rules before treating them as final production rules.
- The isolated feature folders intentionally include copied bins/XML and
  decompiled `.c` references. They are research/porting artifacts and are not
  compiled into the active client unless explicitly wired later.
- The current commit contains both active code and research subprojects. Future
  feature work should keep active integration changes separate when possible.

### 2026-05-23 isolated official UI reverse maps

- `DigiCombineFeature/` remains the isolated reference for the official union
  combine/fusion window. Its asset manifest is based on decompiled UI xrefs,
  with XML/BIN copies kept beside the copied `.c` functions.
- `UnionCombineFusionSelectFeature/` was split out for the newer-looking
  fusion selection screenshot that has rank filters `N` through `SS+`. This is
  the `UnionCombineUI` / `DM_UnionCombine-*` family, not the older
  `DigiCombineUI` map. It copies 28 UnionCombine UI/table/adjacent BGM functions, the
  official UnionCombine XML/BIN tables plus item lookup tables, English item
  language bins, and a Pack01 asset/sound manifest generated from decompiled
  function xrefs.
- `DigitalDrawFeature/` was added for the Digital Draw/Lucky Draw ticket
  selection screen. The fixed UI is the reused `DigiSummonUI` /
  `Random_box` visual family, but the system has its own EXE table reference:
  `DM_Luckydraw_Info-LuckyDrawInfo.bin` / `CLuckyDrawTable`. The subproject
  copies 52 reused UI functions and 6 LuckyDraw table functions, including the
  internal `info. de prob.` popup/list row chain, generates
  xref-based Pack01 manifests, and records that the LuckyDrawInfo XML/BIN is
  referenced by the EXE but missing from the current `Dcommerce` extraction.
- `DigiSummonFeature/` is the isolated reference for the union summon/gacha
  windows. It contains the copied summon XML/BIN tables, decompiled UI/table
  functions, fixed UI asset manifest, and dynamic card image manifest.
- `DungeonEnterFeature/` was added as the isolated reference for the official
  dungeon entry window. It copies the dungeon XML/BIN family from `Dcommerce`
  (`DungeonList`, `DungeonQualification`, `DungeonObtainable`, `DungeonValue`,
  `DungeonReward`, `DungeonStepInfo`, `DungeonClearInfo`, `Dungeon_Old`, and
  `MapDungeonRegion`), English language bins, and the decompiled UI/table
  functions traced from `DungeonEnter`.
- `DigimonAdvanceFeature/` was added as the isolated reference for the official
  Digimon advance/enchant screen. The traced runtime family is
  `EnchantTactics` plus the popups/widgets `DigiPowerEnchant`,
  `EnchantOption`, `EnchantOptionResultUI`, `DigimonTranscendenceHelp`, and
  page name `DigimonEnchantPage`. It copies 106 decompiled functions, the
  official `EvoDigitama`, `EnchantItemInfo`, `EnchantStatInfo`,
  `TranscendNeed`, `TranscendMaterial`, `TranscendCorrect`, and
  `TranscendCorrectEx` XML/BIN tables, item lookup tables, English language
  bins, and xref-based Pack01/scene/sound manifests.
- Dungeon entry fixed UI assets were traced from decompiled xrefs, not guessed:
  core references include `DungeonUI\Dungeon_Bg.tga`,
  `DungeonUI\Dungeon_Entrance_Btn.tga`, list/reward/requirement frames, normal
  difficulty badge, reward button, close button, and `Icon\Mask_Over.dds`.
  Dynamic preview images come from `DungeonList.xml/ImagePath`; the manifest
  currently records 90 dungeon records and 56 unique image paths.
- The screenshot candidate for "Treinamento Gankoomon" is documented as
  `DungeonId=5001`: `DungeonUI\DungeonImage\Dungeon_50.png`, Digimon level
  requirement value `150`, and 9 obtainable reward item ids. Interface textures
  are in Pack 01 (`ClientDist\Data\Pack01.*`); only their official paths are
  recorded so far, because Pack 01 was not extracted into the subproject.
- Digimon advance/enchant fixed refs were traced from decompiled xrefs. Core
  refs include `Enchant_Room.mp3`, `EnchantTactics\card_base.tga`,
  `EnchantTactics\card_base2.tga`, `EnchantTactics\NO.tga`,
  `EnchantTactics\CardAni\Card_R_`, `EnchantTactics\SelectAni\Card_C_`,
  `EnchantTactics\CardSuccess\Succes_R_`,
  `EnchantTactics\CardFail\Fail_R_`, `EnchantTactics\SuccessAni\Succes_`,
  `EnchantTactics\FailAni\Fail_`, `DetailInfo\Enchant_AT.tga`,
  `DetailInfo\Enchant_bl.tga`, `DetailInfo\Enchant_ct.tga`,
  `DetailInfo\Enchant_ev.tga`, `DetailInfo\Enchant_HP.tga`,
  `DigimonStatus_New\Digimon_Status_Enchant_01.tga`,
  `TacticsHouse\CheckOnline.tga`, `Breakthrough\Breakthrough_Main_BG.tga`,
  `Breakthrough\Breakthrough_Digimon_info_BG.tga`,
  `system\LevelEffect\Enchant_K45.nif`,
  `system\LevelEffect\Enchant_K60.nif`, `System\transcendental.wav`,
  `System\Tactics_enchant_fail.nif`, and `game\Enchant_Fail.mp3`.
  Shared cluster refs such as `Accessory\Enchant\...` were kept separately in
  `DigimonAdvanceFeature\assets\manifest\AdjacentCopiedClusterAssetReferences.csv`.

### 2026-05-23 current Digimon Archive / Transcendence debug state

This system is not finished yet. The current code is a stabilization pass for
the Digimon Archive, Transcendence, mercenary slot alignment, hatching, partner
switching, and Growth Fruit side effects. Treat this section as the active
debug map before touching those areas again.

Latest verified build/publish state for this pass:

- Server project `DigitalWorldOnline.Game.csproj` builds with `0` errors.
- Docker `game` image was rebuilt and `dmo-pg-game` restarted successfully.
  Boot log showed `GameServer started`.
- Client was built with `Release_English|Win32`; output
  `C:\Release\GSP\DProject_R_ENG.exe` was copied to:
  - `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe`
  - `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`

Observed root cause before the latest Hyper Charge fix:

- Client logs around `2026-05-23 10:45` showed `packet=16040`
  (`DigimonTranscendence`) being sent, but no matching `packet=16039`
  (`TranscendenceReceiveExpResult` / charge EXP request) for that click.
- Server logs received `16040` and rejected final transcendence because the
  target still had `0/140000` Transcendence EXP:
  `[Transcendence] Finish rejected: digimon {DigimonId} exp {Exp}/{MaxExp}.`
- The C++ UI previously waited for `_EffectEnd()` before sending the charge
  packet. If the charge animation callback did not fire, the UI looked like
  nothing happened and the server never received `16039`.
- Current client change: `PressNormalAddExpBtn()` and
  `PressAdvancedAddExpBtn()` now call `SendExpCharge(...)` immediately after
  validation/lock. `_EffectEnd()` no longer sends the packet; it only keeps the
  effect looping when needed.

Current slot contract:

- Server real active digimon slots are `DigimonModel.Slot = 0..DigimonSlots-1`.
- Archived digimon use `Slot = 255` (`byte.MaxValue`).
- `CharacterModel.RemoveDigimon(slot)` compacts active slots by default.
  `CompactActiveDigimonSlots()` rewrites active slots contiguously from `0`.
  `GetNextActiveDigimonSlot()` returns the current active count, so a restored
  archived digimon goes to the last available active slot after compaction.
- Archive packet `3201` now uses the same client slot contract as
  Transcendence: `0 = current active partner`; `1..8 = visible
  mercenary/real active slot`. It no longer subtracts one from mercenary
  slots, because the client companion grid already sends `1` for the first
  mercenary (`GetTactics(0)`), which is server slot `1`.
- Archive packet `3201` no longer rejects the active partner unconditionally.
  The active slot can be stored or swapped as long as the tamer is not moving
  the only active Digimon into an empty archive slot. When slot `0` is archived,
  server compaction promotes the next active mercenary to slot `0`.
- Archive `3201`, Transcendence charge `16039`, and Transcendence finish
  `16040` share `ClientDigimonSlotResolver` on the server. There is also a
  compact visual-index fallback for older/misaligned client payloads after
  gaps.

Current server logs added for Archive and Transcendence:

- `ARCHIVE 3201 request ...`
  logs tamer, VIP flag, client digivice slot, resolved server slot, archive
  slot, NPC, packet length, digivice partner id/name/slot, archive row
  id/digimon/name, and current tamer slots.
- `ARCHIVE 3201 rejected last active partner move ...`
  confirms the server blocked only the unsafe case where a tamer would be left
  without any active Digimon.
- `ARCHIVE 3201 moved to archive ...`
  logs tamer, digimon id/name, digivice slot, archive slot, and price.
- `ARCHIVE 3201 moved to digivice ...`
  logs tamer, digimon id/name, archive slot, and new active slot.
- `ARCHIVE 3201 swapped ...`
  logs outgoing/incoming digimon ids/names, active slot, archive slot, and
  price.
- `ARCHIVE 3201 move to digivice failed because archive digimon could not be loaded ...`
  catches stale archive rows or missing hydrated Digimon data.
- `ARCHIVE 3201 move to digivice failed because tamer has no available active slot ...`
  catches full mercenary slot state.
- `[Transcendence] Charge request ...`
  logs tamer, target slot, charge type, NPC, active item slot, archive material
  slots, and charge items.
- `[Transcendence] Charge target slot resolved ...`
  logs when the client slot differs from the resolved server slot.
- `[Transcendence] Material planned ...`
  logs target id, material id, archive slot, base EXP, gained EXP, planned
  total EXP, and bonus state before consuming anything.
- `[Transcendence] Material consumed ...`
  logs the material that was actually removed from Archive and the EXP applied.
- `[Transcendence] Archive compact ...`
  logs archive-slot compaction after materials are consumed.
- `[Transcendence] Charge finished ...`
  logs total gained EXP, final EXP, max EXP, and deleted material slots.
- `[Transcendence] Charge rejected ...`
  logs server result code and rejection reason.
- `[Transcendence] Missing base info ...`
  logs missing Digimon base info when material/target validation cannot proceed.
- `[Transcendence] Charge packet failed ...`
  catches unexpected exceptions in `16039`.
- `[Transcendence] Finish failed ...`
  logs missing target on `16040`.
- `[Transcendence] Finish target slot resolved ...`
  logs final transcendence target slot remapping.
- `[Transcendence] Finish rejected ...`
  logs already-transcended, insufficient EXP, and insufficient money states.
- `[Transcendence] Finish cost mismatch ...`
  warns if client and server disagree on the final 5T cost.
- `[Transcendence] Finished ...`
  logs successful final transcendence, cost, and remaining bits.
- `[Transcendence] Finish packet failed ...`
  catches unexpected exceptions in `16040`.

Current client logs added for Archive and Transcendence:

- `ARCHIVE RECV 3204 ...`
  logs packet parsing for archive load: begin/end, storage header, client max,
  slot parse begin/end, `typeAll`, name length, raw scale, EXP,
  Transcendence EXP, level, hatch, base, max evolution count, clone, UID,
  memory skill count, and invalid memory/evolution count aborts.
- `ARCHIVE CONTENT OnRecvEnd ...`
  logs archive end refresh state, whether it is a refresh-only update, current
  max/opened/count/sorted values, and selected archive pointer.
- `ARCHIVE CONTENT OnRecvArchiveInfo ...`
  logs received archive row data, full refresh clearing, replacing existing
  slots, stored slot base/level/scale/incubator flag, and final UI counts.
- `ARCHIVE CONTENT UpdateSlotData ...`
  logs filtering/sorting start and end, source slots, invalid filtered slots,
  and missing tactics explain data.
- `ARCHIVE CONTENT OnRecvChangeResult ...`
  logs source slot, destination slot, price, selected pointer/index, archive
  counts, and sorted counts after `3201`.
- `TRANSCEND material click ...`
  logs target data, material data, archive slot, hatch level, scale,
  incubator/transcend flags, and registered material EXP.
- `TRANSCEND material accepted as exp digimon ...`
  logs Raremon/fixed-EXP material acceptance.
- `TRANSCEND material rejected evoType ...` and
  `TRANSCEND material rejected hatch ...`
  log why a material was refused client-side.
- `TRANSCEND SendExpCharge begin ...`
  logs charge type, selected index, selected pointer/base/level/hatch,
  current Transcendence EXP, material count, and over-EXP flag.
- `TRANSCEND SendExpCharge material ...`
  logs each material slot, raw slot, pointer, base, level, hatch,
  incubator/transcend flags, and EXP.
- `TRANSCEND SendExpCharge item ...`
  logs each charge item inventory slot, item type, and count.
- `TRANSCEND SendExpCharge sent ...`
  logs VIP/portable flags, NPC, selected index, material count, and item count.
- `TRANSCEND RecvExpCharge ...`
  logs server result, target position, charge flag, deleted materials, item
  updates, success rate, charge EXP, and total EXP.
- `TRANSCEND SendDigimonTranscendence begin ...`
  logs final transcend click selected index and target data.
- `TRANSCEND SendDigimonTranscendence expCheck ...`
  logs client-side current EXP, max EXP, and registered material EXP.
- `TRANSCEND SendDigimonTranscendence sent ...`
  logs final request VIP/portable flags, NPC, selected index, and money.
- `TRANSCEND RecvDigimonHatchUp ...`
  logs final server result, target position, hatch level, money, and EXP.

Current logs added for hatching, partner switching, Growth Fruit, and related
slot corruption checks:

- Server hatch logs:
  - `Hatch insert request ...`
  - `Hatch insert accepted ...`
  - `Hatch increase request ...`
  - `Hatch increase result: ... Success/Backuped/Broken/Failed ...`
  - `Hatch remove request ...`
  - `Hatch remove finished ...`
  - `Hatch finish request ...`
  - `Hatch finish slot scan ...`
  - `Hatch finish send ...`
- Server partner switch logs:
  - `Partner switch rejected ...`
  - `Partner switch target slot resolved ...`
  - `Partner switch request ...`
  - `Resetting switched-out partner ...`
  - `Partner switch send ...`
  - `Unhandled PartnerSwitch packet ...`
- Server evolution logs:
  - `Evolution request ignored ...`
  - `Evolution request rejected ...`
  - `Evolution request ...`
  - `Unhandled PartnerEvolution packet ...`
- Server Growth Fruit logs:
  - `Growth fruit request ...`
  - `Growth fruit consume failed ...`
  - `Growth fruit persisted ...`
- Server item/consumable logs that help separate Growth Fruit bugs from generic
  item-use bugs:
  - `Unsupported item use request ...`
  - `Consumable item target was unavailable ...`
  - `Consumable item use rejected: missing skill apply ...`
  - `Consumable item use rejected: unsupported applies ...`
  - `Consumable item applied ...`
  - `Tamer ... tried to expand digimon archive past MaxTacticsHouse ...`
  - `D-skill expansion item ...`
  - `D-skill expansion: ... stat-mutation not implemented ...`
- Account/login logs:
  - `Login request parsed ...`
  - `Login rejected ...`
  - `Login accepted ...`
  - malformed login packet dumps via `DescribeLoginRequestPacket(...)`.
- Client partner/size logs:
  - `PARTNER_SWITCH recv ...`
  - `PARTNER_SCALE ignored invalid packet ...`
  - `PARTNER_SCALE synced ...`
  - `DIGIMON_SIZE repaired invalid scale ...`
- Client load/init logs:
  - `RECV InitGameData ...`
  - especially partner/tactics sections, memory skill counts, invalid tactics
    slot guards, inventory, quest, buff, and load-complete markers.

Known current risks / TODOs:

- Re-test Hyper Charge after the immediate-send fix. The expected sequence is:
  client `network_pgsql.log` shows `send packet=16039`, server logs
  `[Transcendence] Charge request`, then client logs `TRANSCEND RecvExpCharge`.
- `16039` success payload now includes the two-byte `sItemInfo` padding between
  inventory slot and `cItemData`; without it, the client parsed `successRate`
  from the first bytes of `chargeExp` and showed huge invalid EXP values.
- Re-test final Transcendence after a successful charge. The expected sequence
  is `16039` first, then `16040`, with server no longer rejecting
  `{Exp}=0/140000`.
- Re-check whether final Transcendence updates the visible hatch level to 6.
  Previous tests showed materials disappearing and the target becoming
  transcended visually, but tooltip still showed Hatch Level 5.
- Verify that the active Digimon can be archived when another active Digimon is
  available to become slot `0`, but cannot be archived as the only active
  Digimon into an empty archive slot.
- Verify that removing a Digimon from active slots compacts the next slots
  forward both server-side and client-side. The current server model compacts;
  client refresh behavior still needs repeated UI testing.
- The server charge EXP calculation currently uses Raremon fixed EXP fallbacks
  plus same-type/different-type hatch-grade tables. Validate against the
  official bin/table behavior before considering it production-ready.
- The Archive refresh path currently sends/manage-load grouped packets after
  `3201`. If the UI resets too much, prefer a C++ refresh-only update for slots
  rather than reinitializing the whole archive window.
- Keep an eye on message ids. Earlier client asserts for missing message ids
  `1` and `73` were symptoms of result-code/message-table mismatch, not just UI
  text issues.

Useful filters for the next debug pass:

```powershell
docker logs --since 5m dmo-pg-game 2>&1 |
  Select-String -Pattern "ARCHIVE 3201|Transcendence|Hatch|Partner switch|Growth fruit|Evolution request|error|exception" -CaseSensitive:$false

Select-String -Path "C:\0-NewDmo\ClientDist\logs\*.log" `
  -Pattern "packet=16039|packet=16040|ARCHIVE RECV 3204|ARCHIVE CONTENT|TRANSCEND|PARTNER_SWITCH|PARTNER_SCALE|DIGIMON_SIZE|RECV InitGameData" `
  -CaseSensitive:$false |
  Select-Object -Last 200
```

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

Follow-up performance change on 2026-05-25:

- Encyclopedia data preload was moved into the loading flow.
  - `LoadingContents` now builds the encyclopedia static maps during loading and
    sends `SendEncyclopediaOpen()` once if the 3234 data has not been received
    yet.
  - The preload is also requested from `RecvPlayerDataLoaded()`, after the game
    connection is available. This fixes the case where the earlier loading pass
    tried before `net::game` existed, leaving the first manual open to do the
    heavy cache work.
  - `EncyclopediaContents` now tracks an in-flight server request through
    `mbIsRequesting`. Main option and popup encyclopedia opens reuse that
    pending request instead of sending another 3234 packet while preload is
    still waiting for the response.
  - `RecvEncyclopediaOpen()` now only applies/caches the server data and marks
    the encyclopedia as ready; it no longer calls
    `GetDynamicIF(WT_ENCYCLOPEDIA)` by itself. This lets the loading screen
    warm the cache without opening the encyclopedia window automatically.
  - A crash on 2026-05-25 showed `SetGroupInfoMap()` being called from the
    loading screen while walking mismatched deck/group data. The preload can
    build the full static data again after `SetGroupInfoMap()` was hardened.
  - `Recv_OpenData()` builds the encyclopedia open-state map and applies server
    state. The full static data map can be warmed by loading or built on demand
    by the encyclopedia UI/deck flow.
  - `SetGroupInfoMap()` no longer assumes `GetGroupMap()` and `GetDeckMap()`
    have the same count/order. It resolves deck data by `GroupIdx`, skips groups
    without matching deck data, and guards missing Digimon table entries before
    reading model/icon data.
  - `cEncyclopedia::Update()` has a deferred list build guard. If the window is
    created before the 3234 response is fully applied, the first frame after the
    data becomes ready rebuilds the active list automatically instead of waiting
    for the player to switch between Deck and Encyclopedia tabs.
  - Follow-up fix: `cEncyclopedia::Open()` also calls the same initial-list
    guard when the window is shown/re-shown. This fixes the case where the UI
    opened empty and only populated after switching Deck/Encyclopedia tabs.
  - Stronger follow-up: opening the window and receiving
    `ENCYCLOPEDIA_REFRESHLIST` now invalidate both Encyclopedia and Deck visual
    caches before the first lazy build. Fresh first-page builds also force the
    active scrollbar back to position 0 after `ReconfigureScrollbars()`, so an
    old scroll/cursor state cannot make the first opened view look blank.
  - Reopen-state fix: this window's `Close()` only hides it, so opening an
    already-live instance can reuse `m_nMainTab`, submenu, search, and scroll
    state from the previous session. `Open()`/`Create()` now reset the initial
    tab state to `Encyclopedia > Series`, clear search, resync the tab buttons,
    and then run the first lazy build. This preserves the old "open and show
    cards immediately" behavior while keeping lazy append for later rows.
  - Critical correction: the old full-load flow did not wait for the 3234 server
    packet before drawing the Encyclopedia list. The lazy implementation now
    also calls `EnsureStaticDataLoaded()` and runs the same main-tab selection
    path immediately, even when `IsServerDataReceived()` is still false. The
    server packet only updates open/closed state afterward.
  - The Encyclopedia visual list now appends rows lazily instead of drawing the
    whole active page in one frame. The first build adds 8 visible rows and the
    follow-up builder adds 2 rows per frame while the scrollbar is near the
    pending range. Prefetch starts when the remaining scroll rows are less than
    or equal to twice the current render count, so new cards are prepared before
    the player reaches the bottom.
  - Lazy append covers the heavy `Series`, `All`, `Class`, and `Deck` views.
    `Class` is flattened into rank header/card rows internally, so changing the
    class filter no longer forces a full card render in one frame.
  - Encyclopedia/deck list caches are invalidated on main-tab change, submenu
    change, search, reset, and live search-keyword changes. This prevents stale
    lazy cursors when switching between Deck and Encyclopedia or changing
    filters while a partial build is still active.
  - Follow-up filter fix: lazy append now bypasses the expensive full
    `_SetTabList()` rebuild path and calls only the append renderer for the
    active list. The search editbox also compares the typed text after its mouse
    update, so live text search rebuilds the first filtered lazy block instead
    of waiting for a button click or silently doing nothing.
  - Deck follow-up: favorite stars now sort matching deck groups to the top of
    the lazy source list and immediately rebuild the first deck block after a
    favorite toggle. Bookmark hit-testing now runs after the deck list updates
    its current mouse-over item, so clicks land on the correct panel. Deck
    header overlays are rendered for collecting, completed, and active states.
  - Deck completion follow-up: completed/access checks now reconcile the legacy
    base-slot walk with modern converted deck data by treating a deck card as
    collected when its destination Digimon is open anywhere in the Encyclopedia
    cache. This restores completed deck access/activation when the converted
    DBook data points to a different legacy base line.
  - Existing delta/update packets still update the cached encyclopedia state
    through `Recv_SetEvolInfo`, `Recv_InchantResult`, `Recv_EvolutionUpdate`,
    `Recv_BisReward`, and `Recv_Scale`.
  - Perf logging for encyclopedia UI/content/packet paths is now disabled by
    default in release code (`ENCY_PERF_LOG=0`, `ENCY_PACKET_PERF_LOG=0`).
    `EnsureStaticDataLoaded()` also returns immediately when both static maps
    are already built, avoiding repeated log/file I/O during UI interaction.

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

### 2026-05-23 DigiCombine and UnionCombine fusion window mapping

The official fusion window is being mapped in the isolated folder:

```text
C:\0-NewDmo\DigiCombineFeature
```

This folder is for reverse-engineering only before porting into the active
client. Do not wire it into the main project until the original UI asset/layout
map is complete.

Current source map:

- Decompiled UI functions are copied under
  `DigiCombineFeature\decompiled\DigiCombineUI` and
  `DigiCombineFeature\decompiled\UnionCombineUI`.
- Decompiled table/load functions are copied under
  `DigiCombineFeature\decompiled\DigiCombine` and
  `DigiCombineFeature\decompiled\UnionCombine`.
- Official XML table copies live under `DigiCombineFeature\xml`.
- `DigiCombineFeature\include\DigiCombineOriginalWindow.h` records the known
  class sizes, child UI offsets, button rectangles, and official asset paths.
- `DigiCombineFeature\docs\OriginalWindowMap.md` is the current visual/layout
  map for matching the official window screenshot.
- `DigiCombineFeature\assets\manifest\DecompiledUiAssetReferences.csv` is the
  source-of-truth asset list for the port: it contains only paths referenced by
  the copied decompiled UI functions.

Important asset note:

- The official interface images for this window are in `ClientDist\Data\Pack01`
  (`Pack01.hf` / `Pack01.pf`), not Pack03.
- The EXE references them by runtime asset paths such as
  `Random_box\bg.png`, `Random_box\button\done.tga`,
  `DigiCombine\DigiCombine_Slot_Empty.dds`,
  `DigiCombine\DigiCombine_CardBack.dds`, and
  `DigiCombine\DigiCombine_Gacha_*`.
- The last UI asset sweep found `49` unique asset paths directly referenced by
  the copied `.c` UI functions.
- When porting, prefer the client asset loader / `CsFPSystem` paths directly
  instead of adding duplicate texture files to source control, unless loose
  override files are needed for testing.

Key class names from RTTI:

- `CDigiCombineMainUI`, `CUnionCombineMainUI`
- `CDigiCombineRegisterUI`, `CUnionCombineRegisterUI`
- `CDigiCombineRateInfoUI`, `CUnionCombineRateInfoUI`
- `CDigiCombineResultUI`, `CUnionCombineResultUI`
- `CDigiCombineSceneUI`, `CUnionCombineSceneUI`

Known main UI composition:

- Main UI allocation size: `0x144`
- Register UI allocation size: `0x140`
- Rate info UI allocation size: `0x80`
- Result UI allocation size: `0x114`
- Scene UI allocation size: `0x88`
- Main child offsets: `0xAC` active content, `0xB0` register,
  `0xB4` rate info, `0xB8` result, `0xBC` scene.

### 2026-05-23 DigiSummon data invocation window mapping

The data invocation/gacha-style window shown as "Selecione os dados a serem
Invocados." is being mapped in a separate isolated folder:

```text
C:\0-NewDmo\DigiSummonFeature
```

Internal names from the official client/decompiled dump:

- UI name: `DigiSummonUI`
- Main class: `CDigiSummonMainUI`
- Content/data class: `CDigiSummonContents`
- Child UI classes: `CDigiSummonBottomUI`, `CDigiSummonSelectUI`,
  `CDigiSummonTicketSelectUI`, `CDigiSummonRateInfoUI`,
  `CDigiSummonResultUI`, and `CDigiSummonSceneUI`.
- Tables: `DM_DigiSummon-List.bin`, `DM_DigiSummon-ItemList.bin`, and
  `DM_DigiSummon-ItemGroup.bin`.
- Packets: `C2GS_REQ_DIGI_SUMMON_SYNC`,
  `C2GS_REQ_DIGI_SUMMON_PURCHASE`, `GS2C_RES_DIGI_SUMMON_SYNC`, and
  `GS2C_RES_DIGI_SUMMON_PURCHASE`.

Current subproject contents:

- XML copies in `DigiSummonFeature\xml`.
- BIN copies in `DigiSummonFeature\bin`.
- Decompiled UI functions in `DigiSummonFeature\decompiled\DigiSummonUI`.
- Decompiled table loader functions in
  `DigiSummonFeature\decompiled\DigiSummonTable`.
- Source-of-truth asset list in
  `DigiSummonFeature\assets\manifest\DecompiledUiAssetReferences.csv`.
- Dynamic product/card images from `DM_DigiSummon-List.xml` `IconPath` are
  listed separately in
  `DigiSummonFeature\assets\manifest\DynamicXmlAssetReferences.csv`; these do
  not appear as hardcoded paths in the copied `.c` UI functions.

Initial class layout from `011b6020` and `0128f070`:

- Main UI allocation size: `0xD8`
- Bottom UI: `0x94`
- Select UI: `0x1E8`
- Rate info UI: `0x80`
- Result UI: `0x11C`
- Scene UI: `0xA8`
- Ticket select UI: `0xAC`
- Main child offsets: `0xA8` contents, `0xAC` bottom, `0xB0` active content,
  `0xB4` select, `0xB8` rate info, `0xBC` result, `0xC0` scene,
  `0xC4` ticket select.

The UI reuses many `Random_box` assets plus the existing
`DigiCombine\DigiCombine_Gacha_*` scene textures/sounds. The confirmed asset
paths should be loaded from `ClientDist\Data\Pack01`.

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

## Seal Master, new Digimon bins, and tooltip status from 2026-05-24

This section records the active state after the Seal Master work, the attempted
large bin update, the rollback decision, and the safer forward plan for adding
new Digimon/evolutions by converting updated global bins into the older LK-DMO
client/server template.

Current decision:

- Do not bulk-replace every updated global bin directly into the old client and
  server. The first attempt proved that this can trigger unrelated startup and
  runtime crashes because the newer global data carries IDs, model references,
  tactics entries, skill methods, and apply attributes that the older template
  does not understand yet.
- Keep the last stable Seal Master update as the baseline.
- Add new Digimon and evolutions incrementally by converting the newer bin/XML
  rows into the old template, then validating the affected asset/model/tactics
  and packet paths.
- Use `AddDigimon.md` as the manual workflow for future additions.

Seal Master state:

- The Seal Master UI was updated toward the newer global style:
  - card color reflects tier/progress;
  - inventory seal card color should match the Seal Master card tier;
  - detailed seal tooltip was added to resemble global, including tier
    thresholds, owned amount, added value, total, source text, opener hint, and
    active stat section.
- The initial pink-card issue was caused by wrong/missing asset directory
  assumptions. Asset paths must be verified against Pack01/Pack03 instead of
  guessed.
- New seal assets were moved toward bin-backed lookup instead of DB-only lookup:
  added/extracted Seal Master bin family around `SealCard`, `SealLeader`,
  `SealDigimonImage`, `SealPlateImage`, `SealNatureImage`, `SealBaseImage`,
  `SealFilter`, and `SealMasterName_Str`.
- `!openseals` was corrected after the rollback: it no longer opens `3000` for
  every seal. It groups by `SealId` and applies the maximum loaded
  `RequiredAmount`/`SequentialId`, which matters because some official seals
  have lower max values.
- Character `test4` had seals reset during testing.

Rollback and server/bin state:

- A broad update from newer bins was attempted and later rolled back because the
  user decided it was too risky to keep replacing everything at once.
- The rollback target was: keep changes up to the Seal Master bin/UI work, but
  revert the later bulk server/client bin import.
- After rollback, the client startup crash at `MonsterMng.cpp:693` was resolved.
- The server was rebuilt/restarted after the rollback pass.
- If a future crash points to `MonsterMng.cpp`, `TacticsMng.cpp`,
  `QuickEvol.cpp`, `ToolTip_Skill.cpp`, or `Model.dat`, treat it as a missing
  converted dependency, not as a reason to blanket-copy all new bins.

New Digimon/evolution import findings:

- New monster/evolution rows can be imported, but they must be adapted to the
  older template.
- Directly replacing `Model.dat` from the newer client is unsafe. The old
  client can open with added models only after missing model assets are copied
  into Pack01/unpacked data and the model table remains compatible.
- Pack01 missing model/assets source used during debugging:

  ```text
  D:\LK-DMO TOOLS\EXTRATOR-Copiar Daqui\Decryptorv2\data11
  ```

- Example missing model crash:

  ```text
  Model.dat - 77017 ...
  ```

  This indicates a Digimon/model row references a model resource not available
  in the active client assets.

- Example tactics/evolution crashes:

  ```text
  TacticsMng.cpp line 75
  Condition: IsTacticsExplain(dwDigiMonID)
  DigiMon ID = 33005

  TacticsMng.cpp line 75
  Condition: IsTacticsExplain(dwDigiMonID)
  DigiMon ID = 45159
  ```

  These mean new Digimon IDs exist in evolution/monster data but do not have
  compatible tactics explanation/name data in the client table set.

- Example quick evolution crash:

  ```text
  QuickEvol.cpp line 360
  ID 77009
  index 17
  ```

  This points to missing/incompatible quick evolution/evolution-slot metadata
  for the imported route.

Evolution/skill runtime state:

- New evolutions were made visible in the Digimon Information evolution tree.
- A serious control bug was found: in Digimon-control view, new evolutions could
  fail to basic attack or use skills, while Tamer-control view worked.
- Client-side fixes were applied in:
  - `C:\0-NewDmo\dmo-client-main\DProject\Skill.cpp`
  - `C:\0-NewDmo\dmo-client-main\DProject\TamerUser.cpp`
- In Digimon-move mode, normal attack and Digimon skill routing now send the
  partner directly to the attack/skill action and keep the tamer following the
  Digimon instead of trying to move the tamer to the target.
- Server attack/skill processors touched during the same phase:
  - `PartnerAttackPacketProcessor.cs`
  - `PartnerSkillPacketProcessor.cs`

Skill tooltip crash and method compatibility:

- Hovering a skill could crash in:

  ```text
  DProject\_Interface\Game\ToolTip_Skill.cpp
  line 768
  Condition: false
  ```

- Root cause was not a missing texture. It was newer `Skill.bin` method/apply
  data reaching an old tooltip switch.
- The specific Omegamon Merciful Mode sample:

  ```text
  Skill 7112731
  Apply index 1
  ApplyA = 48
  ApplyB = 20
  BuffCode = 24
  MethodID = 209
  ```

- The current `Skill.bin` was scanned and contains these method IDs:

  ```text
  1, 2, 10, 101, 102, 104, 105, 106, 107, 108, 200,
  201, 202, 205, 206, 207, 208, 209, 301, 401, 402,
  403, 501
  ```

- Client support was added for those methods in:
  - `C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ToolTip_Skill.cpp`
  - `C:\0-NewDmo\dmo-client-main\DProject\FmCommon.cpp`
- The fix intentionally does not hide truly unknown data. Unknown methods still
  assert with:

  ```text
  SkillID
  ApplyIndex
  MethodID
  ApplyA
  ```

- A follow-up risk remains: scanning found newer `ApplyA` values that the old
  client may still not understand. One known sample is `ApplyA=91` on
  `MethodID=209` for skill `7700521`. If a future crash says
  `Undefined skill tooltip apply type` with `ApplyA=91`, map the apply attribute
  properly instead of suppressing the assert.

Digivice identify issue:

- A separate bug was reported where right-click identify on Digivice starts the
  identify progress bar but does not complete the identification, leaving the
  item identifiable again.
- Relevant server files touched or suspected:
  - `ItemIdentifyPacketProcessor.cs`
  - `ItemRerollPacketProcessor.cs`
  - `ItemListBinLoader.cs`
- Future investigation should compare the current `ItemList`/identify/reroll
  fields against the updated GDMO data and verify the server persists the new
  option state and sends the correct inventory refresh packet.

GM panel/admin command notes:

- The GM panel was expanded with additional admin pages, including Make Item,
  Summon Monster, Debug Char, Tamer EXP, Digimon EXP, AllEvo, Full Clone,
  Bits, God On/Off, Kick, Ban, Unban, and Membership.
- There was a request to add a functional scrollbar based on an existing
  clon-related window and more admin commands.
- Before extending the GM panel further, check:
  - `GameMasterCommandsProcessor.cs`
  - `DProject\_Interface\Game\GMPanel.cpp`
  - `DProject\_Interface\Game\GMPanel.h`
- Full clone was added for the active Digimon, and the command/page list is
  summarized in `GMPanelAndRecentChanges.md`.

Build/deploy verification after the latest tooltip method support:

- Client build passed with:

  ```powershell
  & "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
    "C:\0-NewDmo\dmo-client-main\DProject.sln" `
    /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
  ```

- The built executable was copied to:

  ```text
  C:\0-NewDmo\ClientDist\DProject_R_ENG.exe
  D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe
  ```

Important practical notes:

- UAC was disabled during testing, but future automation should not assume the
  client can always be killed/replaced. If `DProject_R_ENG.exe` is running,
  close it before copying the new exe.
- The client may show admin/UAC related prompts when launched. Prefer solving
  permission/manifest issues directly if repeated automated client launches are
  needed.
- When a crash dialog includes exact source, line, condition, and details, use
  that as the primary compatibility clue. Avoid replacing unrelated bins just
  because the crash happens after a bin import.

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

## DetailInfo isolated reverse map from 2026-05-23

The official "Detalhes" windows shown in the latest screenshots were mapped
into a separate root subproject:

```text
C:\0-NewDmo\DetailInfoFeature
```

Internal names found in the decompiled client:

- `DetailInfo`
- `cDetailInfoMainUI`
- `cDetailInfoMainUI_Other`
- `cDetailInfoContents`
- `cDetailInfo_Tamer`
- `cDetailInfo_Digimon`
- `cDetailInfo_Equip`
- `cDetailInfo_Seal`
- `cDetailInfo_AdditionalStat`
- `cDetailInfo_AttributeStat`
- `cDetailInfo_PopupUI`
- `AdaptDetailInfo`
- related status constructors: `TamerStatusUI`, `DigimonStatusUI`,
  `DigimonStatusUI_Help`

Primary packet/string references found:

- `C2GS_REQ_OTHER_TAMER_DETAIL_INFO`
- `GS2C_RES_OTHER_TAMER_DETAIL_INFO`
- `nsOtherTamerDetailInfo::stTamer`
- `nsOtherTamerDetailInfo::stDigimon`

Copied official decompiled functions are under:

- `DetailInfoFeature\decompiled\DetailInfoUI`
- `DetailInfoFeature\decompiled\StatusUI`
- `DetailInfoFeature\decompiled\Shared`

The copied files keep the original decompiled bodies but use clean filenames
such as `BuildTamerDetailRoot.original.c`,
`BuildDigimonDetailRoot.original.c`, `BuildVisibilityToggleButton.original.c`,
and `ResolveDetailInfoStatText.original.c`. See
`DetailInfoFeature\docs\FunctionMap.md`.

Last sweep update:

- A final xref sweep against `DetailInfo`, `DETAIL_INFO`, `TamerStatusUI`,
  `DigimonStatusUI`, `OTHER_TAMER_DETAIL_INFO`, `SEALMASTER_STATE`, and related
  window/asset strings found extra Seal Master dependencies.
- These were added to `DetailInfoFeature\decompiled\DetailInfoUI`:
  - `BuildSealMasterCloseButton.original.c`
  - `BuildSealMasterStatRows.original.c`
  - `ResolveSealMasterStatText.original.c`
  - `ResolveSealMasterDsTextCase.original.c`
  - `ResolveSealMasterStatValue.original.c`
  - `BindSealMasterCloseButton.original.c`
- The isolated map now has 31 copied decompiled functions and 36 asset
  references.
- `DetailInfoFeature\docs\LastSweepNotes.md` documents which wide-match hits
  were not copied because they belong to adjacent systems like `AutocropUI`,
  `TamerStatus_New`, or global Seal Master formatters.

The official UI asset references were extracted from copied `.c` files and
Ghidra xrefs into:

- `DetailInfoFeature\assets\manifest\Pack01RequiredAssets.csv`
- `DetailInfoFeature\assets\manifest\DecompiledXrefStringReferences.csv`
- `DetailInfoFeature\assets\manifest\DecompiledUiStringReferences.csv`
- `DetailInfoFeature\assets\manifest\ClassAndPacketStringReferences.csv`

Important: the interface textures for this window are in Pack01 /
`Data\Interface`; this subproject maps the official paths but does not extract
the packed images yet.

Key official paths for the shown windows:

- `DetailInfo\TamerStatus_Win_03_bg.tga`
- `DetailInfo\TamerStatus_Win_04_bg.tga`
- `DetailInfo\TamerStatus_win_02_bg.tga`
- `DetailInfo\Digimon_Status_Win.tga`
- `DetailInfo\Storage_M_field_a.tga`
- `DetailInfo\TamerStatus_Digimon_Tamer.tga`
- `DetailInfo\RemainTimerBG.tga`
- `DetailInfo\Autocrop_ON_Btn.bmp`
- `DetailInfo\Autocrop_OFF_Btn.bmp`
- `DetailInfo\Enchant_AT.tga`
- `DetailInfo\Enchant_ct.tga`
- `DetailInfo\Enchant_bl.tga`
- `DetailInfo\Enchant_HP.tga`
- `DetailInfo\Enchant_ev.tga`

Supporting/reused official paths also found by xref:

- `CommonUI\NinePatch\center_gradient2.tga`
- `DigimonStatus_New\Digimon_Status_Enchant_01.tga`
- `EquipSystem\Upgrade_Slot.tga`
- `Icon\Mask_Over.dds`
- `Mail\Mail_List_Box.tga`
- `TacticsHouse\CheckOnline.tga`
- `TacticsHouse\Storage_M_field.tga`

Copied table/XML dependencies for later porting:

- status: `TamerStats`, `DigimonStats`
- enchant/equipment: `AccEnchant`, `EquipEnchant`, `EnchantItemInfo`,
  `EnchantStatInfo`
- Seal Master: `SealCard`, `SealLeader`, `SealDigimonImage`,
  `SealPlateImage`, `SealNatureImage`, `SealBaseImage`, `SealFilter`
- attributes: `AttributeReward`, `AttributeBase`, `AttributeNature`
- breakthrough: `DM_Breakthrough-Stat`, `DM_Breakthrough-SlotInfo`,
  `DM_Breakthrough-Rate`, `DM_Breakthrough-Cost`,
  `DM_Breakthrough-EtcValue`

## DetailInfo implementation pass from 2026-05-25

Detailed implementation notes were added to `DetailInfo.md`. Read that file
before continuing the `Detalhes` window.

Current state:

- `DetailInfoFeature/` remains the reverse-map/reference folder for the official
  GDMO DetailInfo UI, function chain, assets, and packet names.
- The main client now has an independent movable `WT_DETAIL_INFO` window in
  `MainFrame_DetailInfoUI.cpp/.h`.
- The Tamer/equipment window now exposes a `Detalhes` button that opens this
  independent window instead of mixing DetailInfo content into the existing
  `C`/Tamer Status screen.
- The Tamer tab has the official-style background, equipment slots, 3D tamer
  render, `Equipamento` and `Selo Mestre` buttons, and a right-side
  `Atributos Obtidos` table.
- The Tamer portrait bug was fixed by matching the HUD/target-window behavior:
  render `CTamerUser::GetCharImage()->Render_L(...)`. The large tamer portrait
  asset is already circular and framed. Do not use `Render_S` here and do not
  overlay `DetailInfo\TamerStatus_Digimon_Tamer.tga` on the Tamer face because
  that asset has a dark filled center and will hide/dim the portrait.
- The latest client build passed with `Release_English|Win32` through
  `DProject.sln`, and the exe was copied to both local test clients.

Pending for later:

- Finish the user/other-player DetailInfo flow. Official packet references are
  already mapped as `C2GS_REQ_OTHER_TAMER_DETAIL_INFO`,
  `GS2C_RES_OTHER_TAMER_DETAIL_INFO`,
  `nsOtherTamerDetailInfo::stTamer`, and
  `nsOtherTamerDetailInfo::stDigimon`, but the full backend/client flow is not
  complete yet.
- Continue visual parity for the Digimon tab after the Tamer tab settles.
- Recheck the final Tamer attribute table formatting after the remaining
  user-detail packet work is implemented.

## ExtraInventory isolated reverse map from 2026-05-23

The official "Inventario Exclusivo" window shown in the latest screenshot was
mapped into a separate root subproject:

```text
C:\0-NewDmo\ExtraInventoryFeature
```

Internal names found in the decompiled client:

- `ExtraInventoryUI`
- `CExtraInventoryUI`
- `CExtraInventoryContents`
- `CExtraInvenBaseUI`
- `CExtraInvenSealUI`
- `CExtraInvenTicketUI`
- `CExtraInvenEvoUI`
- `CExtraInvenDigitamaUI`
- `CExtraInvenMaterialUI`
- `CExtraInvenSealData`
- `CExtraInvenTicketData`
- `CExtraInvenEvoData`
- `CExtraInvenDigitamaData`
- `CExtraInvenMaterialData`

Primary packet/string references found:

- `GS2C_NTF_EXTRAINVEN_SLOTEXPANSION`
- `GS2C_NTF_EXTRAINVEN_INFO`
- binder/RTTI references for `cCliExtraInventory`

Copied official decompiled functions are under:

- `ExtraInventoryFeature\decompiled\ExtraInventoryUI`
- `ExtraInventoryFeature\decompiled\ExtraInventoryUI\Categories`
- `ExtraInventoryFeature\decompiled\ExtraInventoryUI\Seal`
- `ExtraInventoryFeature\decompiled\Shared`

The copied files keep the original decompiled bodies but use clean filenames
such as `CreateExtraInventoryUI.original.c`, `BuildExtraInventoryTabs.original.c`,
`BuildSealInventoryCategory.original.c`, `BuildSealOverviewDropdown.original.c`,
and `LoadExtraInventoryEvoDataTable.original.c`. See
`ExtraInventoryFeature\docs\FunctionMap.md`.

The official UI asset references were extracted from copied `.c` files and
Ghidra xrefs into:

- `ExtraInventoryFeature\assets\manifest\RequiredAssets.csv`
- `ExtraInventoryFeature\assets\manifest\AdditionalKnownUiResourceStrings.csv`
- `ExtraInventoryFeature\assets\manifest\DecompiledXrefStringReferences.csv`
- `ExtraInventoryFeature\assets\manifest\DecompiledUiStringReferences.csv`
- `ExtraInventoryFeature\assets\manifest\ClassPacketAndTableStringReferences.csv`

Important: `NewInventory\new_inventory_win.tga` is present as a string in the
official dump and matches the root frame, but did not have a direct xref symbol
inside the copied function set. It is therefore recorded separately in
`AdditionalKnownUiResourceStrings.csv` instead of being mixed into xref-proven
assets.

Final sweep note: `ExtraInventoryFeature\docs\LastSweep_ExtraInventoryXrefs.csv`
was generated from broad searches over `ExtraInventory`, `ExtraInven`,
`EXTRAINVEN`, `EXTRA_INVEN`, `DM_Extra_Inven`, `EvoExInven`, `NewInventory`,
`Invensort`, and Seal Master markers. No new direct `ExtraInventoryUI` window
function was found missing. The remaining `Copied=NO` rows are adjacent systems:
full Seal Master screen code, generic inventory/preset messages, macro/quick
menu labels, shared `NewInventory` notification widgets, DetailInfo Seal helper
functions, and generic filter/table formatters. The interpretation is documented
in `ExtraInventoryFeature\docs\LastSweepNotes.md`.

Key official UI paths for the shown window:

- `NewInventory\new_inventory_win.tga`
- `NewInventory\new_inventory_btn_tab.tga`
- `NewInventory\new_inventory_lock.tga`
- `NewInventory\extra_inventory_notification.tga`
- `NewInventory\extra_red_highlight.tga`
- `inventory\Invensort.tga`
- `CommonUI\CommonUI_btn_s.tga`
- `Control_G\ComboBox\Combo.tga`
- `Control_G\Popup\N2Dlg5.tga`
- `Talk\Common_Seletbar.tga`
- `SealMaster\btn_combobox.tga`
- `SealMaster\dropdown_list_0`
- `SealMaster\sealmaster_highlight.tga`
- `SealMaster\sealmaster_bg_01.tga`
- `DigiCombine\DigiCombine_CardFront.dds`
- `Mail\Mail_Item_Icon.tga`

Copied table/XML dependencies for later porting:

- extra inventory: `DM_Extra_Inven-Evo_Data`
- evolution/exclusive item categories: `ExtraEvolution`, `DEvolutionList`,
  `DEvolutionAdvice`
- items/icons/text: `ItemData`, `ItemType`, `ItemRankData`,
  `ItemRankEffectData`, `QuestItemData`, `DPartItemData`, `EvoDigitama`,
  `TranscendMaterial`, `DM_ItemUpgrade-MaterialGroup`
- Seal Master: `SealCard`, `SealLeader`, `SealDigimonImage`,
  `SealPlateImage`, `SealNatureImage`, `SealBaseImage`, `SealFilter`
- language: `DM_Extra_Inven-EvoExInven_Str`, `SealMasterName_Str`,
  `Item_Str`, `ItemType_Str`, `DigitamaExplain_Str`,
  `DEvolutionAdvice_Str`, `DigimonName_Str`, `DigimonTypeName_Str`

## ItemUpgradeManagement isolated reverse map from 2026-05-23

The official "Gerenciamento" window with tabs `Fortalecimento`,
`Transferencia`, and `Recuperacao` was mapped into:

```text
C:\0-NewDmo\ItemUpgradeManagementFeature
```

Internal names and text keys found in the decompiled client:

- `EquipmentUpgradeSystem`
- `DataTradeDigimonUpgrade`
- `cEquipUpgradeSystemViewer`
- `CItemUpgradeTable`
- `UPGRADESYSTEM_TITLE`
- `EQUIPSYSTEM_UPGRADE`
- `UPGRADESYSTEM_TAB_UPGRADE_DATA_TRANSFER`
- `UPGRADESYSTEM_TAB_UPGRADE_DATA_RESTORE`

Primary copied decompiled anchors:

- `0128c370` -> `CreateEquipmentUpgradeSystemWindow.original.c`
- `0128ac10` -> `CreateDataTradeDigimonUpgradeWindow.original.c`
- `01278ad0` -> `BuildUpgradeManagementWindow.original.c`
- `01276140` -> `BuildStrengthenSlotNetwork.original.c`
- `0126d080` -> `BuildTransferTabLayout.original.c`
- `01268e20` -> `BuildRestoreTabLayout.original.c`
- `015ec9e0` -> `LoadItemUpgradeTables.original.c`

The subproject currently contains 76 copied official `.c` functions, 27 unique
asset/sound paths proven by xrefs in the copied functions, 2 additional visible
official asset strings without direct copied-set xrefs, 27 unique text keys, 11
packet/RTTI strings, 7 additional official strings without direct copied-set
xrefs, 20 copied XML/BIN table files, and 2 copied English language bins.

Key official UI paths:

- `EquipSystem\Recharge_bg.tga`
- `CommonUI\CommonUI_close.tga`
- `CommonUI\Menu_tap_btn.tga`
- `EquipSystem\Upgrade_Slot.tga`
- `EquipSystem\Upgrade_IC_BG.tga`
- `EquipSystem\Upgrade_Line_horizontal.tga`
- `EquipSystem\Upgrade_Line_vertical.tga`
- `Item_Make\Make_ani_gauge_pixel.tga`
- `EquipSystem\Upgrade_Progress_Effect.dds`
- `EquipSystem\Upgrade_Success_Effect.dds`
- `EquipSystem\Upgrade_Fail_Effect.dds`
- `EquipSystem\Upgrade_Destroy_Effect.dds`
- `EquipSystem\ArrowBtn_Up.tga`
- `EquipSystem\ArrowBtn_Down.tga`

Latest asset sweep note: all 27 required direct assets/sounds have official
xrefs. The two `ArrowBtn` textures are official strings visible in the
screenshot but had no direct copied-function xref row, so they are stored in
`ItemUpgradeManagementFeature\assets\manifest\AdditionalKnownAssetStrings.csv`.
Adjacent `Recharge_slot`, `Recharge_gauge`, `Recharge_win`, `Btn_Nor13_R`, and
extra `Item_Make` sequence/gauge assets were checked and excluded as neighboring
systems; see `ItemUpgradeManagementFeature\docs\LastSweepNotes.md`.

Copied table/XML dependencies:

- item upgrade: `DM_ItemUpgrade-Data`, `DM_ItemUpgrade-EtcValue`,
  `DM_ItemUpgrade-Transfer`, `DM_ItemUpgrade-Restore`,
  `DM_ItemUpgrade-MaterialGroup`
- item display: `ItemData`, `ItemType`, `ItemRankData`,
  `ItemRankEffectData`, `QuestItemData`
- language: `Item_Str`, `ItemType_Str`

Packet/result strings recorded separately:

- `C2GS_REQ_ITEM_UPGRADE`
- `C2GS_REQ_ITEM_UPGRADE_DATA_TRANSFER`
- `C2GS_REQ_ITEM_UPGRADE_DATA_RESTORE`
- `GS2C_RES_ITEM_UPGRADE_DATA_TRANSFER`
- `GS2C_RES_ITEM_UPGRADE_DATA_RESTORE`
- `stProcessItemUpgradeResult`

## EnchantOption / OCS / NCS isolated reverse map from 2026-05-24

The official `Encantamento` window shown for accessory/equipment option
improvement was mapped into:

```text
C:\0-NewDmo\EnchantOptionFeature
```

This subproject covers the client cluster for:

- accessory identify;
- OCS-like random option reroll (`nItem::AccOption`);
- NCS-like selected option value reroll (`nItem::OptionValue`);
- `DigiPower`;
- `Renewal`;
- optional Digivice support via `LJW_ENCHANT_OPTION_DIGIVICE_190904`.

Internal active-client anchors:

- `cEnchantOption`
- `cEnchantOptionContents`
- `AdaptEnchantOption`
- `WT_ENCHANT_OPTION`
- `E_CT_ENCHANT_OPTION_CONTENTS`
- `EVENT_CODE::RECV_ENCHANT_OPTION_IDENTIFY_RESULT`
- `EVENT_CODE::RECV_ENCHANT_OPTION_ENCHANT_RESULT`
- `EVENT_CODE::START_ENCHANT_OPTION_ENCHANT`
- `pItem::AccessoryCheck`
- `pItem::AccStoneUse`

Copied decompiled coverage:

- `decompiled\EnchantOptionUI`: 13 official `.original.c` functions,
  including factory `0128c5d0` and init `01371e40`.
- `decompiled\EnchantOptionResultUI`: 16 official `.original.c` functions,
  including factory `0128c7f0` and init `01267760`.
- `decompiled\DigiPowerEnchantUI`: 22 official `.original.c` functions,
  including factory `0128c990` and init `0125dde0`.
- `decompiled\AdditionalXrefs`: 4 direct `unpacked_exe_all` functions:
  `013729d0`, `01372d20`, `01373ea0`, and `015e9390`. These cover extra
  `ACCESSORY_*` text xrefs and the official `AccOption.bin` string.

Key active source references:

- `dmo-client-main\DProject\_Interface\Game\EnchantOption.cpp`
- `dmo-client-main\DProject\_Interface\Game\EnchantOption.h`
- `dmo-client-main\DProject\_Interface\00.Contents\EnchantOptionContents.cpp`
- `dmo-client-main\DProject\_Interface\00.Contents\EnchantOptionContents.h`
- `dmo-client-main\DProject\_Interface\Adapt\AdaptEnchantOption.cpp`
- `dmo-client-main\DProject\network\cCliGameSend.cpp`
- `dmo-client-main\DProject\network\cCliGameReceive.cpp`
- `dmo-client-main\common_vs2019\pItem.h`
- `dmo-client-main\LibProj\CsFileTable\ItemMng.cpp`

Key official UI paths:

- `Accessory\Enchant\Enchant_BG1.tga` (official wide `709x439` window loaded
  through `Data\Interface` from Pack01)
- `Accessory\Enchant\Enchant_BG.tga`
- `Accessory\Enchant\Enchant_EnchantBtn.tga`
- `Accessory\Identify\Identify_BarBG.tga`
- `Accessory\Identify\Identify_Bar.tga`
- `Accessory\Identify\Identify_BarMask.tga`
- `System\Ch_Close.tga`
- `SealMaster\dropdown_list_0`
- `Control_G\ComboBox\Combo2.tga`
- `system\Accessory\Ring.nif`
- `system\Accessory\Necklace.nif`
- `system\Accessory\Earring.nif`
- `system\Accessory\Digivice.nif`

Important packet layout:

- Identify request: `pItem::AccessoryCheck` with `u4 nUID`, `u2 nInvenPos`.
- Enchant request: `pItem::AccStoneUse` with `u4 nUID`, `u2 nItemPos`
  (stone), `u2 nAccPos` (accessory), and optional `u1 nCombIdx` for
  `OptionValue` / NCS.
- Identify response: `nInvenPos`, `nRate`, `nLevel`, `nAccOption[8]`,
  `nAccValues[8]`.
- Enchant response: `nResult`, `nInvenPos`, `nRate`, `nLevel`,
  `nAccOption[8]`, `nAccValues[8]`.

Server-side state discovered:

- The PostgreSQL server already loads `ItemList.bin` static sections
  `AccessoryOption` and `AccessoryEnchant`.
- `AccessoryRollAssetsQueryHandler` projects `AccessoryOption` into
  `AccessoryRollAssetDTO` / `AccessoryRollStatusAssetDTO` and uses
  `AccessoryEnchant` for `RerollAmount`.
- The direct decompiled sweep confirms `AccOption.bin` as an official split
  table string, but the loose split file was not present in this workspace; the
  current server can still consume the `AccessoryOption` section from
  `ItemList.bin`.
- Active GameServer packet processors now exist for `AccessoryCheck` and
  `AccStoneUse`; see the implementation section below.

Subproject docs:

- `EnchantOptionFeature\README.md`
- `EnchantOptionFeature\docs\FunctionMap.md`
- `EnchantOptionFeature\docs\PacketAndEventFlow.md`
- `EnchantOptionFeature\docs\OcsNcsBehavior.md`
- `EnchantOptionFeature\docs\TableAndServerMap.md`
- `EnchantOptionFeature\docs\LastSweepNotes.md`

## EnchantOption / OCS / NCS GameServer implementation from 2026-05-24

The mapped accessory enchant system was made functional server-side without
changing the client packets.

New central module:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\Services\AccessoryEnchantService.cs
```

Changed processors:

- `ItemIdentifyPacketProcessor` now acts as the compatible
  `pItem::AccessoryCheck` endpoint (`GameServerPacketEnum.ItemIdentify = 3968`).
- `ItemRerollPacketProcessor` now acts as the compatible `pItem::AccStoneUse`
  endpoint (`GameServerPacketEnum.ItemReroll = 3969`).

The old hardcoded processor behavior was replaced by the service while keeping
fallback support for the old stone item IDs. Official resolution path:

1. Resolve stone by `stone.ItemInfo.SkillCode`.
2. Find `ItemListBinLoader.Data.AccessoryEnchants[Index == SkillCode]`.
3. Use `ItemAccessoryEnchantRecord.ItemType` as the official stone type:
   `1 = DigiPower`, `2 = Renewal`, `3 = AccOption`, `4 = OptionValue`.
4. Use `AccessoryOption` records to roll option types/values.
5. Preserve packet payloads `3968` and `3969`.

Vital Breath / large option pool note:

- `Vital Breath Digivice -V-` (`47420`) currently resolves through
  `SkillCode = 10000523`, whose `AccOption.bin` pool has 23 rows. The expected
  modern stats are present in that pool, including `CD`, `HPRate`, `DSRate`,
  `SCDRate`, and `FinalDamageRate`.
- The server roll logic now keeps the official row-level pool instead of
  grouping by status type before the random selection. This makes the roll
  chance match the client list (`100 / OptionCount`, e.g. `4.35 %` for 23 rows)
  while still using the repeated rows as the max duplicate count for a stat.
- Re-value/NCS lookup still uses a grouped range by normalized option type, so
  selecting an existing stat rerolls within the full min/max range for that
  stat without changing packet behavior.

Loader fix:

- `ItemListBinLoader` now reads split `AccEnchant.bin` / ItemList
  `AccessoryEnchant` records as the official 8-byte layout:
  `u4 EnchantId`, `u2 OptionType`, `u2 Value`.
- This matches the copied XML/decompiled evidence in
  `EnchantOptionFeature\tables\xml\AccEnchant.xml`.

Validation run:

```text
dotnet build src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj -c Debug --no-restore
dotnet test src\Tests\DigitalWorldOnline.Game.Tests\DigitalWorldOnline.Game.Tests.csproj -c Debug --no-build
```

Result: GameServer build succeeded with 0 errors; Game tests passed 14/14.

## EnchantOption / OCS / NCS client implementation from 2026-05-24

The mapping subproject remains documentation/reference only. The functional
client changes were applied in the main C++ client:

- `C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\EnchantOption.cpp`
- `C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\EnchantOption.h`
- `C:\0-NewDmo\dmo-client-main\DProject\_Interface\00.Contents\EnchantOptionContents.cpp`
- `C:\0-NewDmo\dmo-client-main\DProject\_Interface\00.Contents\EnchantOptionContents.h`

Client behavior now follows the mapped official window more closely:

- `_CreateEnchantUI()` uses the official wide background
  `Accessory\Enchant\Enchant_BG1.tga` at `709x439`, which resolves under
  `Data\Interface` and is served from Pack01 when FilePack mode is enabled.
- The right panel renders available accessory options from the active
  `Accessory_Option` table in Pack03/ItemList data through
  `CsAccessory_Option`.
- Each available row shows option name, min/max range, and equal probability
  based on the number of non-empty option entries, matching the mapped UI
  behavior such as `6.25 %` for 16 possible entries.
- The available-options panel uses `cListBox` with `cScrollBar`, so accessory
  option pools larger than the visible area scroll instead of overlapping the
  footer text.
- The left option panel shows the current item option values and the max value
  found in the same `Accessory_Option` table.
- Accessory and stone item icons are aligned to the wide background slots from
  `Enchant_BG1.tga` instead of the old compact-window coordinates.
- NCS (`OptionValue`) still uses the official combo box assets:
  `SealMaster\dropdown_list_0` and `Control_G\ComboBox\Combo2.tga`.
- Enchant response handling now respects `nResult`: only success consumes the
  local stone; non-success responses synchronize item data and clear the
  registered stone without doing a false local decrement.

Client validation run:

```text
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" "C:\0-NewDmo\dmo-client-main\DProject.sln" /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Result: client build succeeded and produced
`C:\Release\GSP\DProject_R_ENG.exe`. The executable was copied to
`C:\0-NewDmo\ClientDist\DProject_R_ENG.exe` and
`D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`.
