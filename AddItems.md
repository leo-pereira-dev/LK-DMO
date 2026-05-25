# Manual workflow: adicionar e atualizar itens

Last updated: 2026-05-24

Este guia descreve o caminho seguro para atualizar os bins de itens do global/
GDMO dentro do template antigo do LK-DMO.

A regra principal e a mesma usada em Digimon e Tamer: nao substituir tudo no
escuro. O servidor ja entende parte dos bins modernos em formato dividido, mas
o cliente antigo ainda depende de tabelas legadas, principalmente
`ItemList.bin` e `Skill.bin`.

## 1. Locais importantes

Workspace:

```text
C:\0-NewDmo
```

Fonte atualizada descriptografada:

```text
C:\0-NewDmo\Dcommerce\Bin\Table
C:\0-NewDmo\Dcommerce\Bin\Language
C:\0-NewDmo\Dcommerce\xml\AllBins\Data
```

Servidor ativo:

```text
C:\0-NewDmo\dmo-server-main-postgres
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english
```

Pack03 extraido usado como template do cliente:

```text
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english
```

Pasta de mapeamento desta linha de trabalho:

```text
C:\0-NewDmo\ItemBinMigrationFeature
```

## 2. Bins de item que importam

Atualizados/split modernos:

```text
ItemData.bin
Item_Str.bin
SkillCodeItem.bin
RandomBoxList.bin
RandomBoxItem.bin
RandomBoxReward.bin
ItemRankData.bin
ItemRankEffectData.bin
ItemType.bin
ItemType_Str.bin
ItemCreation.bin
ItemCreationGroup.bin
ItemCreationLimitGroup.bin
ItemCreation_Str.bin
AccOption.bin
AccEnchant.bin
Buff_Data.bin
Buff_Str.bin
CoolTime.bin
CashShop*.bin
DM_ItemUpgrade-*.bin
DM_SetItem-*.bin
```

Legado do cliente:

```text
ItemList.bin
Skill.bin
Buff.bin
```

Observacao importante:

- O servidor atual tem `ItemData.bin`, `Item_Str.bin`, `AccOption.bin` e
  `AccEnchant.bin`, entao `ItemListBinLoader` prefere o formato split.
- O Pack03 do cliente ainda tem `ItemList.bin` monolitico. Para novos itens
  aparecerem corretamente no cliente antigo, sera necessario converter o split
  moderno para o layout legado de `ItemList.bin`.
- `SkillCodeItem.bin` nao e usado diretamente pelo client/server antigo no
  runtime principal. Os efeitos precisam existir no `Skill.bin` legado.

## 3. Estado atual encontrado

Comparacao feita em 2026-05-24:

```text
Skill.bin ativo:
  rows: 5812

SkillCodeDigimon.bin:
  rows/ids: 1868
  missing in Skill.bin: 0

SkillCodeTamer.bin:
  rows/ids: 176
  missing in Skill.bin: 0

SkillCodeItem.bin:
  rows/ids: 8114
  missing in Skill.bin antes do merge: 4349
```

Conclusao:

- Digimon e Tamer ja estavam cobertos no `Skill.bin`.
- Itens nao estavam totalmente cobertos.
- Consumiveis, boxes, buffs, alimentos, itens de EXP e outros efeitos que
  dependem de `SkillCodeItem` podem falhar se o `Skill.bin` nao for mesclado.

## 4. Ordem segura de atualizacao

1. Fazer backup dos bins atuais.
2. Confirmar se o servidor ja tem os split bins atualizados.
3. Comparar `SkillCodeItem.bin` contra `Skill.bin`.
4. Mesclar `SkillCodeItem.bin` no `Skill.bin` legado.
5. Copiar o `Skill.bin` convertido para servidor e Pack03 extraido.
6. Testar item consumivel simples: HP/DS/EXP/buff.
7. Testar box/container.
8. Testar item com recompensa quando inventario esta cheio.
9. Testar item temporario.
10. So depois converter `ItemData.bin` + `Item_Str.bin` para `ItemList.bin`
    legado do cliente.

## 4.1. Conversao append-only de ItemList

Ferramenta atual:

```text
C:\0-NewDmo\ItemBinMigrationFeature\tools\Convert-ModernItemDataToLegacyItemList.ps1
```

Regra obrigatoria:

- Se o `ItemId` ja existe no `ItemList.bin` legado, manter o registro legado
  original byte-a-byte.
- Se o `ItemId` existe apenas no `ItemData.bin` moderno, converter para o
  layout antigo e adicionar.
