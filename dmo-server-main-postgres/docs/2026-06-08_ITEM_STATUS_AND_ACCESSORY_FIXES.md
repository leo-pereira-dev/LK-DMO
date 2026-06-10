# 2026-06-08 - Item, ChipSet, Digivice, Tooltip, And Accessory Fixes

## Purpose

This document records the late 2026-06-08 fixes around item movement,
Digivice ChipSet state, ChipSet stat generation, Digimon status tooltip
attribution, and sealed accessory unseal behavior.

The main theme of these changes is client/server contract alignment. Several
visible bugs looked like item-data problems at first, but the fixes landed in
different places:

- client UI/tooltip rendering;
- server item generation and persistence;
- server packet ordering;
- server fallback handling for modern type-170 sealed accessories;
- Pack03/ItemList/AccOption data interpretation.

## Current Contracts

### Digivice Slots

The Digivice layout must stay aligned with the global-style layout:

```text
Normal ChipSet slots: 8
Jogress/Xros slot: 1 dedicated slot
Normal ChipSet container: 4000
Jogress/Xros EvoChip container: 3000
```

Do not add extra normal ChipSet slots to solve equip failures. The equip
failures investigated in this pass were caused by item data/stat/persistence
issues, not by needing more normal slots.

Normal ChipSet and Jogress/Xros ChipSet validation:

```text
Normal ChipSet: Type=52 and SkillCode != 2500245
Jogress/Xros ChipSet: Type=52 and SkillCode == 2500245
```

`CI_EVOCHIP` is only a client drag/drop routing type for the dedicated
container `3000`. It is not a visual cursor type and does not change the
normal cursor rendering contract.

### Tamer Equipment Slots

The current Tamer equipment packet contract is 16 normal equipment slots:

```text
Equipment size: 16
EquipmentMinSlot: 1000
EquipmentMaxSlot: 1015
XAI special packet slot: 1011
Keyring packet slot: 1015
Digivice item container: separate Digivice list
Digivice visual part: 16
```

Modern accessory types must be resolved explicitly instead of using the old
`TypeL - Head` shortcut:

```text
TypeL 34 -> NamePlate / Magnetic ID slot 13
TypeL 35 -> Keyring slot 15
TypeL 36 -> Goggles slot 14
```

The Tamer visual layout should match the original client: Magnetic
ID/NamePlate is rendered next to Costume on the lower-left overlay, Digivice is
above XAI on the right column, and Keyring is the right-column slot immediately
before XAI.

### ChipSet Generation

Normal scanned ChipSets must keep RNG values, matching the global game flow.

GM Make Item is allowed to create completed ChipSets with maximum allowed
values. That max-value behavior must stay scoped to GM creation and must not
change normal scan output.

```text
Normal scan: random power/value range
GM !item / Make Item: max values for completed ChipSets
```

Double ChipSets must populate all valid statuses from `Skill.bin`. The old bug
only copied the first `SkillCodeInfo.Apply`, so CT/HT and CT/EV looked correct
in some client text but applied only the first status on the server.

### ChipSet Family And Attribute

Family binding and Digimon attribute are separate concepts.

Examples:

```text
Family: ALL, NSP, DR, NSo, WG, VB, ME, ...
Attribute: Vaccine, Data, Virus, ...
```

Family ChipSets must keep the same family binding used by the Digimon family
validation. Do not replace family binding with Vaccine/Data/Virus attribute
data.

### CT, EV, And BL Scale

ChipSet CT/EV raw values use the client percent basis-point scale.

Example:

```text
raw CT value: 436
display: 4.36%
server contribution: 436 basis points
```

Do not multiply ChipSet CT/EV by 100 on the server. Doing so turns `436` into
`43600`, which incorrectly clamps or inflates Digimon stats.

BL remains integer percent in the server model, main client display, and combat
comparison. Tooltip attribution can convert BL locally to basis points only for
tooltip math.

```text
Server/main BL display: integer percent
Tooltip-only BL math: basis-point conversion
Combat BL comparison: integer percent
```

## Fixes

### 1. Jogress/Xros Dedicated Slot

Problem:

Jogress/Xros chips were being tested against the normal ChipSet grid and could
not reliably mirror between inventory and the Tamer Information Digivice tab.

Fix:

- client stores and renders one dedicated EvoChip/Jogress slot;
- initial information packet serializes normal `ChipSets` followed by
  `JogressChipSet`;
- server movement validation separates normal ChipSets from Jogress/Xros chips;
- normal grid remains exactly 8 slots.

Primary files:

