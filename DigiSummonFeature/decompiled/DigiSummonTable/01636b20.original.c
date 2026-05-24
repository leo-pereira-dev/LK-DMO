
/* ============================================================ */
/* Function: FUN_01636b20 */
/* Entry: 01636b20 */
/* Signature: undefined FUN_01636b20() */
/* ============================================================ */


void __fastcall FUN_01636b20(undefined4 param_1)

{
  uint uVar1;
  undefined4 *_Dst;
  uint uVar2;
  int iVar3;
  int local_2c [5];
  uint local_18;
  undefined4 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c8b0d;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  FUN_00f1a2b0(u_DM_DigiSummon_List_bin_01a1aa80);
  local_8 = 0;
  FUN_016350a0(local_2c,param_1,FUN_01636df0,0);
  local_8 = 0xffffffff;
  if (7 < local_18) {
    uVar2 = local_18 * 2 + 2;
    iVar3 = local_2c[0];
    if (0xfff < uVar2) {
      iVar3 = *(int *)(local_2c[0] + -4);
      uVar2 = local_18 * 2 + 0x25;
      if (0x1f < (local_2c[0] - iVar3) - 4U) goto LAB_01636c6f;
    }
    FUN_017986fc(iVar3,uVar2,uVar1);
  }
  FUN_00f1a2b0(u_DM_DigiSummon_ItemList_bin_01a1aab0);
  local_8 = 1;
  FUN_016350a0(local_2c,param_1,FUN_01636dd0,0);
  local_8 = 0xffffffff;
  if (7 < local_18) {
    uVar1 = local_18 * 2 + 2;
    iVar3 = local_2c[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_2c[0] + -4);
      uVar1 = local_18 * 2 + 0x25;
      if (0x1f < (local_2c[0] - iVar3) - 4U) goto LAB_01636c6f;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_DM_DigiSummon_ItemGroup_bin_01a1aae8);
  local_8 = 2;
  FUN_016350a0(local_2c,param_1,FUN_01636e10,0);
  local_8 = 0xffffffff;
  if (7 < local_18) {
    uVar1 = local_18 * 2 + 2;
    iVar3 = local_2c[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_2c[0] + -4);
      uVar1 = local_18 * 2 + 0x25;
      if (0x1f < (local_2c[0] - iVar3) - 4U) {
LAB_01636c6f:
        local_8 = 0xffffffff;
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
    }
    FUN_017986fc(iVar3,uVar1);
  }
  _Dst = operator_new(0x40);
  if (_Dst != (undefined4 *)0x0) {
    local_14 = _Dst;
    memset(_Dst,0,0x40);
    *_Dst = &DAT_01a1ab24;
    _Dst[1] = param_1;
    _Dst[2] = 0;
    _Dst[3] = 0;
    _Dst[4] = 0;
    _Dst[5] = 0;
    _Dst[6] = FUN_01636d20;
    _Dst[7] = 0;
    _Dst[8] = 0;
    _Dst[9] = 0;
    _Dst[10] = 0;
    _Dst[0xb] = 0;
    _Dst[0xc] = 0;
    _Dst[0xd] = 7;
    *(undefined2 *)(_Dst + 8) = 0;
    _Dst[0xe] = 0x22;
    if (_Dst != (undefined4 *)0x0) {
      local_14 = _Dst;
      FUN_015d0700(&local_14);
    }
  }
  ExceptionList = local_10;
  return;
}


