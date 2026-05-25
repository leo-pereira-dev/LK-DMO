
/* ============================================================ */
/* Function: FUN_011d17c0 */
/* Entry: 011d17c0 */
/* Signature: undefined FUN_011d17c0() */
/* ============================================================ */


void __thiscall FUN_011d17c0(int param_1,int *param_2)

{
  char cVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  
  piVar4 = (int *)**(int **)(param_1 + 0x84);
  cVar1 = *(char *)((int)piVar4 + 0xd);
  while (cVar1 == '\0') {
    if ((int *)piVar4[5] != (int *)0x0) {
      (**(code **)(*(int *)piVar4[5] + 0x38))(*param_2 == piVar4[4]);
    }
    piVar2 = (int *)piVar4[2];
    if (*(char *)((int)piVar2 + 0xd) == '\0') {
      cVar1 = *(char *)(*piVar2 + 0xd);
      piVar4 = piVar2;
      piVar2 = (int *)*piVar2;
      while (cVar1 == '\0') {
        cVar1 = *(char *)(*piVar2 + 0xd);
        piVar4 = piVar2;
        piVar2 = (int *)*piVar2;
      }
    }
    else {
      cVar1 = *(char *)(piVar4[1] + 0xd);
      piVar3 = (int *)piVar4[1];
      piVar2 = piVar4;
      while ((piVar4 = piVar3, cVar1 == '\0' && (piVar2 == (int *)piVar4[2]))) {
        cVar1 = *(char *)(piVar4[1] + 0xd);
        piVar3 = (int *)piVar4[1];
        piVar2 = piVar4;
      }
    }
    cVar1 = *(char *)((int)piVar4 + 0xd);
  }
  return;
}


