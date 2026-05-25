
/* ============================================================ */
/* Function: FUN_011dac40 */
/* Entry: 011dac40 */
/* Signature: undefined FUN_011dac40() */
/* ============================================================ */


undefined4 * __thiscall FUN_011dac40(undefined4 *param_1,uint param_2)

{
  uint uVar1;
  undefined4 *puVar2;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_019685c0;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_8 = 0;
  *param_1 = &DAT_01ae3560;
  param_1[0x1d] = &DAT_01ae35a8;
  FUN_011dad10(uVar1);
  FUN_00f31c20(param_1 + 0x25);
  FUN_00f1d670();
  *param_1 = &DAT_01ae33ac;
  param_1[0x1d] = &DAT_01ae33f4;
  if (param_1[0x1e] != 0) {
    puVar2 = param_1 + 0x1d;
    if (param_1 == (undefined4 *)0x0) {
      puVar2 = (undefined4 *)0x0;
    }
    FUN_011a0480(puVar2);
  }
  param_1[0x1d] = &DAT_01ae319c;
  FUN_012456d0();
  if ((param_2 & 1) != 0) {
    if ((param_2 & 4) == 0) {
      FUN_01683120();
    }
    else {
      FUN_00f199b0(param_1,0x9c);
    }
  }
  ExceptionList = local_10;
  return param_1;
}


