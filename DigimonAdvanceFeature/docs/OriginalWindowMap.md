# Original Window Map

Print alvo: tela de **Avanco de Digimon** em uma sala 3D com duas capsulas,
painel inferior de atributos, taxa de sucesso, tentativas e botoes
`Redefinir` / `Trocar Atributo`.

## Componentes rastreados

| Area visual | Classe/familia oficial | Evidencia |
| --- | --- | --- |
| Cena principal de avanco/enchant | `EnchantTactics` | Runtime string `EnchantTactics`, factory `01283910`, strings `ENCHANTTACTICS_*`. |
| Seletor superior de Digimon | `DigiPowerEnchant` | Factory `0128c990`, init `0125dde0`, binds de cinco slots. |
| Popup de opcao/troca/redefinicao | `EnchantOption` | Factory `0128c5d0`, init `01371e40`, binds de sete slots. |
| Popup de resultado de opcao | `EnchantOptionResultUI` | Factory `0128c7f0`, init `01267760`. |
| Ajuda de transcendencia | `DigimonTranscendenceHelp` | Factory `0128bfa0`, tabs/body em `01119160` e `01119590`. |
| Entrada de pagina | `DigimonEnchantPage` | Comparacao de nome em `01147380`. |
| Tabelas de regra | `Tb::sEnchant*`, `Tb::sTranscend*` | Loader oficial `0161e970`. |

## Textos oficiais principais

- `ENCHANTTACTICS_TITLE_DIGIMON_ENCHANT`
- `ENCHANTTACTICS_BTN_ENCHANT`
- `ENCHANTTACTICS_BTN_RESET`
- `ENCHANTTACTICS_BTN_BACKUP_DISK`
- `ENCHANTTACTICS_BTN_DIGICLONE`
- `ENCHANTTACTICS_BTN_RESET_ITEM`
- `ENCHANTTACTICS_CANNOT_ENCHANT`
- `ENCHANTTACTICS_ENCHANT_LEVEL`
- `DIGIMONTRANSCEND_TRANS_BTN_TEXT`
- `DIGIMONTRANSCENDHELP_EXPLAIN_TAB_TERMS`
- `DIGIMONTRANSCENDHELP_EXPLAIN_TAB_SYSTEM`
- `DIGIMONTRANSCENDHELP_EXPLAIN_TAB_CHARGE`
- `DIGIMONTRANSCENDHELP_EXPLAIN_TAB_PROGRESS`

## Assets centrais visiveis ou diretamente ligados

Os assets abaixo aparecem no subset `CoreAndPopupAssetReferences.csv`:

- `Enchant_Room.mp3`
- `EnchantTactics\card_base.tga`
- `EnchantTactics\card_base2.tga`
- `EnchantTactics\NO.tga`
- `EnchantTactics\CardAni\Card_R_`
- `EnchantTactics\SelectAni\Card_C_`
- `EnchantTactics\CardSuccess\Succes_R_`
- `EnchantTactics\CardFail\Fail_R_`
- `EnchantTactics\SuccessAni\Succes_`
- `EnchantTactics\FailAni\Fail_`
- `DetailInfo\Enchant_AT.tga`
- `DetailInfo\Enchant_bl.tga`
- `DetailInfo\Enchant_ct.tga`
- `DetailInfo\Enchant_ev.tga`
- `DetailInfo\Enchant_HP.tga`
- `DigimonStatus_New\Digimon_Status_Enchant_01.tga`
- `TacticsHouse\CheckOnline.tga`
- `Breakthrough\Breakthrough_Main_BG.tga`
- `Breakthrough\Breakthrough_Digimon_info_BG.tga`
- `Data\Interface\Breakthrough\Breakthrough_Digimon_info_B.tga`
- `system\LevelEffect\Enchant_K45.nif`
- `system\LevelEffect\Enchant_K60.nif`
- `System\transcendental.wav`
- `System\Tactics_enchant_fail.nif`
- `game\Enchant_Fail.mp3`

## Observacao sobre assets adjacentes

Algumas funcoes copiadas sao classes compartilhadas de enchant/opcoes. Por isso
o trace tambem captura assets como `Accessory\Enchant\...`, `Random_box\...` e
`SeasonPass\...`. Eles foram preservados em
`AdjacentCopiedClusterAssetReferences.csv`, mas nao devem ser assumidos como
visuais obrigatorios da print sem nova confirmacao.
