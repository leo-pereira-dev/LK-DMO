
/* ============================================================ */
/* Function: FUN_011b8e70 */
/* Entry: 011b8e70 */
/* Signature: undefined FUN_011b8e70() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_011b8e70(int param_1,int *param_2,undefined4 *param_3)

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
  undefined4 local_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  ulonglong local_80;
  int *local_78;
  undefined4 uStack_74;
  int *piStack_70;
  int *piStack_6c;
  int *local_68;
  int *piStack_64;
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
  puStack_18 = &LAB_01991408;
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
      local_68 = (int *)0x0;
      local_78 = (int *)0x0;
      uStack_74 = 0;
      piStack_70 = (int *)0x0;
      piStack_6c = (int *)0x0;
      piStack_64 = (int *)0x0;
      local_48 = (undefined4 *)FUN_016830c0(0xa0,uVar3);
      local_14 = 1;
      if (local_48 == (void *)0x0) {
        local_54 = 0;
      }
      else {
        memset(local_48,0,0xa0);
        local_54 = FUN_0123de10();
      }
      uVar1 = DAT_01bee890;
      uVar10 = DAT_01bb9b34;
      local_14 = 0xffffffff;
      if (local_54 != 0) {
        local_48 = DAT_01bee88c;
        local_80 = DAT_01bb9b2c;
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
        iVar6 = local_54;
        uVar12 = 0;
        uVar13 = local_80;
        uVar4 = FUN_0124f150(0,local_80,uVar10);
        FUN_0123e330(0,local_48,uVar1,0x60,0x88,uVar4,uVar12,uVar13,uVar10);
        local_14 = 0xffffffff;
        FUN_00f1a420();
        iVar11 = local_58;
        local_78 = (int *)FUN_01230f40(iVar6,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      }
      local_48 = (undefined4 *)FUN_016830c0(0x8c);
      local_14 = 3;
      if (local_48 == (undefined4 *)0x0) {
        local_54 = 0;
      }
      else {
        memset(local_48,0,0x8c);
        local_54 = FUN_011b9670();
      }
      uVar10 = DAT_01bee890;
      pvVar2 = DAT_01bee88c;
      local_14 = 0xffffffff;
      if (local_54 != 0) {
        uStack_38 = _DAT_01bee894;
        uStack_34 = CONCAT44(uRam01bee89c,uRam01bee898);
        uStack_2c = uRam01bee8a0;
        local_90 = 0;
        uStack_8c = 0;
        uStack_88 = 0;
        uStack_84 = 0;
        local_80 = 0;
        FUN_00f19cc0();
        iVar6 = local_54;
        local_14 = 4;
        FUN_0145ee00(0,pvVar2,uVar10,0x50,0x50,0);
        if ((int)local_80 != 0) {
          FUN_0124af60(&local_90,uStack_38,(int)uStack_34,uStack_34._4_4_,uStack_2c);
        }
        local_14 = 0xffffffff;
        FUN_00f1a420();
        puVar5 = operator_new(0x14);
        *puVar5 = &DAT_01ae1f38;
        puVar5[1] = *local_4c;
        puVar5[2] = 0;
        puVar5[3] = 0;
        *(undefined1 *)(puVar5 + 4) = 0;
        local_48 = puVar5;
        if (*(undefined4 **)(iVar6 + 0x34) != (undefined4 *)0x0) {
          (**(code **)**(undefined4 **)(iVar6 + 0x34))(1);
        }
        *(undefined4 **)(iVar6 + 0x34) = puVar5;
        FUN_011bae50(1,local_50,FUN_011ba150,0);
        iVar11 = local_58;
        uStack_74 = FUN_01231890(iVar6,8,0x1c,DAT_01bee88c,DAT_01bee890);
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
        piStack_70 = (int *)FUN_01231020(iVar6,0xffffffcc,0xffffffe0);
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
        *local_48 = &DAT_01ae1f30;
        local_48[1] = *local_4c;
        if (*(undefined4 **)(iVar6 + 0x34) != (undefined4 *)0x0) {
          (**(code **)**(undefined4 **)(iVar6 + 0x34))(1);
        }
        *(undefined4 **)(iVar6 + 0x34) = local_48;
        FUN_011bae50(0,local_50,FUN_011ba1e0,0);
        *(undefined1 *)(iVar6 + 0x78) = 0;
        piStack_6c = (int *)FUN_01231020(iVar6,0xffffffcc,0xffffffe0);
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
        local_68 = (int *)FUN_01231020(iVar6,0xffffffcc,0xffffffe0);
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
        *local_48 = &DAT_01ae1f30;
        local_48[1] = *local_4c;
        if (*(undefined4 **)(iVar6 + 0x34) != (undefined4 *)0x0) {
          (**(code **)**(undefined4 **)(iVar6 + 0x34))(1);
        }
        *(undefined4 **)(iVar6 + 0x34) = local_48;
        FUN_011bae50(0,local_50,FUN_011ba320,0);
        *(undefined1 *)(iVar6 + 0x78) = 0;
        piVar7 = (int *)FUN_01231020(iVar6,0xffffffcc,0xffffffe0);
        piStack_64 = piVar7;
      }
      if (local_78 != (int *)0x0) {
        (**(code **)(*local_78 + 0x54))(1);
      }
      if (piStack_70 != (int *)0x0) {
        (**(code **)(*piStack_70 + 0x54))(1);
      }
      if (piStack_6c != (int *)0x0) {
        (**(code **)(*piStack_6c + 0x54))(1);
      }
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))(1);
      }
      if (local_68 != (int *)0x0) {
        (**(code **)(*local_68 + 0x54))(1);
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
        FUN_012288c0(iVar11);
        puVar5[5] = *param_3;
        puVar5[6] = param_3[1];
        puVar8 = operator_new(8);
        *puVar8 = &DAT_01ae1f30;
        puVar8[1] = *local_4c;
        local_48 = puVar8;
        if ((undefined4 *)puVar5[2] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar5[2])(1);
        }
        puVar5[2] = puVar8;
        FUN_0122a090(puVar5);
        iVar11 = local_50;
        puVar9 = (undefined8 *)FUN_00f1d720();
        uStack_34 = *puVar9;
        if ((*(char *)(*(int *)(puVar9 + 1) + 0xd) != '\0') ||
           (*local_4c < *(int *)(*(int *)(puVar9 + 1) + 0x10))) {
          if (*(int *)(iVar11 + 0x80) == 0x5555555) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          uVar10 = *(undefined4 *)(iVar11 + 0x7c);
          local_14 = 9;
          local_80 = (ulonglong)(local_50 + 0x7c);
          puVar8 = operator_new(0x30);
          iVar11 = *local_4c;
          puVar8[5] = local_78;
          puVar8[6] = uStack_74;
          puVar8[7] = piStack_70;
          puVar8[8] = piStack_6c;
          puVar8[4] = iVar11;
          *(ulonglong *)(puVar8 + 9) = CONCAT44(piStack_64,local_68);
          puVar8[0xb] = puVar5;
          *puVar8 = uVar10;
          puVar8[1] = uVar10;
          puVar8[2] = uVar10;
          *(undefined2 *)(puVar8 + 3) = 0;
          FUN_00f1b3e0((undefined4)uStack_34,(int)((ulonglong)uStack_34 >> 0x20),puVar8);
        }
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


