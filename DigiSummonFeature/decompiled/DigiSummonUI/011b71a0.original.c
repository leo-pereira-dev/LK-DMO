
/* ============================================================ */
/* Function: FUN_011b71a0 */
/* Entry: 011b71a0 */
/* Signature: undefined FUN_011b71a0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011b71a0(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  short *psVar5;
  short *psVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 local_88 [20];
  undefined1 *local_74;
  undefined1 *local_64;
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
  puStack_18 = &LAB_01991145;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar3;
  iVar4 = FUN_01246da0(s_Random_box_button_Tap_tga_01adaa80,param_1,0x14,0x7a,0xe0,0x30,0,0x30);
  *(int *)(param_1 + 0x84) = iVar4;
  uVar2 = DAT_01b02744;
  if (iVar4 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 0;
    local_34 = DAT_01bfe468 + 8;
    local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_30 = 0xb;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    local_64 = &DAT_3f7ae148;
    local_58 = &DAT_3f7ae148;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    FUN_00f19cc0(s_DIGISUMMON_INFO_RANDOM_LIST_01ae19f8,0x1b);
    psVar5 = (short *)FUN_015ce480(local_88,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    local_14._0_1_ = 1;
    if (7 < *(uint *)(psVar5 + 10)) {
      psVar5 = *(short **)psVar5;
    }
    psVar6 = psVar5;
    do {
      sVar1 = *psVar6;
      psVar6 = psVar6 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar5,(int)psVar6 - (int)(psVar5 + 1) >> 1);
    local_14 = (uint)local_14._1_3_ << 8;
    FUN_00f1a330(uVar3);
    FUN_01226310(&local_60,DAT_01bee88c,DAT_01bee890);
    (**(code **)(**(int **)(param_1 + 0x84) + 0x18))(1);
    iVar4 = *(int *)(param_1 + 0x84);
    local_64 = (undefined1 *)0x0;
    *(ulonglong *)(iVar4 + 0x11c) = CONCAT44(uVar2,uVar2);
    *(undefined4 *)(iVar4 + 0x124) = 0;
    *(ulonglong *)(iVar4 + 0x128) = CONCAT44(uVar2,uVar2);
    local_74 = &DAT_3f7ae148;
    *(undefined1 **)(iVar4 + 0x130) = &DAT_3f7ae148;
    FUN_01227420(0,0);
    local_34 = 0;
    FUN_00f1a330();
  }
  ExceptionList = local_1c;
  return;
}


