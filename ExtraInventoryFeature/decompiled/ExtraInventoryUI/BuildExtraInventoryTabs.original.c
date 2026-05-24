
/* ============================================================ */
/* Function: FUN_011a9f80 */
/* Entry: 011a9f80 */
/* Signature: undefined FUN_011a9f80() */
/* ============================================================ */


void __fastcall FUN_011a9f80(int param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  void *pvVar3;
  undefined4 *puVar4;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 *local_20;
  undefined4 local_1c;
  undefined4 *local_18;
  undefined4 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199040f;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  FUN_00f6e780(DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_30 = 0x80;
  local_2c = 0x20;
  local_28 = 0x140;
  local_24 = 0x1e0;
  puVar2 = (undefined4 *)FUN_016830c0(0x94);
  local_8 = 0;
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    local_14 = puVar2;
    memset(puVar2,0,0x94);
    FUN_01245550();
    puVar2[0x1d] = &DAT_01ae1118;
    puVar2[0x1e] = 0;
    puVar2[0x1d] = &DAT_01ae110c;
    local_8 = CONCAT31(local_8._1_3_,1);
    puVar1 = puVar2 + 0x20;
    *puVar2 = &DAT_01ae1300;
    puVar2[0x1d] = &DAT_01ae129c;
    puVar2[0x1f] = 0;
    *puVar1 = 0;
    puVar2[0x21] = 0;
    local_18 = puVar1;
    pvVar3 = operator_new(0x30);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    *(void **)((int)pvVar3 + 8) = pvVar3;
    *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
    *puVar1 = pvVar3;
    puVar2[0x22] = 0;
    puVar2[0x23] = 0;
    puVar2[0x24] = 0;
  }
  local_8 = 0xffffffff;
  local_14 = puVar2;
  if (puVar2 != (undefined4 *)0x0) {
    FUN_011ad850(param_1,&local_30,&local_28);
    puVar2[0x12] = param_1;
    local_18 = *(undefined4 **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_011aa616;
    local_8 = 2;
    local_1c = 0;
    local_20 = (undefined4 *)(param_1 + 0x58);
    puVar4 = operator_new(0xc);
    puVar1 = local_18;
    local_8 = 0xffffffff;
    local_18 = (undefined4 *)&DAT_0000000b;
    puVar4[2] = puVar2;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar2 = (undefined4 *)puVar1[1];
    *puVar4 = puVar1;
    puVar4[1] = puVar2;
    puVar1[1] = puVar4;
    *puVar2 = puVar4;
    FUN_010126a0(&local_20,&local_18,&local_14);
  }
  puVar2 = (undefined4 *)FUN_016830c0(0x90);
  local_8 = 3;
  local_14 = puVar2;
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    memset(puVar2,0,0x90);
    FUN_01245550();
    puVar2[0x1d] = &DAT_01ae1118;
    puVar2[0x1e] = 0;
    puVar2[0x1d] = &DAT_01ae110c;
    local_8 = CONCAT31(local_8._1_3_,4);
    puVar1 = puVar2 + 0x20;
    *puVar2 = &DAT_01ae12a8;
    puVar2[0x1d] = &DAT_01ae1238;
    puVar2[0x1f] = 0;
    *puVar1 = 0;
    puVar2[0x21] = 0;
    local_18 = puVar1;
    pvVar3 = operator_new(0x24);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    *(void **)((int)pvVar3 + 8) = pvVar3;
    *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
    *puVar1 = pvVar3;
    puVar2[0x22] = 0;
    puVar2[0x23] = 0;
  }
  local_8 = 0xffffffff;
  local_18 = puVar2;
  if (puVar2 != (undefined4 *)0x0) {
    FUN_011b0b60(param_1,&local_30,&local_28);
    puVar2[0x12] = param_1;
    local_14 = *(undefined4 **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_011aa616;
    local_8 = 5;
    local_1c = 0;
    local_20 = (undefined4 *)(param_1 + 0x58);
    puVar4 = operator_new(0xc);
    puVar1 = local_14;
    local_8 = 0xffffffff;
    local_14 = (undefined4 *)&DAT_0000000c;
    puVar4[2] = puVar2;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar2 = (undefined4 *)puVar1[1];
    *puVar4 = puVar1;
    puVar4[1] = puVar2;
    puVar1[1] = puVar4;
    *puVar2 = puVar4;
    FUN_010126a0(&local_20,&local_14,&local_18);
  }
  puVar2 = (undefined4 *)FUN_016830c0(0xa8);
  local_8 = 6;
  local_14 = puVar2;
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    memset(puVar2,0,0xa8);
    FUN_01245550();
    puVar2[0x1d] = &DAT_01ae1118;
    puVar2[0x1e] = 0;
    puVar2[0x1d] = &DAT_01ae110c;
    local_8 = CONCAT31(local_8._1_3_,7);
    puVar1 = puVar2 + 0x20;
    *puVar2 = &DAT_01ae1244;
    puVar2[0x1d] = &DAT_01ae11d4;
    puVar2[0x1f] = 0;
    *puVar1 = 0;
    puVar2[0x21] = 0;
    local_18 = puVar1;
    pvVar3 = operator_new(0x34);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    *(void **)((int)pvVar3 + 8) = pvVar3;
    *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
    *puVar1 = pvVar3;
    puVar2[0x22] = 0;
    puVar2[0x23] = 0;
    puVar2[0x24] = 0;
    puVar2[0x25] = 0;
    puVar2[0x26] = 0;
    puVar2[0x27] = 0;
    puVar2[0x28] = 0;
    puVar2[0x29] = 0;
  }
  local_8 = 0xffffffff;
  local_18 = puVar2;
  if (puVar2 != (undefined4 *)0x0) {
    FUN_011a5270(param_1,&local_30,&local_28);
    puVar2[0x12] = param_1;
    local_14 = *(undefined4 **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_011aa616;
    local_8 = 8;
    local_1c = 0;
    local_20 = (undefined4 *)(param_1 + 0x58);
    puVar4 = operator_new(0xc);
    puVar1 = local_14;
    local_8 = 0xffffffff;
    local_14 = (undefined4 *)&DAT_0000000d;
    puVar4[2] = puVar2;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar2 = (undefined4 *)puVar1[1];
    *puVar4 = puVar1;
    puVar4[1] = puVar2;
    puVar1[1] = puVar4;
    *puVar2 = puVar4;
    FUN_010126a0(&local_20,&local_14,&local_18);
  }
  puVar2 = (undefined4 *)FUN_016830c0(0x90);
  local_8 = 9;
  local_14 = puVar2;
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    memset(puVar2,0,0x90);
    FUN_01245550();
    puVar2[0x1d] = &DAT_01ae1118;
    puVar2[0x1e] = 0;
    puVar2[0x1d] = &DAT_01ae110c;
    local_8 = CONCAT31(local_8._1_3_,10);
    puVar1 = puVar2 + 0x20;
    *puVar2 = &DAT_01ae11e0;
    puVar2[0x1d] = &DAT_01ae1170;
    puVar2[0x1f] = 0;
    *puVar1 = 0;
    puVar2[0x21] = 0;
    local_18 = puVar1;
    pvVar3 = operator_new(0x2c);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    *(void **)((int)pvVar3 + 8) = pvVar3;
    *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
    *puVar1 = pvVar3;
    puVar2[0x22] = 0;
    puVar2[0x23] = 0;
  }
  local_8 = 0xffffffff;
  local_18 = puVar2;
  if (puVar2 != (undefined4 *)0x0) {
    FUN_011a29c0(param_1,&local_30,&local_28);
    puVar2[0x12] = param_1;
    local_14 = *(undefined4 **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_011aa616;
    local_8 = 0xb;
    local_1c = 0;
    local_20 = (undefined4 *)(param_1 + 0x58);
    puVar4 = operator_new(0xc);
    puVar1 = local_14;
    local_8 = 0xffffffff;
    local_14 = (undefined4 *)0xe;
    puVar4[2] = puVar2;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar2 = (undefined4 *)puVar1[1];
    *puVar4 = puVar1;
    puVar4[1] = puVar2;
    puVar1[1] = puVar4;
    *puVar2 = puVar4;
    FUN_010126a0(&local_20,&local_14,&local_18);
  }
  puVar2 = (undefined4 *)FUN_016830c0(0x90);
  local_8 = 0xc;
  local_14 = puVar2;
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    memset(puVar2,0,0x90);
    FUN_01245550();
    puVar2[0x1d] = &DAT_01ae1118;
    puVar2[0x1e] = 0;
    puVar2[0x1d] = &DAT_01ae110c;
    local_8 = CONCAT31(local_8._1_3_,0xd);
    puVar1 = puVar2 + 0x20;
    *puVar2 = &DAT_01ae117c;
    puVar2[0x1d] = &DAT_01ae1164;
    puVar2[0x1f] = 0;
    *puVar1 = 0;
    puVar2[0x21] = 0;
    local_18 = puVar1;
    pvVar3 = operator_new(0x2c);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    *(void **)((int)pvVar3 + 8) = pvVar3;
    *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
    *puVar1 = pvVar3;
    puVar2[0x22] = 0;
    puVar2[0x23] = 0;
  }
  local_8 = 0xffffffff;
  if (puVar2 != (undefined4 *)0x0) {
    local_18 = puVar2;
    FUN_011ab290(param_1,&local_30,&local_28);
    puVar2[0x12] = param_1;
    local_14 = *(undefined4 **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) {
LAB_011aa616:
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8 = 0xe;
    local_1c = 0;
    local_20 = (undefined4 *)(param_1 + 0x58);
    puVar4 = operator_new(0xc);
    puVar1 = local_14;
    local_8 = 0xffffffff;
    local_14 = (undefined4 *)0xf;
    puVar4[2] = puVar2;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar2 = (undefined4 *)puVar1[1];
    *puVar4 = puVar1;
    puVar4[1] = puVar2;
    puVar1[1] = puVar4;
    *puVar2 = puVar4;
    FUN_010126a0(&local_20,&local_14,&local_18);
  }
  ExceptionList = local_10;
  return;
}


