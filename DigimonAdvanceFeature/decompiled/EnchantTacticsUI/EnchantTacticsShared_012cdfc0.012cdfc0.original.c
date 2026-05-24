
/* ============================================================ */
/* Function: FUN_012cdfc0 */
/* Entry: 012cdfc0 */
/* Signature: undefined FUN_012cdfc0() */
/* ============================================================ */


void __fastcall FUN_012cdfc0(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  POINT pt;
  int *piVar5;
  BOOL BVar6;
  RECT *lprc;
  int iVar7;
  undefined1 local_10 [12];
  
  iVar1 = *(int *)(DAT_01bfe474 + 8);
  iVar2 = *(int *)(DAT_01bfe474 + 0xc);
  piVar5 = (int *)FUN_01247c90(local_10);
  lprc = (RECT *)(param_1 + 0xd4);
  iVar3 = *piVar5;
  iVar4 = piVar5[1];
  iVar7 = 0;
  while ((iVar7 == *(int *)(param_1 + 0x178) ||
         (pt.y = iVar2 - iVar4, pt.x = iVar1 - iVar3, BVar6 = PtInRect(lprc,pt), BVar6 != 1))) {
    iVar7 = iVar7 + 1;
    lprc = lprc + 1;
    if (4 < iVar7) {
      FUN_012ce6a0();
      return;
    }
  }
  *(int *)(param_1 + 0x178) = iVar7;
  if (*(int *)(*(int *)(param_1 + 0xb8) + 0x104) == 0) {
    if (*(int *)(param_1 + 0x164) != 0) {
      FUN_00f70840(*(undefined4 *)(param_1 + 0x168));
    }
    *(undefined4 *)(param_1 + 0x164) = 0;
    *(undefined4 *)(param_1 + 0x168) = 0x7fffffff;
    FUN_012ce800();
  }
  FUN_012ce6a0();
  return;
}


