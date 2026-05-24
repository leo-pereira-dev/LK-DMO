
/* ============================================================ */
/* Function: FUN_011ee3c0 */
/* Entry: 011ee3c0 */
/* Signature: undefined FUN_011ee3c0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_011ee3c0(int param_1)

{
  int *piVar1;
  int *piVar2;
  undefined4 uVar3;
  float fVar4;
  float fVar5;
  uint uVar6;
  int iVar7;
  undefined1 *puVar8;
  char cVar9;
  int iVar10;
  char *pcVar11;
  int *piVar12;
  float fVar13;
  float fVar14;
  undefined1 local_448 [348];
  int *local_2ec;
  int *local_34;
  float local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  iVar10 = DAT_01bfb2d8;
  iVar7 = DAT_01bfb2c4;
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019838f8;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  *(undefined4 *)(param_1 + 0x88) = 0;
  *(undefined4 *)(iVar10 + 0x58) = 9;
  *(int *)(param_1 + 8) = DAT_01bfc3b4[0x46];
  *(int *)(param_1 + 0xc) = DAT_01bfc3b4[0x47];
  *(undefined1 *)(param_1 + 0x10) = *(undefined1 *)(iVar7 + 0x1a4);
  iVar10 = DAT_01bfb2a0;
  *(undefined1 *)(iVar7 + 0x1a4) = 0;
  puVar8 = &stack0xfffffffc;
  if (iVar10 != 0) {
    FUN_00f54dc0(local_24);
    iVar10 = DAT_01bfb2a0;
    puVar8 = puStack_20;
  }
  puStack_20 = puVar8;
  DAT_01bb97a0 = 0;
  if (iVar10 != 0) {
    FUN_00f547c0(s_Enchant_Room_mp3_01ac8de8);
  }
  iVar10 = DAT_01bfb2a8;
  *(undefined4 *)(*(int *)(DAT_01bfb2a8 + 8) + 0x8bc) = 0;
  (**(code **)(**(int **)(iVar10 + 8) + 0x70))();
  (**(code **)(**(int **)(DAT_01bfb2a8 + 0x14) + 0x70))();
  local_28 = -100000.0;
  *(undefined8 *)(param_1 + 100) = 0;
  *(undefined4 *)(param_1 + 0x6c) = 0xc7c35000;
  memset(local_448,0,0x40c);
  FUN_0169ab10();
  local_14 = 0;
  cVar9 = FUN_0169c4f0(s_Data_etcobject_DatsCenter_F_DATS_01ac8dfc);
  if (cVar9 == '\0') {
    FUN_015aa730(0,&DAT_01ac8e28,s_Data_etcobject_DatsCenter_F_DATS_01ac8dfc);
    goto LAB_011eea23;
  }
  piVar12 = (int *)*local_2ec;
  piVar1 = piVar12 + 1;
  if (piVar12 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
    UNLOCK();
  }
  local_14 = CONCAT31(local_14._1_3_,1);
  local_34 = piVar12;
  FUN_01489f70(piVar12,1,0);
  if (piVar12 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
    UNLOCK();
  }
  FUN_01487b70(piVar12,0);
  iVar10 = *(int *)(param_1 + 0x28);
  *(undefined8 *)(iVar10 + 0x50) = *(undefined8 *)(param_1 + 100);
  *(undefined4 *)(iVar10 + 0x58) = *(undefined4 *)(param_1 + 0x6c);
  FUN_016a0910();
  FUN_01692d50();
  FUN_01692c40();
  cVar9 = FUN_0169c4f0(s_Data_etcobject_DatsCenter_E_Grad_01ad7020);
  if (cVar9 == '\0') {
    pcVar11 = s_Data_etcobject_DatsCenter_E_Grad_01ad7020;
LAB_011ee6c6:
    FUN_015aa730(0,&DAT_01ac8e28,pcVar11);
  }
  else {
    piVar2 = (int *)*local_2ec;
    if (piVar12 != piVar2) {
      if (piVar12 != (int *)0x0) {
        LOCK();
        iVar10 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar10 == 1) {
          (**(code **)(*piVar12 + 4))();
        }
      }
      piVar12 = piVar2;
      local_34 = piVar2;
      if (piVar2 != (int *)0x0) {
        LOCK();
        piVar2[1] = piVar2[1] + 1;
        UNLOCK();
      }
    }
    FUN_01489f70(piVar12,1,0);
    piVar1 = piVar12 + 1;
    if (piVar12 != (int *)0x0) {
      LOCK();
      *piVar1 = *piVar1 + 1;
      UNLOCK();
    }
    FUN_01487b70(piVar12,0,0);
    iVar10 = *(int *)(param_1 + 0x40);
    *(undefined8 *)(iVar10 + 0x50) = *(undefined8 *)(param_1 + 100);
    *(undefined4 *)(iVar10 + 0x58) = *(undefined4 *)(param_1 + 0x6c);
    FUN_016a0910();
    uVar3 = DAT_01b02768;
    local_28 = 1.0;
    FUN_0148a890(CONCAT44(DAT_01b02768,DAT_01b02768),0x3f800000);
    cVar9 = FUN_0169c4f0(s_Data_etcobject_DatsCenter_D_Grad_01ac8e84);
    if (cVar9 == '\0') {
      pcVar11 = s_Data_etcobject_DatsCenter_D_Grad_01ac8e84;
      goto LAB_011ee6c6;
    }
    piVar2 = (int *)*local_2ec;
    if (piVar12 != piVar2) {
      if (piVar12 != (int *)0x0) {
        LOCK();
        iVar10 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar10 == 1) {
          (**(code **)(*piVar12 + 4))();
        }
      }
      piVar12 = piVar2;
      local_34 = piVar2;
      if (piVar2 != (int *)0x0) {
        LOCK();
        piVar2[1] = piVar2[1] + 1;
        UNLOCK();
      }
    }
    FUN_01489f70(piVar12,1,0);
    if (piVar12 != (int *)0x0) {
      LOCK();
      piVar12[1] = piVar12[1] + 1;
      UNLOCK();
    }
    FUN_01487b70(piVar12,0,0);
    iVar10 = *(int *)(param_1 + 0x60);
    *(undefined8 *)(iVar10 + 0x50) = *(undefined8 *)(param_1 + 100);
    *(undefined4 *)(iVar10 + 0x58) = *(undefined4 *)(param_1 + 0x6c);
    FUN_016a0910();
    local_28 = 1.0;
    FUN_0148a890(CONCAT44(uVar3,uVar3),0x3f800000);
    fVar13 = DAT_01b02b34;
    DAT_01bfc3b4[0x57] = 0x42f00000;
    iVar10 = DAT_01bfc3b4[2];
    local_28 = (float)DAT_01bfc3b4[0x57] - _DAT_01b02aac;
    if (((*(float *)(iVar10 + 0x50) != 0.0) || (*(float *)(iVar10 + 0x54) != fVar13)) ||
       (*(float *)(iVar10 + 0x58) != local_28)) {
      *(ulonglong *)(iVar10 + 0x50) = (ulonglong)(uint)fVar13 << 0x20;
      *(float *)(iVar10 + 0x58) = local_28;
    }
    FUN_0147ab50();
    piVar1 = DAT_01bfc3b4;
    fVar5 = DAT_01b029d8;
    fVar4 = DAT_01b029d4;
    fVar13 = (float)DAT_01bfc3b4[0x4a];
    *(undefined1 *)(DAT_01bfc3b4 + 0x48) = 1;
    piVar1[0x46] = 0x43960000;
    piVar1[0x47] = 0x43960148;
    uVar6 = DAT_01b03420;
    fVar14 = fVar4;
    if ((fVar4 <= fVar13) && (fVar14 = fVar5, fVar13 <= fVar5)) {
      fVar14 = fVar13;
    }
    iVar10 = *piVar1;
    piVar1[0x4a] = (int)fVar14;
    *(ulonglong *)(iVar10 + 0x50) = (ulonglong)((uint)fVar14 ^ uVar6) << 0x20;
    *(undefined4 *)(iVar10 + 0x58) = 0;
    piVar1[0x4d] = piVar1[0x4a];
    piVar1 = DAT_01bfc3b4;
    fVar13 = fVar4;
    if ((((char)DAT_01bfc3b4[0x48] == '\x01') &&
        (fVar13 = (float)DAT_01bfc3b4[0x46], (float)DAT_01bfc3b4[0x46] <= fVar4)) &&
       (fVar13 = (float)DAT_01bfc3b4[0x47], fVar4 <= (float)DAT_01bfc3b4[0x47])) {
      fVar13 = fVar4;
    }
    DAT_01bfc3b4[0x4a] = (int)fVar13;
    iVar10 = *piVar1;
    local_28 = 0.0;
    *(ulonglong *)(iVar10 + 0x50) = (ulonglong)((uint)fVar13 ^ uVar6) << 0x20;
    *(undefined4 *)(iVar10 + 0x58) = 0;
    FUN_01692c40();
    DAT_01bfc3b4[0x20] = DAT_01bfc3b4[8];
    FUN_016aa9d0();
    *(undefined4 *)(param_1 + 0x70) = 0xffffffff;
    *(undefined1 *)(param_1 + 0x74) = 0;
    *(undefined1 *)(param_1 + 0x44) = 0;
    *(undefined4 *)(param_1 + 0x48) = 0;
    if (*(int *)(param_1 + 0x88) != 0) {
      if (DAT_01bfb2a0 != 0) {
        FUN_00f54e80();
      }
      *(undefined4 *)(param_1 + 0x8c) = 0;
      *(undefined1 *)(param_1 + 0x44) = 1;
      *(undefined4 *)(param_1 + 0x48) = 0x3f800000;
      FUN_015b6ef0(s_System_Tactics_enchant_fail_nif_01ac90d4,0x3f800000,0x10,0);
      (**(code **)(*(int *)**(undefined4 **)(*(int *)(*(int *)(param_1 + 0x88) + 0x154) + 0x58) +
                  0xa0))();
    }
    if (*(int *)(param_1 + 0x88) != 0) {
      *(undefined1 *)(param_1 + 0x44) = 0;
      *(undefined4 *)(param_1 + 0x48) = 0;
      FUN_015b8020();
      FUN_00f5c050(*(undefined4 *)(param_1 + 0x88));
    }
    *(undefined4 *)(param_1 + 0x88) = 0;
    FUN_014414a0();
  }
  local_14 = CONCAT31(local_14._1_3_,2);
  if (piVar12 != (int *)0x0) {
    LOCK();
    piVar1 = piVar12 + 1;
    iVar10 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar10 == 1) {
      (**(code **)(*piVar12 + 4))();
    }
  }
LAB_011eea23:
  FUN_0169b2f0();
  ExceptionList = local_1c;
  return;
}


