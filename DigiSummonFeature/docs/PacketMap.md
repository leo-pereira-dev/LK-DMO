# Packet Map

Strings/pacotes encontrados no decompilado para `DigiSummon`.

## Cliente para Servidor

- `C2GS_REQ_DIGI_SUMMON_SYNC`
- `C2GS_REQ_DIGI_SUMMON_PURCHASE`

## Servidor para Cliente

- `GS2C_RES_DIGI_SUMMON_SYNC`
- `GS2C_RES_DIGI_SUMMON_PURCHASE`

## Observacao

O fluxo aparente e:

1. Cliente abre a UI e pede sincronizacao de produtos/contadores.
2. Servidor responde com estado de produtos e limites.
3. Cliente compra/invoca usando ticket/item.
4. Servidor responde com resultado e a UI abre a cena/resultado.
