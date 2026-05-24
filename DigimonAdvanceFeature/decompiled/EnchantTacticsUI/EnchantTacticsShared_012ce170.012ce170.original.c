
/* ============================================================ */
/* Function: FUN_012ce170 */
/* Entry: 012ce170 */
/* Signature: undefined FUN_012ce170() */
/* ============================================================ */


void __fastcall FUN_012ce170(int *param_1)

{
  (**(code **)(*param_1 + 0x14))();
  (**(code **)(*(int *)param_1[0x5d] + 0x3c))();
  (**(code **)(*(int *)param_1[0x30] + 0x3c))();
  (**(code **)(*(int *)param_1[0x31] + 0x3c))();
  (**(code **)(*(int *)param_1[0x2f] + 0x3c))();
  if ((int *)param_1[0x2d] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x2d] + 0x3c))();
  }
  if ((int *)param_1[0x2c] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x2c] + 0x3c))();
  }
  if ((int *)param_1[0x61] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x61] + 0x14))();
  }
  if ((int *)param_1[0x62] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x62] + 0x14))();
  }
  if ((int *)param_1[0x2e] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x2e] + 0x3c))();
  }
  if ((int *)param_1[0x32] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x32] + 0x3c))();
  }
  if ((int *)param_1[0x33] != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x012ce20b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(int *)param_1[0x33] + 0x3c))();
    return;
  }
  return;
}


