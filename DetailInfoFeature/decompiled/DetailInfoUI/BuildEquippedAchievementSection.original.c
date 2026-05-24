
/* ============================================================ */
/* Function: FUN_011dc7f0 */
/* Entry: 011dc7f0 */
/* Signature: undefined FUN_011dc7f0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011dc7f0(int param_1)

{
  short sVar1;
  void *pvVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  short *psVar6;
  short *******pppppppsVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined8 uVar10;
  undefined1 local_d8 [24];
  undefined1 local_c0 [24];
  short ******local_a8 [4];
  undefined8 local_98;
  uint local_8c;
  int local_88;
  void *local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_6c;
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
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01993b25;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_8c = 0;
  local_88 = param_1;
  local_84 = (void *)FUN_016830c0(0xa0,local_24);
  local_14 = 0;
  if (local_84 == (void *)0x0) {
    local_84 = (void *)0x0;
  }
  else {
    memset(local_84,0,0xa0);
    local_84 = (void *)FUN_0123de10();
  }
  uVar5 = DAT_01bb9b34;
  local_14 = 0xffffffff;
  if (local_84 == (void *)0x0) {
    ExceptionList = local_1c;
    return;
  }
  uVar8 = *(undefined4 *)(param_1 + 0x40);
  local_98 = DAT_01bb9b2c;
  FUN_00f19f10();
  local_70 = 0;
  uStack_7c = 0;
  uStack_78 = 0;
  uStack_74 = 0;
  local_6c = 0xf;
  local_80 = 0;
  local_14 = 1;
  FUN_015010a0(&local_80,&DAT_01ae7290,&DAT_01bfbd40,s_TacticsHouse_Storage_M_field_tga_01ad8e18);
  FUN_0124ef30(&local_80);
  pvVar2 = local_84;
  uVar9 = 0;
  uVar10 = local_98;
  uVar3 = FUN_0124f150(0,local_98,uVar5);
  FUN_0123e330(uVar8,0x102,10,0xc2,0x2f,uVar3,uVar9,uVar10,uVar5);
  local_14 = 0xffffffff;
  FUN_00f1a420();
  iVar4 = local_88;
  FUN_012472c0(pvVar2);
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = 0;
  local_28 = 0;
  local_14 = 2;
  local_34 = DAT_01bfe468 + 8;
  local_98 = CONCAT44(&DAT_3f7ae148,(undefined4)local_98);
  local_58 = &DAT_3f7ae148;
  local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
  local_30 = 0xb;
  local_3c = 1;
  local_2c = 0;
  local_38 = 1;
  local_80 = DAT_01b02744;
  uStack_7c = 0;
  uStack_78 = 0;
  uStack_74 = 0;
  if (*(int *)(*(int *)(iVar4 + 0x78) + 0x108) != 0) {
    local_88 = *(int *)(*(int *)(iVar4 + 0x78) + 0x5c);
    iVar4 = FUN_015d6b70(&local_88);
    if (iVar4 != 0) {
      uVar5 = FUN_00f221a0(iVar4 + 0x50);
      local_14 = CONCAT31(local_14._1_3_,3);
      local_8c = 1;
      goto LAB_011dca39;
    }
  }
  uVar5 = 0;
  uVar8 = 0;
  uVar3 = 0;
  uVar10 = 0;
  uVar9 = 0;
  FUN_00f19cc0(s_DETAIL_INFO_NONE_01ace904,0x10);
  uVar5 = FUN_015ce480(local_d8,uVar5,uVar8,uVar3,uVar10,uVar9);
  local_14 = 4;
  local_8c = 2;
LAB_011dca39:
  local_84 = (void *)local_8c;
  FUN_00f221a0(uVar5);
  if (((uint)local_84 & 2) != 0) {
    local_84 = (void *)((uint)local_84 & 0xfffffffd | 4);
    FUN_00f1a330();
  }
  local_14 = 2;
  if (((uint)local_84 & 1) != 0) {
    FUN_00f1a330();
  }
  local_14._0_1_ = 5;
  pppppppsVar7 = local_a8;
  if (7 < local_98._4_4_) {
    pppppppsVar7 = (short *******)local_a8[0];
  }
  do {
    sVar1 = *(short *)pppppppsVar7;
    pppppppsVar7 = (short *******)((int)pppppppsVar7 + 2);
  } while (sVar1 != 0);
  FUN_00f1a040();
  local_14._0_1_ = 2;
  FUN_00f1a330();
  FUN_01246bb0(&local_60,1,0x163,0x1c);
  local_34 = DAT_01bfe468 + 8;
  local_60 = CONCAT44(local_80,local_80);
  local_30 = 0xb;
  local_3c = 1;
  local_2c = 0;
  local_38 = 1;
  local_98 = CONCAT44(&DAT_3f7ae148,(undefined4)local_98);
  local_58 = &DAT_3f7ae148;
  uVar5 = 0;
  uVar8 = 0;
  uVar3 = 0;
  uVar10 = 0;
  uVar9 = 0;
  FUN_00f19cc0(s_DETAIL_INFO_ACHIEVE_01ae3474,0x13);
  psVar6 = (short *)FUN_015ce480(local_c0,uVar5,uVar8,uVar3,uVar10,uVar9);
  local_14._0_1_ = 6;
  if (7 < *(uint *)(psVar6 + 10)) {
    psVar6 = *(short **)psVar6;
  }
  do {
    sVar1 = *psVar6;
    psVar6 = psVar6 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040();
  local_14 = CONCAT31(local_14._1_3_,2);
  FUN_00f1a330();
  FUN_01246bb0(&local_60,1,0x163,0x3e);
  local_34 = 0;
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


