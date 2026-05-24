
/* ============================================================ */
/* Function: FUN_012742c0 */
/* Entry: 012742c0 */
/* Signature: undefined FUN_012742c0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_012742c0(uint *param_1,undefined4 *param_2,undefined4 param_3,undefined4 param_4)

{
  short sVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  short *psVar6;
  undefined4 ****ppppuVar7;
  int iVar8;
  int iVar9;
  undefined4 in_stack_fffffeac;
  undefined4 in_stack_fffffeb0;
  undefined4 uVar10;
  undefined4 in_stack_fffffeb4;
  undefined4 in_stack_fffffeb8;
  undefined4 uVar11;
  undefined8 in_stack_fffffebc;
  undefined8 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 ***local_114;
  undefined4 uStack_110;
  undefined4 uStack_10c;
  undefined4 uStack_108;
  uint local_104;
  uint local_100;
  int local_fc;
  code *local_f8;
  int local_f4;
  undefined4 *local_f0;
  undefined4 *local_ec;
  uint *local_e8;
  undefined4 *local_e4;
  int *local_e0;
  uint *local_dc;
  undefined1 local_d5;
  undefined1 local_d4 [24];
  undefined4 *local_bc;
  uint *local_a0;
  undefined4 ***local_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 local_8c;
  uint local_88;
  undefined4 ***local_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 local_74;
  uint local_70;
  undefined8 local_6c;
  undefined4 local_64;
  undefined8 local_60;
  undefined8 local_58;
  undefined4 local_50;
  undefined4 ***local_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  uint local_38;
  undefined4 ***local_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 local_24;
  uint local_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199c094;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  uVar13 = 1;
  local_e8 = param_1;
  uVar10 = *param_2;
  local_ec = param_2;
  local_14 = uVar3;
  FUN_00f221a0(param_4);
  FUN_012786f0(uVar10,in_stack_fffffeac,in_stack_fffffeb0,in_stack_fffffeb4,in_stack_fffffeb8,
               in_stack_fffffebc,uVar13);
  iVar9 = local_fc;
  if ((((DAT_01bfb2b4 != 0) && (DAT_01bfb2b4 + 0x28 != 0)) && (*local_e8 < 200)) &&
     (iVar8 = *local_e8 * 0x45 + 0x30 + DAT_01bfb2b4 + 0x28, iVar8 != 0)) {
    local_58 = 0;
    local_50 = 0;
    local_f4 = iVar8;
    if (*(int *)(*(int *)(local_fc + 0x78) + 0x148) == 0) {
      local_58._0_4_ = (int *)0x0;
      local_58 = 0;
      local_50 = 0;
    }
    else {
      FUN_015ec6a0(&local_58,*local_ec,*(byte *)(iVar8 + 9) + 1);
    }
    local_8 = 0;
    if (10000 < *(int *)local_58) {
      *(int *)local_58 = 10000;
    }
    local_8c = 0;
    local_88 = 0;
    local_9c = (undefined4 ****)0x0;
    uStack_98 = 0;
    uStack_94 = 0;
    uStack_90 = 0;
    FUN_00f1a200();
    local_8 = CONCAT31(local_8._1_3_,1);
    FUN_01500f80(&local_9c,u__d__02d_01adab20,*(int *)local_58 / 100,*(int *)local_58 % 100,uVar3);
    FUN_00f53390();
    ppppuVar7 = &local_9c;
    if (7 < local_88) {
      ppppuVar7 = (undefined4 ****)local_9c;
    }
    (**(code **)(*(int *)(*(int *)(iVar9 + 0xb0) + 0x14) + 0x10))(ppppuVar7);
    local_6c = 0;
    local_64 = 0;
    if (*(int *)(*(int *)(iVar9 + 0x78) + 0x148) == 0) {
      local_6c = 0;
      local_64 = 0;
    }
    else {
      FUN_015ec6a0(&local_6c,*local_ec,*(byte *)(iVar8 + 9) + 1);
    }
    local_8._0_1_ = 2;
    FUN_015ec600(local_d4,*local_ec,*(byte *)(iVar8 + 9) + 1);
    local_8._0_1_ = 3;
    local_e0 = (int *)0x0;
    do {
      piVar2 = local_e0;
      FUN_015ec470();
      local_e4 = (undefined4 *)((int)piVar2 * 7);
      local_dc = local_a0;
      iVar8 = *(int *)(iVar9 + 0xbc + (int)piVar2 * 0x1c);
      if (iVar8 != 0) {
        uVar3 = *(uint *)(*(int *)(iVar9 + 0x78) + 0x140);
        if (uVar3 == 0) {
          local_114 = (undefined4 ****)0x0;
          uStack_110 = 0;
          uStack_10c = 0;
          uStack_108 = 0;
          local_104 = uVar3;
          local_100 = uVar3;
          FUN_00f1a200();
        }
        else {
          local_e8 = local_a0;
          FUN_015db970();
        }
        local_8._0_1_ = 4;
        ppppuVar7 = &local_114;
        if (7 < local_100) {
          ppppuVar7 = (undefined4 ****)local_114;
        }
        (**(code **)(*(int *)(iVar8 + 0x14) + 0x10))(ppppuVar7);
        local_8._0_1_ = 3;
        FUN_00f1a330();
      }
      piVar2 = *(int **)(iVar9 + 0xb8 + (int)local_e4 * 4);
      if (piVar2 != (int *)0x0) {
        (**(code **)(*piVar2 + 0x40))(1);
        iVar8 = *(int *)(iVar9 + 0xb8 + (int)local_e4 * 4);
        *(undefined4 *)(iVar8 + 0x18) = 10;
        *(uint **)(iVar8 + 0x1c) = local_dc;
        *(undefined4 *)(iVar8 + 0x20) = 1;
        *(undefined4 *)(iVar8 + 0x24) = 0;
      }
      if (local_e0 == (int *)0x1) {
        if (*(int *)(*(int *)(iVar9 + 0x78) + 0x148) == 0) {
          uVar3 = 0;
        }
        else {
          uVar4 = FUN_015ec410();
          uVar3 = 0;
          if (0 < (int)uVar4) {
            uVar3 = uVar4;
          }
        }
      }
      else {
        uVar3 = 0;
        if (*(int *)((int)local_6c + 8) != 0) {
          uVar3 = 1;
        }
      }
      iVar8 = *(int *)(iVar9 + ((int)local_e0 + 7) * 0x1c);
      if (iVar8 != 0) {
        iVar8 = *(int *)(iVar8 + 0x24);
        if (((iVar8 == 0) ||
            (local_e8 = (uint *)__RTDynamicCast(*(undefined4 *)(iVar8 + 0x34),0,&DAT_01bc4a94,
                                                &DAT_01bc6a70,0), local_e8 == (uint *)0x0)) ||
           ((iVar8 = *(int *)(*(int *)(iVar9 + 200 + (int)local_e4 * 4) + 0x24), iVar8 == 0 ||
            (local_dc = (uint *)__RTDynamicCast(*(undefined4 *)(iVar8 + 0x34),0,&DAT_01bc4a94,
                                                &DAT_01bc6a70,0), local_dc == (uint *)0x0))))
        goto LAB_01274f0e;
        local_74 = 0;
        local_70 = 0;
        local_84 = (undefined4 ****)0x0;
        uStack_80 = 0;
        uStack_7c = 0;
        uStack_78 = 0;
        FUN_00f1a200();
        local_8 = CONCAT31(local_8._1_3_,5);
        if ((int)uVar3 < (int)local_e8[2]) {
          local_e8[2] = uVar3;
          local_dc[2] = uVar3;
        }
        FUN_01500f80(&local_84,u__d__d_01ace96c,local_e8[2],uVar3);
        iVar8 = *(int *)(iVar9 + 0xd0 + (int)local_e4 * 4);
        if (iVar8 != 0) {
          ppppuVar7 = &local_84;
          if (7 < local_70) {
            ppppuVar7 = (undefined4 ****)local_84;
          }
          (**(code **)(*(int *)(iVar8 + 0x14) + 0x10))(ppppuVar7);
        }
        piVar2 = local_e0;
        if (local_e0 == (int *)0x1) {
          if (((*(int *)(*(int *)(iVar9 + 0x78) + 0x148) != 0) &&
              (local_dc = (uint *)FUN_015ec410(), 0 < (int)local_dc)) &&
             ((*(int *)(*(int *)(iVar9 + 0x78) + 0x148) != 0 &&
              (iVar8 = FUN_015ec3c0(1), iVar8 != -1)))) {
            local_e0 = (int *)local_58;
            local_e4 = (undefined4 *)*(int *)local_58;
            iVar8 = (int)local_e4 / (iVar8 / 100);
            if (iVar8 != 0) {
              puVar5 = (undefined4 *)((int)local_e4 + local_e8[2] * (iVar8 / (int)local_dc));
              *(int *)local_58 = (int)puVar5;
              if (10000 < (int)puVar5) {
                *(int *)local_58 = 10000;
              }
              local_24 = 0;
              local_20 = 0;
              local_34 = (undefined4 ****)0x0;
              uStack_30 = 0;
              uStack_2c = 0;
              uStack_28 = 0;
              FUN_00f1a200();
              local_8 = CONCAT31(local_8._1_3_,6);
              FUN_01500f80(&local_34,u__d__02d_01adab20,*(int *)local_58 / 100,
                           *(int *)local_58 % 100);
              FUN_00f53390();
              ppppuVar7 = &local_34;
              if (7 < local_20) {
                ppppuVar7 = (undefined4 ****)local_34;
              }
              (**(code **)(*(int *)(*(int *)(iVar9 + 0xb0) + 0x14) + 0x10))(ppppuVar7);
              FUN_00f1a330();
              goto LAB_01274879;
            }
          }
          local_8._0_1_ = 3;
          FUN_00f1a330();
          break;
        }
LAB_01274879:
        local_8._0_1_ = 3;
        FUN_00f1a330();
        local_e0 = piVar2;
      }
      local_e0 = (int *)((int)local_e0 + 1);
    } while ((int)local_e0 < 2);
    local_60 = 0;
    iVar8 = *(int *)(*(int *)(iVar9 + 0x78) + 0x148);
    if (iVar8 == 0) {
      local_60._0_4_ = 0;
      local_60._4_4_ = iVar8;
      puVar5 = operator_new(0x18);
      local_60 = CONCAT44(local_60._4_4_,puVar5);
      *puVar5 = puVar5;
      puVar5[1] = puVar5;
      local_e4 = puVar5;
    }
    else {
      local_1c = 0;
      FUN_015ec940(&local_1c,*local_ec,*(byte *)(local_f4 + 9) + 1);
      local_8._0_1_ = 7;
      if (local_1c._4_4_ == 0) {
        local_60 = local_60 & 0xffffffff;
        local_e4 = operator_new(0x18);
        local_60 = CONCAT44(local_60._4_4_,local_e4);
        *local_e4 = local_e4;
        local_e4[1] = local_e4;
        *(undefined4 *)((undefined4 *)local_1c)[1] = 0;
        puVar5 = (undefined4 *)*(undefined4 *)local_1c;
        while (puVar5 != (undefined4 *)0x0) {
          puVar5 = (undefined4 *)*puVar5;
          FUN_017986fc();
        }
      }
      else {
        puVar5 = operator_new(0x18);
        local_60 = local_1c;
        *puVar5 = puVar5;
        puVar5[1] = puVar5;
        local_e4 = (undefined4 *)local_1c;
        local_1c = ZEXT48(puVar5);
        *(undefined4 *)puVar5[1] = 0;
        puVar5 = (undefined4 *)*puVar5;
        while (puVar5 != (undefined4 *)0x0) {
          puVar5 = (undefined4 *)*puVar5;
          FUN_017986fc();
        }
      }
      puVar5 = local_e4;
      FUN_017986fc();
    }
    local_e0 = (int *)(iVar9 + 0xf0);
    local_8._0_1_ = 8;
    puVar5 = (undefined4 *)*puVar5;
    iVar9 = 3;
    do {
      local_f8 = (code *)(DAT_01bfb2b4 + 0x28);
      if (local_f8 != (code *)0x0) {
        local_f0 = puVar5 + 3;
        local_d5 = 0;
        local_dc = (uint *)*local_f0;
        local_e8 = (uint *)FUN_00f62070();
        local_dc = (uint *)puVar5[4];
        local_d5 = 0;
        iVar8 = FUN_00f62070();
        local_e8 = (uint *)((int)local_e8 + iVar8);
        local_3c = 0;
        local_38 = 0;
        if (999 < (int)local_e8) {
          local_e8 = (uint *)0x3e7;
        }
        local_4c = (undefined4 ****)0x0;
        uStack_48 = 0;
        uStack_44 = 0;
        uStack_40 = 0;
        FUN_00f1a200();
        local_8 = CONCAT31(local_8._1_3_,9);
        FUN_01500f80(&local_4c,u__d__d_01ace96c,local_e8,puVar5[5]);
        local_dc = (uint *)local_e0[1];
        if (local_dc != (uint *)0x0) {
          local_f8 = *(code **)(*local_dc + 0x1c);
          if ((int)local_e8 < (int)puVar5[5]) {
            (*local_f8)(DAT_01b02768,0);
          }
          else {
            (*local_f8)(CONCAT44(DAT_01b026e0,DAT_01b025d4),0x3f800000);
          }
          ppppuVar7 = &local_4c;
          if (7 < local_38) {
            ppppuVar7 = (undefined4 ****)local_4c;
          }
          (**(code **)(*(int *)(local_e0[1] + 0x14) + 0x10))(ppppuVar7);
        }
        if ((int *)*local_e0 != (int *)0x0) {
          (**(code **)(*(int *)*local_e0 + 0x40))(1);
          puVar5 = (undefined4 *)*puVar5;
          iVar8 = *local_e0;
          uVar10 = *local_f0;
          *(undefined4 *)(iVar8 + 0x18) = 10;
          *(undefined4 *)(iVar8 + 0x1c) = uVar10;
          *(undefined4 *)(iVar8 + 0x20) = 1;
          *(undefined4 *)(iVar8 + 0x24) = 0;
        }
        local_8._0_1_ = 8;
        FUN_00f1a330();
      }
      local_e0 = local_e0 + 3;
      iVar9 = iVar9 + -1;
    } while (iVar9 != 0);
    if (*(int *)(local_fc + 0x114) != 0) {
      uVar10 = 0;
      uVar13 = 0;
      uVar11 = 0;
      uVar12 = 0;
      uVar14 = 0;
      FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_01ae8b74,0x13);
      psVar6 = (short *)FUN_015ce480(&local_114,uVar10,uVar13,uVar11,uVar12,uVar14);
      local_8._0_1_ = 10;
      if (7 < *(uint *)(psVar6 + 10)) {
        psVar6 = *(short **)psVar6;
      }
      local_3c = 0;
      local_38 = 0;
      local_4c = (undefined4 ****)0x0;
      uStack_48 = 0;
      uStack_44 = 0;
      uStack_40 = 0;
      do {
        sVar1 = *psVar6;
        psVar6 = psVar6 + 1;
      } while (sVar1 != 0);
      FUN_00f1a200();
      local_8._0_1_ = 0xc;
      FUN_00f1a330();
      local_24 = 0;
      local_20 = 0;
      local_34 = (undefined4 ****)0x0;
      uStack_30 = 0;
      uStack_2c = 0;
      uStack_28 = 0;
      FUN_00f1a200();
      local_8._0_1_ = 0xd;
      FUN_01500f80(&local_34,&DAT_01ae8b44,*(undefined1 *)(local_f4 + 9));
      FUN_00f53390();
      ppppuVar7 = &local_4c;
      if (7 < local_38) {
        ppppuVar7 = (undefined4 ****)local_4c;
      }
      (**(code **)(**(int **)(local_fc + 0x114) + 0x10))(ppppuVar7);
      FUN_00f1a330();
      local_8._0_1_ = 8;
      FUN_00f1a330();
    }
    if (*(int *)(local_fc + 0x118) != 0) {
      uVar10 = 0;
      uVar13 = 0;
      uVar11 = 0;
      uVar12 = 0;
      uVar14 = 0;
      FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_01ae8b74,0x13);
      psVar6 = (short *)FUN_015ce480(&local_114,uVar10,uVar13,uVar11,uVar12,uVar14);
      local_8._0_1_ = 0xe;
      if (7 < *(uint *)(psVar6 + 10)) {
        psVar6 = *(short **)psVar6;
      }
      local_3c = 0;
      local_38 = 0;
      local_4c = (undefined4 ****)0x0;
      uStack_48 = 0;
      uStack_44 = 0;
      uStack_40 = 0;
      do {
        sVar1 = *psVar6;
        psVar6 = psVar6 + 1;
      } while (sVar1 != 0);
      FUN_00f1a200();
      local_8._0_1_ = 0x10;
      FUN_00f1a330();
      local_24 = 0;
      local_20 = 0;
      local_34 = (undefined4 ****)0x0;
      uStack_30 = 0;
      uStack_2c = 0;
      uStack_28 = 0;
      FUN_00f1a200();
      local_8._0_1_ = 0x11;
      FUN_01500f80(&local_34,&DAT_01ae8b44,*(byte *)(local_f4 + 9) + 1);
      FUN_00f53390();
      ppppuVar7 = &local_4c;
      if (7 < local_38) {
        ppppuVar7 = (undefined4 ****)local_4c;
      }
      (**(code **)(**(int **)(local_fc + 0x118) + 0x10))(ppppuVar7);
      FUN_00f1a330();
      local_8._0_1_ = 8;
      FUN_00f1a330();
    }
    FUN_01232880(1);
    FUN_01232880(1);
    local_f0 = (undefined4 *)(uint)*(byte *)(local_f4 + 9);
    local_f8 = (code *)*local_ec;
    local_dc = (uint *)FUN_012b5bd0();
    FUN_01259020(local_fc + 0x11c,*local_ec,&local_dc,1);
    puVar5 = local_ec;
    local_f0 = (undefined4 *)(*(byte *)(local_f4 + 9) + 1);
    local_f8 = (code *)*local_ec;
    local_dc = (uint *)FUN_012b5bd0();
    FUN_01259020(local_fc + 0x13c,*puVar5,&local_dc,1);
    *(undefined4 *)local_e4[1] = 0;
    puVar5 = (undefined4 *)*local_e4;
    while (puVar5 != (undefined4 *)0x0) {
      puVar5 = (undefined4 *)*puVar5;
      FUN_017986fc();
    }
    FUN_017986fc();
LAB_01274f0e:
    *(undefined4 *)local_bc[1] = 0;
    puVar5 = (undefined4 *)*local_bc;
    while (puVar5 != (undefined4 *)0x0) {
      puVar5 = (undefined4 *)*puVar5;
      FUN_017986fc();
    }
    FUN_017986fc();
    FID_conflict__Tidy();
    FUN_00f1a330();
    FID_conflict__Tidy();
  }
  ExceptionList = local_10;
  return;
}


