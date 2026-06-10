# XML Union backend assets

Drop the backend copy of XML D-Unit files in this folder.

The game server loads this folder first and sends only lightweight progress
packets to the client. Keep the full Digimon line/reward XMLs here on disk
instead of sending them through game packets.

Supported files:

- `DM_Union-DigimonLines.xml`
- `DM_Union-DigimonCollections.xml`
- optional `DM_Union-Level.xml`

Current client tooltip note:

- DUnit/Unit stat rows are displayed with `Union\Filter_icon.png`.
- XML content warnings for invalid groups or missing Digimon ids are treated as
  content cleanup warnings, not deploy blockers, unless they are paired with a
  runtime exception.

Optional level format:

```xml
<UnionLevels>
  <Level id="0" requiredExperience="10000" />
  <Level id="1" requiredExperience="12500" />
</UnionLevels>
```
