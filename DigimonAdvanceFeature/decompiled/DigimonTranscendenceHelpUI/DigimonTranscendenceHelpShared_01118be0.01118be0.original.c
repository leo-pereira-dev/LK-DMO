
/* ============================================================ */
/* Function: FUN_01118be0 */
/* Entry: 01118be0 */
/* Signature: undefined FUN_01118be0() */
/* ============================================================ */


undefined4 * __thiscall FUN_01118be0(undefined4 *param_1,uint param_2)

{
  undefined4 *puVar1;
  
  puVar1 = param_1 + 0x1d;
  *puVar1 = &DAT_01ad9264;
  *param_1 = &DAT_01ad9224;
  if (param_1[0x1e] != 0) {
    FUN_00ff8af0(puVar1);
  }
  *puVar1 = &DAT_01ad7e4c;
  FUN_012456d0();
  if ((param_2 & 1) != 0) {
    if ((param_2 & 4) == 0) {
      FUN_01683120();
      return param_1;
    }
    FUN_00f199b0(param_1,0x8c);
  }
  return param_1;
}


