
/* ============================================================ */
/* Function: FUN_012ce6a0 */
/* Entry: 012ce6a0 */
/* Signature: undefined FUN_012ce6a0() */
/* ============================================================ */


void __fastcall FUN_012ce6a0(int param_1)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  undefined4 *puVar5;
  bool bVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int iVar12;
  undefined4 uVar13;
  undefined1 local_54 [24];
  undefined1 local_3c [28];
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  uint local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019a1b8d;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  iVar4 = *(int *)(*(int *)(param_1 + 0xb8) + 0x104);
  if (iVar4 == 0) {
    iVar4 = *(int *)(param_1 + 0xa8);
    bVar6 = *(int *)(param_1 + 0x164) != 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar13 = 0;
    if (bVar6) {
      ExceptionList = &local_1c;
      FUN_00f19cc0(s_ENCHANTTACTICS_BTN_BACKUP_DISK_01aebc28,0x1e);
      puVar5 = (undefined4 *)FUN_015ce480(local_54,uVar7,uVar8,uVar9,uVar10,uVar11,uVar13);
    }
    else {
      ExceptionList = &local_1c;
      puStack_20 = &stack0xfffffffc;
      FUN_00f19cc0(s_ENCHANTTACTICS_BTN_DIGICLONE_01aebbcc,0x1c);
      puVar5 = (undefined4 *)FUN_015ce480(local_54,uVar7,uVar8,uVar9,uVar10,uVar11,uVar13);
    }
    local_14 = (uint)bVar6;
    if (7 < (uint)puVar5[5]) {
      puVar5 = (undefined4 *)*puVar5;
    }
    piVar2 = *(int **)(iVar4 + 200);
    if (piVar2 != (int *)0x0) {
      (**(code **)(*piVar2 + 0x10))(puVar5);
    }
  }
  else {
    iVar4 = iVar4 + -1;
    if (iVar4 != 0) {
      return;
    }
    iVar1 = *(int *)(param_1 + 0xa8);
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    ExceptionList = &local_1c;
    iVar12 = iVar4;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_ENCHANTTACTICS_BTN_RESET_ITEM_01aebc78,0x1d);
    puVar5 = (undefined4 *)FUN_015ce480(local_3c,uVar7,uVar8,uVar9,uVar10,iVar4,iVar12);
    local_14 = 2;
    if (7 < (uint)puVar5[5]) {
      puVar5 = (undefined4 *)*puVar5;
    }
    piVar2 = *(int **)(iVar1 + 200);
    if (piVar2 != (int *)0x0) {
      (**(code **)(*piVar2 + 0x10))(puVar5,uVar3);
    }
  }
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


