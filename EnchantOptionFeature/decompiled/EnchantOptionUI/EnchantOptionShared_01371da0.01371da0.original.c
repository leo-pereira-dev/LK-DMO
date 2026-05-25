
/* ============================================================ */
/* Function: FUN_01371da0 */
/* Entry: 01371da0 */
/* Signature: undefined FUN_01371da0() */
/* ============================================================ */


undefined4 * __thiscall FUN_01371da0(undefined4 *param_1,uint param_2)

{
  *param_1 = &DAT_01af3544;
  param_1[0x29] = &DAT_01af3538;
  FID_conflict__Tidy();
  FID_conflict__Tidy();
  FID_conflict__Tidy();
  FID_conflict__Tidy();
  FID_conflict__Tidy();
  param_1[0x29] = &DAT_01ae8554;
  *param_1 = &DAT_01ad324c;
  FUN_00f1a420();
  FUN_012456d0();
  if ((param_2 & 1) != 0) {
    if ((param_2 & 4) == 0) {
      FUN_01683120();
      return param_1;
    }
    FUN_00f199b0(param_1,0x13c);
  }
  return param_1;
}


