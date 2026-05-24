
/* ============================================================ */
/* Function: FUN_0125e980 */
/* Entry: 0125e980 */
/* Signature: undefined FUN_0125e980() */
/* ============================================================ */


void __fastcall FUN_0125e980(int *param_1)

{
  int iVar1;
  undefined4 unaff_ESI;
  
  (**(code **)(*param_1 + 0x14))();
  iVar1 = param_1[0x36];
  if (iVar1 == 0) {
    return;
  }
  if (*(int **)(iVar1 + 0xc) != (int *)0x0) {
    (**(code **)(**(int **)(iVar1 + 0xc) + 0x38))(DAT_01bc07ec,DAT_01bc07f0,unaff_ESI,iVar1);
    (**(code **)(**(int **)(iVar1 + 0xc) + 0x3c))();
  }
  if (*(int **)(iVar1 + 0x10) != (int *)0x0) {
    (**(code **)(**(int **)(iVar1 + 0x10) + 0x38))(DAT_01bc07ec,DAT_01bc07f0);
    (**(code **)(**(int **)(iVar1 + 0x10) + 0x3c))();
  }
  if (*(int **)(iVar1 + 8) != (int *)0x0) {
    (**(code **)(**(int **)(iVar1 + 8) + 0x3c))();
  }
  return;
}


