
/* ============================================================ */
/* Function: FUN_01630020 */
/* Entry: 01630020 */
/* Signature: undefined FUN_01630020() */
/* ============================================================ */


void __fastcall FUN_01630020(undefined4 param_1)

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
  FUN_00f1a2b0(u_DM_DigiCombine_ItemList_bin_01a1a67c);
  local_8 = 0;
  FUN_0162e3a0(local_28,param_1,FUN_01630200,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar2 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar2) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar2 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016301cf;
    }
    FUN_017986fc(iVar3,uVar2,uVar1);
  }
  FUN_00f1a2b0(u_DM_DigiCombine_RankList_bin_01a1a6b4);
  local_8 = 1;
  FUN_0162e3a0(local_28,param_1,FUN_01630220,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016301cf;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_DM_DigiCombine_ItemGroup_bin_01a1a6ec);
  local_8 = 2;
  FUN_0162e3a0(local_28,param_1,FUN_01630260,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016301cf;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_DM_DigiCombine_CeilGroup_bin_01a1a728);
  local_8 = 3;
  FUN_0162e3a0(local_28,param_1,FUN_01630240,0);
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) {
LAB_016301cf:
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
    }
    FUN_017986fc(iVar3,uVar1);
  }
  ExceptionList = local_10;
  return;
}


