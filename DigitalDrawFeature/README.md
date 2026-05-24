# DigitalDrawFeature

Subprojeto isolado para a tela de **Digital Draw** mostrada no screenshot:

```text
Selecione o ticket que deseja utilizar.
```

Visualmente ela reaproveita a familia `DigiSummonUI` / `Random_box`, mas a
varredura encontrou uma nuance importante: o EXE tambem possui uma tabela
oficial propria para o sistema:

```text
DM_Luckydraw_Info-LuckyDrawInfo.bin
CLuckyDrawTable
```

Por isso este subprojeto nao substitui `DigiSummonFeature`; ele separa o
Digital Draw como uma variante que usa a mesma base visual, mas deve ser
portada com a trilha `LuckyDraw` documentada.

## Conteudo

- `decompiled/DigitalDrawUI`: copia da familia UI usada pela tela
  `DigiSummonUI`, renomeada no subprojeto para o fluxo Digital Draw. Inclui a
  subjanela `info. de prob.` e seus renderizadores internos de lista/linha.
- `decompiled/LuckyDrawTable`: funcoes exclusivas da tabela
  `CLuckyDrawTable`.
- `tables/xml` e `tables/bin`: tabelas de suporte que existem no dump local.
- `docs/MissingOfficialTableFiles.csv`: registra a tabela oficial de LuckyDraw
  referenciada pelo EXE, mas ausente da extracao `Dcommerce` atual.
- `assets/manifest`: assets fixos rastreados por xrefs dos `.c` copiados.
- `include` e `src`: mapa limpo com nomes de classes, funcoes, offsets e assets.

## Nota de fidelidade

A tela do screenshot deve continuar sendo conferida contra os arquivos
decompilados. O mapeamento fiel encontrado ate agora e:

- UI runtime reutilizado: `DigiSummonUI`
- Classes visuais reutilizadas: `CDigiSummonMainUI`,
  `CDigiSummonTicketSelectUI`, `CDigiSummonSelectUI`,
  `CDigiSummonRateInfoUI`, `CDigiSummonResultUI`, `CDigiSummonSceneUI`
- Tabela exclusiva encontrada: `DM_Luckydraw_Info-LuckyDrawInfo.bin`
- Pack de interface: `ClientDist\Data\Pack01.*`

Os cards do screenshot provavelmente dependem do `LuckyDrawInfo` oficial. Esse
arquivo ainda nao esta disponivel no dump XML/BIN local, entao ficou marcado
como dependencia ausente em vez de ser inventado.

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

Para o Digital Draw, o port deve documentar os pacotes de selecao de ticket,
consulta de chance, execucao do sorteio e resultado. Se o server logar
`No processor for packet X`, o problema esta no enum/processor do backend em
execucao, nao no asset da janela.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
