
/* ============================================================ */
/* Function: FUN_0126a6b0 */
/* Entry: 0126a6b0 */
/* Signature: undefined FUN_0126a6b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_0126a6b0(int param_1,uint *param_2,int *param_3,undefined4 param_4,uint param_5)

{
  short sVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  int iVar5;
  short *psVar6;
  undefined4 *******pppppppuVar7;
  short *psVar8;
  uint *puVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined1 local_d0 [24];
  undefined4 ******local_b8 [5];
  uint local_a4;
  uint local_a0;
  uint local_9c;
  uint *local_98;
  uint local_94;
  undefined1 local_8d;
  undefined4 ******local_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 local_7c;
  uint local_78;
  undefined4 ******local_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  uint local_60;
  undefined *local_5c;
  int local_58;
  undefined4 uStack_54;
  uint uStack_50;
  uint uStack_4c;
  undefined4 local_48;
  undefined4 ******local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  uint local_30;
  undefined4 ******local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  uint local_1c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199b999;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  local_a0 = param_5;
  if (DAT_01bfb2b4 == 0) {
    return;
  }
  local_94 = DAT_01bfb2b4 + 0x28;
  if (local_94 == 0) {
    return;
  }
  if (199 < *param_2) {
    return;
  }
  if (*param_2 * 0x45 + 0x30 + local_94 == 0) {
    return;
  }
  iVar5 = *param_3;
  if (iVar5 == 0) {
    return;
  }
  ExceptionList = &local_10;
  local_14 = uVar3;
  FUN_00f221a0(param_5);
  local_8 = 0;
  iVar2 = *(int *)(param_1 + 0x94);
  if (iVar2 != 0) {
    *(undefined4 *)(iVar2 + 0x18) = 10;
    *(int *)(iVar2 + 0x1c) = iVar5;
    *(undefined4 *)(iVar2 + 0x20) = 1;
    *(undefined4 *)(iVar2 + 0x24) = 0;
  }
  if (*(int *)(param_1 + 0x98) != 0) {
    pppppppuVar7 = local_b8;
    if (7 < local_a4) {
      pppppppuVar7 = (undefined4 *******)local_b8[0];
    }
    (**(code **)(*(int *)(*(int *)(param_1 + 0x98) + 0x14) + 0x10))(pppppppuVar7,uVar3);
  }
  local_8 = 0xffffffff;
  FUN_00f1a330();
  uVar3 = *(uint *)(*(int *)(param_1 + 0x78) + 0x48);
  if (((((DAT_01bfb2b4 == 0) || (DAT_01bfb2b4 + 0x28 == 0)) ||
       (*(uint *)(DAT_01bfb2b4 + 0x44) <= uVar3)) ||
      ((199 < uVar3 ||
       (local_98 = (uint *)(uVar3 * 0x45 + 0x30 + DAT_01bfb2b4 + 0x28), local_98 == (uint *)0x0))))
     || ((*local_98 == 0 && ((short)local_98[1] == 0)))) {
    local_98 = (uint *)0x0;
  }
  if (*(int *)(*(int *)(param_1 + 0x78) + 0x148) == 0) {
    local_5c = &DAT_01a18e98;
    local_58 = 0;
    uStack_54 = 0;
    uStack_50 = 0;
    uStack_4c = 0;
    local_48 = 0;
  }
  else {
    FUN_015ec7f0(&local_5c,*local_98,*(undefined1 *)((int)local_98 + 9));
  }
  iVar5 = *param_3;
  FUN_00f221a0(local_a0);
  local_8 = 1;
  iVar2 = *(int *)(param_1 + 0xa8);
  if (iVar2 != 0) {
    *(undefined4 *)(iVar2 + 0x18) = 10;
    *(int *)(iVar2 + 0x1c) = iVar5;
    *(undefined4 *)(iVar2 + 0x20) = 1;
    *(undefined4 *)(iVar2 + 0x24) = 0;
  }
  if (*(int *)(param_1 + 0xac) != 0) {
    pppppppuVar7 = &local_44;
    if (7 < local_30) {
      pppppppuVar7 = (undefined4 *******)local_44;
    }
    (**(code **)(*(int *)(*(int *)(param_1 + 0xac) + 0x14) + 0x10))(pppppppuVar7);
  }
  local_8 = 0xffffffff;
  FUN_00f1a330();
  local_64 = 0;
  if (10000 < local_58) {
    local_58 = 10000;
  }
  local_60 = 0;
  local_74 = (undefined4 *******)0x0;
  uStack_70 = 0;
  uStack_6c = 0;
  uStack_68 = 0;
  FUN_00f1a200(&DAT_01ac5dd8,0);
  local_8 = 2;
  FUN_01500f80(&local_74,u__d__02d_01adab20,local_58 / 100,local_58 % 100);
  FUN_00f53390(&DAT_01ad878c,1);
  pppppppuVar7 = &local_74;
  if (7 < local_60) {
    pppppppuVar7 = (undefined4 *******)local_74;
  }
  (**(code **)(*(int *)(*(int *)(param_1 + 0x8c) + 0x14) + 0x10))(pppppppuVar7);
  iVar5 = *(int *)(param_1 + 0xc0);
  local_9c = uStack_50;
  if (iVar5 != 0) {
    uVar3 = *(uint *)(*(int *)(param_1 + 0x78) + 0x140);
    if (uVar3 == 0) {
      local_2c = (undefined4 *******)0x0;
      uStack_28 = 0;
      uStack_24 = 0;
      uStack_20 = 0;
      local_1c = uVar3;
      local_18 = uVar3;
      FUN_00f1a200(&DAT_01ac5dd8,0);
    }
    else {
      local_a0 = uStack_50;
      FUN_015db970(&local_2c,&local_a0);
    }
    local_8._0_1_ = 3;
    pppppppuVar7 = (undefined4 *******)local_2c;
    if (local_18 < 8) {
      pppppppuVar7 = &local_2c;
    }
    (**(code **)(*(int *)(iVar5 + 0x14) + 0x10))(pppppppuVar7);
    local_8 = CONCAT31(local_8._1_3_,2);
    FUN_00f1a330();
  }
  if (*(int **)(param_1 + 0xbc) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xbc) + 0x40))(1);
    iVar5 = *(int *)(param_1 + 0xbc);
    *(undefined4 *)(iVar5 + 0x18) = 10;
    *(uint *)(iVar5 + 0x1c) = local_9c;
    *(undefined4 *)(iVar5 + 0x20) = 1;
    *(undefined4 *)(iVar5 + 0x24) = 0;
  }
  *(undefined4 *)(param_1 + 0x148) = local_48;
  local_8d = 0;
  local_9c = uStack_50;
  uVar4 = FUN_00f62070(&local_9c,&local_8d);
  *(undefined4 *)(param_1 + 0x144) = uVar4;
  local_94 = uStack_4c;
  local_8d = 0;
  iVar5 = FUN_00f62070(&local_94,&local_8d);
  iVar5 = iVar5 + *(int *)(param_1 + 0x144);
  *(int *)(param_1 + 0x144) = iVar5;
  if (999 < iVar5) {
    *(undefined4 *)(param_1 + 0x144) = 999;
  }
  local_7c = 0;
  local_78 = 0;
  local_8c = (undefined4 *******)0x0;
  uStack_88 = 0;
  uStack_84 = 0;
  uStack_80 = 0;
  FUN_00f1a200(&DAT_01ac5dd8,0);
  local_8 = CONCAT31(local_8._1_3_,4);
  FUN_01500f80(&local_8c,u__d__d_01ace96c,*(undefined4 *)(param_1 + 0x144),
               *(undefined4 *)(param_1 + 0x148));
  if (*(int *)(param_1 + 0xd0) != 0) {
    pppppppuVar7 = &local_8c;
    if (7 < local_78) {
      pppppppuVar7 = (undefined4 *******)local_8c;
    }
    (**(code **)(*(int *)(*(int *)(param_1 + 0xd0) + 0x14) + 0x10))(pppppppuVar7);
  }
  if (*(int *)(param_1 + 0xd4) != 0) {
    uVar4 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_GRADE_01ae8af0,0x19);
    psVar6 = (short *)FUN_015ce480(local_d0,uVar4,uVar10,uVar11,uVar12,uVar13,uVar14);
    local_8._0_1_ = 5;
    if (7 < *(uint *)(psVar6 + 10)) {
      psVar6 = *(short **)psVar6;
    }
    local_34 = 0;
    local_30 = 0;
    local_44 = (undefined4 *******)0x0;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    psVar8 = psVar6;
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a200(psVar6,(int)psVar8 - (int)(psVar6 + 1) >> 1);
    local_8._0_1_ = 7;
    FUN_00f1a330();
    local_1c = 0;
    local_18 = 0;
    local_2c = (undefined4 *******)0x0;
    uStack_28 = 0;
    uStack_24 = 0;
    uStack_20 = 0;
    FUN_00f1a200(&DAT_01ac5dd8,0);
    local_8._0_1_ = 8;
    FUN_01500f80(&local_2c,&DAT_01ae8b44,0);
    pppppppuVar7 = &local_2c;
    if (7 < local_18) {
      pppppppuVar7 = (undefined4 *******)local_2c;
    }
    FUN_00f53390(pppppppuVar7,local_1c);
    pppppppuVar7 = &local_44;
    if (7 < local_30) {
      pppppppuVar7 = (undefined4 *******)local_44;
    }
    (**(code **)(**(int **)(param_1 + 0xd4) + 0x10))(pppppppuVar7);
    FUN_00f1a330();
    local_8 = CONCAT31(local_8._1_3_,4);
    FUN_00f1a330();
  }
  puVar9 = local_98;
  if (*(int *)(param_1 + 0xd8) != 0) {
    uVar4 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_GRADE_01ae8af0,0x19);
    psVar6 = (short *)FUN_015ce480(local_d0,uVar4,uVar10,uVar11,uVar12,uVar13,uVar14);
    local_8._0_1_ = 9;
    if (7 < *(uint *)(psVar6 + 10)) {
      psVar6 = *(short **)psVar6;
    }
    local_34 = 0;
    local_30 = 0;
    local_44 = (undefined4 *******)0x0;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    psVar8 = psVar6;
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a200(psVar6,(int)psVar8 - (int)(psVar6 + 1) >> 1);
    local_8._0_1_ = 0xb;
    FUN_00f1a330();
    local_1c = 0;
    local_18 = 0;
    local_2c = (undefined4 *******)0x0;
    uStack_28 = 0;
    uStack_24 = 0;
    uStack_20 = 0;
    FUN_00f1a200(&DAT_01ac5dd8,0);
    puVar9 = local_98;
    local_8._0_1_ = 0xc;
    FUN_01500f80(&local_2c,&DAT_01ae8b44,*(undefined1 *)((int)local_98 + 9));
    pppppppuVar7 = &local_2c;
    if (7 < local_18) {
      pppppppuVar7 = (undefined4 *******)local_2c;
    }
    FUN_00f53390(pppppppuVar7,local_1c);
    pppppppuVar7 = &local_44;
    if (7 < local_30) {
      pppppppuVar7 = (undefined4 *******)local_44;
    }
    (**(code **)(**(int **)(param_1 + 0xd8) + 0x10))(pppppppuVar7);
    FUN_00f1a330();
    local_8 = CONCAT31(local_8._1_3_,4);
    FUN_00f1a330();
  }
  FUN_01232880(1);
  FUN_01232880(1);
  local_9c = *(uint *)(param_1 + 0x78);
  local_a0 = *puVar9;
  if (((*(int *)(local_9c + 0x148) != 0) && (*(int *)(local_9c + 0x140) != 0)) &&
     ((*(int *)(local_9c + 0x14c) != 0 &&
      (local_94 = local_a0, iVar5 = FUN_015db910(&local_94), iVar5 != 0)))) {
    if (*(char *)(iVar5 + 0xc) != '\x03') {
      local_98 = *(uint **)(iVar5 + 0x10);
      goto LAB_0126ad88;
    }
    if (*(int *)(local_9c + 0x150) != 0) {
      local_9c = local_a0;
      iVar5 = FUN_015d1a30(&local_9c);
      if (iVar5 != 0) {
        local_98 = *(uint **)(iVar5 + 4);
        goto LAB_0126ad88;
      }
    }
  }
  local_98 = (uint *)0x0;
LAB_0126ad88:
  FUN_01259020(param_1 + 0xdc,*puVar9,&local_98,1);
  local_94 = (uint)*(byte *)((int)puVar9 + 9);
  local_9c = *puVar9;
  local_a0 = FUN_012b5bd0(&local_9c,&local_94);
  FUN_01259020(param_1 + 0xfc,*puVar9,&local_a0,1);
  FUN_00f1a330();
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


