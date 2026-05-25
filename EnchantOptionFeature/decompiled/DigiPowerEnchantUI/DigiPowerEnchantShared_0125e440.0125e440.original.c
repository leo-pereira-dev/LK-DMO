
/* ============================================================ */
/* Function: FUN_0125e440 */
/* Entry: 0125e440 */
/* Signature: undefined FUN_0125e440() */
/* ============================================================ */


void __fastcall FUN_0125e440(int *param_1)

{
  if (param_1[0x2a] != 0) {
    FUN_00ff8af0(-(uint)(param_1 != (int *)0x0) & (uint)(param_1 + 0x29));
  }
  (**(code **)(*param_1 + 4))();
  if ((undefined4 *)param_1[0x36] != (undefined4 *)0x0) {
    (*(code *)**(undefined4 **)param_1[0x36])(1);
    param_1[0x36] = 0;
  }
  param_1[0x37] = 0;
  return;
}


