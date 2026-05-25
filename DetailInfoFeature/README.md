# DetailInfoFeature

Subprojeto isolado para mapear as janelas oficiais de **Detalhes** mostradas nas
prints:

- aba `Tamer`, com equipamentos, conquista equipada e atributos obtidos;
- aba `Digimon`, com informacao do Digimon, habilidades, encanto e avanco;
- botao superior de visibilidade `ON/OFF`;
- construtores relacionados de `TamerStatusUI`, `DigimonStatusUI` e
  `DigimonStatusUI_Help`.

O nome interno encontrado nos dumps oficiais e `DetailInfo`. As funcoes copiadas
vieram de `unpacked_exe_all/functions` e continuam com o corpo decompilado
original em arquivos `*.original.c`; os nomes dos arquivos foram saneados para
descrever a funcao aparente de cada bloco.

## Implementacao no cliente

O estado funcional portado para `dmo-client-main` esta documentado em
`../DetailInfo.md`.

Resumo do estado atual:

- a janela `Detalhes` ja existe como janela independente/movivel no client;
- a aba `Tamer` tem background, slots, render 3D, botoes e tabela de atributos;
- o retrato circular do Tamer foi corrigido para usar o mesmo `Render_L` da HUD
  do personagem, sem a moldura escura `DetailInfo\\TamerStatus_Digimon_Tamer`
  por cima;
- a parte de user/outro jogador ainda nao esta finalizada e deve ser retomada
  usando os pacotes oficiais mapeados neste subprojeto.

## Conteudo

- `decompiled/DetailInfoUI`: funcoes oficiais da janela `DetailInfo`.
- `decompiled/DetailInfoMainUI`: shell oficial da janela, abas
  `Tamer`/`Digimon`, estado local/outro jogador e eventos dos botoes.
- `decompiled/DetailInfoAdditionalStat`: popup auxiliar de atributos
  adicionais.
- `decompiled/StatusUI`: construtores oficiais das UIs de status relacionadas.
- `decompiled/Shared`: funcao compartilhada que usa asset de `DetailInfo`.
- `assets/manifest`: CSVs gerados a partir dos `.c` e `xrefs.csv`.
- `tables/xml` e `tables/bin`: tabelas oficiais copiadas para alimentar status,
  encanto, selo mestre, atributos e breakthrough.
- `language/bin`: bins de texto necessarios para nomes de item, digimon, tamer,
  conquista e selo.
- `include` e `src`: mapa C++ limpo, fora do projeto principal.

## Manifestos principais

- `assets/manifest/Pack01RequiredAssets.csv`: sprites/texturas referenciadas
  pelas funcoes decompiladas. A ultima varredura mapeou 42 referencias de
  asset. As imagens de interface ficam no Pack01 / `Data\\Interface`.
- `assets/manifest/DecompiledXrefStringReferences.csv`: referencias por xref,
  com funcao original e endereco.
- `assets/manifest/DecompiledUiStringReferences.csv`: referencias encontradas
  diretamente nos `.c` copiados.
- `assets/manifest/ClassAndPacketStringReferences.csv`: RTTI/classes e pacotes
  relacionados a `DetailInfo`.

## Observacao

As texturas foram mapeadas por referencia oficial, mas nao foram extraidas do
pack aqui. Para renderizar a janela 1:1, a proxima etapa e extrair esses paths do
Pack01 preservando a arvore `Data\\Interface`.

## Ultima varredura

`docs/LastSweep_DetailInfoXrefs.csv` registra todas as referencias amplas
encontradas por xref. A primeira varredura encontrou seis dependencias
adicionais do painel de Selo Mestre, que foram copiadas para
`decompiled/DetailInfoUI`:

- `BuildSealMasterCloseButton.original.c`
- `BuildSealMasterStatRows.original.c`
- `ResolveSealMasterStatText.original.c`
- `ResolveSealMasterDsTextCase.original.c`
- `ResolveSealMasterStatValue.original.c`
- `BindSealMasterCloseButton.original.c`

A varredura profunda seguinte percorreu as chamadas proximas ao bloco
`011d****` e gerou:

- `docs/DeepSweep_DetailInfoCandidateFunctions.csv`: 125 funcoes candidatas,
  classificadas por confianca.
- `docs/DeepSweep_DetailInfoCallEdges.csv`: 85 arestas de chamada entre
  funcoes da familia `DetailInfo`.
- `docs/DeepSweepCopiedFunctionFiles.csv`: 39 arquivos oficiais adicionais
  copiados para este subprojeto.
- `docs/FunctionChainMap.md`: cadeia organizada por janela principal, aba
  `Tamer`, aba `Digimon`, popup de equipamento, popup de atributo adicional e
  Selo Mestre.

Depois dessa varredura, o subprojeto possui 70 funcoes mapeadas entre codigo
de status, janela principal, abas, popups e compartilhados.

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

A janela de detalhe consome muito estado ja carregado do tamer/digimon, mas as
subjanelas novas devem seguir o mesmo contrato: protocolos de ida e resposta
documentados no client e no server. Log `No processor for packet X` indica
handler ausente no backend em execucao.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
