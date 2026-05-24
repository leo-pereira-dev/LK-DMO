# ExtraInventoryFeature

Subprojeto isolado para mapear a janela oficial de **Inventario Exclusivo** da
print, especialmente a aba `Inventario de Selo`.

Nome interno principal encontrado:

- `ExtraInventoryUI`
- `CExtraInventoryUI`
- `CExtraInventoryContents`
- `CExtraInvenSealUI`
- `CExtraInvenTicketUI`
- `CExtraInvenEvoUI`
- `CExtraInvenDigitamaUI`
- `CExtraInvenMaterialUI`

O visual da aba de selo reaproveita recursos do sistema `SealMaster`, por isso
as funcoes e assets de `SealMaster` aparecem como dependencia direta da janela.

## Conteudo

- `decompiled/ExtraInventoryUI`: construtor, inicializacao, abas e slots.
- `decompiled/ExtraInventoryUI/Categories`: categorias laterais da janela.
- `decompiled/ExtraInventoryUI/Seal`: grid de selos, dropdowns e notificacoes.
- `decompiled/ExtraInventoryUI/Evolution`: grid 47x69, dropdown de grade e
  barra de busca da aba `Itens de Evolucao`.
- `decompiled/ExtraInventoryUI/Shared`: grids 32x32 reutilizados por bilhete,
  DigiOvos e materiais.
- `decompiled/Shared`: loaders de tabela/texto e mensagens compartilhadas.
- `assets/manifest`: manifestos gerados dos `.c` copiados e xrefs oficiais.
- `tables/xml` e `tables/bin`: tabelas oficiais copiadas.
- `language/bin`: bins de texto oficiais relacionados.
- `include` e `src`: mapa C++ limpo, fora do projeto principal.

## Manifestos

- `assets/manifest/RequiredAssets.csv`: assets encontrados por referencia
  direta nos `.c`/xrefs copiados.
- `assets/manifest/AdditionalKnownUiResourceStrings.csv`: strings de UI
  relacionadas, mas sem xref direto no dump copiado. Inclui
  `NewInventory\\new_inventory_win.tga`.
- `assets/manifest/DecompiledXrefStringReferences.csv`: referencias por xref.
- `assets/manifest/DecompiledUiStringReferences.csv`: referencias diretas dos
  arquivos `.c`.
- `assets/manifest/ClassPacketAndTableStringReferences.csv`: classes, pacotes e
  tabelas relacionadas.

## Ultima varredura

A varredura final esta registrada em `docs/LastSweepNotes.md` e no CSV bruto
`docs/LastSweep_ExtraInventoryXrefs.csv`.

Resultado: nao apareceu nenhuma nova funcao direta da janela
`ExtraInventoryUI` faltando no subprojeto. As referencias restantes com
`Copied=NO` sao de sistemas vizinhos: tela completa do Seal Master, mensagens
genericas do inventario, macro/quick menu, notificacoes compartilhadas de
`NewInventory`, helpers de DetailInfo e formatadores genericos.

Varredura complementar: as subfuncoes chamadas pelas categorias tambem foram
copiadas do dump principal:

- `FUN_011a58b0` -> `Evolution/BuildEvolutionInventoryGrid.original.c`
- `FUN_011a60d0` -> `Evolution/BuildEvolutionGradeDropdown.original.c`
- `FUN_011a7da0` -> `Evolution/BuildEvolutionSearchControls.original.c`
- `FUN_011b11b0` -> `Shared/BuildTicketInventoryGrid.original.c`
- `FUN_011a3020` -> `Shared/BuildDigitamaInventoryGrid.original.c`
- `FUN_011ab870` -> `Shared/BuildMaterialInventoryGrid.original.c`

`NewInventory\\new_inventory_win.tga` segue como string oficial confirmada, mas
sem xref direto no conjunto de funcoes copiadas. Por isso fica separado em
`AdditionalKnownUiResourceStrings.csv`.

## Observacao

As imagens continuam apenas mapeadas. Para renderizar 1:1, extraia os paths de
interface do Pack01 preservando `Data\\Interface`.

## Protocolos de comunicacao

Referencia confirmada em sistemas ja funcionando:

- Union (`XmlUnionDUnit`): o client envia pacotes explicitos `11050`
  (`SendXmlUnionProgressRequest`) e `11051` (`SendXmlUnionRewardClaim`) em
  `dmo-client-main/DProject/network/cCliGameShop.cpp`. O receive do client
  registra `11050`, `11052` e `11053` em
  `dmo-client-main/DProject/network/cCliGame.cpp`, chamando os handlers em
  `cCliGameReceive.cpp`.
- No server, o mesmo Union registra `XmlUnionProgressRequest = 11050` e
  `XmlUnionRewardClaim = 11051` em `GameServerPacketEnum.cs`. As respostas sao
  `XmlUnionProgressPacket` (`11050`), `XmlUnionRewardClaimResultPacket`
  (`11052`) e `XmlUnionCollectionInfoPacket` (`11053`).
- Enciclopedia usa protocolos oficiais `pDigimon::DigimonBookInfo`,
  `pDigimon::DigimonBookItem`, `pDigimon::DigimonBookDeckUse` e
  `pDigimon::DigimonBookDeckEff`. No server ativo estes aparecem como
  `EncyclopediaLoad = 3234` e `EncyclopediaDeckUse = 3236`; o processor de
  load responde com `writer.Type(3234)`.

Regra para portar esta feature: nao basta criar botao e `send()` no client. O
fluxo precisa ter ida e volta documentadas: send do client, entrada no switch de
receive quando houver resposta propria, enum do server, processor registrado e
packet de retorno que atualize a UI.

Status atual do `Inventario Exclusivo`:

- Movimento manual de item usa o fluxo padrao `pItem::Move`, com slots
  especiais `12000..16000` para as cinco categorias extras.
- Hover/estado do slot deve seguir o mesmo switch do inventario normal:
  `LBUTTON_DOWN`, `RBUTTON_DOWN`, `LBUTTON_PRESS`, `RBUTTON_PRESS`,
  `LBUTTON_UP`, `RBUTTON_UP` e `BUTTON_NONE`, mantendo `RenderMask`/tooltip
  por slot visivel.
- Controles superiores por aba:
  - `Inventario de Selo`: dropdown de stats `SealMaster`.
  - `Bolsa de Bilhete`: somente search box.
  - `Itens de Evolucao`: dropdown de rank (`N` ate `U`) + search box.
  - `DigiOvos`: somente search box.
  - `Materiais`: somente search box.
- O search box reaproveita `TacticsHouse\\Storage_search.tga` e o reset
  reaproveita `inventory\\Invensort.tga`, igual ao inventario normal.
- O dropdown reaproveita `SealMaster\\btn_combobox.tga` e
  `SealMaster\\dropdown_list_0`; na aba de evolucao ele fica estreito para
  deixar a busca ao lado.
- O botao `Trazer todos` envia `pItem::ExtraInventoryCollectAll`, protocolo
  `3988`.
- O log do client confirmou o clique e o envio:
  `SendExtraInventoryCollectAll category=0 protocol=3988`.
- O log do server em execucao retornou `No processor for packet 3988`. Portanto
  a falha atual nao esta no clique da janela; esta no registro/build do handler
  `ExtraInventoryCollectAllPacketProcessor` no backend que esta rodando.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
