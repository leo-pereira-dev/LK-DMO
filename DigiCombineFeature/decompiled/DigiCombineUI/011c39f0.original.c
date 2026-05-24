
/* ============================================================ */
/* Function: FUN_011c39f0 */
/* Entry: 011c39f0 */
/* Signature: undefined FUN_011c39f0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011c39f0(int param_1)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  short *psVar4;
  short *psVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 local_a0;
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined1 local_7c [20];
  undefined1 *local_68;
  undefined8 local_60;
  undefined1 *local_58;
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
  puStack_18 = &LAB_01991ef8;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar2;
  iVar3 = FUN_012462c0(s_Random_box_button_Repurchase_tga_01adac48,4,1,0x280,0x2c0,0x8c,0x28,0,0x32)
  ;
  local_a0 = DAT_01b02744;
  *(int *)(param_1 + 0xa8) = iVar3;
  uStack_9c = 0;
  uStack_98 = 0;
  uStack_94 = 0;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 0;
    local_68 = &DAT_3f7ae148;
    local_34 = DAT_01bfe468 + 8;
    local_60 = CONCAT44(local_a0,local_a0);
    local_30 = 0xb;
    local_58 = &DAT_3f7ae148;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_DIGICOMBINE_ADD_CANCLE_01ae22d0,0x16);
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
    FUN_011c6530(1,param_1,FUN_011c2500,0);
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar3 = FUN_012462c0(s_Digimon_Ev_UI_Ev_Btn_tga_01ad9a64,4,1,0x366,0x247,0x55,0x1e,0,0x36);
  *(int *)(param_1 + 0xb4) = iVar3;
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
    local_68 = &DAT_3f7ae148;
    local_60 = CONCAT44(local_a0,local_a0);
    local_30 = 0xb;
    local_58 = &DAT_3f7ae148;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_RANDOM_BOX_TXT_REWARD_01adac6c,0x15);
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
    FUN_011c6530(1,param_1,FUN_011c2090,0);
    (**(code **)(**(int **)(param_1 + 0xb4) + 0x1c))(0);
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar3 = FUN_012462c0(s_Cash_NewCashShop_Store_btn_G1_tg_01ada978,4,1,0xc2,700,0x78,0x32,0,0x26);
  *(int *)(param_1 + 0xac) = iVar3;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 4;
    local_34 = DAT_01bfe468 + 8;
    local_68 = &DAT_3f7ae148;
    local_60 = CONCAT44(local_a0,local_a0);
    local_30 = 0xb;
    local_58 = &DAT_3f7ae148;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_RANDOM_BOX_TXT_RATEINFO_01adaa9c,0x17);
    psVar4 = (short *)FUN_015ce480(local_7c,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_14._0_1_ = 5;
    if (7 < *(uint *)(psVar4 + 10)) {
      psVar4 = *(short **)psVar4;
    }
    psVar5 = psVar4;
    do {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar4,(int)psVar5 - (int)(psVar4 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,4);
    FUN_00f1a330();
    FUN_01224f40(&local_60);
    FUN_011c6530(1,param_1,&LAB_011c1920,0);
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar3 = FUN_012462c0(s_Random_box_button_Purchase_1_tga_01adaf28,4,1,0x280,0x26c,0x8c,0x28,0,0x3c)
  ;
  *(int *)(param_1 + 0xa4) = iVar3;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 6;
    local_34 = DAT_01bfe468 + 8;
    local_68 = &DAT_3f7ae148;
    local_60 = CONCAT44(local_a0,local_a0);
    local_30 = 0xb;
    local_58 = &DAT_3f7ae148;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_COMMON_TXT_AUTO_REGISTER_01ad9c38,0x18);
    psVar4 = (short *)FUN_015ce480(local_7c,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_14._0_1_ = 7;
    if (7 < *(uint *)(psVar4 + 10)) {
      psVar4 = *(short **)psVar4;
    }
    psVar5 = psVar4;
    do {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar4,(int)psVar5 - (int)(psVar4 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,6);
    FUN_00f1a330();
    FUN_01224f40(&local_60);
    FUN_011c6530(1,param_1,FUN_011c2130,0);
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar3 = FUN_012462c0(s_Random_box_button_done_tga_01adac0c,4,1,0x32a,0x26c,0x8c,0x28,0,0x32);
  *(int *)(param_1 + 0xb0) = iVar3;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 8;
    local_34 = DAT_01bfe468 + 8;
    local_68 = &DAT_3f7ae148;
    local_60 = CONCAT44(local_a0,local_a0);
    local_30 = 0xb;
    local_58 = &DAT_3f7ae148;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_DIGICOMBINE_COMBINE_01ae232c,0x13);
    psVar4 = (short *)FUN_015ce480(local_7c,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_14._0_1_ = 9;
    if (7 < *(uint *)(psVar4 + 10)) {
      psVar4 = *(short **)psVar4;
    }
    psVar5 = psVar4;
    do {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar4,(int)psVar5 - (int)(psVar4 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,8);
    FUN_00f1a330();
    FUN_01224f40(&local_60);
    FUN_011c6530(1,param_1,FUN_011c1940,0);
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar3 = FUN_012462c0(s_Cash_NewCashShop_Store_btn_G1_tg_01ada978,4,1,0x32a,700,0x96,0x32,0,0x26);
  *(int *)(param_1 + 0xb8) = iVar3;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 10;
    local_34 = DAT_01bfe468 + 8;
    local_60 = CONCAT44(local_a0,local_a0);
    local_30 = 0xb;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    local_68 = &DAT_3f7ae148;
    local_58 = &DAT_3f7ae148;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_RANDOM_BOX_TXT_EXIT_01ada9bc,0x13);
    psVar4 = (short *)FUN_015ce480(&local_a0,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_14._0_1_ = 0xb;
    if (7 < *(uint *)(psVar4 + 10)) {
      psVar4 = *(short **)psVar4;
    }
    psVar5 = psVar4;
    do {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar4,(int)psVar5 - (int)(psVar4 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,10);
    FUN_00f1a330();
    FUN_01224f40(&local_60);
    local_34 = 0;
    FUN_00f1a330();
  }
  ExceptionList = local_1c;
  return;
}


