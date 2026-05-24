
/* ============================================================ */
/* Function: FUN_011a72c0 */
/* Entry: 011a72c0 */
/* Signature: undefined FUN_011a72c0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 __thiscall FUN_011a72c0(undefined4 *param_1,int param_2,short *param_3)

{
  short sVar1;
  bool bVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  void *pvVar5;
  int iVar6;
  int *piVar7;
  short *psVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  undefined8 uStack_ec;
  undefined4 *****pppppuStack_e4;
  undefined4 uStack_e0;
  undefined8 local_dc;
  undefined4 *****local_d4;
  int iStack_d0;
  undefined4 *puStack_cc;
  int local_9c;
  int local_98;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 *****local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined8 local_30;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019900a3;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  pvVar5 = (void *)FUN_016830c0();
  local_14 = 0;
  if (pvVar5 == (void *)0x0) {
    iVar6 = 0;
  }
  else {
    puStack_cc = (undefined4 *)0x11a7339;
    memset(pvVar5,0,0x2c);
    iVar6 = FUN_010baa10();
  }
  local_14 = 0xffffffff;
  if (iVar6 != 0) {
    if ((*(int *)(param_2 + 4) == 0) || (*(short *)(param_2 + 8) == 0)) {
      bVar2 = true;
    }
    else {
      bVar2 = false;
    }
    pvVar5 = (void *)FUN_016830c0();
    local_14 = 1;
    if (pvVar5 == (void *)0x0) {
      iVar6 = 0;
    }
    else {
      puStack_cc = (undefined4 *)0x11a73ae;
      memset(pvVar5,0,0xa0);
      iVar6 = FUN_0123de10();
    }
    local_14 = 0xffffffff;
    if (iVar6 != 0) {
      local_30 = 0;
      local_40 = (undefined4 ******)0x0;
      uStack_3c = 0;
      uStack_38 = 0;
      uStack_34 = 0;
      FUN_00f19cc0();
      local_14 = 2;
      pppppuStack_e4 = &local_40;
      if (0xf < local_30._4_4_) {
        pppppuStack_e4 = local_40;
      }
      puStack_cc = (undefined4 *)DAT_01bee88c;
      iStack_d0 = 1;
      uStack_e0 = 0;
      local_dc = DAT_01bb9b2c;
      local_d4 = DAT_01bb9b34;
      uStack_ec = 0x11a745e;
      FUN_0123e0d0();
      puStack_cc = (undefined4 *)DAT_01bee88c;
      local_d4 = (undefined4 *****)0x11a747a;
      iStack_d0 = iVar6;
      piVar7 = (int *)FUN_01230f40();
      *param_1 = piVar7;
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))();
      }
      local_14 = 0xffffffff;
      FUN_00f1a420();
    }
    pvVar5 = (void *)FUN_016830c0();
    local_14 = 3;
    if (pvVar5 == (void *)0x0) {
      local_98 = 0;
    }
    else {
      puStack_cc = (undefined4 *)0x11a74c5;
      memset(pvVar5,0,0xa0);
      local_98 = FUN_0123de10();
    }
    local_14 = 0xffffffff;
    if (local_98 != 0) {
      local_9c = param_2 + 0xc;
      if (0xf < *(uint *)(param_2 + 0x20)) {
        local_9c = *(int *)local_9c;
      }
      puStack_cc = *(undefined4 **)(param_2 + 0x28);
      iStack_d0 = 0x28;
      local_d4 = (undefined4 *****)0x27;
      local_dc = 0x400000000;
      uStack_e0 = 1;
      uStack_ec = DAT_01bb9b2c;
      pppppuStack_e4 = DAT_01bb9b34;
      FUN_0123e200(0,local_9c,&uStack_ec);
      puStack_cc = (undefined4 *)0x4;
      iStack_d0 = local_98;
      local_d4 = (undefined4 *****)0x11a756d;
      piVar7 = (int *)FUN_01230f40();
      param_1[1] = piVar7;
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))();
        (**(code **)(*(int *)param_1[1] + 0x40))();
      }
    }
    if ((*(char *)(param_2 + 0x25) != '\0') && (!bVar2)) {
      pvVar5 = (void *)FUN_016830c0();
      local_14 = 4;
      if (pvVar5 == (void *)0x0) {
        iVar6 = 0;
      }
      else {
        puStack_cc = (undefined4 *)0x11a75df;
        memset(pvVar5,0,0xa0);
        iVar6 = FUN_0123de10();
      }
      local_14 = 0xffffffff;
      if (iVar6 != 0) {
        local_30 = 0;
        local_40 = (undefined4 ******)0x0;
        uStack_3c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        FUN_00f19cc0();
        local_14 = 5;
        pppppuStack_e4 = &local_40;
        if (0xf < local_30._4_4_) {
          pppppuStack_e4 = local_40;
        }
        puStack_cc = (undefined4 *)DAT_01bee88c;
        iStack_d0 = 1;
        uStack_e0 = 0;
        local_dc = DAT_01bb9b2c;
        local_d4 = DAT_01bb9b34;
        uStack_ec = 0x11a7677;
        FUN_0123e0d0();
        puStack_cc = (undefined4 *)DAT_01bee88c;
        local_d4 = (undefined4 *****)0x11a7693;
        iStack_d0 = iVar6;
        piVar7 = (int *)FUN_01230f40();
        param_1[2] = piVar7;
        if (piVar7 != (int *)0x0) {
          (**(code **)(*piVar7 + 0x54))();
          (**(code **)(*(int *)param_1[2] + 0x40))();
        }
        local_14 = 0xffffffff;
        FUN_00f1a420();
      }
      pvVar5 = (void *)FUN_016830c0();
      local_14 = 6;
      if (pvVar5 == (void *)0x0) {
        iVar6 = 0;
      }
      else {
        puStack_cc = (undefined4 *)0x11a76e9;
        memset(pvVar5,0,0xa0);
        iVar6 = FUN_0123de10();
      }
      local_14 = 0xffffffff;
      if (iVar6 != 0) {
        local_30 = 0;
        local_40 = (undefined4 ******)0x0;
        uStack_3c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        FUN_00f19cc0();
        local_14 = 7;
        pppppuStack_e4 = &local_40;
        if (0xf < local_30._4_4_) {
          pppppuStack_e4 = local_40;
        }
        puStack_cc = (undefined4 *)DAT_01bee88c;
        iStack_d0 = 1;
        uStack_e0 = 0;
        local_dc = DAT_01bb9b2c;
        local_d4 = DAT_01bb9b34;
        uStack_ec = 0x11a7781;
        FUN_0123e0d0();
        puStack_cc = (undefined4 *)DAT_01bee88c;
        local_d4 = (undefined4 *****)0x11a779d;
        iStack_d0 = iVar6;
        piVar7 = (int *)FUN_01230f40();
        param_1[3] = piVar7;
        if (piVar7 != (int *)0x0) {
          (**(code **)(*piVar7 + 0x54))();
          (**(code **)(*(int *)param_1[3] + 0x40))();
        }
        FUN_00f1a420();
      }
    }
    local_14 = 8;
    local_30 = CONCAT44(&DAT_3f7ae148,(undefined4)local_30);
    psVar8 = (short *)FUN_015027f0();
    local_14._0_1_ = 9;
    if (7 < *(uint *)(psVar8 + 10)) {
      psVar8 = *(short **)psVar8;
    }
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040();
    local_14 = CONCAT31(local_14._1_3_,8);
    FUN_00f1a330();
    puStack_cc = (undefined4 *)0x11a78ab;
    piVar7 = (int *)FUN_01230c40();
    param_1[4] = piVar7;
    if (piVar7 != (int *)0x0) {
      (**(code **)(*piVar7 + 0x54))();
      (**(code **)(*(int *)param_1[4] + 0x40))();
    }
    local_14 = 0xffffffff;
    FUN_00f1a330();
    pvVar5 = (void *)FUN_016830c0();
    local_14 = 10;
    if (pvVar5 == (void *)0x0) {
      local_9c = 0;
    }
    else {
      puStack_cc = (undefined4 *)0x11a7916;
      memset(pvVar5,0,0xa0);
      local_9c = FUN_0123de10();
    }
    uVar4 = DAT_01bee890;
    uVar3 = DAT_01bee88c;
    local_14 = 0xffffffff;
    if (local_9c != 0) {
      local_30 = DAT_01bb9b2c;
      FUN_00f19f10();
      local_48 = 0;
      uStack_54 = 0;
      uStack_50 = 0;
      uStack_4c = 0;
      local_44 = 0xf;
      local_58 = 0;
      puStack_cc = &local_58;
      local_14 = 0xb;
      iStack_d0 = 0x11a79ac;
      FUN_015010a0();
      FUN_0124ef30();
      puStack_cc = (undefined4 *)0x0;
      iStack_d0 = 0x11a79de;
      iStack_d0 = FUN_0124f150();
      local_d4 = (undefined4 *****)0x45;
      local_dc = CONCAT44(0x2f,uVar4);
      uStack_e0 = uVar3;
      pppppuStack_e4 = (undefined4 ******)0x0;
      uStack_ec = CONCAT44(0x11a79f3,(undefined4)uStack_ec);
      FUN_0123e330();
      local_14 = 0xffffffff;
      FUN_00f1a420();
      puStack_cc = (undefined4 *)DAT_01bee88c;
      iStack_d0 = local_9c;
      local_d4 = (undefined4 *****)0x11a7a1d;
      piVar7 = (int *)FUN_01230f40();
      param_1[5] = piVar7;
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))();
        (**(code **)(*(int *)param_1[5] + 0x40))();
      }
    }
    puVar9 = (undefined4 *)FUN_016830c0();
    if (puVar9 != (undefined4 *)0x0) {
      *puVar9 = 0;
      puVar9[1] = 0;
      puVar9[2] = 0;
      puVar9[3] = 0;
      puVar9[4] = 0;
      puVar9[5] = 0;
      puVar9[6] = 0;
      puVar9[7] = 0;
      *(undefined8 *)(puVar9 + 8) = 0;
      sVar1 = *param_3;
      *puVar9 = &DAT_01ae66e0;
      puVar9[1] = 0;
      puVar9[2] = 0;
      puVar9[3] = (int)sVar1;
      *(undefined1 *)(puVar9 + 4) = 0;
      puVar9[5] = 0x2f;
      puVar9[6] = 0x45;
      *(undefined2 *)(puVar9 + 7) = 0x101;
      puVar9[8] = 0;
      puVar9[9] = 0;
      param_1[7] = puVar9;
      puVar10 = operator_new(0x10);
      *puVar10 = &DAT_01ae10a4;
      *(undefined2 *)(puVar10 + 1) = *(undefined2 *)(param_2 + 10);
      puVar10[2] = *(undefined4 *)(param_2 + 4);
      *(undefined2 *)(puVar10 + 3) = *(undefined2 *)(param_2 + 8);
      *(short *)((int)puVar10 + 0xe) = *param_3;
      if ((undefined4 *)puVar9[2] != (undefined4 *)0x0) {
        (*(code *)**(undefined4 **)puVar9[2])();
      }
      puVar9[2] = puVar10;
      FUN_012288c0();
      ExceptionList = local_1c;
      return 1;
    }
    param_1[7] = 0;
    FUN_011133d0();
    FUN_01683120();
  }
  ExceptionList = local_1c;
  return 0;
}


