
/* ============================================================ */
/* Function: FUN_010578a0 */
/* Entry: 010578a0 */
/* Signature: undefined FUN_010578a0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 __thiscall
FUN_010578a0(int param_1,undefined4 param_2,short *param_3,short *param_4,byte *param_5,
            undefined4 param_6)

{
  undefined4 uVar1;
  char cVar2;
  undefined4 *puVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  undefined1 local_78 [24];
  undefined4 local_60;
  short *local_5c;
  int *local_58;
  int local_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 local_40;
  int local_3c;
  byte *local_38;
  undefined4 local_34;
  short *local_30;
  undefined1 local_2a;
  undefined1 local_29;
  int local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01978d15;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_60 = param_2;
  local_30 = param_4;
  local_38 = param_5;
  local_5c = param_3;
  local_34 = param_6;
  local_54 = param_1;
  cVar2 = FUN_010575a0(local_24);
  if (cVar2 == '\0') {
    local_40 = 0;
    local_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    uStack_44 = 0;
    local_3c = 0;
    FUN_00f19cc0(s_COMMON_NOT_USE_NOW_01ad0cf0,0x12);
    local_14 = 0;
    puVar3 = (undefined4 *)FUN_015ce180(local_78,&local_50);
    local_14 = CONCAT31(local_14._1_3_,1);
  }
  else {
    if (*(int *)(*(int *)(param_1 + 0x18) + 4) == 0) {
      ExceptionList = local_1c;
      return 0;
    }
    if ((((DAT_01bfb2b4 == 0) || (DAT_01bfb2b4 == -0x28)) || (199 < (uint)(int)*param_3)) ||
       (piVar5 = (int *)(DAT_01bfb2b4 + 0x58 + *param_3 * 0x45), piVar5 == (int *)0x0)) {
      piVar5 = (int *)0x0;
    }
    if (piVar5 == (int *)0x0) {
      ExceptionList = local_1c;
      return 0;
    }
    local_28 = *piVar5;
    if (local_28 == 0) {
      ExceptionList = local_1c;
      return 0;
    }
    FUN_00f489b0(&uStack_44,local_38);
    if (*(char *)(local_3c + 0xd) != '\0') {
      ExceptionList = local_1c;
      return 0;
    }
    if (*local_38 < *(byte *)(local_3c + 0x10)) {
      ExceptionList = local_1c;
      return 0;
    }
    if (local_3c == *(int *)(local_54 + 0x10)) {
      ExceptionList = local_1c;
      return 0;
    }
    piVar6 = *(int **)(local_3c + 0x14);
    if (piVar6 == (int *)0x0) {
      ExceptionList = local_1c;
      return 0;
    }
    local_29 = 0;
    local_58 = piVar6;
    cVar2 = (**(code **)(*piVar6 + 0x2c))(local_34,&local_29);
    if (cVar2 == '\0') {
      local_40 = 0;
      local_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      uStack_44 = 0;
      local_3c = 0;
      FUN_00f19cc0(s_COMMON_NOT_USE_NOW_01ad0cf0,0x12);
      local_14 = 2;
      puVar3 = (undefined4 *)FUN_015ce180(local_78,&local_50);
      local_14 = CONCAT31(local_14._1_3_,3);
    }
    else {
      cVar2 = (**(code **)(*piVar6 + 0x30))(&local_28);
      if (cVar2 != '\0') {
        if (*local_30 == 0) {
          ExceptionList = local_1c;
          return 0;
        }
        if (((short)piVar5[1] != *local_30) &&
           (piVar4 = (int *)(**(code **)(*piVar6 + 0x50))(local_34), piVar4 != (int *)0x0)) {
          if (((*piVar4 != 0) || (piVar6 = local_58, (short)piVar4[1] != 0)) &&
             ((piVar4[0x12] != 0 && (*piVar4 != *piVar5)))) {
            ExceptionList = local_1c;
            return 0;
          }
        }
        uVar1 = local_34;
        if (*(int *)(*(int *)(local_54 + 0x18) + 4) == 0) {
          ExceptionList = local_1c;
          return 0;
        }
        cVar2 = FUN_01063cc0(local_60,local_5c,local_30,local_38,local_34);
        if (cVar2 == '\0') {
          ExceptionList = local_1c;
          return 0;
        }
        local_2a = 1;
        (**(code **)(*piVar6 + 0xc))(uVar1,&local_2a);
        ExceptionList = local_1c;
        return 1;
      }
      local_40 = 0;
      local_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      uStack_44 = 0;
      local_3c = 0;
      FUN_00f19cc0(s_EXTRA_INVEN_WRONG_ITEM_MOVE_01ad0df4,0x1b);
      local_14 = 4;
      puVar3 = (undefined4 *)FUN_015ce180(local_78,&local_50);
      local_14 = CONCAT31(local_14._1_3_,5);
    }
  }
  if (DAT_01bfbcf4 != 0) {
    if (7 < (uint)puVar3[5]) {
      puVar3 = (undefined4 *)*puVar3;
    }
    FUN_013bfad0(puVar3);
  }
  FUN_00f1a330();
  FUN_00f1a420();
  ExceptionList = local_1c;
  return 0;
}


