# DigitalWorldOnline.Game.Tests

Focused unit-test harness for packet and slot contracts that are hard to debug
only through the live client.

Run from the repository root:

```powershell
dotnet test "C:\0-NewDmo\dmo-server-main-postgres\src\Tests\DigitalWorldOnline.Game.Tests\DigitalWorldOnline.Game.Tests.csproj" -v:minimal
```

Current coverage:

- Archive `3201` client-to-server slot contract: `0` active, `1..N`
  mercenary slots without subtracting one.
- Active Digimon archive behavior with mocked active/mercenary/archive data.
- Archive restore compaction behavior.
- Transcendence `16039` success/failure packet layout, including the v487
  `sItemInfo` padding before `cItemData`.
