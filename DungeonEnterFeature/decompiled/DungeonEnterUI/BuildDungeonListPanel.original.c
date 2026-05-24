
/* ============================================================ */
/* Function: FUN_010c93b0 */
/* Entry: 010c93b0 */
/* Signature: undefined FUN_010c93b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_010c93b0(int param_1)

{
  short sVar1;
  undefined4 *puVar2;
  int iVar3;
  void *pvVar4;
  uint uVar5;
  int *piVar6;
  short *psVar7;
  short *psVar8;
  void *local_58;
  void *local_54;
  undefined8 local_50;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01980cc2;
  local_10 = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar5;
  local_58 = (void *)FUN_016830c0(0x74,uVar5);
  local_8 = 0;
  if (local_58 == (void *)0x0) {
    local_58 = (void *)0x0;
  }
  else {
    memset(local_58,0,0x74);
    local_58 = (void *)FUN_01245550();
  }
  pvVar4 = local_58;
  local_8 = 0xffffffff;
  if (local_58 != (void *)0x0) {
    *(int *)((int)local_58 + 0x48) = param_1;
    local_54 = *(void **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) {
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8 = 1;
    piVar6 = operator_new(0xc);
    local_8 = 0xffffffff;
    piVar6[2] = (int)pvVar4;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar2 = *(undefined4 **)((int)local_54 + 4);
    *piVar6 = (int)local_54;
    piVar6[1] = (int)puVar2;
    *(int **)((int)local_54 + 4) = piVar6;
    *puVar2 = piVar6;
    FUN_012459a0(*(undefined4 *)(param_1 + 0x40),s_DungeonUI_Dungeon_List_Frame_tga_01ad4a24,0,0,0,0
                 ,10,0x68,0x108,0x256);
    local_54 = (void *)FUN_016830c0(0xa0,uVar5);
    local_8 = 2;
    if (local_54 == (void *)0x0) {
      local_54 = (void *)0x0;
    }
    else {
      memset(local_54,0,0xa0);
      local_54 = (void *)FUN_0123de10();
    }
    local_8 = 0xffffffff;
    FUN_0123e0d0(*(undefined4 *)((int)pvVar4 + 0x40),s_DungeonUI_Dungeon_Line_tga_01ad4a48,0,
                 DAT_01bb9b2c,DAT_01bb9b34,1,0,0xf,0xd9,0x2c);
    FUN_00f55430(&local_54);
    local_34 = 0;
    local_30 = 7;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    local_44 = 0;
    local_18 = 0;
    local_8 = 3;
    local_24 = DAT_01bfe468 + 8;
    local_20 = 0xb;
    local_50 = CONCAT44(DAT_01b02674,DAT_01b02634);
    local_48 = 0x3f76f6f7;
    local_2c = 1;
    local_1c = 0;
    local_28 = 0;
    FUN_00f19cc0(s_DUNGEON_ENTER_LIST_TITLE_DUNGEON_01ad4a64,0x25);
    psVar7 = (short *)FUN_015ce480();
    local_8._0_1_ = 4;
    if (7 < *(uint *)(psVar7 + 10)) {
      psVar7 = *(short **)psVar7;
    }
    psVar8 = psVar7;
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar7,(int)psVar8 - (int)(psVar7 + 1) >> 1);
    local_8 = CONCAT31(local_8._1_3_,3);
    FUN_00f1a330();
    pvVar4 = local_58;
    FUN_01246bb0(&local_50,1,0x19,0x12);
    local_8 = 0xffffffff;
    local_24 = 0;
    FUN_00f1a330();
    local_58 = (void *)FUN_016830c0(0x94);
    local_8 = 5;
    if (local_58 == (void *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(local_58,0,0x94);
      piVar6 = (int *)FUN_0122b460();
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xac) = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0xc))(*(undefined4 *)((int)pvVar4 + 0x40),0xc,0x37,0xf5,0x212,0);
      iVar3 = *(int *)(param_1 + 0xac);
      *(undefined4 *)(iVar3 + 0x7c) = 0;
      *(undefined4 *)(iVar3 + 0x80) = 5;
      iVar3 = *(int *)(param_1 + 0xac);
      *(undefined4 *)(iVar3 + 0x84) = 0;
      *(undefined4 *)(iVar3 + 0x88) = 5;
      FUN_0122b850(s_DungeonUI_Dungeon_List_ClickBtn__01ad4a8c,&DAT_01bee88c,&DAT_01bee88c);
      FUN_0122b9a0(s_DungeonUI_Dungeon_List_ClickBtn__01ad4a8c,&DAT_01bee88c,&DAT_01bee88c);
      *(undefined1 *)(*(int *)(param_1 + 0xac) + 0x8f) = 1;
      *(undefined1 *)(*(int *)(param_1 + 0xac) + 0x8c) = 0;
      FUN_010ccf80(3,param_1,FUN_010ccd60,0);
      local_58 = (void *)FUN_016830c0(0xec);
      local_8 = 6;
      if (local_58 == (void *)0x0) {
        local_54 = (void *)0x0;
      }
      else {
        memset(local_58,0,0xec);
        local_54 = (void *)FUN_01247d90();
      }
      local_8 = 0xffffffff;
      if (local_54 != (void *)0x0) {
        FUN_01248090(5,0,7,0,DAT_01bee88c,DAT_01bee890,0x10,0x212,0x10,0xf,DAT_01bee88c,DAT_01bee890
                     ,0xf5,0x212);
        FUN_0122c3a0(local_54,1);
      }
      FUN_012472c0(*(undefined4 *)(param_1 + 0xac));
      local_58 = *(void **)(*(int *)(param_1 + 0xa8) + 0x10);
      FUN_010c9820(&local_58);
    }
  }
  ExceptionList = local_10;
  return;
}


