# Packet And Event Flow

## Eventos internos do client

| Evento | Origem | Consumidor |
| --- | --- | --- |
| `EVENT_CODE::RECV_ENCHANT_OPTION_IDENTIFY_RESULT` | `cCliGame::RecvEnchantItemIdentify` | `cEnchantOptionContents::RecvIdentifyItem` |
| `EVENT_CODE::RECV_ENCHANT_OPTION_ENCHANT_RESULT` | `cCliGame::RecvEnchantItemEnchant` | `cEnchantOptionContents::RecvEnchantItem` |
| `EVENT_CODE::START_ENCHANT_OPTION_ENCHANT` | `MessageBox.cpp` / confirmacao do usuario | `cEnchantOptionContents::StartEnchantItem` |
| `CONTENTS_EVENT::EStreamEvt_Inventory_UseItem` | uso de item no inventario | `cEnchantOptionContents::SetItem` |

## Client -> Server

### AccessoryCheck

Arquivo ativo:

```text
dmo-client-main/DProject/network/cCliGameSend.cpp
```

Layout:

```text
newp(pItem::AccessoryCheck)
push(u4 nUID)
push(u2 nInvenPos)
endp(pItem::AccessoryCheck)
```

Uso: identificar um acessorio ainda sem opcoes reveladas.

### AccStoneUse

Layout:

```text
newp(pItem::AccStoneUse)
push(u4 nUID)
push(u2 nItemPos)   // slot da pedra
push(u2 nAccPos)    // slot do acessorio
push(u1 nCombIdx)   // opcional, apenas OptionValue/NCS
endp(pItem::AccStoneUse)
```

Uso:

- `DigiPower`, `AccOption` e `Renewal`: envia sem `nCombIdx`.
- `OptionValue`: envia com `nCombIdx`, indice da opcao selecionada na combobox.

## Server -> Client

### Identify result

Estrutura local no client:

```cpp
struct GS2C_RECV_ENCHANT_ITEM_IDENTIFY
{
    u2 nInvenPos;
    u1 nRate;
    u1 nLevel;
    u2 nAccOption[MAX_ACCESSORY_OPTION];
    u2 nAccValues[MAX_ACCESSORY_OPTION];
};
```

Campos:

- `nRate`: DigiPower do acessorio, comentado no client como faixa `95~200%`.
- `nLevel`: contagem/level de renewal.
- `nAccOption`: tipos das opcoes.
- `nAccValues`: valores das opcoes.

### Enchant result

Estrutura local no client:

```cpp
struct GS2C_RECV_ENCHANT_ITEM_ENCHANT
{
    u1 nResult; // 1 sucesso, 2 falha, 3 preservado
    u2 nInvenPos;
    u1 nRate;
    u1 nLevel;
    u2 nAccOption[MAX_ACCESSORY_OPTION];
    u2 nAccValues[MAX_ACCESSORY_OPTION];
};
```

O client atualiza o item local e imprime mensagens por tipo de pedra.

## Pontos do client que registram os packets

- `pItem::AccessoryCheck`: enum em `common_vs2019/pItem.h`.
- `pItem::AccStoneUse`: enum em `common_vs2019/pItem.h`.
- `cCliGame.cpp`: `case pItem::AccessoryCheck: RecvEnchantItemIdentify();`.
- `cCliGameReceive.cpp`: `RecvEnchantItemIdentify` e
  `RecvEnchantItemEnchant`.

## Lacuna atual no server

A busca no server PostgreSQL encontrou assets/tabelas de accessory roll, mas nao
encontrou packet processors ativos para `AccessoryCheck` e `AccStoneUse`. Antes
de implementar a UI completa, o backend precisa registrar handlers equivalentes
ao layout acima e devolver as duas estruturas esperadas pelo client.

