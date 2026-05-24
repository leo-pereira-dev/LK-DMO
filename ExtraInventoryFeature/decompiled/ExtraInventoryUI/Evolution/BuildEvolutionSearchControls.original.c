
/* ============================================================ */
/* Function: FUN_011a7da0 */
/* Entry: 011a7da0 */
/* Signature: undefined FUN_011a7da0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011a7da0(int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 extraout_ECX;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 local_a0;
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  int *local_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined1 *local_68;
  code *local_64;
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
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199013a;
  local_1c = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar1;
  uVar2 = FUN_01246720(s_CommonUI_Deco_Windows_Title3_tga_01adc7dc,1,0x78,3,0x96,0x1a);
  *(undefined4 *)(param_1 + 0x98) = uVar2;
  local_64 = (code *)FUN_016830c0(0x284,uVar1);
  local_14 = 0;
  if (local_64 == (code *)0x0) {
    iVar3 = 0;
  }
  else {
    memset(local_64,0,0x284);
    iVar3 = FUN_010d9880();
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0x9c) = iVar3;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 1;
    local_34 = DAT_01bfe468 + 8;
    local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_68 = &DAT_3f7ae148;
    local_a0 = DAT_01b02744;
    uStack_9c = 0;
    uStack_98 = 0;
    uStack_94 = 0;
    local_30 = 9;
    local_58 = &DAT_3f7ae148;
    local_3c = 1;
    local_2c = 0;
    local_38 = 0;
    FUN_00f1a040(&DAT_01ac5dd8,0);
    uVar2 = 0x1a;
    FUN_01238d50(*(undefined4 *)(param_1 + 0x40),&local_60,extraout_ECX,0x7d,9,0x96,0x1a);
    local_68 = &DAT_3f7ae148;
    FUN_012393c0(local_a0,local_a0,&DAT_3f7ae148,uVar2);
    local_74 = *(int **)(param_1 + 0x9c);
    local_70 = 0x3f000000;
    uVar2 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    local_64 = *(code **)(*local_74 + 100);
    uStack_6c = 0x3f000000;
    local_68 = (undefined1 *)0x3f000000;
    uVar8 = 0;
    uVar9 = 0;
    FUN_00f19cc0(s_D_COMMERCE_BUYER_TXT_SEARCH_EMPT_01adcbb8,0x29);
    puVar4 = (undefined4 *)FUN_015ce480(&local_a0,uVar2,uVar5,uVar6,uVar7,uVar8,uVar9);
    local_14._0_1_ = 2;
    if (7 < (uint)puVar4[5]) {
      puVar4 = (undefined4 *)*puVar4;
    }
    (*local_64)(puVar4,local_70,uStack_6c,local_68);
    local_14 = CONCAT31(local_14._1_3_,1);
    FUN_00f1a330();
    *(undefined1 *)(*(int *)(param_1 + 0x9c) + 0x274) = 1;
    FUN_011a8c90(1,param_1,FUN_011a8840,0);
    FUN_011a8c90(0,param_1,FUN_011a8930,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x9c));
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar3 = FUN_012462c0(s_Encyclopedia_NewEncyclopedia_mai_01add318,3,1,0xf8,4,0x16,0x16,0,0x20);
  *(int *)(param_1 + 0xa0) = iVar3;
  if (iVar3 != 0) {
    FUN_011a8c90(1,param_1,FUN_011a8b50,0);
    FUN_012254b0(*(int *)(param_1 + 0x78) + 0x50);
  }
  iVar3 = FUN_012462c0(s_Encyclopedia_NewEncyclopedia_mai_01add2ec,3,1,0x10e,4,0x16,0x16,0,0x20);
  *(int *)(param_1 + 0xa4) = iVar3;
  if (iVar3 != 0) {
    FUN_011a8c90(1,param_1,&LAB_011a72b0,0);
  }
  ExceptionList = local_1c;
  return;
}


