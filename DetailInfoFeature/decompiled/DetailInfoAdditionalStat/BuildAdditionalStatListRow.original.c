
/* ============================================================ */
/* Function: FUN_011d0390 */
/* Entry: 011d0390 */
/* Signature: undefined FUN_011d0390() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011d0390(int param_1,int param_2,int *param_3)

{
  short sVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  int *piVar8;
  undefined4 *puVar9;
  short *psVar10;
  int *piVar11;
  undefined4 *****pppppuVar12;
  undefined4 extraout_ECX;
  undefined4 uVar13;
  short *psVar14;
  undefined4 extraout_ECX_00;
  int iVar15;
  undefined8 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  int local_100 [6];
  undefined1 local_e8 [24];
  undefined4 ****local_d0;
  int iStack_cc;
  int iStack_c8;
  undefined1 *puStack_c4;
  undefined4 ****local_bc;
  int iStack_b8;
  int iStack_b4;
  int iStack_b0;
  undefined8 local_ac;
  int *local_a4;
  undefined8 local_a0;
  int local_94;
  int local_90;
  int local_8c;
  char local_85;
  undefined4 *local_84;
  undefined8 local_80;
  undefined1 *local_78;
  undefined4 local_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined1 local_5c;
  undefined4 local_58;
  int local_54;
  int local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 ****local_44;
  int iStack_40;
  int iStack_3c;
  int iStack_38;
  undefined8 local_34;
  int *local_2c;
  int *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01992d5d;
  local_1c = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_a0 = (ulonglong)(uint)local_a0;
  iVar6 = *(int *)(param_1 + 0xa8);
  local_94 = param_2;
  local_a4 = param_3;
  local_2c = param_3;
  local_28 = param_3;
  local_8c = param_1;
  local_24 = uVar5;
  FUN_00f1d720(&iStack_cc,&local_28);
  if (((*(char *)((int)puStack_c4 + 0xd) == '\0') &&
      (*(int *)((int)puStack_c4 + 0x10) <= (int)param_3)) &&
     (puStack_c4 != (undefined1 *)*(int *)(iVar6 + 0x28))) {
    iVar6 = FUN_01022810(&local_2c);
    local_85 = *(char *)(iVar6 + 4);
  }
  else {
    local_85 = '\0';
  }
  local_84 = (undefined4 *)FUN_016830c0(0x2c,uVar5);
  local_14 = 0;
  if (local_84 == (undefined4 *)0x0) {
    local_28 = (int *)0x0;
  }
  else {
    memset(local_84,0,0x2c);
    local_28 = (int *)FUN_010baa10();
  }
  local_14 = 0xffffffff;
  local_84 = (undefined4 *)FUN_016830c0(0x1c0,uVar5);
  local_14 = 1;
  if (local_84 == (undefined4 *)0x0) {
    iVar6 = 0;
  }
  else {
    memset(local_84,0,0x1c0);
    iVar6 = FUN_01222670();
  }
  local_14 = 0xffffffff;
  local_84 = (undefined4 *)FUN_016830c0(0xa0);
  local_14 = 2;
  if (local_84 == (undefined4 *)0x0) {
    local_2c = (int *)0x0;
  }
  else {
    memset(local_84,0,0xa0);
    local_2c = (int *)FUN_0123de10();
  }
  local_14 = 0xffffffff;
  local_84 = (undefined4 *)FUN_016830c0(0x1c0);
  local_14 = 3;
  if (local_84 == (undefined4 *)0x0) {
    local_90 = 0;
  }
  else {
    memset(local_84,0,0x1c0);
    local_90 = FUN_01222670();
  }
  iStack_40 = 0;
  iStack_3c = 0;
  iStack_38 = 0;
  local_34._0_4_ = 0;
  local_34._4_4_ = 0xf;
  local_44 = (undefined4 *****)0x0;
  pcVar7 = s_DMG_SKINS_skin_btn_a_tga_01ae28d0;
  local_14 = 4;
  if (local_85 != '\0') {
    pcVar7 = s_DMG_SKINS_skin_btn_b_tga_01ae2898;
  }
  FUN_00f19f10(pcVar7,0x18);
  pppppuVar12 = &local_44;
  if (0xf < local_34._4_4_) {
    pppppuVar12 = (undefined4 *****)local_44;
  }
  FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0x106,0x2d,pppppuVar12,0);
  FUN_01223e30(0,0x2d);
  local_84 = operator_new(0x10);
  piVar11 = local_a4;
  uVar13 = *(undefined4 *)(*(int *)(local_8c + 0xb0) + 0x68);
  local_84[1] = local_94;
  *local_84 = &DAT_01ae2948;
  local_84[2] = uVar13;
  local_84[3] = local_a4;
  uVar13 = 0;
  if (*(undefined4 **)(iVar6 + 0x34) != (undefined4 *)0x0) {
    (**(code **)**(undefined4 **)(iVar6 + 0x34))(1);
    uVar13 = extraout_ECX;
  }
  *(undefined4 **)(iVar6 + 0x34) = local_84;
  FUN_011d0e20(uVar13,local_8c,&LAB_011cfe30,0);
  piVar4 = local_28;
  piVar8 = (int *)FUN_012317d0();
  (**(code **)(*piVar8 + 0x54))(1);
  if (local_94 == 0) {
    if (*(int *)(*(int *)(local_8c + 0xa8) + 0x50) == 0) {
      local_100[4] = 0;
      local_100[5] = 0;
      local_100[0] = 0;
      local_100[1] = 0;
      local_100[2] = 0;
      local_100[3] = 0;
      FUN_00f19cc0(&DAT_01abd9ec,0);
      piVar8 = local_100;
      puVar9 = (undefined4 *)0x2;
    }
    else {
      piVar8 = (int *)FUN_016188a0(local_e8,piVar11);
      puVar9 = (undefined4 *)0x1;
    }
    local_d0 = (undefined4 *****)*piVar8;
    iStack_cc = piVar8[1];
    iStack_c8 = piVar8[2];
    puStack_c4 = (undefined1 *)piVar8[3];
    local_bc = (undefined4 *****)*piVar8;
    iStack_b8 = piVar8[1];
    iStack_b4 = piVar8[2];
    iStack_b0 = piVar8[3];
    local_ac = *(undefined8 *)(piVar8 + 4);
    local_a0 = local_ac;
    piVar8[4] = 0;
    piVar8[5] = 0xf;
    *(undefined1 *)piVar8 = 0;
    if (((uint)puVar9 & 2) != 0) {
      local_84 = (undefined4 *)((uint)puVar9 & 0xfffffffd | 4);
      FUN_00f1a420();
      puVar9 = local_84;
    }
    local_14 = 4;
    if (((uint)puVar9 & 1) != 0) {
      FUN_00f1a420();
    }
  }
  else {
    local_ac = 0;
    local_bc = (undefined4 *****)0x0;
    iStack_b8 = 0;
    iStack_b4 = 0;
    iStack_b0 = 0;
    FUN_00f19cc0(&DAT_01abd9ec,0);
    local_a0 = local_ac;
    local_d0 = local_bc;
    iStack_cc = iStack_b8;
    iStack_c8 = iStack_b4;
    puStack_c4 = (undefined1 *)iStack_b0;
  }
  FUN_00f1a420();
  local_ac = 0xf00000000;
  local_44 = local_d0;
  iStack_40 = iStack_cc;
  iStack_3c = iStack_c8;
  iStack_38 = (int)puStack_c4;
  local_bc = (undefined4 ****)((uint)local_bc & 0xffffff00);
  local_34 = local_a0;
  FUN_00f1a420();
  FUN_00f264c0(s_Number_dds_01ae28c4,10);
  pppppuVar12 = &local_44;
  if (0xf < local_34._4_4_) {
    pppppuVar12 = (undefined4 *****)local_44;
  }
  FUN_0123e0d0(0,pppppuVar12,0,DAT_01bb9b2c,DAT_01bb9b34,1,0xb,0xc,0x2d,0x14);
  if (local_2c[0x14] != 0) {
    FUN_016a9950(0,0,0x3e2aaaab,0,0x3f2aaaab,0x3f000000);
  }
  local_28 = (int *)FUN_01230f40(local_2c,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
  (**(code **)(*local_28 + 0x54))(1);
  puVar9 = *(undefined4 **)(local_8c + 0xb8);
  if (puVar9 == *(undefined4 **)(local_8c + 0xbc)) {
    FUN_011d0fb0(puVar9,&local_28);
  }
  else {
    *puVar9 = local_28;
    *(int *)(local_8c + 0xb8) = *(int *)(local_8c + 0xb8) + 4;
  }
  local_64 = 0;
  local_60 = 7;
  uStack_70 = 0;
  uStack_6c = 0;
  uStack_68 = 0;
  local_74 = 0;
  local_48 = 0;
  local_14._0_1_ = 6;
  local_54 = DAT_01bfe468 + 8;
  local_78 = (undefined1 *)DAT_01bb9b34;
  local_50 = 9;
  local_80 = DAT_01bb9b2c;
  local_5c = 1;
  local_4c = 0;
  local_58 = 0;
  psVar10 = (short *)FUN_010225e0(local_e8,local_94,piVar11);
  local_14._0_1_ = 7;
  if (7 < *(uint *)(psVar10 + 10)) {
    psVar10 = *(short **)psVar10;
  }
  psVar14 = psVar10;
  do {
    sVar1 = *psVar14;
    psVar14 = psVar14 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar10,(int)psVar14 - (int)(psVar10 + 1) >> 1);
  local_14._0_1_ = 6;
  FUN_00f1a330();
  piVar11 = (int *)FUN_01230c40(&local_80,0x44,-2 - (int)((float)(0x2d - local_50) * DAT_01b02af0));
  (**(code **)(*piVar11 + 0x54))(1);
  local_54 = DAT_01bfe468 + 8;
  puStack_c4 = &DAT_3f7ae148;
  local_80 = CONCAT44(DAT_01b02744,DAT_01b02744);
  local_50 = 9;
  local_78 = &DAT_3f7ae148;
  local_5c = 1;
  local_4c = 0;
  local_58 = 1;
  if (local_85 == '\0') {
    FUN_00f19f10(s_DMG_SKINS_Mini_Out_btn_a_tga_01ad83b4,0x1c);
    uVar16 = 0;
    uVar13 = 0;
    uVar17 = 0;
    uVar18 = 0;
    uVar19 = 0;
    FUN_00f19cc0(s_DMG_SKINS_APPLY_01ae2920,0xf);
    psVar10 = (short *)FUN_015ce480(local_e8,uVar16,uVar13,uVar17,uVar18,uVar19);
    local_14._0_1_ = 9;
    psVar14 = psVar10;
    if (7 < *(uint *)(psVar10 + 10)) {
      psVar10 = *(short **)psVar10;
      psVar14 = psVar10;
    }
    do {
      sVar1 = *psVar10;
      psVar10 = psVar10 + 1;
    } while (sVar1 != 0);
  }
  else {
    FUN_00f19f10(s_DMG_SKINS_Mini_Out_btn_b_tga_01ae2900,0x1c);
    uVar16 = 0;
    uVar13 = 0;
    uVar17 = 0;
    uVar18 = 0;
    uVar19 = 0;
    FUN_00f19cc0(s_DMG_SKINS_APPLY_OFF_01ae28ec,0x13);
    psVar10 = (short *)FUN_015ce480(local_e8,uVar16,uVar13,uVar17,uVar18,uVar19);
    local_14._0_1_ = 8;
    psVar14 = psVar10;
    if (7 < *(uint *)(psVar10 + 10)) {
      psVar10 = *(short **)psVar10;
      psVar14 = psVar10;
    }
    do {
      sVar1 = *psVar10;
      psVar10 = psVar10 + 1;
    } while (sVar1 != 0);
  }
  FUN_00f1a040(psVar14,(int)psVar10 - (int)(psVar14 + 1) >> 1);
  local_14 = CONCAT31(local_14._1_3_,6);
  FUN_00f1a330();
  pppppuVar12 = &local_44;
  if (0xf < local_34._4_4_) {
    pppppuVar12 = (undefined4 *****)local_44;
  }
  FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0x46,0x1e,pppppuVar12,0);
  FUN_01223e30(0,0x24);
  puVar9 = operator_new(8);
  local_a0 = CONCAT44(puVar9,(uint)local_a0);
  *puVar9 = &DAT_01ae2940;
  puVar9[1] = local_a4;
  if (*(undefined4 **)(local_90 + 0x34) != (undefined4 *)0x0) {
    (**(code **)**(undefined4 **)(local_90 + 0x34))(1);
  }
  iVar15 = local_90;
  *(undefined4 **)(local_90 + 0x34) = puVar9;
  FUN_01224f40(&local_80);
  FUN_011d0e20(extraout_ECX_00,local_8c,FUN_011cfd50,0);
  iVar6 = *(int *)(iVar15 + 200);
  piVar11 = (int *)FUN_012317d0(iVar15,0,0xbb,7,0x46,0x1e,*(undefined4 *)(iVar6 + 0x24),
                                *(undefined4 *)(iVar6 + 0x28));
  (**(code **)(*piVar11 + 0x54))(1);
  puVar9 = (undefined4 *)FUN_016830c0(0x1c);
  local_a0 = CONCAT44(puVar9,(uint)local_a0);
  if (puVar9 == (undefined4 *)0x0) {
    FUN_0122b3e0(piVar4);
  }
  else {
    *puVar9 = 0;
    puVar9[1] = 0;
    puVar9[2] = 0;
    puVar9[3] = 0;
    *(undefined8 *)(puVar9 + 4) = 0;
    puVar9[6] = 0;
    *puVar9 = &DAT_01ae673c;
    puVar9[1] = 0;
    puVar9[2] = 0;
    puVar9[3] = 0xffffffff;
    *(undefined2 *)(puVar9 + 4) = 0;
    puVar9[5] = 0;
    *(undefined2 *)(puVar9 + 6) = 1;
    FUN_0122b3e0(piVar4);
    iVar6 = *(int *)(local_8c + 0xb0);
    local_84 = puVar9;
    if (puVar9 != (undefined4 *)0x0) {
      puVar2 = *(undefined4 **)(iVar6 + 100);
      iVar15 = 0;
      for (puVar3 = (undefined4 *)*puVar2; puVar3 != puVar2; puVar3 = (undefined4 *)*puVar3) {
        if ((undefined4 *)puVar3[2] == puVar9) {
          if (iVar15 != -1) goto LAB_011d0c32;
          break;
        }
        iVar15 = iVar15 + 1;
      }
      if (*(int *)(iVar6 + 0x78) < (int)puVar9[5]) {
        *(undefined4 *)(iVar6 + 0x78) = puVar9[5];
        puVar9 = (undefined4 *)*puVar2;
        if (puVar9 != puVar2) {
          do {
            *(undefined4 *)(puVar9[2] + 0x14) = *(undefined4 *)(iVar6 + 0x78);
            puVar9 = (undefined4 *)*puVar9;
          } while (puVar9 != *(undefined4 **)(iVar6 + 100));
        }
      }
      else {
        puVar9[5] = *(int *)(iVar6 + 0x78);
      }
      FUN_00f55430(&local_84);
      FUN_0122c760();
    }
  }
LAB_011d0c32:
  local_54 = 0;
  FUN_00f1a330();
  FUN_00f1a420();
  ExceptionList = local_1c;
  return;
}


