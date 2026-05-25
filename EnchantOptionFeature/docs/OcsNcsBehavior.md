# OCS / NCS Behavior Notes

## Tipos de pedra

Os tipos oficiais aparecem em `common_vs2019/pItem.h`:

| Valor | Nome | Uso provavel |
| --- | --- | --- |
| `1` | `DigiPower` | altera power/rate do acessorio |
| `2` | `Renewal` | altera/preserva quantidade de renewal |
| `3` | `AccOption` | reroll aleatorio das opcoes, fluxo OCS |
| `4` | `OptionValue` | reroll/ajuste do valor de uma opcao escolhida, fluxo NCS |

O tipo da pedra vem de:

```text
CsAccessory_Enchant* pEnchant = GetAccessoryEnchant(pFTStone->s_dwSkill)
pEnchant->s_nOpt
```

## Validacoes oficiais observadas

`cEnchantOptionContents::EnchantItem` separa a validacao por tipo:

- `DigiPower`: valida limite maximo de power, com mensagem `30604` ou `30602`.
- `Renewal`: valida `m_nLevel + pEnchantInfo->s_nVal <= pOptionInfo->s_nLimit`,
  com mensagem `30613` ou `30611`.
- `AccOption`: requer acessorio com `m_nLevel > 0`, com mensagem `30607` ou
  `30605`.
- `OptionValue`: requer acessorio com `m_nLevel > 0`, com mensagem `30610` ou
  `30608`.

O item alvo tambem precisa:

- ser Ring, Necklace, Earring, Bracelet ou Digivice quando o define de Digivice
  esta habilitado;
- possuir `s_nSkillCodeType == nsCsItemTable::OPTIONCODE`;
- estar identificado (`m_nRate != 0`) para encantamento;
- nao ser item tutorial bloqueado pela validacao local.

## Fluxo OCS

OCS corresponde ao tipo `nItem::AccOption`.

Comportamento client:

- nao abre combobox de opcao;
- envia `AccStoneUse(uid, stoneIndex, itemIndex)`;
- espera resposta com todos os arrays `nAccOption` e `nAccValues`;
- compara opcoes antigas e novas para imprimir mensagem de resultado.

Comportamento esperado no server:

- consumir uma pedra `AccOption`;
- rerollar os tipos das opcoes usando a tabela `AccessoryOption`;
- manter a quantidade de slots conforme `StatusAmount`;
- gerar valores dentro de `MinValue..MaxValue` de cada opcao escolhida;
- devolver o item atualizado pela estrutura `GS2C_RECV_ENCHANT_ITEM_ENCHANT`.

## Fluxo NCS

NCS corresponde ao tipo `nItem::OptionValue`.

Comportamento client:

- abre combobox com as opcoes existentes do acessorio;
- `SelectOptionValue` guarda o indice selecionado;
- envia `AccStoneUse(uid, stoneIndex, itemIndex, selectedIndex)`;
- espera resposta com todos os arrays atualizados;
- imprime mensagem usando a opcao selecionada.

Comportamento esperado no server:

- validar o indice `selectedIndex` dentro de `MAX_ACCESSORY_OPTION`;
- validar se a opcao selecionada existe;
- consumir uma pedra `OptionValue`;
- rerollar apenas o valor daquela opcao, preservando os demais tipos;
- devolver todos os arrays para manter o client em sincronia.

## Mensagens

As mensagens vistas no client ficam na faixa `30601..30615`, com destaque:

- `30601`: identificacao concluida.
- `30603`, `30606`, `30609`, `30612`, `30615`: mensagens de sucesso/resultado
  por tipo de encantamento.
- `30604`, `30607`, `30610`, `30613`: bloqueios/limites.

