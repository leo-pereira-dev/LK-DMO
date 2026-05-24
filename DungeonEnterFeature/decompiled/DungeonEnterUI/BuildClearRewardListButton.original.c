
/* ============================================================ */
/* Function: FUN_010cdc00 */
/* Entry: 010cdc00 */
/* Signature: undefined FUN_010cdc00() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_010cdc00(int param_1,undefined4 *param_2,undefined1 *param_3,undefined4 *param_4)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  short ****ppppsVar8;
  short ****ppppsVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  undefined4 uVar17;
  undefined4 local_84;
  undefined4 uStack_80;
  undefined4 local_7c;
  int local_78;
  float local_74;
  undefined4 *local_70;
  undefined4 *local_6c;
  undefined8 local_68;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined1 local_44;
  undefined4 local_40;
  int local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  short ***local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01981495;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_78 = param_1;
  local_6c = param_4;
  if (param_1 != 0) {
    local_14 = uVar2;
    local_70 = (undefined4 *)FUN_016830c0(0x2c,uVar2);
    local_8 = 0;
    if (local_70 == (void *)0x0) {
      iVar3 = 0;
    }
    else {
      memset(local_70,0,0x2c);
      iVar3 = FUN_010baa10();
    }
    if (iVar3 != 0) {
      local_1c = 0;
      local_18 = 7;
      uStack_28 = 0;
      uStack_24 = 0;
      uStack_20 = 0;
      local_2c = (short ***)0x0;
      local_8 = 1;
      local_84 = 0;
      uStack_80 = 0;
      local_7c = 0;
      FUN_010cefe0(param_3,&local_2c,&local_84);
      local_70 = (undefined4 *)FUN_016830c0(0x1c0,uVar2);
      local_8._0_1_ = 2;
      if (local_70 == (void *)0x0) {
        iVar4 = 0;
      }
      else {
        memset(local_70,0,0x1c0);
        iVar4 = FUN_01222670();
      }
      local_8._0_1_ = 1;
      if (iVar4 != 0) {
        FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0x6c,0x1e,s_DungeonUI_Dungeon_list_btn_png_01ad503c
                     ,0);
        FUN_0124eff0(&local_74);
        fVar16 = (float)local_70 * DAT_01b028fc;
        *(undefined4 *)(iVar4 + 0xe0) = 0;
        local_74 = local_74 * 0.0;
        *(undefined4 *)(iVar4 + 0xe4) = 0;
        *(undefined4 *)(iVar4 + 0xe8) = 0x3f800000;
        fVar11 = fVar16 * DAT_01b02890;
        *(undefined4 *)(iVar4 + 0xf8) = 0x3f800000;
        *(float *)(iVar4 + 0xec) = fVar16;
        fVar10 = local_74 * DAT_01b02890;
        fVar15 = fVar16 + fVar16;
        *(undefined4 *)(iVar4 + 0x108) = 0x3f800000;
        *(float *)(iVar4 + 0x15c) = fVar11;
        *(float *)(iVar4 + 0x164) = fVar11;
        fVar14 = local_74 + local_74;
        *(float *)(iVar4 + 0x160) = fVar10;
        fVar12 = fVar16 * DAT_01b02898;
        *(undefined4 *)(iVar4 + 0x118) = 0x3f800000;
        fVar10 = local_74 * DAT_01b02898;
        *(undefined4 *)(iVar4 + 0x11c) = 0x3f800000;
        fVar13 = fVar16 * DAT_01b02858;
        *(undefined4 *)(iVar4 + 0x140) = 0;
        fVar11 = local_74 * DAT_01b02858;
        *(undefined4 *)(iVar4 + 0x144) = 0;
        *(float *)(iVar4 + 0x16c) = fVar12;
        *(float *)(iVar4 + 0x170) = fVar10;
        fVar10 = local_74 * DAT_01b0289c;
        *(undefined4 *)(iVar4 + 0x148) = 0x3f800000;
        *(float *)(iVar4 + 0x174) = fVar12;
        fVar12 = fVar16 * DAT_01b0289c;
        *(undefined4 *)(iVar4 + 0x128) = 0x3f800000;
        *(float *)(iVar4 + 0xf0) = local_74;
        *(float *)(iVar4 + 0xf4) = fVar16;
        *(float *)(iVar4 + 0xfc) = fVar15;
        *(float *)(iVar4 + 0x100) = fVar14;
        *(float *)(iVar4 + 0x104) = fVar15;
        *(float *)(iVar4 + 0x10c) = fVar13;
        *(float *)(iVar4 + 0x110) = fVar11;
        *(float *)(iVar4 + 0x114) = fVar13;
        *(float *)(iVar4 + 0x14c) = fVar16;
        *(float *)(iVar4 + 0x120) = local_74;
        *(float *)(iVar4 + 0x124) = fVar16;
        *(float *)(iVar4 + 300) = fVar15;
        *(float *)(iVar4 + 0x130) = fVar14;
        *(float *)(iVar4 + 0x134) = fVar15;
        *(undefined4 *)(iVar4 + 0x138) = 0x3f800000;
        *(float *)(iVar4 + 0x13c) = fVar13;
        *(float *)(iVar4 + 0x150) = fVar11;
        *(float *)(iVar4 + 0x154) = fVar13;
        *(undefined4 *)(iVar4 + 0x158) = 0x3f800000;
        *(undefined4 *)(iVar4 + 0x168) = 0x3f800000;
        *(undefined4 *)(iVar4 + 0x178) = 0x3f800000;
        *(float *)(iVar4 + 0x17c) = fVar12;
        *(float *)(iVar4 + 0x180) = fVar10;
        *(float *)(iVar4 + 0x184) = fVar12;
        *(undefined4 *)(iVar4 + 0x188) = 0x3f800000;
        local_74 = local_74 * DAT_01b028b0;
        *(undefined4 *)(iVar4 + 0x198) = 0x3f800000;
        fVar11 = fVar16 * DAT_01b028b0;
        *(undefined4 *)(iVar4 + 0x1a0) = 0;
        fVar10 = fVar16 * DAT_01b028b4;
        *(undefined4 *)(iVar4 + 0x1a4) = 0;
        *(float *)(iVar4 + 0x18c) = fVar11;
        *(float *)(iVar4 + 400) = local_74;
        *(float *)(iVar4 + 0x194) = fVar11;
        *(float *)(iVar4 + 0x19c) = fVar10;
        *(undefined4 *)(iVar4 + 0x1a8) = 0x3f800000;
        *(float *)(iVar4 + 0x1ac) = fVar16;
        *(float *)(iVar4 + 0x1b0) = fVar14;
        *(float *)(iVar4 + 0x1b4) = fVar15;
        *(undefined4 *)(iVar4 + 0x1b8) = 0x3f800000;
        *(float *)(iVar4 + 0x1bc) = fVar13;
        piVar5 = (int *)FUN_012317d0(iVar4,*local_6c,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,
                                     DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        if (piVar5 != (int *)0x0) {
          (**(code **)(*piVar5 + 0x54))(1);
        }
      }
      local_4c = 0;
      local_48 = 7;
      uStack_58 = 0;
      uStack_54 = 0;
      uStack_50 = 0;
      local_5c = 0;
      local_30 = 0;
      local_8 = CONCAT31(local_8._1_3_,3);
      local_68 = CONCAT44(uStack_80,local_84);
      local_3c = DAT_01bfe468 + 8;
      ppppsVar9 = &local_2c;
      if (7 < local_18) {
        ppppsVar9 = (short ****)local_2c;
      }
      local_38 = 0xb;
      local_60 = local_7c;
      local_34 = 0;
      local_40 = 5;
      local_44 = 0;
      ppppsVar8 = ppppsVar9;
      do {
        sVar1 = *(short *)ppppsVar8;
        ppppsVar8 = (short ****)((int)ppppsVar8 + 2);
      } while (sVar1 != 0);
      FUN_00f1a040(ppppsVar9,(int)ppppsVar8 - (int)((int)ppppsVar9 + 2) >> 1);
      piVar5 = (int *)FUN_01230c40(&local_68,0x36,0xf);
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0x54))(1);
      }
      local_8._0_1_ = 1;
      local_3c = 0;
      FUN_00f1a330();
      local_70 = (undefined4 *)FUN_016830c0(0x38);
      local_8._0_1_ = 4;
      if (local_70 == (undefined4 *)0x0) {
        puVar6 = (undefined4 *)0x0;
      }
      else {
        uVar17 = 0x38;
        memset(local_70,0,0x38);
        puVar6 = (undefined4 *)FUN_01235b20(*local_6c,uVar17);
      }
      local_8 = CONCAT31(local_8._1_3_,1);
      if (puVar6 != (undefined4 *)0x0) {
        *(undefined4 *)(iVar3 + 0x10) = 0x1e;
        FUN_01235ea0(iVar3);
        puVar7 = operator_new(0xc);
        *puVar7 = &DAT_01ad51bc;
        puVar7[1] = *param_2;
        *(undefined1 *)(puVar7 + 2) = *param_3;
        *(undefined1 *)((int)puVar7 + 9) = 1;
        local_70 = puVar7;
        if ((undefined4 *)puVar6[4] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar6[4])(1);
        }
        puVar6[4] = puVar7;
        local_6c = puVar6;
        FUN_00f55430(&local_6c);
      }
      FUN_00f1a330();
    }
  }
  ExceptionList = local_10;
  return;
}


