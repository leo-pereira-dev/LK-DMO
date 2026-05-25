# Deck bin migration

Migration notes for converting recent GDMO deck/encyclopedia data
(`DBook*.xml`) into the LK-DMO legacy table (`Digimon_Book.bin`).

## Source

Modern data:

```text
C:\0-NewDmo\Dcommerce\xml\AllBins\Data
C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str
```

Files used:

```text
DBookDeckData.xml
DBookCompositionData.xml
DBookOptionData.xml
DBookExceptionData.xml
DBookDeck_Str.xml
DBookOpt_Str.xml
DigimonName_Str.xml
```

## Target template

The legacy client loads:

```text
Data\Bin\English\Digimon_Book.bin
```

The active C++ reader is:

```text
C:\0-NewDmo\dmo-client-main\LibProj\CsFileTable\DigimonMng.cpp
```

Validated sections:

- encyclopedia option data;
- exception digimon data;
- deck effect data;
- deck composition data.

Important legacy limit:

```text
ENCY_MAX_OPTION = 3
```

The converter keeps at most 3 effects per deck and reports any fourth modern
effect instead of overflowing the legacy binary layout.

### Required effect padding

The C++ `CsEncy_Deck::sINFO` layout aligns the `UINT s_nProb[3]` and
`UINT s_nTime[3]` arrays to a 4-byte boundary. Because the previous arrays are
`USHORT`, there is a required 2-byte pad between `s_nVal[3]` and `s_nProb[3]`.

Correct effect layout:

```text
USHORT Condition[3]
USHORT AttackType[3]
USHORT Option[3]
USHORT Value[3]
USHORT pad
UINT   Probability[3]
UINT   Time[3]
```

Without this pad, the client reads probability/time shifted by 16 bits and the
tooltip can show values like `1310720.0% chance` or `458752 seconds`.

Current safeguards:

- the converter writes the alignment pad;
- the C++ client loader normalizes both padded and older no-pad converted bins;
- the C# server loader also accepts both layouts.

## Converter

Script:

```text
C:\0-NewDmo\DeckBinMigrationFeature\tools\Convert-ModernDBookToLegacyDigimonBook.ps1
```

Default output:

```text
C:\0-NewDmo\DeckBinMigrationFeature\generated\converted\Digimon_Book.bin
```

Reports:

```text
C:\0-NewDmo\DeckBinMigrationFeature\generated\reports
```

Main reports:

```text
digimon-book-summary.csv
deck-conversion.csv
deck-truncated-options.csv
deck-unsupported-options.csv
missing-digimon-names.csv
string-truncations.csv
```

## Application

After generation and validation, copy the converted bin to:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Digimon_Book.bin
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english\Digimon_Book.bin
```

Then patch the active Pack03 with:

```powershell
.\SealMasterFeature\tools\Patch-Pack03Files.ps1 `
  -RelativeFiles @("data\bin\english\Digimon_Book.bin")
```

Status for the 2026-05-25 pass:

- `Digimon_Book.bin` converted and validated.
- File copied to the server bin folder and extracted Pack03 folder.
- Active `Pack03.pf/.hf` patched in `C:\0-NewDmo\ClientDist\Data`.

Conversion summary:

```text
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

## Modern options

The recent source has deck options up to ID 11:

```text
1  Additional damage to attack
2  Additional damage to skill
3  Increase critical hit damage
4  Reset skill cooldown time
5  Increase Max HP
6  Increase attack speed
7  Increase Hit Rate
8  Attack Amplification
9  Attribute Damage Increase
10 Attack Increase
11 Final Damage Increase
```

The converted bin can carry the modern IDs, but the legacy runtime still needs
explicit effect support before options 7 to 11 are functional. Use
`deck-unsupported-options.csv` as the implementation checklist.

## Runtime status

Detailed client/server flow, deck activation, tooltip formatting, logs, and
validation steps are documented in:

```text
C:\0-NewDmo\EncyclopediaDeckSystem.md
```

Current functional state:

- client loads `Digimon_Book.bin` with `Probability/Time` normalization;
- deck tooltip uses corrected values and wider text spacing;
- server loads `Digimon_Book.bin` through `DigimonBookBinLoader`;
- active deck persists in `Character_Tamer.EncyclopediaDeckId`;
- passive AS (`Option=6`) reduces the real partner AS server-side;
- passive HP (`Option=5`) increases the real partner HP server-side;
- modern options 7 to 11 remain TODO for functional application.
