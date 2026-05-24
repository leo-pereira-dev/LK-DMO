
/* ============================================================ */
/* Function: FUN_011c7b10 */
/* Entry: 011c7b10 */
/* Signature: undefined FUN_011c7b10() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_011c7b10(int param_1,int *param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  void *pvVar2;
  uint uVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  int iVar6;
  int *piVar7;
  undefined4 *puVar8;
  undefined8 *puVar9;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  undefined4 extraout_ECX_03;
  undefined4 extraout_ECX_04;
  undefined4 extraout_ECX_05;
  undefined4 extraout_ECX_06;
  undefined4 uVar10;
  int iVar11;
  undefined4 uVar12;
  ulonglong uVar13;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  ulonglong local_88;
  int *local_7c;
  undefined4 uStack_78;
  undefined4 *puStack_74;
  undefined4 uStack_70;
  int *piStack_6c;
  int *piStack_68;
  int *piStack_64;
  int *piStack_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int *local_4c;
  undefined4 *local_48;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined8 uStack_34;
  undefined4 uStack_2c;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019923eb;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_5c = param_1;
  local_4c = param_2;
  if (param_1 != 0) {
    local_24 = uVar3;
    local_48 = (undefined4 *)FUN_016830c0(0x2c,uVar3);
    local_14 = 0;
    if (local_48 == (void *)0x0) {
      local_58 = 0;
    }
    else {
      memset(local_48,0,0x2c);
      local_58 = FUN_010baa10();
    }
    iVar11 = local_58;
    local_14 = 0xffffffff;
    if (local_58 != 0) {
      local_7c = (int *)0x0;
      uStack_78 = 0;
      uStack_70 = 0;
      piStack_6c = (int *)0x0;
      piStack_68 = (int *)0x0;
      piStack_64 = (int *)0x0;
      piStack_60 = (int *)0x0;
      local_48 = (undefined4 *)FUN_016830c0(0xa0,uVar3);
      local_14 = 1;
      if (local_48 == (void *)0x0) {
        local_50 = 0;
      }
      else {
        memset(local_48,0,0xa0);
        local_50 = FUN_0123de10();
      }
      uVar1 = DAT_01bee890;
      uVar10 = DAT_01bb9b34;
      local_14 = 0xffffffff;
      if (local_50 != 0) {
        local_48 = DAT_01bee88c;
        local_88 = DAT_01bb9b2c;
        FUN_00f19f10();
        uStack_3c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_2c = 0xf;
        local_40 = 0;
        local_14 = 2;
        FUN_015010a0(&local_40,&DAT_01ae7290,&DAT_01bfbd40,
                     s_DigiCombine_DigiCombine_CardBack_01ae1a84);
        FUN_0124ef30(&local_40);
        iVar6 = local_50;
        uVar12 = 0;
        uVar13 = local_88;
        uVar4 = FUN_0124f150(0,local_88,uVar10);
        FUN_0123e330(0,local_48,uVar1,0x60,0x88,uVar4,uVar12,uVar13,uVar10);
        local_14 = 0xffffffff;
        FUN_00f1a420();
        iVar11 = local_58;
        local_7c = (int *)FUN_01230f40(iVar6,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      }
      local_48 = (undefined4 *)FUN_016830c0(0x8c);
      local_14 = 3;
      if (local_48 == (undefined4 *)0x0) {
        local_50 = 0;
      }
      else {
        memset(local_48,0,0x8c);
        local_50 = FUN_011b9670();
      }
      uVar10 = DAT_01bee890;
      pvVar2 = DAT_01bee88c;
      local_14 = 0xffffffff;
      if (local_50 != 0) {
        uStack_38 = _DAT_01bee894;
        uStack_34 = CONCAT44(uRam01bee89c,uRam01bee898);
        uStack_2c = uRam01bee8a0;
        local_98 = 0;
        uStack_94 = 0;
        uStack_90 = 0;
        uStack_8c = 0;
        local_88 = 0;
        FUN_00f19cc0();
        iVar6 = local_50;
        local_14 = 4;
        FUN_0145ee00(0,pvVar2,uVar10,0x50,0x50,0);
        if ((int)local_88 != 0) {
          FUN_0124af60(&local_98,uStack_38,(int)uStack_34,uStack_34._4_4_,uStack_2c);
        }
        local_14 = 0xffffffff;
        FUN_00f1a420();
        puVar5 = operator_new(0x14);
        *puVar5 = &DAT_01ae2374;
        puVar5[1] = *local_4c;
        puVar5[2] = 0;
        puVar5[3] = 0;
        *(undefined1 *)(puVar5 + 4) = 0;
        local_48 = puVar5;
        if (*(undefined4 **)(iVar6 + 0x34) != (undefined4 *)0x0) {
          (**(code **)**(undefined4 **)(iVar6 + 0x34))(1);
        }
        *(undefined4 **)(iVar6 + 0x34) = puVar5;
        FUN_011c94a0(1,local_54,FUN_011c8490,0);
        iVar11 = local_58;
        uStack_70 = FUN_01231890(iVar6,8,0x1c,DAT_01bee88c,DAT_01bee890);
      }
      local_48 = (undefined4 *)FUN_016830c0(0x8c);
      local_14 = 5;
      if (local_48 == (void *)0x0) {
        iVar6 = 0;
        uVar10 = extraout_ECX;
      }
      else {
        memset(local_48,0,0x8c);
        iVar6 = FUN_012404a0();
        uVar10 = extraout_ECX_00;
      }
      local_14 = 0xffffffff;
      if (iVar6 != 0) {
        FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,200,200,
                     s_DigiCombine_DigiCombine_Gacha_St_01ae1a5c,0,0x14,uVar10,200,200,1,5);
        *(undefined4 *)(iVar6 + 100) = 0x3d4ccccd;
        *(undefined4 *)(iVar6 + 0x68) = 0x3d4ccccd;
        *(undefined1 *)(iVar6 + 0x78) = 0;
        piStack_6c = (int *)FUN_01231020(iVar6,0xffffffcc,0xffffffe0);
      }
      local_48 = (undefined4 *)FUN_016830c0(0x8c);
      local_14 = 6;
      if (local_48 == (undefined4 *)0x0) {
        iVar6 = 0;
        uVar10 = extraout_ECX_01;
      }
      else {
        memset(local_48,0,0x8c);
        iVar6 = FUN_012404a0();
        uVar10 = extraout_ECX_02;
      }
      local_14 = 0xffffffff;
      if (iVar6 != 0) {
        FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,200,200,
                     s_DigiCombine_DigiCombine_Gacha_Ch_01ae1ad4,0,0x10,uVar10,200,200,1,4);
        *(undefined4 *)(iVar6 + 100) = 0x3d4ccccd;
        *(undefined4 *)(iVar6 + 0x68) = 0x3d4ccccd;
        local_48 = operator_new(8);
        *local_48 = &DAT_01ae236c;
        local_48[1] = *local_4c;
        if (*(undefined4 **)(iVar6 + 0x34) != (undefined4 *)0x0) {
          (**(code **)**(undefined4 **)(iVar6 + 0x34))(1);
        }
        *(undefined4 **)(iVar6 + 0x34) = local_48;
        FUN_011c94a0(0,local_54,&LAB_011c8530,0);
        *(undefined1 *)(iVar6 + 0x78) = 0;
        piStack_68 = (int *)FUN_01231020(iVar6,0xffffffcc,0xffffffe0);
      }
      local_48 = (undefined4 *)FUN_016830c0(0x8c);
      local_14 = 7;
      if (local_48 == (void *)0x0) {
        iVar6 = 0;
        uVar10 = extraout_ECX_03;
      }
      else {
        memset(local_48,0,0x8c);
        iVar6 = FUN_012404a0();
        uVar10 = extraout_ECX_04;
      }
      local_14 = 0xffffffff;
      if (iVar6 != 0) {
        FUN_01240640(0,0,DAT_01bee88c,DAT_01bee890,200,200,
                     s_DigiCombine_DigiCombine_Gacha_Bg_01ae1aac,0,0x14,uVar10,200,200,1,5);
        *(undefined4 *)(iVar6 + 100) = 0x3d4ccccd;
        *(undefined4 *)(iVar6 + 0x68) = 0x3d4ccccd;
        piStack_64 = (int *)FUN_01231020(iVar6,0xffffffcc,0xffffffe0);
      }
      local_48 = (undefined4 *)FUN_016830c0(0x8c);
      local_14 = 8;
      if (local_48 == (undefined4 *)0x0) {
        iVar6 = 0;
        uVar10 = extraout_ECX_05;
      }
      else {
        memset(local_48,0,0x8c);
        iVar6 = FUN_012404a0();
        uVar10 = extraout_ECX_06;
      }
      local_14 = 0xffffffff;
      if (iVar6 == 0) {
        piVar7 = (int *)0x0;
      }
      else {
        FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,200,200,
                     s_DigiCombine_DigiCombine_Gacha_En_01ae1b28,0,0x10,uVar10,200,200,1,4);
        *(undefined4 *)(iVar6 + 100) = 0x3d4ccccd;
        *(undefined4 *)(iVar6 + 0x68) = 0x3d4ccccd;
        local_48 = operator_new(8);
        *local_48 = &DAT_01ae236c;
        local_48[1] = *local_4c;
        if (*(undefined4 **)(iVar6 + 0x34) != (undefined4 *)0x0) {
          (**(code **)**(undefined4 **)(iVar6 + 0x34))(1);
        }
        *(undefined4 **)(iVar6 + 0x34) = local_48;
        FUN_011c94a0(0,local_54,FUN_011c8650,0);
        *(undefined1 *)(iVar6 + 0x78) = 0;
        piVar7 = (int *)FUN_01231020(iVar6,0xffffffcc,0xffffffe0);
        piStack_60 = piVar7;
      }
      if (local_7c != (int *)0x0) {
        (**(code **)(*local_7c + 0x54))(1);
      }
      if (piStack_6c != (int *)0x0) {
        (**(code **)(*piStack_6c + 0x54))(1);
      }
      if (piStack_68 != (int *)0x0) {
        (**(code **)(*piStack_68 + 0x54))(1);
      }
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))(1);
      }
      if (piStack_64 != (int *)0x0) {
        (**(code **)(*piStack_64 + 0x54))(1);
      }
      puVar5 = (undefined4 *)FUN_016830c0(0x28);
      local_48 = puVar5;
      if (puVar5 == (undefined4 *)0x0) {
        FUN_011133d0();
        FUN_01683120();
      }
      else {
        *puVar5 = 0;
        puVar5[1] = 0;
        puVar5[2] = 0;
        puVar5[3] = 0;
        puVar5[4] = 0;
        puVar5[5] = 0;
        puVar5[6] = 0;
        puVar5[7] = 0;
        *(undefined8 *)(puVar5 + 8) = 0;
        uVar10 = param_3[1];
        uVar1 = *(undefined4 *)(local_5c + 0x68);
        puVar5[5] = *param_3;
        *puVar5 = &DAT_01ae66e0;
        puVar5[1] = 0;
        puVar5[2] = 0;
        puVar5[3] = uVar1;
        *(undefined1 *)(puVar5 + 4) = 0;
        puVar5[6] = uVar10;
        *(undefined2 *)(puVar5 + 7) = 0x101;
        puVar5[8] = 0;
        puVar5[9] = 0;
        puStack_74 = puVar5;
        FUN_012288c0(iVar11);
        puVar5[5] = *param_3;
        puVar5[6] = param_3[1];
        puVar8 = operator_new(8);
        *puVar8 = &DAT_01ae236c;
        puVar8[1] = *local_4c;
        local_48 = puVar8;
        if ((undefined4 *)puVar5[2] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar5[2])(1);
        }
        puVar5[2] = puVar8;
        FUN_0122a090(puVar5);
        piVar7 = local_4c;
        puVar9 = (undefined8 *)FUN_00f1d720();
        uStack_34 = *puVar9;
        if ((*(char *)(*(int *)(puVar9 + 1) + 0xd) != '\0') ||
           (*piVar7 < *(int *)(*(int *)(puVar9 + 1) + 0x10))) {
          if (*(int *)(local_54 + 0x80) == 0x4ec4ec4) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          uVar10 = *(undefined4 *)(local_54 + 0x7c);
          local_14 = 9;
          local_88 = ZEXT48((undefined4 *)(local_54 + 0x7c));
          puVar5 = operator_new(0x34);
          iVar11 = *local_4c;
          puVar5[5] = local_7c;
          puVar5[6] = uStack_78;
          puVar5[7] = puStack_74;
          puVar5[8] = uStack_70;
          puVar5[4] = iVar11;
          puVar5[9] = piStack_6c;
          puVar5[10] = piStack_68;
          puVar5[0xb] = piStack_64;
          puVar5[0xc] = piStack_60;
          *puVar5 = uVar10;
          puVar5[1] = uVar10;
          puVar5[2] = uVar10;
          *(undefined2 *)(puVar5 + 3) = 0;
          FUN_00f1b3e0((undefined4)uStack_34,(int)((ulonglong)uStack_34 >> 0x20),puVar5);
        }
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


