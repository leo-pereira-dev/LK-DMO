
/* ============================================================ */
/* Function: FUN_00f808f0 */
/* Entry: 00f808f0 */
/* Signature: undefined FUN_00f808f0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00f808f0(int param_1)

{
  float fVar1;
  float fVar2;
  float *pfVar3;
  code *pcVar4;
  uint uVar5;
  undefined1 *puVar6;
  char cVar7;
  int iVar8;
  undefined4 uVar9;
  int *piVar10;
  float *pfVar11;
  float fVar12;
  float fVar13;
  undefined8 uStack_4f0;
  undefined4 *puStack_4e8;
  float **ppfStack_4e4;
  float fStack_4e0;
  undefined8 uStack_4dc;
  float fStack_4d4;
  float local_470;
  float *local_46c;
  undefined1 local_468 [348];
  int *local_30c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  float local_2c;
  float *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  iVar8 = DAT_01bfb2c4;
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0196a5a6;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  *(int *)(param_1 + 0x9c) = DAT_01bfc3b4[0x46];
  *(int *)(param_1 + 0xa0) = DAT_01bfc3b4[0x47];
  *(undefined1 *)(param_1 + 0x98) = *(undefined1 *)(iVar8 + 0x1a4);
  *(undefined1 *)(iVar8 + 0x1a4) = 0;
  puVar6 = &stack0xfffffffc;
  if (DAT_01bfb2a0 != 0) {
    FUN_00f54dc0();
    puVar6 = puStack_20;
  }
  puStack_20 = puVar6;
  iVar8 = DAT_01bfb2a0;
  DAT_01bb97a0 = 0;
  *(undefined4 *)(DAT_01bfb2d8 + 0x58) = 3;
  if (iVar8 != 0) {
    FUN_00f547c0();
  }
  local_28 = (float *)0x3e8;
  FUN_00f1b390();
  FUN_01467b00();
  cVar7 = FUN_01293d20();
  if (cVar7 != '\0') {
    local_28 = (float *)0x2328;
    FUN_00f1b390();
    FUN_012eba00();
  }
  cVar7 = FUN_01293d20();
  if (cVar7 != '\0') {
    local_46c = (float *)(DAT_01bfb8cc + 0x40);
    local_28 = (float *)0x44c;
    FUN_00f1b390();
    if ((((*(char *)((int)local_2c + 0xd) == '\0') && (*(uint *)((int)local_2c + 0x10) < 0x44d)) &&
        (local_2c != *local_46c)) &&
       ((piVar10 = *(int **)((int)local_2c + 0x14), piVar10 != (int *)0x0 &&
        (cVar7 = (**(code **)(*piVar10 + 0x24))(), cVar7 == '\x01')))) {
      (**(code **)(*piVar10 + 0x44))();
    }
  }
  cVar7 = FUN_01293d20();
  if (cVar7 != '\0') {
    local_46c = (float *)(DAT_01bfb8cc + 0x40);
    local_28 = (float *)0x12c;
    FUN_00f1b390();
    if (((*(char *)((int)local_2c + 0xd) == '\0') && (*(uint *)((int)local_2c + 0x10) < 0x12d)) &&
       ((local_2c != *local_46c &&
        ((piVar10 = *(int **)((int)local_2c + 0x14), piVar10 != (int *)0x0 &&
         (cVar7 = (**(code **)(*piVar10 + 0x24))(), cVar7 == '\x01')))))) {
      (**(code **)(*piVar10 + 0x44))();
    }
  }
  if ((DAT_01bfb8cc != 0) && (cVar7 = FUN_01293d20(), cVar7 != '\0')) {
    local_28 = (float *)0x3;
    FUN_014414a0();
  }
  local_48 = 0;
  uStack_44 = 0;
  uStack_40 = 0;
  uStack_3c = 0;
  local_38 = 0;
  local_14 = 0;
  local_28 = (float *)0x0;
  local_46c = (float *)0x0;
  FUN_00f5dab0();
  FUN_00f5dab0();
  FUN_014414a0();
  if ((DAT_01bfb8cc != 0) && (cVar7 = FUN_01293d20(), cVar7 != '\0')) {
    fStack_4d4 = 1.24716e-43;
    uStack_4dc = CONCAT44(0xf80bfd,(char *)uStack_4dc);
    iVar8 = FUN_012945f0();
    if (iVar8 != 0) {
      *(undefined1 *)(iVar8 + 0x68) = 0;
    }
  }
  iVar8 = DAT_01bfb2a8;
  *(undefined4 *)(param_1 + 200) = 0;
  *(undefined4 *)(*(int *)(iVar8 + 8) + 0x8bc) = 0;
  (**(code **)(**(int **)(iVar8 + 8) + 0x70))();
  (**(code **)(**(int **)(DAT_01bfb2a8 + 0x14) + 0x70))();
  fVar2 = DAT_01b02b3c;
  fStack_4d4 = 2.277965e-38;
  memset(local_468,0,0x40c);
  FUN_0169ab10();
  local_14._0_1_ = 1;
  cVar7 = FUN_0169c4f0();
  if (cVar7 == '\0') {
    fStack_4d4 = 2.2779726e-38;
    FUN_015aa730();
    goto LAB_00f813e3;
  }
  pfVar11 = (float *)*local_30c;
  pfVar3 = pfVar11 + 1;
  if (pfVar11 != (float *)0x0) {
    LOCK();
    *pfVar3 = (float)((int)*pfVar3 + 1);
    UNLOCK();
  }
  local_14 = CONCAT31(local_14._1_3_,2);
  fStack_4d4 = 2.2779806e-38;
  local_28 = pfVar11;
  FUN_01489f70();
  if (pfVar11 != (float *)0x0) {
    LOCK();
    *pfVar3 = (float)((int)*pfVar3 + 1);
    UNLOCK();
  }
  fStack_4d4 = 2.2779853e-38;
  FUN_01487b70();
  iVar8 = *(int *)(param_1 + 0x1c);
  *(undefined8 *)(iVar8 + 0x50) = 0;
  *(float *)(iVar8 + 0x58) = fVar2;
  FUN_016a0910();
  FUN_01692d50();
  FUN_01692c40();
  cVar7 = FUN_0169c4f0();
  if (cVar7 == '\0') {
LAB_00f80e66:
    fStack_4d4 = 2.2780386e-38;
    FUN_015aa730();
  }
  else {
    local_46c = (float *)*local_30c;
    if (pfVar11 != local_46c) {
      if (pfVar11 != (float *)0x0) {
        LOCK();
        fVar13 = *pfVar3;
        *pfVar3 = (float)((int)*pfVar3 + -1);
        UNLOCK();
        if (fVar13 == 1.4013e-45) {
          (**(code **)((int)*pfVar11 + 4))();
        }
      }
      pfVar11 = local_46c;
      local_28 = local_46c;
      if (local_46c != (float *)0x0) {
        LOCK();
        local_46c[1] = (float)((int)local_46c[1] + 1);
        UNLOCK();
      }
    }
    fStack_4d4 = 2.2780129e-38;
    FUN_01489f70();
    local_46c = pfVar11 + 1;
    if (pfVar11 != (float *)0x0) {
      LOCK();
      *local_46c = (float)((int)*local_46c + 1);
      UNLOCK();
    }
    fStack_4d4 = 2.278019e-38;
    FUN_01487b70();
    iVar8 = *(int *)(param_1 + 0x38);
    *(undefined8 *)(iVar8 + 0x50) = 0;
    *(float *)(iVar8 + 0x58) = fVar2;
    FUN_016a0910();
    local_2c = 1.0;
    fStack_4d4 = 2.2780329e-38;
    FUN_0148a890();
    cVar7 = FUN_0169c4f0();
    if (cVar7 == '\0') goto LAB_00f80e66;
    pfVar3 = (float *)*local_30c;
    if (pfVar11 != pfVar3) {
      if (pfVar11 != (float *)0x0) {
        LOCK();
        fVar13 = *local_46c;
        *local_46c = (float)((int)*local_46c + -1);
        UNLOCK();
        if (fVar13 == 1.4013e-45) {
          (**(code **)((int)*pfVar11 + 4))();
        }
      }
      pfVar11 = pfVar3;
      local_28 = pfVar3;
      if (pfVar3 != (float *)0x0) {
        LOCK();
        pfVar3[1] = (float)((int)pfVar3[1] + 1);
        UNLOCK();
      }
    }
    fStack_4d4 = 2.2780501e-38;
    FUN_01489f70();
    if (pfVar11 != (float *)0x0) {
      LOCK();
      pfVar11[1] = (float)((int)pfVar11[1] + 1);
      UNLOCK();
    }
    fStack_4d4 = 2.278054e-38;
    FUN_01487b70();
    iVar8 = *(int *)(param_1 + 0x50);
    *(undefined8 *)(iVar8 + 0x50) = 0;
    *(float *)(iVar8 + 0x58) = fVar2;
    FUN_016a0910();
    local_2c = 1.0;
    fStack_4d4 = 2.2780664e-38;
    FUN_0148a890();
    iVar8 = DAT_01bfc3b4[2];
    local_2c = (float)DAT_01bfc3b4[0x57] - _DAT_01b02aac;
    if (((*(float *)(iVar8 + 0x50) != DAT_01b02b30) || (*(float *)(iVar8 + 0x54) != DAT_01b02b34))
       || (*(float *)(iVar8 + 0x58) != local_2c)) {
      *(ulonglong *)(iVar8 + 0x50) = CONCAT44(DAT_01b02b34,DAT_01b02b30);
      *(float *)(iVar8 + 0x58) = local_2c;
    }
    FUN_0147ab50();
    piVar10 = DAT_01bfc3b4;
    fVar12 = DAT_01b029d4;
    fVar13 = (float)DAT_01bfc3b4[0x4a];
    *(undefined1 *)(DAT_01bfc3b4 + 0x48) = 1;
    piVar10[0x46] = 0x43960000;
    piVar10[0x47] = 0x44fa0052;
    uVar5 = DAT_01b03420;
    if ((fVar12 <= fVar13) && (fVar12 = DAT_01b02a54, fVar13 <= DAT_01b02a54)) {
      fVar12 = fVar13;
    }
    iVar8 = *piVar10;
    piVar10[0x4a] = (int)fVar12;
    *(ulonglong *)(iVar8 + 0x50) = (ulonglong)((uint)fVar12 ^ uVar5) << 0x20;
    fVar13 = DAT_01b02a50;
    *(undefined4 *)(iVar8 + 0x58) = 0;
    piVar10[0x4d] = piVar10[0x4a];
    DAT_01bfc3b4[0x57] = 0x42f00000;
    piVar10 = DAT_01bfc3b4;
    fVar12 = fVar13;
    if ((((char)DAT_01bfc3b4[0x48] == '\x01') &&
        (fVar12 = (float)DAT_01bfc3b4[0x46], (float)DAT_01bfc3b4[0x46] <= fVar13)) &&
       (fVar12 = (float)DAT_01bfc3b4[0x47], fVar13 <= (float)DAT_01bfc3b4[0x47])) {
      fVar12 = fVar13;
    }
    DAT_01bfc3b4[0x4a] = (int)fVar12;
    iVar8 = *piVar10;
    local_2c = 0.0;
    *(ulonglong *)(iVar8 + 0x50) = (ulonglong)((uint)fVar12 ^ uVar5) << 0x20;
    *(undefined4 *)(iVar8 + 0x58) = 0;
    FUN_01692c40();
    fVar12 = DAT_01b02a50;
    *(undefined4 *)(param_1 + 100) = 0x3e6147ae;
    *(undefined4 *)(param_1 + 0x68) = 0x3ca3d70a;
    *(undefined4 *)(param_1 + 0x60) = 0x44fa0000;
    DAT_01bfc3b4[0x4d] = 0x44fa0000;
    fVar13 = (float)DAT_01bfc3b4[0x46];
    if ((fVar12 < fVar13) || (fVar13 = (float)DAT_01bfc3b4[0x47], fVar13 < fVar12)) {
      DAT_01bfc3b4[0x4d] = (int)fVar13;
    }
    *(undefined4 *)(param_1 + 0x20) = 0;
    local_4c = 0x3f800000;
    local_58 = 0x1001;
    local_54 = 0x110;
    local_50 = 2;
    pcVar4 = *(code **)(**(int **)(param_1 + 0x1c) + 0x5c);
    local_46c = (float *)FUN_01694ab0();
    local_14._0_1_ = 3;
    iVar8 = (*pcVar4)();
    local_14._0_1_ = 4;
    if (local_46c != DAT_01a1e870) {
      pfVar3 = local_46c + -2;
      LOCK();
      fVar13 = *pfVar3;
      *pfVar3 = (float)((int)*pfVar3 + -1);
      UNLOCK();
      if (fVar13 == 2.8026e-45) {
        fStack_4d4 = 2.2781548e-38;
        FUN_01694dd0();
      }
    }
    local_14._0_1_ = 2;
    local_470 = DAT_01b029e8;
    if (iVar8 != 0) {
      local_470 = *(float *)(iVar8 + 0x28) + *(float *)(iVar8 + 0x28);
    }
    fVar13 = *(float *)(iVar8 + 0x50);
    fVar12 = *(float *)(iVar8 + 0x54);
    local_2c = (fVar2 + *(float *)(iVar8 + 0x58)) - DAT_01b029a0;
    uVar9 = (**(code **)(**(int **)(DAT_01bfb2a8 + 0x14) + 0x160))();
    uStack_4dc = CONCAT44(fVar12 + 0.0,fVar13 + 0.0);
    fStack_4e0 = local_2c;
    puStack_4e8 = &local_58;
    fStack_4d4 = local_2c;
    uStack_4f0 = CONCAT44(0xf81258,(undefined4)uStack_4f0);
    ppfStack_4e4 = (float **)uVar9;
    piVar10 = (int *)FUN_00f5c3f0();
    *(int **)(param_1 + 0xc0) = piVar10;
    fVar13 = (float)piVar10[0x61];
    fVar12 = (float)piVar10[0x39];
    fStack_4d4 = 2.2781877e-38;
    (**(code **)(*piVar10 + 0x68))();
    fStack_4d4 = 1.0;
    uStack_4dc._4_4_ = 0xdbfed;
    uStack_4dc._0_4_ = (char *)0xf812b6;
    (**(code **)(**(int **)(param_1 + 0xc0) + 0xa0))();
    local_50 = 3;
    uStack_4dc._0_4_ = s_Digimon_Box02_01ac8ec4;
    pcVar4 = *(code **)(**(int **)(param_1 + 0x1c) + 0x5c);
    fStack_4e0 = 2.2781959e-38;
    local_46c = (float *)FUN_01694ab0();
    uStack_4dc = CONCAT44(uStack_4dc._4_4_,&local_46c);
    local_14._0_1_ = 5;
    fStack_4e0 = 2.2781998e-38;
    iVar8 = (*pcVar4)();
    local_14._0_1_ = 6;
    if (local_46c != DAT_01a1e870) {
      pfVar3 = local_46c + -2;
      LOCK();
      fVar1 = *pfVar3;
      *pfVar3 = (float)((int)*pfVar3 + -1);
      UNLOCK();
      if (fVar1 == 2.8026e-45) {
        ppfStack_4e4 = &local_46c;
        puStack_4e8 = (undefined4 *)0xf8132c;
        fStack_4e0 = (float)(uint)*(ushort *)((int)local_46c + -2);
        FUN_01694dd0();
      }
    }
    local_14 = CONCAT31(local_14._1_3_,2);
    ppfStack_4e4 = (float **)0x0;
    puStack_4e8 = (undefined4 *)((fVar2 + *(float *)(iVar8 + 0x58)) - DAT_01b029a0);
    uStack_4f0 = CONCAT44(*(float *)(iVar8 + 0x54) + 0.0,*(float *)(iVar8 + 0x50) + 0.0);
    local_2c = (float)puStack_4e8;
    piVar10 = (int *)FUN_00f5c3f0(&local_58,uVar9,&uStack_4f0);
    fStack_4e0 = 1.4013e-45;
    *(int **)(param_1 + 0xc4) = piVar10;
    puStack_4e8 = (undefined4 *)0xf813ae;
    ppfStack_4e4 = (float **)(local_470 / (fVar13 * fVar12));
    (**(code **)(*piVar10 + 0x68))();
    puStack_4e8 = (undefined4 *)0x3f800000;
    uStack_4f0 = 0xdbfed00f813c9;
    (**(code **)(**(int **)(param_1 + 0xc4) + 0xa0))();
  }
  local_14._0_1_ = 7;
  if (pfVar11 != (float *)0x0) {
    LOCK();
    pfVar3 = pfVar11 + 1;
    fVar2 = *pfVar3;
    *pfVar3 = (float)((int)*pfVar3 + -1);
    UNLOCK();
    if (fVar2 == 1.4013e-45) {
      (**(code **)((int)*pfVar11 + 4))();
    }
  }
LAB_00f813e3:
  FUN_0169b2f0();
  FUN_00f1e050();
  ExceptionList = local_1c;
  return;
}


