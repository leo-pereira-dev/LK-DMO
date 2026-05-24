
/* ============================================================ */
/* Function: FUN_01607560 */
/* Entry: 01607560 */
/* Signature: undefined FUN_01607560() */
/* ============================================================ */


void __fastcall FUN_01607560(undefined4 param_1)

{
  uint uVar1;
  undefined4 *_Dst;
  uint uVar2;
  int iVar3;
  undefined4 *local_2c;
  int local_28 [5];
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019cafd5;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  FUN_00f1a2b0(u_DungeonList_bin_01a195c4);
  local_8 = 0;
  FUN_01603700(local_28,param_1,FUN_01607be0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar2 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar2) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar2 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016078b7;
    }
    FUN_017986fc(iVar3,uVar2,uVar1);
  }
  FUN_00f1a2b0(u_DungeonQualification_bin_01a195e4);
  local_8 = 1;
  FUN_01603700(local_28,param_1,FUN_016080c0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016078b7;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_DungeonObtainable_bin_01a19618);
  local_8 = 2;
  FUN_01603700(local_28,param_1,FUN_016082b0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016078b7;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_DungeonValue_bin_01a19644);
  local_8 = 3;
  FUN_01603700(local_28,param_1,FUN_01608240,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016078b7;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_DungeonReward_bin_01a19668);
  local_8 = 4;
  FUN_01603700(local_28,param_1,FUN_016080e0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016078b7;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_DungeonStepInfo_bin_01a1968c);
  local_8 = 5;
  FUN_01603700(local_28,param_1,FUN_01608100,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016078b7;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_DungeonClearInfo_bin_01a196b4);
  local_8 = 6;
  FUN_01603700(local_28,param_1,FUN_01607aa0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_016078b7;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_Dungeon_Old_bin_01a196e0);
  local_8 = 7;
  FUN_01603700(local_28,param_1,FUN_016082d0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) {
LAB_016078b7:
        local_8 = 0xffffffff;
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
    }
    FUN_017986fc(iVar3,uVar1);
  }
  _Dst = operator_new(0x40);
  if (_Dst != (undefined4 *)0x0) {
    local_2c = _Dst;
    memset(_Dst,0,0x40);
    *_Dst = &DAT_01a19704;
    _Dst[1] = param_1;
    _Dst[2] = 0;
    _Dst[3] = 0;
    _Dst[4] = 0;
    _Dst[5] = 0;
    _Dst[6] = FUN_01607970;
    _Dst[7] = 0;
    _Dst[8] = 0;
    _Dst[9] = 0;
    _Dst[10] = 0;
    _Dst[0xb] = 0;
    _Dst[0xc] = 0;
    _Dst[0xd] = 7;
    *(undefined2 *)(_Dst + 8) = 0;
    _Dst[0xe] = 9;
    if (_Dst != (undefined4 *)0x0) {
      local_2c = _Dst;
      FUN_015d0700(&local_2c);
    }
  }
  ExceptionList = local_10;
  return;
}


