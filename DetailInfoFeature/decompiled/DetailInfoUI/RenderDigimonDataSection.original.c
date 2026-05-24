
/* ============================================================ */
/* Function: FUN_011d3690 */
/* Entry: 011d3690 */
/* Signature: undefined FUN_011d3690() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011d3690(int param_1)

{
  short sVar1;
  undefined4 *puVar2;
  uint uVar3;
  short *psVar4;
  int iVar5;
  int *piVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 uVar9;
  short *psVar10;
  int iVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined1 local_b0 [24];
  undefined1 local_98 [20];
  undefined4 local_84;
  undefined4 local_80;
  undefined1 *local_78;
  int local_70;
  int local_6c;
  uint local_68;
  undefined4 *local_64;
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
  int *local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  int local_14;
  
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_019930ec;
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
  local_34 = (int *)(DAT_01bfe468 + 8);
  local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
  local_30 = 0xb;
  local_3c = 1;
  local_2c = 0;
  local_38 = 9;
  local_78 = &DAT_3f7ae148;
  local_58 = &DAT_3f7ae148;
  uVar9 = 0;
  uVar12 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar15 = 0;
  uVar16 = 0;
  local_6c = param_1;
  local_24 = uVar3;
  FUN_00f19cc0(s_DETAIL_INFO_DIGIDATA_01ae2be0,0x14);
  psVar4 = (short *)FUN_015ce480(local_98,uVar9,uVar12,uVar13,uVar14,uVar15,uVar16);
  local_14._0_1_ = 1;
  if (7 < *(uint *)(psVar4 + 10)) {
    psVar4 = *(short **)psVar4;
  }
  psVar10 = psVar4;
  do {
    sVar1 = *psVar10;
    psVar10 = psVar10 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar4,(int)psVar10 - (int)(psVar4 + 1) >> 1);
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330(uVar3);
  FUN_01246bb0(&local_60,1,0x163,0x54);
  local_14 = 0xffffffff;
  local_34 = (int *)0x0;
  FUN_00f1a330();
  uVar9 = extraout_ECX;
  if (*(int *)(param_1 + 0x7c) == 0) {
    local_64 = (undefined4 *)FUN_016830c0(0x94);
    local_14 = 2;
    if (local_64 == (undefined4 *)0x0) {
      iVar5 = 0;
      uVar9 = extraout_ECX_00;
    }
    else {
      memset(local_64,0,0x94);
      iVar5 = FUN_0122b460();
      uVar9 = extraout_ECX_01;
    }
    local_14 = 0xffffffff;
    *(int *)(param_1 + 0x7c) = iVar5;
    if (iVar5 == 0) {
      ExceptionList = local_1c;
      return;
    }
  }
  FUN_0122b6f0(*(undefined4 *)(param_1 + 0x40),0x112,0x58,0xa2,0x135,0,uVar9);
  iVar5 = *(int *)(param_1 + 0x7c);
  *(undefined4 *)(iVar5 + 0x84) = 0;
  *(undefined4 *)(iVar5 + 0x88) = 1;
  iVar5 = *(int *)(param_1 + 0x7c);
  *(undefined4 *)(iVar5 + 0x7c) = 0;
  *(undefined4 *)(iVar5 + 0x80) = 1;
  iVar5 = *(int *)(param_1 + 0x7c);
  local_80 = *(undefined4 *)(iVar5 + 0x20);
  *(int *)(iVar5 + 0x78) = *(int *)(iVar5 + 0x24) / 0xb + -1;
  FUN_012472c0(*(undefined4 *)(param_1 + 0x7c));
  local_68 = 0;
  local_70 = *(int *)(*(int *)(param_1 + 0x7c) + 0x78);
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
      local_34 = (int *)(DAT_01bfe468 + 8);
      local_84 = 0x3e40c0c1;
      local_58 = (undefined1 *)0x3e40c0c1;
      local_30 = 10;
      local_60 = CONCAT44(DAT_01b026d0,DAT_01b026f0);
      local_3c = 1;
      local_2c = 0;
      local_38 = 6;
      psVar4 = (short *)FUN_011d5940(local_b0,local_68);
      local_14._0_1_ = 5;
      if (7 < *(uint *)(psVar4 + 10)) {
        psVar4 = *(short **)psVar4;
      }
      psVar10 = psVar4;
      do {
        sVar1 = *psVar10;
        psVar10 = psVar10 + 1;
      } while (sVar1 != 0);
      FUN_00f1a040(psVar4,(int)psVar10 - (int)(psVar4 + 1) >> 1);
      local_14 = CONCAT31(local_14._1_3_,4);
      FUN_00f1a330();
      piVar6 = (int *)FUN_01230c40(&local_60,0x9b,local_70 >> 1);
      if (piVar6 == (int *)0x0) {
        local_14 = 0xffffffff;
        local_34 = piVar6;
        FUN_00f1a330();
      }
      else {
        (**(code **)(*piVar6 + 0x54))(1);
        local_14 = 0xffffffff;
        local_34 = (int *)0x0;
        FUN_00f1a330();
        puVar7 = (undefined4 *)FUN_016830c0(0x1c);
        local_64 = puVar7;
        if (puVar7 != (undefined4 *)0x0) {
          *puVar7 = 0;
          puVar7[1] = 0;
          puVar7[2] = 0;
          puVar7[3] = 0;
          *(undefined8 *)(puVar7 + 4) = 0;
          puVar7[6] = 0;
          *puVar7 = &DAT_01ae673c;
          puVar7[1] = 0;
          puVar7[2] = 0;
          puVar7[3] = 0xffffffff;
          *(undefined2 *)(puVar7 + 4) = 0;
          puVar7[5] = 0;
          *(undefined2 *)(puVar7 + 6) = 1;
          FUN_0122b3e0(iVar5);
          iVar11 = 0;
          iVar5 = *(int *)(local_6c + 0x7c);
          puVar2 = *(undefined4 **)(iVar5 + 100);
          for (puVar8 = (undefined4 *)*puVar2; puVar8 != puVar2; puVar8 = (undefined4 *)*puVar8) {
            if ((undefined4 *)puVar8[2] == puVar7) {
              local_64 = puVar7;
              if (iVar11 != -1) goto LAB_011d3ac9;
              break;
            }
            iVar11 = iVar11 + 1;
          }
          if (*(int *)(iVar5 + 0x78) < (int)puVar7[5]) {
            *(undefined4 *)(iVar5 + 0x78) = puVar7[5];
            puVar8 = (undefined4 *)*puVar2;
            if (puVar8 != puVar2) {
              do {
                *(undefined4 *)(puVar8[2] + 0x14) = *(undefined4 *)(iVar5 + 0x78);
                puVar8 = (undefined4 *)*puVar8;
              } while (puVar8 != *(undefined4 **)(iVar5 + 100));
            }
          }
          else {
            puVar7[5] = *(int *)(iVar5 + 0x78);
          }
          local_64 = puVar7;
          FUN_00f55430(&local_64);
          FUN_0122c760();
        }
      }
    }
LAB_011d3ac9:
    local_68 = local_68 + 1;
    if (8 < local_68) {
      ExceptionList = local_1c;
      return;
    }
  } while( true );
}


