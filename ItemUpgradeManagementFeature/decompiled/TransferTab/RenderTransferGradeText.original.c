
/* ============================================================ */
/* Function: FUN_0126ef40 */
/* Entry: 0126ef40 */
/* Signature: undefined FUN_0126ef40() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall
FUN_0126ef40(int param_1,uint *param_2,int *param_3,undefined4 param_4,int param_5,char param_6)

{
  short sVar1;
  int iVar2;
  undefined4 **ppuVar3;
  undefined4 ***pppuVar4;
  uint uVar5;
  void *pvVar6;
  short *psVar7;
  undefined4 ****ppppuVar8;
  int iVar9;
  short *psVar10;
  undefined1 *in_stack_ffffff48;
  undefined4 in_stack_ffffff4c;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 ***pppuVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined1 local_90 [24];
  int local_78;
  undefined4 **local_74;
  int local_70;
  int *local_6c;
  undefined4 ***local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  uint local_54;
  undefined8 local_50;
  undefined4 ***local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  uint local_34;
  undefined4 ***local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  uint local_20;
  uint local_1c;
  undefined4 ***local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199bccb;
  local_10 = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  local_78 = param_5;
  if (DAT_01bfb2b4 == 0) {
    return;
  }
  iVar9 = DAT_01bfb2b4 + 0x28;
  if (iVar9 == 0) {
    return;
  }
  if (199 < *param_2) {
    return;
  }
  if (*param_2 * 0x45 + 0x30 + iVar9 != 0) {
    local_70 = param_1;
    local_14 = uVar5;
    if (param_6 != '\0') {
      uVar5 = *(uint *)(*(int *)(param_1 + 0x78) + 0x48);
      if ((((*(uint *)(DAT_01bfb2b4 + 0x44) <= uVar5) || (199 < uVar5)) ||
          (local_6c = (int *)(uVar5 * 0x45 + 0x30 + iVar9), local_6c == (int *)0x0)) ||
         ((*local_6c == 0 && ((short)local_6c[1] == 0)))) {
        local_6c = (int *)0x0;
      }
      local_50 = 0;
      iVar9 = *(int *)(*(int *)(param_1 + 0x78) + 0x148);
      if (iVar9 == 0) {
        ExceptionList = &local_10;
        local_50._0_4_ = iVar9;
        local_50._4_4_ = iVar9;
        pvVar6 = operator_new(0x38);
        *(void **)pvVar6 = pvVar6;
        *(void **)((int)pvVar6 + 4) = pvVar6;
        *(void **)((int)pvVar6 + 8) = pvVar6;
        *(undefined2 *)((int)pvVar6 + 0xc) = 0x101;
        local_50 = CONCAT44(local_50._4_4_,pvVar6);
      }
      else {
        ExceptionList = &local_10;
        FUN_015ec8b0(&local_50,*local_6c,*(undefined1 *)((int)local_6c + 9));
      }
      local_8 = 1;
      ppuVar3 = (undefined4 **)*param_3;
      local_74 = ppuVar3;
      FUN_01271bc0(&local_18,&local_74);
      FUN_00f221a0(local_78);
      local_8._0_1_ = 2;
      iVar9 = *(int *)(param_1 + 0xa8);
      if (iVar9 != 0) {
        *(undefined4 *)(iVar9 + 0x18) = 10;
        *(undefined4 ***)(iVar9 + 0x1c) = ppuVar3;
        *(undefined4 *)(iVar9 + 0x20) = 1;
        *(undefined4 *)(iVar9 + 0x24) = 0;
      }
      if (*(int *)(param_1 + 0xac) != 0) {
        ppppuVar8 = &local_48;
        if (7 < local_34) {
          ppppuVar8 = (undefined4 ****)local_48;
        }
        (**(code **)(*(int *)(*(int *)(param_1 + 0xac) + 0x14) + 0x10))(ppppuVar8);
      }
      local_8._0_1_ = 1;
      FUN_00f1a330();
      pppuVar4 = local_18;
      if (10000 < (int)local_18[7]) {
        local_18[7] = (undefined4 ***)0x2710;
      }
      local_58 = 0;
      local_54 = 0;
      local_68 = (undefined4 ****)0x0;
      uStack_64 = 0;
      uStack_60 = 0;
      uStack_5c = 0;
      FUN_00f1a200(&DAT_01ac5dd8,0);
      local_8 = CONCAT31(local_8._1_3_,3);
      FUN_01500f80(&local_68,u__d__02d_01adab20,(int)pppuVar4[7] / 100,(int)pppuVar4[7] % 100);
      FUN_00f53390(&DAT_01ad878c,1);
      ppppuVar8 = &local_68;
      if (7 < local_54) {
        ppppuVar8 = (undefined4 ****)local_68;
      }
      (**(code **)(*(int *)(*(int *)(param_1 + 0x8c) + 0x14) + 0x10))(ppppuVar8);
      local_78 = *(int *)(param_1 + 0xc0);
      local_74 = pppuVar4[10];
      if (local_78 != 0) {
        uVar5 = *(uint *)(*(int *)(param_1 + 0x78) + 0x140);
        if (uVar5 == 0) {
          local_30 = (undefined4 ****)0x0;
          uStack_2c = 0;
          uStack_28 = 0;
          uStack_24 = 0;
          local_20 = uVar5;
          local_1c = uVar5;
          FUN_00f1a200(&DAT_01ac5dd8,0);
        }
        else {
          local_18 = (undefined4 ***)local_74;
          FUN_015db970(&local_30,&local_18);
        }
        local_18 = &local_30;
        local_8._0_1_ = 4;
        ppppuVar8 = (undefined4 ****)local_30;
        if (local_1c < 8) {
          ppppuVar8 = (undefined4 ****)local_18;
        }
        (**(code **)(*(int *)(local_78 + 0x14) + 0x10))(ppppuVar8);
        local_8 = CONCAT31(local_8._1_3_,3);
        FUN_00f1a330();
      }
      if (*(int **)(param_1 + 0xbc) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0xbc) + 0x40))(1);
        iVar9 = *(int *)(param_1 + 0xbc);
        *(undefined4 *)(iVar9 + 0x18) = 10;
        *(undefined4 ***)(iVar9 + 0x1c) = local_74;
        *(undefined4 *)(iVar9 + 0x20) = 1;
        *(undefined4 *)(iVar9 + 0x24) = 0;
      }
      *(undefined4 ***)(param_1 + 0x154) = pppuVar4[0xc];
      if (*(int *)(param_1 + 0xcc) != 0) {
        local_20 = 0;
        local_1c = 0;
        local_30 = (undefined4 ****)0x0;
        uStack_2c = 0;
        uStack_28 = 0;
        uStack_24 = 0;
        FUN_00f1a200(&DAT_01ac5dd8,0);
        local_8 = CONCAT31(local_8._1_3_,5);
        FUN_01500f80(&local_30,u__d__d_01ace96c,*(undefined4 *)(param_1 + 0x150),
                     *(undefined4 *)(param_1 + 0x154));
        if (*(int *)(param_1 + 0xd8) != 0) {
          ppppuVar8 = &local_30;
          if (7 < local_1c) {
            ppppuVar8 = (undefined4 ****)local_30;
          }
          (**(code **)(*(int *)(*(int *)(param_1 + 0xd8) + 0x14) + 0x10))(ppppuVar8);
        }
        local_8 = CONCAT31(local_8._1_3_,3);
        FUN_00f1a330();
      }
      if (*(int *)(param_1 + 0xdc) != 0) {
        in_stack_ffffff4c = 0;
        uVar11 = 0;
        uVar16 = 0;
        uVar12 = 0;
        uVar14 = 0;
        uVar15 = 0;
        FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_GRADE_01ae8af0,0x19);
        in_stack_ffffff48 = local_90;
        psVar7 = (short *)FUN_015ce480(in_stack_ffffff48,in_stack_ffffff4c,uVar11,uVar16,uVar12,
                                       uVar14,uVar15);
        local_8._0_1_ = 6;
        if (7 < *(uint *)(psVar7 + 10)) {
          psVar7 = *(short **)psVar7;
        }
        local_38 = 0;
        local_34 = 0;
        local_48 = (undefined4 ****)0x0;
        uStack_44 = 0;
        uStack_40 = 0;
        uStack_3c = 0;
        psVar10 = psVar7;
        do {
          sVar1 = *psVar10;
          psVar10 = psVar10 + 1;
        } while (sVar1 != 0);
        FUN_00f1a200(psVar7,(int)psVar10 - (int)(psVar7 + 1) >> 1);
        local_8._0_1_ = 8;
        FUN_00f1a330();
        local_20 = 0;
        local_1c = 0;
        local_30 = (undefined4 ****)0x0;
        uStack_2c = 0;
        uStack_28 = 0;
        uStack_24 = 0;
        FUN_00f1a200(&DAT_01ac5dd8,0);
        local_8._0_1_ = 9;
        FUN_01500f80(&local_30,&DAT_01ae8b44,*(undefined1 *)((int)local_6c + 9));
        ppppuVar8 = &local_30;
        if (7 < local_1c) {
          ppppuVar8 = (undefined4 ****)local_30;
        }
        FUN_00f53390(ppppuVar8,local_20);
        param_1 = local_70;
        ppppuVar8 = &local_48;
        if (7 < local_34) {
          ppppuVar8 = (undefined4 ****)local_48;
        }
        (**(code **)(**(int **)(local_70 + 0xdc) + 0x10))(ppppuVar8);
        FUN_00f1a330();
        local_8 = CONCAT31(local_8._1_3_,3);
        FUN_00f1a330();
      }
      if (*(int *)(param_1 + 0xe0) != 0) {
        in_stack_ffffff4c = 0;
        uVar11 = 0;
        uVar16 = 0;
        uVar12 = 0;
        uVar14 = 0;
        uVar15 = 0;
        FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_GRADE_01ae8af0,0x19);
        in_stack_ffffff48 = local_90;
        psVar7 = (short *)FUN_015ce480(in_stack_ffffff48,in_stack_ffffff4c,uVar11,uVar16,uVar12,
                                       uVar14,uVar15);
        local_8._0_1_ = 10;
        if (7 < *(uint *)(psVar7 + 10)) {
          psVar7 = *(short **)psVar7;
        }
        local_38 = 0;
        local_34 = 0;
        local_48 = (undefined4 ****)0x0;
        uStack_44 = 0;
        uStack_40 = 0;
        uStack_3c = 0;
        psVar10 = psVar7;
        do {
          sVar1 = *psVar10;
          psVar10 = psVar10 + 1;
        } while (sVar1 != 0);
        FUN_00f1a200(psVar7,(int)psVar10 - (int)(psVar7 + 1) >> 1);
        local_8._0_1_ = 0xc;
        FUN_00f1a330();
        local_20 = 0;
        local_1c = 0;
        local_30 = (undefined4 ****)0x0;
        uStack_2c = 0;
        uStack_28 = 0;
        uStack_24 = 0;
        FUN_00f1a200(&DAT_01ac5dd8,0);
        local_8._0_1_ = 0xd;
        FUN_01500f80(&local_30,&DAT_01ae8b44,pppuVar4[6]);
        ppppuVar8 = &local_30;
        if (7 < local_1c) {
          ppppuVar8 = (undefined4 ****)local_30;
        }
        FUN_00f53390(ppppuVar8,local_20);
        param_1 = local_70;
        ppppuVar8 = &local_48;
        if (7 < local_34) {
          ppppuVar8 = (undefined4 ****)local_48;
        }
        (**(code **)(**(int **)(local_70 + 0xe0) + 0x10))(ppppuVar8);
        FUN_00f1a330();
        local_8 = CONCAT31(local_8._1_3_,3);
        FUN_00f1a330();
      }
      FUN_01232880(1);
      FUN_01232880(1);
      local_18 = (undefined4 ***)(uint)*(byte *)((int)local_6c + 9);
      local_78 = *local_6c;
      local_74 = (undefined4 **)FUN_012b5bd0(&local_78,&local_18);
      FUN_01259020(param_1 + 0xe4,*local_6c,&local_74,1);
      local_18 = (undefined4 ***)FUN_012b5bd0(pppuVar4 + 4,pppuVar4 + 6);
      iVar2 = local_70;
      ppppuVar8 = &local_18;
      uVar16 = 1;
      pppuVar13 = (undefined4 ***)pppuVar4[4];
      iVar9 = local_70 + 0x104;
      uVar11 = 0x126f589;
      FUN_01259020(iVar9,pppuVar13,ppppuVar8,1);
      if (*(int *)(iVar2 + 0x128) != 0) {
        FUN_00f61870(pppuVar4[9],(int)pppuVar4[9] >> 0x1f);
        FUN_013f3620(in_stack_ffffff48,in_stack_ffffff4c,uVar11,iVar9,pppuVar13,ppppuVar8,uVar16);
      }
      FUN_00f1a330();
      FUN_0115a360();
      ExceptionList = local_10;
      return;
    }
    iVar9 = *param_3;
    if (iVar9 == 0) {
      return;
    }
    ExceptionList = &local_10;
    FUN_00f221a0(param_5);
    local_8 = 0;
    iVar2 = *(int *)(param_1 + 0x94);
    if (iVar2 != 0) {
      *(undefined4 *)(iVar2 + 0x18) = 10;
      *(int *)(iVar2 + 0x1c) = iVar9;
      *(undefined4 *)(iVar2 + 0x20) = 1;
      *(undefined4 *)(iVar2 + 0x24) = 0;
    }
    if (*(int *)(param_1 + 0x98) != 0) {
      ppppuVar8 = &local_48;
      if (7 < local_34) {
        ppppuVar8 = (undefined4 ****)local_48;
      }
      (**(code **)(*(int *)(*(int *)(param_1 + 0x98) + 0x14) + 0x10))(ppppuVar8,uVar5);
    }
    FUN_00f1a330();
    ExceptionList = local_10;
    return;
  }
  return;
}