```text
C:\0-NewDmo\dmo-client-main\DProject\Data_Digivice.h
C:\0-NewDmo\dmo-client-main\DProject\Data_Digivice.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\14.MainFrameUI\TamerStatusUI_Digivice.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameReceive.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameSend.cpp
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\GameServer\InitialInfoPacket.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\ItemMovePacketProcessor.cs
```

Detailed client note:

```text
C:\0-NewDmo\dmo-client-main\docs\portability\20_2026-06-08_DIGIVICE_JOGRESS_SLOT.md
```

### 2. Digivice Unequip And ItemMove Race

Problem:

When removing a Digivice with attached ChipSets, the client can send multiple
item-move packets close together. The in-memory move could complete, then a
parallel persistence save could hit duplicate `Shared_ItemStorageInstance`
state. This caused item visual desync, duplicated-looking inventory icons, or
stuck Digivice state.

Fix:

- `ItemMovePacketProcessor` serializes item movement per tamer with a
  `SemaphoreSlim`;
- the lock covers swap, persistence, stat update, and response packet
  generation;
- `UpdateItemsCommand` has `preserveUnreferencedInstances`;
- move persistence uses the preserve mode so an item temporarily between owner
  lists is not deleted or reinserted incorrectly;
- owner-storage upsert resolves existing item instances from incoming payload
  ids as well as current slot references;
- accessory/socket detail rows are replaced authoritatively for seen payload
  items.

Primary files:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\ItemMovePacketProcessor.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application\Separar\Commands\Update\UpdateItemsCommand.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application\Services\StoragePersistenceDualWriteCoordinator.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Infra\DigitalWorldOnline.Infraestructure\Repositories\Character\CharacterCommandsRepository.cs
```

Important behavior:

Removing a Digivice should also remove/move the attached ChipSets. That packet
flow is expected. The bug was the server save/order handling, not the fact that
attached ChipSets moved.

### 3. ChipSet Scan Multi-Status Generation

Problem:

Scanned double ChipSets could show both status lines but only apply the first
status in server state. The scan processor read only the first
`SkillCodeInfo.Apply`.

Fix:

- `ItemScanPacketProcessor` delegates ChipSet generation to
  `AccessoryEnchantService.ApplyRandomChipsetStats`;
- all valid apply entries from `Skill.bin` are copied into the item instance;
- scan power/value still uses RNG;
- GM-created completed ChipSets still use
  `AccessoryEnchantService.ApplyMaximumDefaultStats`.

Primary files:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\ItemScanPacketProcessor.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\Services\AccessoryEnchantService.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\GameMasterCommandsProcessor.cs
```

Validation examples:

```text
CT/HT ChipSet: both CT and HT must be present in AccessoryStatus
CT/EV ChipSet: both CT and EV must be present in AccessoryStatus
Normal scan: values stay inside RNG range
GM make item: completed ChipSet receives maximum allowed values
```

### 4. ChipSet CT/EV Percent Scale And Tooltip Text

Problem:

ChipSet CT was being interpreted as a raw integer stat in some places and as a
percent value in others. A tooltip like `Critical Hit 436 Increase` should mean
`4.36%`, not `436%` or an integer stat.

Fix:

- server keeps ChipSet CT/EV raw values in basis-point percent scale;
- client item tooltip renders ChipSet CT/EV as percent text;
- integer stats such as HT remain integer display.

Primary files:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\Services\AccessoryEnchantService.cs
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Base\StringAnalysis.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ToolTip_Item.cpp
```

Expected display:

```text
Critical Hit 4.36% Increase
Evasion 2.61% Increase
Hit Rate 270 Increase
```

### 5. Digimon Stat Tooltip Attribution

Problem:

The Digimon status tooltip could invent a red equipment row for unresolved
stat deltas. This made a Digimon with no equipment look like it was receiving
negative or phantom equipment CT/BL.

Fix:

- unresolved leftover deltas are no longer displayed as fake equipment rows;
- unresolved data is logged as `STAT_TOOLTIP_UNRESOLVED`;
- DUnit/Unit rows use the original Union icon source
  `Union\Filter_icon.png`;
- server-detail equipment fallback is allowed only when equipment is actually
  equipped and the server already reports a positive `TamerDetail*` value;
- CT attribution can use the authoritative server total as base when needed;
- BL basis-point conversion is tooltip-only.

Primary files:

```text
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ToolTip_Stat.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\00.Contents\MainFrame_Contents.cpp
```

Important log markers:

```text
STAT_TOOLTIP_SERVER_DETAIL_EQUIP
STAT_TOOLTIP_UNRESOLVED
```

Detailed client note:

```text
C:\0-NewDmo\dmo-client-main\docs\portability\21_2026-06-08_STATUS_TOOLTIP_DUNIT_BL.md
```

### 6. Sealed Accessory Unseal Fallback

Problem:

Some modern sealed accessories are type `170` items and show right-click
unseal text, but they do not have a `Container.bin` reward entry in the server
asset data. Example:

```text
1310631 - Sealed Zero Unit Necklace
```

The old server path treated all type-170 items without special handling as
containers. If no container config existed, the request failed before the item
could become the final accessory.

Fix:

`ItemConsumePacketProcessor` now uses this order:

1. If `Container.bin` has a container entry, keep the normal container path.
2. If no container entry exists, try sealed-accessory fallback.
3. If the fallback is not applicable, keep the old consume failure behavior.

The fallback applies only when all conditions are true:

- source item is type `170`;
- source name contains `sealed`;
- source name contains one accessory kind:
  `ring`, `necklace`, `earring`, or `bracelet`;
- normalized source name resolves to a real unsealed accessory;
- the target accessory has a valid `AccOption` key by `SkillCode` or item type.

When successful, the fallback:

- consumes one sealed item;
- creates the resolved final accessory;
- applies normal accessory identify/options through `AccessoryEnchantService`;
- persists inventory with `UpdateItemsCommand`;
- reloads inventory for the client.

Stacked sealed items require one free inventory slot, because consuming one item
from a stack does not open the original slot.

Primary file:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\ItemConsumePacketProcessor.cs
```

