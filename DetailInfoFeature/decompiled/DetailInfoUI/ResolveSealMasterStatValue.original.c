
/* ============================================================ */
/* Function: FUN_011da5e0 */
/* Entry: 011da5e0 */
/* Signature: undefined FUN_011da5e0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint * __thiscall FUN_011da5e0(int param_1,uint *param_2,uint param_3)

{
  short sVar1;
  uint uVar2;
  uint *puVar3;
  uint *puVar4;
  uint *puVar5;
  int iVar6;
  undefined1 local_78 [8];
  int local_70;
  uint local_6c;
  uint uStack_68;
  uint uStack_64;
  uint uStack_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  uint local_54;
  uint uStack_50;
  uint uStack_4c;
  uint uStack_48;
  undefined4 local_44;
  undefined4 uStack_40;
  uint local_3c;
  uint uStack_38;
  uint uStack_34;
  uint uStack_30;
  undefined4 local_2c;
  undefined4 uStack_28;
  uint *local_24;
  undefined4 local_20;
  float local_1c;
  int local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_0199375e;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_24 = param_2;
  *param_2 = 0;
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[4] = 0;
  param_2[5] = 0;
  param_2[4] = 0;
  param_2[5] = 7;
  *(short *)param_2 = 0;
  local_8 = 0;
  iVar6 = *(int *)(param_1 + 0x78);
  local_20 = 1;
  local_14 = uVar2;
  FUN_00f1b390(local_78,&param_3);
  if (((*(char *)(local_70 + 0xd) == '\0') && (*(uint *)(local_70 + 0x10) <= param_3)) &&
     (local_70 != *(int *)(iVar6 + 0xd0))) {
    local_18 = *(int *)(local_70 + 0x14);
  }
  else {
    local_18 = 0;
  }
  if (((param_3 != 4) && (param_3 != 7)) && (param_3 != 8)) {
    FUN_015011b0(param_2,&local_18);
    uVar2 = param_2[4];
    if (uVar2 != 0x7ffffffe) {
      puVar3 = param_2;
      if (7 < param_2[5]) {
        puVar3 = (uint *)*param_2;
      }
      FUN_00f52ec0(local_24,uVar2,&DAT_01ae2f40,1,puVar3,uVar2);
      if (param_2 != &local_54) {
        FUN_00f1a330();
        uVar2 = local_54;
        local_54 = local_54 & 0xffff0000;
        *param_2 = uVar2;
        param_2[1] = uStack_50;
        param_2[2] = uStack_4c;
        param_2[3] = uStack_48;
        *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_40,local_44);
        local_44 = 0;
        uStack_40 = 7;
      }
      FUN_00f1a330();
      ExceptionList = local_10;
      return param_2;
    }
                    /* WARNING: Subroutine does not return */
    FUN_00f1a480();
  }
  local_1c = (float)local_18 * _DAT_01b0252c;
  FUN_01501290(param_2,&local_1c,uVar2);
  uVar2 = param_2[4];
  if (uVar2 == 0x7ffffffe) {
                    /* WARNING: Subroutine does not return */
    FUN_00f1a480();
  }
  puVar3 = param_2;
  if (7 < param_2[5]) {
    puVar3 = (uint *)*param_2;
  }
  FUN_00f52ec0(local_24,uVar2,&DAT_01ae2f40,1,puVar3,uVar2);
  local_20 = 3;
  if (param_2 != &local_6c) {
    FUN_00f1a330();
    uVar2 = local_6c;
    local_6c = local_6c & 0xffff0000;
    *param_2 = uVar2;
    param_2[1] = uStack_68;
    param_2[2] = uStack_64;
    param_2[3] = uStack_60;
    *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_58,local_5c);
    local_5c = 0;
    uStack_58 = 7;
  }
  FUN_00f1a330();
  uVar2 = param_2[4];
  puVar3 = param_2;
  if (7 < param_2[5]) {
    puVar3 = (uint *)*param_2;
  }
  if (uVar2 != 0) {
    sVar1 = (short)*puVar3;
    puVar4 = puVar3;
    for (; (puVar5 = puVar4, sVar1 != 0x2e && (puVar5 = (uint *)0x0, uVar2 != 1)); uVar2 = uVar2 - 1
        ) {
      puVar4 = (uint *)((int)puVar4 + 2);
      sVar1 = *(short *)puVar4;
    }
    if (puVar5 != (uint *)0x0) {
      iVar6 = (int)puVar5 - (int)puVar3 >> 1;
      goto LAB_011da7e5;
    }
  }
  iVar6 = -1;
LAB_011da7e5:
  local_2c = 0;
  local_3c = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  uVar2 = iVar6 + 3U;
  if (param_2[4] < iVar6 + 3U) {
    uVar2 = param_2[4];
  }
  uStack_28 = 0;
  puVar3 = param_2;
  if (7 < param_2[5]) {
    puVar3 = (uint *)*param_2;
  }
  FUN_00f1a200(puVar3,uVar2);
  local_20 = 7;
  if (param_2 != &local_3c) {
    FUN_00f1a330();
    uVar2 = local_3c;
    local_3c = local_3c & 0xffff0000;
    *param_2 = uVar2;
    param_2[1] = uStack_38;
    param_2[2] = uStack_34;
    param_2[3] = uStack_30;
    *(ulonglong *)(param_2 + 4) = CONCAT44(uStack_28,local_2c);
    local_2c = 0;
    uStack_28 = 7;
  }
  FUN_00f1a330();
  FUN_00f53390(&DAT_01ad878c,1);
  ExceptionList = local_10;
  return param_2;
}


