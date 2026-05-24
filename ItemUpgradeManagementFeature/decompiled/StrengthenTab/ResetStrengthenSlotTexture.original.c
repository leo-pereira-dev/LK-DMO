
/* ============================================================ */
/* Function: FUN_01276eb0 */
/* Entry: 01276eb0 */
/* Signature: undefined FUN_01276eb0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01276eb0(int param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 ***pppuVar3;
  int *piVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 **local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0196fbdd;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  piVar4 = (int *)(param_1 + 0xc0);
  iVar5 = 2;
  local_24 = uVar2;
  do {
    if (piVar4[-1] != 0) {
      (**(code **)(*(int *)(piVar4[-1] + 0x14) + 0x10))(&DAT_01ac5dd8,uVar2);
    }
    if ((int *)*piVar4 != (int *)0x0) {
      (**(code **)(*(int *)*piVar4 + 0x58))(s_EquipSystem_Upgrade_Slot_tga_01ae2c40,1);
      (**(code **)(*(int *)*piVar4 + 0x40))(1);
    }
    if ((int *)piVar4[-2] != (int *)0x0) {
      (**(code **)(*(int *)piVar4[-2] + 0x40))(0);
      iVar1 = piVar4[-2];
      *(undefined4 *)(iVar1 + 0x18) = 10;
      *(undefined4 *)(iVar1 + 0x1c) = 0;
      *(undefined4 *)(iVar1 + 0x20) = 1;
      *(undefined4 *)(iVar1 + 0x24) = 0;
    }
    if (piVar4[4] != 0) {
      local_2c = 0;
      local_28 = 0;
      local_3c = (undefined4 ***)0x0;
      uStack_38 = 0;
      uStack_34 = 0;
      uStack_30 = 0;
      FUN_00f1a200(&DAT_01ac5dd8,0);
      local_14 = 0;
      FUN_01500f80(&local_3c,u__d__d_01ace96c,0,0);
      pppuVar3 = &local_3c;
      if (7 < local_28) {
        pppuVar3 = (undefined4 ***)local_3c;
      }
      (**(code **)(*(int *)(piVar4[4] + 0x14) + 0x10))(pppuVar3);
      local_14 = 0xffffffff;
      FUN_00f1a330();
    }
    piVar4 = piVar4 + 7;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  iVar5 = 3;
  piVar4 = (int *)(param_1 + 0xf0);
  do {
    if (piVar4[1] != 0) {
      (**(code **)(*(int *)(piVar4[1] + 0x14) + 0x10))(&DAT_01ac5dd8);
    }
    if ((int *)piVar4[2] != (int *)0x0) {
      (**(code **)(*(int *)piVar4[2] + 0x40))(1);
    }
    if ((int *)*piVar4 != (int *)0x0) {
      (**(code **)(*(int *)*piVar4 + 0x40))(0);
      iVar1 = *piVar4;
      *(undefined4 *)(iVar1 + 0x18) = 10;
      *(undefined4 *)(iVar1 + 0x1c) = 0;
      *(undefined4 *)(iVar1 + 0x20) = 1;
      *(undefined4 *)(iVar1 + 0x24) = 0;
    }
    piVar4 = piVar4 + 3;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  uVar12 = 1;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  FUN_00f1a200(&DAT_01ac5dd8,0);
  FUN_012786f0(0,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  if (*(int *)(param_1 + 0xa0) != 0) {
    (**(code **)(*(int *)(*(int *)(param_1 + 0xa0) + 0x14) + 0x10))(&DAT_01ac5dd8);
  }
  uVar12 = 1;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  FUN_00f1a200(&DAT_01ac5dd8,0);
  FUN_012786f0(0,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  if (*(int *)(param_1 + 0xb0) != 0) {
    (**(code **)(*(int *)(*(int *)(param_1 + 0xb0) + 0x14) + 0x10))(&DAT_01ac5dd8);
  }
  if (*(int **)(param_1 + 0x114) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x114) + 0x10))(&DAT_01ac5dd8);
  }
  if (*(int **)(param_1 + 0x118) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x118) + 0x10))(&DAT_01ac5dd8);
  }
  FUN_01232880(1);
  FUN_01232880(1);
  ExceptionList = local_1c;
  return;
}


