
/* ============================================================ */
/* Function: FUN_01352d20 */
/* Entry: 01352d20 */
/* Signature: undefined FUN_01352d20() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01352d20(int param_1,uint *param_2)

{
  int *piVar1;
  int *piVar2;
  code *pcVar3;
  undefined8 uVar4;
  undefined1 uVar5;
  char cVar6;
  uint uVar7;
  undefined4 ****ppppuVar8;
  undefined4 uVar9;
  float *pfVar10;
  int iVar11;
  int iVar12;
  undefined1 local_4e1;
  float *local_4e0;
  int local_4dc;
  undefined1 local_4d8 [348];
  int *local_37c;
  undefined1 local_cc [24];
  undefined1 local_b4 [24];
  undefined4 ***local_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined8 local_8c;
  undefined4 ***local_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined8 local_74;
  undefined4 ***local_6c;
  undefined4 **ppuStack_68;
  undefined4 **ppuStack_64;
  undefined4 **ppuStack_60;
  undefined8 local_5c;
  undefined4 ***local_54;
  undefined4 **ppuStack_50;
  undefined4 **ppuStack_4c;
  undefined4 **ppuStack_48;
  undefined8 local_44;
  int *local_3c;
  undefined8 local_38;
  float local_30;
  undefined4 ***local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  undefined4 uStack_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_019a9d39;
  local_10 = ExceptionList;
  uVar7 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar11 = *(int *)(param_1 + 0xa8);
  ppuStack_68 = (undefined4 ***)0x0;
  ppuStack_64 = (undefined4 ***)0x0;
  ppuStack_60 = (undefined4 ***)0x0;
  local_5c = 0xf00000000;
  local_6c = (undefined4 ****)0x0;
  local_8 = 0;
  local_4dc = 1;
  local_14 = uVar7;
  FUN_00f1b390(&local_38,param_2);
  if ((((*(char *)((int)local_30 + 0xd) == '\0') && (*(uint *)((int)local_30 + 0x10) <= *param_2))
      && (local_30 != *(float *)(iVar11 + 0x14))) &&
     (*(int *)(*(int *)(param_1 + 0xa8) + 0x5c) != 0)) {
    local_4e1 = 0;
    if (*(int *)((int)local_30 + 0x44) != -1) {
      local_4e1 = (char)*(int *)((int)local_30 + 0x44);
    }
    ppppuVar8 = (undefined4 ****)FUN_01624950(&local_2c,&local_4e1);
    if (&local_6c != ppppuVar8) {
      FUN_00f1a420(uVar7);
      local_6c = *ppppuVar8;
      ppuStack_68 = ppppuVar8[1];
      ppuStack_64 = ppppuVar8[2];
      ppuStack_60 = ppppuVar8[3];
      local_5c = *(undefined8 *)(ppppuVar8 + 4);
      ppppuVar8[4] = (undefined4 ***)0x0;
      ppppuVar8[5] = (undefined4 ***)0xf;
      *(undefined1 *)ppppuVar8 = 0;
    }
    FUN_00f1a420(uVar7);
  }
  local_8 = 1;
  memset(local_4d8,0,0x40c);
  FUN_0169ab10();
  local_8._0_1_ = 2;
  ppppuVar8 = &local_6c;
  if (0xf < local_5c._4_4_) {
    ppppuVar8 = (undefined4 ****)local_6c;
  }
  cVar6 = FUN_0169c4f0(ppppuVar8);
  if (cVar6 == '\0') goto LAB_01353711;
  piVar2 = (int *)*local_37c;
  piVar1 = piVar2 + 1;
  if (piVar2 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
    UNLOCK();
  }
  local_8._0_1_ = 3;
  if (piVar2 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
    UNLOCK();
  }
  local_3c = piVar2;
  FUN_01487b70(piVar2,2,0);
  FUN_016a0910(piVar2,0);
  FUN_01692d50();
  FUN_01692c40(0,1);
  *(undefined4 *)(param_1 + 0x124) = 0;
  FUN_0108ed50(local_cc,param_2);
  local_8._0_1_ = 4;
  pcVar3 = *(code **)(*piVar2 + 0x5c);
  local_4dc = FUN_01694ab0(s_sealmaster_bg_01af1eb4);
  local_8._0_1_ = 5;
  uVar9 = (*pcVar3)(&local_4dc);
  local_8._0_1_ = 6;
  if (local_4dc != DAT_01a1e870) {
    piVar1 = (int *)(local_4dc + -8);
    LOCK();
    iVar11 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar11 == 2) {
      FUN_01694dd0(&local_4dc,*(undefined2 *)(local_4dc + -2));
    }
  }
  local_8._0_1_ = 4;
  FUN_01353750(uVar9,local_cc);
  ppuStack_50 = (undefined4 ***)0x0;
  ppuStack_4c = (undefined4 ***)0x0;
  ppuStack_48 = (undefined4 ***)0x0;
  local_44 = 0xf00000000;
  local_4e0 = *(float **)(param_1 + 0xa8);
  local_54 = (undefined4 ****)0x0;
  pfVar10 = local_4e0 + 5;
  local_8 = CONCAT31(local_8._1_3_,7);
  local_4dc = 2;
  FUN_00f1b390(&local_38,param_2);
  if (((*(char *)((int)local_30 + 0xd) == '\0') && (*(uint *)((int)local_30 + 0x10) <= *param_2)) &&
     ((local_30 != *pfVar10 && (local_4e0[0x17] != 0.0)))) {
    local_4e1 = 0;
    if (*(int *)((int)local_30 + 0x44) != -1) {
      local_4e1 = (char)*(int *)((int)local_30 + 0x44);
    }
    ppppuVar8 = (undefined4 ****)FUN_01624810(&local_2c,&local_4e1);
    if (&local_54 != ppppuVar8) {
      FUN_00f1a420();
      local_54 = *ppppuVar8;
      ppuStack_50 = ppppuVar8[1];
      ppuStack_4c = ppppuVar8[2];
      ppuStack_48 = ppppuVar8[3];
      local_44 = *(undefined8 *)(ppppuVar8 + 4);
      ppppuVar8[4] = (undefined4 ***)0x0;
      ppppuVar8[5] = (undefined4 ***)0xf;
      *(undefined1 *)ppppuVar8 = 0;
    }
    FUN_00f1a420();
  }
  local_8._0_1_ = 8;
  local_8._1_3_ = 0;
  pcVar3 = *(code **)(*piVar2 + 0x5c);
  local_4dc = FUN_01694ab0(s_sealmaster_blind_01af1ea0);
  local_8._0_1_ = 9;
  iVar11 = (*pcVar3)(&local_4dc);
  local_8._0_1_ = 10;
  if (local_4dc != DAT_01a1e870) {
    piVar1 = (int *)(local_4dc + -8);
    LOCK();
    iVar12 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar12 == 2) {
      FUN_01694dd0(&local_4dc,*(undefined2 *)(local_4dc + -2));
    }
  }
  local_8._0_1_ = 8;
  if (iVar11 != 0) {
    iVar11 = FUN_01691e40(8);
    if (iVar11 != 0) {
      ppppuVar8 = &local_54;
      if (0xf < local_44._4_4_) {
        ppppuVar8 = (undefined4 ****)local_54;
      }
      local_4dc = FUN_01694ab0(ppppuVar8);
      local_8._0_1_ = 0xb;
      local_4e0 = (float *)FUN_0168b4c0(&local_4dc,&DAT_01bb9b20,0,0);
      local_8._0_1_ = 0xc;
      if (local_4dc != DAT_01a1e870) {
        piVar1 = (int *)(local_4dc + -8);
        LOCK();
        iVar11 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar11 == 2) {
          FUN_01694dd0(&local_4dc,*(undefined2 *)(local_4dc + -2));
        }
      }
      local_8._0_1_ = 8;
      FUN_00f4ca80(local_4e0);
    }
  }
  uStack_28 = 0;
  uStack_24 = 0;
  uStack_20 = 0;
  local_1c = 0;
  uStack_18 = 0xf;
  iVar11 = *(int *)(param_1 + 0xa8);
  local_2c = (undefined4 ****)0x0;
  local_4e0 = (float *)(iVar11 + 0x14);
  local_8 = CONCAT31(local_8._1_3_,0xd);
  FUN_00f1b390(&local_38,param_2);
  if (((*(char *)((int)local_30 + 0xd) == '\0') && (*(uint *)((int)local_30 + 0x10) <= *param_2)) &&
     ((local_30 != *local_4e0 && (*(int *)(iVar11 + 0x68) != 0)))) {
    local_4e0 = *(float **)((int)local_30 + 0x48);
    iVar12 = FUN_015d6b70(&local_4e0);
    if ((iVar12 == 0) || (*(int *)(iVar11 + 0x5c) == 0)) goto LAB_013531f3;
    FUN_01624b20(&local_9c,iVar12 + 0x42);
  }
  else {
LAB_013531f3:
    local_9c = local_2c;
    local_2c = (undefined4 ***)((uint)local_2c & 0xffffff00);
    uStack_98 = uStack_28;
    uStack_94 = uStack_24;
    uStack_90 = uStack_20;
    local_8c = CONCAT44(uStack_18,local_1c);
    uStack_18 = 0xf;
    local_1c = 0;
  }
  FUN_00f1a420();
  local_8._0_1_ = 0xe;
  pcVar3 = *(code **)(*piVar2 + 0x5c);
  local_4dc = FUN_01694ab0(s_sealmaster_elemental_01af1ed4);
  local_8._0_1_ = 0xf;
  iVar11 = (*pcVar3)(&local_4dc);
  local_8._0_1_ = 0x10;
  if (local_4dc != DAT_01a1e870) {
    piVar1 = (int *)(local_4dc + -8);
    LOCK();
    iVar12 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar12 == 2) {
      FUN_01694dd0(&local_4dc,*(undefined2 *)(local_4dc + -2));
    }
  }
  local_8._0_1_ = 0xe;
  if (iVar11 != 0) {
    iVar11 = FUN_01691e40(8);
    if (iVar11 != 0) {
      ppppuVar8 = &local_9c;
      if (0xf < local_8c._4_4_) {
        ppppuVar8 = (undefined4 ****)local_9c;
      }
      local_4dc = FUN_01694ab0(ppppuVar8);
      local_8._0_1_ = 0x11;
      local_4e0 = (float *)FUN_0168b4c0(&local_4dc,&DAT_01bb9b20,0,0);
      local_8._0_1_ = 0x12;
      if (local_4dc != DAT_01a1e870) {
        piVar1 = (int *)(local_4dc + -8);
        LOCK();
        iVar11 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar11 == 2) {
          FUN_01694dd0(&local_4dc,*(undefined2 *)(local_4dc + -2));
        }
      }
      local_8._0_1_ = 0xe;
      FUN_00f4ca80(local_4e0);
    }
  }
  uStack_28 = 0;
  uStack_24 = 0;
  uStack_20 = 0;
  local_1c = 0;
  uStack_18 = 0xf;
  iVar11 = *(int *)(param_1 + 0xa8);
  local_2c = (undefined4 ****)0x0;
  local_4e0 = (float *)(iVar11 + 0x14);
  local_8 = CONCAT31(local_8._1_3_,0x13);
  FUN_00f1b390(&local_38,param_2);
  if ((((*(char *)((int)local_30 + 0xd) == '\0') && (*(uint *)((int)local_30 + 0x10) <= *param_2))
      && (local_30 != *local_4e0)) && (*(int *)(iVar11 + 0x68) != 0)) {
    local_4e0 = *(float **)((int)local_30 + 0x48);
    iVar12 = FUN_015d6b70(&local_4e0);
    if ((iVar12 == 0) || (*(int *)(iVar11 + 0x5c) == 0)) goto LAB_013533d3;
    FUN_01624330(&local_84,iVar12 + 0x3a);
  }
  else {
LAB_013533d3:
    local_84 = local_2c;
    local_2c = (undefined4 ***)((uint)local_2c & 0xffffff00);
    uStack_80 = uStack_28;
    uStack_7c = uStack_24;
    uStack_78 = uStack_20;
    local_74 = CONCAT44(uStack_18,local_1c);
    uStack_18 = 0xf;
    local_1c = 0;
  }
  FUN_00f1a420();
  local_8._0_1_ = 0x14;
  pcVar3 = *(code **)(*piVar2 + 0x5c);
  local_4dc = FUN_01694ab0(s_sealmaster_type_01af1ec4);
  local_8._0_1_ = 0x15;
  iVar11 = (*pcVar3)(&local_4dc);
  local_8._0_1_ = 0x16;
  if (local_4dc != DAT_01a1e870) {
    piVar1 = (int *)(local_4dc + -8);
    LOCK();
    iVar12 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar12 == 2) {
      FUN_01694dd0(&local_4dc,*(undefined2 *)(local_4dc + -2));
    }
  }
  local_8._0_1_ = 0x14;
  uVar5 = (undefined1)local_8;
  local_8._0_1_ = 0x14;
  if (iVar11 != 0) {
    iVar11 = FUN_01691e40(8);
    uVar5 = (undefined1)local_8;
    if (iVar11 != 0) {
      ppppuVar8 = &local_84;
      if (0xf < local_74._4_4_) {
        ppppuVar8 = (undefined4 ****)local_84;
      }
      local_4dc = FUN_01694ab0(ppppuVar8);
      local_8._0_1_ = 0x17;
      local_4e0 = (float *)FUN_0168b4c0(&local_4dc,&DAT_01bb9b20,0,0);
      local_8._0_1_ = 0x18;
      if (local_4dc != DAT_01a1e870) {
        piVar1 = (int *)(local_4dc + -8);
        LOCK();
        iVar11 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar11 == 2) {
          FUN_01694dd0(&local_4dc,*(undefined2 *)(local_4dc + -2));
        }
      }
      local_8._0_1_ = 0x14;
      FUN_00f4ca80(local_4e0);
      uVar5 = (undefined1)local_8;
    }
  }
  local_8._0_1_ = uVar5;
  FUN_0108ee60(local_b4,param_2);
  local_8._0_1_ = 0x19;
  pcVar3 = *(code **)(*piVar2 + 0x5c);
  local_4dc = FUN_01694ab0(s_sealmaster_monster_01af1f04);
  local_8._0_1_ = 0x1a;
  uVar9 = (*pcVar3)(&local_4dc);
  local_8._0_1_ = 0x1b;
  if (local_4dc != DAT_01a1e870) {
    piVar1 = (int *)(local_4dc + -8);
    LOCK();
    iVar11 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar11 == 2) {
      FUN_01694dd0(&local_4dc,*(undefined2 *)(local_4dc + -2));
    }
  }
  local_8._0_1_ = 0x19;
  FUN_01353750(uVar9,local_b4);
  local_4e0 = (float *)FUN_016830c0(0x54);
  local_8._0_1_ = 0x1c;
  if (local_4e0 == (float *)0x0) {
    uVar9 = 0;
  }
  else {
    memset(local_4e0,0,0x54);
    uVar9 = FUN_0125b110();
  }
  local_8 = CONCAT31(local_8._1_3_,0x19);
  *(undefined4 *)(param_1 + 0x108) = uVar9;
  cVar6 = FUN_0125b300(0,DAT_01bc07ec,DAT_01bc07f0);
  if (cVar6 == '\0') {
    if (*(undefined4 **)(param_1 + 0x108) != (undefined4 *)0x0) {
      (**(code **)**(undefined4 **)(param_1 + 0x108))(1);
      *(undefined4 *)(param_1 + 0x108) = 0;
    }
  }
  else {
    *(undefined4 *)(*(int *)(param_1 + 0x108) + 0x50) = 0xc0300000;
  }
  piVar1 = DAT_01bfb32c;
  *(undefined1 *)(param_1 + 0xe8) = 1;
  if (piVar1 != (int *)0x0) {
    cVar6 = (**(code **)(*piVar1 + 4))(s_System_SealMaster_wav_01af1eec);
    if (cVar6 != '\0') {
      (**(code **)(*DAT_01bfb32c + 8))(s_System_SealMaster_wav_01af1eec);
    }
    (**(code **)*DAT_01bfb32c)(s_System_SealMaster_wav_01af1eec);
  }
  local_38 = DAT_01beef18;
  uVar4 = local_38;
  local_30 = DAT_01beef20 + *(float *)(*(int *)(DAT_01bfc3b4 + 0x1b0) + 0x48);
  iVar11 = *(int *)(*(int *)(DAT_01bfc3b4 + 0x1b0) + 8);
  local_38._0_4_ = (float)DAT_01beef18;
  if (((*(float *)(iVar11 + 0x50) != (float)local_38) ||
      (local_38._4_4_ = (float)((ulonglong)DAT_01beef18 >> 0x20),
      *(float *)(iVar11 + 0x54) != local_38._4_4_)) || (*(float *)(iVar11 + 0x58) != local_30)) {
    *(undefined8 *)(iVar11 + 0x50) = DAT_01beef18;
    *(float *)(iVar11 + 0x58) = local_30;
  }
  local_38 = uVar4;
  FUN_00f1a420();
  FUN_00f1a420();
  FUN_00f1a420();
  FUN_00f1a420();
  FUN_00f1a420();
  local_8._0_1_ = 0x1d;
  if (piVar2 != (int *)0x0) {
    LOCK();
    piVar1 = piVar2 + 1;
    iVar11 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar11 == 1) {
      (**(code **)(*piVar2 + 4))();
    }
  }
LAB_01353711:
  FUN_0169b2f0();
  FUN_00f1a420();
  ExceptionList = local_10;
  return;
}


