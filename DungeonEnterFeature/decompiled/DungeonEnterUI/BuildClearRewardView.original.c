
/* ============================================================ */
/* Function: FUN_010cd1d0 */
/* Entry: 010cd1d0 */
/* Signature: undefined FUN_010cd1d0() */
/* ============================================================ */


void __thiscall FUN_010cd1d0(int param_1,undefined4 param_2)

{
  uint uVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 local_5c [24];
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  void *local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198136f;
  local_1c = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  FUN_012459a0(param_2,s_DungeonUI_Dungeon_frame_new_png_01ad4f84,0,0,0,1,9,0xc,0x247,0x160);
  local_30 = DAT_01bee88c;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  local_24 = DAT_01bee890;
  local_44 = 0x3ef0a3d7;
  uStack_40 = 0x3f6b851f;
  local_3c = 0x3f800000;
  uVar7 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_DUNGEON_ENTER_CLEAR_REWARD_TITLE_01ad4fa4,0x20);
  puVar2 = (undefined4 *)FUN_015ce480(local_5c,uVar4,uVar5,uVar6,uVar7,uVar8);
  local_14 = 0;
  if (7 < (uint)puVar2[5]) {
    puVar2 = (undefined4 *)*puVar2;
  }
  FUN_012461a0(puVar2,0xb,CONCAT44(uStack_40,local_44),local_3c,local_30,local_24);
  local_14 = 0xffffffff;
  FUN_00f1a330(uVar1);
  iVar3 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,0x226,5,0x1e,0x1c,0,0x1c);
  *(int *)(param_1 + 0x8c) = iVar3;
  if (iVar3 != 0) {
    FUN_010cf590();
  }
  local_34 = 0;
  local_38 = (void *)FUN_016830c0(0x94);
  local_14 = 1;
  if (local_38 == (void *)0x0) {
    iVar3 = 0;
  }
  else {
    memset(local_38,0,0x94);
    iVar3 = FUN_012360f0();
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0x7c) = iVar3;
  if (iVar3 != 0) {
    FUN_012472c0(iVar3);
    (**(code **)(**(int **)(param_1 + 0x7c) + 0xc))
              (*(undefined4 *)(param_1 + 0x40),10,0x23,0x6c,0x11c,0);
    FUN_010cf590();
    FUN_010cd5f0(&local_34);
  }
  FUN_010ce150();
  FUN_010ce810();
  *(undefined1 *)(param_1 + 0x68) = 0;
  ExceptionList = local_1c;
  return;
}


