
/* ============================================================ */
/* Function: FUN_0126cb80 */
/* Entry: 0126cb80 */
/* Signature: undefined FUN_0126cb80() */
/* ============================================================ */


undefined4 * __fastcall FUN_0126cb80(undefined4 *param_1)

{
  undefined4 uVar1;
  void *pvVar2;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199bb97;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  FUN_01245550(DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  param_1[0x1d] = &DAT_01ae9024;
  param_1[0x1e] = 0;
  param_1[0x1d] = &DAT_01aeb570;
  local_8 = 0;
  *param_1 = &DAT_01ae8d4c;
  param_1[0x1d] = &DAT_01ae8d98;
  uVar1 = DAT_01bee890;
  param_1[0x1f] = DAT_01bee88c;
  param_1[0x20] = uVar1;
  param_1[0x21] = 0;
  param_1[0x22] = 0;
  param_1[0x23] = 0;
  param_1[0x24] = 0;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  param_1[0x27] = 0;
  param_1[0x28] = 0;
  param_1[0x2a] = 0;
  param_1[0x2b] = 0;
  param_1[0x2c] = 0;
  param_1[0x2d] = 0;
  param_1[0x2f] = 0;
  param_1[0x30] = 0;
  param_1[0x31] = 0;
  param_1[0x32] = 0;
  param_1[0x33] = 0;
  param_1[0x34] = 0;
  param_1[0x35] = 0;
  param_1[0x36] = 0;
  param_1[0x3a] = 0;
  param_1[0x3b] = 0;
  param_1[0x3c] = 0;
  pvVar2 = operator_new(0xc);
  *(void **)pvVar2 = pvVar2;
  *(void **)((int)pvVar2 + 4) = pvVar2;
  param_1[0x3b] = pvVar2;
  *(undefined1 *)(param_1 + 0x3d) = 1;
  param_1[0x3e] = 0;
  param_1[0x3f] = 0;
  param_1[0x40] = 0;
  local_8._0_1_ = 1;
  param_1[0x42] = 0;
  param_1[0x43] = 0;
  param_1[0x44] = 0;
  pvVar2 = operator_new(0xc);
  *(void **)pvVar2 = pvVar2;
  *(void **)((int)pvVar2 + 4) = pvVar2;
  param_1[0x43] = pvVar2;
  *(undefined1 *)(param_1 + 0x45) = 1;
  param_1[0x46] = 0;
  param_1[0x47] = 0;
  param_1[0x48] = 0;
  local_8._0_1_ = 2;
  *(undefined1 *)(param_1 + 0x49) = 0;
  param_1[0x4a] = 0;
  param_1[0x4b] = 0;
  param_1[0x4c] = 0;
  param_1[0x4d] = 0;
  param_1[0x4e] = 0;
  param_1[0x4f] = 0;
  param_1[0x50] = 0;
  param_1[0x51] = 0;
  pvVar2 = operator_new(0x1c);
  *(void **)pvVar2 = pvVar2;
  *(void **)((int)pvVar2 + 4) = pvVar2;
  *(void **)((int)pvVar2 + 8) = pvVar2;
  *(undefined2 *)((int)pvVar2 + 0xc) = 0x101;
  param_1[0x50] = pvVar2;
  local_8 = CONCAT31(local_8._1_3_,3);
  param_1[0x52] = 0xffffffff;
  param_1[0x53] = 0;
  param_1[0x54] = 0;
  param_1[0x55] = 0;
  FUN_0126cfc0();
  ExceptionList = local_10;
  return param_1;
}


