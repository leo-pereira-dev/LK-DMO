# DigiSummonFeature

Subprojeto isolado para mapear a tela oficial de invocacao de dados
(`DigiSummonUI`) antes de portar qualquer coisa para o cliente principal.

Pelo decompilado, essa tela usa a familia interna `DigiSummon`, apesar de estar
ligada visualmente ao Union/data summon no jogo.

## Estrutura

- `xml/`: XMLs oficiais de `Dcommerce/xml/AllBins/Data`.
- `bin/`: BINs oficiais de `Dcommerce/Bin/Table`.
- `decompiled/DigiSummonUI/`: funcoes `.c` da interface.
- `decompiled/DigiSummonTable/`: funcoes `.c` de carregamento das tabelas.
- `assets/manifest/`: assets rastreados a partir das funcoes decompiladas.
- `include/`: headers limpos com o mapa inicial de classes/offsets.
- `docs/`: notas de engenharia reversa.

## Tabelas Oficiais

- `DM_DigiSummon-List`: produtos/banner da invocacao.
- `DM_DigiSummon-ItemList`: itens/tickets aceitos pela invocacao.
- `DM_DigiSummon-ItemGroup`: pools de recompensa.

## Fonte de Verdade de Assets

Use `assets/manifest/DecompiledUiAssetReferences.csv` para portar a tela. Esse
arquivo contem somente assets que aparecem diretamente nos `.c` decompilados
copiados para esta feature.

As imagens dinamicas dos cards/produtos nao aparecem hardcoded nos `.c`; elas
vem de `DM_DigiSummon-List.xml` no campo `IconPath` e estao listadas em
`assets/manifest/DynamicXmlAssetReferences.csv`.

Os assets visuais estao no `ClientDist\Data\Pack01` e devem ser carregados por
path pelo loader do cliente.

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

Quando esta tela for portada para gameplay real, seguir esse padrao: definir
protocolo de ida, processor no server e pacote de retorno consumido pelo switch
de receive do client. Log `No processor for packet X` significa que o backend em
execucao nao registrou handler para aquele protocolo.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