- Se o `ItemId` existe apenas no legado, manter no final como `legacy-only`.
- Nunca substituir todos os itens antigos pelos modernos em bloco. Isso quebra
  nomes, visuais de costume/aura e outros campos legados.

Detalhe importante do `Item_Str.bin` moderno:

- A primeira string UTF-16 e o nome real exibido do item.
- A segunda string UTF-16 e uma categoria/tipo, por exemplo `Container item`,
  `Ring`, `Earring`.
- Para nome do item, usar a primeira string. Usar a segunda como fallback
  somente se a primeira vier vazia.

Relatorios gerados:

```text
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\itemlist-modern-to-legacy.csv
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\itemlist-added-items.csv
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\itemlist-name-diff.csv
```

Estado validado em 2026-05-24:

```text
ItemList legado:       14907 itens
ItemData moderno:      27320 itens
ItemList convertido:   28507 itens
Preservados do legado: 13720 itens
Novos convertidos:     13600 itens
Somente legado:         1187 itens
```

Exemplos de itens novos confirmados:

```text
45054   Zero Unit Earring
45055   Sealed Zero Unit Earring
45056   Sealed Zero Unit Ring
45057   Zero Unit Bracelet
45058   Sealed Zero Unit Bracelet
79752   Zero Unit Accessory Random Box
180708  Sealed Zero Unit Accessory Selection Box
1310630 Zero Unit Necklace
1310631 Sealed Zero Unit Necklace
1310632 Zero Unit Ring
```

## 4.2. Acessorios e opcoes modernas

O item instanciado ja trabalha com ate 8 adds/opcoes:

```text
Client: nLimit::MAX_ACCESSORY_OPTION = 8
Server: ItemModel inicializa 8 ItemAccessoryStatusModel
Pacotes: u2[8] de tipo + u2[8] de valor
```

Isso e diferente do pool de possibilidades de cada acessorio em `AccOption`.
No layout antigo do cliente, esse pool ficava em `CsAccessory_Option::sINFO`
com `MAX_OPT_COUNT=16`. O `AccOption.bin` moderno atual tem pools de ate 44
entradas, entao o client foi ajustado para:

```text
C:\0-NewDmo\dmo-client-main\LibProj\CsFileTable\Item.h
MAX_OPT_COUNT = 44
```

O conversor de `ItemList.bin` tambem substitui a cauda antiga de acessorios:

```text
AccOption antigo:   184 registros
AccOption moderno:  284 registros
AccEnchant antigo:   24 registros
AccEnchant moderno:  36 registros
Pool max moderno:    44 opcoes
```

Depois de alterar `MAX_OPT_COUNT`, recompilar o client antes de testar, porque
o tamanho binario de `CsAccessory_Option::sINFO` muda. Usar sempre o alvo:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

## 5. SkillCodeItem e efeitos de item

`SkillCodeItem.bin` usa registros de 196 bytes:

```text
SkillTableKey
SkillId
ApplyCount
Apply[ApplyCount]
LevelupPoint
MaxLevel
AttributeType
NatureType
FamilyType
UseHP
UseDS
Icon
Target
AttType
AttRange
AttRangeMinDmg
AttRangeNorDmg
AttRangeMaxDmg
AttSphere
CastingTime
DamageTime
DistanceTime
CooldownTime
SkillVelocity
SkillAccel
SkillType
LimitLevel
SkillGroup
SkillRank
MemorySkill
ReqItem
```

O template legado `Skill.bin` aceita somente 3 applies fixos por skill. Se um
registro moderno tiver mais de 3 applies, registrar em relatorio e converter
conscientemente os 3 que mantem o efeito principal.

Ferramenta criada:

```text
C:\0-NewDmo\ItemBinMigrationFeature\tools\Merge-SkillCodeItemIntoSkillBin.ps1
```

Uso:

```powershell
& "C:\0-NewDmo\ItemBinMigrationFeature\tools\Merge-SkillCodeItemIntoSkillBin.ps1" `
  -BaseSkillBin "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Skill.bin" `
  -SkillCodeItemBin "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\SkillCodeItem.bin" `
  -SkillTextXml "C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str\Skill_Str.xml" `
  -OutputSkillBin "C:\0-NewDmo\ItemBinMigrationFeature\generated\converted\Skill.bin" `
  -ReplaceExisting
