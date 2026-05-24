
/* ============================================================ */
/* Function: FUN_013cb8a0 */
/* Entry: 013cb8a0 */
/* Signature: undefined FUN_013cb8a0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_013cb8a0(undefined8 *param_1,undefined4 *param_2,int param_3,int param_4)

{
  short sVar1;
  undefined1 *puVar2;
  uint uVar3;
  undefined4 ****ppppuVar4;
  undefined8 *puVar5;
  short ****ppppsVar6;
  short ****ppppsVar7;
  char *pcVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined1 local_b4 [24];
  int local_9c;
  void *local_98;
  undefined8 *local_94;
  undefined8 local_90;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined1 local_6c;
  undefined4 local_68;
  int local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  short ***local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  uint local_40;
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
  puStack_18 = &LAB_019b1daf;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_9c = param_3;
  ppuStack_38 = (undefined4 ***)0x0;
  ppuStack_34 = (undefined4 ***)0x0;
  ppuStack_30 = (undefined4 ***)0x0;
  local_94 = param_1;
  local_2c = 0x700000000;
  local_3c = (undefined4 ****)0x0;
  local_14 = 0;
  local_24 = uVar3;
  puVar2 = &stack0xfffffffc;
  switch(param_4) {
  case 0:
    uVar9 = 0x11;
    pcVar8 = s_COMMON_TXT_NORMAL_01ace66c;
    break;
  case 1:
    uVar9 = 0x19;
    pcVar8 = s_TOOLTIP_SEALMASTER_BRONZE_01af7570;
    break;
  case 2:
    uVar9 = 0x19;
    pcVar8 = s_TOOLTIP_SEALMASTER_SILVER_01af758c;
    break;
  case 3:
    uVar9 = 0x17;
    pcVar8 = s_TOOLTIP_SEALMASTER_GOLD_01af7608;
    break;
  case 4:
    uVar9 = 0x1b;
    pcVar8 = s_TOOLTIP_SEALMASTER_PLATINUM_01af7620;
    break;
  case 5:
    uVar9 = 0x19;
    pcVar8 = s_TOOLTIP_SEALMASTER_MASTER_01af75dc;
    break;
  default:
    goto switchD_013cb920_default;
  }
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar15 = 0;
  FUN_00f19cc0(pcVar8,uVar9);
  ppppuVar4 = (undefined4 ****)FUN_015ce480(local_b4,uVar10,uVar11,uVar12,uVar13,uVar14,uVar15);
  if (&local_3c != ppppuVar4) {
    FUN_00f1a330();
    local_3c = *ppppuVar4;
    ppuStack_38 = ppppuVar4[1];
    ppuStack_34 = ppppuVar4[2];
    ppuStack_30 = ppppuVar4[3];
    local_2c = *(undefined8 *)(ppppuVar4 + 4);
    ppppuVar4[4] = (undefined4 ***)0x0;
    *(undefined2 *)ppppuVar4 = 0;
    ppppuVar4[5] = (undefined4 ***)0x7;
  }
  FUN_00f1a330();
  puVar2 = puStack_20;
switchD_013cb920_default:
  puStack_20 = puVar2;
  puVar5 = local_94;
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = (short ***)0x0;
  local_14 = CONCAT31(local_14._1_3_,1);
  if (param_4 == -1) {
    if (7 < (uint)param_2[5]) {
      param_2 = (undefined4 *)*param_2;
    }
    FUN_01500f80(&local_54,&DAT_01aec624,param_2);
  }
  else {
    ppppuVar4 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppuVar4 = (undefined4 ****)local_3c;
    }
    if (7 < (uint)param_2[5]) {
      param_2 = (undefined4 *)*param_2;
    }
    FUN_01500f80(&local_54,u__s__s__01af75f8,param_2,ppppuVar4,uVar3);
  }
  local_74 = 0;
  local_70 = 7;
  uStack_80 = 0;
  uStack_7c = 0;
  uStack_78 = 0;
  local_84 = 0;
  local_58 = 0;
  local_14._0_1_ = 2;
  local_90 = *puVar5;
  local_64 = DAT_01bfe468 + 8;
  local_88 = *(undefined4 *)(puVar5 + 1);
  local_60 = 0xb;
  local_6c = 1;
  local_5c = 0;
  local_68 = 0;
  local_98 = (void *)FUN_016830c0(0x2c);
  local_14._0_1_ = 3;
  if (local_98 == (void *)0x0) {
    puVar5 = (undefined8 *)0x0;
  }
  else {
    memset(local_98,0,0x2c);
    puVar5 = (undefined8 *)FUN_010baa10();
  }
  local_14 = CONCAT31(local_14._1_3_,2);
  ppppsVar7 = &local_54;
  if (7 < local_40) {
    ppppsVar7 = (short ****)local_54;
  }
  ppppsVar6 = ppppsVar7;
  do {
    sVar1 = *(short *)ppppsVar6;
    ppppsVar6 = (short ****)((int)ppppsVar6 + 2);
  } while (sVar1 != 0);
  FUN_00f1a040(ppppsVar7,(int)ppppsVar6 - (int)((int)ppppsVar7 + 2) >> 1);
  FUN_01230c40(&local_90,DAT_01bee88c,DAT_01bee890);
  local_94 = puVar5;
  FUN_00f58160(*(undefined4 *)(local_9c + 8),&local_94);
  local_64 = 0;
  FUN_00f1a330();
  FUN_00f1a330();
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


