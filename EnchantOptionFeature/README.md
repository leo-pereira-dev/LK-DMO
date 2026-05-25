# EnchantOptionFeature

Subprojeto isolado para mapear a janela oficial de **Encantamento** de
acessorios/equipamentos mostrada na print, incluindo:

- identificacao de acessorio sem opcoes reveladas;
- troca aleatoria de opcoes de acessorio, normalmente associada a OCS;
- troca/ajuste de valor de uma opcao escolhida, normalmente associada a NCS;
- `DigiPower`, `Renewal`, `AccOption` e `OptionValue`;
- suporte condicional a `Digivice` por `LJW_ENCHANT_OPTION_DIGIVICE_190904`.

O nome interno encontrado no client ativo e nos dumps e `EnchantOption`. O
conteudo principal fica em:

- `cEnchantOption`
- `cEnchantOptionContents`
- `AdaptEnchantOption`
- `WT_ENCHANT_OPTION`
- `E_CT_ENCHANT_OPTION_CONTENTS`

## Conteudo

- `decompiled/EnchantOptionUI`: criacao da janela principal, Identify UI,
  Enchant UI, botao de encantamento, combobox da opcao NCS e helpers visuais.
- `decompiled/EnchantOptionResultUI`: janela/fluxo de resultado do
  encantamento e handler do resultado do reset.
- `decompiled/DigiPowerEnchantUI`: janela vizinha do cluster oficial de
  encantamento/poder, mantida porque compartilha assets, fluxo e factories.
- `decompiled/AdditionalXrefs`: referencias extras encontradas na varredura
  direta do `unpacked_exe_all`, incluindo textos `ACCESSORY_*` e loader de
  `AccOption.bin`.
- `assets/manifest`: assets, textos, eventos, packets e fontes ativas.
- `tables/bin` e `tables/xml`: dependencias copiadas ja conhecidas; `AccEnchant`
  foi copiada daqui. A varredura direta confirmou string oficial
  `AccOption.bin` em `015e9390`, mas esse arquivo split nao esta solto nesta
  workspace; no server atual a secao tambem pode vir do `ItemList.bin`.
- `language/bin`: reservado para bins de texto se a proxima etapa extrair as
  strings oficiais do pack.
- `include` e `src`: reservado para um mapa C++ limpo fora do projeto
  principal.

## Pontos principais

- O client abre a janela dinamica `WT_ENCHANT_OPTION`.
- O conteudo e registrado por `ContentsSystemFactory` como
  `E_CT_ENCHANT_OPTION_CONTENTS`.
- O botao de confirmacao dispara `EVENT_CODE::START_ENCHANT_OPTION_ENCHANT`.
- Identificacao envia `pItem::AccessoryCheck`.
- Encantamento envia `pItem::AccStoneUse`.
- Resposta de identificacao chega por `RecvEnchantItemIdentify` e levanta
  `EVENT_CODE::RECV_ENCHANT_OPTION_IDENTIFY_RESULT`.
- Resposta de encantamento chega por `RecvEnchantItemEnchant` e levanta
  `EVENT_CODE::RECV_ENCHANT_OPTION_ENCHANT_RESULT`.

## Manifestos principais

- `docs/CopiedFunctionFiles.csv`: todos os `.original.c` copiados para este
  subprojeto.
- `docs/FunctionMap.md`: mapa por janela, fluxo e responsabilidade.
- `docs/PacketAndEventFlow.md`: layout dos pacotes client/server e eventos.
- `docs/TableAndServerMap.md`: tabelas, registros do `ItemList.bin` e pontos
  ja existentes no server PostgreSQL.
- `docs/OcsNcsBehavior.md`: comportamento esperado para OCS/NCS, validacoes e
  mensagens.
- `docs/ServerImplementation.md`: implantacao funcional no GameServer.
- `assets/manifest/RequiredAssetsAndSounds.csv`: assets/sounds encontrados no
  client ativo e no cluster copiado.
- `assets/manifest/TextKeyReferences.csv`: chaves oficiais de texto usadas pela
  janela.
- `assets/manifest/PacketAndEventReferences.csv`: referencias de protocolo e
  eventos.
- `assets/manifest/ActiveClientSourceReferences.csv`: arquivos do client ativo
  que implementam ou chamam o sistema.

## Observacao

As texturas seguem apenas mapeadas. Para renderizar 1:1, extrair os paths do
Pack01 preservando `Data\\Interface`. O audio `Enchant_Room.mp3` foi copiado
porque ja existia como asset adjacente em `DigimonAdvanceFeature`.

## Implantacao server-side

O sistema foi plugado no GameServer mantendo os packets compativeis:

- `ItemIdentify = 3968` como `AccessoryCheck`;
- `ItemReroll = 3969` como `AccStoneUse`.

A logica antiga baseada diretamente nos processors foi substituida por
`AccessoryEnchantService`, que usa `AccEnchant`/`AccessoryOption` e mantem
fallback para os IDs antigos de stone.
