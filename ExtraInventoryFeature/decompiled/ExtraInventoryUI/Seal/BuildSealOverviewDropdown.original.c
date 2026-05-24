
/* ============================================================ */
/* Function: FUN_011a60d0 */
/* Entry: 011a60d0 */
/* Signature: undefined FUN_011a60d0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_011a60d0(int param_1)

{
  int *piVar1;
  undefined1 *puVar2;
  void *pvVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
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
  puStack_18 = &LAB_0198ff7b;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  puVar2 = &stack0xfffffffc;
  if (*(int *)(param_1 + 0x88) == 0) {
    pvVar3 = (void *)FUN_016830c0(0x218,local_24);
    local_14 = 0;
    if (pvVar3 == (void *)0x0) {
      local_5c = 0;
    }
    else {
      memset(pvVar3,0,0x218);
      local_5c = FUN_012276b0();
    }
    local_14 = 0xffffffff;
    *(int *)(param_1 + 0x88) = local_5c;
    if (local_5c == 0) {
      ExceptionList = local_1c;
      return;
    }
    uVar5 = *(undefined4 *)(param_1 + 0x40);
    FUN_012228b0(uVar5,0xf,5,0x68,0x16,s_Control_G_ComboBox_Combo_tga_01ae65fc,0);
    FUN_01223e30();
    *(undefined4 *)(local_5c + 0x1c8) = 0;
    *(undefined4 *)(local_5c + 0x1cc) = 0x16;
    puVar4 = (undefined4 *)FUN_016830c0(0x8c);
    local_14 = 1;
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = (undefined4 *)0x0;
    }
    else {
      memset(puVar4,0,0x8c);
      FUN_0145eca0();
      *puVar4 = &DAT_01ad5210;
      puVar4[0x1d] = 0;
      puVar4[0x1e] = 0;
      puVar4[0x1f] = 0;
      puVar4[0x20] = 0;
      puVar4[0x21] = 0;
      puVar4[0x22] = 0;
      puVar4[0x14] = 0;
      puVar4[0x15] = 0;
      puVar4[0x16] = 0;
      puVar4[0x17] = 0;
      puVar4[0x18] = 0;
      puVar4[0x19] = 0;
      puVar4[0x1a] = 0;
      puVar4[0x1b] = 0;
      puVar4[0x1c] = 0;
    }
    local_14 = 0xffffffff;
    *(undefined4 **)(local_5c + 0x1c0) = puVar4;
    FUN_01259fa0(uVar5,s_Control_G_Popup_ON2Dlg_01ae661c,&DAT_01ac8f80,0,
                 *(int *)(local_5c + 0x1c8) + 0xf,*(int *)(local_5c + 0x1cc) + 5,0x68,10,0x10,0x10);
    pvVar3 = (void *)FUN_016830c0(0xa0);
    local_14 = 2;
    if (pvVar3 == (void *)0x0) {
      uVar5 = 0;
    }
    else {
      memset(pvVar3,0,0xa0);
      uVar5 = FUN_0123de10();
    }
    local_14 = 0xffffffff;
    *(undefined4 *)(local_5c + 0x1c4) = uVar5;
    iVar6 = FUN_01446f10(9);
    uVar11 = DAT_01bee890;
    uVar9 = DAT_01bee88c;
    uVar5 = DAT_01bb9b34;
    uVar12 = DAT_01bb9b2c;
    iVar8 = *(int *)(iVar6 * 4 + 0x40 + extraout_ECX);
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
    uVar7 = FUN_0124f150(0,uVar12,uVar5);
    FUN_0123e330(0,uVar9,uVar11,0x68,iVar8 + 2,uVar7,uVar10,uVar12,uVar5);
    local_14 = 0xffffffff;
    FUN_00f1a420();
    iVar8 = DAT_01bfe468;
    *(undefined2 *)(local_5c + 0x1d8) = 0;
    *(undefined4 *)(local_5c + 0x1fc) = 9;
    *(undefined4 *)(local_5c + 0x200) = 1;
    *(undefined4 *)(local_5c + 0x20c) = 0xffffffff;
    *(undefined4 *)(local_5c + 0x210) = 0;
    *(undefined2 *)(local_5c + 0x214) = 0;
    *(int *)(local_5c + 0x208) = (0x16 - *(int *)(iVar6 * 4 + 0x40 + iVar8)) / 2;
    FUN_01227910(s_SealMaster_btn_combobox_tga_01ad7abc,0,0x16);
    iVar8 = 0xb;
    if (*(int *)(*(int *)(param_1 + 0x88) + 0x200) != 2) {
      iVar8 = -0xb;
    }
    piVar1 = (int *)(*(int *)(param_1 + 0x88) + 0x204);
    *piVar1 = *piVar1 + iVar8;
    FUN_01227a10(s_SealMaster_dropdown_list_0_01ad7b14);
    FUN_01224210();
    FUN_012472c0(*(undefined4 *)(param_1 + 0x88));
    puVar2 = puStack_20;
  }
  puStack_20 = puVar2;
  iVar8 = *(int *)(param_1 + 0x88);
  FUN_01232880(1);
  *(undefined4 *)(iVar8 + 0x20c) = 0xffffffff;
  *(undefined4 *)(iVar8 + 0x210) = 0;
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_SEALMASTER_SHOW_ALL_01ae0fbc,0x13);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 4;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,0,0x3f7ae14800000000,0);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_N_01ad0628,0x14);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 5;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,1,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_A_01ad0640,0x14);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 6;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,2,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_APLUS_01ad0658,0x18);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 7;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,3,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_S_01ad0674,0x14);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 8;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,4,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_SPLUS_01ad068c,0x18);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 9;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,5,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_SS_01ad06a8,0x15);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 10;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,6,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_SSPLUS_01ad06c0,0x19);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 0xb;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,7,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_SSS_01ad06dc,0x16);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 0xc;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,8,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_SSSPLUS_01ad06f4,0x1a);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 0xd;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,9,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  uVar5 = 0;
  uVar9 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_ENCY_DIGIMON_GRADE_U_01ad0710,0x14);
  puVar4 = (undefined4 *)FUN_015ce480(&local_3c,uVar5,uVar9,uVar11,uVar12,uVar7);
  local_14 = 0xe;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012281c0(puVar4,10,0x3f7ae1483f7ae148,&DAT_3f7ae148);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  iVar8 = *(int *)(param_1 + 0x88);
  if ((0 < *(int *)(iVar8 + 0x1e8)) && (*(int *)(iVar8 + 0x20c) != 0)) {
    *(undefined4 *)(iVar8 + 0x20c) = 0;
    piVar1 = (int *)**(int **)(iVar8 + 0x1e4);
    if (piVar1 == *(int **)(iVar8 + 0x1e4)) {
      uVar5 = 0;
    }
    else {
      uVar5 = piVar1[2];
    }
    *(undefined4 *)(iVar8 + 0x210) = uVar5;
    FUN_00f1b390();
    if ((((DAT_3f7ae155 == '\0') && (_DAT_3f7ae158 < 4)) &&
        (*(undefined1 **)(iVar8 + 4) != &DAT_3f7ae148)) && (_DAT_3f7ae15c != (int *)0x0)) {
      (**(code **)(*_DAT_3f7ae15c + 4))((undefined4 *)(iVar8 + 0x20c));
    }
  }
  FUN_011a8c90(3,param_1,&LAB_011a6b90,0);
  ExceptionList = local_1c;
  return;
}


