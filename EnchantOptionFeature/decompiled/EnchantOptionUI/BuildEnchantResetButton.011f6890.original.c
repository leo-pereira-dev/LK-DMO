
/* ============================================================ */
/* Function: FUN_011f6890 */
/* Entry: 011f6890 */
/* Signature: undefined FUN_011f6890() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011f6890(int param_1)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  short *psVar4;
  undefined4 extraout_ECX;
  short *psVar5;
  undefined4 extraout_ECX_00;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined1 local_7c [28];
  undefined8 local_60;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined1 local_3c;
  undefined4 local_38;
  int local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  int local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01995585;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar2;
  iVar3 = FUN_012462c0(s_Breakthrough_Data_Exchange_Butto_01ae44e0,0x17,1,0x31e,0x9f,0x78,0x36,0,
                       0x36);
  *(int *)(param_1 + 0xc0) = iVar3;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 0;
    local_34 = DAT_01bfe468 + 8;
    local_60 = DAT_01bb9b2c;
    local_30 = 9;
    local_58 = DAT_01bb9b34;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_ENCHANTTACTICS_BTN_RESET_01ae45b8,0x18);
    psVar4 = (short *)FUN_015ce480(local_7c,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_14._0_1_ = 1;
    if (7 < *(uint *)(psVar4 + 10)) {
      psVar4 = *(short **)psVar4;
    }
    psVar5 = psVar4;
    do {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar4,(int)psVar5 - (int)(psVar4 + 1) >> 1);
    local_14 = (uint)local_14._1_3_ << 8;
    FUN_00f1a330(uVar2);
    FUN_01224f40(&local_60);
    FUN_01223cb0(*(int *)(*(int *)(param_1 + 0xc0) + 0x10) + 0x3c,
                 *(int *)(*(int *)(param_1 + 0xc0) + 0x14) + 0x1b);
    FUN_011f9450(extraout_ECX,param_1,FUN_011f8b60,0);
    local_14 = 0xffffffff;
    *(undefined4 *)(*(int *)(param_1 + 0xc0) + 0xd8) = 3;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar3 = FUN_012462c0(s_Breakthrough_Data_Exchange_Butto_01ae44e0,0x17,1,0x31e,0xe5,0x78,0x36,0,
                       0x36);
  *(int *)(param_1 + 0xc4) = iVar3;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 2;
    local_34 = DAT_01bfe468 + 8;
    local_60 = DAT_01bb9b2c;
    local_30 = 9;
    local_58 = DAT_01bb9b34;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_Breakthrough_Change_Ability_01ae423c,0x1b);
    psVar4 = (short *)FUN_015ce480(local_7c,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_14._0_1_ = 3;
    if (7 < *(uint *)(psVar4 + 10)) {
      psVar4 = *(short **)psVar4;
    }
    psVar5 = psVar4;
    do {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar4,(int)psVar5 - (int)(psVar4 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,2);
    FUN_00f1a330();
    FUN_01224f40(&local_60);
    FUN_01223cb0(*(int *)(*(int *)(param_1 + 0xc4) + 0x10) + 0x3c,
                 *(int *)(*(int *)(param_1 + 0xc4) + 0x14) + 0x1b);
    FUN_011f9450(extraout_ECX_00,param_1,FUN_011f8bf0,0);
    *(undefined4 *)(*(int *)(param_1 + 0xc4) + 0xd8) = 3;
    local_34 = 0;
    FUN_00f1a330();
  }
  ExceptionList = local_1c;
  return;
}


