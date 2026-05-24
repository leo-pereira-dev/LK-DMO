# DigimonAdvanceFeature

Subprojeto isolado para mapear a tela oficial de **Avanco de Digimon** mostrada
na print, incluindo a cena 3D, o painel inferior de atributos, o seletor
compacto superior e as popups ligadas ao fluxo.

Nomes internos encontrados nos `.c` decompilados:

- `EnchantTactics`
- `DigiPowerEnchant`
- `EnchantOption`
- `EnchantOptionResultUI`
- `DigimonTranscendenceHelp`
- `DigimonEnchantPage`

## Conteudo

- `decompiled/EnchantTacticsUI`: janela principal, botoes, validacoes,
  labels, painel de taxa/tentativas e textos oficiais.
- `decompiled/DigiPowerEnchantUI`: seletor superior "Avanco de Digimon" com
  slots de Digimon e requisitos `Encantar` / `Transcendencia`.
- `decompiled/EnchantOptionUI`: popup/opcao usada por redefinir/trocar atributo.
- `decompiled/EnchantOptionResultUI`: popup de resultado da opcao de enchant.
- `decompiled/DigimonTranscendenceHelpUI`: janela de ajuda/explicacao de
  transcendencia.
- `decompiled/DigimonAdvanceTables`: loader e callbacks de tabelas oficiais.
- `decompiled/SceneAndEffects`: som da sala, efeitos e assets dinamicos da cena.
- `tables/xml`, `tables/bin` e `tables/schemas`: XML/BIN/struct copiados do
  `Dcommerce`.
- `assets/manifest`: xrefs oficiais de assets, sons, tabelas e textos.
- `include` e `src`: mapa C++ limpo, fora do projeto principal.

## Tabelas oficiais copiadas

Estas tabelas sao carregadas diretamente por `RegisterDigimonAdvanceTables`
(`0161e970`):

- `EvoDigitama`
- `EnchantItemInfo`
- `EnchantStatInfo`
- `TranscendNeed`
- `TranscendMaterial`
- `TranscendCorrect`
- `TranscendCorrectEx`

Tambem foram copiadas tabelas de lookup usadas para nome, tooltip e item:
`ItemData`, `ItemType`, `ItemRankData`, `ItemRankEffectData`,
`QuestItemData` e `DEvolutionAdvice`.

## Manifestos principais

- `docs/CopiedFunctionFiles.csv`: 106 funcoes copiadas e renomeadas.
- `assets/manifest/RequiredAssetsTablesAndSounds.csv`: todas as referencias
  diretas encontradas nos xrefs das funcoes copiadas.
- `assets/manifest/ClassifiedAssetReferences.csv`: separacao entre assets do
  nucleo da tela, controles compartilhados e assets adjacentes de classes
  reutilizadas.
- `assets/manifest/CoreAndPopupAssetReferences.csv`: subset recomendado para
  portar a janela e suas popups.
- `assets/manifest/TextClassAndPacketReferences.csv`: chaves oficiais de texto,
  nomes de classes e RTTI.
- `assets/manifest/DirectTableLoaderBinRefs.csv`: BINs carregados pela funcao
  oficial de registro de tabelas.

## Nota de fidelidade

O trace foi feito a partir de strings e xrefs dos arquivos decompilados em
`unpacked_exe_all/functions`. As imagens soltas de interface nao estao
extraidas no workspace; a referencia de pack permanece `ClientDist\Data\Pack01.*`.
O unico asset fisico solto encontrado e copiado foi:

```text
ClientDist\Data\Music\Enchant_Room.mp3
```

Para renderizar 1:1 depois, extraia os paths do manifesto a partir do Pack01 e
preserve a arvore oficial `Data\Interface`.

## Protocolos de comunicacao

Referencia confirmada em sistemas ja funcionando:

- Union (`XmlUnionDUnit`) envia `11050` para pedir progresso e `11051` para
  reivindicar recompensa. O client recebe `11050`, `11052` e `11053`.
- No server, Union registra `XmlUnionProgressRequest = 11050` e
  `XmlUnionRewardClaim = 11051`; as respostas sao `XmlUnionProgressPacket`
  (`11050`), `XmlUnionRewardClaimResultPacket` (`11052`) e
  `XmlUnionCollectionInfoPacket` (`11053`).
- Enciclopedia usa os protocolos oficiais `pDigimon::DigimonBookInfo`,
  `pDigimon::DigimonBookItem`, `pDigimon::DigimonBookDeckUse` e
  `pDigimon::DigimonBookDeckEff`; no server os pedidos ativos sao
  `EncyclopediaLoad = 3234` e `EncyclopediaDeckUse = 3236`.

O avanco de Digimon deve ser portado com pacotes separados para sync da tela,
tentativa, troca/redefinicao de atributo e resultado. Log `No processor for
packet X` significa que o backend em execucao nao registrou handler para o
protocolo enviado pelo client.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
