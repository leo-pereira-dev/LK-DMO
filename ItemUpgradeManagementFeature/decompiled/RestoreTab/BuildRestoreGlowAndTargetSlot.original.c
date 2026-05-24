
/* ============================================================ */
/* Function: FUN_0126ae30 */
/* Entry: 0126ae30 */
/* Signature: undefined FUN_0126ae30() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0126ae30(int param_1)

{
  uint uVar1;
  void *pvVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  int *piVar6;
  float10 fVar7;
  undefined4 uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  undefined4 local_64;
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
  puStack_18 = &LAB_0199ba18;
  local_1c = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar1;
  pvVar2 = (void *)FUN_016830c0(0x2c,uVar1);
  local_14 = 0;
  if (pvVar2 == (void *)0x0) {
    iVar3 = 0;
  }
  else {
    memset(pvVar2,0,0x2c);
    iVar3 = FUN_010baa10();
  }
  local_14 = 0xffffffff;
  if (iVar3 != 0) {
    pvVar2 = (void *)FUN_016830c0(0xa0,uVar1);
    local_14 = 1;
    if (pvVar2 == (void *)0x0) {
      iVar3 = 0;
    }
    else {
      memset(pvVar2,0,0xa0);
      iVar3 = FUN_0123de10();
    }
    uVar10 = DAT_01bb9b34;
    uVar9 = DAT_01bb9b2c;
    local_14 = 0xffffffff;
    if (iVar3 != 0) {
      FUN_00f19f10();
      local_68 = 0;
      uStack_74 = 0;
      uStack_70 = 0;
      uStack_6c = 0;
      local_64 = 0xf;
      local_78 = 0;
      local_14 = 2;
      FUN_015010a0(&local_78,&DAT_01ae7290,&DAT_01bfbd40,s_EquipSystem_glow_bg_tga_01ae8b2c);
      FUN_0124ef30(&local_78);
      uVar8 = 0;
      uVar4 = FUN_0124f150(0,uVar9,uVar10);
      FUN_0123e330(0,0x10f,0x107,0x51,0x1c,uVar4,uVar8,uVar9,uVar10);
      local_14 = 0xffffffff;
      FUN_00f1a420();
      piVar5 = (int *)FUN_01230f40(iVar3,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
      *(int **)(param_1 + 0xcc) = piVar5;
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0x54))(1);
        (**(code **)(**(int **)(param_1 + 0xcc) + 0x40))();
      }
      local_44 = 0;
      local_40 = 7;
      uStack_50 = 0;
      uStack_4c = 0;
      uStack_48 = 0;
      local_54 = 0;
      local_28 = 0;
      local_14 = 3;
      local_34 = DAT_01bfe468 + 8;
      local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_30 = 10;
      local_58 = &DAT_3f7ae148;
      local_3c = 1;
      local_2c = 0;
      local_38 = 1;
      FUN_00f1a040();
      piVar5 = (int *)FUN_01230c40(&local_60,0x138,0x10f);
      *(int **)(param_1 + 0xd0) = piVar5;
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0x54))(1);
        (**(code **)(**(int **)(param_1 + 0xd0) + 0x40))();
      }
      local_34 = DAT_01bfe468 + 8;
      local_30 = 0xb;
      local_60 = CONCAT44(DAT_01b026e0,DAT_01b025d4);
      local_58 = (undefined1 *)0x3f800000;
      local_3c = 1;
      local_2c = 0;
      local_38 = 1;
      FUN_00f1a040();
      piVar5 = (int *)FUN_01230c40(&local_60,0x138,0xe3);
      *(int **)(param_1 + 0xc0) = piVar5;
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0x54))(1);
        (**(code **)(**(int **)(param_1 + 0xc0) + 0x40))();
      }
      pvVar2 = (void *)FUN_016830c0(0x1ac);
      local_14._0_1_ = 4;
      if (pvVar2 == (void *)0x0) {
        piVar5 = (int *)0x0;
      }
      else {
        memset(pvVar2,0,0x1ac);
        piVar5 = (int *)FUN_012435e0();
      }
      local_14 = CONCAT31(local_14._1_3_,3);
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0xc))
                  (*(undefined4 *)(param_1 + 0x40),DAT_01bee88c,DAT_01bee890,0x37,0x37,0);
        piVar6 = piVar5 + 0x2c;
        piVar5[0x37] = 0;
        piVar5[0x30] = 0;
        if (7 < (uint)piVar5[0x31]) {
          piVar6 = (int *)*piVar6;
        }
        *(undefined2 *)piVar6 = 0;
        FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
        FUN_01242970(s_EquipSystem_Upgrade_Slot_tga_01ae2c40);
        piVar5[0x35] = 0x3c23d70a;
        piVar5[0x3b] = 0;
        piVar5[0x3c] = 0;
        fVar7 = (float10)fmaxf(0,0x3f000000);
        piVar5[0x32] = (int)(float)fVar7;
        FUN_01243010();
        (**(code **)(*piVar5 + 0x48))();
        FUN_01242bf0();
        piVar5 = (int *)FUN_012311b0(piVar5,0x11d,0xac);
        *(int **)(param_1 + 0xc4) = piVar5;
        if (piVar5 != (int *)0x0) {
          (**(code **)(*piVar5 + 0x40))(1);
          (**(code **)(**(int **)(param_1 + 0xc4) + 0x54))();
        }
      }
      piVar5 = (int *)FUN_01231410(10,0,1,0x2a,0x2a,0x123,0xb2);
      *(int **)(param_1 + 0xbc) = piVar5;
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0x40))(0);
        (**(code **)(**(int **)(param_1 + 0xbc) + 0x54))();
        iVar3 = *(int *)(param_1 + 0xbc);
        *(undefined4 *)(iVar3 + 0x18) = 10;
        *(undefined4 *)(iVar3 + 0x1c) = 0;
        *(undefined4 *)(iVar3 + 0x20) = 1;
        *(undefined4 *)(iVar3 + 0x24) = 0;
      }
      local_34 = 0;
      FUN_00f1a330();
    }
  }
  ExceptionList = local_1c;
  return;
}


