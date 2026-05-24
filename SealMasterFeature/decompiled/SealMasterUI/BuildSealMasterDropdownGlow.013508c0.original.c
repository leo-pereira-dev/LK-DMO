
/* ============================================================ */
/* Function: FUN_013508c0 */
/* Entry: 013508c0 */
/* Signature: undefined FUN_013508c0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_013508c0(int param_1,undefined4 param_2,uint param_3)

{
  char cVar1;
  short sVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  uint uVar7;
  short *psVar8;
  short *psVar9;
  undefined4 *puVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  undefined4 uVar18;
  undefined1 local_bc [4];
  float local_b8;
  float local_b4;
  undefined4 local_b0;
  undefined4 *local_ac;
  void *local_a8;
  void *local_a4;
  void *local_a0;
  int local_9c;
  int local_98;
  short *local_94;
  int local_90;
  undefined8 local_8c;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined1 local_68;
  undefined4 local_64;
  int local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
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
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019a9a81;
  local_10 = ExceptionList;
  uVar7 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar3 = *(int *)(param_1 + 0xa8);
  local_b0 = param_2;
  local_14 = uVar7;
  FUN_00f1b390(local_bc,&param_3);
  if ((*(char *)((int)local_b4 + 0xd) != '\0') ||
     (fVar11 = local_b4, param_3 < *(uint *)((int)local_b4 + 0x10))) {
    fVar11 = *(float *)(iVar3 + 0xc);
  }
  puVar4 = *(undefined4 **)((int)fVar11 + 0x38);
  local_9c = 0;
  puVar10 = (undefined4 *)*puVar4;
  while( true ) {
    if (puVar10 == puVar4) {
      ExceptionList = local_10;
      return;
    }
    local_a0 = (void *)FUN_016830c0(0x38,uVar7);
    local_8 = 0;
    if (local_a0 == (void *)0x0) {
      local_90 = 0;
    }
    else {
      uVar18 = 0x38;
      memset(local_a0,0,0x38);
      local_90 = FUN_01235b20(local_9c,uVar18);
    }
    local_8 = 0xffffffff;
    if (local_90 == 0) {
      ExceptionList = local_10;
      return;
    }
    local_a4 = (void *)FUN_016830c0(0x2c,uVar7);
    local_8 = 1;
    if (local_a4 == (void *)0x0) {
      local_98 = 0;
    }
    else {
      memset(local_a4,0,0x2c);
      local_98 = FUN_010baa10();
    }
    local_8 = 0xffffffff;
    if (local_98 == 0) {
      ExceptionList = local_10;
      return;
    }
    local_a8 = (void *)FUN_016830c0(0x1c0);
    local_8 = 2;
    if (local_a8 == (void *)0x0) {
      local_94 = (short *)0x0;
    }
    else {
      memset(local_a8,0,0x1c0);
      local_94 = (short *)FUN_01222670();
    }
    if (local_94 == (short *)0x0) break;
    local_70 = 0;
    local_6c = 7;
    uStack_7c = 0;
    uStack_78 = 0;
    uStack_74 = 0;
    local_80 = 0;
    local_54 = 0;
    local_8 = 3;
    local_60 = DAT_01bfe468 + 8;
    local_5c = 10;
    local_8c = DAT_01bb9b2c;
    local_84 = DAT_01bb9b34;
    local_68 = 1;
    local_58 = 0;
    local_64 = 0;
    FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0xbc,0x1f,s_SealMaster_common_dropdown_glow__01af1df0,0
                );
    FUN_0124eff0(&local_b8);
    fVar17 = local_b4 * DAT_01b028e0;
    *(undefined4 *)((int)local_94 + 0xe0) = 0;
    fVar16 = local_b8 * 0.0;
    *(undefined4 *)((int)local_94 + 0xe4) = 0;
    *(undefined4 *)((int)local_94 + 0xe8) = 0x3f800000;
    fVar15 = fVar17 + fVar17;
    *(float *)((int)local_94 + 0xec) = fVar17;
    *(float *)((int)local_94 + 0xf0) = fVar16;
    fVar11 = fVar16 * DAT_01b02858;
    *(undefined4 *)((int)local_94 + 0xf8) = 0x3f800000;
    fVar14 = fVar16 + fVar16;
    *(float *)((int)local_94 + 0xf4) = fVar17;
    *(float *)((int)local_94 + 0xfc) = fVar15;
    fVar13 = fVar17 * DAT_01b02858;
    *(undefined4 *)((int)local_94 + 0x108) = 0x3f800000;
    fVar12 = fVar17 * DAT_01b02890;
    *(undefined4 *)((int)local_94 + 0x118) = 0x3f800000;
    *(float *)((int)local_94 + 0x110) = fVar11;
    *(float *)((int)local_94 + 0x150) = fVar11;
    *(float *)((int)local_94 + 0x100) = fVar14;
    *(float *)((int)local_94 + 0x104) = fVar15;
    *(float *)((int)local_94 + 0x10c) = fVar13;
    *(float *)((int)local_94 + 0x114) = fVar13;
    *(undefined4 *)((int)local_94 + 0x11c) = 0x3f800000;
    *(undefined4 *)((int)local_94 + 0x140) = 0;
    *(undefined4 *)((int)local_94 + 0x144) = 0;
    *(undefined4 *)((int)local_94 + 0x148) = 0x3f800000;
    *(float *)((int)local_94 + 0x14c) = fVar17;
    *(float *)((int)local_94 + 0x120) = fVar16;
    *(float *)((int)local_94 + 0x124) = fVar17;
    *(undefined4 *)((int)local_94 + 0x128) = 0x3f800000;
    *(float *)((int)local_94 + 300) = fVar15;
    *(float *)((int)local_94 + 0x130) = fVar14;
    *(float *)((int)local_94 + 0x134) = fVar15;
    *(undefined4 *)((int)local_94 + 0x138) = 0x3f800000;
    *(float *)((int)local_94 + 0x13c) = fVar13;
    *(float *)((int)local_94 + 0x154) = fVar13;
    *(undefined4 *)((int)local_94 + 0x158) = 0x3f800000;
    *(float *)((int)local_94 + 0x15c) = fVar12;
    fVar11 = fVar16 * DAT_01b02890;
    *(float *)((int)local_94 + 0x164) = fVar12;
    fVar12 = fVar17 * DAT_01b02898;
    *(undefined4 *)((int)local_94 + 0x168) = 0x3f800000;
    *(float *)((int)local_94 + 0x160) = fVar11;
    fVar11 = fVar16 * DAT_01b02898;
    *(undefined4 *)((int)local_94 + 0x178) = 0x3f800000;
    *(float *)((int)local_94 + 0x16c) = fVar12;
    *(float *)((int)local_94 + 0x174) = fVar12;
    fVar12 = fVar17 * DAT_01b0289c;
    *(undefined4 *)((int)local_94 + 0x188) = 0x3f800000;
    *(float *)((int)local_94 + 0x170) = fVar11;
    fVar11 = fVar16 * DAT_01b0289c;
    *(undefined4 *)((int)local_94 + 0x198) = 0x3f800000;
    fVar16 = fVar16 * DAT_01b028b0;
    *(undefined4 *)((int)local_94 + 0x1a0) = 0;
    *(float *)((int)local_94 + 0x180) = fVar11;
    fVar11 = fVar17 * DAT_01b028b0;
    *(undefined4 *)((int)local_94 + 0x1a4) = 0;
    *(float *)((int)local_94 + 0x17c) = fVar12;
    *(float *)((int)local_94 + 0x184) = fVar12;
    *(float *)((int)local_94 + 0x18c) = fVar11;
    *(float *)((int)local_94 + 0x194) = fVar11;
    fVar11 = fVar17 * DAT_01b028b4;
    *(undefined4 *)((int)local_94 + 0x1a8) = 0x3f800000;
    *(float *)((int)local_94 + 400) = fVar16;
    *(float *)((int)local_94 + 0x1ac) = fVar17;
    *(float *)((int)local_94 + 0x19c) = fVar11;
    *(float *)((int)local_94 + 0x1b0) = fVar14;
    *(float *)((int)local_94 + 0x1b4) = fVar15;
    *(undefined4 *)((int)local_94 + 0x1b8) = 0x3f800000;
    *(float *)((int)local_94 + 0x1bc) = fVar13;
    local_ac = operator_new(0x10);
    uVar18 = puVar10[4];
    local_ac[1] = local_9c;
    local_ac[2] = param_3;
    *local_ac = &DAT_01af1f50;
    local_ac[3] = uVar18;
    if (*(undefined4 **)((int)local_94 + 0x34) != (undefined4 *)0x0) {
      (**(code **)**(undefined4 **)((int)local_94 + 0x34))(1);
    }
    *(undefined4 **)((int)local_94 + 0x34) = local_ac;
    FUN_012317d0(local_94,0,0,0,0xbc,0x1f,DAT_01bee88c,DAT_01bee890);
    local_34 = 0;
    local_30 = 7;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    local_44 = 0;
    local_18 = 0;
    local_8 = CONCAT31(local_8._1_3_,4);
    psVar9 = (short *)(puVar10 + 6);
    local_24 = DAT_01bfe468 + 8;
    local_20 = 10;
    local_50 = DAT_01bb9b2c;
    local_48 = DAT_01bb9b34;
    local_2c = 1;
    local_1c = 0;
    local_28 = 0;
    if (7 < (uint)puVar10[0xb]) {
      psVar9 = *(short **)psVar9;
    }
    local_94 = psVar9 + 1;
    psVar8 = psVar9;
    do {
      sVar2 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar2 != 0);
    FUN_00f1a040(psVar9,(int)psVar8 - (int)local_94 >> 1);
    FUN_01230c40(&local_50,0x19,8);
    FUN_01235ea0(local_98);
    FUN_00f55430(&local_90);
    local_24 = 0;
    FUN_00f1a330();
    local_8 = 0xffffffff;
    local_60 = 0;
    FUN_00f1a330();
    puVar5 = (undefined4 *)puVar10[2];
    if (*(char *)((int)puVar5 + 0xd) == '\0') {
      cVar1 = *(char *)((int)*puVar5 + 0xd);
      puVar10 = puVar5;
      puVar5 = (undefined4 *)*puVar5;
      while (cVar1 == '\0') {
        cVar1 = *(char *)((int)*puVar5 + 0xd);
        puVar10 = puVar5;
        puVar5 = (undefined4 *)*puVar5;
      }
    }
    else {
      cVar1 = *(char *)((int)puVar10[1] + 0xd);
      puVar6 = (undefined4 *)puVar10[1];
      puVar5 = puVar10;
      while ((puVar10 = puVar6, cVar1 == '\0' && (puVar5 == (undefined4 *)puVar10[2]))) {
        cVar1 = *(char *)((int)puVar10[1] + 0xd);
        puVar6 = (undefined4 *)puVar10[1];
        puVar5 = puVar10;
      }
    }
    local_9c = local_9c + 1;
  }
  ExceptionList = local_10;
  return;
}


