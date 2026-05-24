
/* ============================================================ */
/* Function: FUN_01267bd0 */
/* Entry: 01267bd0 */
/* Signature: undefined FUN_01267bd0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01267bd0(int param_1)

{
  short sVar1;
  uint uVar2;
  void *_Dst;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  short *psVar6;
  short *psVar7;
  undefined4 local_54;
  undefined8 local_50;
  undefined4 local_48;
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
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199b632;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar2;
  FUN_012457d0(0,1,0,0,1,(DAT_01bc07ec + -500) / 2,(DAT_01bc07f0 + -300) / 2,500,300);
  uVar4 = DAT_01bee890;
  local_54 = DAT_01bee88c;
  _Dst = (void *)FUN_016830c0(0xa0,uVar2);
  local_8 = 0;
  if (_Dst == (void *)0x0) {
    uVar3 = 0;
  }
  else {
    memset(_Dst,0,0xa0);
    uVar3 = FUN_0123de10();
  }
  local_8 = 0xffffffff;
  FUN_0123e0d0(*(undefined4 *)(param_1 + 0x40),s_SeasonPass_Pass_ExpPopup_bg_png_01ad7e6c,0,
               DAT_01bb9b2c,DAT_01bb9b34,1,local_54,uVar4,500,300);
  local_54 = uVar3;
  FUN_00f55430(&local_54);
  *(undefined4 *)(param_1 + 0xac) = uVar3;
  FUN_00f19cc0(s_ACCESSORY_ENCHANT_RESULT_01ae8958,0x18);
  FUN_015ce480();
  local_8 = 1;
  uVar4 = FUN_012461a0();
  *(undefined4 *)(param_1 + 0xb4) = uVar4;
  local_8 = 0xffffffff;
  FUN_00f1a330();
  uVar4 = FUN_01246720(s_Random_box_bar_arrow_png_01ae893c,1,0xeb,0x6e,0x1e,0x2d);
  *(undefined4 *)(param_1 + 0xb0) = uVar4;
  iVar5 = FUN_012462c0(s_Accessory_Enchant_Enchant_Enchan_01ae83e0,0x17,1,0xcd,0xfa,0x65,0x21,0,0x1a
                      );
  *(int *)(param_1 + 0xf8) = iVar5;
  if (iVar5 != 0) {
    local_34 = 0;
    local_30 = 7;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    local_44 = 0;
    local_18 = 0;
    local_8 = 2;
    local_24 = DAT_01bfe468 + 8;
    local_50 = DAT_01bb9b2c;
    local_20 = 9;
    local_48 = DAT_01bb9b34;
    local_2c = 1;
    local_1c = 0;
    local_28 = 1;
    FUN_00f19cc0(s_SECONDPASSWORD_STRING_CONFIRM_01ae8974,0x1d);
    psVar6 = (short *)FUN_015ce480();
    local_8._0_1_ = 3;
    if (7 < *(uint *)(psVar6 + 10)) {
      psVar6 = *(short **)psVar6;
    }
    psVar7 = psVar6;
    do {
      sVar1 = *psVar7;
      psVar7 = psVar7 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar6,(int)psVar7 - (int)(psVar6 + 1) >> 1);
    local_8 = CONCAT31(local_8._1_3_,2);
    FUN_00f1a330();
    FUN_01224f40(&local_50);
    FUN_01268320();
    local_24 = 0;
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


