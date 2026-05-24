
/* ============================================================ */
/* Function: FUN_011d30b0 */
/* Entry: 011d30b0 */
/* Signature: undefined FUN_011d30b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011d30b0(int param_1)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  short ****ppppsVar4;
  int iVar5;
  int iVar6;
  short *psVar7;
  short ****ppppsVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined8 uVar13;
  undefined1 local_e0 [28];
  int local_c4;
  short ***local_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined4 local_b0;
  uint local_ac;
  short ***local_9c;
  short **ppsStack_98;
  short **ppsStack_94;
  short **ppsStack_90;
  undefined8 local_8c;
  short ***local_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 local_74;
  undefined1 *puStack_70;
  uint local_6c;
  uint local_68;
  uint local_64;
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
  int local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01993067;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_6c = 0;
  local_c4 = param_1;
  local_24 = uVar2;
  FUN_01246720(s_DetailInfo_RemainTimerBG_tga_01ae2bf8,1,10,0xaa,0xf1,0x5c);
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
  local_30 = 10;
  local_3c = 1;
  local_2c = 0;
  local_38 = 1;
  puStack_70 = &DAT_3f7ae148;
  local_58 = &DAT_3f7ae148;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  local_c0 = DAT_01b02744;
  uStack_bc = 0;
  uStack_b8 = 0;
  uStack_b4 = 0;
  uVar13 = 0;
  FUN_00f19cc0(s_EVOLUTION_ADVICE_TITLE_INFO_01ad9800,0x1b);
  psVar3 = (short *)FUN_015ce480(&local_9c,uVar9,uVar10,uVar11,uVar12,uVar13);
  local_14._0_1_ = 1;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar7 = psVar3;
  do {
    sVar1 = *psVar7;
    psVar7 = psVar7 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar7 - (int)(psVar3 + 1) >> 1);
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330(uVar2);
  FUN_01246bb0(&local_60,1,0x84,0xb1);
  local_34 = 0;
  FUN_00f1a330();
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = 0;
  local_28 = 0;
  local_14 = 2;
  local_34 = DAT_01bfe468 + 8;
  puStack_70 = &DAT_3f7ae148;
  local_58 = &DAT_3f7ae148;
  local_60 = CONCAT44(local_c0,local_c0);
  local_30 = 10;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  if (*(int *)(*(int *)(param_1 + 0x78) + 0x110) == 0) {
    local_74 = 0;
    puStack_70 = (undefined1 *)0x0;
    local_84 = (short ***)0x0;
    uStack_80 = 0;
    uStack_7c = 0;
    uStack_78 = 0;
    FUN_00f1a200(u_Unknown_01ace9b4,7);
    ppppsVar4 = &local_84;
    uVar2 = 2;
  }
  else {
    local_64 = (uint)*(byte *)(*(int *)(param_1 + 0x78) + 0x98);
    ppppsVar4 = (short ****)FUN_015e5a20(local_e0,&local_64);
    uVar2 = 1;
  }
  local_9c = *ppppsVar4;
  ppsStack_98 = (short **)ppppsVar4[1];
  ppsStack_94 = (short **)ppppsVar4[2];
  ppsStack_90 = (short **)ppppsVar4[3];
  local_8c = *(undefined8 *)(ppppsVar4 + 4);
  ppppsVar4[4] = (short ***)0x0;
  ppppsVar4[5] = (short ***)0x7;
  *(undefined2 *)ppppsVar4 = 0;
  local_68 = uVar2 | 4;
  local_6c = uVar2 | 4;
  if ((uVar2 & 2) != 0) {
    local_6c = uVar2 & 0xfffffffd | 4;
    local_68 = local_6c;
    FUN_00f1a330();
  }
  local_14 = 2;
  if ((local_68 & 1) != 0) {
    local_6c = local_68 & 0xfffffffe;
    local_68 = local_6c;
    FUN_00f1a330();
  }
  local_14._0_1_ = 4;
  ppppsVar4 = &local_9c;
  if (7 < local_8c._4_4_) {
    ppppsVar4 = (short ****)local_9c;
  }
  ppppsVar8 = ppppsVar4;
  do {
    sVar1 = *(short *)ppppsVar8;
    ppppsVar8 = (short ****)((int)ppppsVar8 + 2);
  } while (sVar1 != 0);
  FUN_00f1a040(ppppsVar4,(int)ppppsVar8 - (int)((int)ppppsVar4 + 2) >> 1);
  local_14 = CONCAT31(local_14._1_3_,2);
  FUN_00f1a330();
  FUN_01246bb0(&local_60,1,0x23,0xcb);
  iVar5 = FUN_01446f10();
  local_64 = *(int *)(local_34 + 0x38 + iVar5 * 4) + 0xcb;
  local_34 = 0;
  FUN_00f1a330();
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = 0;
  local_28 = 0;
  iVar5 = *(int *)(param_1 + 0x78);
  local_34 = DAT_01bfe468 + 8;
  puStack_70 = &DAT_3f7ae148;
  local_60 = CONCAT44(local_c0,local_c0);
  local_30 = 10;
  local_58 = &DAT_3f7ae148;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  local_14._0_1_ = 6;
  local_14._1_3_ = 0;
  uVar9 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar13 = 0;
  FUN_00f19cc0(s_DIGIMONSTATUS_BASEINFO_HATCHLV_01ace938,0x1e);
  FUN_015ce480(&local_84,uVar9,uVar10,uVar11,uVar12,uVar13);
  uVar2 = local_68;
  local_6c = local_68 | 8;
  local_9c = (short ***)0x0;
  ppsStack_98 = (short **)0x0;
  ppsStack_94 = (short **)0x0;
  ppsStack_90 = (short **)0x0;
  local_8c = 0;
  local_68 = (uint)*(byte *)(iVar5 + 0x99);
  FUN_00f1a200(u__HatchLv__01ace958,9);
  local_14._0_1_ = 7;
  FUN_01501660(&local_84,&local_9c,&local_68);
  local_14._0_1_ = 6;
  FUN_00f1a330();
  ppppsVar4 = &local_84;
  if ((undefined1 *)0x7 < puStack_70) {
    ppppsVar4 = (short ****)local_84;
  }
  ppppsVar8 = ppppsVar4;
  do {
    sVar1 = *(short *)ppppsVar8;
    ppppsVar8 = (short ****)((int)ppppsVar8 + 2);
  } while (sVar1 != 0);
  FUN_00f1a040(ppppsVar4,(int)ppppsVar8 - (int)((int)ppppsVar4 + 2) >> 1);
  local_14 = CONCAT31(local_14._1_3_,5);
  FUN_00f1a330();
  iVar5 = local_c4;
  FUN_01246bb0(&local_60,1,0x23,local_64);
  iVar6 = FUN_01446f10();
  local_64 = local_64 + *(int *)(local_34 + 0x38 + iVar6 * 4);
  local_34 = 0;
  FUN_00f1a330();
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = 0;
  local_28 = 0;
  local_34 = DAT_01bfe468 + 8;
  puStack_70 = &DAT_3f7ae148;
  local_60 = CONCAT44(local_c0,local_c0);
  local_58 = &DAT_3f7ae148;
  local_30 = 10;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  local_14._0_1_ = 9;
  local_14._1_3_ = 0;
  uStack_bc = 0;
  uStack_b8 = 0;
  uStack_b4 = 0;
  local_b0 = 0;
  local_ac = 7;
  local_c0 = (short ***)0x0;
  local_6c = uVar2 & 0xfffffff7 | 0x10;
  FUN_01500f80(&local_c0,u__d_cm____2f____01ace918,*(undefined4 *)(*(int *)(iVar5 + 0x78) + 0x9c),
               (double)*(float *)(*(int *)(iVar5 + 0x78) + 0xa0));
  ppppsVar4 = &local_c0;
  if (7 < local_ac) {
    ppppsVar4 = (short ****)local_c0;
  }
  ppppsVar8 = ppppsVar4;
  do {
    sVar1 = *(short *)ppppsVar8;
    ppppsVar8 = (short ****)((int)ppppsVar8 + 2);
  } while (sVar1 != 0);
  FUN_00f1a040(ppppsVar4,(int)ppppsVar8 - (int)((int)ppppsVar4 + 2) >> 1);
  local_14 = CONCAT31(local_14._1_3_,8);
  FUN_00f1a330();
  FUN_01246bb0(&local_60,1,0x23,local_64);
  local_34 = 0;
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


