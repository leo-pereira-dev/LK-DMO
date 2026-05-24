
/* ============================================================ */
/* Function: FUN_010cb8e0 */
/* Entry: 010cb8e0 */
/* Signature: undefined FUN_010cb8e0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_010cb8e0(int param_1,int *param_2,undefined4 *param_3)

{
  short sVar1;
  int ***pppiVar2;
  int iVar3;
  uint uVar4;
  undefined4 *puVar5;
  int *****pppppiVar6;
  uint uVar7;
  int ****ppppiVar8;
  int *****pppppiVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  int *****pppppiVar16;
  undefined1 local_a8 [28];
  undefined4 *local_8c;
  int ***local_88;
  int local_84;
  undefined4 *local_80;
  int ****local_7c;
  uint local_78;
  int ***local_74;
  int ****local_70;
  int ****local_6c;
  uint local_68;
  int ***local_64;
  int ***local_60;
  int ***pppiStack_5c;
  int ***pppiStack_58;
  int ***pppiStack_54;
  undefined8 local_50;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined8 local_38;
  undefined1 local_30;
  int ****local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  uint uStack_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_019810d4;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_80 = param_3;
  local_78 = 0;
  local_68 = 0;
  pppppiVar6 = (int *****)*param_2;
  pppppiVar9 = (int *****)*pppppiVar6;
  local_84 = param_1;
  local_7c = (int ****)pppppiVar6;
  local_14 = uVar4;
  if (pppppiVar9 != pppppiVar6) {
    do {
      local_38 = 0x700000000;
      uStack_44 = 0;
      uStack_40 = 0;
      uStack_3c = 0;
      local_48 = 0;
      local_30 = 0;
      local_8 = 0;
      sVar1 = *(short *)(pppppiVar9 + 3);
      local_6c = (int ****)pppppiVar9;
      if (sVar1 == 1) {
        local_30 = FUN_01036860(pppppiVar9 + 4);
        iVar3 = local_84;
        local_74 = (int ***)pppppiVar9[4];
        ppppiVar8 = (int ****)*local_74;
        pppppiVar6 = (int *****)local_7c;
        if (ppppiVar8 != (int ****)local_74) {
          do {
            local_70 = (int ****)ppppiVar8[2];
            if (*(int *)(*(int *)(iVar3 + 0xa8) + 0xa4) == 0) {
              local_1c = 0;
              uStack_18 = 0;
              local_2c = (int ****)0x0;
              uStack_28 = 0;
              uStack_24 = 0;
              uStack_20 = 0;
              FUN_00f1a200(&DAT_01ac5dd8,0);
              pppppiVar6 = &local_2c;
              uVar7 = local_68 | 2;
            }
            else {
              pppppiVar6 = (int *****)FUN_015ef010(local_a8,&local_70);
              uVar7 = local_68 | 1;
            }
            local_78 = uVar7 | 4;
            local_60 = (int ***)*pppppiVar6;
            pppiStack_5c = (int ***)pppppiVar6[1];
            pppiStack_58 = (int ***)pppppiVar6[2];
            pppiStack_54 = (int ***)pppppiVar6[3];
            local_50 = *(undefined8 *)(pppppiVar6 + 4);
            pppppiVar6[4] = (int ****)0x0;
            pppppiVar6[5] = (int ****)0x7;
            *(undefined2 *)pppppiVar6 = 0;
            local_68 = local_78;
            if ((uVar7 & 2) != 0) {
              local_78 = uVar7 & 0xfffffffd | 4;
              local_68 = local_78;
              FUN_00f1a330();
            }
            local_8 = 0;
            if ((local_68 & 1) != 0) {
              local_78 = local_68 & 0xfffffffe;
              local_68 = local_78;
              FUN_00f1a330();
            }
            local_8 = CONCAT31(local_8._1_3_,2);
            local_64 = DAT_01bfb258;
            uVar10 = 0;
            uVar11 = 0;
            uVar12 = 0;
            uVar13 = 0;
            uVar14 = 0;
            uVar15 = 0;
            FUN_00f19cc0(s_DUNGEON_ENTER_REQUIRE_TXT_QUEST_01ad4cc8,0x1f);
            local_64 = (int ***)FUN_015ce480(local_a8,uVar10,uVar11,uVar12,uVar13,uVar14,uVar15);
            if ((int ***)&local_48 != local_64) {
              FUN_00f1a330();
              local_48 = *local_64;
              uStack_44 = local_64[1];
              uStack_40 = local_64[2];
              uStack_3c = local_64[3];
              local_38 = *(undefined8 *)(local_64 + 4);
              local_64[4] = (int **)0x0;
              local_64[5] = (int **)0x7;
              *(undefined2 *)local_64 = 0;
            }
            FUN_00f1a330();
            local_64 = (int ***)&local_60;
            if (7 < local_50._4_4_) {
              local_64 = local_60;
            }
            local_2c = (int ****)0x0;
            uStack_28 = 0;
            uStack_24 = 0;
            uStack_20 = 0;
            local_1c = 0;
            uStack_18 = 0;
            FUN_00f1a200(u__Quest__01ad4ce8,7);
            local_8._0_1_ = 3;
            FUN_01501b00(&local_48,&local_2c,local_64);
            FUN_00f1a330();
            local_8 = (uint)local_8._1_3_ << 8;
            FUN_00f1a330();
            ppppiVar8 = (int ****)*ppppiVar8;
            pppppiVar6 = (int *****)local_7c;
            pppppiVar9 = (int *****)local_6c;
          } while (ppppiVar8 != (int ****)local_74);
        }
LAB_010cbc65:
        local_64 = (int ***)*local_80;
        if (local_80[1] == 0x71c71c7) {
                    /* WARNING: Subroutine does not return */
          std::_Xlength_error(s_list_too_long_01abdaa4);
        }
        local_8c = local_80;
        local_8 = CONCAT31(local_8._1_3_,8);
        local_88 = (int ***)0x0;
        local_88 = operator_new(0x24);
        local_74 = local_88;
        FUN_00f221a0(&local_48);
        local_88 = (int ***)0x0;
        *(undefined1 *)(local_74 + 8) = local_30;
        local_80[1] = local_80[1] + 1;
        pppiVar2 = (int ***)local_64[1];
        *local_74 = (int **)local_64;
        local_74[1] = (int **)pppiVar2;
        local_64[1] = (int **)local_74;
        *pppiVar2 = (int **)local_74;
      }
      else {
        if (sVar1 == 2) {
          local_30 = FUN_010368e0(pppppiVar9 + 4);
          FUN_010cc950(&local_2c,pppppiVar9 + 4);
          local_8._0_1_ = 4;
          local_6c = (int ****)&local_2c;
          if (7 < uStack_18) {
            local_6c = local_2c;
          }
          local_70 = (int ****)DAT_01bfb258;
          uVar10 = 0;
          uVar11 = 0;
          uVar12 = 0;
          uVar13 = 0;
          uVar14 = 0;
          uVar15 = 0;
          FUN_00f19cc0(s_DUNGEON_ENTER_REQUIRE_TXT_TAMER__01ad4cf8,0x25);
          puVar5 = (undefined4 *)FUN_015ce480(&local_60,uVar10,uVar11,uVar12,uVar13,uVar14,uVar15);
          local_8._0_1_ = 5;
          pppppiVar16 = (int *****)local_6c;
          if (7 < (uint)puVar5[5]) {
            puVar5 = (undefined4 *)*puVar5;
          }
LAB_010cba57:
          FUN_01500f80(&local_48,u__s__s_01acfd54,puVar5,pppppiVar16);
          FUN_00f1a330();
          local_8 = (uint)local_8._1_3_ << 8;
          FUN_00f1a330();
          goto LAB_010cbc65;
        }
        if (sVar1 == 3) {
          local_30 = FUN_01036940(pppppiVar9 + 4);
          FUN_010cc950(&local_2c,pppppiVar9 + 4);
          local_8._0_1_ = 6;
          local_70 = (int ****)&local_2c;
          if (7 < uStack_18) {
            local_70 = local_2c;
          }
          local_6c = (int ****)DAT_01bfb258;
          uVar10 = 0;
          uVar11 = 0;
          uVar12 = 0;
          uVar13 = 0;
          uVar14 = 0;
          uVar15 = 0;
          FUN_00f19cc0(s_DUNGEON_ENTER_REQUIRE_TXT_DIGIMO_01ad4d20,0x27);
          puVar5 = (undefined4 *)FUN_015ce480(&local_60,uVar10,uVar11,uVar12,uVar13,uVar14,uVar15);
          local_8._0_1_ = 7;
          pppppiVar16 = (int *****)local_70;
          if (7 < (uint)puVar5[5]) {
            puVar5 = (undefined4 *)*puVar5;
          }
          goto LAB_010cba57;
        }
      }
      local_8 = 0xffffffff;
      FUN_00f1a330(uVar4);
      pppppiVar9 = (int *****)*pppppiVar9;
    } while (pppppiVar9 != pppppiVar6);
  }
  ExceptionList = local_10;
  return;
}


