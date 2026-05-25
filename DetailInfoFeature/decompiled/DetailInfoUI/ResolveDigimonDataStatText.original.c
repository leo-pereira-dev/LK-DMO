
/* ============================================================ */
/* Function: FUN_011d5940 */
/* Entry: 011d5940 */
/* Signature: undefined FUN_011d5940() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint * __thiscall FUN_011d5940(int param_1,uint *param_2,uint param_3)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  uint *puVar4;
  uint *puVar5;
  uint uVar6;
  uint *puVar7;
  float10 fVar8;
  undefined1 local_8c [8];
  int local_84;
  uint *local_80;
  uint local_7c;
  uint uStack_78;
  uint uStack_74;
  uint uStack_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  uint local_64;
  uint uStack_60;
  uint uStack_5c;
  uint uStack_58;
  undefined4 local_54;
  undefined4 uStack_50;
  uint local_4c;
  uint uStack_48;
  uint uStack_44;
  uint uStack_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined8 local_30;
  int *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_019933ee;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_80 = param_2;
  *param_2 = 0;
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[4] = 0;
  param_2[5] = 0;
  param_2[4] = 0;
  param_2[5] = 7;
  *(short *)param_2 = 0;
  local_14 = 0;
  local_28 = (int *)(*(int *)(param_1 + 0x78) + 0xe0);
  local_34 = 1;
  local_24 = uVar2;
  FUN_00f1b390(local_8c,&param_3);
  uVar6 = param_3;
  if (((*(char *)(local_84 + 0xd) == '\0') && (*(uint *)(local_84 + 0x10) <= param_3)) &&
     (local_84 != *local_28)) {
    iVar3 = *(int *)(local_84 + 0x14);
  }
  else {
    iVar3 = 0;
  }
  local_30 = (double)iVar3;
  fVar8 = (float10)round(local_30,uVar2);
  iVar3 = (int)fVar8;
  local_30 = (double)CONCAT44(iVar3,SUB84((double)fVar8,0));
  if ((uVar6 != 4) && (uVar6 != 8)) {
    if (uVar6 == 3) {
      local_28 = (int *)((float)iVar3 * _DAT_01b0251c);
      FUN_01501290(param_2,&local_28);
      uVar6 = param_2[4];
      puVar5 = param_2;
      if (7 < param_2[5]) {
        puVar5 = (uint *)*param_2;
      }
      if (uVar6 != 0) {
        sVar1 = (short)*puVar5;
        puVar4 = puVar5;
        for (; (puVar7 = puVar4, sVar1 != 0x2e && (puVar7 = (uint *)0x0, uVar6 != 1));
            uVar6 = uVar6 - 1) {
          puVar4 = (uint *)((int)puVar4 + 2);
          sVar1 = *(short *)puVar4;
        }
        if (puVar7 != (uint *)0x0) {
          iVar3 = (int)puVar7 - (int)puVar5 >> 1;
          goto LAB_011d5a95;
        }
      }
      iVar3 = -1;
LAB_011d5a95:
      local_3c = 0;
      local_4c = 0;
      uStack_48 = 0;
      uStack_44 = 0;
      uStack_40 = 0;
      uVar6 = iVar3 + 4U;
      if (param_2[4] < iVar3 + 4U) {
        uVar6 = param_2[4];
      }
      uStack_38 = 0;
      puVar5 = param_2;
      if (7 < param_2[5]) {
        puVar5 = (uint *)*param_2;
      }
      FUN_00f1a200(puVar5,uVar6);
      if (param_2 != &local_4c) {
        FUN_00f1a330();
        uVar6 = local_4c;
        local_4c = local_4c & 0xffff0000;
        *param_2 = uVar6;
        param_2[1] = uStack_48;
        param_2[2] = uStack_44;
        param_2[3] = uStack_40;
        *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_38,local_3c);
        local_3c = 0;
        uStack_38 = 7;
      }
      FUN_00f1a330();
      ExceptionList = local_1c;
      return param_2;
    }
    if (uVar6 != 7) {
      FUN_015011b0(param_2,(int)&local_30 + 4);
      ExceptionList = local_1c;
      return param_2;
    }
    local_28 = (int *)((float)iVar3 * _DAT_01b0252c);
    FUN_01501290(param_2,&local_28);
    uVar6 = param_2[4];
    puVar5 = param_2;
    if (7 < param_2[5]) {
      puVar5 = (uint *)*param_2;
    }
    if (uVar6 == 0) {
LAB_011d5b6b:
      uVar6 = 0xffffffff;
    }
    else {
      sVar1 = (short)*puVar5;
      puVar4 = puVar5;
      for (; (puVar7 = puVar4, sVar1 != 0x2e && (puVar7 = (uint *)0x0, uVar6 != 1));
          uVar6 = uVar6 - 1) {
        puVar4 = (uint *)((int)puVar4 + 2);
        sVar1 = *(short *)puVar4;
      }
      if (puVar7 == (uint *)0x0) goto LAB_011d5b6b;
      uVar6 = (int)puVar7 - (int)puVar5 >> 1;
    }
    local_64 = 0;
    uStack_60 = 0;
    uStack_5c = 0;
    uStack_58 = 0;
    if (param_2[4] < uVar6) {
      uVar6 = param_2[4];
    }
    local_54 = 0;
    uStack_50 = 0;
    puVar5 = param_2;
    if (7 < param_2[5]) {
      puVar5 = (uint *)*param_2;
    }
    FUN_00f1a200(puVar5,uVar6);
    local_34 = 9;
    if (param_2 != &local_64) {
      FUN_00f1a330();
      uVar6 = local_64;
      local_64 = local_64 & 0xffff0000;
      *param_2 = uVar6;
      param_2[1] = uStack_60;
      param_2[2] = uStack_5c;
      param_2[3] = uStack_58;
      *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_50,local_54);
      local_54 = 0;
      uStack_50 = 7;
    }
    goto LAB_011d5cbb;
  }
  local_28 = (int *)((float)iVar3 * _DAT_01b0252c);
  FUN_01501290(param_2,&local_28);
  uVar6 = param_2[4];
  puVar5 = param_2;
  if (7 < param_2[5]) {
    puVar5 = (uint *)*param_2;
  }
  if (uVar6 == 0) {
LAB_011d5c4b:
    uVar6 = 0xffffffff;
  }
  else {
    sVar1 = (short)*puVar5;
    puVar4 = puVar5;
    for (; (puVar7 = puVar4, sVar1 != 0x2e && (puVar7 = (uint *)0x0, uVar6 != 1)); uVar6 = uVar6 - 1
        ) {
      puVar4 = (uint *)((int)puVar4 + 2);
      sVar1 = *(short *)puVar4;
    }
    if (puVar7 == (uint *)0x0) goto LAB_011d5c4b;
    uVar6 = (int)puVar7 - (int)puVar5 >> 1;
  }
  local_7c = 0;
  uStack_78 = 0;
  uStack_74 = 0;
  uStack_70 = 0;
  if (param_2[4] < uVar6) {
    uVar6 = param_2[4];
  }
  local_6c = 0;
  uStack_68 = 0;
  puVar5 = param_2;
  if (7 < param_2[5]) {
    puVar5 = (uint *)*param_2;
  }
  FUN_00f1a200(puVar5,uVar6);
  local_34 = 3;
  if (param_2 != &local_7c) {
    FUN_00f1a330();
    uVar6 = local_7c;
    local_7c = local_7c & 0xffff0000;
    *param_2 = uVar6;
    param_2[1] = uStack_78;
    param_2[2] = uStack_74;
    param_2[3] = uStack_70;
    *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_68,local_6c);
    local_6c = 0;
    uStack_68 = 7;
  }
LAB_011d5cbb:
  FUN_00f1a330();
  FUN_00f53390(&DAT_01ad878c,1);
  ExceptionList = local_1c;
  return param_2;
}


