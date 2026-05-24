# UnionCombineFusionSelectFeature

Subprojeto isolado para mapear a tela oficial de selecao de item para fusao
mostrada no screenshot novo.

Esta tela parece a janela de `DigiCombineFeature`, mas a varredura mostrou que
ela pertence a outra familia oficial:

- Window runtime name: `UnionCombineUI`
- Classe principal: `CUnionCombineMainUI`
- Tabelas oficiais: `DM_UnionCombine-*`
- Rank list oficial: 7 registros (`N`, `A`, `A+`, `S`, `S+`, `SS`, `SS+`)
- Pack de interface: `ClientDist\Data\Pack01.*`

O ponto que separa esta tela da janela antiga e o rank list: `DM_DigiCombine`
tem 8 registros no dump atual, enquanto `DM_UnionCombine` tem 7, batendo com
os filtros visiveis no screenshot (`N` ate `SS+`).

## Conteudo

- `decompiled/UnionCombineUI`: funcoes `.c` copiadas do dump original para a UI.
  Inclui tambem a entrada adjacente `010a78d0`, que referencia diretamente
  `UnionCombine.mp3`.
- `decompiled/UnionCombineTables`: funcoes `.c` copiadas do dump original para
  leitura/parsing das tabelas.
- `tables/xml`: XML oficiais copiados de `Dcommerce\xml\AllBins\Data`.
- `tables/bin`: BIN oficiais copiados de `Dcommerce\Bin\Table`.
- `language/bin/English`: bins de idioma necessarios para nomes de itens.
- `assets/manifest`: manifestos gerados a partir de xrefs dos `.c` copiados.
- `include` e `src`: mapa limpo com nomes de classes, funcoes, offsets,
  botoes, filtros e assets principais.

## Arquivos principais

- `docs/CopiedFunctionFiles.csv`
- `docs/TableSourceInventory.csv`
- `docs/RankListComparison.csv`
- `assets/manifest/RequiredAssetsAndSounds.csv`
- `assets/manifest/DecompiledXrefStringReferences.csv`
- `include/UnionCombineFusionSelectOriginalWindow.h`
- `src/UnionCombineFusionSelectOriginalWindow.cpp`

Os assets nao foram extraidos fisicamente do Pack01; o subprojeto registra os
caminhos oficiais usados pelo executavel para que a porta carregue os mesmos
recursos do cliente.

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

Para esta janela, qualquer implementacao real de fusao Union precisa mapear o
request, o result e o refresh da lista seguindo esse modelo. Log
`No processor for packet X` indica protocolo sem processor registrado no
backend em execucao.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