```

`-ReplaceExisting` deve ser usado quando o objetivo for atualizar efeitos de
itens antigos tambem, nao apenas adicionar os ausentes.

## 6. Boxes e containers

No servidor atual, uso de box/container passa por:

```text
dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\ItemConsumePacketProcessor.cs
```

Metodo:

```text
ContainerItem(...)
```

Fluxo atual:

- item precisa ter `Type == 170`;
- procura configuracao em `_assets.Container` por `ItemId`;
- sorteia recompensas em `containerAsset.Rewards`;
- entrega no inventario;
- se inventario estiver cheio, tenta `GiftWarehouse`;
- se ambos estiverem cheios, falha sem consumir a box.

Checklist para box:

1. `ItemData.bin` define o item como tipo de container correto.
2. `Item_Str.bin` tem nome/descricao correta.
3. Existe configuracao de container no banco/assets do servidor.
4. Recompensas existem em `ItemInfo`.
5. Chance/min/max estao coerentes.
6. Inventario cheio manda para GiftWarehouse.
7. Box so e consumida depois de conseguir entregar as recompensas.

Atencao:

- `RandomBoxList.bin`, `RandomBoxItem.bin` e `RandomBoxReward.bin` existem na
  fonte global, mas o servidor atual ainda usa `_assets.Container` vindo do
  banco/config. Para ficar 100% global, precisa criar/importar um loader desses
  bins ou gerar seed SQL a partir deles.
- Enquanto esse loader nao existir, atualizar apenas `ItemData` nao atualiza as
  recompensas reais das boxes.
- Auditoria de 2026-05-24:
  - `RandomBoxList.bin`: 335 linhas.
  - `RandomBoxItem.bin`: 173 grupos.
  - `RandomBoxReward.bin`: 101 grupos.
  - Banco atual: `Asset_Container` com 1809 containers e
    `Asset_ContainerReward` com 6366 recompensas.
  - Nenhum `RandomBoxList.BoxId` bate diretamente com
    `Asset_Container.ItemId`.
  - Nenhum `RandomBoxList.ConsumeItems.ItemId` bate diretamente com
    `Asset_Container.ItemId`.
  - Relatorio:
    `C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\randombox-container-audit.csv`.
- Auditoria por `ItemData.TypeL=170`:
  - `ItemData.bin` moderno tem 5837 itens `ScanUseItem`/tipo 170.
  - 1806 desses existem em `Asset_Container`.
  - 4031 nao possuem configuracao em `Asset_Container`.
  - Isso significa que parte dos itens tipo 170 ainda falhara no servidor com
    "No container configuration" ate existir uma fonte segura de recompensa.
- Conclusao: nao importar `RandomBox*.bin` para `Asset_Container` por
  correspondencia direta. Eles parecem representar um sistema/global table
  separado, nao o mesmo contrato usado hoje por `ContainerItem(...)`.

Atualizacao de 2026-05-24:

- Foi criado um loader de bin no servidor:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\Bins\ContainerBinLoader.cs
```

- O loader le:

```text
RandomBoxList.bin
RandomBoxItem.bin
RandomBoxReward.bin
```

- O servidor agora mistura containers do banco com containers do bin, com
  prioridade para o bin quando houver o mesmo `ItemId`.
- O banco continua como fallback para nao quebrar as 1809 boxes antigas.
- O consumo de item agora chama `ContainerItem(...)` se o item tiver entrada em
  `_assets.Container`, mesmo quando `TypeL` nao for `170`.
- Isso permite testar boxes novas do `RandomBoxList` que existem no
  `ItemList.bin` moderno, como os `Attribute Item` de IDs baixos.

Mapeamento conservador usado no loader:

- `RandomBoxList.BoxId` vira `ContainerAsset.ItemId`.
- `RandomBoxList.ItemGroupId` aponta para recompensas em `RandomBoxItem.bin`.
- `RandomBoxReward.bin` fica como fallback se algum `ItemGroupId` faltar.
- `RewardAmount = 1`.
- `RandomBoxItem.FieldA / 100` vira chance em escala `0..100`.
- `RandomBoxItem.FieldB >= 4` marca `Rare`.
- Como nao existe campo de quantidade claro nesse struct, cada recompensa entra
  com `MinAmount = 1` e `MaxAmount = 1`.

Validacao no boot:

```text
Loaded RandomBox container bins: Boxes=335, ItemGroups=173, RewardGroups=101,
Containers=335, MissingItemGroups=0, MissingRewardGroups=0
```

IDs usaveis para teste foram exportados em:

```text
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\random-box-usable-test-ids.csv
```

## 7. Scan, reward e item especial

Rotas a validar:

```text
Item scan:       ItemScan packet / ScanDetail assets
Container box:   Container assets
Buff item:       Type 63/64 + SkillCode + Buff.bin/Buff_Data
Food/consume:    Type 61/71/201 + SkillCodeItem
Expansion item:  Type 155/156/159/160
Portal item:     Type 72
Memory chip:     Type 67/68/69
Tamer skill:     Type 180
Skill level open Type 202
```

