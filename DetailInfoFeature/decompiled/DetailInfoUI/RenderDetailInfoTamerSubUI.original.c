
/* ============================================================ */
/* Function: FUN_011dad40 */
/* Entry: 011dad40 */
/* Signature: undefined FUN_011dad40() */
/* ============================================================ */


undefined4 __fastcall FUN_011dad40(int param_1)

{
  char cVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  undefined4 uVar5;
  int *piVar6;
  
  piVar6 = (int *)**(int **)(param_1 + 0x8c);
  cVar1 = *(char *)((int)piVar6 + 0xd);
  while (cVar1 == '\0') {
    piVar2 = (int *)piVar6[5];
    if (((piVar2 != (int *)0x0) && ((char)piVar2[0x1a] != '\0')) &&
       (iVar4 = (**(code **)(*piVar2 + 0x34))(), iVar4 != 0)) {
      return 1;
    }
    piVar2 = (int *)piVar6[2];
    if (*(char *)((int)piVar2 + 0xd) == '\0') {
      cVar1 = *(char *)(*piVar2 + 0xd);
      piVar6 = piVar2;
      piVar2 = (int *)*piVar2;
      while (cVar1 == '\0') {
        cVar1 = *(char *)(*piVar2 + 0xd);
        piVar6 = piVar2;
        piVar2 = (int *)*piVar2;
      }
    }
    else {
      cVar1 = *(char *)(piVar6[1] + 0xd);
      piVar3 = (int *)piVar6[1];
      piVar2 = piVar6;
      while ((piVar6 = piVar3, cVar1 == '\0' && (piVar2 == (int *)piVar6[2]))) {
        cVar1 = *(char *)(piVar6[1] + 0xd);
        piVar3 = (int *)piVar6[1];
        piVar2 = piVar6;
      }
    }
    cVar1 = *(char *)((int)piVar6 + 0xd);
  }
  if ((*(int **)(param_1 + 0x88) != (int *)0x0) &&
     (iVar4 = (**(code **)(**(int **)(param_1 + 0x88) + 0x48))
                        (*(undefined4 *)(DAT_01bfe474 + 8),*(undefined4 *)(DAT_01bfe474 + 0xc)),
     iVar4 != 0)) {
    return 1;
  }
  if ((*(int **)(param_1 + 0x7c) != (int *)0x0) &&
     (iVar4 = (**(code **)(**(int **)(param_1 + 0x7c) + 0x48))
                        (*(undefined4 *)(DAT_01bfe474 + 8),*(undefined4 *)(DAT_01bfe474 + 0xc)),
     iVar4 != 0)) {
    uVar5 = FUN_011dcf20(*(undefined4 *)(*(int *)(param_1 + 0x7c) + 0x70));
    return uVar5;
  }
  if ((*(int **)(param_1 + 0x80) != (int *)0x0) &&
     (iVar4 = (**(code **)(**(int **)(param_1 + 0x80) + 0x48))
                        (*(undefined4 *)(DAT_01bfe474 + 8),*(undefined4 *)(DAT_01bfe474 + 0xc)),
     iVar4 != 0)) {
    uVar5 = FUN_011dcf20(*(undefined4 *)(*(int *)(param_1 + 0x80) + 0x70));
    return uVar5;
  }
  return 0;
}