Important log markers:

```text
Sealed accessory unseal request
Sealed accessory unseal success
Sealed accessory unseal failed: no target found
Sealed accessory unseal failed: identify rejected target
Sealed accessory unseal failed: no inventory slot for stack split
Sealed accessory unseal failed: source consume rejected
Sealed accessory unseal failed: target add rejected after consume
```

BIN coverage validation:

```text
Sealed accessory type-170 candidates: 103
Resolved to final accessory: 103
Resolved with AccOption: 103
Missing AccOption: 0
```

Non-accessory sealed digivice/box items are intentionally not covered by this
fallback. They must continue to use their own container/use path.

## Testing Checklist

### Digivice And ChipSet

- Equip a final Digivice.
- Equip normal ChipSets into the 8 normal slots.
- Equip a Jogress/Xros chip into only the dedicated slot.
- Remove the Digivice and confirm attached ChipSets are moved/removed
  consistently.
- Reopen Tamer Information and inventory to confirm no visual duplicate or
  stuck item remains.
- Confirm no recent duplicate-key or item-move rejection logs appear.

### ChipSet Stats

- Create/scanned CT/HT and CT/EV ChipSets.
- Confirm both status lines are in item tooltip.
- Equip them and confirm both statuses change the Digimon status.
- Confirm normal scan output remains RNG and GM Make Item output remains max.
- Confirm family ChipSets still validate against family binding, not
  Vaccine/Data/Virus attribute.

### Tooltip Attribution

- Test with no equipment equipped.
- Hover CT, BL, HT, AT, DS, DE, EV.
- Confirm no phantom red equipment row appears.
- Confirm DUnit/Unit row has the Union icon when DUnit contributes stats.
- Check client logs for `STAT_TOOLTIP_UNRESOLVED` if totals still do not
  decompose cleanly.

### Sealed Accessories

- Right-click `1310631 - Sealed Zero Unit Necklace`.
- Confirm it consumes one sealed item.
- Confirm final `Zero Unit Necklace` appears.
- Confirm the final accessory is identified and has option rows.
- Test at least one ring, earring, necklace, and bracelet sealed item.
- Test stacked sealed items with full inventory and with one free slot.

## Build And Deploy

Server build:

```powershell
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln" --no-restore
```

Client x64 bridge build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=X64_WinDX9_Bridge /p:Platform=x64 /v:minimal
```

VPS deploy wrapper:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1
```

Latest deploy for this pass:

```text
Date: 2026-06-08
Archive: .codex-tmp\dmo-server-main-postgres-20260608-155153.tar.gz
Archive size: 91.7 MB
Remote backup: /home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-185210
Services: account, character, game, routine, api, admin
Postgres: not deployed or recreated
Result: target app containers rebuilt/recreated and reported Started/Up
```

## Guardrails For Future Work

- Do not change Digivice slot count while investigating item equip or tooltip
  bugs.
- Do not make normal scanned ChipSets max value.
- Do not collapse family and attribute metadata into one concept.
- Do not convert server BL to basis points without also updating main client
  display and combat comparison.
- Do not route every type-170 item through the sealed-accessory fallback. It is
  intentionally limited to sealed ring/necklace/earring/bracelet names with a
  valid final accessory and `AccOption`.
- Do not use `ReceiveItemPacket` alone as the source of truth after container
  or unseal operations; inventory reload/persistence is the authoritative
  correction path for these flows.
