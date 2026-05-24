# Recent LK-DMO changes: Seal, Digimon, skills, and GM Panel

Last updated: 2026-05-24

This file records the recent chat-driven changes that should be remembered
before continuing work in this workspace. It complements `CONTEXT.MD` and
`AddDigimon.md`.

## Build and deploy reminder

Default client build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Expected output:

```text
C:\Release\GSP\DProject_R_ENG.exe
```

Copy after build:

```text
C:\0-NewDmo\ClientDist\DProject_R_ENG.exe
D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe
```

Server game rebuild/restart:

```powershell
docker compose -f "C:\0-NewDmo\dmo-server-main-postgres\docker-compose.yml" up -d --build game
```

## Seal Master and seal inventory

The Seal Master UI was aligned closer to the current global layout:

- Seal card color is tier-aware instead of always using one color.
- The seal inventory card color follows the same tier visual as Seal Master.
- Seal inventory tooltip was updated to show the detailed seal information:
  type, class, quantity, threshold rows, added seal count, total seal count,
  source text, opener hint, master level note, and stat effect.
- A wrong asset path originally caused magenta/pink cards. The fix was to use
  the correct extracted Pack03/Seal Master asset location rather than relying
  on a missing texture path.

Command behavior:

- `!openseals` was corrected after the rollback so it no longer blindly opens
  every seal at 3000.
- It now groups seal info by `SealId` and applies the maximum known
  `RequiredAmount`/`SequentialId` from the current loaded seal table.
- This matters because many seals do not have a 3000 master threshold.

Main files involved:

```text
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\SealMaster.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ExtraInventory.cpp
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\GameMasterCommandsProcessor.cs
```

## Bin update strategy and rollback lesson

A broad attempt to replace/adapt many updated bins at once caused multiple
client compatibility failures. The important lesson is to avoid replacing all
modern bins directly into the old client/server template.

Observed failures during broad import/testing:

- `MonsterMng.cpp:693` assert from missing/inconsistent monster IDs.
- `Model.dat` missing model IDs such as `77017`.
- `TacticsMng.cpp:75` missing tactics explain for Digimon IDs such as `33005`
  and `45159`.
- `QuickEvol.cpp:360` assert for imported evolution data.
- Tooltip crashes when newer skill/buff methods were present without matching
  old-client rendering support.

Current workflow:

- Convert data family by family from updated global/GDMO bins into the old
  LK-DMO structures.
- Do not wholesale replace `Model.dat`, `Monster.bin/dat`, `Tactics.bin`,
  `Skill.bin`, or related tables without confirming the client parser and all
  referenced assets.
- Keep `AddDigimon.md` as the detailed manual workflow for adding Digimon and
  evolutions.

## New Digimon, evolutions, skills, and buffs

Recent Digimon/evolution import work focused on converting updated bin data
into the older template instead of direct replacement.

Areas touched or investigated:

- `Digimon_List.bin`
- `DigimonEvo.bin`
- `Skill.bin`
- `Tactics.bin`
- `Model.dat`
- skill effect and animation references
- client name/string bins for skill and buff display

Important fixes and notes:

- Evolution slots were made visible for newly added Digimon.
- Missing return/base evolution icon behavior was investigated for OMM and
  should apply to all newly added Digimon, not just one line.
- Some imported Digimon initially did not attack or use skills when viewed from
  the Digimon perspective. The behavior differed between tamer view and Digimon
  view, which indicates a client state/packet/targeting difference rather than
  a simple stat issue.
- OMM skill work included F3/F4 animation/effect/buff behavior and icon
  corrections. F3 was restored after a regression where the buff stopped
  applying; F4 had separate effect/animation issues.
- Skill tooltip support was extended for newer apply methods beyond the
  original method set. Method `209` was one known case, and future work should
  search for additional global methods before patching around crashes.
- Buff/debuff conversion should create visible buff icons on the player and
  visible debuff icons on monsters when the global behavior expects that.
- Skill names and buff names must be present in the relevant client string bins;
  otherwise the tooltip may show fallback text like `Skill 7112731` or
  `Buff 64421`.

Formatting rule from the latest tooltip work:

- Buff text should be readable English with spaces, for example:
  `With 100% chance Critical Damage 20% Increase (30 Second)`.
