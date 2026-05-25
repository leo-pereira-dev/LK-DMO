
/* ============================================================ */
/* Function: FUN_011d28b0 */
/* Entry: 011d28b0 */
/* Signature: undefined FUN_011d28b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

bool __fastcall FUN_011d28b0(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  char cVar5;
  int iVar6;
  int *piVar7;
  int local_10;
  uint local_c;
  
  local_c = DAT_01bbb1c0 ^ (uint)&local_10;
  piVar7 = (int *)**(int **)(param_1 + 0x8c);
  cVar5 = *(char *)((int)piVar7 + 0xd);
  local_10 = param_1;
  while (cVar5 == '\0') {
    piVar1 = (int *)piVar7[5];
    if (((piVar1 != (int *)0x0) && ((char)piVar1[0x1a] != '\0')) &&
       (iVar6 = (**(code **)(*piVar1 + 0x34))(), iVar6 != 0)) {
      return true;
    }
    piVar1 = (int *)piVar7[2];
    if (*(char *)((int)piVar1 + 0xd) == '\0') {
      cVar5 = *(char *)(*piVar1 + 0xd);
      piVar7 = piVar1;
      piVar1 = (int *)*piVar1;
      while (cVar5 == '\0') {
        cVar5 = *(char *)(*piVar1 + 0xd);
        piVar7 = piVar1;
        piVar1 = (int *)*piVar1;
      }
    }
    else {
      cVar5 = *(char *)(piVar7[1] + 0xd);
      piVar4 = (int *)piVar7[1];
      piVar1 = piVar7;
      while ((piVar7 = piVar4, cVar5 == '\0' && (piVar1 == (int *)piVar7[2]))) {
        cVar5 = *(char *)(piVar7[1] + 0xd);
        piVar4 = (int *)piVar7[1];
        piVar1 = piVar7;
      }
    }
    cVar5 = *(char *)((int)piVar7 + 0xd);
  }
  uVar2 = *(undefined4 *)(DAT_01bfe474 + 8);
  uVar3 = *(undefined4 *)(DAT_01bfe474 + 0xc);
  if (((*(int **)(local_10 + 0x84) == (int *)0x0) ||
      (iVar6 = (**(code **)(**(int **)(local_10 + 0x84) + 0x48))(uVar2,uVar3), iVar6 == 0)) ||
     (cVar5 = FUN_011d5350(*(undefined4 *)(*(int *)(local_10 + 0x84) + 0x70),uVar2,uVar3),
     cVar5 == '\0')) {
    if (*(int **)(local_10 + 0x80) != (int *)0x0) {
      (**(code **)(**(int **)(local_10 + 0x80) + 0x48))(uVar2,uVar3);
    }
    if (((*(int **)(local_10 + 0x88) == (int *)0x0) ||
        (iVar6 = (**(code **)(**(int **)(local_10 + 0x88) + 0x48))(uVar2,uVar3), iVar6 == 0)) ||
       (cVar5 = FUN_011d5590(*(undefined4 *)(*(int *)(local_10 + 0x88) + 0x70),uVar2,uVar3),
       cVar5 == '\0')) {
      if (*(int **)(local_10 + 0x94) != (int *)0x0) {
        iVar6 = (**(code **)(**(int **)(local_10 + 0x94) + 0x44))();
        return iVar6 != 0;
      }
      return false;
    }
  }
  return true;
}


