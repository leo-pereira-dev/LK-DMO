
/* ============================================================ */
/* Function: FUN_011a58b0 */
/* Entry: 011a58b0 */
/* Signature: undefined FUN_011a58b0() */
/* ============================================================ */


void __fastcall FUN_011a58b0(int param_1)

{
  int iVar1;
  int iVar2;
  int iStack_48;
  int iStack_44;
  void *local_3c;
  undefined4 local_38;
  void *local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198fe79;
  local_1c = ExceptionList;
  ExceptionList = &local_1c;
  if (*(int *)(param_1 + 0x7c) != 0) {
    puStack_20 = &stack0xfffffffc;
    FUN_0122a180();
    FUN_011a8c40();
    ExceptionList = local_1c;
    return;
  }
  local_3c = (void *)FUN_016830c0(0xac,DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
  local_14 = 0;
  if (local_3c == (void *)0x0) {
    iVar1 = 0;
  }
  else {
    memset(local_3c,0,0xac);
    iVar1 = FUN_01228950();
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0x7c) = iVar1;
  if (iVar1 != 0) {
    FUN_012472c0(iVar1);
    local_24 = *(int *)(param_1 + 0x3c);
    local_34 = (void *)(local_24 + -0x69);
    iStack_44 = local_24 + -0x6e;
    local_2c = *(int *)(param_1 + 0x38) + -0x1e;
    iStack_48 = *(int *)(param_1 + 0x38) + -0x28;
    local_3c = (void *)(iStack_44 / 0x45);
    local_28 = iStack_48 / 0x2f;
    local_30 = iStack_48 % 0x2f;
    iVar1 = 0;
    if (1 < local_28) {
      iVar1 = local_30 / local_28 + -1;
    }
    iVar2 = 0;
    if (1 < (int)local_3c) {
      local_30 = 0x45;
      iVar2 = ((local_24 + -0x6e) % 0x45) / (int)local_3c + -1;
    }
    FUN_01228c50(*(undefined4 *)(param_1 + 0x40),0x14,0x28,local_2c,local_34,iVar1,iVar2,0x2f,0x45,0
                 ,2,0,local_3c,local_28);
    local_38 = 0xfffffffb;
    local_34 = (void *)0xfffffffb;
    FUN_01228cf0(s_Control_G_Popup_N2Dlg5_tga_01ae0f00,&local_38,&iStack_48);
    FUN_01228dd0(s_SealMaster_Sealmaster_highlight__01ae0fd0,&DAT_01bee88c,
                 *(int *)(param_1 + 0x7c) + 0x78);
    *(undefined1 *)(*(int *)(param_1 + 0x7c) + 0x94) = 0;
    iVar1 = *(int *)(param_1 + 0x7c);
    *(undefined1 *)(iVar1 + 0xa9) = 0;
    if (*(int *)(iVar1 + 0x6c) != 0) {
      *(undefined1 *)(*(int *)(iVar1 + 0x6c) + 0x10) = 0;
      *(undefined4 *)(iVar1 + 0x6c) = 0;
    }
    FUN_011a8c90(7,param_1,FUN_011a5ec0,0);
    FUN_011a8c90(6,param_1,&LAB_011a7080,0);
    local_34 = (void *)FUN_016830c0(0xec);
    local_14 = 1;
    if (local_34 == (void *)0x0) {
      iVar1 = 0;
    }
    else {
      memset(local_34,0,0xec);
      iVar1 = FUN_01247d90();
    }
    local_14 = 0xffffffff;
    if (iVar1 != 0) {
      iStack_48 = local_2c + -10;
      iStack_44 = local_24 + -0x6e;
      local_28 = iVar1;
      FUN_01248090(0,0,local_3c,0,DAT_01bee88c,DAT_01bee890,0x10,local_24 + -0x4d,0x10,0xf,
                   DAT_01bee88c,DAT_01bee890,iStack_48,iStack_44);
      FUN_0122a200(iVar1);
    }
  }
  ExceptionList = local_1c;
  return;
}


