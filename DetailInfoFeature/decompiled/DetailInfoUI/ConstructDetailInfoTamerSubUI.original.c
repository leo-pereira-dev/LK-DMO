
/* ============================================================ */
/* Function: FUN_011dab40 */
/* Entry: 011dab40 */
/* Signature: undefined FUN_011dab40() */
/* ============================================================ */


undefined4 * __fastcall FUN_011dab40(undefined4 *param_1)

{
  void *pvVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019937b9;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  FUN_01245550(DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  param_1[0x1d] = &DAT_01ae319c;
  param_1[0x1e] = 0;
  param_1[0x1d] = &DAT_01ae33f4;
  local_8 = 0;
  *param_1 = &DAT_01ae3560;
  param_1[0x1d] = &DAT_01ae35a8;
  param_1[0x1f] = 0;
  param_1[0x20] = 0;
  param_1[0x21] = 0;
  param_1[0x22] = 0;
  param_1[0x23] = 0;
  param_1[0x24] = 0;
  pvVar1 = operator_new(0x18);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)((int)pvVar1 + 8) = pvVar1;
  *(undefined2 *)((int)pvVar1 + 0xc) = 0x101;
  param_1[0x23] = pvVar1;
  local_8._0_1_ = 1;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  pvVar1 = operator_new(0x18);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)((int)pvVar1 + 8) = pvVar1;
  *(undefined2 *)((int)pvVar1 + 0xc) = 0x101;
  param_1[0x25] = pvVar1;
  local_8 = CONCAT31(local_8._1_3_,2);
  FUN_011d1200();
  ExceptionList = local_10;
  return param_1;
}


