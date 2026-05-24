
/* ============================================================ */
/* Function: FUN_011d65d0 */
/* Entry: 011d65d0 */
/* Signature: undefined FUN_011d65d0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011d65d0(int param_1)

{
  char cVar1;
  short sVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined1 *puVar5;
  undefined4 *puVar6;
  void *pvVar7;
  int iVar8;
  undefined8 *puVar9;
  int *piVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  int iVar13;
  short ******ppppppsVar14;
  int iVar15;
  undefined4 *puVar16;
  int *piVar17;
  undefined8 uVar18;
  undefined4 uStack_1b8;
  undefined4 uStack_1b4;
  undefined1 *puStack_1b0;
  undefined4 uStack_1ac;
  int iStack_1a8;
  undefined4 uStack_1a4;
  undefined4 uStack_1a0;
  int iStack_19c;
  undefined4 *puStack_198;
  char *pcStack_194;
  undefined4 *puStack_190;
  undefined4 *local_18c;
  undefined8 *puStack_188;
  short *****pppppsStack_184;
  int local_154;
  int local_148;
  undefined1 *local_124;
  undefined4 *local_11c;
  undefined4 *local_118;
  undefined4 local_110;
  undefined4 uStack_10c;
  uint local_100;
  int local_fc;
  undefined4 *local_f0;
  undefined4 *local_e0 [3];
  undefined8 local_d4;
  undefined1 local_cc [16];
  int local_bc;
  undefined4 local_b4;
  undefined4 uStack_b0;
  undefined4 *puStack_ac;
  undefined4 uStack_a8;
  int local_a4;
  undefined1 *local_a0;
  short *****local_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined8 local_8c;
  undefined8 local_84;
  undefined1 *local_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined1 local_60;
  undefined4 local_5c;
  int *local_58;
  undefined8 *local_54;
  undefined1 *local_50;
  undefined4 local_4c;
  undefined8 local_48;
  short *****local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01993544;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  puVar6 = (undefined4 *)((*(int *)(param_1 + 0x38) + -0x144) / 3);
  puStack_190 = (undefined4 *)0x1b;
  local_18c = (undefined4 *)0x0;
  puStack_188 = (undefined8 *)0x0;
  pppppsStack_184 = (short *****)0x0;
  pcStack_194 = s_INVENTORY_FILTER_TEXT_EQUIP_01ae2e58;
  puStack_198 = (undefined4 *)0x11d666b;
  FUN_00f19cc0();
  puStack_190 = (undefined4 *)local_cc;
  pcStack_194 = (char *)0x11d6679;
  FUN_015ce480();
  local_14 = 0;
  pppppsStack_184 = (short *****)((int)puVar6 + 0x1e);
  puStack_188 = (undefined8 *)0x11d669f;
  FUN_011d7cf0();
  local_14 = 0xffffffff;
  FUN_00f1a330();
  puStack_190 = (undefined4 *)0x14;
  local_18c = (undefined4 *)0x0;
  puStack_188 = (undefined8 *)0x0;
  pppppsStack_184 = (short *****)0x0;
  pcStack_194 = s_DETAIL_INFO_EQUIP_LV_01ae2e74;
  puStack_198 = (undefined4 *)0x11d66dc;
  FUN_00f19cc0();
  puStack_190 = (undefined4 *)local_cc;
  pcStack_194 = (char *)0x11d66ea;
  FUN_015ce480();
  local_14 = 1;
  pppppsStack_184 = (short *****)((int)puVar6 + 0x61);
  puStack_188 = (undefined8 *)0x11d6710;
  FUN_011d7cf0();
  local_14 = 0xffffffff;
  FUN_00f1a330();
  puStack_190 = (undefined4 *)0x16;
  local_18c = (undefined4 *)0x0;
  puStack_188 = (undefined8 *)0x0;
  pppppsStack_184 = (short *****)0x0;
  pcStack_194 = s_DETAIL_INFO_EQUIP_NAME_01ae2eb8;
  puStack_198 = (undefined4 *)0x11d674d;
  FUN_00f19cc0();
  puStack_190 = (undefined4 *)local_cc;
  pcStack_194 = (char *)0x11d675b;
  FUN_015ce480();
  local_14 = 2;
  pppppsStack_184 = (short *****)((int)puVar6 + 0xdf);
  puStack_188 = (undefined8 *)0x11d6783;
  FUN_011d7cf0();
  local_14 = 0xffffffff;
  FUN_00f1a330();
  pvVar7 = (void *)FUN_016830c0();
  local_14 = 3;
  if (pvVar7 == (void *)0x0) {
    iVar8 = 0;
  }
  else {
    pppppsStack_184 = (short *****)0x11d67c2;
    memset(pvVar7,0,0xa0);
    iVar8 = FUN_0123de10();
  }
  local_14 = 0xffffffff;
  if (iVar8 != 0) {
    uVar3 = *(undefined4 *)(param_1 + 0x40);
    FUN_00f19f10();
    local_a4 = 0;
    uStack_b0 = 0;
    puStack_ac = (undefined4 *)0x0;
    uStack_a8 = 0;
    local_a0 = (undefined1 *)0xf;
    local_b4 = 0;
    pppppsStack_184 = (short *****)&local_b4;
    local_14 = 4;
    puStack_188 = (undefined8 *)0x11d6854;
    FUN_015010a0();
    FUN_0124ef30();
    pppppsStack_184 = (short *****)0x0;
    puStack_188 = (undefined8 *)0x11d688c;
    puStack_188 = (undefined8 *)FUN_0124f150();
    local_18c = (undefined4 *)0x1d;
    puStack_190 = (undefined4 *)0x134;
    pcStack_194 = (char *)0x19;
    uStack_1a0 = 0x11d68a4;
    iStack_19c = uVar3;
    puStack_198 = puVar6;
    FUN_0123e330();
    local_14 = 0xffffffff;
    FUN_00f1a420();
    FUN_012472c0();
    if (*(int *)(param_1 + 0x94) == 0) {
      pvVar7 = (void *)FUN_016830c0();
      local_14 = 5;
      if (pvVar7 == (void *)0x0) {
        iVar8 = 0;
      }
      else {
        pppppsStack_184 = (short *****)0x11d68fb;
        memset(pvVar7,0,0x94);
        iVar8 = FUN_0122b460();
      }
      local_14 = 0xffffffff;
      *(int *)(param_1 + 0x94) = iVar8;
      if (iVar8 == 0) {
        ExceptionList = local_1c;
        return;
      }
    }
    pppppsStack_184 = (short *****)(*(int *)(param_1 + 0x38) + (int)puVar6 * -2);
    puStack_188 = (undefined8 *)0x36;
    puStack_190 = *(undefined4 **)(param_1 + 0x40);
    pcStack_194 = (char *)0x11d694d;
    local_18c = puVar6;
    FUN_0122b6f0();
    iVar8 = *(int *)(param_1 + 0x94);
    *(undefined4 *)(iVar8 + 0x7c) = 0;
    *(undefined4 *)(iVar8 + 0x80) = 0;
    *(undefined4 *)(*(int *)(param_1 + 0x94) + 0x78) = 0x36;
    iVar8 = *(int *)(param_1 + 0x94);
    *(undefined4 *)(iVar8 + 0x84) = 0;
    *(undefined4 *)(iVar8 + 0x88) = 0;
    FUN_012472c0();
    puVar16 = (undefined4 *)0x0;
    local_f0 = (undefined4 *)0x0;
    local_100 = 0;
    if (*(int *)(param_1 + 0x88) - *(int *)(param_1 + 0x84) >> 2 != 0) {
      do {
        if (local_100 == 0) {
LAB_011d6c63:
          if (puVar16 < *(undefined4 **)(*(int *)(param_1 + 0x84) + local_100 * 4)) {
            do {
              local_e0[0] = puVar16;
              FUN_00f1d720();
              if (((*(char *)(local_148 + 0xd) == '\0') &&
                  (*(int *)(local_148 + 0x10) <= (int)puVar16)) &&
                 (local_148 != *(int *)(param_1 + 0x7c))) {
                puVar5 = *(undefined1 **)(local_148 + 0x14);
                local_28 = (undefined4 *)puVar5;
                pvVar7 = (void *)FUN_016830c0();
                local_14 = 9;
                if (pvVar7 == (void *)0x0) {
                  iVar8 = 0;
                }
                else {
                  pppppsStack_184 = (short *****)0x11d6cf0;
                  memset(pvVar7,0,0x2c);
                  iVar8 = FUN_010baa10();
                }
                local_14 = 0xffffffff;
                if (iVar8 != 0) {
                  piVar17 = (int *)(param_1 + 0x9c);
                  local_e0[0] = (undefined4 *)*piVar17;
                  piVar10 = *(int **)((int)local_e0[0] + 4);
                  cVar1 = *(char *)((int)piVar10 + 0xd);
                  while (cVar1 == '\0') {
                    FUN_00f1d770();
                    piVar10 = (int *)*piVar10;
                    FUN_017986fc();
                    cVar1 = *(char *)((int)piVar10 + 0xd);
                  }
                  *(undefined4 **)((int)local_e0[0] + 4) = local_e0[0];
                  *local_e0[0] = local_e0[0];
                  *(undefined4 **)((int)local_e0[0] + 8) = local_e0[0];
                  *(undefined4 *)(param_1 + 0xa0) = 0;
                  piVar10 = (int *)FUN_010102c0();
                  if (piVar17 != piVar10) {
                    FUN_00f6e780();
                    iVar15 = *piVar17;
                    *piVar17 = *piVar10;
                    *piVar10 = iVar15;
                    iVar15 = *(int *)(param_1 + 0xa0);
                    *(int *)(param_1 + 0xa0) = piVar10[1];
                    piVar10[1] = iVar15;
                  }
                  FUN_00f31c20();
                  local_118 = (undefined4 *)0x0;
                  FUN_00f1b390();
                  if ((*(char *)(local_154 + 0xd) == '\0') &&
                     (*(undefined1 **)(local_154 + 0x10) <= local_28)) {
                    local_fc = local_154;
                    if (local_154 != *piVar17) {
                      iVar15 = *(int *)(local_154 + 0x14);
                      if (iVar15 != 0) {
                        local_118 = *(undefined4 **)(iVar15 + 4);
                        if ((local_118 == (undefined4 *)0x0) && (*(short *)(iVar15 + 8) == 0)) {
                          local_118 = (undefined4 *)0x0;
                        }
                      }
                    }
                  }
                  else {
                    local_fc = *piVar17;
                  }
                  pvVar7 = (void *)FUN_016830c0();
                  local_14 = 10;
                  if (pvVar7 == (void *)0x0) {
                    local_28 = (undefined4 *)0x0;
                  }
                  else {
                    pppppsStack_184 = (short *****)0x11d6e4b;
                    memset(pvVar7,0,0xa0);
                    local_28 = (undefined4 *)FUN_0123de10();
                  }
                  uVar3 = DAT_01bee890;
                  local_14 = 0xffffffff;
                  local_e0[0] = DAT_01bee88c;
                  FUN_00f19f10();
                  local_30 = 0;
                  uStack_3c = 0;
                  uStack_38 = 0;
                  uStack_34 = 0;
                  uStack_2c = 0xf;
                  local_40 = (short *****)0x0;
                  pppppsStack_184 = (short *****)&local_40;
                  local_14 = 0xb;
                  puStack_188 = (undefined8 *)0x11d6ed6;
                  FUN_015010a0();
                  FUN_0124ef30();
                  puVar16 = local_28;
                  pppppsStack_184 = (short *****)0x0;
                  puStack_188 = (undefined8 *)0x11d6f05;
                  puStack_188 = (undefined8 *)FUN_0124f150();
                  local_18c = (undefined4 *)0x36;
                  puStack_190 = (undefined4 *)0x134;
                  pcStack_194 = (char *)uVar3;
                  puStack_198 = local_e0[0];
                  iStack_19c = 0;
                  uStack_1a0 = 0x11d6f1d;
                  FUN_0123e330();
                  local_14 = 0xffffffff;
                  FUN_00f1a420();
                  pppppsStack_184 = (short *****)DAT_01bee88c;
                  puStack_188 = (undefined8 *)puVar16;
                  local_18c = (undefined4 *)0x11d6f4d;
                  piVar10 = (int *)FUN_01230f40();
                  if (piVar10 != (int *)0x0) {
                    (**(code **)(*piVar10 + 0x54))();
                  }
                  if (local_118 == (undefined4 *)0x0) {
                    local_11c = (undefined4 *)FUN_016830c0();
                    local_14 = 0xc;
                    if (local_11c == (undefined4 *)0x0) {
                      puVar16 = (undefined4 *)0x0;
                    }
                    else {
                      pppppsStack_184 = (short *****)0x11d6fb6;
                      memset(local_11c,0,0xbc);
                      FUN_0123de10();
                      *local_11c = &DAT_01ae721c;
                      *(undefined2 *)(local_11c + 0x28) = 0;
                      *(undefined1 *)((int)local_11c + 0xa2) = 0;
                      local_11c[0x29] = 1;
                      local_11c[0x2a] = 1;
                      local_11c[0x2b] = 0xffffffff;
                      local_11c[0x2c] = 0x3f800000;
                      local_11c[0x2d] = 0x3f800000;
                      local_11c[0x2e] = 0;
                      puVar16 = local_11c;
                    }
                    local_14 = 0xffffffff;
                    if (puVar16 != (undefined4 *)0x0) {
                      pppppsStack_184 = (short *****)0x23;
                      puStack_188 = (undefined8 *)0x23;
                      local_18c = (undefined4 *)DAT_01bee890;
                      puStack_190 = DAT_01bee88c;
                      pcStack_194 = (char *)0x0;
                      puStack_198 = (undefined4 *)0x11d7043;
                      FUN_0123d7c0();
                      FUN_0123d9a0();
                      pppppsStack_184 = (short *****)&DAT_0000000d;
                      puStack_188 = (undefined8 *)local_f0;
                      puStack_190 = (undefined4 *)0x11d7076;
                      local_18c = puVar16;
                      piVar10 = (int *)FUN_01231700();
                      goto LAB_011d7076;
                    }
                  }
                  else {
                    pppppsStack_184 = (short *****)&DAT_00000023;
                    puStack_188 = (undefined8 *)0x1;
                    local_18c = local_118;
                    puStack_190 = (undefined4 *)0xa;
                    pcStack_194 = (char *)0x11d6f84;
                    piVar10 = (int *)FUN_01231410();
LAB_011d7076:
                    if (piVar10 != (int *)0x0) {
                      (**(code **)(*piVar10 + 0x54))();
                    }
                  }
                  local_28 = (undefined4 *)0x0;
                  if ((local_fc != *(int *)(param_1 + 0x9c)) && (*(int *)(local_fc + 0x14) != 0)) {
                    local_28 = (undefined4 *)(uint)*(byte *)(*(int *)(local_fc + 0x14) + 0xd);
                  }
                  local_8c._0_4_ = (undefined8 *)0x0;
                  local_8c._4_4_ = 7;
                  uStack_98 = 0;
                  uStack_94 = 0;
                  uStack_90 = 0;
                  local_9c = (short *****)0x0;
                  local_14 = 0xd;
                  FUN_015011b0();
                  if ((undefined8 *)local_8c == (undefined8 *)0x7ffffffe) {
                    /* WARNING: Subroutine does not return */
                    FUN_00f1a480();
                  }
                  pppppsStack_184 = (short *****)&DAT_01ae2f40;
                  puStack_188 = (undefined8 *)local_8c;
                  local_18c = local_11c;
                  puStack_190 = (undefined4 *)0x11d7125;
                  FUN_00f52ec0();
                  FUN_00f1a330();
                  local_9c = local_40;
                  local_40 = (short *****)((uint)local_40 & 0xffff0000);
                  uStack_98 = uStack_3c;
                  uStack_94 = uStack_38;
                  uStack_90 = uStack_34;
                  local_8c = CONCAT44(uStack_2c,local_30);
                  local_30 = 0;
                  uStack_2c = 7;
                  FUN_00f1a330();
                  local_68 = 0;
                  local_64 = 7;
                  uStack_74 = 0;
                  uStack_70 = 0;
                  uStack_6c = 0;
                  local_78 = 0;
                  local_4c = 0;
                  local_14 = CONCAT31(local_14._1_3_,0xe);
                  ppppppsVar14 = &local_9c;
                  if (7 < local_8c._4_4_) {
                    ppppppsVar14 = (short ******)local_9c;
                  }
                  local_58 = (int *)(DAT_01bfe468 + 8);
                  local_54 = (undefined8 *)0xa;
                  local_84 = CONCAT44(DAT_01b02744,DAT_01b02744);
                  local_7c = &DAT_3f7ae148;
                  local_60 = 1;
                  local_50 = (undefined1 *)0x0;
                  local_5c = 5;
                  do {
                    sVar2 = *(short *)ppppppsVar14;
                    ppppppsVar14 = (short ******)((int)ppppppsVar14 + 2);
                  } while (sVar2 != 0);
                  FUN_00f1a040();
                  pppppsStack_184 = (short *****)0x11d721f;
                  piVar10 = (int *)FUN_01230c40();
                  if (piVar10 == (int *)0x0) {
                    local_58 = piVar10;
                    FUN_00f1a330();
                    local_14 = 0xffffffff;
                    FUN_00f1a330();
                  }
                  else {
                    (**(code **)(*piVar10 + 0x54))();
                    local_58 = (int *)0x0;
                    FUN_00f1a330();
                    local_14 = 0xffffffff;
                    FUN_00f1a330();
                    FUN_01010530();
                    local_14 = 0xf;
                    if (local_bc == 0) {
                      FUN_00f1a040();
                    }
                    local_30 = 0;
                    uStack_2c = 0;
                    local_40 = (short *****)0x0;
                    uStack_3c = 0;
                    uStack_38 = 0;
                    uStack_34 = 0;
                    FUN_00f1a200();
                    local_14._0_1_ = 0x10;
                    pppppsStack_184 = (short *****)0x11d72ea;
                    FUN_01502130();
                    FUN_00f1a330();
                    local_68 = 0;
                    local_64 = 7;
                    uStack_74 = 0;
                    uStack_70 = 0;
                    uStack_6c = 0;
                    local_78 = 0;
                    local_4c = 0;
                    local_14 = CONCAT31(local_14._1_3_,0x11);
                    local_58 = (int *)(DAT_01bfe468 + 8);
                    iVar15 = *(int *)(param_1 + 0x78);
                    local_a0 = &DAT_3f7ae148;
                    local_7c = &DAT_3f7ae148;
                    local_84 = CONCAT44(DAT_01b02744,DAT_01b02744);
                    local_54 = (undefined8 *)&DAT_00000009;
                    local_60 = 1;
                    local_50 = (undefined1 *)0x0;
                    local_5c = 0;
                    iVar13 = FUN_010103b0();
                    local_124 = DAT_01bb9b34;
                    uVar18 = DAT_01bb9b2c;
                    if ((iVar13 != 0) &&
                       (((*(undefined4 **)(iVar13 + 4) != (undefined4 *)0x0 ||
                         (*(short *)(iVar13 + 8) != 0)) && (*(int *)(iVar13 + 0x4c) != 0)))) {
                      local_124 = (undefined1 *)0x0;
                      if (*(int *)(iVar15 + 0x100) != 0) {
                        local_e0[0] = *(undefined4 **)(iVar13 + 4);
                        FUN_00f22020();
                      }
                      uVar18 = 0;
                    }
                    local_d4 = 0;
                    local_84 = uVar18;
                    local_7c = local_124;
                    local_28 = (undefined4 *)local_124;
                    FUN_0114b070();
                    local_14._0_1_ = 0x12;
                    local_48 = 0;
                    local_e0[0] = (undefined4 *)
                                  ((*(int *)(*(int *)(param_1 + 0x94) + 0x20) - (int)puVar6) + -0xa6
                                  );
                    pppppsStack_184 = (short *****)local_cc;
                    puStack_188 = &local_48;
                    local_18c = (undefined4 *)0x11d7483;
                    FUN_0114b510();
                    local_14 = CONCAT31(local_14._1_3_,0x13);
                    FUN_01446f10();
                    local_e0[0] = (undefined4 *)*(undefined4 *)local_48;
                    if (*(char *)((int)local_e0[0] + 0xd) == '\0') {
                      local_110 = (undefined4)uVar18;
                      uStack_10c = (undefined4)((ulonglong)uVar18 >> 0x20);
                      while( true ) {
                        uStack_1b8 = local_110;
                        uStack_1b4 = uStack_10c;
                        puStack_1b0 = (undefined1 *)local_28;
                        local_28 = &uStack_1b8;
                        FUN_00f221a0(&local_78);
                        pcStack_194 = (char *)CONCAT31(pcStack_194._1_3_,local_60);
                        puStack_190 = (undefined4 *)local_5c;
                        local_18c = local_58;
                        puStack_188 = local_54;
                        pppppsStack_184 = (short *****)local_50;
                        FUN_01156e80(iVar8);
                        std::
                        _Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
                        ::operator++((_Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
                                      *)local_e0);
                        if (*(char *)((int)local_e0[0] + 0xd) != '\0') break;
                        local_28 = (undefined4 *)local_7c;
                        local_110 = (undefined4)local_84;
                        uStack_10c = local_84._4_4_;
                      }
                    }
                    FUN_010c3ea0();
                    FUN_017986fc();
                    FUN_0114b110();
                    local_58 = (int *)0x0;
                    FUN_00f1a330();
                    local_14 = 0xffffffff;
                    FUN_00f1a330();
                    puVar16 = (undefined4 *)FUN_016830c0();
                    if (puVar16 != (undefined4 *)0x0) {
                      *puVar16 = 0;
                      puVar16[1] = 0;
                      puVar16[2] = 0;
                      puVar16[3] = 0;
                      *(undefined8 *)(puVar16 + 4) = 0;
                      puVar16[6] = 0;
                      *puVar16 = &DAT_01ae673c;
                      puVar16[1] = 0;
                      puVar16[2] = 0;
                      puVar16[3] = 0xffffffff;
                      *(undefined2 *)(puVar16 + 4) = 0;
                      puVar16[5] = 0;
                      *(undefined2 *)(puVar16 + 6) = 1;
                      puVar11 = operator_new(0xc);
                      puVar11[1] = local_118;
                      *puVar11 = &DAT_01ae2fa0;
                      puVar11[2] = puVar5;
                      if ((undefined4 *)puVar16[2] != (undefined4 *)0x0) {
                        (*(code *)**(undefined4 **)puVar16[2])();
                      }
                      puVar16[2] = puVar11;
                      FUN_0122b3e0();
                      iVar15 = 0;
                      iVar8 = *(int *)(param_1 + 0x94);
                      for (puVar11 = (undefined4 *)**(undefined4 **)(iVar8 + 100);
                          puVar11 != *(undefined4 **)(iVar8 + 100); puVar11 = (undefined4 *)*puVar11
                          ) {
                        if ((undefined4 *)puVar11[2] == puVar16) {
                          if (iVar15 != -1) goto LAB_011d76c7;
                          break;
                        }
                        iVar15 = iVar15 + 1;
                      }
                      if (*(int *)(iVar8 + 0x78) < (int)puVar16[5]) {
                        *(undefined4 *)(iVar8 + 0x78) = puVar16[5];
                        piVar10 = (int *)**(int **)(iVar8 + 100);
                        if (piVar10 != *(int **)(iVar8 + 100)) {
                          do {
                            *(undefined4 *)(piVar10[2] + 0x14) = *(undefined4 *)(iVar8 + 0x78);
                            piVar10 = (int *)*piVar10;
                          } while (piVar10 != (int *)*(int *)(iVar8 + 100));
                        }
                      }
                      else {
                        puVar16[5] = *(int *)(iVar8 + 0x78);
                      }
                      puVar16[5] = puVar16[5];
                      FUN_00f55430();
                      FUN_0122c760();
                    }
                  }
                }
              }
