
/* ============================================================ */
/* Function: FUN_01055f10 */
/* Entry: 01055f10 */
/* Signature: undefined FUN_01055f10() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

uint FUN_01055f10(byte *param_1)

{
  int *piVar1;
  char cVar2;
  undefined4 *puVar3;
  uint uVar4;
  undefined1 local_60 [24];
  uint local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  int local_30;
  undefined1 local_29;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01978af5;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  FUN_00ff2740(local_24);
  cVar2 = FUN_010575a0();
  if (cVar2 == '\0') {
    local_34 = 0;
    local_44 = 0;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    local_30 = 0;
    FUN_00f19cc0(s_COMMON_NOT_USE_NOW_01ad0cf0,0x12);
    local_14 = 0;
    puVar3 = (undefined4 *)FUN_015ce180(local_60,&local_44);
    local_14 = CONCAT31(local_14._1_3_,1);
  }
  else {
    uVar4 = FUN_00f489b0(&uStack_38,param_1);
    if ((((*(char *)(local_30 + 0xd) != '\0') ||
         (uVar4 = CONCAT31((int3)(uVar4 >> 8),*param_1), *param_1 < *(byte *)(local_30 + 0x10))) ||
        (uVar4 = local_48, local_30 == *(int *)(local_48 + 0x10))) ||
       (piVar1 = *(int **)(local_30 + 0x14), piVar1 == (int *)0x0)) goto LAB_01055fdc;
    cVar2 = (**(code **)(*piVar1 + 0x4c))();
    if (cVar2 == '\0') {
      local_34 = 0;
      local_44 = 0;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_30 = 0;
      FUN_00f19cc0(s_COMMON_NOT_USE_NOW_01ad0cf0,0x12);
      local_14 = 2;
      puVar3 = (undefined4 *)FUN_015ce180(local_60,&local_44);
      local_14 = CONCAT31(local_14._1_3_,3);
    }
    else if (*(double *)(piVar1 + 4) <= DAT_01bee678 - *(double *)(piVar1 + 2)) {
      if (*DAT_01bfe460 == 0) {
        local_28 = 0;
      }
      else {
        local_28 = FUN_014e3cc0();
      }
      cVar2 = (**(code **)(*piVar1 + 0x60))(&local_28);
      if (cVar2 == '\0') {
        FUN_0105c120();
        local_29 = 1;
        FUN_010574a0(param_1,&local_29);
        uVar4 = FUN_00f47ad0();
        ExceptionList = local_1c;
        return uVar4;
      }
      local_34 = 0;
      local_44 = 0;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_30 = 0;
      FUN_00f19cc0(s_EXTRA_INVEN_ALREADY_SORTED_01ad0d1c,0x1a);
      local_14 = 6;
      puVar3 = (undefined4 *)FUN_015ce180(local_60,&local_44);
      local_14 = CONCAT31(local_14._1_3_,7);
    }
    else {
      local_34 = 0;
      local_44 = 0;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_30 = 0;
      FUN_00f19cc0(s_COMMON_NOT_USE_TIME_01acfe54,0x13);
      local_14 = 4;
      puVar3 = (undefined4 *)FUN_015ce180(local_60,&local_44);
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
  uVar4 = FUN_00f1a420();
LAB_01055fdc:
  ExceptionList = local_1c;
  return uVar4 & 0xffffff00;
}


