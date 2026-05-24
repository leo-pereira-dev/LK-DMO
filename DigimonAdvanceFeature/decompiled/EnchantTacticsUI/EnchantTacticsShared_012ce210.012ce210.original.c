
/* ============================================================ */
/* Function: FUN_012ce210 */
/* Entry: 012ce210 */
/* Signature: undefined FUN_012ce210() */
/* ============================================================ */


void __fastcall FUN_012ce210(int param_1)

{
  FUN_01247700();
  (**(code **)(**(int **)(param_1 + 0x174) + 0x3c))();
  (**(code **)(**(int **)(param_1 + 0xc0) + 0x3c))();
  (**(code **)(**(int **)(param_1 + 0xc4) + 0x3c))();
  (**(code **)(**(int **)(param_1 + 0xbc) + 0x3c))();
  if (*(int **)(param_1 + 0xb4) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xb4) + 0x3c))();
  }
  if (*(int **)(param_1 + 0xb0) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xb0) + 0x3c))();
  }
  if (*(int **)(param_1 + 0x184) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x184) + 0x14))();
  }
  if (*(int **)(param_1 + 0x188) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x188) + 0x14))();
  }
  if (*(int **)(param_1 + 0xb8) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xb8) + 0x3c))();
  }
  if (*(int **)(param_1 + 200) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 200) + 0x3c))();
  }
  if (*(int **)(param_1 + 0xcc) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x012ce2ab. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(int **)(param_1 + 0xcc) + 0x3c))();
    return;
  }
  return;
}


