# Dungeon Enter Original Window Map

## Janela

- Classe/string UI: `DungeonEnter` (`01ae9e30`), criada por `CreateDungeonEnterWindow.original.c`.
- Tamanho base: `900 x 715`, centralizado pelo cliente.
- Fundo: `DungeonUI\Dungeon_Bg.tga`.
- Botao fechar: `CommonUI\CommonUI_close.tga`.
- Botao entrar: `DungeonUI\Dungeon_Entrance_Btn.tga`, texto `DUNGEON_ENTER_BTN_ENTER`.

## Paineis da screenshot

- Lista esquerda "Lista de Calaboucos":
  - `BuildDungeonListPanel.original.c`
  - `DungeonUI\Dungeon_List_Frame.tga`
  - `DungeonUI\Dungeon_Line.tga`
  - `DungeonUI\Dungeon_List_ClickBtn_Stroke.tga`
  - `DUNGEON_ENTER_LIST_TITLE_DUNGEON_LIST`
- Botao/item da lista:
  - `BuildDungeonListButton.original.c`
  - `DungeonUI\Dungeon_List_Btn_Image.tga`
  - `DungeonUI\Dungeon_List_Btn.tga`
  - Imagem dinamica de `DungeonList.xml/ImagePath`.
- Preview central:
  - `BuildDungeonPreviewPanel.original.c`
  - `DungeonUI\Dungeon_Image_Frame.tga`
  - `DungeonUI\Dungeon_name_Frame.tga`
  - `DungeonUI\Dungeon_Level_Easy.tga`
  - `DungeonUI\Dungeon_Level_Normal.tga`
  - `DungeonUI\Dungeon_Level_Hard.tga`
  - `DungeonUI\Dungeon_reward_btn.png`
- Condicoes de participacao:
  - `BuildParticipationPanel.original.c`
  - `BuildRequirementTabs.original.c`
  - `RenderRequirementRow.original.c`
  - `DungeonUI\Dungeon_Target_Participation_Frame.tga`
  - `DungeonUI\Dungeon_Target_Participation_Unsatisfactory_Frame.tga`
  - `DungeonUI\Dungeon_User_Icon.tga`
  - Text keys `DUNGEON_ENTER_REQUIRE_*`.
- Recompensa monstro:
  - `BuildRewardPanel.original.c`
  - `BuildRewardSlot.original.c`
  - `DungeonUI\Dungeon_Reward_Frame.tga`
  - `DungeonUI\Dungeon_Slot.tga`
  - `DUNGEON_ENTER_REWARD_TITLE_REWARD_LIST`

## Registro que bate com a print

O candidato principal para "Treinamento Gankoomon" e:

- `DungeonList.xml`: `DungeonId=5001`, `ImagePath=DungeonUI\DungeonImage\Dungeon_50.png`, dificuldade `FieldA=2`, grupo `FieldB=1`.
- `DungeonQualification.xml`: `DungeonId=5001`, `RequirementType=3`, valores `150, 0`.
- `DungeonObtainable.xml`: `DungeonId=5001`, 9 itens obtiveis.

Tambem foram destacados os registros recentes `300701`, `300801` e `300901`, porque aparecem no final da tabela de dungeon e usam imagens `Dungeon_70`, `Dungeon_407` e `Dungeon_1701`.

## Observacao de fidelidade

O layout e os assets fixos acima foram rastreados por xrefs das funcoes decompiladas. As imagens grandes de dungeon nao ficam hardcoded na funcao: elas vem dinamicamente de `DungeonList.xml`.
