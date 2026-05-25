# Encyclopedia deck system

Last updated: 2026-05-25

This note documents the current LK-DMO implementation for Digimon Encyclopedia
decks: converted deck data, lazy UI rendering, tooltip formatting, active deck
persistence, and server-side stat application.

## Data source

The active legacy client/server deck table is:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Digimon_Book.bin
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english\Digimon_Book.bin
```

The conversion subproject is:

```text
C:\0-NewDmo\DeckBinMigrationFeature
```

The converter reads modern GDMO DBook XML from:

```text
C:\0-NewDmo\Dcommerce\xml\AllBins\Data
C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str
```

and writes:

```text
C:\0-NewDmo\DeckBinMigrationFeature\generated\converted\Digimon_Book.bin
```

## Legacy deck record layout

The legacy client reads `CsEncy_Deck::sINFO` from
`LibProj/CsFileTable/Digimon.h`. The important deck-effect layout is:

```text
USHORT GroupIdx
TCHAR  GroupName[64]      = 128 bytes
TCHAR  Explain[512]       = 1024 bytes
USHORT Condition[3]
USHORT AttackType[3]
USHORT Option[3]
USHORT Value[3]
USHORT pad                = required C++ alignment before UINT arrays
UINT   Probability[3]
UINT   Time[3]
```

The 2-byte pad before `Probability` is mandatory. If it is missing, the client
reads `Probability` and `Time` shifted by 16 bits and the tooltip shows values
such as `1310720.0% chance` or `458752 seconds`.

Current compatibility:

- `DeckBinMigrationFeature/tools/Convert-ModernDBookToLegacyDigimonBook.ps1`
  writes the alignment pad.
- `LibProj/CsFileTable/DigimonMng.cpp` normalizes both layouts when loading,
  so older no-pad converted bins do not break the tooltip while packs are being
  refreshed.
- `Application.GameAssets/Bins/DigimonBookBinLoader.cs` also accepts both
  layouts on the server and chooses the sane `Probability/Time` values.

## Option IDs

Legacy deck option IDs currently used by client/server code:

```text
1 AT additional damage
2 Skill additional damage
3 Critical damage increase
4 Skill cooldown reset
5 Max HP increase
6 Attack speed increase
```

The converted modern data may also contain IDs 7 to 11:

```text
7 Hit Rate
8 Attack Amplification
9 Attribute Damage Increase
10 Attack Increase
11 Final Damage Increase
```

The data can be displayed, but effect application for IDs 7 to 11 is still a
TODO in the legacy runtime. Check
`DeckBinMigrationFeature/generated/reports/deck-unsupported-options.csv` before
adding those effects.

## Client flow

Key files:

```text
dmo-client-main/LibProj/CsFileTable/DigimonMng.cpp
dmo-client-main/DProject/_Interface/Game/Encyclopedia.cpp
dmo-client-main/DProject/_Interface/Game/EncyclopediaContents.cpp
dmo-client-main/DProject/_Interface/Game/ToolTip.cpp
dmo-client-main/DProject/_Interface/Game/BuffInfo.cpp
```

Important behavior:

- `DigimonMng.cpp` loads `Digimon_Book.bin` and normalizes deck timing fields.
- `EncyclopediaContents` owns the static encyclopedia/deck maps and active deck
  state received from the server.
- `Encyclopedia.cpp` builds the encyclopedia/deck UI lazily to avoid large frame
  stalls when opening, switching tabs, filtering, or scrolling.
- Deck favorites are cached locally and sorted to the top of the Deck view.
- `ToolTip.cpp` displays deck effects with corrected probability/time values and
  wider label/value spacing.
- `BuffInfo.cpp` uses wider deck-buff tooltips so effect text does not collide.

## Server flow

Key files:

```text
dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/Bins/DigimonBook.cs
dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/Bins/DigimonBookBinLoader.cs
dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/Program.cs
dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/EncyclopediaDeckUsePacketProcessor.cs
dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Character/CharacterModelBehavior.cs
dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Digimon/DigimonModelBehavior.cs
dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Utils/Utils.cs
```

Boot flow:

```text
Program.cs
  -> DigimonBookBinLoader.Load()
  -> UtilitiesFunctions.RegisterEncyclopediaDeckEffects(...)
```

Use flow:

```text
Client sends pDigimon::DigimonBookDeckUse / GameServerPacketEnum.EncyclopediaDeckUse
  -> EncyclopediaDeckUsePacketProcessor
  -> CharacterModel.SetEncyclopediaDeck(deckId)
  -> UpdateCharacterEncyclopediaDeckCommand persists Character_Tamer.EncyclopediaDeckId
  -> UpdateStatusPacket refreshes visible partner status
```

Current server-side stat effects:

- Passive option `6` applies attack speed increase as a reduction to final AS:

```text
AS = baseAS - otherASBonuses - floor(baseAS * deckPercent / 100)
```

Example:

```text
baseAS=2500, deck AS=22%
AS = 2500 - floor(2500 * 22 / 100) = 1950
```

- Passive option `5` applies Max HP increase:

```text
HP += floor(baseHP * deckPercent / 100)
```

Other option IDs are loaded/logged but still need explicit server combat/stat
integration before they are considered functional.

## Logs

At Game.Host boot:

```text
Loaded Digimon_Book.bin: 11 options, 57 decks, 58 composition groups
```

When a player enables/disables a deck:

```text
[EncyclopediaDeckUse] Character <id> persisted encyclopedia deck <deckId> enabled=<bool>; effects=cond=.../atk=.../opt=.../val=.../prob=.../time=...
```

Useful reference deck:

```text
Group 1030 - White Wings : Gurei Tou of Courage
Condition: 1,3,3
AttackType: 0,1,1
Option: 6,3,1
Value: 22,125,30
Probability: 0,1000,2000
Time: 0,10,7
```

Expected tooltip display for group 1030:

```text
Activation Condition  Always
Activation Effect     Attack Speed 22% Increase
Lasting time          Permanent effect enabled

Activation Condition  Normal attack With 10.0% chance
Activation Effect     Critical hit damage 125% Increase
Lasting time          lasts for 10 seconds after activation

Activation Condition  Normal attack With 20.0% chance
Activation Effect     Attack 30% Additional Damage
Lasting time          lasts for 7 seconds after activation
```

## Validation commands

Client build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Server build:

```powershell
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj" -c Release --no-restore
```

Docker deploy:

```powershell
docker compose -f "C:\0-NewDmo\dmo-server-main-postgres\docker-compose.yml" build game
docker compose -f "C:\0-NewDmo\dmo-server-main-postgres\docker-compose.yml" up -d --no-deps --force-recreate game
```

Validated on 2026-05-25:

- Client `Release_English|Win32` built successfully.
- Server `DigitalWorldOnline.Game.csproj` built successfully.
- Docker `game` image rebuilt and `dmo-pg-game` recreated.
- Boot log confirmed `Digimon_Book.bin` registration.
