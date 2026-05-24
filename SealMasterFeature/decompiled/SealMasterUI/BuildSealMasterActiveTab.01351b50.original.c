
/* ============================================================ */
/* Function: FUN_01351b50 */
/* Entry: 01351b50 */
/* Signature: undefined FUN_01351b50() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_01351b50(void)

{
  short sVar1;
  void *pvVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  short ****ppppsVar7;
  short ****ppppsVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  float local_74;
  undefined4 *local_70;
  void *local_6c;
  short ***local_68 [5];
  uint local_54;
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
  puStack_c = &LAB_019a9af5;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar3;
  local_6c = (void *)FUN_016830c0(0x38,uVar3);
  local_8 = 0;
  if (local_6c == (void *)0x0) {
    iVar4 = 0;
  }
  else {
    uVar20 = 0x38;
    memset(local_6c,0,0x38);
    iVar4 = FUN_01235b20(2,uVar20);
  }
  local_8 = 0xffffffff;
  if (iVar4 != 0) {
    local_6c = (void *)FUN_016830c0(0x2c,uVar3);
    local_8 = 1;
    if (local_6c == (void *)0x0) {
      local_6c = (void *)0x0;
    }
    else {
      memset(local_6c,0,0x2c);
      local_6c = (void *)FUN_010baa10();
    }
    local_8 = 0xffffffff;
    if (local_6c != (void *)0x0) {
      local_70 = (undefined4 *)FUN_016830c0(0x1c0,uVar3);
      local_8 = 2;
      if (local_70 == (void *)0x0) {
        iVar5 = 0;
      }
      else {
        memset(local_70,0,0x1c0);
        iVar5 = FUN_01222670();
      }
      local_8 = 0xffffffff;
      if (iVar5 != 0) {
        FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0xbc,0x1f,s_CommonUI_CommonUI_tap_btn_tga_01adc364,
                     0);
        FUN_0124eff0(&local_74);
        fVar15 = (float)local_70 * DAT_01b02900;
        *(undefined4 *)(iVar5 + 0xe0) = 0;
        local_74 = local_74 * 0.0;
        *(undefined4 *)(iVar5 + 0xe4) = 0;
        *(undefined4 *)(iVar5 + 0xe8) = 0x3f800000;
        fVar10 = fVar15 * DAT_01b02890;
        *(undefined4 *)(iVar5 + 0xf8) = 0x3f800000;
        *(float *)(iVar5 + 0xec) = fVar15;
        fVar9 = local_74 * DAT_01b02890;
        fVar14 = fVar15 + fVar15;
        *(undefined4 *)(iVar5 + 0x108) = 0x3f800000;
        *(float *)(iVar5 + 0x15c) = fVar10;
        *(float *)(iVar5 + 0x164) = fVar10;
        fVar13 = local_74 + local_74;
        *(float *)(iVar5 + 0x160) = fVar9;
        fVar11 = fVar15 * DAT_01b02898;
        *(undefined4 *)(iVar5 + 0x118) = 0x3f800000;
        fVar9 = local_74 * DAT_01b02898;
        *(undefined4 *)(iVar5 + 0x11c) = 0x3f800000;
        fVar12 = fVar15 * DAT_01b02858;
        *(undefined4 *)(iVar5 + 0x140) = 0;
        fVar10 = local_74 * DAT_01b02858;
        *(undefined4 *)(iVar5 + 0x144) = 0;
        *(float *)(iVar5 + 0x16c) = fVar11;
        *(float *)(iVar5 + 0x170) = fVar9;
        fVar9 = local_74 * DAT_01b0289c;
        *(undefined4 *)(iVar5 + 0x148) = 0x3f800000;
        *(float *)(iVar5 + 0x174) = fVar11;
        fVar11 = fVar15 * DAT_01b0289c;
        *(undefined4 *)(iVar5 + 0x128) = 0x3f800000;
        *(float *)(iVar5 + 0xf0) = local_74;
        *(float *)(iVar5 + 0xf4) = fVar15;
        *(float *)(iVar5 + 0xfc) = fVar14;
        *(float *)(iVar5 + 0x100) = fVar13;
        *(float *)(iVar5 + 0x104) = fVar14;
        *(float *)(iVar5 + 0x10c) = fVar12;
        *(float *)(iVar5 + 0x110) = fVar10;
        *(float *)(iVar5 + 0x114) = fVar12;
        *(float *)(iVar5 + 0x14c) = fVar15;
        *(float *)(iVar5 + 0x120) = local_74;
        *(float *)(iVar5 + 0x124) = fVar15;
        *(float *)(iVar5 + 300) = fVar14;
        *(float *)(iVar5 + 0x130) = fVar13;
        *(float *)(iVar5 + 0x134) = fVar14;
        *(undefined4 *)(iVar5 + 0x138) = 0x3f800000;
        *(float *)(iVar5 + 0x13c) = fVar12;
        *(float *)(iVar5 + 0x150) = fVar10;
        *(float *)(iVar5 + 0x154) = fVar12;
        *(undefined4 *)(iVar5 + 0x158) = 0x3f800000;
        *(undefined4 *)(iVar5 + 0x168) = 0x3f800000;
        *(undefined4 *)(iVar5 + 0x178) = 0x3f800000;
        *(float *)(iVar5 + 0x17c) = fVar11;
        *(float *)(iVar5 + 0x180) = fVar9;
        *(float *)(iVar5 + 0x184) = fVar11;
        *(undefined4 *)(iVar5 + 0x188) = 0x3f800000;
        local_74 = local_74 * DAT_01b028b0;
        fVar10 = fVar15 * DAT_01b028b0;
        *(undefined4 *)(iVar5 + 0x198) = 0x3f800000;
        fVar9 = fVar15 * DAT_01b028b4;
        *(undefined4 *)(iVar5 + 0x1a0) = 0;
        *(float *)(iVar5 + 0x18c) = fVar10;
        *(float *)(iVar5 + 400) = local_74;
        *(float *)(iVar5 + 0x194) = fVar10;
        *(float *)(iVar5 + 0x19c) = fVar9;
        *(undefined4 *)(iVar5 + 0x1a4) = 0;
        *(undefined4 *)(iVar5 + 0x1a8) = 0x3f800000;
        *(float *)(iVar5 + 0x1ac) = fVar15;
        *(float *)(iVar5 + 0x1b0) = fVar13;
        *(float *)(iVar5 + 0x1b4) = fVar14;
        *(undefined4 *)(iVar5 + 0x1b8) = 0x3f800000;
        *(float *)(iVar5 + 0x1bc) = fVar12;
        FUN_012317d0(iVar5,0,0,0,0xbc,0x1f,DAT_01bee88c,DAT_01bee890);
        local_34 = 0;
        local_30 = 7;
        uStack_40 = 0;
        uStack_3c = 0;
        uStack_38 = 0;
        local_44 = 0;
        local_18 = 0;
        local_8 = 3;
        local_24 = DAT_01bfe468 + 8;
        local_50 = DAT_01bb9b2c;
        local_20 = 10;
        local_48 = DAT_01bb9b34;
        local_2c = 1;
        local_1c = 0;
        local_28 = 0;
        uVar20 = 0;
        uVar16 = 0;
        uVar17 = 0;
        uVar18 = 0;
        uVar19 = 0;
        uVar21 = 0;
        FUN_00f19cc0(s_SEALMASTER_SHOW_ACTIVE_01af1e3c,0x16);
        FUN_015ce480(local_68,uVar20,uVar16,uVar17,uVar18,uVar19,uVar21);
        local_8 = CONCAT31(local_8._1_3_,4);
        ppppsVar8 = local_68;
        if (7 < local_54) {
          ppppsVar8 = (short ****)local_68[0];
        }
        ppppsVar7 = ppppsVar8;
        do {
          sVar1 = *(short *)ppppsVar7;
          ppppsVar7 = (short ****)((int)ppppsVar7 + 2);
        } while (sVar1 != 0);
        FUN_00f1a040(ppppsVar8,(int)ppppsVar7 - (int)((int)ppppsVar8 + 2) >> 1);
        pvVar2 = local_6c;
        FUN_01230c40(&local_50,0x19,8);
        FUN_01235ea0(pvVar2);
        puVar6 = operator_new(0x10);
        *puVar6 = &DAT_01af1f50;
        puVar6[1] = 2;
        puVar6[2] = 0;
        puVar6[3] = 0xffffffff;
        local_70 = puVar6;
        if (*(undefined4 **)(iVar4 + 0x10) != (undefined4 *)0x0) {
          (**(code **)**(undefined4 **)(iVar4 + 0x10))(1);
        }
        *(undefined4 **)(iVar4 + 0x10) = puVar6;
        FUN_01236f00(iVar4);
        FUN_00f1a330();
        local_24 = 0;
        FUN_00f1a330();
      }
    }
  }
  ExceptionList = local_10;
  return;
}


