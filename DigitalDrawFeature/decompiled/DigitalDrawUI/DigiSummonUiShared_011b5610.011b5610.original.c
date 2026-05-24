
/* ============================================================ */
/* Function: FUN_011b5610 */
/* Entry: 011b5610 */
/* Signature: undefined FUN_011b5610() */
/* ============================================================ */


void __thiscall FUN_011b5610(int param_1,int *param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  float10 fVar4;
  
  if (*(int **)(param_1 + 0x8c) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x8c) + 0x1c))((char)*param_2);
    piVar1 = *(int **)(param_1 + 0x8c);
    if ((char)*param_2 == '\0') {
      *(undefined1 *)(piVar1 + 0x39) = 0;
      return;
    }
    iVar2 = *param_2;
    iVar3 = FUN_01799060();
    piVar1[0x3b] = iVar3;
    piVar1[0x3c] = 0;
    fVar4 = (float10)fmaxf(0,(float)iVar2);
    piVar1[0x32] = (int)(float)fVar4;
    FUN_01243010();
    (**(code **)(*piVar1 + 0x48))();
    FUN_01242bf0();
    iVar2 = *(int *)(param_1 + 0x8c);
    *(undefined1 *)(iVar2 + 0xe4) = 1;
    *(undefined4 *)(iVar2 + 0xe8) = 0;
  }
  return;
}


