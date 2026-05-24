
/* ============================================================ */
/* Function: FUN_01268e20 */
/* Entry: 01268e20 */
/* Signature: undefined FUN_01268e20() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01268e20(int param_1,undefined4 param_2)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  int iVar4;
  short *psVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined1 local_6c [28];
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
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0197f785;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar2;
  FUN_012459a0(param_2,0,0,0,0,1,10,100,0x28a,700);
  local_34 = 0;
  local_30 = 7;
  uStack_40 = 0;
  uStack_3c = 0;
  uStack_38 = 0;
  local_44 = 0;
  local_18 = 0;
  local_8 = 0;
  local_24 = DAT_01bfe468 + 8;
  local_50 = DAT_01bb9b2c;
  local_20 = 0xb;
  local_48 = DAT_01bb9b34;
  local_2c = 1;
  local_1c = 0;
  local_28 = 1;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  FUN_00f19cc0(s_UPGRADESYSTEM_DATA_RESTORE_BTN_01ae8a8c,0x1e);
  psVar3 = (short *)FUN_015ce480(local_6c,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
  local_8._0_1_ = 1;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar5 = psVar3;
  do {
    sVar1 = *psVar5;
    psVar5 = psVar5 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar5 - (int)(psVar3 + 1) >> 1);
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330(uVar2);
  iVar4 = FUN_012462c0(s_System_Btn_Nor13_tga_01ae3e9c,0x17,1,0xdc,0x212,0xa0,0x23,0,0x25);
  *(int *)(param_1 + 0x88) = iVar4;
  if (iVar4 != 0) {
    FUN_01224f40(&local_50);
    FUN_0126c890(1,param_1,&LAB_0126c380,0);
  }
  local_8 = 0xffffffff;
  local_24 = 0;
  FUN_00f1a330();
  FUN_0126a270();
  FUN_01246720(s_EquipSystem_Mail_List_Box_1_tga_01ae8a6c,1,0x12,0x13d,0x24d,200);
  FUN_01246720(s_Control_G_Popup_N2Dlg_Line_tga_01ae8ad0,1,0x1b,0x16c,0x23c,1);
  FUN_0126ae30();
  FUN_0126b830();
  FUN_012696e0();
  FUN_0126b2e0();
  FUN_0126b5b0();
  FUN_01246720(s_Random_box_bar_arrow_png_01ae893c,1,300,0x154,0xf,0xf);
  FUN_01246720(s_Random_box_bar_arrow_png_01ae893c,1,300,400,0x1e,0x2d);
  ExceptionList = local_10;
  return;
}


