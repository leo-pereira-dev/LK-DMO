# Dungeon Enter Asset Inventory

## Manifestos

- `assets/manifest/DecompiledUiAssetReferences.csv`: referencias hardcoded nas funcoes `.c`.
- `assets/manifest/DynamicXmlAssetReferences.csv`: imagens dinamicas vindas de `DungeonList.xml`.
- `assets/manifest/Pack01RequiredAssets.csv`: lista consolidada de 78 assets unicos esperados no Pack 01.

## Pack

As texturas de interface desta janela foram identificadas como caminhos `DungeonUI\...` e, conforme o contexto do projeto, pertencem ao Pack 01:

- `C:\0-NewDmo\ClientDist\Data\Pack01.hf`
- `C:\0-NewDmo\ClientDist\Data\Pack01.pf`

Nesta etapa eu nao extraí o Pack 01 inteiro para evitar duplicar dezenas de GB. A pasta guarda os caminhos oficiais e a origem exata de cada referencia para uma extracao seletiva posterior. O helper `tools/PackAssetProbe.cpp` foi copiado para esta pasta para facilitar essa extracao seletiva.

## Assets fixos principais

- `DungeonUI\Dungeon_Bg.tga`
- `DungeonUI\Dungeon_Entrance_Btn.tga`
- `DungeonUI\Dungeon_List_Frame.tga`
- `DungeonUI\Dungeon_Line.tga`
- `DungeonUI\Dungeon_List_ClickBtn_Stroke.tga`
- `DungeonUI\Dungeon_List_Btn_Image.tga`
- `DungeonUI\Dungeon_List_Btn.tga`
- `DungeonUI\Dungeon_Reward_Frame.tga`
- `DungeonUI\Dungeon_Slot.tga`
- `DungeonUI\Dungeon_Target_Participation_Frame.tga`
- `DungeonUI\Dungeon_Target_Participation_Unsatisfactory_Frame.tga`
- `DungeonUI\Dungeon_User_Icon.tga`
- `DungeonUI\Dungeon_Image_Frame.tga`
- `DungeonUI\Dungeon_name_Frame.tga`
- `DungeonUI\Dungeon_Level_Easy.tga`
- `DungeonUI\Dungeon_Level_Normal.tga`
- `DungeonUI\Dungeon_Level_Hard.tga`
- `DungeonUI\Dungeon_reward_btn.png`
- `CommonUI\CommonUI_close.tga`
- `Icon\Mask_Over.dds`
