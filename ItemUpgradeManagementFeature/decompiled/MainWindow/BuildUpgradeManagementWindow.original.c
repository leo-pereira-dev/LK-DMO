
/* ============================================================ */
/* Function: FUN_01278ad0 */
/* Entry: 01278ad0 */
/* Signature: undefined FUN_01278ad0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01278ad0(int param_1)

{
  char cVar1;
  short sVar2;
  uint uVar3;
  undefined4 *puVar4;
  int iVar5;
  short *psVar6;
  int *piVar7;
  undefined4 *puVar8;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 uVar9;
  short *psVar10;
  undefined8 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined1 local_7c [24];
  undefined1 *local_64;
  int *piStack_60;
  undefined1 *local_5c;
  undefined4 *local_58;
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
  puStack_c = &LAB_0199c4eb;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  cVar1 = *(char *)(param_1 + 0x74);
  *(undefined2 *)(param_1 + 0x74) = 0x101;
  if (cVar1 == '\0') {
    local_14 = uVar3;
    FUN_012457d0(s_EquipSystem_Recharge_bg_tga_01ae8ed4,1,1,1,1,
                 (int)((float)(DAT_01bc07ec + -0x28a) * DAT_01b0256c),
                 (int)((float)(DAT_01bc07f0 + -700) * DAT_01b025f8),0x28a,700);
    local_64 = &DAT_3f7ae148;
    piStack_60 = (int *)&DAT_3f7ae148;
    local_5c = &DAT_3f7ae148;
    uVar9 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(s_UPGRADESYSTEM_TITLE_01ad2990,0x13);
    puVar4 = (undefined4 *)FUN_015ce480(local_7c,uVar9,uVar11,uVar12,uVar13,uVar14);
    local_8 = 0;
    if (7 < (uint)puVar4[5]) {
      puVar4 = (undefined4 *)*puVar4;
    }
    FUN_012461a0(puVar4,0xf,CONCAT44(piStack_60,local_64),local_5c,0,0xc);
    local_8 = 0xffffffff;
    FUN_00f1a330(uVar3);
    iVar5 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,0x25e,0xc,0x20,0x20,0,0x1c);
    *(int *)(param_1 + 0xb0) = iVar5;
    uVar9 = extraout_ECX;
    if (iVar5 != 0) {
      FUN_01279670();
      uVar9 = extraout_ECX_00;
    }
    iVar5 = FUN_01246da0(s_CommonUI_Menu_tap_btn_tga_01ad7890,uVar9,0x16,0x40,0x96,0x26,0,0x26);
    *(int *)(param_1 + 0xac) = iVar5;
    if (iVar5 != 0) {
      local_34 = 0;
      local_30 = 7;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_44 = 0;
      local_18 = 0;
      local_8 = 1;
      local_24 = DAT_01bfe468 + 8;
      local_50 = DAT_01bb9b2c;
      local_20 = 0xb;
      local_48 = DAT_01bb9b34;
      local_2c = 1;
      local_1c = 0;
      local_28 = 1;
      uVar9 = 0;
      uVar11 = 0;
      uVar12 = 0;
      uVar13 = 0;
      uVar14 = 0;
      FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_01ae8b74,0x13);
      psVar6 = (short *)FUN_015ce480(local_7c,uVar9,uVar11,uVar12,uVar13,uVar14);
      local_8._0_1_ = 2;
      if (7 < *(uint *)(psVar6 + 10)) {
        psVar6 = *(short **)psVar6;
      }
      psVar10 = psVar6;
      do {
        sVar2 = *psVar10;
        psVar10 = psVar10 + 1;
      } while (sVar2 != 0);
      FUN_00f1a040(psVar6,(int)psVar10 - (int)(psVar6 + 1) >> 1);
      local_8 = CONCAT31(local_8._1_3_,1);
      FUN_00f1a330();
      FUN_01226310(&local_50,DAT_01bee88c,DAT_01bee890);
      local_8 = 0xffffffff;
      local_24 = 0;
      FUN_00f1a330();
      FUN_01279670();
      local_54 = *(int **)(param_1 + 0xac);
      puVar4 = operator_new(8);
      *puVar4 = &DAT_01ae8f50;
      puVar4[1] = 0;
      local_58 = puVar4;
      if (*(undefined4 **)((int)local_54 + 0x34) != (undefined4 *)0x0) {
        (**(code **)**(undefined4 **)((int)local_54 + 0x34))(1);
      }
      *(undefined4 **)((int)local_54 + 0x34) = puVar4;
      (**(code **)(**(int **)(param_1 + 0xac) + 0x18))(1);
      FUN_01227420(0,0);
      local_54 = (int *)FUN_01226f60(s_CommonUI_Menu_tap_btn_tga_01ad7890,0xac,0x40,0x96,0x26,0,0x26
                                    );
      if (local_54 != (int *)0x0) {
        local_34 = 0;
        local_30 = 7;
        uStack_40 = 0;
        uStack_3c = 0;
        uStack_38 = 0;
        local_44 = 0;
        local_18 = 0;
        local_8 = 3;
        local_24 = DAT_01bfe468 + 8;
        local_50 = DAT_01bb9b2c;
        local_20 = 0xb;
        local_48 = DAT_01bb9b34;
        local_2c = 1;
        local_1c = 0;
        local_28 = 1;
        uVar9 = 0;
        uVar11 = 0;
        uVar12 = 0;
        uVar13 = 0;
        uVar14 = 0;
        FUN_00f19cc0(s_UPGRADESYSTEM_TAB_UPGRADE_DATA_T_01ae8f18,0x27);
        psVar6 = (short *)FUN_015ce480(local_7c,uVar9,uVar11,uVar12,uVar13,uVar14);
        local_8._0_1_ = 4;
        if (7 < *(uint *)(psVar6 + 10)) {
          psVar6 = *(short **)psVar6;
        }
        psVar10 = psVar6;
        do {
          sVar2 = *psVar10;
          psVar10 = psVar10 + 1;
        } while (sVar2 != 0);
        FUN_00f1a040(psVar6,(int)psVar10 - (int)(psVar6 + 1) >> 1);
        local_8 = CONCAT31(local_8._1_3_,3);
        FUN_00f1a330();
        piVar7 = local_54;
        FUN_01226310(&local_50,DAT_01bee88c,DAT_01bee890);
        local_8 = 0xffffffff;
        local_24 = 0;
        FUN_00f1a330();
        local_54 = operator_new(8);
        *local_54 = (int)&DAT_01ae8f50;
        local_54[1] = 1;
        if ((undefined4 *)piVar7[0xd] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)piVar7[0xd])(1);
        }
        piVar7[0xd] = (int)local_54;
        (**(code **)(*piVar7 + 0x18))(1);
      }
      local_54 = (int *)FUN_01226f60(s_CommonUI_Menu_tap_btn_tga_01ad7890,0x142,0x40,0x96,0x26,0,
                                     0x26);
      if (local_54 != (int *)0x0) {
        local_34 = 0;
        local_30 = 7;
        uStack_40 = 0;
        uStack_3c = 0;
        uStack_38 = 0;
        local_44 = 0;
        local_18 = 0;
        local_8 = 5;
        local_24 = DAT_01bfe468 + 8;
        local_50 = DAT_01bb9b2c;
        local_20 = 0xb;
        local_48 = DAT_01bb9b34;
        local_2c = 1;
        local_1c = 0;
        local_28 = 1;
        uVar9 = 0;
        uVar11 = 0;
        uVar12 = 0;
        uVar13 = 0;
        uVar14 = 0;
        FUN_00f19cc0(s_UPGRADESYSTEM_TAB_UPGRADE_DATA_R_01ae8ef0,0x26);
        psVar6 = (short *)FUN_015ce480(local_7c,uVar9,uVar11,uVar12,uVar13,uVar14);
        local_8._0_1_ = 6;
        if (7 < *(uint *)(psVar6 + 10)) {
          psVar6 = *(short **)psVar6;
        }
        psVar10 = psVar6;
        do {
          sVar2 = *psVar10;
          psVar10 = psVar10 + 1;
        } while (sVar2 != 0);
        FUN_00f1a040(psVar6,(int)psVar10 - (int)(psVar6 + 1) >> 1);
        local_8 = CONCAT31(local_8._1_3_,5);
        FUN_00f1a330();
        piVar7 = local_54;
        FUN_01226310(&local_50,DAT_01bee88c,DAT_01bee890);
        local_8 = 0xffffffff;
        local_24 = 0;
        FUN_00f1a330();
        local_54 = operator_new(8);
        *local_54 = (int)&DAT_01ae8f50;
        local_54[1] = 2;
        if ((undefined4 *)piVar7[0xd] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)piVar7[0xd])(1);
        }
        piVar7[0xd] = (int)local_54;
        (**(code **)(*piVar7 + 0x18))(1);
      }
    }
    local_58 = (undefined4 *)FUN_016830c0(0x198);
    local_8 = 7;
    if (local_58 == (void *)0x0) {
      piVar7 = (int *)0x0;
    }
    else {
      memset(local_58,0,0x198);
      piVar7 = (int *)FUN_01271e80();
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xb8) = piVar7;
    if (piVar7 != (int *)0x0) {
      (**(code **)(*piVar7 + 0x2c))(*(undefined4 *)(param_1 + 0x40),0);
      local_54 = *(int **)(param_1 + 0xb8);
      if (local_54 != (int *)0x0) {
        local_54[0x12] = param_1;
        local_58 = *(undefined4 **)(param_1 + 0x58);
        if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_01279260;
        local_8 = 8;
        local_5c = (undefined1 *)0x0;
        piStack_60 = (int *)(param_1 + 0x58);
        puVar8 = operator_new(0xc);
        local_8 = 0xffffffff;
        puVar8[2] = local_54;
        *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
        puVar4 = *(undefined4 **)((int)local_58 + 4);
        *puVar8 = local_58;
        puVar8[1] = puVar4;
        *(undefined4 **)((int)local_58 + 4) = puVar8;
        *puVar4 = puVar8;
      }
    }
    local_58 = (undefined4 *)FUN_016830c0(0x158);
    local_8 = 9;
    if (local_58 == (void *)0x0) {
      piVar7 = (int *)0x0;
    }
    else {
      memset(local_58,0,0x158);
      piVar7 = (int *)FUN_0126cb80();
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xc0) = piVar7;
    if (piVar7 != (int *)0x0) {
      (**(code **)(*piVar7 + 0x2c))(*(undefined4 *)(param_1 + 0x40),0);
      local_58 = *(undefined4 **)(param_1 + 0xc0);
      if (local_58 != (void *)0x0) {
        *(int *)((int)local_58 + 0x48) = param_1;
        local_54 = *(int **)(param_1 + 0x58);
        if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_01279260;
        local_8 = 10;
        local_5c = (undefined1 *)0x0;
        piStack_60 = (int *)(param_1 + 0x58);
        puVar8 = operator_new(0xc);
        local_8 = 0xffffffff;
        puVar8[2] = local_58;
        *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
        puVar4 = (undefined4 *)local_54[1];
        *puVar8 = local_54;
        puVar8[1] = puVar4;
        local_54[1] = (int)puVar8;
        *puVar4 = puVar8;
      }
    }
    local_58 = (undefined4 *)FUN_016830c0(0x14c);
    local_8 = 0xb;
    if (local_58 == (void *)0x0) {
      piVar7 = (int *)0x0;
    }
    else {
      memset(local_58,0,0x14c);
      piVar7 = (int *)FUN_01268580();
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xc4) = piVar7;
    if (piVar7 != (int *)0x0) {
      (**(code **)(*piVar7 + 0x2c))(*(undefined4 *)(param_1 + 0x40),0);
      local_58 = *(undefined4 **)(param_1 + 0xc4);
      if (local_58 != (void *)0x0) {
        *(int *)((int)local_58 + 0x48) = param_1;
        local_54 = *(int **)(param_1 + 0x58);
        if (*(int *)(param_1 + 0x5c) == 0x15555555) {
LAB_01279260:
                    /* WARNING: Subroutine does not return */
          std::_Xlength_error(s_list_too_long_01abdaa4);
        }
        local_8 = 0xc;
        local_5c = (undefined1 *)0x0;
        piStack_60 = (int *)(param_1 + 0x58);
        puVar8 = operator_new(0xc);
        local_8 = 0xffffffff;
        puVar8[2] = local_58;
        *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
        puVar4 = (undefined4 *)local_54[1];
        *puVar8 = local_54;
        puVar8[1] = puVar4;
        local_54[1] = (int)puVar8;
        *puVar4 = puVar8;
      }
    }
    piVar7 = *(int **)(param_1 + 0xc0);
    if (piVar7 != (int *)0x0) {
      *(undefined1 *)(piVar7 + 2) = 0;
      *(undefined1 *)(piVar7 + 0x1a) = 0;
      (**(code **)(*piVar7 + 0x44))();
    }
    piVar7 = *(int **)(param_1 + 0xc4);
    if (piVar7 != (int *)0x0) {
      *(undefined1 *)(piVar7 + 2) = 0;
      *(undefined1 *)(piVar7 + 0x1a) = 0;
      (**(code **)(*piVar7 + 0x44))();
    }
    piVar7 = *(int **)(param_1 + 0xb8);
    if (piVar7 != (int *)0x0) {
      *(undefined1 *)(piVar7 + 2) = 1;
      *(undefined1 *)(piVar7 + 0x1a) = 1;
      (**(code **)(*piVar7 + 0x44))();
      *(int **)(param_1 + 0xb4) = piVar7;
    }
  }
  ExceptionList = local_10;
  return;
}