Se o item mostra "Item function is not connected" ou falha sem efeito:

1. conferir `ItemData.TypeL`;
2. conferir `SkillCode` do item;
3. conferir se o `SkillCode` existe no `Skill.bin`;
4. conferir `ApplyA`, `ApplyB`, `ApplyId` e `BuffCode`;
5. conferir se o `ItemConsumePacketProcessor` tem rota para o tipo do item;
6. conferir se o client possui tooltip/nome/icone no `ItemList.bin` legado.

## 8. ItemList.bin legado do cliente

Este e o passo mais delicado. Em 2026-05-24 foi criada a primeira conversao
segura para o client antigo.

O split moderno contem:

```text
ItemData.bin
Item_Str.bin
AccOption.bin
AccEnchant.bin
```

O cliente antigo espera `ItemList.bin` com varias secoes concatenadas:

```text
Items
ItemTabs
CoolTimes
MapDisp
TypeNames
Ranks
ElementItem1
ElementItem2
Exchange
AccessoryOptions
AccessoryEnchants
ItemProduction
AssistItemGroups
XaiItems
RankEffectGroups
DPartItems
```

Regra de conversao usada:

- converter a secao `Items` de `ItemData + Item_Str`;
- preservar todas as secoes antigas apos `Items`;
- registrar qualquer campo moderno descartado;
- manter `SkillCode`, `TypeL`, `TypeS`, `TypeValue`, `UseTime`, `ScanPrice`,
  `Overlap`, `BoundType` e niveis corretamente.

Conversor criado:

```text
C:\0-NewDmo\ItemBinMigrationFeature\tools\Convert-ModernItemDataToLegacyItemList.ps1
```

Resultado aplicado em 2026-05-24:

```text
Old item count: 14907
New item count: 27320
Legacy item record size: 1596
Preserved tail bytes: 213877
Output:
C:\0-NewDmo\ItemBinMigrationFeature\generated\converted\ItemList.bin
Report:
C:\0-NewDmo\ItemBinMigrationFeature\generated\reports\itemlist-modern-to-legacy.csv
```

O `ItemList.bin` convertido foi copiado para:

```text
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english\ItemList.bin
```

E o `Pack03` ativo do `ClientDist` foi atualizado com:

```text
data\bin\english\ItemList.bin
data\bin\english\Skill.bin
```

Backup do `ItemList.bin` anterior:

```text
C:\0-NewDmo\ItemBinMigrationFeature\generated\backups
```

Validacao do arquivo gerado:

```text
ItemList.bin count: 27320
first item: 3 / Bracelet
last item: 1330029
next preserved section count: 22
```

Ainda nao converter sem novo mapeamento:

```text
ItemTap
CoolTime
MapDisp
TypeName
Rank
ElementItem
Exchange
ItemProduction
AssistItemGroup
XAI
RankEffect
DPartItem
```

Essas secoes foram preservadas do template antigo para reduzir risco de crash.

## 9. Validacao obrigatoria

Testar pelo menos:

1. item HP;
2. item DS;
3. item EXP tamer;
4. item EXP Digimon;
5. buff item;
6. box/container com inventario livre;
7. box/container com inventario cheio;
8. item temporario;
9. item de expansao de inventario/warehouse/archive;
10. memory skill chip;
11. item de tamer skill;
12. item de portal;
13. item scan;
14. tooltip do item no cliente;
15. relog depois de consumir/equipar.

## 10. Arquivos principais

Cliente:

```text
C:\0-NewDmo\dmo-client-main\LibProj\CsFileTable\ItemMng.cpp
C:\0-NewDmo\dmo-client-main\LibProj\CsFileTable\SkillMng.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ToolTip.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ExtraInventory.cpp
```

Servidor:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\Bins\ItemListBinLoader.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\Bins\SkillBinLoader.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\AssetsLoader.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\ItemConsumePacketProcessor.cs
```

## 11. Registro por importacao

Para cada lote de itens, registrar:

```text
Fonte:
Bins alterados:
Item IDs adicionados:
SkillCodeItem IDs adicionados:
SkillCodeItem IDs substituidos:
Boxes atualizadas:
RandomBox bins usados:
Seeds SQL geradas:
ItemList.bin legado gerado:
Pack03 atualizado:
Servidor atualizado:
Client build:
Server build:
Testes feitos:
Crashes/falhas:
Pendencias:
```
