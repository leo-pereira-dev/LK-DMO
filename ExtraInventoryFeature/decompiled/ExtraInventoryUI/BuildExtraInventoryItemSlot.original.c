
/* ============================================================ */
/* Function: FUN_011a36a0 */
/* Entry: 011a36a0 */
/* Signature: undefined FUN_011a36a0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 __thiscall FUN_011a36a0(undefined4 *param_1,int param_2,short *param_3)

{
  short sVar1;
  undefined4 uVar2;
  void *pvVar3;
  undefined1 uVar4;
  uint uVar5;
  int *piVar6;
  undefined4 uVar7;
  short *psVar8;
  undefined4 *puVar9;
  undefined4 ******ppppppuVar10;
  undefined8 *puVar11;
  char cVar12;
  undefined4 uVar13;
  undefined8 uVar14;
  undefined4 uVar15;
  undefined8 local_a8;
  undefined8 local_a0;
  short *local_98;
  undefined4 *local_8c;
  int local_88;
  int local_84;
  undefined4 *local_80;
  void *local_7c;
  undefined8 local_78;
  undefined1 *local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  uint uStack_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined1 local_54;
  undefined4 local_50;
  int local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 *****local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198fc84;
  local_1c = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_88 = param_2;
  local_8c = param_1;
  local_24 = uVar5;
  local_80 = (undefined4 *)FUN_016830c0(0x2c,uVar5);
  local_14 = 0;
  if (local_80 == (void *)0x0) {
    local_84 = 0;
  }
  else {
    memset(local_80,0,0x2c);
    local_84 = FUN_010baa10();
  }
  local_14 = 0xffffffff;
  if (local_84 != 0) {
    local_98 = (short *)(local_88 + 4);
    if ((*(int *)(local_88 + 4) == 0) || (*(short *)(local_88 + 8) == 0)) {
      uVar4 = 0;
    }
    else {
      uVar4 = 1;
    }
    local_80 = (undefined4 *)CONCAT31(local_80._1_3_,uVar4);
    local_7c = (void *)FUN_016830c0(0xa0,uVar5);
    local_14 = 1;
    if (local_7c == (void *)0x0) {
      local_7c = (void *)0x0;
    }
    else {
      memset(local_7c,0,0xa0);
      local_7c = (void *)FUN_0123de10();
    }
    local_14 = 0xffffffff;
    if (local_7c != (void *)0x0) {
      local_2c = 0;
      local_28 = 0;
      local_3c = (undefined4 ******)0x0;
      uStack_38 = 0;
      uStack_34 = 0;
      uStack_30 = 0;
      FUN_00f19cc0();
      local_14 = 2;
      ppppppuVar10 = &local_3c;
      if (0xf < local_28) {
        ppppppuVar10 = (undefined4 ******)local_3c;
      }
      FUN_0123e0d0(0,ppppppuVar10,0,DAT_01bb9b2c,DAT_01bb9b34,1,DAT_01bee88c,DAT_01bee890,0x20,0x20)
      ;
      piVar6 = (int *)FUN_01230f40(local_7c,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      *param_1 = piVar6;
      if (piVar6 != (int *)0x0) {
        (**(code **)(*piVar6 + 0x54))(1);
      }
      local_14 = 0xffffffff;
      FUN_00f1a420();
    }
    local_7c = (void *)FUN_016830c0(0xa0);
    local_14 = 3;
    if (local_7c == (void *)0x0) {
      local_7c = (void *)0x0;
    }
    else {
      memset(local_7c,0,0xa0);
      local_7c = (void *)FUN_0123de10();
    }
    local_14 = 0xffffffff;
    if (local_7c != (void *)0x0) {
      local_a8 = (ulonglong)(uint)local_a8;
      FUN_0123a800(&local_78,local_98,(int)&local_a8 + 4);
      uVar2 = DAT_01bee890;
      uVar15 = DAT_01bb9b34;
      local_14 = 4;
      local_98 = DAT_01bee88c;
      local_a8 = DAT_01bb9b2c;
      FUN_00f19f10();
      local_2c = 0;
      uStack_38 = 0;
      uStack_34 = 0;
      uStack_30 = 0;
      local_28 = 0xf;
      local_3c = (undefined4 ******)0x0;
      local_14._0_1_ = 5;
      FUN_00f19f10();
      FUN_0124ef30(&local_3c);
      pvVar3 = local_7c;
      uVar13 = 0;
      uVar14 = local_a8;
      uVar7 = FUN_0124f150(0,local_a8,uVar15);
      FUN_0123e330(0,local_98,uVar2,0x1e,0x1e,uVar7,uVar13,uVar14,uVar15);
      local_14 = CONCAT31(local_14._1_3_,4);
      FUN_00f1a420();
      piVar6 = (int *)FUN_01230f40(pvVar3,1,1,DAT_01bee88c,DAT_01bee890);
      param_1 = local_8c;
      local_8c[1] = piVar6;
      (**(code **)(*piVar6 + 0x54))(1);
      puVar11 = &local_78;
      if (0xf < uStack_64) {
        puVar11 = (undefined8 *)local_78;
      }
      if (*(int *)(param_1[1] + 0x28) != 0) {
        FUN_0123fcd0(puVar11,&uStack_60,1);
      }
      (**(code **)(*(int *)param_1[1] + 0x40))();
      FUN_00f1a420();
    }
    local_5c = 0;
    local_58 = 7;
    uStack_68 = 0;
    uStack_64 = 0;
    uStack_60 = 0;
    local_6c = 0;
    local_40 = 0;
    local_14 = 6;
    local_4c = DAT_01bfe468 + 8;
    local_98 = (short *)&DAT_3f7ae148;
    local_70 = &DAT_3f7ae148;
    local_78 = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_48 = 8;
    local_54 = 1;
    local_44 = 0;
    local_50 = 2;
    local_a8 = (ulonglong)(int)*(short *)(local_88 + 8);
    psVar8 = (short *)FUN_015027f0();
    local_14._0_1_ = 7;
    if (7 < *(uint *)(psVar8 + 10)) {
      psVar8 = *(short **)psVar8;
    }
    local_98 = psVar8 + 1;
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040();
    local_14 = CONCAT31(local_14._1_3_,6);
    FUN_00f1a330();
    piVar6 = (int *)FUN_01230c40(&local_78,0x20,0x14);
    param_1[2] = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x54))(1);
      (**(code **)(*(int *)param_1[2] + 0x40))();
    }
    local_14 = 0xffffffff;
    local_4c = 0;
    FUN_00f1a330();
    local_80 = (undefined4 *)FUN_016830c0(0xa0);
    local_14 = 8;
    if (local_80 == (void *)0x0) {
      local_7c = (void *)0x0;
    }
    else {
      memset(local_80,0,0xa0);
      local_7c = (void *)FUN_0123de10();
    }
    uVar2 = DAT_01bee890;
    uVar15 = DAT_01bb9b34;
    local_14 = 0xffffffff;
    if (local_7c != (void *)0x0) {
      local_80 = DAT_01bee88c;
      local_a0 = DAT_01bb9b2c;
      FUN_00f19f10();
      local_2c = 0;
      uStack_38 = 0;
      uStack_34 = 0;
      uStack_30 = 0;
      local_28 = 0xf;
      local_3c = (undefined4 ******)0x0;
      local_14 = 9;
      FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_Control_G_Popup_N2Dlg5_tga_01ae0f00);
      FUN_0124ef30(&local_3c);
      pvVar3 = local_7c;
      uVar13 = 0;
      uVar14 = local_a0;
      uVar7 = FUN_0124f150(0,local_a0,uVar15);
      FUN_0123e330(0,local_80,uVar2,0x20,0x20,uVar7,uVar13,uVar14,uVar15);
      local_14 = 0xffffffff;
      FUN_00f1a420();
      piVar6 = (int *)FUN_01230f40(pvVar3,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      param_1 = local_8c;
      local_8c[3] = piVar6;
      if (piVar6 != (int *)0x0) {
        (**(code **)(*piVar6 + 0x54))(1);
        cVar12 = *(char *)(local_88 + 0x24);
        if (cVar12 == '\0') {
          cVar12 = *(char *)(local_88 + 0x25) == '\0';
        }
        local_7c = (void *)CONCAT31(local_7c._1_3_,cVar12);
        (**(code **)(*(int *)param_1[3] + 0x40))();
      }
    }
    local_80 = (undefined4 *)FUN_016830c0(0xa0);
    local_14 = 10;
    if (local_80 == (void *)0x0) {
      local_7c = (void *)0x0;
    }
    else {
      memset(local_80,0,0xa0);
      local_7c = (void *)FUN_0123de10();
    }
    uVar2 = DAT_01bee890;
    uVar15 = DAT_01bb9b34;
    local_14 = 0xffffffff;
    if (local_7c != (void *)0x0) {
      local_80 = DAT_01bee88c;
      local_a0 = DAT_01bb9b2c;
      FUN_00f19f10();
      local_2c = 0;
      uStack_38 = 0;
      uStack_34 = 0;
      uStack_30 = 0;
      local_28 = 0xf;
      local_3c = (undefined4 ******)0x0;
      local_14 = 0xb;
      FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_NewInventory_new_inventory_lock__01ae0f3c
                  );
      FUN_0124ef30(&local_3c);
      pvVar3 = local_7c;
      uVar13 = 0;
      uVar14 = local_a0;
      uVar7 = FUN_0124f150(0,local_a0,uVar15);
      FUN_0123e330(0,local_80,uVar2,0x18,0x22,uVar7,uVar13,uVar14,uVar15);
      local_14 = 0xffffffff;
      FUN_00f1a420();
      piVar6 = (int *)FUN_01230f40(pvVar3,4,0xffffffff,DAT_01bee88c,DAT_01bee890);
      param_1 = local_8c;
      local_8c[4] = piVar6;
      if (piVar6 != (int *)0x0) {
        (**(code **)(*piVar6 + 0x54))(1);
        (**(code **)(*(int *)param_1[4] + 0x40))();
      }
    }
    puVar9 = (undefined4 *)FUN_016830c0(0x28);
    local_80 = puVar9;
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
      sVar1 = *param_3;
      *puVar9 = &DAT_01ae66e0;
      puVar9[1] = 0;
      puVar9[2] = 0;
      puVar9[3] = (int)sVar1;
      *(undefined1 *)(puVar9 + 4) = 0;
      puVar9[5] = 0x20;
      puVar9[6] = 0x20;
      *(undefined2 *)(puVar9 + 7) = 0x101;
      puVar9[8] = 0;
      puVar9[9] = 0;
      param_1[5] = puVar9;
      local_80 = operator_new(0xc);
      *local_80 = &DAT_01ae0f94;
      local_80[1] = *(undefined4 *)(local_88 + 4);
      *(undefined2 *)(local_80 + 2) = *(undefined2 *)(local_88 + 8);
      *(short *)((int)local_80 + 10) = *param_3;
      if ((undefined4 *)puVar9[2] != (undefined4 *)0x0) {
        (*(code *)**(undefined4 **)puVar9[2])(1);
      }
      puVar9[2] = local_80;
      FUN_012288c0(local_84);
      ExceptionList = local_1c;
      return 1;
    }
    param_1[5] = 0;
    FUN_011133d0();
    FUN_01683120();
  }
  ExceptionList = local_1c;
  return 0;
}


