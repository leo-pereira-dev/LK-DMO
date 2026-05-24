# DigiCombineFeature

Feature isolada para reconstruir o sistema de fusao de itens de evolucao.

O cliente decompilado mostra duas familias quase identicas:

- `DigiCombine`: usa `DM_DigiCombine-*` e a UI `DigiCombineUI`.
- `UnionCombine`: usa `DM_UnionCombine-*` e a UI `UnionCombineUI`.

As duas seguem o mesmo formato de tabela:

- `ItemList`: itens aceitos como material e o rank/tier de cada item.
- `RankList`: regra por rank, incluindo teto/ceil para tentativas acumuladas.
- `ItemGroup`: pool normal de recompensa por rank.
- `CeilGroup`: pool de recompensa usado pelo teto/ceil.

## Estrutura

- `include/`: headers limpos para usar como base fora do projeto principal.
- `src/`: implementacao standalone de parser XML e logica de roll.
- `xml/`: copias dos XMLs extraidos de `Dcommerce/xml/AllBins/Data`.
- `bin/`: copias dos BINs oficiais extraidos de `Dcommerce/Bin/Table`.
- `decompiled/`: funcoes `.c` copiadas de `unpacked_exe_all/functions`, com nomes de arquivo por papel.
- `docs/`: mapas e notas da engenharia reversa.
- `assets/manifest/`: inventario dos paths oficiais usados pela janela.

## Janela oficial

A janela original nao e montada pelos arquivos `DigiCombineTable.*` e
`DigiCombineService.*`; esses arquivos tratam apenas dados e regra.

O mapa da interface oficial foi adicionado em
`docs/OriginalWindowMap.md`, e as funcoes de UI decompiladas foram copiadas para:

- `decompiled/DigiCombineUI`
- `decompiled/UnionCombineUI`

O header `include/DigiCombineOriginalWindow.h` concentra os tamanhos de classes,
offsets internos, retangulos de botoes e paths de assets oficiais usados pela
janela.

Os assets visuais oficiais da janela estao no `ClientDist\Data\Pack01`
(`Pack01.hf` / `Pack01.pf`). O port para o projeto principal deve carregar os
paths oficiais pelo asset loader do cliente, sem duplicar texturas no source
control a menos que a gente queira overrides soltos para teste.

O inventario amplo esta em `assets/manifest/DigiCombineAssets.csv`.
O inventario fiel aos `.c` decompilados copiados esta em
`assets/manifest/DecompiledUiAssetReferences.csv`; use esse como fonte de
verdade para portar a janela original.

## Regra principal

O fluxo de gameplay esperado e:

1. O jogador escolhe 4 materiais.
2. O servidor valida se os 4 existem e pertencem ao mesmo rank.
3. O rank seleciona o grupo normal de recompensa.
4. Se o rank tiver regra de ceiling e o contador de falhas alcancar o limite, usa o `CeilGroup`.
5. O resultado e sorteado por `chanceBasisPoints`.
6. A recompensa pode manter o rank ou subir para um rank/tier maior.

## Arquivos de origem encontrados

Strings e RTTI no decompilado confirmam:

- `C2GS_REQ_DIGI_COMBINE_SYNC`
- `C2GS_REQ_DIGI_COMBINE`
- `C2GS_REQ_DIGI_COMBINE_REWARD`
- `GS2C_RES_DIGI_COMBINE_SYNC`
- `GS2C_RES_DIGI_COMBINE`
- `GS2C_RES_DIGI_COMBINE_REWARD`
- `C2GS_REQ_UNION_COMBINE_SYNC`
- `C2GS_REQ_UNION_COMBINE`
- `C2GS_REQ_UNION_COMBINE_REWARD`
- `GS2C_RES_UNION_COMBINE`
- `GS2C_RES_UNION_COMBINE_REWARD`

No projeto principal ativo, `CsUnionTableMng` ja conhece os bins
`DM_UnionCombine-*`, mas nao ha implementacao ativa completa da UI/packet flow.

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

Os pacotes oficiais listados acima para `DigiCombine`/`UnionCombine` ainda
precisam ser ligados no projeto principal com enum do server, processor,
resposta e handler no switch de receive do client. Log `No processor for packet
X` significa que o backend que esta rodando nao conhece aquele protocolo.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
