# Damage Formula System

Last updated: 2026-05-24

This document records the server-side final damage formula module added to the
LK-DMO PostgreSQL GameServer. It exists because official GDMO/DMO sources confirm
the existence of battle attribute/element interactions and separate damage
options such as Skill Damage, Critical Damage, and Final Damage, but they do not
publish one complete final-damage formula. The implementation is therefore
centralized, configurable, and designed for in-game calibration.

## External References

- GAMEKING battle guide:
  <https://dmo.gameking.com/Guide/BattleSystem.aspx>
  - Confirms the battle system uses Digimon attribute and element effectiveness.
  - Confirms damage is represented in battle as combat numbers.
- GAMEKING tamer/digimon guide:
  <https://dmo.gameking.com/Guide/TamerDigimon.aspx>
  - Confirms Digimon detail data exposes attribute and elemental attribute, and
    that favorable/unfavorable battle attributes can be checked.
- GAMEKING event page:
  <https://dmo.gameking.com/News/EventView.aspx?idx=798>
  - Lists Final Damage as an enhancement/stat option.
  - Lists Critical hit damage and Skill Damage as distinct effect lines.
- DMO Wiki attribute notes:
  <https://dmowiki.com/Digimon_Attributes>
  - Community reference for attribute/element experience and advantage context.
- DMO Wiki/Fandom attribute page:
  <https://dmo.fandom.com/wiki/Attribute>
  - Community reference for DMO attribute/status terminology.

Important limitation: none of these references publishes the full official
server-side final-damage formula. That is why the implementation is not treated
as an immutable "official formula"; it is a calibrated model with flags and
constants.

## Local Code References

New formula module:

- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Combat/DamageFormula.cs`

GameServer integration:

- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/PartnerAttackPacketProcessor.cs`
- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/PartnerSkillPacketProcessor.cs`

Configuration:

- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/appsettings.json`
- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/appsettings.Development.Example.json`

Tests:

- `dmo-server-main-postgres/src/Tests/DigitalWorldOnline.Game.Tests/Combat/DamageFormulaTests.cs`

Related existing systems:

- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Utils/Utils.cs`
  - `ApplyNatureMatrixDamage(...)`
  - `GetAttributePoint(...)`
  - `GetElementDelta(...)`
  - `HasAttributeAdvantage(...)`
  - `HasElementAdvantage(...)`
- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Digimon/DigimonModelBehavior.cs`
  - `AT`, `CC`, `CD`, `ATT`, `SCD`, `SKD`, `SkillDamagePercent`,
    `FinalDamageBasisPoints`
- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Enums/AccessoryStatusTypeEnum.cs`
  - `SCD`, `ATT`, `CD`, `SCDRate`, `FinalDamageRate`
- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Character/CharacterModelBehavior.cs`
  - equipment/accessory/chipset/digivice status aggregation.

## Why This Is Isolated

The existing project already had damage logic in packet processors. Recent work
had also introduced Final Damage and Skill Damage handling directly in those
processors. That worked, but it made future calibration risky because normal
attack, critical attack, skill attack, attribute/element bonus, and final damage
could drift or be applied twice.

The new module keeps the formula in one place:

- `DamageFormulaInput`
- `DamageFormulaConfig`
- `DamageFormulaResult`
- `DamageFormula.CalculateNormalDamage(...)`
- `DamageFormula.CalculateCriticalDamage(...)`
- `DamageFormula.CalculateSkillDamage(...)`
- `DamageFormula.CalculateDamage(...)`

The packet processors now extract current server values and call the formula.
Miss, block, aggro, cooldown, DS/HP consumption, debuff reduction, HP update,
kill handling, EXP/drop flow, and combat packets remain in the existing battle
flow.

## Configuration

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

Config meaning:

- `Enable`
  - `true`: use the centralized `DamageFormula` module.
  - `false`: use the old processor-local calculation paths.
- `EnableLog`
  - Enables per-hit `[DamageFormula]` logs. Keep off during normal gameplay.
- `CritBaseRate`
  - Base extra critical damage rate.
  - Default is `1.0`.
  - `0.8` is a candidate value for in-game testing if criticals are too high.
- `ApplyAttributeToCriticalExtra`
  - `false`: Critical Damage extra is added after the attribute-multiplied
    attack portion.
  - `true`: Critical Damage extra is also multiplied by attribute/element.
- `ApplyAttackToSkill`
  - `true`: skills include current partner AT.
  - `false`: skills use skill base/bonus only.
- `ApplyAttributeToSkillFlat`
  - `false`: flat Skill Damage is added after attribute/element.
  - `true`: flat Skill Damage is included inside attribute/element.
- `ApplyFinalDamageToSkill`
  - `true`: Final Damage affects skill damage.
  - `false`: Final Damage affects only normal/critical attacks.
- `ApplyElementDamage`
  - `true`: element advantage from `Nature.bin` is represented as a multiplier.
  - `false`: ignores the element multiplier in the formula module.

## Formula Inputs

`DamageFormulaInput` fields:

```csharp
int Attack;
int ExtraAttack;
int SkillBaseDamage;
int SkillDamageFlat;
double AttributePercent;
double ElementPercent;
double SkillDamagePercent;
double CriticalDamageExtraPercent;
double FinalDamagePercent;
double TargetReductionPercent;
bool HasAttributeAdvantage;
bool HasElementAdvantage;
bool IsCritical;
bool IsSkill;
int AttackerIndex;
int TargetIndex;
int SkillId;
```

Current value mapping:

- `Attack`
  - Normal PvE/summon attack: `client.Tamer.Partner.AT`.
  - PvP partner attack fallback path: `max(0, attacker.AT - target.DE + random)`.
  - Skill: `client.Tamer.Partner.AT`.
- `ExtraAttack`
  - Currently `0`.
  - TODO: use this only if the project gains a separated fixed extra-attack
    source that is not already folded into `Partner.AT`.
- `SkillBaseDamage`
  - `SkillCode.Apply.Value + CurrentSkillLevel * IncreaseValue`, after current
    clone/SCD pre-processing in `PartnerSkillPacketProcessor`.
- `SkillDamageFlat`
  - `client.Tamer.Partner.SKD`.
- `AttributePercent`
  - `client.Tamer.Partner.ATT`.
- `ElementPercent`
  - Positive element delta from `attackerElement.GetElementDelta(targetElement)`.
- `SkillDamagePercent`
  - `client.Tamer.Partner.SkillDamagePercent`.
- `CriticalDamageExtraPercent`
  - `client.Tamer.Partner.CD`.
- `FinalDamagePercent`
  - `client.Tamer.Partner.FinalDamageBasisPoints / 100.0`.
  - The internal model stores Final Damage as basis points.
- `TargetReductionPercent`
  - Currently used for block as `50`.
  - Existing debuff reductions are still applied after formula in current attack
    flow where they already existed.
- `HasAttributeAdvantage`
  - `attackerAttribute.HasAttributeAdvantage(targetAttribute)`.
- `HasElementAdvantage`
  - `ElementPercent > 0`.
- `IsCritical`
  - Set by the existing critical roll.
- `IsSkill`
  - Set by the skill processor.

## Multipliers

Attribute:

```text
M_attr = 1 + AttributePercent / 200
```

Only applies when `HasAttributeAdvantage = true`. Otherwise:

```text
M_attr = 1.0
```

Examples:

```text
100% attribute damage => 1.5x
 50% attribute damage => 1.25x
  0% attribute damage => 1.0x
```

Element:

```text
M_element = 1 + ElementPercent / 100
```

Only applies when `ApplyElementDamage = true` and `HasElementAdvantage = true`.
Otherwise:

```text
M_element = 1.0
```

Final Damage:

```text
M_fd = 1 + FinalDamagePercent / 100
```

Target reduction:

```text
M_target = 1 - TargetReductionPercent / 100
```

Safety:

- Negative percentages are clamped to `0`.
- `M_target` cannot go below `0`.
- Final damage floors at the end.
- Final damage returns at least `1` for positive/active hit calculations. Miss
  and block-zero behavior should remain outside the formula if ever needed.
- Overflow is guarded by calculating in `double` and clamping final value to
  `int.MaxValue`.

## Normal Damage

Formula:

```text
D_normal = floor(
    (Attack + ExtraAttack)
    * M_attr
    * M_element
    * M_fd
    * M_target
)
```

Notes:

- `Attack` is the already-effective server AT value.
- `ExtraAttack` is currently unused because known AT sources are already folded
  into partner status.
- Existing miss handling remains outside this formula.
- Existing debuff target reductions are not folded into the initial module yet
  except block as `TargetReductionPercent=50`.

## Critical Damage

Formula:

```text
CritBaseExtra = Attack * CritBaseRate
CritExtraFromBonus = CritBaseExtra * (CriticalDamageExtraPercent / 100)
```

Default behavior:

```text
D_crit = floor(
    (
        ((Attack + CritBaseExtra) * M_attr * M_element)
        + CritExtraFromBonus
    )
    * M_fd
    * M_target
)
```

If `ApplyAttributeToCriticalExtra = true`:

```text
D_crit = floor(
    (
        Attack
        + CritBaseExtra
        + CritExtraFromBonus
    )
    * M_attr
    * M_element
    * M_fd
    * M_target
)
```

Default:

```text
CritBaseRate = 1.0
ApplyAttributeToCriticalExtra = false
```

Calibration candidate:

```text
CritBaseRate = 0.8
```

## Skill Damage

Formula:

```text
SkillBaseWithPercent = SkillBaseDamage * (1 + SkillDamagePercent / 100)

