# Digimon Bin Migration Feature

This folder maps and converts newer Global/Dcommerce Digimon data into the
older LK-DMO v487 client template.

## Goal

Use updated source tables:

- `Dcommerce/xml/AllBins/Data/DigimonListData.xml`
- `Dcommerce/xml/AllBins/Data/DEvolutionList.xml`

to generate compatible legacy bins:

- `Digimon_List.bin` (`4 + count * 572`)
- `DigimonEvo.bin` (`count + 328-byte evolve objects + legacy battle-evo tail`)

The converter keeps the old binary layout expected by the current client and
server, instead of copying the newer dynamic-format bins directly.

## Current Mapping

`DigimonListData` -> `Digimon_List`:

- Existing legacy rows are regenerated from the new XML, but still written with
  the old fixed 572-byte record layout.
- Names and effect paths are written as legacy wide-string fixed buffers.
- Family/nature arrays are truncated/padded to the legacy 3-entry fields.
- New 6-skill lists are reduced to the legacy 4 hotbar skill slots.

`DEvolutionList` -> `DigimonEvo`:

- All evolution trees from the new XML are written using the old
  `CDigimonEvolveObj` 328-byte layout.
- New target arrays are copied into the legacy 9-slot evolution list.
- The legacy quick-evolution UI reserves target index `8` for devolution.
  Modern Global stores that relationship as `BaseTarget`/`DEvolReturn` data,
  so the converter materializes every non-base evolution's `BaseTarget` into
  `m_nEvolutionList[8]`. This keeps the return icon and fast-degenerate key
  visible for newly imported evolutions such as Omegamon Merciful Mode.
- Render/icon/effect/jogress data is mapped into the matching old fields.
- The old trailing battle-evolution section is preserved from the current
  legacy `DigimonEvo.bin`.

## Outputs

Generated files are written to `generated/converted/`.
Reports are written to `generated/reports/`.
