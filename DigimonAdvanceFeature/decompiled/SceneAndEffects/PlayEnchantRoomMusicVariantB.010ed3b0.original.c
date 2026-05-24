
/* ============================================================ */
/* Function: FUN_010ed3b0 */
/* Entry: 010ed3b0 */
/* Signature: undefined FUN_010ed3b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_010ed3b0(int param_1)

{
  int *piVar1;
  float fVar2;
  int *piVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  undefined1 *puVar7;
  char cVar8;
  int iVar9;
  char *pcVar10;
  int *piVar11;
  float fVar12;
  undefined1 local_448 [348];
  int *local_2ec;
  int *local_34;
  float local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  iVar9 = DAT_01bfb2d8;
  iVar6 = DAT_01bfb2c4;
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019838f8;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  *(undefined4 *)(param_1 + 0xcc) = 0;
  *(undefined4 *)(param_1 + 0xd0) = 0;
  *(undefined4 *)(iVar9 + 0x58) = 4;
  *(int *)(param_1 + 0x88) = DAT_01bfc3b4[0x46];
  *(int *)(param_1 + 0x8c) = DAT_01bfc3b4[0x47];
  *(undefined1 *)(param_1 + 0x84) = *(undefined1 *)(iVar6 + 0x1a4);
  iVar9 = DAT_01bfb2a0;
  *(undefined1 *)(iVar6 + 0x1a4) = 0;
  puVar7 = &stack0xfffffffc;
  if (iVar9 != 0) {
    FUN_00f54dc0(local_24);
    iVar9 = DAT_01bfb2a0;
    puVar7 = puStack_20;
  }
  puStack_20 = puVar7;
  DAT_01bb97a0 = 0;
  if (iVar9 != 0) {
    FUN_00f547c0(s_Enchant_Room_mp3_01ac8de8);
  }
  iVar9 = DAT_01bfb2a8;
  *(undefined4 *)(*(int *)(DAT_01bfb2a8 + 8) + 0x8bc) = 0;
  (**(code **)(**(int **)(iVar9 + 8) + 0x70))();
  (**(code **)(**(int **)(DAT_01bfb2a8 + 0x14) + 0x70))();
  local_28 = -100000.0;
  *(undefined8 *)(param_1 + 0x6c) = 0;
  *(undefined4 *)(param_1 + 0x74) = 0xc7c35000;
  memset(local_448,0,0x40c);
  FUN_0169ab10();
  local_14 = 0;
  cVar8 = FUN_0169c4f0(s_Data_etcobject_DatsCenter_F_DATS_01ac8dfc);
  if (cVar8 == '\0') {
    FUN_015aa730(0,&DAT_01ac8e28,s_Data_etcobject_DatsCenter_F_DATS_01ac8dfc);
    goto LAB_010eda7e;
  }
  piVar11 = (int *)*local_2ec;
  piVar1 = piVar11 + 1;
  if (piVar11 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
    UNLOCK();
  }
  local_14 = CONCAT31(local_14._1_3_,1);
  local_34 = piVar11;
  FUN_01489f70(piVar11,1,0);
  if (piVar11 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
    UNLOCK();
  }
  FUN_01487b70(piVar11,0);
  iVar9 = *(int *)(param_1 + 0x1c);
  *(undefined8 *)(iVar9 + 0x50) = *(undefined8 *)(param_1 + 0x6c);
  *(undefined4 *)(iVar9 + 0x58) = *(undefined4 *)(param_1 + 0x74);
  FUN_016a0910();
  FUN_01692d50();
  FUN_01692c40();
  cVar8 = FUN_0169c4f0(s_Data_etcobject_DatsCenter_E_Grad_01ad7020);
  if (cVar8 == '\0') {
    pcVar10 = s_Data_etcobject_DatsCenter_E_Grad_01ad7020;
LAB_010ed7b6:
    FUN_015aa730(0,&DAT_01ac8e28,pcVar10);
  }
  else {
    piVar3 = (int *)*local_2ec;
    if (piVar11 != piVar3) {
      if (piVar11 != (int *)0x0) {
        LOCK();
        iVar9 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar9 == 1) {
          (**(code **)(*piVar11 + 4))();
        }
      }
      piVar11 = piVar3;
      local_34 = piVar3;
      if (piVar3 != (int *)0x0) {
        LOCK();
        piVar3[1] = piVar3[1] + 1;
        UNLOCK();
      }
    }
    FUN_01489f70(piVar11,1,0);
    piVar1 = piVar11 + 1;
    if (piVar11 != (int *)0x0) {
      LOCK();
      *piVar1 = *piVar1 + 1;
      UNLOCK();
    }
    FUN_01487b70(piVar11,0,0);
    iVar9 = *(int *)(param_1 + 0x38);
    *(undefined8 *)(iVar9 + 0x50) = *(undefined8 *)(param_1 + 0x6c);
    *(undefined4 *)(iVar9 + 0x58) = *(undefined4 *)(param_1 + 0x74);
    FUN_016a0910();
    uVar4 = DAT_01b02768;
    local_28 = 1.0;
    FUN_0148a890(CONCAT44(DAT_01b02768,DAT_01b02768),0x3f800000);
    cVar8 = FUN_0169c4f0(s_Data_etcobject_DatsCenter_E_Grad_01ad706c);
    if (cVar8 == '\0') {
      pcVar10 = s_Data_etcobject_DatsCenter_E_Grad_01ad706c;
      goto LAB_010ed7b6;
    }
    piVar3 = (int *)*local_2ec;
    if (piVar11 != piVar3) {
      if (piVar11 != (int *)0x0) {
        LOCK();
        iVar9 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar9 == 1) {
          (**(code **)(*piVar11 + 4))();
        }
      }
      piVar11 = piVar3;
      local_34 = piVar3;
      if (piVar3 != (int *)0x0) {
        LOCK();
        piVar3[1] = piVar3[1] + 1;
        UNLOCK();
      }
    }
    FUN_01489f70(piVar11,1,0);
    piVar1 = piVar11 + 1;
    if (piVar11 != (int *)0x0) {
      LOCK();
      *piVar1 = *piVar1 + 1;
      UNLOCK();
    }
    FUN_01487b70(piVar11,0,0);
    iVar9 = *(int *)(param_1 + 0x50);
    *(undefined8 *)(iVar9 + 0x50) = *(undefined8 *)(param_1 + 0x6c);
    *(undefined4 *)(iVar9 + 0x58) = *(undefined4 *)(param_1 + 0x74);
    FUN_016a0910();
    local_28 = 1.0;
    FUN_0148a890(CONCAT44(uVar4,uVar4),0x3f800000);
    cVar8 = FUN_0169c4f0(s_Data_etcobject_DatsCenter_D_Grad_01ac8e84);
    if (cVar8 == '\0') {
      pcVar10 = s_Data_etcobject_DatsCenter_D_Grad_01ac8e84;
      goto LAB_010ed7b6;
    }
    piVar3 = (int *)*local_2ec;
    if (piVar11 != piVar3) {
      if (piVar11 != (int *)0x0) {
        LOCK();
        iVar9 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar9 == 1) {
          (**(code **)(*piVar11 + 4))();
        }
      }
      piVar11 = piVar3;
      local_34 = piVar3;
      if (piVar3 != (int *)0x0) {
        LOCK();
        piVar3[1] = piVar3[1] + 1;
        UNLOCK();
      }
    }
    FUN_01489f70(piVar11,1,0);
    if (piVar11 != (int *)0x0) {
      LOCK();
      piVar11[1] = piVar11[1] + 1;
      UNLOCK();
    }
    FUN_01487b70(piVar11,0,0);
    iVar9 = *(int *)(param_1 + 0x68);
    *(undefined8 *)(iVar9 + 0x50) = *(undefined8 *)(param_1 + 0x6c);
    *(undefined4 *)(iVar9 + 0x58) = *(undefined4 *)(param_1 + 0x74);
    FUN_016a0910();
    local_28 = 1.0;
    FUN_0148a890(CONCAT44(uVar4,uVar4),0x3f800000);
    fVar2 = DAT_01b02b34;
    DAT_01bfc3b4[0x57] = 0x42f00000;
    iVar9 = DAT_01bfc3b4[2];
    local_28 = (float)DAT_01bfc3b4[0x57] - _DAT_01b02aac;
    if (((*(float *)(iVar9 + 0x50) != 0.0) || (*(float *)(iVar9 + 0x54) != fVar2)) ||
       (*(float *)(iVar9 + 0x58) != local_28)) {
      *(ulonglong *)(iVar9 + 0x50) = (ulonglong)(uint)fVar2 << 0x20;
      *(float *)(iVar9 + 0x58) = local_28;
    }
    FUN_0147ab50();
    piVar1 = DAT_01bfc3b4;
    fVar12 = DAT_01b029d4;
    fVar2 = (float)DAT_01bfc3b4[0x4a];
    *(undefined1 *)(DAT_01bfc3b4 + 0x48) = 1;
    piVar1[0x46] = 0x43960000;
    piVar1[0x47] = 0x44fa0052;
    uVar5 = DAT_01b03420;
    if ((fVar12 <= fVar2) && (fVar12 = DAT_01b02a54, fVar2 <= DAT_01b02a54)) {
      fVar12 = fVar2;
    }
    iVar9 = *piVar1;
    piVar1[0x4a] = (int)fVar12;
    fVar2 = DAT_01b02a50;
    *(ulonglong *)(iVar9 + 0x50) = (ulonglong)((uint)fVar12 ^ uVar5) << 0x20;
    *(undefined4 *)(iVar9 + 0x58) = 0;
    piVar1[0x4d] = piVar1[0x4a];
    piVar1 = DAT_01bfc3b4;
    fVar12 = fVar2;
    if ((((char)DAT_01bfc3b4[0x48] == '\x01') &&
        (fVar12 = (float)DAT_01bfc3b4[0x46], (float)DAT_01bfc3b4[0x46] <= fVar2)) &&
       (fVar12 = (float)DAT_01bfc3b4[0x47], fVar2 <= (float)DAT_01bfc3b4[0x47])) {
      fVar12 = fVar2;
    }
    DAT_01bfc3b4[0x4a] = (int)fVar12;
    iVar9 = *piVar1;
    local_28 = 0.0;
    *(ulonglong *)(iVar9 + 0x50) = (ulonglong)((uint)fVar12 ^ uVar5) << 0x20;
    *(undefined4 *)(iVar9 + 0x58) = 0;
    *(undefined4 *)(param_1 + 0x7c) = 0x3e8a3d71;
    *(undefined4 *)(param_1 + 0x80) = 0x3ca3d70a;
    *(undefined4 *)(param_1 + 0x78) = 0x44fa0000;
    DAT_01bfc3b4[0x4d] = 0x44fa0000;
    fVar12 = (float)DAT_01bfc3b4[0x46];
    if ((fVar2 < fVar12) || (fVar12 = (float)DAT_01bfc3b4[0x47], fVar12 < fVar2)) {
      DAT_01bfc3b4[0x4d] = (int)fVar12;
    }
    *(undefined4 *)(param_1 + 0x20) = 0xffffffff;
    *(undefined1 *)(param_1 + 0x91) = 0;
    FUN_014414a0();
  }
  local_14 = CONCAT31(local_14._1_3_,2);
  if (piVar11 != (int *)0x0) {
    LOCK();
    piVar1 = piVar11 + 1;
    iVar9 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar9 == 1) {
      (**(code **)(*piVar11 + 4))();
    }
  }
LAB_010eda7e:
  FUN_0169b2f0();
  ExceptionList = local_1c;
  return;
}


