
/* ============================================================ */
/* Function: FUN_011d3b00 */
/* Entry: 011d3b00 */
/* Signature: undefined FUN_011d3b00() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011d3b00(int param_1)

{
  short sVar1;
  undefined2 uVar2;
  uint uVar3;
  short *psVar4;
  int iVar5;
  void *pvVar6;
  int *piVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 uVar11;
  short ******ppppppsVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined8 uVar16;
  undefined4 uVar17;
  int local_d0;
  int local_c4;
  void *local_b4;
  undefined4 local_b0;
  int local_ac;
  int *local_a8;
  undefined4 local_9c;
  uint local_98;
  undefined1 local_94;
  undefined4 local_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 local_80;
  undefined4 local_7c;
  short *****local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined8 local_68;
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
  puStack_18 = &LAB_01993189;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
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
  local_38 = 9;
  local_68 = CONCAT44(&DAT_3f7ae148,(undefined4)local_68);
  local_58 = &DAT_3f7ae148;
  uVar11 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar16 = 0;
  uVar17 = 0;
  local_ac = param_1;
  local_24 = uVar3;
  FUN_00f19cc0(s_ENCY_SKILL_INFO_01adda60,0xf);
  psVar4 = (short *)FUN_015ce480(&local_90,uVar11,uVar13,uVar14,uVar16,uVar17);
  local_14._0_1_ = 1;
  if (7 < *(uint *)(psVar4 + 10)) {
    psVar4 = *(short **)psVar4;
  }
  do {
    sVar1 = *psVar4;
    psVar4 = psVar4 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040();
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330(uVar3);
  FUN_01246bb0(&local_60,1,0x84,0x122);
  local_14 = 0xffffffff;
  local_34 = 0;
  FUN_00f1a330();
  uVar11 = extraout_ECX;
  if (*(int *)(param_1 + 0x80) == 0) {
    local_b4 = (void *)FUN_016830c0(0xac);
    local_14 = 2;
    if (local_b4 == (void *)0x0) {
      iVar5 = 0;
      uVar11 = extraout_ECX_00;
    }
    else {
      memset(local_b4,0,0xac);
      iVar5 = FUN_01228950();
      uVar11 = extraout_ECX_01;
    }
    local_14 = 0xffffffff;
    *(int *)(param_1 + 0x80) = iVar5;
    if (iVar5 == 0) {
      ExceptionList = local_1c;
      return;
    }
  }
  FUN_01228bd0(*(undefined4 *)(param_1 + 0x40),0x23,0x122,0xbf,0x60,0x23,0x10,0x28,0x28,0,2,uVar11,3
              );
  iVar5 = *(int *)(param_1 + 0x80);
  *(undefined4 *)(iVar5 + 0xa0) = 0;
  *(undefined4 *)(iVar5 + 0xa4) = 0;
  FUN_012472c0(*(undefined4 *)(param_1 + 0x80));
  FUN_0122a180();
  local_98 = 0;
  do {
    pvVar6 = (void *)FUN_016830c0(0x2c);
    local_14 = 3;
    if (pvVar6 == (void *)0x0) {
      iVar5 = 0;
    }
    else {
      memset(pvVar6,0,0x2c);
      iVar5 = FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (iVar5 == 0) {
      ExceptionList = local_1c;
      return;
    }
    pvVar6 = (void *)FUN_016830c0(0xa0);
    local_14 = 4;
    if (pvVar6 == (void *)0x0) {
      local_b0 = 0;
    }
    else {
      memset(pvVar6,0,0xa0);
      local_b0 = FUN_0123de10();
    }
    uVar13 = DAT_01bee890;
    uVar11 = DAT_01bb9b34;
    local_14 = 0xffffffff;
    local_a8 = (int *)DAT_01bee88c;
    local_68 = DAT_01bb9b2c;
    FUN_00f19f10();
    local_80 = 0;
    uStack_8c = 0;
    uStack_88 = 0;
    uStack_84 = 0;
    local_7c = 0xf;
    local_90 = 0;
    local_14 = 5;
    FUN_015010a0(&local_90,&DAT_01ae7290,&DAT_01bfbd40,s_EquipSystem_Upgrade_Slot_tga_01ae2c40);
    FUN_0124ef30(&local_90);
    uVar14 = local_b0;
    uVar15 = 0;
    uVar16 = local_68;
    uVar17 = FUN_0124f150(0,local_68,uVar11);
    FUN_0123e330(0,local_a8,uVar13,0x28,0x28,uVar17,uVar15,uVar16,uVar11);
    local_14 = 0xffffffff;
    FUN_00f1a420();
    piVar7 = (int *)FUN_01230f40(uVar14,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
    if (piVar7 != (int *)0x0) {
      (**(code **)(*piVar7 + 0x54))(1);
    }
    local_94 = (undefined1)local_98;
    iVar8 = *(int *)(local_ac + 0x78);
    FUN_00f489b0();
    if ((((*(char *)(local_c4 + 0xd) == '\0') && (*(byte *)(local_c4 + 0x10) <= (byte)local_98)) &&
        (local_c4 != *(int *)(iVar8 + 0xa4))) && (*(int *)(iVar8 + 0x104) != 0)) {
      local_b4 = *(void **)(local_c4 + 0x14);
      iVar8 = FUN_015d17e0(&local_b4);
    }
    else {
      iVar8 = 0;
    }
    local_94 = (undefined1)local_98;
    local_a8 = (int *)(*(int *)(local_ac + 0x78) + 0xac);
    FUN_01012bc0();
    if (((*(char *)(local_d0 + 0xd) == '\0') && (*(byte *)(local_d0 + 0xe) <= (byte)local_98)) &&
       (local_d0 != *local_a8)) {
      uVar2 = *(undefined2 *)(local_d0 + 0xf);
      local_9c = CONCAT22(local_9c._2_2_,uVar2);
    }
    else {
      local_9c = (uint)local_9c._2_2_ << 0x10;
      uVar2 = 0;
    }
    if (((iVar8 == 0) || ((char)uVar2 == '\0')) || ((char)(local_9c >> 8) == '\0')) {
      puVar9 = (undefined4 *)FUN_016830c0(0x28);
      local_a8 = puVar9;
      if (puVar9 != (undefined4 *)0x0) {
        *puVar9 = 0;
        puVar9[1] = 0;
        puVar9[2] = 0;
        puVar9[3] = 0;
        puVar9[4] = 0;
        puVar9[5] = 0;
        puVar9[6] = 0;
        puVar9[7] = 0;
        *(undefined8 *)(puVar9 + 8) = 0;
        *puVar9 = &DAT_01ae66e0;
        puVar9[1] = 0;
        puVar9[2] = 0;
        puVar9[3] = local_98;
        *(undefined1 *)(puVar9 + 4) = 0;
        puVar9[5] = 0x28;
        puVar9[6] = 0x28;
        *(undefined2 *)(puVar9 + 7) = 0x101;
        puVar9[8] = 0;
        puVar9[9] = 0;
        FUN_012288c0(iVar5);
        puVar10 = operator_new(8);
        *puVar10 = &DAT_01ae2dfc;
        puVar10[1] = local_98;
        local_a8 = puVar10;
        if ((undefined4 *)puVar9[2] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar9[2])(1);
        }
        puVar9[2] = puVar10;
        FUN_0122a090(puVar9);
      }
    }
    else {
      piVar7 = (int *)FUN_012314f0(*(undefined2 *)(iVar8 + 0xa2),*(undefined2 *)(iVar8 + 0xa2),0,
                                   0x20,0x20,4,4);
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))(1);
      }
      local_44 = 0;
      local_40 = 7;
      uStack_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      local_54 = 0;
      local_28 = 0;
      local_14 = 6;
      local_34 = DAT_01bfe468 + 8;
      local_58 = &DAT_3f7ae148;
      local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_30 = 9;
      local_3c = 1;
      local_2c = 0;
      local_38 = 1;
      if (local_9c._1_1_ == '\x01') {
        FUN_00f1a040();
        local_60 = CONCAT44(DAT_01b025f4,DAT_01b026a8);
        local_58 = (undefined1 *)0x3f800000;
      }
      else {
        local_68 = 0x700000000;
        uStack_74 = 0;
        uStack_70 = 0;
        uStack_6c = 0;
        local_78 = (short *****)0x0;
        local_14._0_1_ = 7;
        local_14._1_3_ = 0;
        FUN_01500f80(&local_78,u__d__d_01ace96c,local_9c & 0xff,local_9c._1_1_);
        ppppppsVar12 = &local_78;
        if (7 < local_68._4_4_) {
          ppppppsVar12 = (short ******)local_78;
        }
        do {
          sVar1 = *(short *)ppppppsVar12;
          ppppppsVar12 = (short ******)((int)ppppppsVar12 + 2);
        } while (sVar1 != 0);
        FUN_00f1a040();
        if ((char)local_9c == local_9c._1_1_) {
          local_58 = (undefined1 *)0x3e40c0c1;
          uVar11 = DAT_01b026d0;
          uVar13 = DAT_01b026f0;
        }
        else {
          local_58 = (undefined1 *)0x3f800000;
          uVar11 = DAT_01b02728;
          uVar13 = DAT_01b025c0;
        }
        local_60 = CONCAT44(uVar11,uVar13);
        local_14 = CONCAT31(local_14._1_3_,6);
        FUN_00f1a330();
      }
      piVar7 = (int *)FUN_01230c40(&local_60,0x14,0x28);
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))(1);
      }
      puVar9 = (undefined4 *)FUN_016830c0(0x28);
      local_a8 = puVar9;
      if (puVar9 != (undefined4 *)0x0) {
        *puVar9 = 0;
        puVar9[1] = 0;
        puVar9[2] = 0;
        puVar9[3] = 0;
        puVar9[4] = 0;
        puVar9[5] = 0;
        puVar9[6] = 0;
        puVar9[7] = 0;
        *(undefined8 *)(puVar9 + 8) = 0;
        *puVar9 = &DAT_01ae66e0;
        puVar9[1] = 0;
        puVar9[2] = 0;
        puVar9[3] = local_98;
        *(undefined1 *)(puVar9 + 4) = 0;
        puVar9[5] = 0x28;
        puVar9[6] = 0x28;
        *(undefined2 *)(puVar9 + 7) = 0x101;
        puVar9[8] = 0;
        puVar9[9] = 0;
        FUN_012288c0(iVar5);
        puVar10 = operator_new(8);
        *puVar10 = &DAT_01ae2dfc;
        puVar10[1] = local_98;
        local_a8 = puVar10;
        if ((undefined4 *)puVar9[2] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar9[2])(1);
        }
        puVar9[2] = puVar10;
        FUN_0122a090(puVar9);
      }
      local_14 = 0xffffffff;
      local_34 = 0;
      FUN_00f1a330();
    }
    local_98 = local_98 + 1;
  } while (local_98 < 6);
  ExceptionList = local_1c;
  return;
}


