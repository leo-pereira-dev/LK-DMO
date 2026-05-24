
/* ============================================================ */
/* Function: FUN_011ba0f0 */
/* Entry: 011ba0f0 */
/* Signature: undefined FUN_011ba0f0() */
/* ============================================================ */


void __fastcall FUN_011ba0f0(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = (int *)**(int **)(DAT_01bfe480 + 4);
  iVar3 = (*(int **)(DAT_01bfe480 + 4))[1] - (int)piVar1 >> 2;
  iVar2 = 0;
  if (0 < iVar3) {
    do {
      if (*piVar1 == 0xe) {
        FUN_00f8d670(0xf);
        break;
      }
      iVar2 = iVar2 + 1;
      piVar1 = piVar1 + 1;
    } while (iVar2 < iVar3);
  }
  iVar2 = *(int *)(param_1 + 0x78);
  *(undefined4 *)(iVar2 + 0x68) = 0;
  *(undefined4 *)(iVar2 + 0x6c) = 0;
  *(undefined4 *)(iVar2 + 0x74) = 0;
  return;
}


