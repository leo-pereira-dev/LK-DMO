# Last Sweep Notes

Data: 2026-05-24

## Escopo

Varredura focada no sistema da janela `Encantamento` da print, associada a
OCS/NCS e melhoria de acessorio/equipamento.

## Fontes usadas

- CodeGraph sobre o client ativo.
- Busca textual focada por `EnchantOption`, `AccessoryCheck`,
  `AccStoneUse`, `AccOption`, `OptionValue`, `DigiPower`, `Renewal` e
  `AccessoryRoll`.
- Decompilado ja copiado em `DigimonAdvanceFeature`.
- Client ativo em `dmo-client-main`.
- Server PostgreSQL em `dmo-server-main-postgres`.

## Resultado

- 55 funcoes decompiladas copiadas para o novo subprojeto:
  - 13 em `EnchantOptionUI`;
  - 16 em `EnchantOptionResultUI`;
  - 22 em `DigiPowerEnchantUI`;
  - 4 em `AdditionalXrefs`.
- 1 tabela binaria copiada: `AccEnchant.bin`.
- 1 XML copiado: `AccEnchant.xml`.
- 1 audio adjacente copiado: `Enchant_Room.mp3`.
- Assets, textos, packets e arquivos ativos foram registrados em manifestos.

## O que nao foi encontrado pronto

- Processor server ativo para `AccessoryCheck`.
- Processor server ativo para `AccStoneUse`.
- Arquivo split `AccOption.bin` fora do `ItemList.bin`; a string oficial existe
  no decompilado em `015e9390`, mas o bin split nao esta presente solto aqui.

## Proxima etapa pratica

Concluida: os processors compativeis `ItemIdentify` e `ItemReroll` agora usam
`AccessoryEnchantService` como nucleo funcional para identify, OCS, NCS,
`DigiPower` e `Renewal`.

Proxima etapa pratica: testar in-game a janela oficial com stones reais para
calibrar se os valores de `DigiPower` devem aceitar perda/preservacao aleatoria
em alguns stones ou se devem seguir estritamente o `Value` de `AccEnchant`.
