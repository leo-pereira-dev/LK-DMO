
/* ============================================================ */
/* Function: FUN_010ccc80 */
/* Entry: 010ccc80 */
/* Signature: undefined FUN_010ccc80() */
/* ============================================================ */


undefined4 * FUN_010ccc80(undefined4 *param_1,int *param_2)

{
  int iVar1;
  
  iVar1 = *param_2;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  if (iVar1 == 1) {
    FUN_00f19cc0(s_DungeonUI_Dungeon_Level_Easy_tga_01ad4d90,0x20);
    return param_1;
  }
  if (iVar1 != 2) {
    if (iVar1 != 3) {
      FUN_00f19cc0(&DAT_01abd9ec,0);
      return param_1;
    }
    FUN_00f19cc0(s_DungeonUI_Dungeon_Level_Hard_tga_01ad4e78,0x20);
    return param_1;
  }
  FUN_00f19cc0(s_DungeonUI_Dungeon_Level_Normal_t_01ad4e54,0x22);
  return param_1;
}


