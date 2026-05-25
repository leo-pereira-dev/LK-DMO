# Table And Server Map

## Client tables

O client ativo busca metadados de pedra por:

```text
nsCsFileTable::g_pItemMng->GetAccessoryEnchant(pFTStone->s_dwSkill)
```

O loader C++ antigo usa `ItemMng.cpp` para preencher:

```text
m_mapAccessoryEnchant
```

Campos observados em `CsAccessory_Enchant` no uso da janela:

- indice/id da tabela;
- `s_nOpt`: tipo da pedra (`DigiPower`, `Renewal`, `AccOption`, `OptionValue`);
- valor minimo/maximo ou valor associado, dependendo da leitura da tabela.

## Server PostgreSQL atual

O server ja possui leitura estatica do `ItemList.bin`:

- `ItemAccessoryOptionRecord`
- `ItemAccessoryOptionSlot`
- `ItemAccessoryEnchantRecord`
- `ItemListSectionCounts.AccessoryOption`
- `ItemListSectionCounts.AccessoryEnchant`

Arquivos principais:

- `dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/Bins/ItemListBinLoader.cs`
- `dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/Bins/ItemList.cs`
- `dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/Queries/AccessoryRollAssetsQueryHandler.cs`
- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/Program.cs`

O log do GameServer ja informa:

```text
Loaded ItemList.bin static sections: ... AccessoryOption={AccessoryOption}, AccessoryEnchant={AccessoryEnchant}
```

`AccessoryRollAssetsQueryHandler` agrupa opcoes por `ItemType`, cria
`AccessoryRollAssetDTO`, popula `StatusAmount` e usa `AccessoryEnchant` para
`RerollAmount`.

## Arquivos/tabelas copiados aqui

- `tables/bin/AccEnchant.bin`
- `tables/xml/AccEnchant.xml`

`AccOption.bin` nao foi encontrado como arquivo solto nesta workspace durante a
varredura; porem a busca direta no decompilado confirmou a string oficial
`AccOption.bin` em `015e9390`. No server moderno ele pode vir do `ItemList.bin`
ou de arquivo split quando `AccOption.bin` existir no diretorio de bins.

## Implementacao server pendente

Implementacao concluida no backend:

1. `ItemIdentifyPacketProcessor` atende `pItem::AccessoryCheck` / `3968`.
2. `ItemRerollPacketProcessor` atende `pItem::AccStoneUse` / `3969`.
3. `AccessoryEnchantService` centraliza as regras.
4. `ItemListBinLoader` le `AccEnchant.bin` split como registros oficiais de 8
   bytes.
5. Identify preenche `nRate`, `nLevel`, `nAccOption` e `nAccValues`.
6. `AccOption` escolhe novos tipos/valores.
7. `OptionValue` rerolla apenas `nCombIdx`.
8. Status do item e consumo da pedra sao persistidos.

## Testes recomendados

- Identificar acessorio sem opcoes e confirmar que o client recebe
  `RECV_ENCHANT_OPTION_IDENTIFY_RESULT`.
- Usar OCS em item com opcoes e conferir se todos os arrays mudam de forma
  coerente com `AccessoryOption`.
- Usar NCS selecionando cada indice valido e conferir se so o valor selecionado
  muda.
- Enviar indice invalido no NCS e confirmar erro sem consumo da pedra.
- Testar limite de `Renewal` e `DigiPower`.
- Validar que item travado/em trade/warehouse nao pode ser alterado.
