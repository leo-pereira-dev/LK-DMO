
/* ============================================================ */
/* Function: FUN_011fbcd0 */
/* Entry: 011fbcd0 */
/* Signature: undefined FUN_011fbcd0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 __thiscall FUN_011fbcd0(int param_1,undefined4 param_2)

{
  short sVar1;
  char cVar2;
  undefined1 uVar3;
  undefined4 *puVar4;
  short *psVar5;
  short *psVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined1 local_88 [28];
  undefined1 *local_6c;
  undefined1 *puStack_68;
  undefined4 local_64;
  longlong local_60;
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
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01991b1d;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  cVar2 = FUN_011fba10(local_24);
  uVar3 = 0;
  if (cVar2 != '\0') {
    FUN_012459a0(param_2,0,0,3,3,1,*(undefined4 *)(param_1 + 0x30),*(undefined4 *)(param_1 + 0x34),
                 DAT_01bee88c,DAT_01bee890);
    FUN_01246720(s_Random_box_popup_rate_popup_rate_01ae2224,1,600,0x78,0x18e,0x23a);
    local_6c = &DAT_3f7ae148;
    puStack_68 = &DAT_3f7ae148;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    local_64 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_DIGICOMBINE_POSSIBLE_ITEM_01ae2270,0x19);
    puVar4 = (undefined4 *)FUN_015ce480(local_88,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_14 = 0;
    if (7 < (uint)puVar4[5]) {
      puVar4 = (undefined4 *)*puVar4;
    }
    FUN_012461a0(puVar4,0xb,CONCAT44(puStack_68,local_6c),local_64,800,0x82);
    FUN_00f1a330();
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 1;
    local_64 = 0;
    local_34 = DAT_01bfe468 + 8;
    local_60 = (ulonglong)DAT_01b02744 << 0x20;
    local_30 = 9;
    local_58 = 0;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    FUN_00f19cc0(s_DIGICOMBINE_COMBINE_DESC_01ae2254,0x18);
    psVar5 = (short *)FUN_015ce480(local_88,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_14._0_1_ = 2;
    if (7 < *(uint *)(psVar5 + 10)) {
      psVar5 = *(short **)psVar5;
    }
    psVar6 = psVar5;
    do {
      sVar1 = *psVar6;
      psVar6 = psVar6 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar5,(int)psVar6 - (int)(psVar5 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,1);
    FUN_00f1a330();
    FUN_01246bb0();
    FUN_011fd5f0();
    FUN_011fe570();
    FUN_011fcd50();
    FUN_011fea60();
    FUN_011fdd70();
    local_34 = 0;
    FUN_00f1a330();
    uVar3 = 1;
  }
  ExceptionList = local_1c;
  return uVar3;
}


