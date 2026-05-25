
/* ============================================================ */
/* Function: FUN_01372d20 */
/* Entry: 01372d20 */
/* Signature: undefined FUN_01372d20() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01372d20(int param_1,char param_2)

{
  short sVar1;
  undefined4 *puVar2;
  short *psVar3;
  undefined4 uVar4;
  int *piVar5;
  int iVar6;
  short ****ppppsVar7;
  short *psVar8;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  short ****ppppsVar9;
  float10 fVar10;
  undefined8 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  void *local_7c;
  short ***local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  uint local_64;
  undefined8 local_60;
  undefined4 local_58;
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
  puStack_18 = &LAB_019abc71;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  FUN_012457d0(0,1,0,0,1,(DAT_01bc07ec + -0x2c5) / 2,(DAT_01bc07f0 + -0x1b7) / 2,0x2c5,0x1b7);
  FUN_01246720(s_Accessory_Enchant_Enchant_BG1_tg_01af3450,1,0,0,0x2c5,0x1b7);
  uVar4 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar13 = 0;
  uVar14 = 0;
  FUN_00f19cc0(s_ACCESSORY_ENCHANT_ENCHANTMENT_01ae837c,0x1d);
  puVar2 = (undefined4 *)FUN_015ce480(&local_78,uVar4,uVar11,uVar12,uVar13,uVar14);
  local_14 = 0;
  if (7 < (uint)puVar2[5]) {
    puVar2 = (undefined4 *)*puVar2;
  }
  FUN_012461a0(puVar2,0xf,0x3f7ae1483f7ae148,&DAT_3f7ae148,0,0xc);
  local_14 = 0xffffffff;
  FUN_00f1a330();
  if (param_2 != '\0') {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 1;
    local_34 = DAT_01bfe468 + 8;
    local_60 = DAT_01bb9b2c;
    local_30 = 0xb;
    local_58 = DAT_01bb9b34;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar4 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(s_ACCESSORY_ENCHANT_REGIST_STONE_01af3474,0x1e);
    psVar3 = (short *)FUN_015ce480(&local_78,uVar4,uVar11,uVar12,uVar13,uVar14);
    local_14._0_1_ = 2;
    if (7 < *(uint *)(psVar3 + 10)) {
      psVar3 = *(short **)psVar3;
    }
    psVar8 = psVar3;
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar3,(int)psVar8 - (int)(psVar3 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,1);
    FUN_00f1a330();
    uVar4 = FUN_01246bb0();
    local_14 = 0xffffffff;
    *(undefined4 *)(param_1 + 0xe4) = uVar4;
    local_34 = 0;
    FUN_00f1a330();
  }
  if (*(int *)(*(int *)(param_1 + 0xa8) + 0x204) == 0) {
    uVar4 = 0;
  }
  else {
    local_7c = (void *)0x0;
    iVar6 = FUN_015d18f0(&local_7c);
    if (iVar6 == 0) {
      uVar4 = 0;
    }
    else {
      uVar4 = *(undefined4 *)(iVar6 + 4);
    }
  }
  local_68 = 0;
  local_64 = 0;
  local_78 = (short ***)0x0;
  uStack_74 = 0;
  uStack_70 = 0;
  uStack_6c = 0;
  FUN_00f1a200(&DAT_01ac5dd8,0);
  local_14 = 3;
  FUN_01500f80(&local_78,&DAT_01ae08c4,uVar4);
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = 0;
  local_28 = 0;
  local_14 = CONCAT31(local_14._1_3_,4);
  ppppsVar9 = &local_78;
  if (7 < local_64) {
    ppppsVar9 = (short ****)local_78;
  }
  local_34 = DAT_01bfe468 + 8;
  local_30 = 10;
  local_60 = CONCAT44(DAT_01b026d0,DAT_01b025f8);
  local_58 = 0x3f800000;
  local_3c = 1;
  local_2c = 0;
  local_38 = 0;
  ppppsVar7 = ppppsVar9;
  do {
    sVar1 = *(short *)ppppsVar7;
    ppppsVar7 = (short ****)((int)ppppsVar7 + 2);
  } while (sVar1 != 0);
  FUN_00f1a040(ppppsVar9,(int)ppppsVar7 - (int)((int)ppppsVar9 + 2) >> 1);
  uVar4 = FUN_01246bb0();
  *(undefined4 *)(param_1 + 0xe8) = uVar4;
  local_34 = 0;
  FUN_00f1a330();
  local_14 = 0xffffffff;
  FUN_00f1a330();
  if (param_2 != '\0') {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 5;
    local_34 = DAT_01bfe468 + 8;
    local_60 = DAT_01bb9b2c;
    local_30 = 0xb;
    local_58 = DAT_01bb9b34;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar4 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(s_COMMON_TXT_ACCESSORY_01ae2e40,0x14);
    psVar3 = (short *)FUN_015ce480(&local_78,uVar4,uVar11,uVar12,uVar13,uVar14);
    local_14._0_1_ = 6;
    if (7 < *(uint *)(psVar3 + 10)) {
      psVar3 = *(short **)psVar3;
    }
    psVar8 = psVar3;
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar3,(int)psVar8 - (int)(psVar3 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,5);
    FUN_00f1a330();
    uVar4 = FUN_01246bb0();
    local_14 = 0xffffffff;
    *(undefined4 *)(param_1 + 0xe0) = uVar4;
    local_34 = 0;
    FUN_00f1a330();
  }
  local_7c = (void *)FUN_016830c0(0x1ac);
  local_14 = 7;
  if (local_7c == (void *)0x0) {
    piVar5 = (int *)0x0;
  }
  else {
    memset(local_7c,0,0x1ac);
    piVar5 = (int *)FUN_012435e0();
  }
  local_14 = 0xffffffff;
  *(int **)(param_1 + 0xb0) = piVar5;
  if (piVar5 != (int *)0x0) {
    (**(code **)(*piVar5 + 0xc))
              (*(undefined4 *)(param_1 + 0x40),DAT_01bee88c,DAT_01bee890,0x2c5,0x1b7,0);
    FUN_01377e70(1,param_1,FUN_01260f60,0);
    iVar6 = *(int *)(param_1 + 0xb0);
    *(undefined4 *)(iVar6 + 0xc0) = 0;
    *(undefined4 *)(iVar6 + 0xdc) = 0;
    puVar2 = (undefined4 *)(iVar6 + 0xb0);
    if (7 < *(uint *)(iVar6 + 0xc4)) {
      puVar2 = (undefined4 *)*puVar2;
    }
    *(undefined2 *)puVar2 = 0;
    piVar5 = *(int **)(param_1 + 0xb0);
    piVar5[0x3b] = 1;
    piVar5[0x3c] = 0;
    fVar10 = (float10)fmaxf(0,0x3fd56042);
    piVar5[0x32] = (int)(float)fVar10;
    FUN_01243010();
    (**(code **)(*piVar5 + 0x48))();
    FUN_01242bf0();
    *(undefined1 *)(*(int *)(param_1 + 0xb0) + 0xe4) = 0;
    FUN_012472c0(*(undefined4 *)(param_1 + 0xb0));
  }
  iVar6 = FUN_012462c0(s_Accessory_Enchant_Enchant_Enchan_01ae83e0,0x17,1,0x9e,0x80,0x65,0x21,0,0x1a
                      );
  *(int *)(param_1 + 0xd0) = iVar6;
  if (iVar6 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 8;
    local_34 = DAT_01bfe468 + 8;
    local_60 = DAT_01bb9b2c;
    local_30 = 9;
    local_58 = DAT_01bb9b34;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    uVar4 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(s_ACCESSORY_ENCHANT_ENCHANTMENT_01ae837c,0x1d);
    psVar3 = (short *)FUN_015ce480(&local_78,uVar4,uVar11,uVar12,uVar13,uVar14);
    local_14._0_1_ = 9;
    if (7 < *(uint *)(psVar3 + 10)) {
      psVar3 = *(short **)psVar3;
    }
    psVar8 = psVar3;
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar3,(int)psVar8 - (int)(psVar3 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,8);
    FUN_00f1a330();
    FUN_01224f40(&local_60);
    FUN_01377e70();
    (**(code **)(**(int **)(param_1 + 0xd0) + 0x18))(0);
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar6 = FUN_012462c0(s_System_Ch_Close_tga_01ad561c,0x17,1,0x2a8,0x10,0x14,0x14,0,0x10);
  *(int *)(param_1 + 0xd4) = iVar6;
  if (iVar6 != 0) {
    FUN_01377e70();
  }
  local_7c = (void *)FUN_016830c0(0x8c);
  local_14 = 10;
  if (local_7c == (void *)0x0) {
    iVar6 = 0;
    uVar4 = extraout_ECX;
  }
  else {
    memset(local_7c,0,0x8c);
    iVar6 = FUN_012404a0();
    uVar4 = extraout_ECX_00;
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 300) = iVar6;
  if (iVar6 != 0) {
    FUN_01240640(1,*(undefined4 *)(param_1 + 0x40),0xffffffd8,0x55,500,0x1b8,
                 s_Accessory_Enchant_EnchantChange__01af340c,0,0x10,uVar4,0x100,0x100,1,4);
    iVar6 = *(int *)(param_1 + 300);
    *(undefined4 *)(iVar6 + 100) = 0x3d4ccccd;
    *(undefined4 *)(iVar6 + 0x68) = 0x3d4ccccd;
    (**(code **)(**(int **)(param_1 + 300) + 0x1c))(0);
    (**(code **)(**(int **)(param_1 + 300) + 0x44))(0x3f333333);
    FUN_012472c0(*(undefined4 *)(param_1 + 300));
  }
  FUN_01373b60();
  FUN_01373520();
  FUN_01373840();
  FUN_01373ea0();
  ExceptionList = local_1c;
  return;
}


