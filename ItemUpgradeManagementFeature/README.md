# ItemUpgradeManagementFeature

Subprojeto isolado para mapear a janela oficial de **Gerenciamento** mostrada
na print, com as abas:

- `Fortalecimento`
- `Transferencia`
- `Recuperacao`

O nome funcional encontrado no dump fica na familia de upgrade de equipamento /
data upgrade:

- `EquipmentUpgradeSystem`
- `DataTradeDigimonUpgrade`
- `cEquipUpgradeSystemViewer`
- `CItemUpgradeTable`

O titulo e as abas aparecem por chaves oficiais:

- `UPGRADESYSTEM_TITLE`
- `EQUIPSYSTEM_UPGRADE`
- `UPGRADESYSTEM_TAB_UPGRADE_DATA_TRANSFER`
- `UPGRADESYSTEM_TAB_UPGRADE_DATA_RESTORE`

## Conteudo

- `decompiled/Launcher`: criacao/registro das janelas oficiais relacionadas.
- `decompiled/MainWindow`: frame, titulo, close, tabs e troca de paineis.
- `decompiled/StrengthenTab`: aba Fortalecimento.
- `decompiled/TransferTab`: aba Transferencia.
- `decompiled/RestoreTab`: aba Recuperacao.
- `decompiled/ValidationAndPackets`: validacoes, mensagens e handlers de
  resultado.
- `decompiled/Tables`: loaders/parsers das tabelas `DM_ItemUpgrade-*`.
- `assets/manifest`: assets, textos e strings de pacote/RTTI.
- `tables/xml` e `tables/bin`: tabelas oficiais copiadas.
- `language/bin`: bins de texto de item usados para nomes/tooltips.
- `include` e `src`: mapa C++ limpo, fora do projeto principal.

## Manifestos

- `docs/CopiedFunctionFiles.csv`: todas as funcoes copiadas com nome limpo.
- `assets/manifest/RequiredAssetsAndSounds.csv`: assets/sounds encontrados por
  xref direta nas funcoes copiadas.
- `assets/manifest/AdditionalKnownAssetStrings.csv`: assets oficiais visiveis
  na print, mas sem xref direto dentro do conjunto de funcoes copiadas.
- `assets/manifest/AdjacentOrExcludedAssetStrings.csv`: assets proximos no dump
  que pertencem a sistemas adjacentes, nao a esta janela.
- `assets/manifest/TextKeyReferences.csv`: chaves oficiais de texto usadas pela
  janela.
- `assets/manifest/KnownPacketAndRttiStrings.csv`: strings oficiais de pacotes
  e RTTI relacionadas.
- `assets/manifest/AdditionalKnownOfficialStrings.csv`: strings oficiais
  relacionadas, mas sem xref direto dentro do conjunto de funcoes copiadas.
- `assets/manifest/AllCopiedDataXrefs.csv`: xrefs brutas das funcoes copiadas.
- `assets/manifest/DecompiledUiStringReferences.csv`: strings visuais achadas
  diretamente nos `.c` copiados.

## Ultima varredura de assets

Registrada em `docs/LastSweepNotes.md` e
`docs/LastSweep_AssetXrefs.csv`.

Resultado: os 27 assets/sounds obrigatorios do manifesto possuem xref oficial.
Foram adicionadas duas strings oficiais visiveis na print, sem xref direto no
conjunto copiado:

- `EquipSystem\\ArrowBtn_Down.tga`
- `EquipSystem\\ArrowBtn_Up.tga`

## Observacao

As imagens continuam apenas mapeadas. Para renderizar 1:1, extraia os paths de
interface do Pack01 preservando `Data\\Interface`.

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

Para o sistema de gerenciamento, cada aba precisa ter request, validacao no
server e resposta de resultado/refresh. Log `No processor for packet X` indica
que o protocolo enviado pelo client nao possui processor registrado no backend
em execucao.

## Cuidados com texturas

Antes de implementar a UI no projeto principal, leia
`docs/TexturePackPitfalls.md`. Ele registra o problema de sprite rosa/magenta
causado por path sem `Data\\Interface` e uso incorreto de
`ChangeTexture(..., false)`.
