
/* ============================================================ */
/* Function: FUN_01682880 */
/* Entry: 01682880 */
/* Signature: undefined FUN_01682880() */
/* ============================================================ */


undefined4 * __thiscall FUN_01682880(undefined4 *param_1,byte param_2)

{
  *param_1 = &DAT_01a1dc8c;
  thunk_FUN_00f1a330();
  *param_1 = &DAT_01a1848c;
  if ((param_2 & 1) != 0) {
    FUN_017986fc(param_1,0x40);
  }
  return param_1;
}


