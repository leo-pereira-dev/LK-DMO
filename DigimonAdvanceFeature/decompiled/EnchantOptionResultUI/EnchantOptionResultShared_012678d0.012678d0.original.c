
/* ============================================================ */
/* Function: FUN_012678d0 */
/* Entry: 012678d0 */
/* Signature: undefined FUN_012678d0() */
/* ============================================================ */


void __fastcall FUN_012678d0(int *param_1)

{
  if ((char)param_1[0x1d] != '\0') {
    (**(code **)(*param_1 + 0x30))();
  }
  param_1[0x1b] = DAT_01bee88c;
  param_1[0x1c] = DAT_01bee890;
  *(undefined2 *)(param_1 + 0x1d) = 0;
  FUN_01232880(1);
  FUN_01232880(1);
  return;
}