D_skill = floor(
    (
        ((SkillBaseWithPercent + Attack) * M_attr * M_element)
        + SkillDamageFlat
    )
    * M_fd
    * M_target
)
```

Flags:

```text
ApplyAttackToSkill = true
ApplyAttributeToSkillFlat = false
ApplyFinalDamageToSkill = true
```

If `ApplyAttackToSkill = false`, `Attack` is treated as `0` for skill damage.

If `ApplyAttributeToSkillFlat = true`, `SkillDamageFlat` is included inside the
attribute/element multiplier:

```text
(SkillBaseWithPercent + Attack + SkillDamageFlat) * M_attr * M_element
```

If `ApplyFinalDamageToSkill = false`, skill damage uses `M_fd = 1.0`.

## Logging

Enabled by:

```json
"DamageFormula": {
  "EnableLog": true
}
```

Log format:

```text
[DamageFormula]
Attacker=%d Target=%d Skill=%d
IsSkill=%d IsCritical=%d
Attack=%d ExtraAttack=%d SkillBase=%d SkillDamageFlat=%d
AttrPercent=%.2f AttrAdv=%d AttrMul=%.4f
ElementPercent=%.2f ElementAdv=%d ElementMul=%.4f
SkillDamagePercent=%.2f CriticalDamageExtraPercent=%.2f FinalDamagePercent=%.2f
FinalDamageMul=%.4f TargetReduction=%.2f TargetMul=%.4f
RawBeforeFD=%.2f RawAfterFD=%.2f FinalDamage=%d
```

Keep logs disabled outside focused formula validation. Combat can generate many
hits per second.

## Tests

Test file:

```text
dmo-server-main-postgres/src/Tests/DigitalWorldOnline.Game.Tests/Combat/DamageFormulaTests.cs
```

Command:

```powershell
dotnet test src\Tests\DigitalWorldOnline.Game.Tests\DigitalWorldOnline.Game.Tests.csproj -c Debug
```

Current test scenarios:

### Test 1

Input:

```text
Attack=1000
AttributePercent=100
HasAttributeAdvantage=true
FinalDamagePercent=0
TargetReductionPercent=0
IsCritical=false
IsSkill=false
```

Expected:

```text
1000 * 1.5 = 1500
```

### Test 2

Input:

```text
Attack=1000
AttributePercent=100
HasAttributeAdvantage=true
FinalDamagePercent=20
TargetReductionPercent=0
IsCritical=false
IsSkill=false
```

Expected:

```text
floor(1000 * 1.5 * 1.2) = 1800
```

### Test 3

Input:

```text
Attack=1000
CritBaseRate=1.0
CriticalDamageExtraPercent=50
AttributePercent=100
HasAttributeAdvantage=true
FinalDamagePercent=0
IsCritical=true
ApplyAttributeToCriticalExtra=false
```

Expected:

```text
CritBaseExtra = 1000
CritExtraFromBonus = 500
D_crit = ((1000 + 1000) * 1.5) + 500 = 3500
```

### Test 4

Input:

```text
SkillBaseDamage=10000
Attack=1000
SkillDamagePercent=30
SkillDamageFlat=2000
AttributePercent=100
HasAttributeAdvantage=true
FinalDamagePercent=10
IsSkill=true
```

Expected:

```text
SkillBaseWithPercent = 10000 * 1.3 = 13000
Base = (13000 + 1000) * 1.5 + 2000 = 23000
Final = floor(23000 * 1.1) = 25300
```

## Validation Performed

Commands run on 2026-05-24:

```powershell
dotnet test src\Tests\DigitalWorldOnline.Game.Tests\DigitalWorldOnline.Game.Tests.csproj -c Debug --no-build
```

Result:

```text
Passed: 14 / Failed: 0
```

Command:

```powershell
dotnet build src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj -c Debug --no-restore
```

Result:

```text
Build succeeded, 0 errors.
```

Warnings remain from existing project code, including nullable warnings, `net7.0`
end-of-support warning, and known vulnerable package warnings. They are not
introduced by the formula module.

## Current Integration Scope

Integrated now:

- Partner normal attack against configured mobs.
- Partner normal attack against summon mobs.
- Partner normal attack against PvP partner target.
- Partner skill F1/F2/F3 against configured mobs.
- Partner skill F1/F2/F3 against summon mobs.

Not changed in this pass:

- Packets.
- Database schema.
- UI/client code.
- Item/stat persistence.
- Monster-to-tamer damage.
- Tamer operation duplicate/parallel damage code.
- Memory skill formula path.

## Known Parallel Damage Paths

These paths still calculate damage separately and should be reviewed before
declaring all server damage unified:

- `MapServerTamerOperation.CalculateDamage(...)`
- `DungeonsServerTamerOperation.CalculateDamage(...)`
- `EventServerTamerOperation.CalculateDamage(...)`
- `PvpServerTamerOperation.CalculateDamage(...)`
- `MapInstanceMobBehavior.AttackTarget(...)`
- `MemorySkillUsePacketProcessor` damage path.

They were left untouched to avoid reworking battle server flow and packet timing
in the same pass. The active packet processors used by player attack/skill now
have the central formula and config fallback.

## Calibration Checklist

Use `DamageFormula:EnableLog=true` temporarily and compare against in-game
expected values.

Calibrate:

- `CritBaseRate`
  - Start at `1.0`.
  - Test `0.8` if critical hits are too high.
- `ApplyAttributeToCriticalExtra`
  - Default `false`.
  - Turn on only if in-game evidence shows CD extra is attribute-scaled.
- `AttributePercent`
  - Currently mapped from `Partner.ATT`.
  - Confirm that `ATT` is the intended attribute-damage value after equipment,
    digivice, chipset, D-Unit, and buffs.
- `ElementPercent`
  - Currently uses positive `Nature.bin` element delta.
  - Confirm if element damage options should be separate from nature advantage
    and whether item-provided element damage should feed this input later.
- `SkillDamagePercent`
  - Currently maps to `Partner.SkillDamagePercent`.
  - Existing skill path still pre-processes base skill with `Partner.SCD`.
    Verify in-game whether SCD should stay in the base pre-processing, move into
    `SkillDamagePercent`, or be split into separate official-like buckets.
- `SkillDamageFlat`
  - Currently maps to `Partner.SKD`.
  - Verify if this should be affected by attribute and final damage.
- `FinalDamagePercent`
  - Currently maps from `FinalDamageBasisPoints / 100.0`.
  - Confirm all Final Damage sources are additive before entering the formula.
- `TargetReductionPercent`
  - Currently only block is represented as `50`.
  - Existing debuff reductions remain in old flow after formula.
  - Dungeon/boss/resistance reductions should be added here only after their
    current source fields are identified.

## Safety Notes

- Do not apply `UtilitiesFunctions.ApplyNatureMatrixDamage(...)` around the
  result of `DamageFormula` unless intentionally comparing legacy behavior.
  The formula already has attribute/element multipliers.
- Do not apply `ApplyFinalDamageBonus(...)` after `DamageFormula` when
  `DamageFormula:Enable=true`. The formula already includes Final Damage.
- Do not apply Skill Damage to non-skill hits.
- Do not apply Critical Damage unless `IsCritical=true`.
- Keep `DamageFormula:Enable=false` available when testing regressions.
- Keep `DamageFormula:EnableLog=false` by default.

## Next Recommended Pass

After in-game validation, unify remaining damage paths by routing them through
the same module:

1. `MemorySkillUsePacketProcessor`
2. `MapServerTamerOperation`
3. `DungeonsServerTamerOperation`
4. `EventServerTamerOperation`
5. `PvpServerTamerOperation`
6. `MapInstanceMobBehavior` for mob-to-tamer damage

Before changing those, confirm which paths are active in the current local
runtime versus legacy/background flow.
