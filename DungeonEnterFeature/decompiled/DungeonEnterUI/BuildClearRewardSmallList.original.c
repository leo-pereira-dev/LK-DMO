
/* ============================================================ */
/* Function: FUN_010cd810 */
/* Entry: 010cd810 */
/* Signature: undefined FUN_010cd810() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int __thiscall FUN_010cd810(int param_1,char *param_2)

{
  short sVar1;
  uint uVar2;
  short ****ppppsVar3;
  void *pvVar4;
  undefined4 uVar5;
  int *piVar6;
  int iVar7;
  undefined4 *puVar8;
  char *pcVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined8 uVar13;
  undefined4 uVar14;
  int local_8c;
  int local_88;
  undefined8 local_80;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined1 local_5c;
  undefined4 local_58;
  int local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  short ***local_2c;
  short **ppsStack_28;
  short **ppsStack_24;
  short **ppsStack_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  puStack_c = &LAB_0198141d;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  if (*(int *)(param_1 + 0x7c) == 0) {
    return 0;
  }
  local_1c = 0x700000000;
  ppsStack_28 = (short **)0x0;
  ppsStack_24 = (short **)0x0;
  ppsStack_20 = (short **)0x0;
  local_2c = (short ***)0x0;
  local_8 = 0;
  ExceptionList = &local_10;
  local_14 = uVar2;
  if (*param_2 == '\0') {
    uVar10 = 0x29;
    pcVar9 = s_DUNGEON_ENTER_CLEAR_REWARD_DEFAU_01ad4fc8;
LAB_010cd89d:
    uVar11 = 0;
    uVar12 = 0;
    uVar5 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(pcVar9,uVar10);
    ppppsVar3 = (short ****)FUN_015ce480(&local_44,uVar11,uVar12,uVar5,uVar13,uVar14);
    if (&local_2c != ppppsVar3) {
      FUN_00f1a330();
      local_2c = *ppppsVar3;
      ppsStack_28 = (short **)ppppsVar3[1];
      ppsStack_24 = (short **)ppppsVar3[2];
      ppsStack_20 = (short **)ppppsVar3[3];
      local_1c = *(undefined8 *)(ppppsVar3 + 4);
      *(undefined2 *)ppppsVar3 = 0;
      ppppsVar3[5] = (short ***)0x7;
      ppppsVar3[4] = (short ***)0x0;
    }
    FUN_00f1a330();
  }
  else if (*param_2 == '\x01') {
    uVar10 = 0x27;
    pcVar9 = s_DUNGEON_ENTER_CLEAR_REWARD_EXTRA_01ad4ff4;
    goto LAB_010cd89d;
  }
  pvVar4 = (void *)FUN_016830c0(0x2c,uVar2);
  local_8._0_1_ = 1;
  if (pvVar4 == (void *)0x0) {
    local_8c = 0;
  }
  else {
    memset(pvVar4,0,0x2c);
    local_8c = FUN_010baa10();
  }
  local_8._0_1_ = 0;
  if (local_8c != 0) {
    pvVar4 = (void *)FUN_016830c0(0xa0,uVar2);
    local_8._0_1_ = 2;
    if (pvVar4 == (void *)0x0) {
      local_88 = 0;
    }
    else {
      memset(pvVar4,0,0xa0);
      local_88 = FUN_0123de10();
    }
    uVar12 = DAT_01bee890;
    uVar11 = DAT_01bee88c;
    uVar10 = DAT_01bb9b34;
    uVar13 = DAT_01bb9b2c;
    local_8._0_1_ = 0;
    if (local_88 != 0) {
      FUN_00f19f10();
      local_34 = 0;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_30 = 0xf;
      local_44 = 0;
      local_8._0_1_ = 3;
      FUN_015010a0(&local_44,&DAT_01ae7290,&DAT_01bfbd40,s_DungeonUI_Dungeon_list_S_png_01ad501c);
      FUN_0124ef30(&local_44);
      uVar14 = 0;
      uVar5 = FUN_0124f150(0,uVar13,uVar10);
      FUN_0123e330(0,uVar11,uVar12,0x6c,0x1e,uVar5,uVar14,uVar13,uVar10);
      local_8._0_1_ = 0;
      FUN_00f1a420();
      piVar6 = (int *)FUN_01230f40(local_88,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      if (piVar6 != (int *)0x0) {
        (**(code **)(*piVar6 + 0x54))(1);
      }
    }
    local_64 = 0;
    local_60 = 7;
    uStack_70 = 0;
    uStack_6c = 0;
    uStack_68 = 0;
    local_74 = 0;
    local_48 = 0;
    local_8 = CONCAT31(local_8._1_3_,4);
    local_54 = DAT_01bfe468 + 8;
    ppppsVar3 = &local_2c;
    if (7 < local_1c._4_4_) {
      ppppsVar3 = (short ****)local_2c;
    }
    local_50 = 0xb;
    local_80 = DAT_01bb9b2c;
    local_78 = DAT_01bb9b34;
    local_5c = 1;
    local_4c = 0;
    local_58 = 5;
    do {
      sVar1 = *(short *)ppppsVar3;
      ppppsVar3 = (short ****)((int)ppppsVar3 + 2);
    } while (sVar1 != 0);
    FUN_00f1a040();
    piVar6 = (int *)FUN_01230c40(&local_80,0x36,0xf);
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x54))(1);
    }
    local_8 = local_8 & 0xffffff00;
    local_54 = 0;
    FUN_00f1a330();
    pvVar4 = (void *)FUN_016830c0(0x38);
    local_8._0_1_ = 5;
    if (pvVar4 == (void *)0x0) {
      iVar7 = 0;
    }
    else {
      memset(pvVar4,0,0x38);
      iVar7 = FUN_01235b20();
    }
    local_8._0_1_ = 0;
    if (iVar7 != 0) {
      *(undefined4 *)(local_8c + 0x10) = 0x1e;
      FUN_01235ea0(local_8c);
      puVar8 = operator_new(0xc);
      *puVar8 = &DAT_01ad51bc;
      puVar8[1] = 0;
      *(undefined2 *)(puVar8 + 2) = 0;
      if (*(undefined4 **)(iVar7 + 0x10) != (undefined4 *)0x0) {
        (**(code **)**(undefined4 **)(iVar7 + 0x10))(1);
      }
      *(undefined4 **)(iVar7 + 0x10) = puVar8;
      FUN_01236f00(iVar7);
      goto LAB_010cdbca;
    }
  }
  local_8._0_1_ = 0;
  iVar7 = 0;
LAB_010cdbca:
  FUN_00f1a330();
  ExceptionList = local_10;
  return iVar7;
}


