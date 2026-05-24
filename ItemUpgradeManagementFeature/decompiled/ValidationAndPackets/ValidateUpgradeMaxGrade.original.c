
/* ============================================================ */
/* Function: FUN_012b0060 */
/* Entry: 012b0060 */
/* Signature: undefined FUN_012b0060() */
/* ============================================================ */


undefined4 __thiscall FUN_012b0060(int param_1,uint *param_2,int *param_3)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  
  if ((((*(int *)(param_1 + 0x148) != 0) && (DAT_01bfb2b4 != 0)) && (DAT_01bfb2b4 + 0x28 != 0)) &&
     ((*param_2 < 200 &&
      (puVar1 = (undefined4 *)(*param_2 * 0x45 + 0x30 + DAT_01bfb2b4 + 0x28),
      puVar1 != (undefined4 *)0x0)))) {
    iVar2 = FUN_015ec4f0(*puVar1);
    if ((iVar2 != 0) && (*param_3 == iVar2)) {
      uVar3 = 0;
      uVar4 = 0;
      uVar5 = 0;
      uVar6 = 0;
      uVar7 = 0;
      uVar8 = 0;
      FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_MAX_GRADE_01aeb164,0x1d);
      FUN_010052f0(uVar3,uVar4,uVar5,uVar6,uVar7,uVar8);
      return 1;
    }
  }
  return 0;
}


