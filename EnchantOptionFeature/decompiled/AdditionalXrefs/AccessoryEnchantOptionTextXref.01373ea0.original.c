
/* ============================================================ */
/* Function: FUN_01373ea0 */
/* Entry: 01373ea0 */
/* Signature: undefined FUN_01373ea0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_01373ea0(void)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  short *psVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined1 local_7c [20];
  undefined4 local_68;
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
  puStack_18 = &LAB_01995585;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
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
  local_30 = 0xb;
  local_58 = DAT_01bb9b34;
  local_3c = 1;
  local_2c = 0;
  local_38 = 1;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  local_24 = uVar2;
  FUN_00f19cc0(s_ACCESSORY_ENCHANT_OPTION_01af34b4,0x18);
  psVar3 = (short *)FUN_015ce480(local_7c,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
  local_14._0_1_ = 1;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar4 = psVar3;
  do {
    sVar1 = *psVar4;
    psVar4 = psVar4 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar4 - (int)(psVar3 + 1) >> 1);
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330(uVar2);
  FUN_01246bb0(&local_60,1,0x226,0x32);
  local_34 = 0;
  FUN_00f1a330();
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = 0;
  local_28 = 0;
  local_14 = 2;
  local_68 = 0;
  local_34 = DAT_01bfe468 + 8;
  local_30 = 0xb;
  local_60 = CONCAT44(DAT_01b02754,DAT_01b02618);
  local_58 = 0;
  local_3c = 1;
  local_2c = 0;
  local_38 = 1;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  FUN_00f19cc0(s_ACCESSORY_ENCHANT_SAME_OPTION_01af34d0,0x1d);
  psVar3 = (short *)FUN_015ce480(local_7c,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
  local_14._0_1_ = 3;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar4 = psVar3;
  do {
    sVar1 = *psVar4;
    psVar4 = psVar4 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar4 - (int)(psVar3 + 1) >> 1);
  local_14 = CONCAT31(local_14._1_3_,2);
  FUN_00f1a330();
  FUN_01246bb0(&local_60,1,0x226,400);
  local_34 = 0;
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


