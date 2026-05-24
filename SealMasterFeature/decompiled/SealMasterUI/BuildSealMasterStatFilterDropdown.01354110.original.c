
/* ============================================================ */
/* Function: FUN_01354110 */
/* Entry: 01354110 */
/* Signature: undefined FUN_01354110() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01354110(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  void *pvVar5;
  undefined4 *_Dst;
  undefined4 uVar6;
  int iVar7;
  undefined4 uVar8;
  undefined4 ****ppppuVar9;
  int extraout_ECX;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  ulonglong uVar13;
  undefined8 uVar14;
  int local_8c;
  undefined1 local_80 [16];
  undefined8 local_70;
  int local_64;
  uint local_60 [9];
  undefined4 ***local_3c;
  undefined4 **ppuStack_38;
  undefined4 **ppuStack_34;
  undefined4 **ppuStack_30;
  undefined8 local_2c;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198450b;
  local_1c = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_64 = param_1;
  local_24 = uVar4;
  pvVar5 = (void *)FUN_016830c0(0x218,uVar4);
  local_14 = 0;
  if (pvVar5 == (void *)0x0) {
    local_8c = 0;
  }
  else {
    memset(pvVar5,0,0x218);
    local_8c = FUN_012276b0();
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0xb4) = local_8c;
  if (local_8c != 0) {
    uVar6 = *(undefined4 *)(param_1 + 0x40);
    local_70 = (ulonglong)(uint)local_70;
    FUN_012228b0(uVar6,0x21e,0x37,0xcc,0x16,s_Control_G_ComboBox_Combo_tga_01ae65fc,0);
    FUN_01223e30();
    *(undefined4 *)(local_8c + 0x1c8) = 0;
    *(undefined4 *)(local_8c + 0x1cc) = 0x16;
    _Dst = (undefined4 *)FUN_016830c0(0x8c,uVar4);
    local_14 = 1;
    if (_Dst == (undefined4 *)0x0) {
      _Dst = (undefined4 *)0x0;
    }
    else {
      memset(_Dst,0,0x8c);
      FUN_0145eca0();
      *_Dst = &DAT_01ad5210;
      _Dst[0x1d] = 0;
      _Dst[0x1e] = 0;
      _Dst[0x1f] = 0;
      _Dst[0x20] = 0;
      _Dst[0x21] = 0;
      _Dst[0x22] = 0;
      _Dst[0x14] = 0;
      _Dst[0x15] = 0;
      _Dst[0x16] = 0;
      _Dst[0x17] = 0;
      _Dst[0x18] = 0;
      _Dst[0x19] = 0;
      _Dst[0x1a] = 0;
      _Dst[0x1b] = 0;
      _Dst[0x1c] = 0;
    }
    local_14 = 0xffffffff;
    *(undefined4 **)(local_8c + 0x1c0) = _Dst;
    FUN_01259fa0(uVar6,s_Control_G_Popup_ON2Dlg_01ae661c,&DAT_01ac8f80,0,
                 *(int *)(local_8c + 0x1c8) + 0x21e,*(int *)(local_8c + 0x1cc) + 0x37,0xcc,10,0x10,
                 0x10);
    pvVar5 = (void *)FUN_016830c0(0xa0);
    local_14 = 2;
    if (pvVar5 == (void *)0x0) {
      uVar6 = 0;
    }
    else {
      memset(pvVar5,0,0xa0);
      uVar6 = FUN_0123de10();
    }
    local_14 = 0xffffffff;
    *(undefined4 *)(local_8c + 0x1c4) = uVar6;
    iVar7 = FUN_01446f10(9);
    uVar12 = DAT_01bee890;
    uVar10 = DAT_01bee88c;
    uVar6 = DAT_01bb9b34;
    local_70 = DAT_01bb9b2c;
    iVar1 = *(int *)(extraout_ECX + iVar7 * 4 + 0x40);
    FUN_00f19f10();
    local_60[4] = 0;
    local_60[1] = 0;
    local_60[2] = 0;
    local_60[3] = 0;
    local_60[5] = 0xf;
    local_60[0] = 0;
    local_14 = 3;
    FUN_015010a0(local_60,&DAT_01ae7290,&DAT_01bfbd40,s_Talk_Common_Seletbar_tga_01ad36a8);
    FUN_0124ef30(local_60);
    uVar11 = 0;
    uVar13 = local_70;
    uVar8 = FUN_0124f150(0,local_70,uVar6);
    FUN_0123e330(0,uVar10,uVar12,0xcc,iVar1 + 2,uVar8,uVar11,uVar13,uVar6);
    local_14 = 0xffffffff;
    FUN_00f1a420();
    iVar3 = local_64;
    iVar1 = DAT_01bfe468;
    *(undefined2 *)(local_8c + 0x1d8) = 0;
    *(undefined4 *)(local_8c + 0x1fc) = 9;
    *(undefined4 *)(local_8c + 0x200) = 1;
    *(undefined4 *)(local_8c + 0x20c) = 0xffffffff;
    *(undefined4 *)(local_8c + 0x210) = 0;
    *(undefined2 *)(local_8c + 0x214) = 0;
    *(int *)(local_8c + 0x208) = (0x16 - *(int *)(iVar7 * 4 + 0x40 + iVar1)) / 2;
    FUN_01227910(s_SealMaster_btn_combobox_tga_01ad7abc,0,0x16);
    FUN_01227a10(s_SealMaster_dropdown_list_0_01ad7b14);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_ALL_01ae13ac,0x1a);
    FUN_015ce480(&local_3c,uVar6,uVar10,uVar12,uVar14,uVar8);
    local_14 = 4;
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = local_70 & 0xffffffff;
    local_60[0] = DAT_01b02744;
    local_60[1] = 0;
    local_60[2] = 0;
    local_60[3] = 0;
    FUN_012281c0(ppppuVar9,0,(ulonglong)DAT_01b02744 << 0x20,0);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_HP_01ae1400,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = CONCAT44(&DAT_3f7ae148,(uint)local_70);
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_DS_01ae13e4,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = CONCAT44(&DAT_3f7ae148,(uint)local_70);
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_AT_01ae1438,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = CONCAT44(&DAT_3f7ae148,(uint)local_70);
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_AS_01ae141c,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = CONCAT44(&DAT_3f7ae148,(uint)local_70);
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_CT_01ae1470,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = CONCAT44(&DAT_3f7ae148,(uint)local_70);
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_HT_01ae1454,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = CONCAT44(&DAT_3f7ae148,(uint)local_70);
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_DE_01ae14a8,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = CONCAT44(&DAT_3f7ae148,(uint)local_70);
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_BL_01ae148c,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70 = CONCAT44(&DAT_3f7ae148,(uint)local_70);
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    uVar6 = 0;
    uVar10 = 0;
    uVar12 = 0;
    uVar14 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_SEALMASTER_STATE_COMBO_EV_01ae14f4,0x19);
    ppppuVar9 = (undefined4 ****)FUN_015ce480(local_80,uVar6,uVar10,uVar12,uVar14,uVar8);
    if (&local_3c != ppppuVar9) {
      FUN_00f1a330();
      local_3c = *ppppuVar9;
      ppuStack_38 = ppppuVar9[1];
      ppuStack_34 = ppppuVar9[2];
      ppuStack_30 = ppppuVar9[3];
      local_2c = *(undefined8 *)(ppppuVar9 + 4);
      ppppuVar9[4] = (undefined4 ***)0x0;
      ppppuVar9[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar9 = 0;
    }
    FUN_00f1a330();
    ppppuVar9 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar9 = (undefined4 ****)local_3c;
    }
    local_70._4_4_ = &DAT_3f7ae148;
    FUN_012281c0(ppppuVar9,0,CONCAT44(local_60[0],local_60[0]),&DAT_3f7ae148);
    iVar1 = *(int *)(iVar3 + 0xb4);
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
      local_64 = 3;
      FUN_00f1b390();
      if ((((local_70._4_4_[0xd] == '\0') && (*(uint *)(local_70._4_4_ + 0x10) < 4)) &&
          (local_70._4_4_ != *(undefined1 **)(iVar1 + 4))) &&
         (*(int **)(local_70._4_4_ + 0x14) != (int *)0x0)) {
        (**(code **)(**(int **)(local_70._4_4_ + 0x14) + 4))((undefined4 *)(iVar1 + 0x20c));
      }
    }
    FUN_01356ef0(3,iVar3,&LAB_01353be0,0);
    FUN_012472c0(*(undefined4 *)(iVar3 + 0xb4));
    FUN_00f1a330();
  }
  ExceptionList = local_1c;
  return;
}


