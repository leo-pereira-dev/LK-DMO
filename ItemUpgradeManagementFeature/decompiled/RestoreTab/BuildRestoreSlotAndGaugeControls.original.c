
/* ============================================================ */
/* Function: FUN_012696e0 */
/* Entry: 012696e0 */
/* Signature: undefined FUN_012696e0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_012696e0(int param_1)

{
  undefined1 uVar1;
  void *pvVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 uVar9;
  int *piVar10;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 uVar11;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  float10 fVar12;
  undefined4 uVar13;
  undefined8 uVar14;
  int local_b8;
  undefined8 local_b4;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined1 local_90;
  undefined4 local_8c;
  int local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined8 local_78;
  undefined4 local_70;
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
  puStack_18 = &LAB_0199b87a;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  pvVar2 = (void *)FUN_016830c0(0x94,local_24);
  local_14 = 0;
  if (pvVar2 == (void *)0x0) {
    iVar3 = 0;
    uVar11 = extraout_ECX;
  }
  else {
    memset(pvVar2,0,0x94);
    iVar3 = FUN_0122b460();
    uVar11 = extraout_ECX_00;
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0xa4) = iVar3;
  if (iVar3 != 0) {
    FUN_0122b6f0(*(undefined4 *)(param_1 + 0x40),0x78,0x4e,0x37,0x37,0,uVar11);
    FUN_0126c890(8,param_1,FUN_0126c420,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0xa4));
    pvVar2 = (void *)FUN_016830c0(0x2c);
    local_14 = 1;
    if (pvVar2 == (void *)0x0) {
      iVar3 = 0;
    }
    else {
      memset(pvVar2,0,0x2c);
      iVar3 = FUN_010baa10();
    }
    uVar11 = DAT_01b025d4;
    if (iVar3 != 0) {
      local_98 = 0;
      local_94 = 7;
      uStack_a4 = 0;
      uStack_a0 = 0;
      uStack_9c = 0;
      local_a8 = 0;
      local_7c = 0;
      local_14 = 2;
      local_88 = DAT_01bfe468 + 8;
      local_b4 = CONCAT44(DAT_01b026e0,DAT_01b025d4);
      local_84 = 0xb;
      local_ac = 0x3f800000;
      local_90 = 1;
      local_80 = 0;
      local_8c = 1;
      FUN_00f1a040();
      piVar4 = (int *)FUN_01230c40(&local_b4,0x94,0x2c);
      *(int **)(param_1 + 0x98) = piVar4;
      if (piVar4 != (int *)0x0) {
        (**(code **)(*piVar4 + 0x54))(1);
      }
      pvVar2 = (void *)FUN_016830c0(0xa0);
      local_14._0_1_ = 3;
      if (pvVar2 == (void *)0x0) {
        iVar5 = 0;
      }
      else {
        memset(pvVar2,0,0xa0);
        iVar5 = FUN_0123de10();
      }
      uVar9 = DAT_01bb9b34;
      uVar14 = DAT_01bb9b2c;
      local_14._0_1_ = 2;
      uVar1 = (undefined1)local_14;
      local_14._0_1_ = 2;
      if (iVar5 != 0) {
        FUN_00f19f10();
        local_2c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_30 = 0;
        local_28 = 0xf;
        local_3c = 0;
        local_14._0_1_ = 4;
        FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_EquipSystem_Upgrade_Slot_tga_01ae2c40);
        FUN_0124ef30(&local_3c);
        uVar13 = 0;
        uVar6 = FUN_0124f150(0,uVar14,uVar9);
        FUN_0123e330(0,0x78,0x4e,0x37,0x37,uVar6,uVar13,uVar14,uVar9);
        local_14._0_1_ = 2;
        FUN_00f1a420();
        piVar4 = (int *)FUN_01230f40(iVar5,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        uVar1 = (undefined1)local_14;
        if (piVar4 != (int *)0x0) {
          (**(code **)(*piVar4 + 0x54))(1);
          uVar1 = (undefined1)local_14;
        }
      }
      local_14._0_1_ = uVar1;
      piVar4 = (int *)FUN_01231410(10,0,1,0x2a,0x2a,0x7e,0x54);
      *(int **)(param_1 + 0x94) = piVar4;
      if (piVar4 != (int *)0x0) {
        (**(code **)(*piVar4 + 0x54))(1);
      }
      pvVar2 = (void *)FUN_016830c0(0x1ac);
      local_14._0_1_ = 5;
      if (pvVar2 == (void *)0x0) {
        piVar4 = (int *)0x0;
      }
      else {
        memset(pvVar2,0,0x1ac);
        piVar4 = (int *)FUN_012435e0();
      }
      local_14 = CONCAT31(local_14._1_3_,2);
      if (piVar4 != (int *)0x0) {
        (**(code **)(*piVar4 + 0xc))
                  (*(undefined4 *)(param_1 + 0x40),DAT_01bee88c,DAT_01bee890,0x37,0x37,0);
        piVar10 = piVar4 + 0x2c;
        piVar4[0x37] = 0;
        piVar4[0x30] = 0;
        if (7 < (uint)piVar4[0x31]) {
          piVar10 = (int *)*piVar10;
        }
        *(undefined2 *)piVar10 = 0;
        FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
        FUN_01242970(s_EquipSystem_Upgrade_Slot_tga_01ae2c40);
        piVar4[0x35] = 0x3c23d70a;
        piVar4[0x3b] = 0;
        piVar4[0x3c] = 0;
        fVar12 = (float10)fmaxf(0,0x3f000000);
        piVar4[0x32] = (int)(float)fVar12;
        FUN_01243010();
        (**(code **)(*piVar4 + 0x48))();
        FUN_01242bf0();
        FUN_0126c890(1,param_1,FUN_0126c670,0);
        piVar4 = (int *)FUN_012311b0(piVar4,0x78,0x4e);
        *(int **)(param_1 + 0x9c) = piVar4;
        if (piVar4 != (int *)0x0) {
          (**(code **)(*piVar4 + 0x40))(1);
          (**(code **)(**(int **)(param_1 + 0x9c) + 0x54))();
          FUN_012695b0();
        }
      }
      puVar7 = (undefined4 *)FUN_016830c0(0x1c);
      if (puVar7 == (undefined4 *)0x0) {
        *(undefined4 *)(param_1 + 0xa0) = 0;
      }
      else {
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
        *(undefined4 **)(param_1 + 0xa0) = puVar7;
        FUN_0122b3e0(iVar3);
        iVar3 = *(int *)(param_1 + 0xa0);
        puVar8 = operator_new(0xc);
        *puVar8 = &DAT_01ae8cac;
        puVar8[1] = 0;
        puVar8[2] = 0;
        puVar7 = *(undefined4 **)(iVar3 + 8);
        if (puVar7 != (undefined4 *)0x0) {
          (**(code **)*puVar7)(1);
        }
        *(undefined4 **)(iVar3 + 8) = puVar8;
        local_b8 = *(int *)(param_1 + 0xa0);
        iVar3 = *(int *)(param_1 + 0xa4);
        if (local_b8 != 0) {
          puVar7 = *(undefined4 **)(iVar3 + 100);
          iVar5 = 0;
          for (puVar8 = (undefined4 *)*puVar7; puVar8 != puVar7; puVar8 = (undefined4 *)*puVar8) {
            if (puVar8[2] == local_b8) {
              if (iVar5 != -1) goto LAB_01269ce1;
              break;
            }
            iVar5 = iVar5 + 1;
          }
          if (*(int *)(iVar3 + 0x78) < *(int *)(local_b8 + 0x14)) {
            *(int *)(iVar3 + 0x78) = *(int *)(local_b8 + 0x14);
            puVar8 = (undefined4 *)*puVar7;
            if (puVar8 != puVar7) {
              do {
                *(undefined4 *)(puVar8[2] + 0x14) = *(undefined4 *)(iVar3 + 0x78);
                puVar8 = (undefined4 *)*puVar8;
              } while (puVar8 != *(undefined4 **)(iVar3 + 100));
            }
          }
          else {
            *(int *)(local_b8 + 0x14) = *(int *)(iVar3 + 0x78);
          }
          *(undefined4 *)(local_b8 + 0x14) = *(undefined4 *)(local_b8 + 0x14);
          FUN_00f55430(&local_b8);
          FUN_0122c760();
        }
      }
LAB_01269ce1:
      pvVar2 = (void *)FUN_016830c0(0x94);
      local_14._0_1_ = 6;
      if (pvVar2 == (void *)0x0) {
        iVar3 = 0;
        uVar9 = extraout_ECX_01;
      }
      else {
        memset(pvVar2,0,0x94);
        iVar3 = FUN_0122b460();
        uVar9 = extraout_ECX_02;
      }
      local_14._0_1_ = 2;
      uVar1 = (undefined1)local_14;
      local_14._0_1_ = 2;
      *(int *)(param_1 + 0xb8) = iVar3;
      if (iVar3 != 0) {
        FUN_0122b6f0(*(undefined4 *)(param_1 + 0x40),0x1c2,0x4e,0x37,0x37,0,uVar9);
        FUN_012472c0(*(undefined4 *)(param_1 + 0xb8));
        pvVar2 = (void *)FUN_016830c0(0x2c);
        local_14._0_1_ = 7;
        if (pvVar2 == (void *)0x0) {
          iVar3 = 0;
        }
        else {
          memset(pvVar2,0,0x2c);
          iVar3 = FUN_010baa10();
        }
        local_14._0_1_ = 2;
        uVar1 = (undefined1)local_14;
        if (iVar3 != 0) {
          local_5c = 0;
          local_58 = 7;
          uStack_68 = 0;
          uStack_64 = 0;
          uStack_60 = 0;
          local_6c = 0;
          local_40 = 0;
          local_14 = CONCAT31(local_14._1_3_,8);
          local_4c = DAT_01bfe468 + 8;
          local_78 = CONCAT44(DAT_01b026e0,uVar11);
          local_48 = 0xb;
          local_70 = 0x3f800000;
          local_54 = 1;
          local_44 = 0;
          local_50 = 1;
          FUN_00f1a040();
          piVar4 = (int *)FUN_01230c40(&local_78,0x1de,0x2c);
          *(int **)(param_1 + 0xac) = piVar4;
          if (piVar4 != (int *)0x0) {
            (**(code **)(*piVar4 + 0x54))(1);
          }
          pvVar2 = (void *)FUN_016830c0(0xa0);
          local_14._0_1_ = 9;
          if (pvVar2 == (void *)0x0) {
            iVar5 = 0;
          }
          else {
            memset(pvVar2,0,0xa0);
            iVar5 = FUN_0123de10();
          }
          uVar11 = DAT_01bb9b34;
          uVar14 = DAT_01bb9b2c;
          local_14._0_1_ = 8;
          uVar1 = (undefined1)local_14;
          local_14._0_1_ = 8;
          if (iVar5 != 0) {
            FUN_00f19f10();
            local_2c = 0;
            uStack_38 = 0;
            uStack_34 = 0;
            uStack_30 = 0;
            local_28 = 0xf;
            local_3c = 0;
            local_14._0_1_ = 10;
            FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,
                         s_EquipSystem_Upgrade_Slot_tga_01ae2c40);
            FUN_0124ef30(&local_3c);
            uVar6 = 0;
            uVar9 = FUN_0124f150(0,uVar14,uVar11);
            FUN_0123e330(0,0x1c2,0x4e,0x37,0x37,uVar9,uVar6,uVar14,uVar11);
            local_14._0_1_ = 8;
            FUN_00f1a420();
            piVar4 = (int *)FUN_01230f40(iVar5,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
            uVar1 = (undefined1)local_14;
            if (piVar4 != (int *)0x0) {
              (**(code **)(*piVar4 + 0x54))(1);
              uVar1 = (undefined1)local_14;
            }
          }
          local_14._0_1_ = uVar1;
          piVar4 = (int *)FUN_01231410(10,0,1,0x2a,0x2a,0x1c8,0x54);
          *(int **)(param_1 + 0xa8) = piVar4;
          if (piVar4 != (int *)0x0) {
            (**(code **)(*piVar4 + 0x54))(1);
          }
          pvVar2 = (void *)FUN_016830c0(0x1ac);
          local_14._0_1_ = 0xb;
          if (pvVar2 == (void *)0x0) {
            piVar4 = (int *)0x0;
          }
          else {
            memset(pvVar2,0,0x1ac);
            piVar4 = (int *)FUN_012435e0();
          }
          local_14._0_1_ = 8;
          if (piVar4 != (int *)0x0) {
            (**(code **)(*piVar4 + 0xc))
                      (*(undefined4 *)(param_1 + 0x40),DAT_01bee88c,DAT_01bee890,0x37,0x37,0);
            piVar10 = piVar4 + 0x2c;
            piVar4[0x37] = 0;
            piVar4[0x30] = 0;
            if (7 < (uint)piVar4[0x31]) {
              piVar10 = (int *)*piVar10;
            }
            *(undefined2 *)piVar10 = 0;
            FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
            FUN_01242970(s_EquipSystem_Upgrade_Slot_tga_01ae2c40);
            piVar4[0x35] = 0x3c23d70a;
            piVar4[0x3b] = 0;
            piVar4[0x3c] = 0;
            fVar12 = (float10)fmaxf(0,0x3f000000);
            piVar4[0x32] = (int)(float)fVar12;
            FUN_01243010();
            (**(code **)(*piVar4 + 0x48))();
            FUN_01242bf0();
            FUN_0126c890(1,param_1,FUN_0126c670,0);
            piVar4 = (int *)FUN_012311b0(piVar4,0x1c2,0x4e);
            *(int **)(param_1 + 0xb0) = piVar4;
            if (piVar4 != (int *)0x0) {
              (**(code **)(*piVar4 + 0x40))(1);
              (**(code **)(**(int **)(param_1 + 0xb0) + 0x54))();
              FUN_012695b0();
            }
          }
          puVar7 = (undefined4 *)FUN_016830c0(0x1c);
          if (puVar7 == (undefined4 *)0x0) {
            *(undefined4 *)(param_1 + 0xb4) = 0;
          }
          else {
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
            *(undefined4 **)(param_1 + 0xb4) = puVar7;
            FUN_0122b3e0(iVar3);
            iVar3 = *(int *)(param_1 + 0xb4);
            puVar8 = operator_new(0xc);
            *puVar8 = &DAT_01ae8cac;
            puVar8[1] = 0;
            puVar8[2] = 0;
            puVar7 = *(undefined4 **)(iVar3 + 8);
            if (puVar7 != (undefined4 *)0x0) {
              (**(code **)*puVar7)(1);
            }
            *(undefined4 **)(iVar3 + 8) = puVar8;
            local_b8 = *(int *)(param_1 + 0xb4);
            iVar3 = *(int *)(param_1 + 0xb8);
            if (local_b8 != 0) {
              iVar5 = 0;
              puVar7 = *(undefined4 **)(iVar3 + 100);
              for (puVar8 = (undefined4 *)*puVar7; puVar8 != puVar7; puVar8 = (undefined4 *)*puVar8)
              {
                if (puVar8[2] == local_b8) {
                  if (iVar5 != -1) goto LAB_0126a228;
                  break;
                }
                iVar5 = iVar5 + 1;
              }
              if (*(int *)(iVar3 + 0x78) < *(int *)(local_b8 + 0x14)) {
                *(int *)(iVar3 + 0x78) = *(int *)(local_b8 + 0x14);
                puVar8 = (undefined4 *)*puVar7;
                if (puVar8 != puVar7) {
                  do {
                    *(undefined4 *)(puVar8[2] + 0x14) = *(undefined4 *)(iVar3 + 0x78);
                    puVar8 = (undefined4 *)*puVar8;
                  } while (puVar8 != *(undefined4 **)(iVar3 + 100));
                }
              }
              else {
                *(int *)(local_b8 + 0x14) = *(int *)(iVar3 + 0x78);
              }
              FUN_00f55430(&local_b8);
              FUN_0122c760();
            }
          }
LAB_0126a228:
          local_4c = 0;
          FUN_00f1a330();
          uVar1 = (undefined1)local_14;
        }
      }
      local_14._0_1_ = uVar1;
      local_88 = 0;
      FUN_00f1a330();
    }
  }
  ExceptionList = local_1c;
  return;
}


