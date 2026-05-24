# Table Schema

Mapa inicial dos XML/BIN usados pela tela.

## DM_DigiSummon-List

Arquivo de produtos/banners da tela de invocacao.

- XML copiado: `xml\DM_DigiSummon-List.xml`
- BIN copiado: `bin\DM_DigiSummon-List.bin`
- Registros encontrados: `21`

Campos observados:

- `SummonKey`: chave do produto/banner.
- `IconPath`: sprite do tipo/rank do produto, exemplos:
  `Legend_slot.png`, `Epic_slot.png`, `Unique_slot.png`, `Rare_slot.png`.
- `Entries`: pares/triplas relacionados ao produto, provavelmente tickets,
  grupos ou custos.

## DM_DigiSummon-ItemList

Lista de itens/tickets aceitos.

- XML copiado: `xml\DM_DigiSummon-ItemList.xml`
- BIN copiado: `bin\DM_DigiSummon-ItemList.bin`
- Registros encontrados: `393`

Campos observados:

- `ItemId`: item/ticket.
- `FlagOrType`: tipo/rank/categoria ainda a confirmar.
- `Value`: valor associado, provavelmente chave de produto/grupo.

## DM_DigiSummon-ItemGroup

Pools de recompensa.

- XML copiado: `xml\DM_DigiSummon-ItemGroup.xml`
- BIN copiado: `bin\DM_DigiSummon-ItemGroup.bin`
- Registros encontrados: `27`

Esta tabela e grande e deve ser usada para a lista de recompensas aleatorias e
garantidas exibida no popup de probabilidade.
