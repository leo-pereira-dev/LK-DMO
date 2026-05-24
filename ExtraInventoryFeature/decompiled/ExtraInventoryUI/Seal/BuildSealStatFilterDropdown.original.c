
/* ============================================================ */
/* Function: FUN_011ae540 */
/* Entry: 011ae540 */
/* Signature: undefined FUN_011ae540() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_011ae540(int param_1)

{
  int iVar1;
  int *piVar2;
  undefined1 *puVar3;
  void *pvVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int iVar7;
  undefined4 uVar8;
  int extraout_ECX;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined8 uVar12;
  int local_5c;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01990853;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  puVar3 = &stack0xfffffffc;
  if (*(int *)(param_1 + 0x88) == 0) {
    pvVar4 = (void *)FUN_016830c0(0x218,local_24);
    local_14 = 0;
    if (pvVar4 == (void *)0x0) {
      local_5c = 0;
    }
    else {
      memset(pvVar4,0,0x218);
      local_5c = FUN_012276b0();
    }
    local_14 = 0xffffffff;
    *(int *)(param_1 + 0x88) = local_5c;
    if (local_5c == 0) {
      ExceptionList = local_1c;
      return;
    }
    uVar6 = *(undefined4 *)(param_1 + 0x40);
    FUN_012228b0(uVar6,0xf,5,0xcc,0x16,s_Control_G_ComboBox_Combo_tga_01ae65fc,0);
    FUN_01223e30();
    *(undefined4 *)(local_5c + 0x1c8) = 0;
    *(undefined4 *)(local_5c + 0x1cc) = 0x16;
    puVar5 = (undefined4 *)FUN_016830c0(0x8c);
    local_14 = 1;
    if (puVar5 == (undefined4 *)0x0) {
      puVar5 = (undefined4 *)0x0;
    }
    else {
      memset(puVar5,0,0x8c);
      FUN_0145eca0();
      *puVar5 = &DAT_01ad5210;
      puVar5[0x1d] = 0;
      puVar5[0x1e] = 0;
      puVar5[0x1f] = 0;
      puVar5[0x20] = 0;
      puVar5[0x21] = 0;
      puVar5[0x22] = 0;
      puVar5[0x14] = 0;
      puVar5[0x15] = 0;
      puVar5[0x16] = 0;
      puVar5[0x17] = 0;
      puVar5[0x18] = 0;
      puVar5[0x19] = 0;
      puVar5[0x1a] = 0;
      puVar5[0x1b] = 0;
      puVar5[0x1c] = 0;
    }
    local_14 = 0xffffffff;
    *(undefined4 **)(local_5c + 0x1c0) = puVar5;
    FUN_01259fa0(uVar6,s_Control_G_Popup_ON2Dlg_01ae661c,&DAT_01ac8f80,0,
                 *(int *)(local_5c + 0x1c8) + 0xf,*(int *)(local_5c + 0x1cc) + 5,0xcc,10,0x10,0x10);
    pvVar4 = (void *)FUN_016830c0(0xa0);
    local_14 = 2;
    if (pvVar4 == (void *)0x0) {
      uVar6 = 0;
    }
    else {
      memset(pvVar4,0,0xa0);
      uVar6 = FUN_0123de10();
    }
    local_14 = 0xffffffff;
    *(undefined4 *)(local_5c + 0x1c4) = uVar6;
    iVar7 = FUN_01446f10(9);
    uVar11 = DAT_01bee890;
    uVar9 = DAT_01bee88c;
    uVar6 = DAT_01bb9b34;
    uVar12 = DAT_01bb9b2c;
    iVar1 = *(int *)(iVar7 * 4 + 0x40 + extraout_ECX);
    FUN_00f19f10();
    local_2c = 0;
    uStack_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    local_28 = 0xf;
    local_3c = 0;
    local_14 = 3;
    FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_Talk_Common_Seletbar_tga_01ad36a8);
    FUN_0124ef30(&local_3c);
    uVar10 = 0;
    uVar8 = FUN_0124f150(0,uVar12,uVar6);
    FUN_0123e330(0,uVar9,uVar11,0xcc,iVar1 + 2,uVar8,uVar10,uVar12,uVar6);
    local_14 = 0xffffffff;
    FUN_00f1a420();
    iVar1 = DAT_01bfe468;
    *(undefined2 *)(local_5c + 0x1d8) = 0;
    *(undefined4 *)(local_5c + 0x1fc) = 9;
    *(undefined4 *)(local_5c + 0x200) = 1;
    *(undefined4 *)(local_5c + 0x20c) = 0xffffffff;
    *(undefined4 *)(local_5c + 0x210) = 0;
    *(undefined2 *)(local_5c + 0x214) = 0;
    *(int *)(local_5c + 0x208) = (0x16 - *(int *)(iVar7 * 4 + 0x40 + iVar1)) / 2;
    FUN_01227910(s_SealMaster_btn_combobox_tga_01ad7abc,0,0x16);
    FUN_01227a10(s_SealMaster_dropdown_list_0_01ad7b14);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x88));
    puVar3 = puStack_20;
  }
  puStack_20 = puVar3;
  iVar1 = *(int *)(param_1 + 0x88);
  FUN_01232880(1);
  *(undefined4 *)(iVar1 + 0x20c) = 0xffffffff;
  *(undefined4 *)(iVar1 + 0x210) = 0;
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_ALL_01ae13ac,0x1a);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 4;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,0,0x3f7ae14800000000,0);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_HP_01ae1400,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 5;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,1,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_DS_01ae13e4,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 6;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,2,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_AT_01ae1438,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 7;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,3,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_AS_01ae141c,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 8;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,4,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_CT_01ae1470,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 9;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,5,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_HT_01ae1454,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 10;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,6,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_DE_01ae14a8,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 0xb;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,7,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_BL_01ae148c,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 0xc;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,8,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar6 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_EV_01ae14f4,0x19);
  puVar5 = (undefined4 *)FUN_015ce480(&local_3c,uVar6,uVar9,uVar11,uVar12,uVar8);
  local_14 = 0xd;
  if (7 < (uint)puVar5[5]) {
    puVar5 = (undefined4 *)*puVar5;
  }
  FUN_012281c0(puVar5,9,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  iVar1 = *(int *)(param_1 + 0x88);
  if ((0 < *(int *)(iVar1 + 0x1e8)) && (*(int *)(iVar1 + 0x20c) != 0)) {
    *(undefined4 *)(iVar1 + 0x20c) = 0;
    piVar2 = (int *)**(int **)(iVar1 + 0x1e4);
    if (piVar2 == *(int **)(iVar1 + 0x1e4)) {
      uVar6 = 0;
    }
    else {
      uVar6 = piVar2[2];
    }
    *(undefined4 *)(iVar1 + 0x210) = uVar6;
    FUN_00f1b390();
    if ((((DAT_3f7ae155 == '\0') && (_DAT_3f7ae158 < 4)) &&
        (*(undefined1 **)(iVar1 + 4) != &DAT_3f7ae148)) && (_DAT_3f7ae15c != (int *)0x0)) {
      (**(code **)(*_DAT_3f7ae15c + 4))((undefined4 *)(iVar1 + 0x20c));
    }
  }
  FUN_011afe70(3,param_1,&LAB_011aef40,0);
  ExceptionList = local_1c;
  return;
}