- Avoid compact generated text like `chanceCritical`.
- When one case is found, scan the generated tooltip strings for the same
  formatting problem and fix the batch.

## GM Panel overview

The GM Panel was expanded from Make Item into a broader admin tool.

Main client files:

```text
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\GMPanel.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\GMPanel.h
```

Main server files:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\GameMasterCommandsProcessor.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Models\Summon\SummonMobModelBehavior.cs
```

Current panel pages/actions include:

- Make Item
- Summon Monster
- Desbug Char
- Tamer EXP
- Digimon EXP
- AllEvo
- Full Clone
- Bits
- God On
- God Off
- Kick
- Ban
- Unban
- Membership

The left menu uses a functional scrollbar. A bug where scrolled buttons lost
their labels was fixed by refreshing visible button text from the logical page
index instead of letting hidden/offscreen buttons keep stale empty text.

## GM Panel: Summon Monster

The Summon Monster page was added to the GM Panel.

Client behavior:

- Search supports monster name or monster ID using the loaded monster table.
- Search works even when the Custom preset is selected.
- The search result list is limited to three visible entries on the summon page
  so it does not overlap the stat inputs.
- Monster result scroll clamps against three visible rows, not the item-search
  row count.
- The window height was increased to give the summon controls room.
- The stat input block was moved downward to avoid overlap with the selected
  monster text/result buttons.
- Difficulty buttons show visual selected/pressed feedback, similar in spirit
  to the Boss ranking toggle.
- Selecting a difficulty fills the stat inputs with the calculated values.

Presets:

```text
Easy   = 60% of base stats/level
Normal = 100% of base stats/level
Medium = 150% of base stats/level
Hard   = 250% of base stats/level
Custom = manual values
```

Level scales with the preset and is clamped to the client/server byte-safe
range.

Custom stat input order:

```text
Level HP DS AT DE HT EV CT AS
```

Client-side input limits:

```text
Level: 1..255
HP/DS/AT/DE/HT/EV/CT/AS: 1..999999999
Amount: existing panel amount rules, server caps summon amount to 50
```

Server-side validation also clamps values. Do not rely only on the client UI,
because a GM command can still be typed manually.

Command formats:

```text
!summonmonster {monsterId} [amount] [easy|normal|medium|hard] [boss|normal]
!summonmonster {monsterId} [amount] custom Level HP DS AT DE HT EV CT AS [boss|normal]
```

Boss behavior:

- The UI has a `Boss ranking` checkbox.
- The server uses `summon.SetClass(8)` for boss summons.
- `SummonMobModelBehavior.SetClass(int mobClass)` updates both `Class` and
  `BossMonster`.
- Boss summons also get an empty drop reward model so the boss/ranking path has
  the expected structure.

## GM Panel: admin commands

Server-side GM commands were extended to support the panel.

Important command families:

- Make item to self or online target.
- Full clone for the active Digimon.
- Bits.
- Tamer and Digimon EXP.
- AllEvo.
- God on/off.
- Kick online player.
- Ban by online/offline tamer target.
- Unban by username or tamer target.
- Membership days.
- Summon monster with preset/custom/boss controls.

Access level checks are important for kick/ban paths. Do not allow acting on
equal or higher access level targets.

## Verification from latest pass

Latest client verification:

- `DProject.sln` built successfully in `Release_English|Win32`.
- The resulting `C:\Release\GSP\DProject_R_ENG.exe` was copied to:
  - `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe`
  - `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`

Latest server verification:

- `docker compose -f "C:\0-NewDmo\dmo-server-main-postgres\docker-compose.yml" up -d --build game`
  completed.
- `dmo-pg-game` was running after rebuild.

## Future caution points

- If Custom stat fields reset while toggling Boss ranking after manual edits,
  inspect `_UpdateSummonPresetStatus()` in `GMPanel.cpp`. The custom branch may
  refill fields from base monster data when only the status text should update.
- If summon search results seem incomplete, remember the summon page uses three
  visible rows and `_ClampMonsterResultScroll()` must match that count.
- If a Digimon skill tooltip crashes, prefer finding the missing method/string
  conversion in the bins/client renderer instead of hiding the assert with a
  generic fallback.
- If a Digimon appears but does not attack, compare tamer-view and Digimon-view
  packet/state behavior before changing stats.
- If a broad bin import causes asserts, revert to the last known stable pack and
  import the specific table family manually.
