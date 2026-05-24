# Original Window Map

Mapa inicial da tela oficial de invocacao de dados.

## Classes RTTI Encontradas

- `CDigiSummonMainUI`
- `CDigiSummonContents`
- `CDigiSummonBaseUI`
- `CDigiSummonBottomUI`
- `CDigiSummonTicketSelectUI`
- `CDigiSummonRateInfoUI`
- `CDigiSummonResultUI`
- `CDigiSummonSceneUI`
- `CDigiSummonSelectUI`

## Tamanho das Classes

Extraido de `011b6020` e `0128f070`.

- Main UI: `0xD8`
- Bottom UI: `0x94`
- Select UI: `0x1E8`
- RateInfo UI: `0x80`
- Result UI: `0x11C`
- Scene UI: `0xA8`
- TicketSelect UI: `0xAC`

## Ponteiros do Main UI

```text
0xA8 -> contents
0xAC -> bottom UI
0xB0 -> active content
0xB4 -> select UI
0xB8 -> rate info UI
0xBC -> result UI
0xC0 -> scene UI
0xC4 -> ticket select UI
```

## Funcoes Principais

| Funcao | Papel |
| --- | --- |
| `0128f070` | Cria `DigiSummonUI`, registra o nome `DigiSummonUI` e chama create/open. |
| `011b6020` | Cria a composicao principal da janela e os filhos. |
| `011bb4b0` | Monta a tela de selecao de produtos/cards com setas laterais. |
| `011bbb20` | Monta card/produto da invocacao. |
| `011bc4e0` | Monta botoes inferiores de comprar/informacao de probabilidade. |
| `011b71a0` | Monta popup/lista de informacao de probabilidade. |
| `011b8e70` | Cena/gacha com card back, start, change, bg/end textures. |
| `011bab00` | Seleciona textura final por rank. |
| `011bac80` | Seleciona background da cena por rank. |
| `01636b20` | Carrega `DM_DigiSummon-List`, `ItemList` e `ItemGroup`. |

## Assets Confirmados

O manifesto fiel aos decompilados esta em:

```text
assets\manifest\DecompiledUiAssetReferences.csv
```

Ele contem `45` paths unicos de assets encontrados diretamente nas funcoes
copiadas da UI.

As imagens dos cards/produtos exibidos no carrossel sao dinamicas. Elas nao
aparecem como paths fixos nas funcoes `.c`; o decompilado renderiza o card com
dados do produto, e o path vem do XML `DM_DigiSummon-List.xml` no campo
`IconPath`. Esses paths ficam em:

```text
assets\manifest\DynamicXmlAssetReferences.csv
```
