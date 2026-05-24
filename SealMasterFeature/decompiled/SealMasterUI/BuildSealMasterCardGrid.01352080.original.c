
/* ============================================================ */
/* Function: FUN_01352080 */
/* Entry: 01352080 */
/* Signature: undefined FUN_01352080() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_01352080(int param_1)

{
  undefined1 uVar1;
  char cVar2;
  short sVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  int iVar9;
  uint3 uVar10;
  void *pvVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  short ****ppppsVar15;
  undefined4 uVar16;
  int *piVar17;
  int *piVar18;
  int *piVar19;
  undefined4 *puVar20;
  undefined4 *puVar21;
  int extraout_ECX;
  short *psVar22;
  uint uVar23;
  char ****ppppcVar24;
  uint uVar25;
  undefined4 uVar26;
  undefined1 *puVar27;
  int local_f4;
  char ***local_bc [5];
  uint local_a8;
  short ***local_a4 [4];
  uint local_94;
  uint local_90;
  undefined8 local_8c;
  undefined1 *local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined1 local_68;
  undefined4 local_64;
  int local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  int local_50;
  undefined8 local_4c;
  short local_44 [2];
  undefined4 uStack_40;
  int iStack_3c;
  int iStack_38;
  int *piStack_34;
  int *piStack_30;
  undefined8 uStack_2c;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  uint local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019a9c01;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  FUN_011a40a0();
  if (*(int *)(param_1 + 0xb0) != 0) {
    local_4c = 0;
    FUN_01356e10();
    local_14 = 0;
    local_f4 = 0;
    local_50 = *(int *)local_4c;
    if ((int *)local_50 != (int *)local_4c) {
      do {
        iVar9 = local_50;
        pvVar11 = (void *)FUN_016830c0();
        local_14._0_1_ = 1;
        if (pvVar11 == (void *)0x0) {
          iVar12 = 0;
        }
        else {
          memset(pvVar11,0,0x2c);
          iVar12 = FUN_010baa10();
        }
        uVar10 = local_14._1_3_;
        local_14 = (uint)local_14._1_3_ << 8;
        if (iVar12 == 0) break;
        local_70 = 0;
        local_6c = 7;
        uStack_7c = 0;
        uStack_78 = 0;
        uStack_74 = 0;
        local_80 = 0;
        local_54 = 0;
        local_14._0_1_ = 2;
        local_60 = DAT_01bfe468 + 8;
        local_8c = CONCAT44(DAT_01b02744,DAT_01b02744);
        local_5c = 0xb;
        local_84 = &DAT_3f7ae148;
        local_68 = 1;
        local_58 = 0;
        local_64 = 0;
        local_14._1_3_ = uVar10;
        iVar12 = FUN_01446f10();
        psVar22 = (short *)(iVar9 + 0x18);
        iVar12 = *(int *)(extraout_ECX + 0xc + iVar12 * 4);
        iVar13 = FUN_01250f00();
        if (iVar13 < 99) {
          if (7 < *(uint *)(iVar9 + 0x2c)) {
            psVar22 = *(short **)psVar22;
          }
          do {
            sVar3 = *psVar22;
            psVar22 = psVar22 + 1;
          } while (sVar3 != 0);
          FUN_00f1a040();
        }
        else {
          FUN_00f221a0();
          local_14._0_1_ = 3;
          iStack_3c = 0;
          iStack_38 = 0;
          piStack_34 = (int *)0x0;
          piStack_30 = (int *)0x0;
          uStack_2c = 0;
          FUN_00f1a200();
          local_14._0_1_ = 4;
          iVar13 = 0;
          uVar25 = 0;
          uVar23 = local_94;
          if (local_94 != 0) {
            do {
              ppppsVar15 = local_a4;
              if (7 < local_90) {
                ppppsVar15 = (short ****)local_a4[0];
              }
              if (*(short *)((int)ppppsVar15 + uVar25 * 2) == 0xd) {
                iVar14 = 0;
              }
              else {
                FUN_014a5220();
                FUN_014a59f0();
                iVar14 = *(int *)(*(int *)(iVar12 + 0x54) + 0x40) >> 6;
                uVar23 = local_94;
              }
              iVar13 = iVar13 + iVar14;
              if (0x56 < iVar13) {
                FUN_012050c0(uVar25 - 1);
                ppppsVar15 = local_a4;
                if (7 < local_90) {
                  ppppsVar15 = (short ****)local_a4[0];
                }
                if (*(short *)((int)ppppsVar15 + uVar25 * 2) != 0xd) {
                  FUN_014a5220();
                  FUN_014a59f0();
                }
                break;
              }
              uVar25 = uVar25 + 1;
            } while (uVar25 < uVar23);
          }
          ppppsVar15 = local_a4;
          if (7 < local_90) {
            ppppsVar15 = (short ****)local_a4[0];
          }
          do {
            sVar3 = *(short *)ppppsVar15;
            ppppsVar15 = (short ****)((int)ppppsVar15 + 2);
          } while (sVar3 != 0);
          FUN_00f1a040();
          FUN_00f1a330();
          local_14._0_1_ = 2;
          FUN_00f1a330();
        }
        uVar16 = DAT_01bb9b34;
        uVar8 = DAT_01bb9b2c;
        if (((DAT_01bfb2b4 != 0) && (*(int *)(*(int *)(param_1 + 0xa8) + 100) != 0)) &&
           (iVar12 = FUN_015db910(), iVar12 != 0)) {
          FUN_00f22020();
        }
        local_8c = uVar8;
        local_84 = (undefined1 *)uVar16;
        FUN_01230c40();
        local_14._0_1_ = 0;
        local_60 = 0;
        FUN_00f1a330();
        pvVar11 = (void *)FUN_016830c0();
        local_14._0_1_ = 5;
        if (pvVar11 == (void *)0x0) {
          iVar12 = 0;
        }
        else {
          memset(pvVar11,0,0xa0);
          iVar12 = FUN_0123de10();
        }
        local_14._0_1_ = 0;
        uVar1 = (undefined1)local_14;
        local_14._0_1_ = 0;
        if (iVar12 != 0) {
          FUN_00f19f10();
          iStack_38 = 0;
          piStack_34 = (int *)0x0;
          piStack_30 = (int *)0x0;
          uStack_2c = 0xf00000000;
          iStack_3c = 0;
          local_14._0_1_ = 6;
          FUN_015010a0(&iStack_3c);
          FUN_0124ef30();
          uVar26 = 0;
          uVar16 = FUN_0124f150(0);
          FUN_0123e330(0,0,0x16,0x56,0x80,uVar16,uVar26);
          local_14._0_1_ = 0;
          FUN_00f1a420();
          piVar17 = (int *)FUN_01230f40(iVar12,DAT_01bee88c);
          (**(code **)(*piVar17 + 0x54))();
          uVar1 = (undefined1)local_14;
        }
        local_14._0_1_ = uVar1;
        pvVar11 = (void *)FUN_016830c0();
        local_14._0_1_ = 7;
        if (pvVar11 == (void *)0x0) {
          iVar13 = 0;
        }
        else {
          memset(pvVar11,0,0xa0);
          iVar13 = FUN_0123de10();
        }
        local_14._0_1_ = 0;
        uVar1 = (undefined1)local_14;
        local_14._0_1_ = 0;
        if (iVar13 != 0) {
          FUN_00f19f10();
          iStack_38 = 0;
          piStack_34 = (int *)0x0;
          piStack_30 = (int *)0x0;
          uStack_2c = 0xf00000000;
          iStack_3c = 0;
          local_14._0_1_ = 8;
          FUN_015010a0(&iStack_3c);
          FUN_0124ef30();
          uVar26 = 0;
          uVar16 = FUN_0124f150(0);
          FUN_0123e330(0,2,0x1a,0x54,0x6a,uVar16,uVar26);
          local_14._0_1_ = 0;
          FUN_00f1a420();
          piVar17 = (int *)FUN_01230f40(iVar13,DAT_01bee88c);
          (**(code **)(*piVar17 + 0x54))();
          uVar1 = (undefined1)local_14;
        }
        local_14._0_1_ = uVar1;
        pvVar11 = (void *)FUN_016830c0();
        local_14._0_1_ = 9;
        if (pvVar11 == (void *)0x0) {
          piVar17 = (int *)0x0;
        }
        else {
          memset(pvVar11,0,0x1c0);
          piVar17 = (int *)FUN_01222670();
        }
        local_14._0_1_ = 0;
        if (piVar17 != (int *)0x0) {
          FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0x20);
          FUN_01223e30();
          (**(code **)(*piVar17 + 0x1c))();
          FUN_012317d0(piVar17,local_f4,0xfffffffb,0x11,0x20);
        }
        pvVar11 = (void *)FUN_016830c0();
        local_14._0_1_ = 10;
        if (pvVar11 == (void *)0x0) {
          piVar18 = (int *)0x0;
        }
        else {
          memset(pvVar11,0,0x1c0);
          piVar18 = (int *)FUN_01222670();
        }
        local_14._0_1_ = 0;
        if (piVar18 != (int *)0x0) {
          FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0x20);
          FUN_01223e30();
          (**(code **)(*piVar18 + 0x1c))();
          FUN_012317d0(piVar18,local_f4,0xfffffffb,0x11,0x20);
        }
        pvVar11 = (void *)FUN_016830c0();
        local_14._0_1_ = 0xb;
        if (pvVar11 == (void *)0x0) {
          iVar14 = 0;
        }
        else {
          memset(pvVar11,0,0xa0);
          iVar14 = FUN_0123de10();
        }
        local_14._0_1_ = 0;
        if (iVar14 != 0) {
          FUN_00f19f10();
          iStack_38 = 0;
          piStack_34 = (int *)0x0;
          piStack_30 = (int *)0x0;
          uStack_2c = 0xf00000000;
          iStack_3c = 0;
          local_14._0_1_ = 0xc;
          FUN_015010a0(&iStack_3c);
          FUN_0124ef30();
          puVar27 = &stack0xfffffe80;
          uVar16 = FUN_0124f150(&stack0xfffffe80);
          FUN_0123e4d0(0,0,0x84,0x56,0xe,_DAT_01b02d10,uRam01b02d14,uRam01b02d18,uRam01b02d1c,uVar16
                       ,puVar27);
          local_14._0_1_ = 0;
          FUN_00f1a420();
          piVar19 = (int *)FUN_01230f40(iVar14,DAT_01bee88c);
          (**(code **)(*piVar19 + 0x54))();
        }
        local_70 = 0;
        local_6c = 7;
        uStack_7c = 0;
        uStack_78 = 0;
        uStack_74 = 0;
        local_80 = 0;
        local_54 = 0;
        local_14._0_1_ = 0xd;
        local_60 = DAT_01bfe468 + 8;
        local_84 = (undefined1 *)DAT_01bb9b34;
        local_8c = DAT_01bb9b2c;
        local_5c = 0xb;
        local_68 = 1;
        local_58 = 0;
        local_64 = 0;
        local_44[0] = 0;
        local_44[1] = 0;
        uStack_40 = 0;
        iStack_3c = 0;
        iStack_38 = 0;
        piStack_34 = (int *)0x0;
        piStack_30 = (int *)0x0;
        uStack_2c = 0;
        FUN_00f222a0(local_44);
        psVar22 = local_44;
        do {
          sVar3 = *psVar22;
          psVar22 = psVar22 + 1;
        } while (sVar3 != 0);
        FUN_00f1a040();
        local_64 = 1;
        uVar16 = FUN_01230c40();
        local_14 = (uint)local_14._1_3_ << 8;
        local_60 = 0;
        FUN_00f1a330();
        puVar20 = (undefined4 *)FUN_016830c0();
        if (puVar20 == (undefined4 *)0x0) break;
        *puVar20 = 0;
        puVar20[1] = 0;
        puVar20[2] = 0;
        puVar20[3] = 0;
        puVar20[4] = 0;
        puVar20[5] = 0;
        puVar20[6] = 0;
        puVar20[7] = 0;
        *(undefined8 *)(puVar20 + 8) = 0;
        *puVar20 = &DAT_01ae66e0;
        puVar20[1] = 0;
        puVar20[2] = 0;
        puVar20[3] = local_f4;
        *(undefined1 *)(puVar20 + 4) = 0;
        puVar20[5] = 0x56;
        puVar20[6] = 0x80;
        *(undefined2 *)(puVar20 + 7) = 0x101;
        puVar20[8] = 0;
        puVar20[9] = 0;
        puVar21 = operator_new(0x20);
        uVar26 = *(undefined4 *)(iVar9 + 0x44);
        uVar4 = *(undefined4 *)(iVar9 + 0x40);
        uVar5 = *(undefined4 *)(iVar9 + 0x48);
        uVar1 = *(undefined1 *)(iVar9 + 0x34);
        uVar6 = *(undefined4 *)(iVar9 + 0x38);
        uVar7 = *(undefined4 *)(iVar9 + 0x4c);
        puVar21[1] = *(undefined4 *)(iVar9 + 0x30);
        *puVar21 = &DAT_01af1f48;
        puVar21[2] = uVar5;
        puVar21[5] = uVar7;
        puVar21[4] = uVar26;
        puVar21[6] = uVar6;
        puVar21[3] = uVar4;
        *(undefined1 *)(puVar21 + 7) = uVar1;
        if ((undefined4 *)puVar20[2] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar20[2])();
        }
        puVar20[2] = puVar21;
        FUN_012288c0();
        FUN_0108ed50();
        local_14._0_1_ = 0xe;
        FUN_0134f710();
        FUN_0108ee60();
        local_14 = CONCAT31(local_14._1_3_,0xf);
        if (iVar13 != 0) {
          ppppcVar24 = local_bc;
          if (0xf < local_a8) {
            ppppcVar24 = (char ****)local_bc[0];
          }
          if (ppppcVar24 != (char ****)0x0) {
            *(undefined4 *)(iVar13 + 0x58) = DAT_01bee88c;
            iStack_38 = 0;
            piStack_34 = (int *)0x0;
            piStack_30 = (int *)0x0;
            *(undefined4 *)(iVar13 + 0x5c) = DAT_01bee890;
            uStack_2c = 0xf00000000;
            iStack_3c = 0;
            local_14._0_1_ = 0x10;
            do {
              cVar2 = *(char *)ppppcVar24;
              ppppcVar24 = (char ****)((int)ppppcVar24 + 1);
            } while (cVar2 != '\0');
            FUN_00f264c0();
            FUN_0124ee50();
            local_14 = CONCAT31(local_14._1_3_,0xf);
            FUN_00f1a420();
          }
        }
        uVar1 = *(undefined1 *)(iVar9 + 0x34);
        if (piVar17 != (int *)0x0) {
          (**(code **)(*piVar17 + 0x1c))();
        }
        if (piVar18 != (int *)0x0) {
          (**(code **)(*piVar18 + 0x1c))();
        }
        if ((puVar20 != (undefined4 *)0x0) && (iVar14 = __RTDynamicCast(puVar20[2],0), iVar14 != 0))
        {
          *(undefined1 *)(iVar14 + 0x1c) = uVar1;
        }
        FUN_0134f3b0();
        FUN_0122a090();
        uStack_40 = *(undefined4 *)(iVar9 + 0x10);
        uStack_2c = CONCAT44(puVar20,uVar16);
        iStack_3c = iVar12;
        iStack_38 = iVar13;
        piStack_34 = piVar17;
        piStack_30 = piVar18;
        FUN_013570a0();
        FUN_00f1a420();
        local_14 = local_14 & 0xffffff00;
        FUN_00f1a420();
        std::
        _Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
        ::operator++((_Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
                      *)&local_50);
        local_f4 = local_f4 + 1;
      } while ((int *)local_50 != (int *)local_4c);
    }
    FUN_0108fac0();
    FUN_017986fc();
  }
  ExceptionList = local_1c;
  return;
}


