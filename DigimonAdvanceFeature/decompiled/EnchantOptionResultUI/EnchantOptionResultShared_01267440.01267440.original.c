
/* ============================================================ */
/* Function: FUN_01267440 */
/* Entry: 01267440 */
/* Signature: undefined FUN_01267440() */
/* ============================================================ */


void __fastcall FUN_01267440(int param_1)

{
  int *piVar1;
  uint uVar2;
  float10 fVar3;
  
  piVar1 = *(int **)(param_1 + 0x178);
  *(undefined1 *)(param_1 + 8) = 1;
  if (piVar1 != (int *)0x0) {
    piVar1[0x3b] = 0;
    piVar1[0x3c] = 0;
    fVar3 = (float10)fmaxf(0,0x3ecccccd);
    piVar1[0x32] = (int)(float)fVar3;
    FUN_01243010();
    (**(code **)(*piVar1 + 0x48))();
    FUN_01242bf0();
    *(undefined1 *)(*(int *)(param_1 + 0x178) + 0xe4) = 0;
  }
  if (*(int **)(param_1 + 0x158) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x158) + 0x18))(1);
  }
  if (*(int **)(param_1 + 0x160) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x160) + 0x18))(1);
  }
  uVar2 = FUN_010324e0();
  FUN_01266c80((uVar2 & 0xff ^ 1) * 2);
  return;
}


