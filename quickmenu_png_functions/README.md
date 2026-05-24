# QuickMenu PNG references from unpacked exe

Origem pesquisada: `unpacked_exe_all`.

Funcoes copiadas:

- `010bbf30_FUN_010bbf30.c`
  - Referencia `QuickMenu\QuickMenu_Sub_Icon.png`
- `013a8b10_FUN_013a8b10.c`
  - Referencia `QuickMenu\QuickMenu_Button_menu.png`
  - Referencia `QuickMenu\QuickMenu_Button_box.png`
  - Referencia `QuickMenu\QuickMenu_Button_community.png`
  - Referencia `QuickMenu\QuickMenu_Button_inventory.png`
  - Referencia `QuickMenu\QuickMenu_Button_data.png`
  - Referencia `QuickMenu\QuickMenu_Button_character.png`
  - Referencia `QuickMenu\QuickMenu_Button_event.png`

Observacao:

- `01283910_FUN_01283910.c` tambem contem `QuickMenuUI`, mas nao contem referencia a imagem `.png`; por isso nao foi copiada para este recorte.
- Os nomes do exe original usam `QuickMenu_Button_*` com iniciais maiusculas, diferente dos fallbacks minuscule usados temporariamente no cliente atual.

Protocolos de comunicacao
-------------------------

Este recorte e apenas de assets do QuickMenu, mas fica registrada a referencia
usada pelos subprojetos com backend:

- Union (`XmlUnionDUnit`) envia `11050` e `11051`; o client recebe `11050`,
  `11052` e `11053`.
- No server, Union registra `XmlUnionProgressRequest = 11050` e
  `XmlUnionRewardClaim = 11051`; as respostas sao `XmlUnionProgressPacket`
  (`11050`), `XmlUnionRewardClaimResultPacket` (`11052`) e
  `XmlUnionCollectionInfoPacket` (`11053`).
- Enciclopedia usa `pDigimon::DigimonBookInfo`, `pDigimon::DigimonBookItem`,
  `pDigimon::DigimonBookDeckUse` e `pDigimon::DigimonBookDeckEff`; no server os
  requests ativos sao `EncyclopediaLoad = 3234` e `EncyclopediaDeckUse = 3236`.

Log `No processor for packet X` significa que o backend em execucao nao tem
processor registrado para o protocolo enviado pelo client.
