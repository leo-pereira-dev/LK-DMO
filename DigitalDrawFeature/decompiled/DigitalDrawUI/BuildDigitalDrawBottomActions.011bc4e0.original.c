
/* ============================================================ */
/* Function: FUN_011bc4e0 */
/* Entry: 011bc4e0 */
/* Signature: undefined FUN_011bc4e0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011bc4e0(int param_1)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  int iVar4;
  undefined4 uVar5;
  short *psVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
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
  
  uVar5 = DAT_01b02744;
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_019915fd;
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
  local_68 = &DAT_3f7ae148;
  local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
  local_30 = 0xb;
  local_58 = &DAT_3f7ae148;
  local_3c = 1;
  local_2c = 0;
  local_38 = 1;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  local_24 = uVar2;
  FUN_00f19cc0(s_DIGISUMMON_BUY_01ae1fac,0xe);
  psVar3 = (short *)FUN_015ce480(local_7c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_14._0_1_ = 1;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330(uVar2);
  iVar4 = FUN_012462c0(s_Random_box_button_Purchase_1_tga_01adaf28,4,1,0x1c0,0x24c,0x80,0x20,0,0x3d)
  ;
  if (iVar4 != 0) {
    FUN_01224f40(&local_60);
    FUN_011bd920(1,param_1,FUN_011bd330,0);
    *(int *)(param_1 + 0x90) = iVar4;
  }
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  FUN_00f19cc0(s_RANDOM_BOX_TXT_RATEINFO_01adaa9c,0x17);
  psVar3 = (short *)FUN_015ce480(local_7c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_14._0_1_ = 2;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330();
  iVar4 = FUN_012462c0(s_Cash_NewCashShop_Store_btn_G1_tg_01ada978,4,1,0x1be,0x27e,0x84,0x26,0,0x26)
  ;
  if (iVar4 != 0) {
    FUN_01224f40(&local_60);
    FUN_011bd920(1,param_1,FUN_011bd3b0,0);
    *(int *)(param_1 + 0x94) = iVar4;
  }
  local_34 = DAT_01bfe468 + 8;
  local_68 = (undefined1 *)0x0;
  local_30 = 0xf;
  local_60 = CONCAT44(uVar5,uVar5);
  local_58 = (undefined1 *)0x0;
  local_3c = 1;
  local_2c = 0;
  local_38 = 1;
  FUN_00f1a040(&DAT_01ac59a8,1);
  uVar5 = FUN_01246bb0(&local_60,1,0x200,0x24c);
  *(undefined4 *)(param_1 + 0x98) = uVar5;
  if (*(int **)(param_1 + 0x90) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x90) + 0x1c))(0);
  }
  if (*(int **)(param_1 + 0x94) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x94) + 0x1c))(0);
  }
  if (*(int *)(param_1 + 0x98) != 0) {
    (**(code **)(*(int *)(*(int *)(param_1 + 0x98) + 0x14) + 0x1c))(0);
  }
  local_34 = 0;
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


