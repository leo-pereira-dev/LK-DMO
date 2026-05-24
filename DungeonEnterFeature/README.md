# DungeonEnterFeature

Subprojeto isolado para mapear a janela oficial de entrada de dungeon antes de portar para o projeto principal.

## Origem

- Funcoes UI rastreadas em `C:\0-NewDmo\unpacked_exe_all\functions`.
- Strings, assets e RTTI rastreados em `unpacked_GDMO.exe_strings.csv`, `unpacked_GDMO.exe_symbols.csv` e `unpacked_GDMO.exe_xrefs.csv`.
- XML/BIN copiados de `C:\0-NewDmo\Dcommerce`.
- Texturas da interface pertencem ao Pack 01 (`ClientDist\Data\Pack01.*`), mas os arquivos fisicos ainda nao estao extraidos nesta pasta.

## Conteudo

- `xml/`: XML oficiais de dungeon decompilados.
- `bin/`: BIN oficiais de tabela e idioma usados pela janela.
- `decompiled/DungeonEnterUI/`: funcoes `.c` originais copiadas e renomeadas por papel.
- `decompiled/DungeonTables/`: loaders de tabelas de dungeon.
- `assets/manifest/DecompiledUiAssetReferences.csv`: assets e keys fixos vindos das funcoes `.c`.
- `assets/manifest/DynamicXmlAssetReferences.csv`: imagens dinamicas vindas de `DungeonList.xml`.
- `include/` e `src/`: nomes legiveis, constantes e classes/estruturas auxiliares para o port futuro.
- `docs/`: mapa da janela, tabelas, pacotes e funcoes.

## Status

O mapeamento estrutural esta pronto para a janela da screenshot. A amostra que melhor bate com a tela "Treinamento Gankoomon" e `DungeonId=5001`: usa `DungeonUI\DungeonImage\Dungeon_50.png`, requisito de Digimon nivel 150 e 9 itens obtiveis. O texto final exibido na tela vem de bins de idioma/localizacao, nao de XML solto.

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

Para portar entrada de dungeon, mapear explicitamente request de lista/detalhe,
request de entrada e resposta de erro/sucesso. Log `No processor for packet X`
significa que o backend em execucao nao registrou o handler daquele protocolo.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
