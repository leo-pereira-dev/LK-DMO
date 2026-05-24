
/* ============================================================ */
/* Function: FUN_010ca940 */
/* Entry: 010ca940 */
/* Signature: undefined FUN_010ca940() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_010ca940(int param_1)

{
  short sVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  undefined4 uVar7;
  short *psVar8;
  short *psVar9;
  undefined1 local_80 [24];
  undefined4 local_68;
  int *local_64;
  undefined4 uStack_60;
  int *piStack_5c;
  int *piStack_58;
  int *local_54;
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
  puStack_c = &LAB_01980ef2;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar3;
  piStack_58 = (int *)FUN_016830c0(0x74,uVar3);
  local_8 = 0;
  if (piStack_58 == (int *)0x0) {
    iVar4 = 0;
  }
  else {
    memset(piStack_58,0,0x74);
    iVar4 = FUN_01245550();
  }
  local_8 = 0xffffffff;
  if (iVar4 != 0) {
    piStack_5c = (int *)(param_1 + 0x58);
    *(int *)(iVar4 + 0x48) = param_1;
    local_54 = (int *)*piStack_5c;
    if (*(int *)(param_1 + 0x5c) == 0x15555555) {
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8 = 1;
    piStack_58 = (int *)0x0;
    piVar5 = operator_new(0xc);
    local_8 = 0xffffffff;
    piVar5[2] = iVar4;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar2 = *(undefined4 **)((int)local_54 + 4);
    *piVar5 = (int)local_54;
    piVar5[1] = (int)puVar2;
    *(int **)((int)local_54 + 4) = piVar5;
    *puVar2 = piVar5;
    FUN_012459a0(*(undefined4 *)(param_1 + 0x40),s_DungeonUI_Dungeon_Target_Partici_01ad4b7c,0,0,0,0
                 ,0x11f,0x1e2,0x156,0xdb);
    local_54 = (int *)FUN_016830c0(0x94,uVar3);
    local_8 = 2;
    if (local_54 == (void *)0x0) {
      iVar6 = 0;
    }
    else {
      memset(local_54,0,0x94);
      iVar6 = FUN_012360f0();
    }
    local_8 = 0xffffffff;
    *(int *)(param_1 + 0xb4) = iVar6;
    if (iVar6 != 0) {
      FUN_012363b0();
      iVar6 = *(int *)(param_1 + 0xb4);
      *(undefined4 *)(iVar6 + 0x7c) = 0;
      *(undefined4 *)(iVar6 + 0x80) = 5;
      *(undefined4 *)(*(int *)(param_1 + 0xb4) + 0x8c) = 0x1d;
      FUN_010ccf80(5,param_1,&LAB_010ccd40,0);
      local_54 = (int *)FUN_016830c0(0xec);
      local_8 = 3;
      if (local_54 == (void *)0x0) {
        local_54 = (int *)0x0;
      }
      else {
        memset(local_54,0,0xec);
        local_54 = (int *)FUN_01247d90();
      }
      local_8 = 0xffffffff;
      if (local_54 != (int *)0x0) {
        uStack_60 = DAT_01bee890;
        local_64 = DAT_01bee88c;
        piStack_5c = (int *)0x14a;
        piStack_58 = (int *)0xc8;
        FUN_01248090(5,0,3,0,DAT_01bee88c,DAT_01bee890,0x10,200,0x10,0xf,DAT_01bee88c,DAT_01bee890,
                     0x14a,200);
        FUN_012363e0(local_54);
      }
      FUN_012472c0(*(undefined4 *)(param_1 + 0xb4));
      piStack_58 = DAT_01bee88c;
      local_68 = DAT_01bee890;
      local_54 = (int *)FUN_016830c0(0xa0);
      local_8 = 4;
      if (local_54 == (void *)0x0) {
        local_54 = (int *)0x0;
      }
      else {
        memset(local_54,0,0xa0);
        local_54 = (int *)FUN_0123de10();
      }
      local_8 = 0xffffffff;
      FUN_0123e0d0(*(undefined4 *)(iVar4 + 0x40),s_DungeonUI_Dungeon_Target_Partici_01ad4bb0,0,
                   DAT_01bb9b2c,DAT_01bb9b34,1,piStack_58,local_68,0x156,0xdb);
      piStack_58 = local_54;
      FUN_00f55430(&piStack_58);
      *(int **)(param_1 + 0xb8) = local_54;
      if (local_54 != (int *)0x0) {
        (**(code **)(*local_54 + 0x4c))(0x3f000000);
        (**(code **)(**(int **)(param_1 + 0xb8) + 0x1c))(0);
      }
      uVar7 = FUN_01246720(s_DungeonUI_Dungeon_User_Icon_tga_01ad4bf0,1,0x160,0x9a,0x4b,0x41);
      *(undefined4 *)(param_1 + 200) = uVar7;
      local_34 = 0;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_30 = 7;
      local_44 = 0;
      local_18 = 0;
      local_8 = 5;
      local_24 = DAT_01bfe468 + 8;
      local_48 = 0x3f7dfdfe;
      local_20 = 0xd;
      local_50 = CONCAT44(DAT_01b026a4,DAT_01b02628);
      local_2c = 1;
      local_1c = 0;
      local_28 = 5;
      piStack_58 = (int *)0x0;
      psVar8 = (short *)FUN_010ccb60(local_80,&piStack_58);
      local_8._0_1_ = 6;
      if (7 < *(uint *)(psVar8 + 10)) {
        psVar8 = *(short **)psVar8;
      }
      piStack_58 = (int *)(psVar8 + 1);
      psVar9 = psVar8;
      do {
        sVar1 = *psVar9;
        psVar9 = psVar9 + 1;
      } while (sVar1 != 0);
      FUN_00f1a040(psVar8,(int)psVar9 - (int)piStack_58 >> 1);
      local_8 = CONCAT31(local_8._1_3_,5);
      FUN_00f1a330();
      uVar7 = FUN_01246bb0(&local_50,1,0x185,0xcf);
      *(undefined4 *)(param_1 + 0xcc) = uVar7;
      local_24 = 0;
      FUN_00f1a330();
    }
  }
  ExceptionList = local_10;
  return;
}


