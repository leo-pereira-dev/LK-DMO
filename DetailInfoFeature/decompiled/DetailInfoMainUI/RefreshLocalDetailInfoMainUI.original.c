
/* ============================================================ */
/* Function: FUN_011d8c20 */
/* Entry: 011d8c20 */
/* Signature: undefined FUN_011d8c20() */
/* ============================================================ */


void __fastcall FUN_011d8c20(int param_1)

{
  char cVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  undefined4 local_c [2];
  
  FUN_012473e0(*(undefined4 *)(param_1 + 0xb0));
  *(undefined4 *)(param_1 + 0xb0) = 0;
  FUN_011d82a0();
  piVar4 = (int *)**(int **)(param_1 + 0xb8);
  cVar1 = *(char *)((int)piVar4 + 0xd);
  while (cVar1 == '\0') {
    if ((int *)piVar4[5] != (int *)0x0) {
      (**(code **)(*(int *)piVar4[5] + 0x40))(*(undefined4 *)(param_1 + 0x40),0,0x3f,0x1cf,0x25d);
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
  local_c[0] = 0;
  FUN_0122a840(local_c,1);
  piVar4 = (int *)**(int **)(param_1 + 0xb8);
  cVar1 = *(char *)((int)piVar4 + 0xd);
  while (cVar1 == '\0') {
    if ((int *)piVar4[5] != (int *)0x0) {
      (**(code **)(*(int *)piVar4[5] + 0x38))(piVar4[4] == 0);
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


