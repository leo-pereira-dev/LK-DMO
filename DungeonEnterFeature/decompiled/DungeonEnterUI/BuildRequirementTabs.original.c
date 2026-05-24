
/* ============================================================ */
/* Function: FUN_010cb260 */
/* Entry: 010cb260 */
/* Signature: undefined FUN_010cb260() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int __thiscall FUN_010cb260(int param_1,int param_2)

{
  short sVar1;
  undefined1 *puVar2;
  uint uVar3;
  short ****ppppsVar4;
  void *pvVar5;
  undefined4 uVar6;
  int *piVar7;
  int iVar8;
  char *pcVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined8 uVar13;
  undefined4 uVar14;
  int local_a0;
  int local_9c;
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
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 local_40;
  short ***local_3c;
  short **ppsStack_38;
  short **ppsStack_34;
  short **ppsStack_30;
  undefined8 local_2c;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  uint local_14;
  
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_01980fe6;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  if (*(int *)(param_1 + 0xb4) == 0) {
    return 0;
  }
  local_2c = 0x700000000;
  ppsStack_38 = (short **)0x0;
  ppsStack_34 = (short **)0x0;
  ppsStack_30 = (short **)0x0;
  local_3c = (short ***)0x0;
  local_14 = 0;
  ExceptionList = &local_1c;
  local_24 = uVar3;
  if (param_2 == 0) {
    uVar10 = 0x26;
    pcVar9 = s_DUNGEON_ENTER_REQUIRE_TITLE_CLEA_01ad4c50;
LAB_010cb318:
    uVar11 = 0;
    uVar12 = 0;
    uVar6 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(pcVar9,uVar10);
    ppppsVar4 = (short ****)FUN_015ce480(&local_54,uVar11,uVar12,uVar6,uVar13,uVar14);
    if (&local_3c != ppppsVar4) {
      FUN_00f1a330();
      local_3c = *ppppsVar4;
      ppsStack_38 = (short **)ppppsVar4[1];
      ppsStack_34 = (short **)ppppsVar4[2];
      ppsStack_30 = (short **)ppppsVar4[3];
      local_2c = *(undefined8 *)(ppppsVar4 + 4);
      *(undefined2 *)ppppsVar4 = 0;
      ppppsVar4[5] = (short ***)0x7;
      ppppsVar4[4] = (short ***)0x0;
    }
    FUN_00f1a330();
    puVar2 = puStack_20;
  }
  else {
    if (param_2 == 1) {
      uVar10 = 0x26;
      pcVar9 = s_DUNGEON_ENTER_REQUIRE_TITLE_ENTE_01ad4c78;
      goto LAB_010cb318;
    }
    puVar2 = &stack0xfffffffc;
    if (param_2 == 2) {
      uVar10 = 0x25;
      pcVar9 = s_DUNGEON_ENTER_REQUIRE_TITLE_ITEM_01ad4ca0;
      goto LAB_010cb318;
    }
  }
  puStack_20 = puVar2;
  pvVar5 = (void *)FUN_016830c0(0x2c,uVar3);
  local_14._0_1_ = 1;
  if (pvVar5 == (void *)0x0) {
    local_a0 = 0;
  }
  else {
    memset(pvVar5,0,0x2c);
    local_a0 = FUN_010baa10();
  }
  local_14._0_1_ = 0;
  if (local_a0 != 0) {
    pvVar5 = (void *)FUN_016830c0(0xa0,uVar3);
    local_14._0_1_ = 2;
    if (pvVar5 == (void *)0x0) {
      local_9c = 0;
    }
    else {
      memset(pvVar5,0,0xa0);
      local_9c = FUN_0123de10();
    }
    uVar12 = DAT_01bee890;
    uVar11 = DAT_01bee88c;
    uVar10 = DAT_01bb9b34;
    uVar13 = DAT_01bb9b2c;
    local_14._0_1_ = 0;
    if (local_9c != 0) {
      FUN_00f19f10();
      local_44 = 0;
      uStack_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      local_40 = 0xf;
      local_54 = 0;
      local_14._0_1_ = 3;
      FUN_015010a0(&local_54,&DAT_01ae7290,&DAT_01bfbd40,s_DungeonUI_Dungeon_Line_tga_01ad4a48);
      FUN_0124ef30(&local_54);
      uVar14 = 0;
      uVar6 = FUN_0124f150(0,uVar13,uVar10);
      FUN_0123e330(0,uVar11,uVar12,0xd9,0x2c,uVar6,uVar14,uVar13,uVar10);
      local_14._0_1_ = 0;
      FUN_00f1a420();
      piVar7 = (int *)FUN_01230f40(local_9c,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))(1);
      }
    }
    local_74 = 0;
    local_70 = 7;
    uStack_80 = 0;
    uStack_7c = 0;
    uStack_78 = 0;
    local_84 = 0;
    local_58 = 0;
    local_14 = CONCAT31(local_14._1_3_,4);
    local_64 = DAT_01bfe468 + 8;
    ppppsVar4 = &local_3c;
    if (7 < local_2c._4_4_) {
      ppppsVar4 = (short ****)local_3c;
    }
    local_60 = 0xb;
    local_90 = CONCAT44(DAT_01b02674,DAT_01b02634);
    local_88 = 0x3f76f6f7;
    local_6c = 1;
    local_5c = 0;
    local_68 = 0;
    do {
      sVar1 = *(short *)ppppsVar4;
      ppppsVar4 = (short ****)((int)ppppsVar4 + 2);
    } while (sVar1 != 0);
    FUN_00f1a040();
    piVar7 = (int *)FUN_01230c40(&local_90,0x14,3);
    if (piVar7 != (int *)0x0) {
      (**(code **)(*piVar7 + 0x54))(1);
    }
    local_14 = local_14 & 0xffffff00;
    local_64 = 0;
    FUN_00f1a330();
    pvVar5 = (void *)FUN_016830c0(0x38);
    local_14._0_1_ = 5;
    if (pvVar5 == (void *)0x0) {
      iVar8 = 0;
    }
    else {
      memset(pvVar5,0,0x38);
      iVar8 = FUN_01235b20();
    }
    local_14._0_1_ = 0;
    if (iVar8 != 0) {
      *(undefined4 *)(local_a0 + 0x10) = 0x1e;
      FUN_01235ea0(local_a0);
      FUN_01236f00(iVar8);
      goto LAB_010cb637;
    }
  }
  local_14._0_1_ = 0;
  iVar8 = 0;
LAB_010cb637:
  FUN_00f1a330();
  ExceptionList = local_1c;
  return iVar8;
}


