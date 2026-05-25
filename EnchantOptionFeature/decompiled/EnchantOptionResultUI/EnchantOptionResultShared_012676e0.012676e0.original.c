
/* ============================================================ */
/* Function: FUN_012676e0 */
/* Entry: 012676e0 */
/* Signature: undefined FUN_012676e0() */
/* ============================================================ */


undefined4 * __thiscall FUN_012676e0(undefined4 *param_1,uint param_2)

{
  *param_1 = &DAT_01ae89a4;
  param_1[0x29] = &DAT_01ae8a64;
  FUN_012327f0();
  FUN_012327f0();
  param_1[0x29] = &DAT_01ae8554;
  *param_1 = &DAT_01ad324c;
  FUN_00f1a420();
  FUN_012456d0();
  if ((param_2 & 1) != 0) {
    if ((param_2 & 4) == 0) {
      FUN_01683120();
      return param_1;
    }
    FUN_00f199b0(param_1,0xfc);
  }
  return param_1;
}


