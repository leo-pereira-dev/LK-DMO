
/* ============================================================ */
/* Function: FUN_010cbd20 */
/* Entry: 010cbd20 */
/* Signature: undefined FUN_010cbd20() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_010cbd20(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  void *pvVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  undefined4 uVar11;
  undefined8 uVar12;
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
  int local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198116b;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar2;
  pvVar3 = (void *)FUN_016830c0(0x74,uVar2);
  local_14 = 0;
  if (pvVar3 == (void *)0x0) {
    iVar4 = 0;
  }
  else {
    memset(pvVar3,0,0x74);
    iVar4 = FUN_01245550();
  }
  local_14 = 0xffffffff;
  if (iVar4 != 0) {
    *(int *)(iVar4 + 0x48) = param_1;
    iVar6 = *(int *)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) {
LAB_010cc1ff:
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_14 = 1;
    piVar5 = operator_new(0xc);
    local_14 = 0xffffffff;
    piVar5[2] = iVar4;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar10 = *(undefined4 **)(iVar6 + 4);
    *piVar5 = iVar6;
    piVar5[1] = (int)puVar10;
    *(int **)(iVar6 + 4) = piVar5;
    *puVar10 = piVar5;
    FUN_012459a0(*(undefined4 *)(param_1 + 0x40),s_DungeonUI_Dungeon_Image_Frame_tg_01ad4d48,0,0,0,0
                 ,0x11f,0x68,600,0x172);
    pvVar3 = (void *)FUN_016830c0(0xa0,uVar2);
    local_14 = 2;
    if (pvVar3 == (void *)0x0) {
      iVar6 = 0;
    }
    else {
      memset(pvVar3,0,0xa0);
      iVar6 = FUN_0123de10();
    }
    local_14 = 0xffffffff;
    *(int *)(param_1 + 0xbc) = iVar6;
    uVar8 = DAT_01bb9b34;
    uVar12 = DAT_01bb9b2c;
    if (iVar6 != 0) {
      uVar1 = *(undefined4 *)(iVar4 + 0x40);
      FUN_00f19f10();
      local_2c = 0;
      uStack_38 = 0;
      uStack_34 = 0;
      uStack_30 = 0;
      local_28 = 0xf;
      local_3c = 0;
      local_14 = 3;
      FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,&DAT_01abd9ec);
      FUN_0124ef30(&local_3c);
      uVar11 = 0;
      uVar7 = FUN_0124f150(0,uVar12,uVar8);
      FUN_0123e330(uVar1,0xb,0xe,0x243,0x15d,uVar7,uVar11,uVar12,uVar8);
      local_14 = 0xffffffff;
      FUN_00f1a420();
      FUN_012472c0(*(undefined4 *)(param_1 + 0xbc));
    }
    FUN_01246720(s_DungeonUI_Dungeon_name_Frame_tga_01ad4d6c,1,0xb,0xe,0xfe,0x3f);
    local_5c = 0;
    local_58 = 7;
    uStack_68 = 0;
    uStack_64 = 0;
    uStack_60 = 0;
    local_6c = 0;
    local_40 = 0;
    local_14 = 4;
    local_4c = DAT_01bfe468 + 8;
    local_48 = 0x13;
    local_78 = DAT_01bb9b2c;
    local_70 = DAT_01bb9b34;
    local_54 = 1;
    local_44 = 0;
    local_50 = 0;
    FUN_00f1a040();
    uVar8 = FUN_01246bb0(&local_78,1,0x15,0x22);
    local_14 = 0xffffffff;
    *(undefined4 *)(param_1 + 0xc4) = uVar8;
    local_4c = 0;
    FUN_00f1a330();
    uVar8 = FUN_01246720(s_DungeonUI_Dungeon_Level_Easy_tga_01ad4d90,1,0xb,0x4d,0x67,0x1d);
    *(undefined4 *)(param_1 + 0xc0) = uVar8;
    iVar4 = FUN_012462c0(s_DungeonUI_Dungeon_reward_btn_png_01ad4db4,0x17,1,0x221,0x14,0x27,0x21,0,
                         0x21);
    *(int *)(param_1 + 0xe0) = iVar4;
    if (iVar4 != 0) {
      FUN_010ccf80(1,param_1,FUN_010cce60,0);
      iVar4 = *(int *)(param_1 + 0xe0);
      puVar9 = operator_new(8);
      *puVar9 = &DAT_01ad4eac;
      puVar9[1] = 0;
      puVar10 = *(undefined4 **)(iVar4 + 0x34);
      if (puVar10 != (undefined4 *)0x0) {
        (**(code **)*puVar10)(1);
      }
      *(undefined4 **)(iVar4 + 0x34) = puVar9;
      (**(code **)(**(int **)(param_1 + 0xe0) + 0x18))(0);
      puVar10 = (undefined4 *)FUN_016830c0(0x90);
      local_14 = 5;
      if (puVar10 == (undefined4 *)0x0) {
        puVar10 = (undefined4 *)0x0;
      }
      else {
        memset(puVar10,0,0x90);
        FUN_01245550();
        puVar10[0x1d] = &DAT_01ad41f8;
        puVar10[0x1e] = 0;
        iVar4 = DAT_01bfe470;
        local_14 = CONCAT31(local_14._1_3_,7);
        *puVar10 = &DAT_01ad51d0;
        puVar10[0x1d] = &DAT_01ad51c4;
        puVar10[0x1f] = 0;
        puVar10[0x20] = 0;
        puVar10[0x21] = 0;
        puVar10[0x22] = 0;
        puVar10[0x23] = 0;
        if (iVar4 != 0) {
          FUN_00f1d720();
          if (((*(char *)(local_28 + 0xd) == '\0') && (*(int *)(local_28 + 0x10) < 0x2f)) &&
             (local_28 != *(int *)(iVar4 + 4))) {
            uVar8 = *(undefined4 *)(local_28 + 0x14);
          }
          else {
            uVar8 = 0;
          }
          uVar8 = __RTDynamicCast(uVar8,0,&DAT_01bc3cf8,&DAT_01bd4280,0);
          puVar10[0x1e] = uVar8;
        }
      }
      local_14 = 0xffffffff;
      *(undefined4 **)(param_1 + 0xe4) = puVar10;
      if (puVar10 != (undefined4 *)0x0) {
        FUN_010cd1d0();
        iVar4 = *(int *)(param_1 + 0xe4);
        if (iVar4 != 0) {
          *(int *)(iVar4 + 0x48) = param_1;
          iVar6 = *(int *)(param_1 + 0x58);
          if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_010cc1ff;
          local_14 = 8;
          piVar5 = operator_new(0xc);
          piVar5[2] = iVar4;
          *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
          puVar10 = *(undefined4 **)(iVar6 + 4);
          *piVar5 = iVar6;
          piVar5[1] = (int)puVar10;
          *(int **)(iVar6 + 4) = piVar5;
          *puVar10 = piVar5;
        }
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


