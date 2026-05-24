
/* ============================================================ */
/* Function: FUN_010cad70 */
/* Entry: 010cad70 */
/* Signature: undefined FUN_010cad70() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_010cad70(int param_1,ushort *param_2)

{
  ushort uVar1;
  undefined4 *puVar2;
  code *pcVar3;
  undefined1 uVar4;
  int iVar5;
  uint uVar6;
  void *pvVar7;
  undefined4 uVar8;
  undefined4 *puVar9;
  int *piVar10;
  undefined4 ****ppppuVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 local_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 local_6c;
  undefined4 local_68;
  int local_64;
  uint local_60;
  int local_5c;
  int local_58;
  uint local_54;
  undefined4 ***local_50 [4];
  int local_40;
  uint local_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined8 local_28;
  undefined1 local_20 [4];
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01980f5d;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if (*(int *)(param_1 + 0xb4) != 0) {
    local_64 = param_1;
    FUN_01236650(local_14);
    iVar5 = *(int *)(param_1 + 0xa8);
    local_54 = (uint)*param_2;
    FUN_00f48220(local_20,&local_54);
    if ((((*(char *)(local_1c._4_4_ + 0xd) == '\0') &&
         (*(ushort *)(local_1c._4_4_ + 0x10) <= *param_2)) &&
        (local_1c._4_4_ != *(int *)(iVar5 + 0xc))) &&
       ((*(int *)(iVar5 + 0x98) != 0 &&
        (local_5c = FUN_015db910(local_1c._4_4_ + 0x14), local_5c != 0)))) {
      uVar1 = *param_2;
      local_54 = (uint)uVar1;
      FUN_00f48220(local_20,&local_54);
      if ((*(char *)(local_1c._4_4_ + 0xd) == '\0') &&
         ((*(ushort *)(local_1c._4_4_ + 0x10) <= uVar1 &&
          (local_1c._4_4_ != *(int *)(*(int *)(param_1 + 0xa8) + 0xc))))) {
        iVar5 = FUN_01036690(local_1c._4_4_ + 0x14);
        local_58 = CONCAT31(local_58._1_3_,iVar5 != 0);
      }
      else {
        local_58 = CONCAT31(local_58._1_3_,1);
      }
      if (*(int **)(param_1 + 0xb8) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0xb8) + 0x1c))(local_58);
      }
      uVar6 = FUN_010cb260(1);
      local_58 = local_5c;
      local_54 = uVar6;
      if (uVar6 != 0) {
        local_1c = 0;
        pvVar7 = operator_new(0x24);
        *(void **)pvVar7 = pvVar7;
        *(void **)((int)pvVar7 + 4) = pvVar7;
        local_1c = CONCAT44(local_1c._4_4_,pvVar7);
        local_8 = 0;
        FUN_010cb8e0(local_5c + 0x10,&local_1c);
        if (local_1c._4_4_ == 0) {
          uVar17 = 1;
          uVar8 = 0;
          uVar12 = 0;
          uVar13 = 0;
          uVar14 = 0;
          uVar15 = 0;
          uVar16 = 0;
          FUN_00f19cc0(s_DUNGEON_ENTER_REQUIRE_TXT_EMPTY__01ad4c10,0x27);
          uVar8 = FUN_015ce480(local_50,uVar8,uVar12,uVar13,uVar14,uVar15,uVar16);
          local_8 = CONCAT31(local_8._1_3_,1);
          FUN_010cb670(uVar6,uVar8,uVar17);
          FUN_00f1a330();
          local_58 = local_5c;
        }
        else {
          local_58 = local_5c;
          piVar10 = (int *)*(int *)local_1c;
          if (piVar10 != (int *)local_1c) {
            do {
              FUN_010cb670(local_54,piVar10 + 2,(char)piVar10[8]);
              piVar10 = (int *)*piVar10;
            } while (piVar10 != (int *)local_1c);
          }
        }
        if (*(int *)(local_54 + 0x24) != 0) {
          *(undefined1 *)(local_54 + 0x1a) = 1;
        }
        local_8 = 0xffffffff;
        *(undefined4 *)((int *)local_1c)[1] = 0;
        puVar9 = (undefined4 *)*(int *)local_1c;
        while (puVar9 != (undefined4 *)0x0) {
          puVar2 = (undefined4 *)*puVar9;
          FUN_00f1a330();
          FUN_017986fc(puVar9,0x24);
          puVar9 = puVar2;
        }
        FUN_017986fc((int *)local_1c,0x24);
        param_1 = local_64;
      }
      local_60 = FUN_010cb260(2);
      if (local_60 != 0) {
        piVar10 = (int *)(local_58 + 4);
        local_1c = CONCAT44(local_58 + 8,(int *)local_1c);
        uVar4 = FUN_010367e0(piVar10,local_58 + 8);
        local_54 = CONCAT31(local_54._1_3_,uVar4);
        uVar8 = 0;
        uVar12 = 0;
        uVar13 = 0;
        uVar14 = 0;
        uVar15 = 0;
        uVar16 = 0;
        FUN_00f19cc0(s_TOOLTIP_ITEM_NAME_COUNT_01ad4c38,0x17);
        FUN_015ce480(&local_38,uVar8,uVar12,uVar13,uVar14,uVar15,uVar16);
        local_8 = 2;
        local_5c = *piVar10;
        FUN_010369a0(local_50,&local_5c);
        local_8._0_1_ = 3;
        if (local_40 == 0) {
          local_54 = CONCAT31(local_54._1_3_,1);
          uVar8 = 0;
          uVar12 = 0;
          uVar13 = 0;
          uVar14 = 0;
          uVar15 = 0;
          uVar16 = 0;
          FUN_00f19cc0(s_DUNGEON_ENTER_REQUIRE_TXT_EMPTY__01ad4c10,0x27);
          puVar9 = (undefined4 *)FUN_015ce480(&local_7c,uVar8,uVar12,uVar13,uVar14,uVar15,uVar16);
          if (&local_38 != puVar9) {
            FUN_00f1a330();
            local_38 = *puVar9;
            uStack_34 = puVar9[1];
            uStack_30 = puVar9[2];
            uStack_2c = puVar9[3];
            local_28 = *(undefined8 *)(puVar9 + 4);
            puVar9[4] = 0;
            puVar9[5] = 7;
            *(undefined2 *)puVar9 = 0;
          }
        }
        else {
          ppppuVar11 = local_50;
          if (7 < local_3c) {
            ppppuVar11 = (undefined4 ****)local_50[0];
          }
          local_7c = 0;
          uStack_78 = 0;
          uStack_74 = 0;
          uStack_70 = 0;
          local_6c = 0;
          local_68 = 0;
          FUN_00f1a200(u__Name__01acb254,6);
          local_8._0_1_ = 4;
          FUN_01501b00(&local_38,&local_7c,ppppuVar11);
          local_8._0_1_ = 3;
          FUN_00f1a330();
          local_6c = 0;
          local_68 = 0;
          local_7c = 0;
          uStack_78 = 0;
          uStack_74 = 0;
          uStack_70 = 0;
          FUN_00f1a200(u__Count__01ace788,7);
          local_8._0_1_ = 5;
          FUN_01501780(&local_38,&local_7c,local_1c._4_4_);
          local_8._0_1_ = 3;
        }
        FUN_00f1a330();
        uVar6 = local_60;
        FUN_010cb670(local_60,&local_38,local_54);
        if (*(int *)(uVar6 + 0x24) != 0) {
          *(undefined1 *)(uVar6 + 0x1a) = 1;
        }
        FUN_00f1a330();
        local_8 = 0xffffffff;
        FUN_00f1a330();
        param_1 = local_64;
      }
      iVar5 = FUN_010cb260(0);
      if ((iVar5 != 0) && (FUN_010cb670(iVar5,local_58 + 0x18,1), *(int *)(iVar5 + 0x24) != 0)) {
        *(undefined1 *)(iVar5 + 0x1a) = 1;
      }
      FUN_01237670();
      iVar5 = *(int *)(param_1 + 0xa8);
      local_60 = (uint)*param_2;
      FUN_00f48220(local_20,&local_60);
      if (((((*(char *)(local_1c._4_4_ + 0xd) == '\0') &&
            (*(ushort *)(local_1c._4_4_ + 0x10) <= *param_2)) &&
           (local_1c._4_4_ != *(int *)(iVar5 + 0xc))) &&
          ((*(int *)(iVar5 + 0x98) != 0 && (iVar5 = FUN_015d17e0(local_1c._4_4_ + 0x14), iVar5 != 0)
           ))) && (*(int **)(param_1 + 0xcc) != (int *)0x0)) {
        pcVar3 = *(code **)(**(int **)(param_1 + 0xcc) + 0x10);
        local_60 = (uint)*(short *)(iVar5 + 10);
        puVar9 = (undefined4 *)FUN_010ccb60(&local_7c,&local_60);
        local_8 = 6;
        if (7 < (uint)puVar9[5]) {
          puVar9 = (undefined4 *)*puVar9;
        }
        (*pcVar3)(puVar9);
        FUN_00f1a330();
      }
    }
  }
  ExceptionList = local_10;
  return;
}


