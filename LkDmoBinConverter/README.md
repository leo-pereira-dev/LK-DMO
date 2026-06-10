# LKDMO Bin Converter

Dark WPF utility for converting selected GDMO split bins into LKDMO legacy templates.

Current MVP:

- Converts `ItemCreation.bin` + `ItemCreation_Str.bin` into the legacy `ItemProductionTable` section embedded in `ItemList.bin`.
- Uses `ItemCreationGroup.bin` as the legacy `AssistItemGroupTable` section.
- Preserves `ItemCreationLimitGroup.bin` as a sidecar file and reports how many crafts reference it.
- Defaults craft apply to safe mode: sidecar craft bins can be patched while the generated embedded `ItemList.bin` stays staged for diff/analyze only.
- Converts GDMO `NpcInfo.xml` Type 25 dungeon/portal NPCs into the LKDMO legacy `Npc.bin` template.
- Cross-checks Type 25 NPCs against GDMO `MapNpc.xml` placements and `MapPortal.xml` NPC portal links.
- Supports dry-run staging and optional `Pack03` patching with automatic `.bak_binconverter_YYYYMMDD_HHMMSS` backups.
- Provides a `--self-test` mode that validates conversion against local bins without patching the pack.

## Conversion modes

### Craft / ItemList.bin

Input templates:

- `ItemCreation.bin`
- `ItemCreation_Str.bin`
- `ItemCreationGroup.bin`
- `ItemCreationLimitGroup.bin`

Output:

- staged `ItemList.bin`
- optional experimental Pack03 patch for `data\bin\english\ItemList.bin`
- sidecar craft files staged/patched at their real Pack03 table paths:
  `data\bin\table\ItemCreation.bin`,
  `data\bin\table\ItemCreationGroup.bin`,
  and `data\bin\table\ItemCreationLimitGroup.bin`

Safe apply mode is the default because the legacy client reads the craft window from the embedded `ItemList.bin` section and that loader has no bounds checks. The app still generates the converted `ItemList.bin` so it can be diffed against the active pack. Enable `Patch embedded ItemList craft section` in the UI, or pass `--patch-itemlist-craft` in CLI self-test/apply mode, only after the generated embedded template is confirmed client-safe.

Crash note from the Pack03 craft investigation:

- The legacy `ItemList.bin` prefix must preserve every section before `ItemProductionTable`.
- `CsAccessory_Option::sINFO` uses `MAX_OPT_COUNT = 44`; using 16 cuts the prefix too early.
- `CsAccessory_Enchant` records are `int key + sINFO`, 12 bytes total.
- Cutting those sections early removed 176,008 bytes from the old template and caused the client FileTable thread to keep reading craft bytes as accessory data before it could reach `ProductionTable begin`.
- A correct conversion must report `ItemList.bin` byte-identical when the active pack already contains the same embedded craft payload.

### NPC Dungeon Type 25 / Npc.bin

Input templates:

- `NpcInfo.xml`
- `MapNpc.xml`
- `MapPortal.xml`

Output:

- staged `Npc.bin`
- CSV with changed Type 25 NPCs
- CSV with full active-vs-GDMO type diff
- CSV with GDMO Type 25 NPCs missing from the active LKDMO pack
- optional Pack03 patch for `data\bin\english\Npc.bin`

The Type 25 conversion intentionally writes the LKDMO-safe legacy payload:

```text
Type = 25
Payload = ItemCount = 0
```

Do not copy the raw GDMO Type 25 payload directly into LKDMO `Npc.bin`. GDMO Type 25 is an extended `NpcInfo` section with fields such as `FieldG`, `ValueCount`, and `Values[]`; the old LKDMO loader does not consume that payload layout yet. The app preserves those GDMO values in the CSV report so they can be used later when the real dungeon window and server teleport flow are implemented.

## Template expansion plan

The app is now structured as a mode-based converter. Each new GDMO template should be added as its own converter service with:

- source resolver for GDMO bin/xml exports;
- parser for the GDMO template;
- parser for the LKDMO legacy template;
- dry-run output file;
- CSV/report summary;
- optional Pack03 patch through `Pack03Archive`.

## Self-test comparison report

`--self-test` now writes two reports:

- `self-test-report.txt`: validates the conversion/parsing rules.
- `bin-compare-report.txt`: compares staged output files against the current `Pack03`.

The comparison report is informational. A file may be conversion-valid and still differ from the current pack when the pack has not been patched to that generated state yet.

Planned template families:

- `NpcInfo`, `MapNpc`, `MapPortal`, dungeon qualification tables;
- `Quest`, `QuestItemData`, quest owner/progress-related tables;
- `ItemData`, `ItemList`, craft and sidecar creation tables;
- dungeon tables such as `DungeonList`, `DungeonQualification`, `DungeonReward`, `DungeonStepInfo`, and `DungeonValue`.

Example self-test:

```powershell
dotnet run --project C:\0-NewDmo\LkDmoBinConverter -- --self-test --root C:\0-NewDmo
```

NPC Type 25 dry-run from CLI:

```powershell
dotnet run --project C:\0-NewDmo\LkDmoBinConverter -- --self-test --npc-type25 --root C:\0-NewDmo
```
