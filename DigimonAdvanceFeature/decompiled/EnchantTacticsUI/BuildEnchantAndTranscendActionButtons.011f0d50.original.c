
/* ============================================================ */
/* Function: FUN_011f0d50 */
/* Entry: 011f0d50 */
/* Signature: undefined FUN_011f0d50() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011f0d50(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  short *psVar5;
  undefined4 ***pppuVar6;
  undefined8 local_d4;
  undefined1 *local_cc;
  undefined4 uStack_c8;
  undefined8 *puStack_c4;
  undefined4 uStack_c0;
  short *local_bc;
  int local_b8;
  uint uStack_b4;
  undefined4 **local_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 local_78;
  uint local_74;
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
  undefined4 local_14;
  
  uVar4 = DAT_01b02744;
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_01994ff5;
  local_1c = ExceptionList;
  uStack_b4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
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
  local_30 = 9;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  local_64 = &DAT_3f7ae148;
  local_58 = &DAT_3f7ae148;
  local_cc = (undefined1 *)0x0;
  uStack_c8 = 0;
  puStack_c4 = (undefined8 *)0x0;
  uStack_c0 = 0;
  local_d4 = 0x1201ad9b10;
  local_bc = (short *)0x0;
  local_b8 = 0;
  local_24 = uStack_b4;
  FUN_00f19cc0();
  local_d4 = CONCAT44(&local_88,0x11f0e35);
  local_bc = (short *)FUN_015ce480();
  local_14._0_1_ = 1;
  if (7 < *(uint *)(local_bc + 10)) {
    local_bc = *(short **)local_bc;
  }
  psVar5 = local_bc;
  do {
    sVar1 = *psVar5;
    psVar5 = psVar5 + 1;
  } while (sVar1 != 0);
  local_b8 = (int)psVar5 - (int)(local_bc + 1) >> 1;
  uStack_c0 = 0x11f0e69;
  FUN_00f1a040();
  local_14._0_1_ = 0;
  local_b8 = 0x11f0e75;
  FUN_00f1a330();
  local_b8 = 0x10;
  local_bc = (short *)(*(int *)(param_1 + 0x38) >> 1);
  uStack_c0 = 1;
  puStack_c4 = &local_60;
  local_38 = 5;
  uStack_c8 = 0x11f0e91;
  uVar2 = FUN_01246bb0();
  *(undefined4 *)(param_1 + 0x8c) = uVar2;
  local_34 = DAT_01bfe468 + 8;
  local_60 = CONCAT44(uVar4,uVar4);
  local_30 = 9;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  local_64 = &DAT_3f7ae148;
  local_58 = &DAT_3f7ae148;
  local_cc = (undefined1 *)0x0;
  uStack_c8 = 0;
  puStack_c4 = (undefined8 *)0x0;
  uStack_c0 = 0;
  local_d4 = 0x1a01ae4078;
  local_bc = (short *)0x0;
  local_b8 = 0;
  FUN_00f19cc0();
  local_d4 = CONCAT44(&local_88,0x11f0f10);
  local_bc = (short *)FUN_015ce480();
  local_14._0_1_ = 2;
  if (7 < *(uint *)(local_bc + 10)) {
    local_bc = *(short **)local_bc;
  }
  psVar5 = local_bc;
  do {
    sVar1 = *psVar5;
    psVar5 = psVar5 + 1;
  } while (sVar1 != 0);
  local_b8 = (int)psVar5 - (int)(local_bc + 1) >> 1;
  uStack_c0 = 0x11f0f3c;
  FUN_00f1a040();
  local_14._0_1_ = 0;
  local_b8 = 0x11f0f48;
  FUN_00f1a330();
  local_b8 = 0x2e;
  local_bc = (short *)0x14;
  uStack_c0 = 1;
  puStack_c4 = &local_60;
  local_38 = 4;
  uStack_c8 = 0x11f0f60;
  FUN_01246bb0();
  local_34 = DAT_01bfe468 + 8;
  local_60 = CONCAT44(uVar4,uVar4);
  local_30 = 9;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  local_64 = &DAT_3f7ae148;
  local_58 = &DAT_3f7ae148;
  local_cc = (undefined1 *)0x0;
  uStack_c8 = 0;
  puStack_c4 = (undefined8 *)0x0;
  uStack_c0 = 0;
  local_d4 = 0x1f01ad8ee8;
  local_bc = (short *)0x0;
  local_b8 = 0;
  FUN_00f19cc0();
  local_d4 = CONCAT44(&local_88,0x11f0fd9);
  local_bc = (short *)FUN_015ce480();
  local_14._0_1_ = 3;
  if (7 < *(uint *)(local_bc + 10)) {
    local_bc = *(short **)local_bc;
  }
  psVar5 = local_bc;
  do {
    sVar1 = *psVar5;
    psVar5 = psVar5 + 1;
  } while (sVar1 != 0);
  local_b8 = (int)psVar5 - (int)(local_bc + 1) >> 1;
  uStack_c0 = 0x11f1009;
  FUN_00f1a040();
  local_14._0_1_ = 0;
  local_b8 = 0x11f1015;
  FUN_00f1a330();
  local_b8 = 0x51;
  local_bc = (short *)0x14;
  uStack_c0 = 1;
  puStack_c4 = &local_60;
  local_38 = 4;
  uStack_c8 = 0x11f102d;
  FUN_01246bb0();
  local_34 = DAT_01bfe468 + 8;
  local_b8 = 0;
  local_64 = &DAT_3f7ae148;
  local_bc = &DAT_01ac5dd8;
  local_30 = 9;
  local_60 = CONCAT44(uVar4,uVar4);
  local_58 = &DAT_3f7ae148;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  uStack_c0 = 0x11f107f;
  FUN_00f1a040();
  local_b8 = 0x2e;
  local_bc = (short *)0x82;
  uStack_c0 = 1;
  puStack_c4 = &local_60;
  local_38 = 5;
  uStack_c8 = 0x11f109a;
  uVar2 = FUN_01246bb0();
  *(undefined4 *)(param_1 + 0x90) = uVar2;
  local_34 = DAT_01bfe468 + 8;
  local_b8 = 0;
  local_64 = &DAT_3f7ae148;
  local_bc = &DAT_01ac5dd8;
  local_30 = 9;
  local_60 = CONCAT44(uVar4,uVar4);
  local_58 = &DAT_3f7ae148;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  uStack_c0 = 0x11f10f2;
  FUN_00f1a040();
  local_b8 = 0x51;
  local_bc = (short *)0x82;
  uStack_c0 = 1;
  puStack_c4 = &local_60;
  local_38 = 5;
  uStack_c8 = 0x11f110d;
  uVar3 = FUN_01246bb0();
  uVar2 = *(undefined4 *)(param_1 + 0x38);
  local_b8 = 0x3b;
  *(undefined4 *)(param_1 + 0x94) = uVar3;
  local_bc = (short *)s_Data_Interface_Breakthrough_Brea_01ae4094;
  local_64 = &DAT_3f7ae148;
  local_88 = (undefined4 ***)0x0;
  uStack_84 = 0;
  uStack_80 = 0;
  uStack_7c = 0;
  local_78 = 0;
  local_74 = 0;
  uStack_c0 = 0x11f1141;
  FUN_00f19cc0();
  local_b8 = 0x28;
  uStack_c0 = 0x1a;
  local_14._0_1_ = 4;
  pppuVar6 = &local_88;
  if (0xf < local_74) {
    pppuVar6 = (undefined4 ***)local_88;
  }
  puStack_c4 = (undefined8 *)0x0;
  uStack_c8 = 0;
  local_d4 = CONCAT44(uVar4,uVar4);
  local_cc = local_64;
  local_bc = (short *)uVar2;
  uVar2 = FUN_01246800(pppuVar6,&local_d4);
  *(undefined4 *)(param_1 + 0x98) = uVar2;
  local_14._0_1_ = 0;
  local_b8 = 0x11f1190;
  FUN_00f1a420();
  uVar2 = *(undefined4 *)(param_1 + 0x38);
  local_b8 = 0x3b;
  local_64 = &DAT_3f7ae148;
  local_bc = (short *)s_Data_Interface_Breakthrough_Brea_01ae4094;
  local_88 = (undefined4 ***)0x0;
  uStack_84 = 0;
  uStack_80 = 0;
  uStack_7c = 0;
  local_78 = 0;
  local_74 = 0;
  uStack_c0 = 0x11f11be;
  FUN_00f19cc0();
  local_b8 = 0x28;
  uStack_c0 = 0x3d;
  local_14 = CONCAT31(local_14._1_3_,5);
  pppuVar6 = &local_88;
  if (0xf < local_74) {
    pppuVar6 = (undefined4 ***)local_88;
  }
  puStack_c4 = (undefined8 *)0x0;
  uStack_c8 = 0;
  local_d4 = CONCAT44(uVar4,uVar4);
  local_cc = local_64;
  local_bc = (short *)uVar2;
  uVar4 = FUN_01246800(pppuVar6,&local_d4);
  *(undefined4 *)(param_1 + 0x9c) = uVar4;
  local_b8 = 0x11f1206;
  FUN_00f1a420();
  local_34 = 0;
  local_b8 = 0x11f1215;
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


