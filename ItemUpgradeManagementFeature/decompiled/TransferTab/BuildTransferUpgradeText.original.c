
/* ============================================================ */
/* Function: FUN_01270110 */
/* Entry: 01270110 */
/* Signature: undefined FUN_01270110() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01270110(int param_1)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  int iVar4;
  void *pvVar5;
  short *psVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 local_6c [24];
  void *local_54;
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
  
  puStack_c = &LAB_0199ba7b;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
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
  local_28 = 0;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  local_14 = uVar2;
  FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_01ae8b74,0x13);
  psVar3 = (short *)FUN_015ce480(local_6c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_8._0_1_ = 1;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330(uVar2);
  iVar4 = FUN_01246bb0(&local_50,1,100,0x154);
  *(int *)(param_1 + 0xdc) = iVar4;
  if (iVar4 != 0) {
    (**(code **)(*(int *)(iVar4 + 0x14) + 0x1c))(0);
  }
  local_28 = 0;
  uVar7 = 0;
  uVar8 = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_01ae8b74,0x13);
  psVar3 = (short *)FUN_015ce480(local_6c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
  local_8._0_1_ = 2;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar6 = psVar3;
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar6 - (int)(psVar3 + 1) >> 1);
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330();
  iVar4 = FUN_01246bb0(&local_50,1,400,0x154);
  *(int *)(param_1 + 0xe0) = iVar4;
  if (iVar4 != 0) {
    (**(code **)(*(int *)(iVar4 + 0x14) + 0x1c))(0);
  }
  local_54 = (void *)FUN_016830c0(0x2c);
  local_8._0_1_ = 3;
  if (local_54 == (void *)0x0) {
    pvVar5 = (void *)0x0;
  }
  else {
    memset(local_54,0,0x2c);
    pvVar5 = (void *)FUN_010baa10();
  }
  local_8._0_1_ = 0;
  local_28 = 0;
  FUN_00f1a040(&DAT_01ac5dd8,0);
  FUN_01230c40(&local_50,DAT_01bee88c,DAT_01bee890);
  local_54 = pvVar5;
  FUN_00f58160(*(undefined4 *)(param_1 + 0xec),&local_54);
  *(undefined1 *)(param_1 + 0xf4) = 0;
  local_54 = (void *)FUN_016830c0(0x2c);
  local_8._0_1_ = 4;
  if (local_54 == (void *)0x0) {
    pvVar5 = (void *)0x0;
  }
  else {
    memset(local_54,0,0x2c);
    pvVar5 = (void *)FUN_010baa10();
  }
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_01230c40(&local_50,DAT_01bee88c,DAT_01bee890);
  local_54 = pvVar5;
  FUN_00f58160(*(undefined4 *)(param_1 + 0x10c),&local_54);
  local_24 = 0;
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


