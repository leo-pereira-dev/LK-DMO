
/* ============================================================ */
/* Function: FUN_01643ff0 */
/* Entry: 01643ff0 */
/* Signature: undefined FUN_01643ff0() */
/* ============================================================ */


void __fastcall FUN_01643ff0(undefined4 param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  int local_2c [5];
  uint local_18;
  undefined4 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c9424;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  FUN_00f1a2b0(u_DM_Extra_Inven_Evo_Data_bin_01a1b130);
  local_8 = 0;
  puVar2 = operator_new(0x40);
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    local_14 = puVar2;
    memset(puVar2,0,0x40);
    local_8 = CONCAT31(local_8._1_3_,2);
    *puVar2 = &DAT_01a1b16c;
    puVar2[1] = param_1;
    puVar2[2] = FUN_01644200;
    puVar2[3] = 0;
    puVar2[4] = 0;
    puVar2[5] = 0;
    puVar2[6] = 0;
    puVar2[7] = 0;
    FUN_00f221a0(local_2c);
    puVar2[0xe] = 0;
  }
  local_8 = local_8 & 0xffffff00;
  local_14 = puVar2;
  if (puVar2 != (undefined4 *)0x0) {
    FUN_015d0700(&local_14);
  }
  local_8 = 0xffffffff;
  if (7 < local_18) {
    uVar3 = local_18 * 2 + 2;
    iVar4 = local_2c[0];
    if (0xfff < uVar3) {
      iVar4 = *(int *)(local_2c[0] + -4);
      uVar3 = local_18 * 2 + 0x25;
      if (0x1f < (local_2c[0] - iVar4) - 4U) {
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
    }
    FUN_017986fc(iVar4,uVar3,uVar1);
  }
  puVar2 = operator_new(0x40);
  if (puVar2 != (undefined4 *)0x0) {
    local_14 = puVar2;
    memset(puVar2,0,0x40);
    *puVar2 = &DAT_01a1b16c;
    puVar2[1] = param_1;
    puVar2[2] = 0;
    puVar2[3] = 0;
    puVar2[4] = 0;
    puVar2[5] = 0;
    puVar2[6] = FUN_01644190;
    puVar2[7] = 0;
    puVar2[8] = 0;
    puVar2[9] = 0;
    puVar2[10] = 0;
    puVar2[0xb] = 0;
    puVar2[0xc] = 0;
    puVar2[0xd] = 7;
    *(undefined2 *)(puVar2 + 8) = 0;
    puVar2[0xe] = 0x23;
    if (puVar2 != (undefined4 *)0x0) {
      local_14 = puVar2;
      FUN_015d0700(&local_14);
    }
  }
  ExceptionList = local_10;
  return;
}


