
/* ============================================================ */
/* Function: FUN_011d4bd0 */
/* Entry: 011d4bd0 */
/* Signature: undefined FUN_011d4bd0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011d4bd0(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined1 uVar4;
  char cVar5;
  uint uVar6;
  short *psVar7;
  int iVar8;
  void *pvVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int *piVar12;
  undefined4 *puVar13;
  undefined4 *puVar14;
  short *psVar15;
  int iVar16;
  int extraout_ECX;
  int extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  int iVar17;
  undefined4 ***pppuVar18;
  undefined4 *puVar19;
  void *pvVar20;
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
  int local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 **local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  uint local_34;
  undefined8 local_30;
  undefined1 *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  int local_14;
  
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_01993301;
  local_1c = ExceptionList;
  uVar6 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_68 = 0;
  local_64 = 7;
  uStack_74 = 0;
  uStack_70 = 0;
  uStack_6c = 0;
  local_78 = 0;
  local_4c = 0;
  local_14 = 0;
  local_58 = DAT_01bfe468 + 8;
  local_28 = &DAT_3f7ae148;
  local_84 = CONCAT44(DAT_01b02744,DAT_01b02744);
  local_54 = 10;
  local_7c = &DAT_3f7ae148;
  local_60 = 1;
  local_50 = 0;
  local_5c = 5;
  local_24 = uVar6;
  FUN_00f19cc0(s_Breakthrough_Title_01ad2a7c,0x12);
  psVar7 = (short *)FUN_015ce480();
  local_14._0_1_ = 1;
  if (7 < *(uint *)(psVar7 + 10)) {
    psVar7 = *(short **)psVar7;
  }
  psVar15 = psVar7;
  do {
    sVar1 = *psVar15;
    psVar15 = psVar15 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar7,(int)psVar15 - (int)(psVar7 + 1) >> 1);
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330(uVar6);
  FUN_01246bb0(&local_84,1,0x163,0x1a8);
  local_14 = 0xffffffff;
  local_58 = 0;
  FUN_00f1a330();
  iVar16 = *(int *)(param_1 + 0x78);
  iVar8 = 0;
  local_28 = (void *)0x0;
  local_30 = 0;
  uVar6 = *(int *)(iVar16 + 0xec) - *(int *)(iVar16 + 0xe8) >> 4;
  iVar17 = 0;
  if (uVar6 != 0) {
    if (0xfffffff < uVar6) {
                    /* WARNING: Subroutine does not return */
      FUN_00f19c70();
    }
    uVar6 = uVar6 * 0x10;
    if (uVar6 == 0) {
      pvVar20 = (void *)0x0;
    }
    else if (uVar6 < 0x1000) {
      pvVar20 = operator_new(uVar6);
    }
    else {
      if (uVar6 + 0x23 <= uVar6) {
                    /* WARNING: Subroutine does not return */
        FUN_00f1a5c0();
      }
      pvVar9 = operator_new(uVar6 + 0x23);
      if (pvVar9 == (void *)0x0) {
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
      pvVar20 = (void *)((int)pvVar9 + 0x23U & 0xffffffe0);
      *(void **)((int)pvVar20 - 4) = pvVar9;
    }
    local_28 = (undefined1 *)(uVar6 + (int)pvVar20);
    local_30 = CONCAT44(pvVar20,pvVar20);
    pvVar9 = *(void **)(iVar16 + 0xe8);
    uVar6 = *(int *)(iVar16 + 0xec) - (int)pvVar9;
    memcpy(pvVar20,pvVar9,uVar6);
    iVar8 = (uVar6 & 0xfffffff0) + (int)pvVar20;
    local_30 = CONCAT44(iVar8,(undefined4 *)local_30);
    iVar17 = (int)(undefined4 *)local_30;
  }
  local_14._0_1_ = 2;
  local_14._1_3_ = 0;
  iVar16 = (int)(((iVar8 - iVar17 >> 4) + 1U >> 1) * -0x25 + 0x7c) >> 1;
  iVar8 = iVar16;
  if (*(int *)(param_1 + 0x88) == 0) {
    pvVar20 = (void *)FUN_016830c0(0xac);
    local_14._0_1_ = 3;
    if (pvVar20 == (void *)0x0) {
      uVar10 = 0;
      iVar8 = extraout_ECX;
    }
    else {
      memset(pvVar20,0,0xac);
      uVar10 = FUN_01228950();
      iVar8 = extraout_ECX_00;
    }
    *(undefined4 *)(param_1 + 0x88) = uVar10;
  }
  local_14._0_1_ = 2;
  FUN_01228c50(*(undefined4 *)(param_1 + 0x40),0x112,0x1b8,0xa2,0x77,0x1e,5,0x20,0x20,0,2,
               s_TacticsHouse_CheckOnline_tga_01ae2c60,iVar8,2);
  iVar8 = *(int *)(param_1 + 0x88);
  *(int *)(iVar8 + 0xa4) = iVar16;
  *(undefined4 *)(iVar8 + 0xa0) = 0x22;
  FUN_012472c0(*(undefined4 *)(param_1 + 0x88));
  FUN_0122a180();
  puVar3 = local_30._4_4_;
  uVar4 = (undefined1)local_14;
  if ((undefined4 *)local_30 != local_30._4_4_) {
    puVar19 = (undefined4 *)local_30 + 3;
    do {
      pvVar20 = (void *)FUN_016830c0(0x2c);
      local_14._0_1_ = 4;
      if (pvVar20 == (void *)0x0) {
        iVar8 = 0;
      }
      else {
        memset(pvVar20,0,0x2c);
        iVar8 = FUN_010baa10();
      }
      local_14._0_1_ = 2;
      uVar4 = (undefined1)local_14;
      local_14._0_1_ = 2;
      if (iVar8 == 0) break;
      pvVar20 = (void *)FUN_016830c0(0xa0);
      local_14._0_1_ = 5;
      if (pvVar20 == (void *)0x0) {
        uVar11 = 0;
        uVar10 = extraout_ECX_01;
      }
      else {
        memset(pvVar20,0,0xa0);
        uVar11 = FUN_0123de10();
        uVar10 = extraout_ECX_02;
      }
      local_38 = 0;
      uStack_44 = 0;
      uStack_40 = 0;
      uStack_3c = 0;
      local_34 = 0xf;
      local_48 = (undefined4 ***)0x0;
      local_14 = CONCAT31(local_14._1_3_,6);
      cVar5 = FUN_00f20ef0(uVar10);
      if (cVar5 != '\0') {
        pppuVar18 = &local_48;
        if (0xf < local_34) {
          pppuVar18 = (undefined4 ***)local_48;
        }
        FUN_0123e0d0(0,pppuVar18,0,DAT_01bb9b2c,DAT_01bb9b34,1,DAT_01bee88c,DAT_01bee890,0x20,0x20);
        piVar12 = (int *)FUN_01230f40(uVar11,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        if (piVar12 != (int *)0x0) {
          (**(code **)(*piVar12 + 0x54))(1);
        }
        puVar13 = (undefined4 *)FUN_016830c0(0x28);
        if (puVar13 != (undefined4 *)0x0) {
          *puVar13 = 0;
          puVar13[1] = 0;
          puVar13[2] = 0;
          puVar13[3] = 0;
          puVar13[4] = 0;
          puVar13[5] = 0;
          puVar13[6] = 0;
          puVar13[7] = 0;
          *(undefined8 *)(puVar13 + 8) = 0;
          uVar10 = *(undefined4 *)(*(int *)(param_1 + 0x88) + 0x68);
          *puVar13 = &DAT_01ae66e0;
          puVar13[1] = 0;
          puVar13[2] = 0;
          puVar13[3] = uVar10;
          *(undefined1 *)(puVar13 + 4) = 0;
          puVar13[5] = 0x20;
          puVar13[6] = 0x20;
          *(undefined2 *)(puVar13 + 7) = 0x101;
          puVar13[8] = 0;
          puVar13[9] = 0;
          FUN_012288c0(iVar8);
          puVar14 = operator_new(0x10);
          uVar10 = puVar19[-2];
          uVar11 = *puVar19;
          uVar2 = puVar19[-1];
          *puVar14 = &DAT_01ae2df4;
          puVar14[1] = uVar2;
          puVar14[2] = uVar11;
          puVar14[3] = uVar10;
          if ((undefined4 *)puVar13[2] != (undefined4 *)0x0) {
            (*(code *)**(undefined4 **)puVar13[2])(1);
          }
          puVar13[2] = puVar14;
          FUN_0122a090(puVar13);
        }
      }
      local_14._0_1_ = 2;
      FUN_00f1a420();
      puVar13 = puVar19 + 1;
      puVar19 = puVar19 + 4;
      uVar4 = (undefined1)local_14;
    } while (puVar13 != puVar3);
  }
  local_14._0_1_ = uVar4;
  FUN_00f30f30();
  ExceptionList = local_1c;
  return;
}


