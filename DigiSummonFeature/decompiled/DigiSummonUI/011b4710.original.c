
/* ============================================================ */
/* Function: FUN_011b4710 */
/* Entry: 011b4710 */
/* Signature: undefined FUN_011b4710() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 __thiscall FUN_011b4710(int *param_1,undefined4 param_2)

{
  short sVar1;
  char cVar2;
  undefined1 uVar3;
  int iVar4;
  short *psVar5;
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
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0197f785;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  cVar2 = (**(code **)(*param_1 + 0x20))(local_14);
  uVar3 = 0;
  if (cVar2 != '\0') {
    FUN_012459a0(param_2,0,0,3,2,1,0,0x29c,0x400,100);
    iVar4 = FUN_012462c0(s_Cash_NewCashShop_Store_btn_G1_tg_01ada978,4,1,0x34c,0x23,0x96,0x32,0,0x26
                        );
    param_1[0x22] = iVar4;
    if (iVar4 != 0) {
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
      FUN_00f19cc0(s_RANDOM_BOX_TXT_EXIT_01ada9bc,0x13);
      psVar5 = (short *)FUN_015ce480(local_6c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
      local_8._0_1_ = 1;
      if (7 < *(uint *)(psVar5 + 10)) {
        psVar5 = *(short **)psVar5;
      }
      psVar6 = psVar5;
      do {
        sVar1 = *psVar6;
        psVar6 = psVar6 + 1;
      } while (sVar1 != 0);
      FUN_00f1a040(psVar5,(int)psVar6 - (int)(psVar5 + 1) >> 1);
      local_8 = (uint)local_8._1_3_ << 8;
      FUN_00f1a330();
      FUN_01224f40(&local_50);
      local_8 = 0xffffffff;
      local_24 = 0;
      FUN_00f1a330();
    }
    FUN_011b5000();
    uVar3 = 1;
  }
  ExceptionList = local_10;
  return uVar3;
}


