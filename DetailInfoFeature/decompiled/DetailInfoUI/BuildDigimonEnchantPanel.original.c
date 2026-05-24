
/* ============================================================ */
/* Function: FUN_011d4320 */
/* Entry: 011d4320 */
/* Signature: undefined FUN_011d4320() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_011d4320(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  uint uVar3;
  short *psVar4;
  int iVar5;
  void *pvVar6;
  int iVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  int *piVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  short *******pppppppsVar13;
  undefined4 uVar14;
  undefined8 uVar15;
  undefined4 uStack_9c;
  undefined4 local_90;
  uint local_7c;
  undefined8 local_78;
  undefined1 *local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined1 local_54;
  undefined4 local_50;
  int local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  short *******local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  int local_14;
  
  uVar9 = DAT_01b02744;
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_0199326e;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_5c = 0;
  uStack_68 = 0;
  uStack_64 = 0;
  uStack_60 = 0;
  local_58 = 7;
  local_6c = 0;
  local_40 = 0;
  local_14 = 0;
  local_4c = DAT_01bfe468 + 8;
  local_78 = CONCAT44(DAT_01b02744,DAT_01b02744);
  local_48 = 10;
  local_54 = 1;
  local_44 = 0;
  local_50 = 5;
  local_70 = &DAT_3f7ae148;
  local_24 = uVar3;
  FUN_00f19cc0(s_ENCHANTTACTICS_TITLE_DIGIMON_ENC_01ae2c18,0x24);
  psVar4 = (short *)FUN_015ce480();
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
  FUN_01246bb0(&local_78,1,0x84,0x1a8);
  local_4c = 0;
  FUN_00f1a330();
  local_5c = 0;
  local_58 = 7;
  uStack_68 = 0;
  uStack_64 = 0;
  uStack_60 = 0;
  local_6c = 0;
  local_40 = 0;
  local_4c = DAT_01bfe468 + 8;
  local_78 = CONCAT44(uVar9,uVar9);
  local_70 = &DAT_3f7ae148;
  local_48 = 9;
  local_54 = 1;
  local_44 = 0;
  local_50 = 6;
  local_14._0_1_ = 3;
  local_14._1_3_ = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_2c = 0;
  local_28 = 7;
  local_3c = (short *******)0x0;
  FUN_01500f80(&local_3c,u__d__d_01ace96c,*(undefined2 *)(*(int *)(param_1 + 0x78) + 0xb4),0x4b);
  pppppppsVar13 = (short *******)&local_3c;
  if (7 < local_28) {
    pppppppsVar13 = local_3c;
  }
  do {
    sVar1 = *(short *)pppppppsVar13;
    pppppppsVar13 = (short *******)((int)pppppppsVar13 + 2);
  } while (sVar1 != 0);
  FUN_00f1a040();
  local_14._0_1_ = 2;
  FUN_00f1a330();
  FUN_01246bb0(&local_78,1,0xf3,0x1a8);
  iVar5 = FUN_01250310();
  pvVar6 = (void *)FUN_016830c0(0xa0);
  local_14._0_1_ = 4;
  if (pvVar6 == (void *)0x0) {
    iVar7 = 0;
  }
  else {
    memset(pvVar6,0,0xa0);
    iVar7 = FUN_0123de10();
  }
  uVar9 = DAT_01bb9b34;
  uVar15 = DAT_01bb9b2c;
  local_14._0_1_ = 2;
  if (iVar7 == 0) {
    local_4c = iVar7;
    FUN_00f1a330();
  }
  else {
    uVar2 = *(undefined4 *)(param_1 + 0x40);
    FUN_00f19f10();
    local_2c = 0;
    uStack_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    local_28 = 0xf;
    local_3c = (short *******)0x0;
    local_14._0_1_ = 5;
    FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_DigimonStatus_New_Digimon_Status_01ae2c80);
    FUN_0124ef30(&local_3c);
    uVar14 = 0;
    uVar8 = FUN_0124f150(0,uVar15,uVar9);
    FUN_0123e330(uVar2,0xd3 - iVar5,0x198,0x20,0x20,uVar8,uVar14,uVar15,uVar9);
    local_14 = CONCAT31(local_14._1_3_,2);
    FUN_00f1a420();
    FUN_012472c0(iVar7);
    local_14 = 0xffffffff;
    local_4c = 0;
    FUN_00f1a330();
    pvVar6 = (void *)FUN_016830c0(0xa0);
    local_14 = 6;
    if (pvVar6 == (void *)0x0) {
      iVar5 = 0;
    }
    else {
      memset(pvVar6,0,0xa0);
      iVar5 = FUN_0123de10();
    }
    uVar9 = DAT_01bb9b34;
    uVar15 = DAT_01bb9b2c;
    local_14 = 0xffffffff;
    if (iVar5 != 0) {
      uVar2 = *(undefined4 *)(param_1 + 0x40);
      FUN_00f19f10();
      local_2c = 0;
      uStack_38 = 0;
      uStack_34 = 0;
      uStack_30 = 0;
      local_28 = 0xf;
      local_3c = (short *******)0x0;
      local_14 = 7;
      FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_TacticsHouse_CheckOnline_tga_01ae2c60);
      FUN_0124ef30(&local_3c);
      uVar14 = 0;
      uVar8 = FUN_0124f150(0,uVar15,uVar9);
      FUN_0123e330(uVar2,0x12,0x1b8,0xe1,0x77,uVar8,uVar14,uVar15,uVar9);
      local_14 = 0xffffffff;
      FUN_00f1a420();
      FUN_012472c0(iVar5);
      uVar9 = extraout_ECX;
      if (*(int *)(param_1 + 0x84) == 0) {
        pvVar6 = (void *)FUN_016830c0(0xac);
        local_14 = 8;
        if (pvVar6 == (void *)0x0) {
          iVar5 = 0;
          uVar9 = extraout_ECX_00;
        }
        else {
          memset(pvVar6,0,0xac);
          iVar5 = FUN_01228950();
          uVar9 = extraout_ECX_01;
        }
        local_14 = 0xffffffff;
        *(int *)(param_1 + 0x84) = iVar5;
        if (iVar5 == 0) {
          ExceptionList = local_1c;
          return;
        }
      }
      FUN_01228bd0(*(undefined4 *)(param_1 + 0x40),0x12,0x1b8,0xe1,0x77,DAT_01bee88c,DAT_01bee890,
                   0x70,0x27,0,2,uVar9,2);
      FUN_012472c0(*(undefined4 *)(param_1 + 0x84));
      FUN_0122a180();
      local_7c = 0;
      do {
        pvVar6 = (void *)FUN_016830c0(0x2c);
        local_14 = 9;
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
        local_14 = 10;
        if (pvVar6 == (void *)0x0) {
          uVar9 = 0;
        }
        else {
          memset(pvVar6,0,0xa0);
          uVar9 = FUN_0123de10();
        }
        uVar2 = DAT_01bb9b34;
        uVar15 = DAT_01bb9b2c;
        local_14 = 0xffffffff;
        switch(local_7c) {
        case 0:
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
        }
        local_2c = 0;
        local_28 = 0;
        local_3c = (short *******)0x0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_30 = 0;
        FUN_00f19cc0();
        local_14 = 0xb;
        pppppppsVar13 = (short *******)&local_3c;
        if (0xf < local_28) {
          pppppppsVar13 = local_3c;
        }
        FUN_0123e0d0(0,pppppppsVar13,0,uVar15,uVar2,1,0x19,0xb,0x3f,0x10);
        local_14 = 0xffffffff;
        FUN_00f1a420();
        piVar10 = (int *)FUN_01230f40(uVar9,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        if (piVar10 != (int *)0x0) {
          (**(code **)(*piVar10 + 0x54))(1);
        }
        FUN_01012bc0();
        iVar7 = *(int *)(param_1 + 0x78);
        FUN_00f489b0();
        if (((DAT_0000000d == '\0') && (bRam00000010 <= (byte)local_7c)) &&
           (*(int *)(iVar7 + 0xc0) != 0)) {
          uStack_9c = _DAT_00000014;
          local_90 = _DAT_00000018;
        }
        else {
          uStack_9c = 0;
          local_90 = 0;
        }
        piVar10 = (int *)FUN_01231410();
        if (piVar10 != (int *)0x0) {
          (**(code **)(*piVar10 + 0x54))(1);
        }
        puVar11 = (undefined4 *)FUN_016830c0(0x28);
        if (puVar11 != (undefined4 *)0x0) {
          *puVar11 = 0;
          puVar11[1] = 0;
          puVar11[2] = 0;
          puVar11[3] = 0;
          puVar11[4] = 0;
          puVar11[5] = 0;
          puVar11[6] = 0;
          puVar11[7] = 0;
          *(undefined8 *)(puVar11 + 8) = 0;
          *puVar11 = &DAT_01ae66e0;
          puVar11[1] = 0;
          puVar11[2] = 0;
          puVar11[3] = local_7c;
          *(undefined1 *)(puVar11 + 4) = 0;
          puVar11[5] = 0x70;
          puVar11[6] = 0x27;
          *(undefined2 *)(puVar11 + 7) = 0x101;
          puVar11[8] = 0;
          puVar11[9] = 0;
          FUN_012288c0(iVar5);
          puVar12 = operator_new(0xc);
          puVar12[1] = uStack_9c;
          *puVar12 = &DAT_01ae2e04;
          puVar12[2] = local_90;
          if ((undefined4 *)puVar11[2] != (undefined4 *)0x0) {
            (*(code *)**(undefined4 **)puVar11[2])(1);
          }
          puVar11[2] = puVar12;
          FUN_0122a090(puVar11);
        }
        local_7c = local_7c + 1;
      } while (local_7c < 5);
    }
  }
  ExceptionList = local_1c;
  return;
}


