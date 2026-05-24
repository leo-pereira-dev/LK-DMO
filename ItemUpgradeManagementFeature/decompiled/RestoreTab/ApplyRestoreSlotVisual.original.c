
/* ============================================================ */
/* Function: FUN_0126c420 */
/* Entry: 0126c420 */
/* Signature: undefined FUN_0126c420() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0126c420(int param_1)

{
  int iVar1;
  undefined4 ***pppuVar2;
  undefined4 **local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0197c65d;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar1 = *(int *)(param_1 + 0x78);
  if (*(char *)(iVar1 + 0x120) != '\x01') {
    if (*(int *)(iVar1 + 0x4c) != 2) {
      *(undefined4 *)(iVar1 + 0x4c) = 0;
      FUN_012b1f10(local_14);
    }
    iVar1 = *(int *)(param_1 + 0x94);
    if (iVar1 != 0) {
      *(undefined4 *)(iVar1 + 0x18) = 10;
      *(undefined4 *)(iVar1 + 0x1c) = 0;
      *(undefined4 *)(iVar1 + 0x20) = 1;
      *(undefined4 *)(iVar1 + 0x24) = 0;
    }
    if (*(int *)(param_1 + 0x98) != 0) {
      (**(code **)(*(int *)(*(int *)(param_1 + 0x98) + 0x14) + 0x10))(&DAT_01ac5dd8);
    }
    if (*(int **)(param_1 + 0x9c) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x9c) + 0x40))(1);
    }
    iVar1 = *(int *)(param_1 + 0xa8);
    if (iVar1 != 0) {
      *(undefined4 *)(iVar1 + 0x18) = 10;
      *(undefined4 *)(iVar1 + 0x1c) = 0;
      *(undefined4 *)(iVar1 + 0x20) = 1;
      *(undefined4 *)(iVar1 + 0x24) = 0;
    }
    if (*(int *)(param_1 + 0xac) != 0) {
      (**(code **)(*(int *)(*(int *)(param_1 + 0xac) + 0x14) + 0x10))(&DAT_01ac5dd8);
    }
    if (*(int **)(param_1 + 0xb0) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0xb0) + 0x40))(1);
    }
    if (*(int *)(param_1 + 0x8c) != 0) {
      (**(code **)(*(int *)(*(int *)(param_1 + 0x8c) + 0x14) + 0x10))(&DAT_01ac5dd8);
    }
    if (*(int *)(param_1 + 0xc0) != 0) {
      (**(code **)(*(int *)(*(int *)(param_1 + 0xc0) + 0x14) + 0x10))(&DAT_01ac5dd8);
    }
    if (*(int **)(param_1 + 0xc4) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0xc4) + 0x58))(s_EquipSystem_Upgrade_Slot_tga_01ae2c40,1);
      (**(code **)(**(int **)(param_1 + 0xc4) + 0x40))(1);
    }
    if (*(int **)(param_1 + 0xbc) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0xbc) + 0x40))(0);
      iVar1 = *(int *)(param_1 + 0xbc);
      *(undefined4 *)(iVar1 + 0x18) = 10;
      *(undefined4 *)(iVar1 + 0x1c) = 0;
      *(undefined4 *)(iVar1 + 0x20) = 1;
      *(undefined4 *)(iVar1 + 0x24) = 0;
    }
    if (*(int *)(param_1 + 0xd0) != 0) {
      local_1c = 0;
      local_18 = 0;
      local_2c = (undefined4 ***)0x0;
      uStack_28 = 0;
      uStack_24 = 0;
      uStack_20 = 0;
      FUN_00f1a200(&DAT_01ac5dd8,0);
      local_8 = 0;
      FUN_01500f80(&local_2c,u__d__d_01ace96c,0,0);
      pppuVar2 = &local_2c;
      if (7 < local_18) {
        pppuVar2 = (undefined4 ***)local_2c;
      }
      (**(code **)(*(int *)(*(int *)(param_1 + 0xd0) + 0x14) + 0x10))(pppuVar2);
      local_8 = 0xffffffff;
      FUN_00f1a330();
    }
    if (*(int **)(param_1 + 0xd4) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0xd4) + 0x10))(&DAT_01ac5dd8);
    }
    if (*(int **)(param_1 + 0xd8) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0xd8) + 0x10))(&DAT_01ac5dd8);
    }
    FUN_01232880(1);
    FUN_01232880(1);
  }
  ExceptionList = local_10;
  return;
}


