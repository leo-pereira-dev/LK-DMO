
/* ============================================================ */
/* Function: FUN_011d1690 */
/* Entry: 011d1690 */
/* Signature: undefined FUN_011d1690() */
/* ============================================================ */


undefined1 __fastcall FUN_011d1690(int *param_1)

{
  char cVar1;
  int *piVar2;
  code *pcVar3;
  int *piVar4;
  undefined1 uVar5;
  int iVar6;
  int *piVar7;
  int aiStack_18 [3];
  int iStack_c;
  
  piVar7 = *(int **)param_1[0x21];
  cVar1 = *(char *)((int)piVar7 + 0xd);
  while (cVar1 == '\0') {
    piVar2 = (int *)piVar7[5];
    if (((piVar2 != (int *)0x0) && ((char)piVar2[0x1a] != '\0')) &&
       (iVar6 = (**(code **)(*piVar2 + 0x34))(), iVar6 != 0)) {
      return 1;
    }
    piVar2 = (int *)piVar7[2];
    if (*(char *)((int)piVar2 + 0xd) == '\0') {
      cVar1 = *(char *)(*piVar2 + 0xd);
      piVar7 = piVar2;
      piVar2 = (int *)*piVar2;
      while (cVar1 == '\0') {
        cVar1 = *(char *)(*piVar2 + 0xd);
        piVar7 = piVar2;
        piVar2 = (int *)*piVar2;
      }
    }
    else {
      cVar1 = *(char *)(piVar7[1] + 0xd);
      piVar4 = (int *)piVar7[1];
      piVar2 = piVar7;
      while ((piVar7 = piVar4, cVar1 == '\0' && (piVar2 == (int *)piVar7[2]))) {
        cVar1 = *(char *)(piVar7[1] + 0xd);
        piVar4 = (int *)piVar7[1];
        piVar2 = piVar7;
      }
    }
    cVar1 = *(char *)((int)piVar7 + 0xd);
  }
  if ((((int *)param_1[0x1f] == (int *)0x0) ||
      (iVar6 = (**(code **)(*(int *)param_1[0x1f] + 0x44))(), iVar6 == 0)) &&
     (((int *)param_1[0x20] == (int *)0x0 ||
      (iVar6 = (**(code **)(*(int *)param_1[0x20] + 0x44))(), iVar6 == -1)))) {
    aiStack_18[1] = 0;
    pcVar3 = *(code **)(*param_1 + 0x18);
    iVar6 = *(int *)(DAT_01bfe474 + 8);
    iStack_c = *(int *)(DAT_01bfe474 + 0xc);
    aiStack_18[0] = iVar6;
    if (param_1[0x12] != 0) {
      piVar7 = (int *)FUN_01247ce0(aiStack_18);
      uVar5 = (*pcVar3)(iVar6 - *piVar7,iStack_c - piVar7[1]);
      return uVar5;
    }
    uVar5 = (*pcVar3)(iVar6 - param_1[0xc],iStack_c - param_1[0xd]);
    return uVar5;
  }
  return 1;
}


