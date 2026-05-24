
/* ============================================================ */
/* Function: FUN_011b11b0 */
/* Entry: 011b11b0 */
/* Signature: undefined FUN_011b11b0() */
/* ============================================================ */


void __fastcall FUN_011b11b0(int param_1)

{
  void *pvVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int local_48;
  int local_44;
  void *local_40;
  int local_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  int iStack_30;
  void *pvStack_2c;
  int local_28;
  int local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198fb99;
  local_1c = ExceptionList;
  ExceptionList = &local_1c;
  pvStack_2c = (void *)param_1;
  if (*(int *)(param_1 + 0x7c) != 0) {
    puStack_20 = &stack0xfffffffc;
    FUN_0122a180();
    FUN_01072dd0();
    ExceptionList = local_1c;
    return;
  }
  local_40 = (void *)FUN_016830c0(0xac,DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
  local_14 = 0;
  if (local_40 == (void *)0x0) {
    iVar2 = 0;
  }
  else {
    memset(local_40,0,0xac);
    iVar2 = FUN_01228950();
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0x7c) = iVar2;
  if (iVar2 != 0) {
    FUN_012472c0(iVar2);
    pvVar1 = pvStack_2c;
    local_3c = *(int *)(param_1 + 0x3c);
    local_44 = local_3c + -0x6e;
    local_48 = *(int *)(param_1 + 0x38) + -0x28;
    local_24 = (int)(local_44 + (local_44 >> 0x1f & 0x1fU)) >> 5;
    iVar2 = (int)(local_48 + (local_48 >> 0x1f & 0x1fU)) >> 5;
    uVar4 = 0;
    if (1 < iVar2) {
      uVar4 = 2;
    }
    local_28 = 3;
    uVar3 = 0;
    if (1 < local_24) {
      uVar3 = 3;
    }
    local_40 = (void *)local_44;
    FUN_01228c50(*(undefined4 *)((int)pvStack_2c + 0x40),0x14,0x28,*(int *)(param_1 + 0x38) + -0x1e,
                 local_3c + -0x69,uVar4,uVar3,0x20,0x20,0,2,0,uVar3,iVar2);
    iStack_30 = -5;
    pvStack_2c = (void *)0xfffffffb;
    FUN_01228cf0(s_Control_G_Popup_N2Dlg5_tga_01ae0f00,&iStack_30,&local_48);
    FUN_01228dd0(s_SealMaster_Sealmaster_highlight__01ae0fd0,&DAT_01bee88c,
                 *(int *)((int)pvVar1 + 0x7c) + 0x78);
    *(undefined1 *)(*(int *)((int)pvVar1 + 0x7c) + 0x94) = 0;
    iVar2 = *(int *)((int)pvVar1 + 0x7c);
    *(undefined1 *)(iVar2 + 0xa9) = 0;
    if (*(int *)(iVar2 + 0x6c) != 0) {
      *(undefined1 *)(*(int *)(iVar2 + 0x6c) + 0x10) = 0;
      *(undefined4 *)(iVar2 + 0x6c) = 0;
    }
    FUN_011b1f80(7,pvVar1,FUN_011b1570,0);
    FUN_011b1f80(6,pvVar1,&LAB_011b1770,0);
    pvStack_2c = (void *)FUN_016830c0(0xec);
    local_14 = 1;
    if (pvStack_2c == (void *)0x0) {
      iVar2 = 0;
    }
    else {
      memset(pvStack_2c,0,0xec);
      iVar2 = FUN_01247d90();
    }
    local_14 = 0xffffffff;
    if (iVar2 != 0) {
      iStack_30 = local_48;
      pvStack_2c = local_40;
      local_38 = DAT_01bee88c;
      uStack_34 = DAT_01bee890;
      local_28 = iVar2;
      FUN_01248090(0,0,local_24,0,DAT_01bee88c,DAT_01bee890,0x10,local_3c + -0x4d,0x10,0xf,
                   DAT_01bee88c,DAT_01bee890,local_48,local_40);
      FUN_0122a200(iVar2);
    }
  }
  ExceptionList = local_1c;
  return;
}


