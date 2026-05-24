
/* ============================================================ */
/* Function: FUN_011c3270 */
/* Entry: 011c3270 */
/* Signature: undefined FUN_011c3270() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011c3270(int param_1)

{
  undefined8 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 uVar4;
  int iVar5;
  void *pvVar6;
  undefined4 *puVar7;
  int *piVar8;
  undefined4 uVar9;
  undefined4 extraout_ECX;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 local_b0;
  undefined4 uStack_ac;
  int *local_94;
  int *local_90;
  undefined4 local_8c;
  undefined4 local_88;
  int *local_84;
  undefined4 *local_80;
  undefined1 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 *local_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 *local_60;
  int local_5c;
  int local_58;
  int local_54;
  uint local_50;
  undefined4 *local_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  undefined4 local_34;
  void *local_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01991e5b;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_58 = param_1;
  local_4c = (undefined4 *)FUN_016830c0(0xac,local_24);
  local_14 = 0;
  if (local_4c == (void *)0x0) {
    iVar5 = 0;
  }
  else {
    memset(local_4c,0,0xac);
    iVar5 = FUN_01228950();
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0x90) = iVar5;
  if (iVar5 != 0) {
    FUN_012472c0(iVar5);
    FUN_01228c50(*(undefined4 *)(param_1 + 0x40),0x3c,0x91,0x10e,0x226,8,0x34,0x38,0x38,0,2,0,
                 extraout_ECX,4);
    FUN_01228dd0(s_SealMaster_sealmaster_highlight__01ae0f60,&DAT_01bee88c,
                 *(int *)(param_1 + 0x90) + 0x78);
    *(undefined1 *)(*(int *)(param_1 + 0x90) + 0x94) = 0;
    iVar5 = *(int *)(param_1 + 0x90);
    *(undefined1 *)(iVar5 + 0xa9) = 0;
    if (*(int *)(iVar5 + 0x6c) != 0) {
      *(undefined1 *)(*(int *)(iVar5 + 0x6c) + 0x10) = 0;
      *(undefined4 *)(iVar5 + 0x6c) = 0;
    }
    local_4c = (undefined4 *)FUN_016830c0(0x2c);
    local_14 = 1;
    if (local_4c == (undefined4 *)0x0) {
      iVar5 = 0;
    }
    else {
      memset(local_4c,0,0x2c);
      iVar5 = FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (iVar5 != 0) {
      local_30 = (void *)0x0;
      local_2c = 0;
      local_30 = operator_new(0x38);
      *(void **)local_30 = local_30;
      *(void **)((int)local_30 + 4) = local_30;
      *(void **)((int)local_30 + 8) = local_30;
      *(undefined2 *)((int)local_30 + 0xc) = 0x101;
      local_28 = 0;
      local_14 = 2;
      local_50 = 0;
      do {
        pvVar6 = (void *)FUN_016830c0(0x2c);
        local_14._0_1_ = 3;
        if (pvVar6 == (void *)0x0) {
          local_5c = 0;
        }
        else {
          memset(pvVar6,0,0x2c);
          local_5c = FUN_010baa10();
        }
        iVar5 = local_5c;
        local_14._0_1_ = 2;
        uVar4 = (undefined1)local_14;
        local_14._0_1_ = 2;
        if (local_5c == 0) goto LAB_011c39c5;
        local_8c = 0;
        local_88 = 0;
        local_84 = (int *)0x0;
        local_7c = 0;
        local_78 = 0;
        local_74 = 0;
        puVar7 = (undefined4 *)FUN_016830c0(0xbc);
        local_14._0_1_ = 4;
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
        local_14._0_1_ = 2;
        if (puVar7 != (undefined4 *)0x0) {
          FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x38,0x38,
                       s_DigiCombine_DigiCombine_Slot_Emp_01ae228c,0,1);
          piVar8 = (int *)FUN_01231700(puVar7,0,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890)
          ;
          if (piVar8 != (int *)0x0) {
            (**(code **)(*piVar8 + 0x54))(1);
          }
        }
        pvVar6 = (void *)FUN_016830c0(0xa0);
        local_14._0_1_ = 5;
        if (pvVar6 == (void *)0x0) {
          local_54 = 0;
        }
        else {
          memset(pvVar6,0,0xa0);
          local_54 = FUN_0123de10();
        }
        uVar2 = DAT_01bee890;
        uVar11 = DAT_01bb9b34;
        uVar1 = DAT_01bb9b2c;
        local_14._0_1_ = 2;
        uVar4 = (undefined1)local_14;
        local_14._0_1_ = 2;
        if (local_54 != 0) {
          local_4c = DAT_01bee88c;
          FUN_00f19f10(&DAT_01abd9ec,0);
          local_38 = 0;
          uStack_44 = 0;
          uStack_40 = 0;
          uStack_3c = 0;
          local_34 = 0xf;
          local_48 = 0;
          local_14._0_1_ = 6;
          FUN_015010a0(&local_48,&DAT_01ae7290,&DAT_01bfbd40,&DAT_01abd9ec);
          FUN_0124ef30(&local_48);
          iVar3 = local_54;
          local_b0 = (undefined4)uVar1;
          uStack_ac = (undefined4)((ulonglong)uVar1 >> 0x20);
          uVar10 = 0;
          uVar9 = FUN_0124f150(0,local_b0,uStack_ac,uVar11);
          FUN_0123e330(0,local_4c,uVar2,0x32,0x32,uVar9,uVar10,local_b0,uStack_ac,uVar11);
          local_14._0_1_ = 2;
          FUN_00f1a420();
          iVar5 = local_5c;
          piVar8 = (int *)FUN_01230f40(iVar3,5,5,DAT_01bee88c,DAT_01bee890);
          local_84 = piVar8;
          uVar4 = (undefined1)local_14;
          if (piVar8 != (int *)0x0) {
            (**(code **)(*piVar8 + 0x40))(0);
            (**(code **)(*piVar8 + 0x54))(1);
            uVar4 = (undefined1)local_14;
          }
        }
        local_14._0_1_ = uVar4;
        puVar7 = (undefined4 *)FUN_016830c0(0xbc);
        local_14._0_1_ = 7;
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
        local_14 = CONCAT31(local_14._1_3_,2);
        FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x38,0x38,
                     s_DigiCombine_DigiCombine_Slot_ran_01ae22e8,0,1);
        FUN_0123d9a0(0x50,0x50);
        piVar8 = (int *)FUN_01231700(puVar7,0,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        local_94 = piVar8;
        if (piVar8 != (int *)0x0) {
          (**(code **)(*piVar8 + 0x40))(0);
          (**(code **)(*piVar8 + 0x54))(1);
        }
        puVar7 = (undefined4 *)FUN_016830c0(0xbc);
        local_14._0_1_ = 8;
        local_60 = puVar7;
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
        local_14 = CONCAT31(local_14._1_3_,2);
        FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x1c,0x1c,s_CommonUI_digimon_rank_dds_01ad8c38,0,1)
        ;
        FUN_0123d9a0(0x48,0x3e);
        piVar8 = (int *)FUN_01231700(puVar7,0,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        local_90 = piVar8;
        if (piVar8 != (int *)0x0) {
          (**(code **)(*piVar8 + 0x40))(0);
          (**(code **)(*piVar8 + 0x54))(1);
        }
        puVar7 = (undefined4 *)FUN_016830c0(0x28);
        local_4c = puVar7;
        if (puVar7 == (undefined4 *)0x0) {
          local_80 = (undefined4 *)0x0;
        }
        else {
          *puVar7 = 0;
          puVar7[1] = 0;
          puVar7[2] = 0;
          puVar7[3] = 0;
          puVar7[4] = 0;
          puVar7[5] = 0;
          puVar7[6] = 0;
          puVar7[7] = 0;
          *(undefined8 *)(puVar7 + 8) = 0;
          *puVar7 = &DAT_01ae66e0;
          puVar7[1] = 0;
          puVar7[2] = 0;
          puVar7[3] = local_50;
          *(undefined1 *)(puVar7 + 4) = 0;
          puVar7[5] = 0x38;
          puVar7[6] = 0x38;
          *(undefined2 *)(puVar7 + 7) = 0x101;
          puVar7[8] = 0;
          puVar7[9] = 0;
          local_80 = puVar7;
          FUN_012288c0(iVar5);
          iVar5 = local_58;
          FUN_0122a090(puVar7);
          FUN_011c6530(6,iVar5,FUN_011c25f0,0);
          FUN_011c66b0(&uStack_68,&local_50,&local_94);
        }
        local_50 = local_50 + 1;
      } while (local_50 < 0x2c);
      local_60 = (undefined4 *)FUN_016830c0(0xec);
      local_14._0_1_ = 9;
      if (local_60 == (undefined4 *)0x0) {
        piVar8 = (int *)0x0;
      }
      else {
        memset(local_60,0,0xec);
        piVar8 = (int *)FUN_01247d90();
      }
      local_14._0_1_ = 2;
      uVar4 = (undefined1)local_14;
      if (piVar8 != (int *)0x0) {
        uStack_6c = DAT_01bee890;
        local_70 = DAT_01bee88c;
        uStack_68 = 0x104;
        uStack_64 = 0x221;
        FUN_01248090(0,0,5,0,DAT_01bee88c,DAT_01bee890,0x10,0x242,0x10,0xf,DAT_01bee88c,DAT_01bee890
                     ,0x104,0x221);
        (**(code **)(*piVar8 + 0x1c))(0);
        FUN_0122a200(piVar8);
        uVar4 = (undefined1)local_14;
      }
LAB_011c39c5:
      local_14._0_1_ = uVar4;
      FUN_0115a360();
    }
  }
  ExceptionList = local_1c;
  return;
}


