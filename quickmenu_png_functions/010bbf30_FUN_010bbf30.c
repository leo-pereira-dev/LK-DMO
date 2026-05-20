
/* ============================================================ */
/* Function: FUN_010bbf30 */
/* Entry: 010bbf30 */
/* Signature: undefined FUN_010bbf30() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_010bbf30(int param_1,undefined4 *param_2,int param_3)

{
  short sVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  uint uVar5;
  int iVar6;
  undefined4 *puVar7;
  int *piVar8;
  undefined4 uVar9;
  undefined4 *puVar10;
  short *psVar11;
  undefined4 uVar12;
  undefined8 uVar13;
  undefined4 uVar14;
  int local_88;
  undefined4 *local_80 [2];
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
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0197fb78;
  local_1c = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar5;
  local_80[0] = (undefined4 *)FUN_016830c0(0x2c,uVar5);
  local_14 = 0;
  if (local_80[0] == (undefined4 *)0x0) {
    iVar6 = 0;
  }
  else {
    memset(local_80[0],0,0x2c);
    iVar6 = FUN_010baa10();
  }
  local_14 = 0xffffffff;
  if (iVar6 != 0) {
    puVar7 = (undefined4 *)FUN_016830c0(0xbc,uVar5);
    local_14 = 1;
    local_80[0] = puVar7;
    if (puVar7 == (undefined4 *)0x0) {
      puVar7 = (undefined4 *)0x0;
    }
    else {
      memset(puVar7,0,0xbc);
      FUN_0123de10();
      *puVar7 = &DAT_01ae721c;
      *(undefined2 *)(puVar7 + 0x28) = 0;
      *(undefined1 *)((int)puVar7 + 0xa2) = 0;
      puVar7[0x29] = 1;
      puVar7[0x2a] = 1;
      puVar7[0x2b] = 0xffffffff;
      puVar7[0x2c] = 0x3f800000;
      puVar7[0x2d] = 0x3f800000;
      puVar7[0x2e] = 0;
    }
    local_14 = 0xffffffff;
    if (puVar7 != (undefined4 *)0x0) {
      FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x1a,0x1a,s_QuickMenu_QuickMenu_Sub_Icon_png_01ad36c4
                   ,0,1);
      FUN_0123d9a0();
      piVar8 = (int *)FUN_01231700(puVar7,*(undefined4 *)(param_3 + 8),4,param_2[1] + -0x1a >> 1,
                                   DAT_01bee88c,DAT_01bee890);
      if (piVar8 != (int *)0x0) {
        (**(code **)(*piVar8 + 0x54))(1);
      }
    }
    local_5c = 0;
    local_58 = 7;
    uStack_68 = 0;
    uStack_64 = 0;
    uStack_60 = 0;
    local_6c = 0;
    local_40 = 0;
    local_14._0_1_ = 2;
    local_14._1_3_ = 0;
    psVar11 = (short *)(param_3 + 0xc);
    local_4c = DAT_01bfe468 + 8;
    local_78 = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_48 = 9;
    local_70 = &DAT_3f7ae148;
    local_54 = 1;
    local_44 = 0;
    local_50 = 4;
    if (7 < *(uint *)(param_3 + 0x20)) {
      psVar11 = *(short **)psVar11;
    }
    do {
      sVar1 = *psVar11;
      psVar11 = psVar11 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040();
    piVar8 = (int *)FUN_01230c40(&local_78,0x28,(int)param_2[1] >> 1);
    if (piVar8 != (int *)0x0) {
      (**(code **)(*piVar8 + 0x54))(1);
    }
    uVar4 = (undefined1)local_14;
    if (*(char *)(param_3 + 0x24) == '\x01') {
      local_80[0] = (undefined4 *)FUN_016830c0(0xa0);
      local_14._0_1_ = 3;
      if (local_80[0] == (undefined4 *)0x0) {
        local_88 = 0;
      }
      else {
        memset(local_80[0],0,0xa0);
        local_88 = FUN_0123de10();
      }
      uVar3 = DAT_01bee890;
      uVar14 = DAT_01bb9b34;
      uVar13 = DAT_01bb9b2c;
      local_14._0_1_ = 2;
      uVar4 = (undefined1)local_14;
      local_14._0_1_ = 2;
      if (local_88 != 0) {
        local_80[0] = DAT_01bee88c;
        FUN_00f19f10();
        local_2c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_30 = 0;
        local_28 = 0xf;
        local_3c = 0;
        local_14._0_1_ = 4;
        FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,
                     s_NewInventory_attention_notificat_01ad3584);
        FUN_0124ef30(&local_3c);
        uVar12 = 0;
        uVar9 = FUN_0124f150(0,uVar13,uVar14);
        FUN_0123e330(0,local_80[0],uVar3,0x10,0x10,uVar9,uVar12,uVar13,uVar14);
        local_14._0_1_ = 2;
        FUN_00f1a420();
        piVar8 = (int *)FUN_01230f40(local_88,0xfffffffc,(param_2[1] + -0x1a >> 1) + -2,DAT_01bee88c
                                     ,DAT_01bee890);
        uVar4 = (undefined1)local_14;
        if (piVar8 != (int *)0x0) {
          (**(code **)(*piVar8 + 0x54))(1);
          uVar4 = (undefined1)local_14;
        }
      }
    }
    local_14._0_1_ = uVar4;
    puVar7 = (undefined4 *)FUN_016830c0(0x1c);
    local_80[0] = puVar7;
    if (puVar7 != (undefined4 *)0x0) {
      *puVar7 = 0;
      puVar7[1] = 0;
      puVar7[2] = 0;
      puVar7[3] = 0;
      *(undefined8 *)(puVar7 + 4) = 0;
      puVar7[6] = 0;
      uVar14 = *(undefined4 *)(param_1 + 0x68);
      *puVar7 = &DAT_01ae673c;
      puVar7[1] = 0;
      puVar7[2] = 0;
      puVar7[3] = uVar14;
      *(undefined2 *)(puVar7 + 4) = 0;
      puVar7[5] = 0;
      *(undefined2 *)(puVar7 + 6) = 1;
      uVar14 = *param_2;
      *(undefined4 *)(iVar6 + 0x10) = param_2[1];
      *(undefined4 *)(iVar6 + 0xc) = uVar14;
      FUN_0122b3e0(iVar6);
      local_80[0] = operator_new(8);
      *local_80[0] = &DAT_01ad3708;
      local_80[0][1] = *(undefined4 *)(param_3 + 4);
      if ((undefined4 *)puVar7[2] != (undefined4 *)0x0) {
        (*(code *)**(undefined4 **)puVar7[2])(1);
      }
      puVar7[2] = local_80[0];
      iVar6 = 0;
      puVar2 = *(undefined4 **)(param_1 + 100);
      for (puVar10 = (undefined4 *)*puVar2; puVar10 != puVar2; puVar10 = (undefined4 *)*puVar10) {
        if ((undefined4 *)puVar10[2] == puVar7) {
          local_80[0] = puVar7;
          if (iVar6 != -1) goto LAB_010bc3c9;
          break;
        }
        iVar6 = iVar6 + 1;
      }
      if (*(int *)(param_1 + 0x78) < (int)puVar7[5]) {
        *(undefined4 *)(param_1 + 0x78) = puVar7[5];
        puVar10 = (undefined4 *)*puVar2;
        if (puVar10 != puVar2) {
          do {
            *(undefined4 *)(puVar10[2] + 0x14) = *(undefined4 *)(param_1 + 0x78);
            puVar10 = (undefined4 *)*puVar10;
          } while (puVar10 != *(undefined4 **)(param_1 + 100));
        }
      }
      else {
        puVar7[5] = *(int *)(param_1 + 0x78);
      }
      local_80[0] = puVar7;
      FUN_00f55430(local_80);
      FUN_0122c760();
    }
LAB_010bc3c9:
    local_4c = 0;
    FUN_00f1a330();
  }
  ExceptionList = local_1c;
  return;
}


