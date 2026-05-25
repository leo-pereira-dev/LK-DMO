
/* ============================================================ */
/* Function: FUN_015e9390 */
/* Entry: 015e9390 */
/* Signature: undefined FUN_015e9390() */
/* ============================================================ */


void __fastcall FUN_015e9390(undefined4 param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int local_28 [5];
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c8d25;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  FUN_00f1a2b0(u_AccOption_bin_01a18db8);
  local_8 = 0;
  FUN_015e8610(local_28,param_1,FUN_015e9590,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar2 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar2) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar2 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_015e953f;
    }
    FUN_017986fc(iVar3,uVar2,uVar1);
  }
  FUN_00f1a2b0(u_AccEnchant_bin_01a18dd4);
  local_8 = 1;
  FUN_015e8610(local_28,param_1,FUN_015e9570,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_015e953f;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_OptionLock_bin_01a18df4);
  local_8 = 2;
  FUN_015e8610(local_28,param_1,FUN_015e95d0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_015e953f;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_EquipEnchant_bin_01a18e14);
  local_8 = 3;
  FUN_015e8610(local_28,param_1,FUN_015e95b0,0);
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) {
LAB_015e953f:
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
    }
    FUN_017986fc(iVar3,uVar1);
  }
  ExceptionList = local_10;
  return;
}


