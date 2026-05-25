# Server Implementation

Data: 2026-05-24

## Implantado

O GameServer agora usa um modulo central para o sistema oficial de
`AccessoryCheck` / `AccStoneUse`:

```text
dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/Services/AccessoryEnchantService.cs
```

O modulo centraliza:

- identificacao de acessorio;
- resolucao de pedra por `AccEnchant`;
- fallback por IDs antigos de stone;
- `DigiPower`;
- `Renewal`;
- `AccOption` / OCS;
- `OptionValue` / NCS;
- rolagem por `AccessoryOption`;
- validacao de slot NCS;
- preservacao do payload antigo `3968` e `3969`.

## Processors substituidos

`ItemIdentifyPacketProcessor` agora e o endpoint compativel de
`pItem::AccessoryCheck`.

```text
GameServerPacketEnum.ItemIdentify = 3968
```

`ItemRerollPacketProcessor` agora e o endpoint compativel de
`pItem::AccStoneUse`.

```text
GameServerPacketEnum.ItemReroll = 3969
```

Os nomes C# foram mantidos para compatibilidade com o projeto, mas os logs usam
os nomes oficiais:

- `AccessoryCheck/ItemIdentify`
- `AccStoneUse/ItemReroll`

## Loader corrigido

O `AccEnchant.bin` split oficial tem registros de 8 bytes:

```text
u4 EnchantId
u2 OptionType
u2 Value
```

O loader foi corrigido em:

```text
dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/Bins/ItemListBinLoader.cs
```

Agora `ItemAccessoryEnchantRecord` recebe:

- `Index = EnchantId`
- `ItemType = OptionType`
- `MinValue = Value`
- `MaxValue = Value`

Isso bate com o decompilado:

```text
GetAccessoryEnchant(pFTStone->s_dwSkill)->s_nOpt
```

## Comportamento

### Identify

1. Le o acessorio no slot enviado.
2. Resolve `AccessoryOption` por `SkillCode` ou `Type`.
3. Rola os tipos/valores de opcoes.
4. Rola `Power` por `Rank` quando existir, fallback `95..102`.
5. Define `RerollLeft` pelo limite da tabela.
6. Envia `ItemIdentifyPacket` (`3968`) com `Power`, `RerollLeft`,
   `AccOption[8]`, `AccValues[8]`.

### AccStoneUse

1. Le pedra e acessorio.
2. Resolve a pedra por `AccEnchant.EnchantId == stone.ItemInfo.SkillCode`.
3. Se a tabela nao resolver, usa fallback dos IDs antigos ja existentes.
4. Aplica:
   - `DigiPower`: altera `Power`;
   - `Renewal`: aumenta `RerollLeft`;
   - `AccOption`: rerolla tipos/valores e consome 1 renewal;
   - `OptionValue`: rerolla somente o valor da opcao selecionada e consome 1
     renewal.
5. Consome a pedra apenas quando a operacao foi aplicada.
6. Persiste status do acessorio e item da pedra.
7. Envia `ItemRerollPacket` (`3969`) com o layout oficial.

## Validacao

Comandos executados:

```powershell
dotnet build src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj -c Debug --no-restore
dotnet test src\Tests\DigitalWorldOnline.Game.Tests\DigitalWorldOnline.Game.Tests.csproj -c Debug --no-build
```

Resultado:

- build do GameServer: sucesso, 0 erros;
- testes Game: 14/14 passaram.

