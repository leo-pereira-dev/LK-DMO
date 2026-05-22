# XML Union backend assets

Drop the backend copy of XML D-Unit files in this folder.

The game server loads this folder first and sends only lightweight progress
packets to the client. Keep the full Digimon line/reward XMLs here on disk
instead of sending them through game packets.

Supported files:

- `DM_Union-DigimonLines.xml`
- `DM_Union-DigimonCollections.xml`
- optional `DM_Union-Level.xml`

Optional level format:

```xml
<UnionLevels>
  <Level id="0" requiredExperience="10000" />
  <Level id="1" requiredExperience="12500" />
</UnionLevels>
```
