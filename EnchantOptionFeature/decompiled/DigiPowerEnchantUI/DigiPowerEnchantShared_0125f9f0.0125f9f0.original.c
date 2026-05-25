
/* ============================================================ */
/* Function: FUN_0125f9f0 */
/* Entry: 0125f9f0 */
/* Signature: undefined FUN_0125f9f0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0125f9f0(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  uint uVar4;
  void *pvVar5;
  undefined4 *_Dst;
  undefined4 uVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  int extraout_ECX;
  undefined4 uVar10;
  undefined8 uVar11;
  int local_48;
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
  puStack_18 = &LAB_0199b0b3;
  local_1c = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar4;
  pvVar5 = (void *)FUN_016830c0(0x218,uVar4);
  local_14 = 0;
  if (pvVar5 == (void *)0x0) {
    local_48 = 0;
  }
  else {
    memset(pvVar5,0,0x218);
    local_48 = FUN_012276b0();
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0xcc) = local_48;
  if (local_48 != 0) {
    uVar6 = *(undefined4 *)(param_1 + 0x40);
    FUN_012228b0(uVar6,0x47,0x96,0xb4,0x19,s_Control_G_ComboBox_Combo2_tga_01ae65dc,0);
    FUN_01223e30();
    *(undefined4 *)(local_48 + 0x1c8) = 0;
    *(undefined4 *)(local_48 + 0x1cc) = 0x19;
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
    *(undefined4 **)(local_48 + 0x1c0) = _Dst;
    FUN_01259fa0(uVar6,s_Control_G_Popup_ON2Dlg_01ae661c,&DAT_01ac8f80,0,
                 *(int *)(local_48 + 0x1c8) + 0x47,*(int *)(local_48 + 0x1cc) + 0x96,0xb4,10,0x10,
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
    *(undefined4 *)(local_48 + 0x1c4) = uVar6;
    iVar7 = FUN_01446f10(9);
    uVar3 = DAT_01bee890;
    uVar2 = DAT_01bee88c;
    uVar6 = DAT_01bb9b34;
    uVar11 = DAT_01bb9b2c;
    iVar9 = *(int *)(extraout_ECX + 0x40 + iVar7 * 4);
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
    uVar8 = FUN_0124f150(0,uVar11,uVar6);
    FUN_0123e330(0,uVar2,uVar3,0xb4,iVar9 + 2,uVar8,uVar10,uVar11,uVar6);
    local_14 = 0xffffffff;
    FUN_00f1a420();
    iVar9 = DAT_01bfe468;
    *(undefined2 *)(local_48 + 0x1d8) = 0;
    *(undefined4 *)(local_48 + 0x1fc) = 9;
    *(undefined4 *)(local_48 + 0x200) = 0;
    *(undefined4 *)(local_48 + 0x20c) = 0xffffffff;
    *(undefined4 *)(local_48 + 0x210) = 0;
    *(undefined2 *)(local_48 + 0x214) = 0;
    *(int *)(local_48 + 0x208) = (0x19 - *(int *)(iVar9 + 0x40 + iVar7 * 4)) / 2;
    FUN_012611d0(3,param_1,&LAB_01260fc0,0);
    FUN_01227a10(s_SealMaster_dropdown_list_0_01ad7b14);
    iVar9 = -10;
    if (*(int *)(*(int *)(param_1 + 0xcc) + 0x200) != 2) {
      iVar9 = 10;
    }
    piVar1 = (int *)(*(int *)(param_1 + 0xcc) + 0x204);
    *piVar1 = *piVar1 + iVar9;
    (**(code **)(**(int **)(param_1 + 0xcc) + 0x1c))(0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0xcc));
  }
  ExceptionList = local_1c;
  return;
}


