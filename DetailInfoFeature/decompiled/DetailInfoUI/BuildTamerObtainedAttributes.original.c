
/* ============================================================ */
/* Function: FUN_011dbca0 */
/* Entry: 011dbca0 */
/* Signature: undefined FUN_011dbca0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011dbca0(int param_1)

{
  short sVar1;
  undefined4 *puVar2;
  uint uVar3;
  short *psVar4;
  int iVar5;
  int *piVar6;
  undefined4 *puVar7;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 uVar8;
  short *psVar9;
  int iVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined1 local_bc [24];
  undefined1 local_a4 [20];
  undefined4 local_90;
  undefined1 *local_84;
  undefined4 local_80;
  undefined1 *local_78;
  int local_70;
  int local_6c;
  uint local_68;
  short *local_64;
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
  puStack_18 = &LAB_019939cf;
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
  local_30 = 0xb;
  local_3c = 1;
  local_2c = 0;
  local_38 = 9;
  local_78 = &DAT_3f7ae148;
  local_58 = &DAT_3f7ae148;
  uVar8 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar15 = 0;
  local_6c = param_1;
  local_24 = uVar3;
  FUN_00f19cc0(s_DETAIL_INFO_DATA_01ae3424,0x10);
  psVar4 = (short *)FUN_015ce480(local_a4,uVar8,uVar11,uVar12,uVar13,uVar14,uVar15);
  local_14._0_1_ = 1;
  if (7 < *(uint *)(psVar4 + 10)) {
    psVar4 = *(short **)psVar4;
  }
  psVar9 = psVar4;
  do {
    sVar1 = *psVar9;
    psVar9 = psVar9 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar4,(int)psVar9 - (int)(psVar4 + 1) >> 1);
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330(uVar3);
  FUN_01246bb0(&local_60,1,0x163,0x7a);
  local_14 = 0xffffffff;
  local_34 = 0;
  FUN_00f1a330();
  uVar8 = extraout_ECX;
  if (*(int *)(param_1 + 0x84) == 0) {
    local_64 = (short *)FUN_016830c0(0x94);
    local_14 = 2;
    if (local_64 == (short *)0x0) {
      iVar5 = 0;
      uVar8 = extraout_ECX_00;
    }
    else {
      memset(local_64,0,0x94);
      iVar5 = FUN_0122b460();
      uVar8 = extraout_ECX_01;
    }
    local_14 = 0xffffffff;
    *(int *)(param_1 + 0x84) = iVar5;
    if (iVar5 == 0) {
      ExceptionList = local_1c;
      return;
    }
  }
  FUN_0122b6f0(*(undefined4 *)(param_1 + 0x40),0x102,0x84,0xc2,0x16e,0,uVar8);
  iVar5 = *(int *)(param_1 + 0x84);
  *(undefined4 *)(iVar5 + 0x84) = 0;
  *(undefined4 *)(iVar5 + 0x88) = 1;
  iVar5 = *(int *)(param_1 + 0x84);
  *(undefined4 *)(iVar5 + 0x7c) = 0;
  *(undefined4 *)(iVar5 + 0x80) = 1;
  iVar5 = *(int *)(param_1 + 0x84);
  local_80 = *(undefined4 *)(iVar5 + 0x20);
  *(int *)(iVar5 + 0x78) = *(int *)(iVar5 + 0x24) / 0xd + -1;
  FUN_012472c0(*(undefined4 *)(param_1 + 0x84));
  local_68 = 0;
  local_70 = *(int *)(*(int *)(param_1 + 0x84) + 0x78);
  do {
    local_78 = (undefined1 *)FUN_016830c0(0x2c);
    local_14 = 3;
    if (local_78 == (undefined1 *)0x0) {
      iVar5 = 0;
    }
    else {
      memset(local_78,0,0x2c);
      iVar5 = FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (iVar5 != 0) {
      local_44 = 0;
      local_40 = 7;
      uStack_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      local_54 = 0;
      local_28 = 0;
      local_14 = 4;
      local_34 = DAT_01bfe468 + 8;
      local_84 = &DAT_3f7ae148;
      local_58 = &DAT_3f7ae148;
      local_30 = 10;
      local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_3c = 1;
      local_2c = 0;
      local_38 = 4;
      psVar4 = (short *)FUN_011dd050(local_bc,local_68);
      local_14._0_1_ = 5;
      if (7 < *(uint *)(psVar4 + 10)) {
        psVar4 = *(short **)psVar4;
      }
      psVar9 = psVar4;
      do {
        sVar1 = *psVar9;
        psVar9 = psVar9 + 1;
      } while (sVar1 != 0);
      FUN_00f1a040(psVar4,(int)psVar9 - (int)(psVar4 + 1) >> 1);
      local_14 = CONCAT31(local_14._1_3_,4);
      FUN_00f1a330();
      iVar10 = local_70 >> 1;
      piVar6 = (int *)FUN_01230c40(&local_60,2,iVar10);
      if (piVar6 != (int *)0x0) {
        (**(code **)(*piVar6 + 0x54))(1);
        local_34 = 0;
        FUN_00f1a330();
        local_44 = 0;
        local_40 = 7;
        uStack_50 = 0;
        uStack_4c = 0;
        uStack_48 = 0;
        local_54 = 0;
        local_28 = 0;
        local_14 = 6;
        local_34 = DAT_01bfe468 + 8;
        local_90 = 0x3e40c0c1;
        local_58 = (undefined1 *)0x3e40c0c1;
        local_30 = 10;
        local_60 = CONCAT44(DAT_01b026d0,DAT_01b026f0);
        local_3c = 1;
        local_2c = 0;
        local_38 = 6;
        psVar4 = (short *)FUN_011dd1d0(local_bc,local_68);
        local_14._0_1_ = 7;
        if (7 < *(uint *)(psVar4 + 10)) {
          psVar4 = *(short **)psVar4;
        }
        local_64 = psVar4 + 1;
        psVar9 = psVar4;
        do {
          sVar1 = *psVar9;
          psVar9 = psVar9 + 1;
        } while (sVar1 != 0);
        FUN_00f1a040(psVar4,(int)psVar9 - (int)local_64 >> 1);
        local_14 = CONCAT31(local_14._1_3_,6);
        FUN_00f1a330();
        piVar6 = (int *)FUN_01230c40(&local_60,0xbd,iVar10);
        if (piVar6 != (int *)0x0) {
          (**(code **)(*piVar6 + 0x54))(1);
          local_14 = 0xffffffff;
          local_34 = 0;
          FUN_00f1a330();
          psVar4 = (short *)FUN_016830c0(0x1c);
          local_64 = psVar4;
          if (psVar4 != (short *)0x0) {
            psVar4[0] = 0;
            psVar4[1] = 0;
            psVar4[2] = 0;
            psVar4[3] = 0;
            psVar4[4] = 0;
            psVar4[5] = 0;
            psVar4[6] = 0;
            psVar4[7] = 0;
            psVar4[8] = 0;
            psVar4[9] = 0;
            psVar4[10] = 0;
            psVar4[0xb] = 0;
            psVar4[0xc] = 0;
            psVar4[0xd] = 0;
            *(undefined4 **)psVar4 = &DAT_01ae673c;
            psVar4[2] = 0;
            psVar4[3] = 0;
            psVar4[4] = 0;
            psVar4[5] = 0;
            psVar4[6] = -1;
            psVar4[7] = -1;
            psVar4[8] = 0;
            psVar4[10] = 0;
            psVar4[0xb] = 0;
            psVar4[0xc] = 1;
            FUN_0122b3e0(iVar5);
            iVar10 = 0;
            iVar5 = *(int *)(local_6c + 0x84);
            puVar2 = *(undefined4 **)(iVar5 + 100);
            for (puVar7 = (undefined4 *)*puVar2; puVar7 != puVar2; puVar7 = (undefined4 *)*puVar7) {
              if ((short *)puVar7[2] == psVar4) {
                local_64 = psVar4;
                if (iVar10 != -1) goto LAB_011dc1d9;
                break;
              }
              iVar10 = iVar10 + 1;
            }
            if (*(int *)(iVar5 + 0x78) < *(int *)(psVar4 + 10)) {
              *(int *)(iVar5 + 0x78) = *(int *)(psVar4 + 10);
              puVar7 = (undefined4 *)*puVar2;
              if (puVar7 != puVar2) {
                do {
                  *(undefined4 *)(puVar7[2] + 0x14) = *(undefined4 *)(iVar5 + 0x78);
                  puVar7 = (undefined4 *)*puVar7;
                } while (puVar7 != *(undefined4 **)(iVar5 + 100));
              }
            }
            else {
              *(int *)(psVar4 + 10) = *(int *)(iVar5 + 0x78);
            }
            local_64 = psVar4;
            FUN_00f55430(&local_64);
            FUN_0122c760();
          }
          goto LAB_011dc1d9;
        }
      }
      local_14 = 0xffffffff;
      local_34 = 0;
      FUN_00f1a330();
    }
LAB_011dc1d9:
    local_68 = local_68 + 1;
    if (0xc < local_68) {
      ExceptionList = local_1c;
      return;
    }
  } while( true );
}


