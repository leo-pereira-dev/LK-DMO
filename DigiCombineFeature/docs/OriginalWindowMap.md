# Original Window Map

Esta parte e o mapa da janela oficial, separado da regra/tabela.

## Classes RTTI encontradas

- `CDigiCombineMainUI`
- `CUnionCombineMainUI`
- `CDigiCombineContents`
- `CUnionCombineContents`
- `CDigiCombineBaseUI`
- `CUnionCombineBaseUI`
- `CDigiCombineRegisterUI`
- `CUnionCombineRegisterUI`
- `CDigiCombineRateInfoUI`
- `CUnionCombineRateInfoUI`
- `CDigiCombineResultUI`
- `CUnionCombineResultUI`
- `CDigiCombineSceneUI`
- `CUnionCombineSceneUI`

## Tamanho das classes no decompilado

- Main UI: `0x144`
- Register UI: `0x140`
- RateInfo UI: `0x80`
- Result UI: `0x114`
- Scene UI: `0x88`

## Ponteiros do Main UI

```text
0xAC -> active content
0xB0 -> register UI
0xB4 -> rate info UI
0xB8 -> result UI
0xBC -> scene UI
```

## Botoes do Register UI

Valores extraidos de `011c39f0` para DigiCombine e `011fdd70` para
UnionCombine.

```text
Add/Cancel    x=0x280 y=0x2C0 w=0x8C h=0x28
Reward        x=0x366 y=0x247 w=0x55 h=0x1E
Rate Info     x=0x0C2 y=0x2BC w=0x78 h=0x32
Auto Register x=0x280 y=0x26C w=0x8C h=0x28
Combine       x=0x32A y=0x26C w=0x8C h=0x28
Exit          x=0x32A y=0x2BC w=0x96 h=0x32
```

## Funcoes copiadas

Os arquivos originais copiados ficam em:

- `decompiled/DigiCombineUI`
- `decompiled/UnionCombineUI`

As funcoes mais importantes:

| Funcao | Papel |
| --- | --- |
| `0128f1b0` | Cria `DigiCombineUI`, instancia main UI e chama create/open. |
| `01290160` | Cria `UnionCombineUI`, instancia main UI e chama create/open. |
| `011befd0` | Cria a composicao da janela `CDigiCombineMainUI`. |
| `011f9d20` | Cria a composicao da janela `CUnionCombineMainUI`. |
| `011c39f0` | Cria botoes do register UI DigiCombine. |
| `011fdd70` | Cria botoes do register UI UnionCombine. |
| `011b8e70`, `011c7b10`, `012012e0` | Cena/gacha com card back, start, change, bg/end textures. |
| `011bab00`, `011c91c0`, `01202700` | Seleciona textura final por rank. |
| `011bac80`, `011c9340`, `01202880` | Seleciona background da cena por rank. |

## Assets oficiais referenciados

Os assets de interface desta janela ficam no `ClientDist\Data\Pack01`
(`Pack01.hf` / `Pack01.pf`). O EXE referencia os paths abaixo e o client deve
resolver via pack/`CsFPSystem`.

```text
DigiCombine\DigiCombine_CardFront.dds
DigiCombine\DigiCombine_CardBack.dds
DigiCombine\DigiCombine_Slot_Empty.dds
DigiCombine\DigiCombine_Slot_rank.dds
DigiCombine\DigiCombine_Gacha_Start.dds
DigiCombine\DigiCombine_Gacha_Change.dds
DigiCombine\DigiCombine_Gacha_End_A.dds
DigiCombine\DigiCombine_Gacha_End_Aplus.dds
DigiCombine\DigiCombine_Gacha_End_S.dds
DigiCombine\DigiCombine_Gacha_End_Splus.dds
DigiCombine\DigiCombine_Gacha_End_SS.dds
DigiCombine\DigiCombine_Gacha_End_SSplus.dds
DigiCombine\DigiCombine_Gacha_End_SSS.dds
DigiCombine\DigiCombine_Gacha_End_SSSplus.dds
DigiCombine\DigiCombine_Gacha_End_U.dds
DigiCombine\DigiCombine_Gacha_End_Uplus.dds
DigiCombine\DigiCombine_Gacha_End_N.dds
DigiCombine\DigiCombine_Gacha_Bg_A.dds
DigiCombine\DigiCombine_Gacha_Bg_Aplus.dds
DigiCombine\DigiCombine_Gacha_Bg_S.dds
DigiCombine\DigiCombine_Gacha_Bg_Splus.dds
DigiCombine\DigiCombine_Gacha_Bg_SS.dds
DigiCombine\DigiCombine_Gacha_Bg_SSplus.dds
DigiCombine\DigiCombine_Gacha_Bg_SSS.dds
DigiCombine\DigiCombine_Gacha_Bg_SSSplus.dds
DigiCombine\DigiCombine_Gacha_Bg_U.dds
DigiCombine\DigiCombine_Gacha_Bg_Uplus.dds
DigiCombine\DigiCombine_Gacha_Bg_N.dds
System\DigiCombine_Gacha_Progress.wav
System\DigiCombine_Gacha_Rare.wav
System\DigiCombine_Gacha_Change.wav
System\DigiCombine_Gacha_Default.wav
```
