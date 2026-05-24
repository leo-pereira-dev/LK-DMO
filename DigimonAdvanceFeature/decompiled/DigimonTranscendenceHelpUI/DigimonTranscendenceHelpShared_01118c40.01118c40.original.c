
/* ============================================================ */
/* Function: FUN_01118c40 */
/* Entry: 01118c40 */
/* Signature: undefined FUN_01118c40() */
/* ============================================================ */


void __fastcall FUN_01118c40(undefined4 *param_1)

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
  return;
}


