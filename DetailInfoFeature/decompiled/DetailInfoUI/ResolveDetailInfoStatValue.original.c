
/* ============================================================ */
/* Function: FUN_011dd1d0 */
/* Entry: 011dd1d0 */
/* Signature: undefined FUN_011dd1d0() */
/* ============================================================ */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint * __thiscall FUN_011dd1d0(int param_1,uint *param_2,uint param_3)

{
  short sVar1;
  uint uVar2;
  uint *puVar3;
  uint *puVar4;
  uint *puVar5;
  int iVar6;
  float10 fVar7;
  float fVar8;
  undefined1 local_80 [8];
  int local_78;
  uint *local_74;
  uint local_70;
  uint uStack_6c;
  uint uStack_68;
  uint uStack_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  uint local_58;
  uint uStack_54;
  uint uStack_50;
  uint uStack_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  uint local_40;
  uint uStack_3c;
  uint uStack_38;
  uint uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 local_28;
  float local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_01993bae;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  fVar8 = 0.0;
  local_74 = param_2;
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
  iVar6 = *(int *)(param_1 + 0x78);
  local_28 = 1;
  FUN_00f1b390(local_80,&param_3);
  if (((*(char *)(local_78 + 0xd) == '\0') && (*(uint *)(local_78 + 0x10) <= param_3)) &&
     (local_78 != *(int *)(iVar6 + 0xd8))) {
    fVar8 = *(float *)(local_78 + 0x14);
  }
  local_24 = (float)(int)fVar8;
  if (((param_3 != 4) && (param_3 != 0xb)) && ((param_3 != 0xc && (param_3 != 3)))) {
    if (((param_3 != 6) && (param_3 != 7)) && (param_3 != 9)) {
      fVar7 = (float10)roundf(local_24,uVar2);
      local_24 = (float)fVar7;
      FUN_01501290(param_2,&local_24);
      uVar2 = param_2[4];
      puVar4 = param_2;
      if (7 < param_2[5]) {
        puVar4 = (uint *)*param_2;
      }
      if (uVar2 != 0) {
        sVar1 = (short)*puVar4;
        puVar3 = puVar4;
        for (; (puVar5 = puVar3, sVar1 != 0x2e && (puVar5 = (uint *)0x0, uVar2 != 1));
            uVar2 = uVar2 - 1) {
          puVar3 = (uint *)((int)puVar3 + 2);
          sVar1 = *(short *)puVar3;
        }
        if (puVar5 != (uint *)0x0) {
          uVar2 = (int)puVar5 - (int)puVar4 >> 1;
          goto LAB_011dd323;
        }
      }
      uVar2 = 0xffffffff;
LAB_011dd323:
      local_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      uStack_34 = 0;
      if (param_2[4] < uVar2) {
        uVar2 = param_2[4];
      }
      local_30 = 0;
      uStack_2c = 0;
      puVar4 = param_2;
      if (7 < param_2[5]) {
        puVar4 = (uint *)*param_2;
      }
      FUN_00f1a200(puVar4,uVar2);
      if (param_2 != &local_40) {
        FUN_00f1a330();
        uVar2 = local_40;
        local_40 = local_40 & 0xffff0000;
        *param_2 = uVar2;
        param_2[1] = uStack_3c;
        param_2[2] = uStack_38;
        param_2[3] = uStack_34;
        *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_2c,local_30);
        local_30 = 0;
        uStack_2c = 7;
      }
      FUN_00f1a330();
      ExceptionList = local_1c;
      return param_2;
    }
    fVar7 = (float10)roundf(local_24,uVar2);
    local_24 = (float)fVar7;
    FUN_01501290(param_2,&local_24);
    uVar2 = param_2[4];
    puVar4 = param_2;
    if (7 < param_2[5]) {
      puVar4 = (uint *)*param_2;
    }
    if (uVar2 == 0) {
LAB_011dd3eb:
      uVar2 = 0xffffffff;
    }
    else {
      sVar1 = (short)*puVar4;
      puVar3 = puVar4;
      for (; (puVar5 = puVar3, sVar1 != 0x2e && (puVar5 = (uint *)0x0, uVar2 != 1));
          uVar2 = uVar2 - 1) {
        puVar3 = (uint *)((int)puVar3 + 2);
        sVar1 = *(short *)puVar3;
      }
      if (puVar5 == (uint *)0x0) goto LAB_011dd3eb;
      uVar2 = (int)puVar5 - (int)puVar4 >> 1;
    }
    local_58 = 0;
    uStack_54 = 0;
    uStack_50 = 0;
    uStack_4c = 0;
    if (param_2[4] < uVar2) {
      uVar2 = param_2[4];
    }
    local_48 = 0;
    uStack_44 = 0;
    puVar4 = param_2;
    if (7 < param_2[5]) {
      puVar4 = (uint *)*param_2;
    }
    FUN_00f1a200(puVar4,uVar2);
    local_28 = 5;
    if (param_2 != &local_58) {
      FUN_00f1a330();
      uVar2 = local_58;
      local_58 = local_58 & 0xffff0000;
      *param_2 = uVar2;
      param_2[1] = uStack_54;
      param_2[2] = uStack_50;
      param_2[3] = uStack_4c;
      *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_44,local_48);
      local_48 = 0;
      uStack_44 = 7;
    }
    goto LAB_011dd534;
  }
  local_24 = local_24 * _DAT_01b0252c;
  fVar7 = (float10)roundf(local_24,uVar2);
  local_24 = (float)fVar7;
  FUN_01501290(param_2,&local_24);
  uVar2 = param_2[4];
  puVar4 = param_2;
  if (7 < param_2[5]) {
    puVar4 = (uint *)*param_2;
  }
  if (uVar2 == 0) {
LAB_011dd4c1:
    iVar6 = -1;
  }
  else {
    sVar1 = (short)*puVar4;
    puVar3 = puVar4;
    for (; (puVar5 = puVar3, sVar1 != 0x2e && (puVar5 = (uint *)0x0, uVar2 != 1)); uVar2 = uVar2 - 1
        ) {
      puVar3 = (uint *)((int)puVar3 + 2);
      sVar1 = *(short *)puVar3;
    }
    if (puVar5 == (uint *)0x0) goto LAB_011dd4c1;
    iVar6 = (int)puVar5 - (int)puVar4 >> 1;
  }
  local_60 = 0;
  local_70 = 0;
  uStack_6c = 0;
  uStack_68 = 0;
  uStack_64 = 0;
  uVar2 = iVar6 + 3U;
  if (param_2[4] < iVar6 + 3U) {
    uVar2 = param_2[4];
  }
  uStack_5c = 0;
  puVar4 = param_2;
  if (7 < param_2[5]) {
    puVar4 = (uint *)*param_2;
  }
  FUN_00f1a200(puVar4,uVar2);
  local_28 = 3;
  if (param_2 != &local_70) {
    FUN_00f1a330();
    uVar2 = local_70;
    local_70 = local_70 & 0xffff0000;
    *param_2 = uVar2;
    param_2[1] = uStack_6c;
    param_2[2] = uStack_68;
    param_2[3] = uStack_64;
    *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_5c,local_60);
    local_60 = 0;
    uStack_5c = 7;
  }
LAB_011dd534:
  FUN_00f1a330();
  FUN_00f53390(&DAT_01ad878c,1);
  ExceptionList = local_1c;
  return param_2;
}


