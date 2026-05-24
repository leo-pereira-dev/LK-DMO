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

## Conteudo

- `decompiled/DetailInfoUI`: funcoes oficiais da janela `DetailInfo`.
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
  pelas funcoes decompiladas. A ultima varredura mapeou 36 referencias de
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
encontradas por xref. A varredura encontrou seis dependencias adicionais do
painel de Selo Mestre, que foram copiadas para `decompiled/DetailInfoUI`:

- `BuildSealMasterCloseButton.original.c`
- `BuildSealMasterStatRows.original.c`
- `ResolveSealMasterStatText.original.c`
- `ResolveSealMasterDsTextCase.original.c`
- `ResolveSealMasterStatValue.original.c`
- `BindSealMasterCloseButton.original.c`

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
