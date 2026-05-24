
/* ============================================================ */
/* Function: FUN_012ce0b0 */
/* Entry: 012ce0b0 */
/* Signature: undefined FUN_012ce0b0() */
/* ============================================================ */


undefined4 __thiscall FUN_012ce0b0(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  POINT pt;
  POINT pt_00;
  POINT pt_01;
  int *piVar2;
  BOOL BVar3;
  int iVar4;
  int iVar5;
  undefined1 local_10 [12];
  
  iVar4 = *(int *)(DAT_01bfe474 + 0xc);
  iVar5 = *(int *)(DAT_01bfe474 + 8);
  piVar2 = (int *)FUN_01247c90(local_10);
  iVar5 = iVar5 - *piVar2;
  iVar4 = iVar4 - piVar2[1];
  if (param_2 != 1) {
    return 0;
  }
  iVar1 = *(int *)(*(int *)(param_1 + 0xb8) + 0x104);
  if (iVar1 == 0) {
    pt_00.y = iVar4;
    pt_00.x = iVar5;
    BVar3 = PtInRect((RECT *)(param_1 + 0x124),pt_00);
    if (BVar3 == 1) {
      FUN_012cf2e0(param_3);
    }
    pt_01.y = iVar4;
    pt_01.x = iVar5;
    iVar4 = PtInRect((RECT *)(param_1 + 0x134),pt_01);
  }
  else {
    if (iVar1 != 1) goto LAB_012ce151;
    pt.y = iVar4;
    pt.x = iVar5;
    iVar4 = PtInRect((RECT *)(param_1 + 0x144),pt);
  }
  if (iVar4 == 1) {
    FUN_012cf2e0(param_3);
  }
LAB_012ce151:
  FUN_00ff2740();
  return 1;
}


