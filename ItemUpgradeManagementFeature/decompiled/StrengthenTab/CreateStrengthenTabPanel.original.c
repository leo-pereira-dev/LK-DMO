
/* ============================================================ */
/* Function: FUN_01271e80 */
/* Entry: 01271e80 */
/* Signature: undefined FUN_01271e80() */
/* ============================================================ */


undefined4 * __fastcall FUN_01271e80(undefined4 *param_1)

{
  undefined4 uVar1;
  void *pvVar2;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199be57;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  FUN_01245550(DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  param_1[0x1d] = &DAT_01ae9024;
  param_1[0x1e] = 0;
  param_1[0x1d] = &DAT_01aeb570;
  local_8 = 0;
  *param_1 = &DAT_01ae8e84;
  param_1[0x1d] = &DAT_01ae8e70;
  param_1[0x1f] = 0;
  param_1[0x20] = 0;
  param_1[0x21] = 0;
  uVar1 = DAT_01bee890;
  param_1[0x22] = DAT_01bee88c;
  param_1[0x23] = uVar1;
  param_1[0x24] = 0;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  param_1[0x27] = 0;
  param_1[0x28] = 0;
  param_1[0x29] = 0;
  param_1[0x2a] = 0;
  param_1[0x2b] = 0;
  param_1[0x2c] = 0;
  param_1[0x2d] = 0;
  _eh_vector_constructor_iterator_(param_1 + 0x2e,0x1c,2,FUN_01271de0,FUN_00f199b0);
  local_8._0_1_ = 1;
  _eh_vector_constructor_iterator_(param_1 + 0x3c,0xc,3,FUN_00f2df90,FUN_00f199b0);
  local_8._0_1_ = 2;
  param_1[0x48] = 0;
  param_1[0x49] = 0;
  param_1[0x4a] = 0;
  pvVar2 = operator_new(0xc);
  *(void **)pvVar2 = pvVar2;
  *(void **)((int)pvVar2 + 4) = pvVar2;
  param_1[0x49] = pvVar2;
  *(undefined1 *)(param_1 + 0x4b) = 1;
  param_1[0x4c] = 0;
  param_1[0x4d] = 0;
  param_1[0x4e] = 0;
  local_8._0_1_ = 3;
  param_1[0x50] = 0;
  param_1[0x51] = 0;
  param_1[0x52] = 0;
  pvVar2 = operator_new(0xc);
  *(void **)pvVar2 = pvVar2;
  *(void **)((int)pvVar2 + 4) = pvVar2;
  param_1[0x51] = pvVar2;
  *(undefined1 *)(param_1 + 0x53) = 1;
  param_1[0x54] = 0;
  param_1[0x55] = 0;
  param_1[0x56] = 0;
  local_8._0_1_ = 4;
  *(undefined2 *)(param_1 + 0x57) = 0;
  param_1[0x58] = 0;
  param_1[0x59] = 0;
  param_1[0x5a] = 0;
  param_1[0x5b] = 0;
  param_1[0x5c] = 0;
  param_1[0x5d] = 0;
  param_1[0x5e] = 0;
  param_1[0x5f] = 0;
  param_1[0x60] = 0;
  param_1[0x61] = 0;
  param_1[0x62] = 0;
  param_1[99] = 0;
  pvVar2 = operator_new(0x1c);
  *(void **)pvVar2 = pvVar2;
  *(void **)((int)pvVar2 + 4) = pvVar2;
  *(void **)((int)pvVar2 + 8) = pvVar2;
  *(undefined2 *)((int)pvVar2 + 0xc) = 0x101;
  param_1[0x62] = pvVar2;
  local_8 = CONCAT31(local_8._1_3_,5);
  param_1[100] = 0xffffffff;
  param_1[0x65] = 0;
  FUN_01272200();
  ExceptionList = local_10;
  return param_1;
}


