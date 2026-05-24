
/* ============================================================ */
/* Function: FUN_011ff560 */
/* Entry: 011ff560 */
/* Signature: undefined FUN_011ff560() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011ff560(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  int *piVar5;
  undefined4 uVar6;
  undefined4 extraout_ECX;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  undefined1 local_f0 [8];
  undefined8 local_e8;
  void *local_dc;
  undefined4 *local_d8;
  void *local_d4;
  int *local_d0;
  int *local_cc;
  int *local_c8;
  int *local_c4;
  undefined4 *local_c0;
  undefined1 local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 *local_b0;
  undefined4 uStack_ac;
  undefined4 uStack_a8;
  undefined1 *puStack_a4;
  undefined4 local_9c;
  undefined4 *local_98;
  undefined4 local_94;
  undefined4 *local_90;
  int local_8c;
  undefined4 *local_88;
  int local_84;
  int local_80;
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
  puStack_18 = &LAB_0199215c;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_84 = param_1;
  local_88 = (undefined4 *)FUN_016830c0(0xac,local_24);
  local_14 = 0;
  if (local_88 == (undefined4 *)0x0) {
    iVar3 = 0;
  }
  else {
    memset(local_88,0,0xac);
    iVar3 = FUN_01228950();
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0x9c) = iVar3;
  if (iVar3 != 0) {
    FUN_012472c0(iVar3);
    local_94 = 0x168;
    local_90 = (undefined4 *)0x18e;
    FUN_01228c50(*(undefined4 *)(param_1 + 0x40),0x264,0xa7,0x172,0x193,1,8,0x46,0x46,0,2,0,
                 extraout_ECX,5);
    local_9c = 0xfffffffb;
    local_98 = (undefined4 *)0xfffffffb;
    FUN_01228cf0(s_Control_G_Popup_N2Dlg5_tga_01ae0f00,&local_9c,&local_94);
    FUN_01228dd0(s_SealMaster_sealmaster_highlight__01ae0f60,&DAT_01bee88c,
                 *(int *)(param_1 + 0x9c) + 0x78);
    *(undefined1 *)(*(int *)(param_1 + 0x9c) + 0x94) = 0;
    iVar3 = *(int *)(param_1 + 0x9c);
    *(undefined1 *)(iVar3 + 0xa9) = 0;
    if (*(int *)(iVar3 + 0x6c) != 0) {
      *(undefined1 *)(*(int *)(iVar3 + 0x6c) + 0x10) = 0;
      *(undefined4 *)(iVar3 + 0x6c) = 0;
    }
    local_7c = 0;
    do {
      local_d4 = (void *)FUN_016830c0(0x2c);
      local_14 = 1;
      if (local_d4 == (void *)0x0) {
        local_8c = 0;
      }
      else {
        memset(local_d4,0,0x2c);
        local_8c = FUN_010baa10();
      }
      iVar3 = local_8c;
      local_14 = 0xffffffff;
      if (local_8c == 0) {
        ExceptionList = local_1c;
        return;
      }
      local_c8 = (int *)0x0;
      local_bc = 0;
      local_b8 = 0;
      local_b4 = 0;
      puVar4 = (undefined4 *)FUN_016830c0(0xbc);
      local_14 = 2;
      local_d8 = puVar4;
      if (puVar4 == (undefined4 *)0x0) {
        puVar4 = (undefined4 *)0x0;
      }
      else {
        memset(puVar4,0,0xbc);
        FUN_0123de10();
        *puVar4 = &DAT_01ae721c;
        *(undefined2 *)(puVar4 + 0x28) = 0;
        *(undefined1 *)((int)puVar4 + 0xa2) = 0;
        puVar4[0x29] = 1;
        puVar4[0x2a] = 1;
        puVar4[0x2b] = 0xffffffff;
        puVar4[0x2c] = 0x3f800000;
        puVar4[0x2d] = 0x3f800000;
        puVar4[0x2e] = 0;
      }
      local_14 = 0xffffffff;
      if (puVar4 != (undefined4 *)0x0) {
        FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x46,0x46,
                     s_CommonUI_Slot_Windows_Scroll2_tg_01adc924,0,1);
        piVar5 = (int *)FUN_01231700(puVar4,0,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        if (piVar5 != (int *)0x0) {
          (**(code **)(*piVar5 + 0x54))(1);
        }
      }
      local_dc = (void *)FUN_016830c0(0xa0);
      local_14 = 3;
      if (local_dc == (void *)0x0) {
        local_80 = 0;
      }
      else {
        memset(local_dc,0,0xa0);
        local_80 = FUN_0123de10();
      }
      uVar1 = DAT_01bee890;
      uVar9 = DAT_01bb9b34;
      local_14 = 0xffffffff;
      if (local_80 != 0) {
        local_88 = DAT_01bee88c;
        local_e8 = DAT_01bb9b2c;
        FUN_00f19f10();
        local_2c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_30 = 0;
        local_28 = 0xf;
        local_3c = 0;
        local_14 = 4;
        FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,&DAT_01abd9ec);
        FUN_0124ef30(&local_3c);
        iVar2 = local_80;
        uVar7 = 0;
        uVar8 = local_e8;
        uVar6 = FUN_0124f150(0,local_e8,uVar9);
        FUN_0123e330(0,local_88,uVar1,0x3c,0x3c,uVar6,uVar7,uVar8,uVar9);
        local_14 = 0xffffffff;
        FUN_00f1a420();
        iVar3 = local_8c;
        piVar5 = (int *)FUN_01230f40(iVar2,5,5,DAT_01bee88c,DAT_01bee890);
        local_c8 = piVar5;
        if (piVar5 != (int *)0x0) {
          (**(code **)(*piVar5 + 0x40))(0);
          (**(code **)(*piVar5 + 0x54))();
        }
      }
      puVar4 = (undefined4 *)FUN_016830c0(0xbc);
      local_14 = 5;
      local_98 = puVar4;
      if (puVar4 == (undefined4 *)0x0) {
        puVar4 = (undefined4 *)0x0;
      }
      else {
        memset(puVar4,0,0xbc);
        FUN_0123de10();
        *puVar4 = &DAT_01ae721c;
        *(undefined2 *)(puVar4 + 0x28) = 0;
        *(undefined1 *)((int)puVar4 + 0xa2) = 0;
        puVar4[0x29] = 1;
        puVar4[0x2a] = 1;
        puVar4[0x2b] = 0xffffffff;
        puVar4[0x2c] = 0x3f800000;
        puVar4[0x2d] = 0x3f800000;
        puVar4[0x2e] = 0;
      }
      local_14 = 0xffffffff;
      FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x46,0x46,s_DigiCombine_DigiCombine_Slot_ran_01ae22e8
                   ,0,1);
      FUN_0123d9a0();
      piVar5 = (int *)FUN_01231700(puVar4,0,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      local_d0 = piVar5;
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0x40))(0);
        (**(code **)(*piVar5 + 0x54))();
      }
      puVar4 = (undefined4 *)FUN_016830c0(0xbc);
      local_14 = 6;
      local_90 = puVar4;
      if (puVar4 == (undefined4 *)0x0) {
        puVar4 = (undefined4 *)0x0;
      }
      else {
        memset(puVar4,0,0xbc);
        FUN_0123de10();
        *puVar4 = &DAT_01ae721c;
        *(undefined2 *)(puVar4 + 0x28) = 0;
        *(undefined1 *)((int)puVar4 + 0xa2) = 0;
        puVar4[0x29] = 1;
        puVar4[0x2a] = 1;
        puVar4[0x2b] = 0xffffffff;
        puVar4[0x2c] = 0x3f800000;
        puVar4[0x2d] = 0x3f800000;
        puVar4[0x2e] = 0;
      }
      local_14 = 0xffffffff;
      FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x23,0x23,s_CommonUI_Digimon_rank_new_dds_01ae49e4,0,
                   1);
      FUN_0123d9a0();
      piVar5 = (int *)FUN_01231700(puVar4,0,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      local_cc = piVar5;
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0x40))(0);
        (**(code **)(*piVar5 + 0x54))();
      }
      local_5c = 0;
      local_58 = 7;
      uStack_68 = 0;
      uStack_64 = 0;
      uStack_60 = 0;
      local_6c = 0;
      local_40 = 0;
      local_14 = 7;
      local_4c = DAT_01bfe468 + 8;
      puStack_a4 = &DAT_3f7ae148;
      local_70 = &DAT_3f7ae148;
      local_78 = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_48 = 9;
      local_54 = 1;
      local_44 = 0;
      local_50 = 2;
      local_c4 = (int *)FUN_01230c40(&local_78,DAT_01bee88c,DAT_01bee890);
      if (local_c4 != (int *)0x0) {
        (**(code **)(*local_c4 + 0x40))(0);
      }
      puVar4 = (undefined4 *)FUN_016830c0(0x28);
      local_88 = puVar4;
      if (puVar4 == (undefined4 *)0x0) {
        local_c0 = (undefined4 *)0x0;
      }
      else {
        *puVar4 = 0;
        puVar4[1] = 0;
        puVar4[2] = 0;
        puVar4[3] = 0;
        puVar4[4] = 0;
        puVar4[5] = 0;
        puVar4[6] = 0;
        puVar4[7] = 0;
        *(undefined8 *)(puVar4 + 8) = 0;
        *puVar4 = &DAT_01ae66e0;
        puVar4[1] = 0;
        puVar4[2] = 0;
        puVar4[3] = local_7c;
        *(undefined1 *)(puVar4 + 4) = 0;
        puVar4[5] = 0x46;
        puVar4[6] = 0x46;
        *(undefined2 *)(puVar4 + 7) = 0x101;
        puVar4[8] = 0;
        puVar4[9] = 0;
        local_c0 = puVar4;
        FUN_012288c0(iVar3);
        iVar3 = local_84;
        FUN_0122a090(puVar4);
        FUN_01200260(6,iVar3,&LAB_011fc250,0);
        FUN_011c67d0(local_f0,&local_7c,&local_d0);
      }
      local_14 = 0xffffffff;
      local_4c = 0;
      FUN_00f1a330();
      local_7c = local_7c + 1;
    } while (local_7c < 200);
    local_90 = (undefined4 *)FUN_016830c0(0xec);
    local_14 = 8;
    if (local_90 == (void *)0x0) {
      iVar3 = 0;
    }
    else {
      memset(local_90,0,0xec);
      iVar3 = FUN_01247d90();
    }
    local_14 = 0xffffffff;
    if (iVar3 != 0) {
      uStack_ac = DAT_01bee890;
      local_b0 = DAT_01bee88c;
      uStack_a8 = 0x168;
      puStack_a4 = (undefined1 *)0x18e;
      FUN_01248090(0,0,5,0,DAT_01bee88c,DAT_01bee890,0x10,0x1af,0x10,0xf,DAT_01bee88c,DAT_01bee890,
                   0x168,0x18e);
      FUN_0122a200(iVar3);
    }
  }
  ExceptionList = local_1c;
  return;
}


