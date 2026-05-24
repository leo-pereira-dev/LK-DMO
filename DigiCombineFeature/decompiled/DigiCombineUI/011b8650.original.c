
/* ============================================================ */
/* Function: FUN_011b8650 */
/* Entry: 011b8650 */
/* Signature: undefined FUN_011b8650() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011b8650(int param_1)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  int iVar4;
  undefined4 *puVar5;
  short *psVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 local_6c [20];
  undefined1 *local_58;
  undefined8 local_50;
  undefined1 *local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_01991305;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_34 = 0;
  local_30 = 7;
  uStack_40 = 0;
  uStack_3c = 0;
  uStack_38 = 0;
  local_44 = 0;
  local_18 = 0;
  local_8 = 0;
  local_24 = DAT_01bfe468 + 8;
  local_58 = &DAT_3f7ae148;
  local_50 = CONCAT44(DAT_01b02744,DAT_01b02744);
  local_20 = 0xb;
  local_48 = &DAT_3f7ae148;
  local_2c = 1;
  local_1c = 0;
  local_28 = 1;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  local_14 = uVar2;
  FUN_00f19cc0(s_RANDOM_BOX_TXT_CONFIRM_01adabc4,0x16);
  psVar3 = (short *)FUN_015ce480(local_6c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_8._0_1_ = 1;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330(uVar2);
  iVar4 = FUN_012462c0(s_Random_box_button_done_tga_01adac0c,4,1,0x158,0x26a,0x80,0x20,0,0x32);
  *(int *)(param_1 + 0x98) = iVar4;
  if (iVar4 != 0) {
    FUN_01224f40(&local_50);
    FUN_011bae50(1,param_1,FUN_011b9df0,0);
  }
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  FUN_00f19cc0(s_RANDOM_BOX_BUY_AGAIN_01adabf4,0x14);
  psVar3 = (short *)FUN_015ce480(local_6c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_8._0_1_ = 2;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330();
  iVar4 = FUN_012462c0(s_Random_box_button_Repurchase_tga_01adac48,4,1,0x228,0x26a,0x80,0x20,0,0x32)
  ;
  *(int *)(param_1 + 0x9c) = iVar4;
  if (iVar4 != 0) {
    FUN_01224f40(&local_50);
    puVar5 = operator_new(8);
    *puVar5 = &DAT_01ae1f88;
    puVar5[1] = 0;
    FUN_0145efe0(puVar5);
    FUN_011bae50(1,param_1,FUN_011b9e30,0);
  }
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  FUN_00f19cc0(s_DIGISUMMON_CHECK_ALL_01ae1a44,0x14);
  psVar3 = (short *)FUN_015ce480(local_6c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_8._0_1_ = 3;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330();
  iVar4 = FUN_012462c0(s_Cash_NewCashShop_Store_btn_G1_tg_01ada978,4,1,0x1c0,0x26a,0x80,0x20,0,0x26)
  ;
  *(int *)(param_1 + 0x90) = iVar4;
  if (iVar4 != 0) {
    FUN_01224f40(&local_50);
    FUN_011bae50(1,param_1,FUN_011b9e90,0);
  }
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  FUN_00f19cc0(s_RANDOM_BOX_DYNAMICEFFECT_SKIP_01adad88,0x1d);
  psVar3 = (short *)FUN_015ce480(local_6c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_8._0_1_ = 4;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330();
  iVar4 = FUN_012462c0(s_Cash_NewCashShop_Store_btn_G1_tg_01ada978,4,1,0x1c0,0x26a,0x80,0x20,0,0x26)
  ;
  *(int *)(param_1 + 0x94) = iVar4;
  if (iVar4 != 0) {
    FUN_01224f40(&local_50);
    FUN_011bae50(1,param_1,FUN_011b9f70,0);
  }
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  FUN_00f19cc0(s_NPC_TALK_DIGI_COMBINE_01ad2bd4,0x15);
  psVar3 = (short *)FUN_015ce480(local_6c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_8._0_1_ = 5;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330();
  iVar4 = FUN_012462c0(s_Cash_NewCashShop_Store_btn_G1_tg_01ada978,4,1,0x1c0,0x29c,0x80,0x20,0,0x26)
  ;
  *(int *)(param_1 + 0xa0) = iVar4;
  if (iVar4 != 0) {
    FUN_01224f40(&local_50);
    FUN_011bae50(1,param_1,FUN_011ba0f0,0);
  }
  local_24 = 0;
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


