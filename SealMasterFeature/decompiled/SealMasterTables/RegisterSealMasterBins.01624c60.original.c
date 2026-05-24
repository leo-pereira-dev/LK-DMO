
/* ============================================================ */
/* Function: FUN_01624c60 */
/* Entry: 01624c60 */
/* Signature: undefined FUN_01624c60() */
/* ============================================================ */


void __fastcall FUN_01624c60(undefined4 param_1)

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
  puStack_c = &LAB_019c81ad;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  FUN_00f1a2b0(u_SealCard_bin_01a1a0e8);
  local_8 = 0;
  FUN_016228e0(local_28,param_1,FUN_01625130,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar2 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar2) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar2 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_01624f4f;
    }
    FUN_017986fc(iVar3,uVar2,uVar1);
  }
  FUN_00f1a2b0(u_SealLeader_bin_01a1a104);
  local_8 = 1;
  FUN_016228e0(local_28,param_1,FUN_01625190,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_01624f4f;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_SealDigimonImage_bin_01a1a124);
  local_8 = 2;
  FUN_016228e0(local_28,param_1,FUN_01625150,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_01624f4f;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_SealPlateImage_bin_01a1a150);
  local_8 = 3;
  FUN_016228e0(local_28,param_1,FUN_016251d0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_01624f4f;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_SealNatureImage_bin_01a1a178);
  local_8 = 4;
  FUN_016228e0(local_28,param_1,FUN_016251b0,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_01624f4f;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_SealBaseImage_bin_01a1a1a0);
  local_8 = 5;
  FUN_016228e0(local_28,param_1,FUN_01625110,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) goto LAB_01624f4f;
    }
    FUN_017986fc(iVar3,uVar1);
  }
  FUN_00f1a2b0(u_SealFilter_bin_01a1a1c4);
  local_8 = 6;
  FUN_016228e0(local_28,param_1,FUN_01625170,0);
  local_8 = 0xffffffff;
  if (7 < local_14) {
    uVar1 = local_14 * 2 + 2;
    iVar3 = local_28[0];
    if (0xfff < uVar1) {
      iVar3 = *(int *)(local_28[0] + -4);
      uVar1 = local_14 * 2 + 0x25;
      if (0x1f < (local_28[0] - iVar3) - 4U) {
LAB_01624f4f:
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
    *_Dst = &DAT_01a1a1e8;
    _Dst[1] = param_1;
    _Dst[2] = 0;
    _Dst[3] = 0;
    _Dst[4] = 0;
    _Dst[5] = 0;
    _Dst[6] = FUN_01625000;
    _Dst[7] = 0;
    _Dst[8] = 0;
    _Dst[9] = 0;
    _Dst[10] = 0;
    _Dst[0xb] = 0;
    _Dst[0xc] = 0;
    _Dst[0xd] = 7;
    *(undefined2 *)(_Dst + 8) = 0;
    _Dst[0xe] = 0x18;
    if (_Dst != (undefined4 *)0x0) {
      local_2c = _Dst;
      FUN_015d0700(&local_2c);
    }
  }
  ExceptionList = local_10;
  return;
}


