
/* ============================================================ */
/* Function: FUN_011bbb20 */
/* Entry: 011bbb20 */
/* Signature: undefined FUN_011bbb20() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011bbb20(int param_1,undefined4 *param_2)

{
  short sVar1;
  undefined1 uVar2;
  void *_Dst;
  undefined4 *puVar3;
  undefined4 uVar4;
  short *psVar5;
  int iVar6;
  undefined8 *puVar7;
  int *piVar8;
  undefined4 *puVar9;
  int iVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined8 uVar14;
  undefined8 local_e8;
  undefined1 *local_e0;
  int local_d4;
  undefined4 *local_d0;
  int local_c4;
  int *local_c0;
  int *piStack_bc;
  int *piStack_b8;
  int *piStack_b4;
  int *local_b0;
  int *local_ac;
  int *local_a8;
  undefined4 *local_a4;
  undefined4 local_a0;
  undefined4 *local_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_6c;
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
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019915b4;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if (*(int *)(param_1 + 0x84) != 0) {
    _Dst = (void *)FUN_016830c0(0x2c,local_24);
    local_14 = 0;
    if (_Dst == (void *)0x0) {
      local_d4 = 0;
    }
    else {
      memset(_Dst,0,0x2c);
      local_d4 = FUN_010baa10();
    }
    iVar10 = local_d4;
    local_14 = 0xffffffff;
    if (local_d4 != 0) {
      local_b0 = (int *)0x0;
      local_c0 = (int *)0x0;
      piStack_bc = (int *)0x0;
      piStack_b8 = (int *)0x0;
      piStack_b4 = (int *)0x0;
      local_ac = (int *)0x0;
      local_a8 = (int *)0x0;
      local_a0 = 0;
      puVar3 = operator_new(0xc);
      local_9c = (undefined4 *)0x0;
      *puVar3 = puVar3;
      puVar3[1] = puVar3;
      local_14 = 1;
      local_a4 = puVar3;
      local_d0 = (undefined4 *)FUN_016830c0();
      local_14._0_1_ = 2;
      if (local_d0 == (void *)0x0) {
        local_c4 = 0;
      }
      else {
        memset(local_d0,0,0xa0);
        local_c4 = FUN_0123de10();
      }
      uVar12 = DAT_01bee890;
      uVar11 = DAT_01bb9b34;
      local_14._0_1_ = 1;
      uVar2 = (undefined1)local_14;
      local_14._0_1_ = 1;
      if (local_c4 != 0) {
        local_d0 = DAT_01bee88c;
        local_e8 = DAT_01bb9b2c;
        FUN_00f19f10();
        local_70 = 0;
        uStack_7c = 0;
        uStack_78 = 0;
        uStack_74 = 0;
        local_6c = 0xf;
        local_80 = 0;
        local_14._0_1_ = 3;
        FUN_015010a0(&local_80,&DAT_01ae7290,&DAT_01bfbd40,s_Random_box_bg_big_png_01adae60);
        FUN_0124ef30(&local_80);
        iVar6 = local_c4;
        uVar13 = 0;
        uVar14 = local_e8;
        uVar4 = FUN_0124f150(0,local_e8,uVar11);
        FUN_0123e330(0,local_d0,uVar12,0xc6,0x14d,uVar4,uVar13,uVar14,uVar11);
        local_14._0_1_ = 1;
        FUN_00f1a420();
        iVar10 = local_d4;
        local_c0 = (int *)FUN_01230f40(iVar6,0x1a,0x21,DAT_01bee88c,DAT_01bee890);
        uVar2 = (undefined1)local_14;
        if (local_c0 != (int *)0x0) {
          (**(code **)(*local_c0 + 0x54))(1);
          uVar2 = (undefined1)local_14;
        }
      }
      local_14._0_1_ = uVar2;
      local_d0 = (undefined4 *)FUN_016830c0(0xa0);
      local_14._0_1_ = 4;
      if (local_d0 == (void *)0x0) {
        local_c4 = 0;
      }
      else {
        memset(local_d0,0,0xa0);
        local_c4 = FUN_0123de10();
      }
      uVar12 = DAT_01bee890;
      uVar11 = DAT_01bb9b34;
      local_14._0_1_ = 1;
      if (local_c4 != 0) {
        local_d0 = DAT_01bee88c;
        local_e8 = DAT_01bb9b2c;
        FUN_00f19f10();
        local_70 = 0;
        uStack_7c = 0;
        uStack_78 = 0;
        uStack_74 = 0;
        local_6c = 0xf;
        local_80 = 0;
        local_14._0_1_ = 5;
        FUN_015010a0(&local_80,&DAT_01ae7290,&DAT_01bfbd40,&DAT_01abd9ec);
        FUN_0124ef30(&local_80);
        iVar6 = local_c4;
        uVar13 = 0;
        uVar14 = local_e8;
        uVar4 = FUN_0124f150(0,local_e8,uVar11);
        FUN_0123e330(0,local_d0,uVar12,0xc6,0xb4,uVar4,uVar13,uVar14,uVar11);
        local_14._0_1_ = 1;
        FUN_00f1a420();
        iVar10 = local_d4;
        piStack_bc = (int *)FUN_01230f40(iVar6,0x1a,0x5f,DAT_01bee88c,DAT_01bee890);
        if (piStack_bc != (int *)0x0) {
          (**(code **)(*piStack_bc + 0x54))(1);
        }
      }
      local_44 = 0;
      local_40 = 7;
      uStack_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      local_54 = 0;
      local_28 = 0;
      local_14._0_1_ = 6;
      local_34 = DAT_01bfe468 + 8;
      local_e0 = &DAT_3f7ae148;
      local_58 = &DAT_3f7ae148;
      local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_80 = DAT_01b02744;
      uStack_7c = 0;
      uStack_78 = 0;
      uStack_74 = 0;
      local_30 = 10;
      local_3c = 1;
      local_2c = 0;
      local_38 = 5;
      piStack_b8 = (int *)FUN_01230c40(&local_60,0x7d,0x41);
      local_14._0_1_ = 1;
      local_34 = 0;
      FUN_00f1a330();
      local_d0 = (undefined4 *)FUN_016830c0(0xa0);
      local_14._0_1_ = 7;
      if (local_d0 == (void *)0x0) {
        local_c4 = 0;
      }
      else {
        memset(local_d0,0,0xa0);
        local_c4 = FUN_0123de10();
      }
      uVar12 = DAT_01bee890;
      uVar11 = DAT_01bb9b34;
      local_14._0_1_ = 1;
      if (local_c4 != 0) {
        local_d0 = DAT_01bee88c;
        local_e8 = DAT_01bb9b2c;
        FUN_00f19f10();
        local_88 = 0;
        uStack_94 = 0;
        uStack_90 = 0;
        uStack_8c = 0;
        local_84 = 0xf;
        local_98 = 0;
        local_14._0_1_ = 8;
        FUN_015010a0(&local_98,&DAT_01ae7290,&DAT_01bfbd40,s_Random_box_loding_png_01adaebc);
        FUN_0124ef30(&local_98);
        iVar6 = local_c4;
        uVar13 = 0;
        uVar14 = local_e8;
        uVar4 = FUN_0124f150(0,local_e8,uVar11);
        FUN_0123e330(0,local_d0,uVar12,0x40,0x40,uVar4,uVar13,uVar14,uVar11);
        local_14._0_1_ = 1;
        FUN_00f1a420();
        iVar10 = local_d4;
        piStack_b4 = (int *)FUN_01230f40(iVar6,0x5d,0xa8,DAT_01bee88c,DAT_01bee890);
        if (piStack_b4 != (int *)0x0) {
          (**(code **)(*piStack_b4 + 0x54))(1);
        }
      }
      local_44 = 0;
      local_40 = 7;
      uStack_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      local_54 = 0;
      local_28 = 0;
      local_14._0_1_ = 9;
      local_34 = DAT_01bfe468 + 8;
      local_e0 = (undefined1 *)0x3f000000;
      local_58 = (undefined1 *)0x3f000000;
      local_60 = CONCAT44(DAT_01b02620,DAT_01b02620);
      local_30 = 0xb;
      local_3c = 1;
      local_2c = 0;
      local_38 = 1;
      local_b0 = (int *)FUN_01230c40(&local_60,0x7d,0xf2);
      local_34 = 0;
      FUN_00f1a330();
      local_44 = 0;
      local_40 = 7;
      uStack_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      local_54 = 0;
      local_28 = 0;
      local_14._0_1_ = 10;
      local_34 = DAT_01bfe468 + 8;
      local_60 = CONCAT44(local_80,local_80);
      local_30 = 9;
      local_3c = 1;
      local_2c = 0;
      local_38 = 4;
      local_e0 = &DAT_3f7ae148;
      local_58 = &DAT_3f7ae148;
      uVar11 = 0;
      uVar12 = 0;
      uVar4 = 0;
      uVar14 = 0;
      uVar13 = 0;
      FUN_00f19cc0(s_DIGISUMMON_RANK_01ae1fbc,0xf);
      psVar5 = (short *)FUN_015ce480(&local_98,uVar11,uVar12,uVar4,uVar14,uVar13);
      local_14._0_1_ = 0xb;
      if (7 < *(uint *)(psVar5 + 10)) {
        psVar5 = *(short **)psVar5;
      }
      do {
        sVar1 = *psVar5;
        psVar5 = psVar5 + 1;
      } while (sVar1 != 0);
      FUN_00f1a040();
      local_14._0_1_ = 10;
      FUN_00f1a330();
      local_ac = (int *)FUN_01230c40(&local_60,0x23,300);
      local_14._0_1_ = 1;
      local_34 = 0;
      FUN_00f1a330();
      local_d0 = (undefined4 *)FUN_016830c0(0x1c0);
      local_14._0_1_ = 0xc;
      if (local_d0 == (void *)0x0) {
        iVar6 = 0;
      }
      else {
        memset(local_d0,0,0x1c0);
        iVar6 = FUN_01222670();
      }
      local_14 = CONCAT31(local_14._1_3_,1);
      if (iVar6 == 0) {
        piVar8 = (int *)0x0;
      }
      else {
        FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0x20,0x20,
                     s_Random_box_button_definite_infor_01adaee8,0);
        FUN_01223e30();
        puVar7 = operator_new(0xc);
        *puVar7 = 0;
        *(undefined4 *)(puVar7 + 1) = 0;
        *(undefined **)puVar7 = &DAT_01ae1ff4;
        FUN_0145efe0(puVar7);
        piVar8 = (int *)FUN_012317d0(iVar6,*param_2,0xb8,0x13b,0x20,0x20,DAT_01bee88c,DAT_01bee890);
        local_a8 = piVar8;
      }
      if (local_c0 != (int *)0x0) {
        (**(code **)(*local_c0 + 0x40))(0);
      }
      if (piStack_bc != (int *)0x0) {
        (**(code **)(*piStack_bc + 0x40))(0);
      }
      if (piStack_b8 != (int *)0x0) {
        (**(code **)(*piStack_b8 + 0x40))(0);
      }
      if (piStack_b4 != (int *)0x0) {
        (**(code **)(*piStack_b4 + 0x40))(0);
      }
      if (local_b0 != (int *)0x0) {
        (**(code **)(*local_b0 + 0x40))(0);
      }
      if (local_ac != (int *)0x0) {
        (**(code **)(*local_ac + 0x40))(0);
      }
      if (piVar8 != (int *)0x0) {
        (**(code **)(*piVar8 + 0x40))(0);
      }
      puVar9 = (undefined4 *)FUN_016830c0(0x28);
      local_d0 = puVar9;
      if (puVar9 == (undefined4 *)0x0) {
        local_9c = (undefined4 *)0x0;
        FUN_011133d0();
        FUN_01683120();
      }
      else {
        *puVar9 = 0;
        puVar9[1] = 0;
        puVar9[2] = 0;
        puVar9[3] = 0;
        puVar9[4] = 0;
        puVar9[5] = 0;
        puVar9[6] = 0;
        puVar9[7] = 0;
        *(undefined8 *)(puVar9 + 8) = 0;
        uVar11 = *param_2;
        *puVar9 = &DAT_01ae66e0;
        puVar9[1] = 0;
        puVar9[2] = 0;
        puVar9[3] = uVar11;
        *(undefined1 *)(puVar9 + 4) = 0;
        puVar9[5] = 0xfa;
        puVar9[6] = 400;
        *(undefined2 *)(puVar9 + 7) = 0x101;
        puVar9[8] = 0;
        puVar9[9] = 0;
        local_9c = puVar9;
        FUN_012288c0(iVar10);
        puVar7 = operator_new(0xc);
        *puVar7 = 0;
        *(undefined4 *)(puVar7 + 1) = 0;
        *(undefined **)puVar7 = &DAT_01ae1ff4;
        if ((undefined4 *)puVar9[2] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar9[2])(1);
        }
        puVar9[2] = puVar7;
        FUN_0122a090(puVar9);
        FUN_011bdaa0((int)&local_e8 + 4,param_2,&local_c0);
        puVar3 = local_a4;
      }
      *(undefined4 *)puVar3[1] = 0;
      puVar3 = (undefined4 *)*puVar3;
      while (puVar3 != (undefined4 *)0x0) {
        puVar3 = (undefined4 *)*puVar3;
        FUN_017986fc();
      }
      FUN_017986fc();
    }
  }
  ExceptionList = local_1c;
  return;
}