LAB_011d76c7:
              puVar16 = (undefined4 *)((int)local_f0 + 1);
              local_f0 = puVar16;
            } while (puVar16 < *(undefined4 **)(*(int *)(param_1 + 0x84) + local_100 * 4));
          }
        }
        else {
          pvVar7 = (void *)FUN_016830c0();
          local_14 = 6;
          if (pvVar7 == (void *)0x0) {
            iVar8 = 0;
          }
          else {
            pppppsStack_184 = (short *****)0x11d6a14;
            memset(pvVar7,0,0x2c);
            iVar8 = FUN_010baa10();
          }
          local_14 = 0xffffffff;
          puVar16 = local_f0;
          if (iVar8 != 0) {
            pvVar7 = (void *)FUN_016830c0();
            local_14 = 7;
            if (pvVar7 == (void *)0x0) {
              puVar9 = (undefined8 *)0x0;
            }
            else {
              pppppsStack_184 = (short *****)0x11d6a64;
              memset(pvVar7,0,0xa0);
              puVar9 = (undefined8 *)FUN_0123de10();
            }
            local_14 = 0xffffffff;
            if (puVar9 != (undefined8 *)0x0) {
              FUN_00f19f10();
              local_30 = 0;
              uStack_3c = 0;
              uStack_38 = 0;
              uStack_34 = 0;
              uStack_2c = 0xf;
              local_40 = (short *****)0x0;
              pppppsStack_184 = (short *****)&local_40;
              local_14 = 8;
              puStack_188 = (undefined8 *)0x11d6ae0;
              FUN_015010a0();
              FUN_0124ef30();
              pppppsStack_184 = (short *****)0x0;
              puStack_188 = (undefined8 *)0x11d6b0b;
              puStack_188 = (undefined8 *)FUN_0124f150();
              local_18c = (undefined4 *)0xa;
              puStack_190 = (undefined4 *)0xc8;
              pcStack_194 = (char *)0xfffffffb;
              puStack_198 = (undefined4 *)0x0;
              iStack_19c = 0;
              uStack_1a0 = 0x11d6b20;
              FUN_0123e330();
              local_14 = 0xffffffff;
              FUN_00f1a420();
              pppppsStack_184 =
                   (short *****)(*(int *)(*(int *)(param_1 + 0x94) + 0x20) + -0xd8 >> 1);
              local_18c = (undefined4 *)0x11d6b64;
              puStack_188 = puVar9;
              piVar10 = (int *)FUN_01230f40();
              if (piVar10 != (int *)0x0) {
                (**(code **)(*piVar10 + 0x54))();
              }
            }
            puVar11 = (undefined4 *)FUN_016830c0();
            if (puVar11 != (undefined4 *)0x0) {
              *puVar11 = 0;
              puVar11[1] = 0;
              puVar11[2] = 0;
              puVar11[3] = 0;
              *(undefined8 *)(puVar11 + 4) = 0;
              puVar11[6] = 0;
              *puVar11 = &DAT_01ae673c;
              puVar11[1] = 0;
              puVar11[2] = 0;
              puVar11[3] = 0xffffffff;
              *(undefined2 *)(puVar11 + 4) = 0;
              puVar11[5] = 0;
              *(undefined2 *)(puVar11 + 6) = 1;
              FUN_0122b3e0();
              iVar15 = 0;
              iVar8 = *(int *)(param_1 + 0x94);
              puVar4 = *(undefined4 **)(iVar8 + 100);
              for (puVar12 = (undefined4 *)*puVar4; local_e0[0] = puVar11, puVar12 != puVar4;
                  puVar12 = (undefined4 *)*puVar12) {
                if ((undefined4 *)puVar12[2] == puVar11) {
                  if (iVar15 != -1) goto LAB_011d6c63;
                  break;
                }
                iVar15 = iVar15 + 1;
              }
              if (*(int *)(iVar8 + 0x78) < (int)puVar11[5]) {
                *(undefined4 *)(iVar8 + 0x78) = puVar11[5];
                puVar12 = (undefined4 *)*puVar4;
                if (puVar12 != puVar4) {
                  do {
                    *(undefined4 *)(puVar12[2] + 0x14) = *(undefined4 *)(iVar8 + 0x78);
                    puVar12 = (undefined4 *)*puVar12;
                  } while (puVar12 != *(undefined4 **)(iVar8 + 100));
                }
              }
              else {
                puVar11[5] = *(int *)(iVar8 + 0x78);
              }
              puVar11[5] = puVar11[5];
              FUN_00f55430();
              FUN_0122c760();
              goto LAB_011d6c63;
            }
          }
        }
        local_100 = local_100 + 1;
      } while (local_100 < (uint)(*(int *)(param_1 + 0x88) - *(int *)(param_1 + 0x84) >> 2));
    }
    pvVar7 = (void *)FUN_016830c0();
    local_14 = 0x14;
    if (pvVar7 == (void *)0x0) {
      local_e0[0] = (undefined4 *)0x0;
    }
    else {
      pppppsStack_184 = (short *****)0x11d7750;
      memset(pvVar7,0,0xec);
      local_e0[0] = (undefined4 *)FUN_01247d90();
    }
    local_14 = 0xffffffff;
    *(undefined4 **)(param_1 + 0x98) = local_e0[0];
    if (local_e0[0] != (undefined4 *)0x0) {
      iStack_1a8 = *(int *)(param_1 + 0x94);
      puStack_ac = DAT_01bee88c;
      local_a4 = *(int *)(iStack_1a8 + 0x20);
      puStack_190 = (undefined4 *)(*(int *)(iStack_1a8 + 0x24) + 0x14);
      uStack_a8 = DAT_01bee890;
      puStack_188 = (undefined8 *)0xf;
      local_18c = (undefined4 *)0x10;
      pcStack_194 = (char *)0x10;
      puStack_198 = (undefined4 *)0xffffffe3;
      iStack_19c = (local_a4 - (int)puVar6) + -0x10;
      uStack_1a0 = 0;
      uStack_1a4 = 0;
      uStack_1ac = 0;
      pppppsStack_184 = (short *****)DAT_01bee88c;
      puStack_1b0 = (undefined1 *)0x11d77ee;
      local_a0 = (undefined1 *)puStack_190;
      FUN_01248090();
      iVar8 = *(int *)(param_1 + 0x98);
      iVar15 = *(int *)(*(int *)(param_1 + 0x94) + 0x68);
      if ((*(int *)(iVar8 + 0x78) != 0) || (*(int *)(iVar8 + 0x7c) != iVar15)) {
        *(undefined4 *)(iVar8 + 0x78) = 0;
        *(int *)(iVar8 + 0x7c) = iVar15;
        FUN_0124a620();
      }
      FUN_012473e0();
      FUN_0122c520();
    }
  }
  ExceptionList = local_1c;
  return;
}


