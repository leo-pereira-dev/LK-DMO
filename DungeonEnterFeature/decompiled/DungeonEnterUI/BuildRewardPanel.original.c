
/* ============================================================ */
/* Function: FUN_010ca260 */
/* Entry: 010ca260 */
/* Signature: undefined FUN_010ca260() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_010ca260(int param_1)

{
  short sVar1;
  undefined4 *puVar2;
  uint uVar3;
  void *pvVar4;
  int *piVar5;
  short *psVar6;
  int iVar7;
  short *psVar8;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 uVar9;
  int local_58;
  void *local_54;
  undefined8 local_50;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01980e02;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar3;
  pvVar4 = (void *)FUN_016830c0(0x74,uVar3);
  local_8 = 0;
  if (pvVar4 == (void *)0x0) {
    local_58 = 0;
  }
  else {
    memset(pvVar4,0,0x74);
    local_58 = FUN_01245550();
  }
  local_8 = 0xffffffff;
  if (local_58 != 0) {
    *(int *)(local_58 + 0x48) = param_1;
    local_54 = *(void **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) {
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8 = 1;
    piVar5 = operator_new(0xc);
    local_8 = 0xffffffff;
    piVar5[2] = local_58;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar2 = *(undefined4 **)((int)local_54 + 4);
    *piVar5 = (int)local_54;
    piVar5[1] = (int)puVar2;
    *(int **)((int)local_54 + 4) = piVar5;
    *puVar2 = piVar5;
    FUN_012459a0(*(undefined4 *)(param_1 + 0x40),s_DungeonUI_Dungeon_Reward_Frame_t_01ad4b00,0,0,0,0
                 ,0x27c,0x1e2,0xfa,0x8e);
    local_54 = (void *)FUN_016830c0(0xa0,uVar3);
    local_8 = 2;
    if (local_54 == (void *)0x0) {
      local_54 = (void *)0x0;
    }
    else {
      memset(local_54,0,0xa0);
      local_54 = (void *)FUN_0123de10();
    }
    local_8 = 0xffffffff;
    FUN_0123e0d0(*(undefined4 *)(local_58 + 0x40),s_DungeonUI_Dungeon_Line_tga_01ad4a48,0,
                 DAT_01bb9b2c,DAT_01bb9b34,1,1,10,0xd9,0x2c);
    FUN_00f55430(&local_54);
    local_34 = 0;
    local_30 = 7;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    local_44 = 0;
    local_18 = 0;
    local_8 = 3;
    local_24 = DAT_01bfe468 + 8;
    local_20 = 0xb;
    local_50 = CONCAT44(DAT_01b02674,DAT_01b02634);
    local_48 = 0x3f76f6f7;
    local_2c = 1;
    local_1c = 0;
    local_28 = 0;
    FUN_00f19cc0(s_DUNGEON_ENTER_REWARD_TITLE_REWAR_01ad4b24,0x26);
    psVar6 = (short *)FUN_015ce480();
    local_8._0_1_ = 4;
    if (7 < *(uint *)(psVar6 + 10)) {
      psVar6 = *(short **)psVar6;
    }
    psVar8 = psVar6;
    do {
      sVar1 = *psVar8;
      psVar8 = psVar8 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar6,(int)psVar8 - (int)(psVar6 + 1) >> 1);
    local_8 = CONCAT31(local_8._1_3_,3);
    FUN_00f1a330();
    FUN_01246bb0(&local_50,1,0x19,0xd);
    local_8 = 0xffffffff;
    local_24 = 0;
    FUN_00f1a330();
    pvVar4 = (void *)FUN_016830c0(0xac);
    local_8 = 5;
    if (pvVar4 == (void *)0x0) {
      iVar7 = 0;
      uVar9 = extraout_ECX;
    }
    else {
      memset(pvVar4,0,0xac);
      iVar7 = FUN_01228950();
      uVar9 = extraout_ECX_00;
    }
    local_8 = 0xffffffff;
    *(int *)(param_1 + 0xb0) = iVar7;
    if (iVar7 != 0) {
      FUN_01228bd0(*(undefined4 *)(local_58 + 0x40),0x11,0x2a,0xda,0x62,5,5,0x2c,0x2c,0,2,uVar9,4);
      iVar7 = *(int *)(param_1 + 0xb0);
      *(undefined1 *)(iVar7 + 0xa9) = 0;
      if (*(int *)(iVar7 + 0x6c) != 0) {
        *(undefined1 *)(*(int *)(iVar7 + 0x6c) + 0x10) = 0;
        *(undefined4 *)(iVar7 + 0x6c) = 0;
      }
      *(undefined1 *)(*(int *)(param_1 + 0xb0) + 0x94) = 0;
      FUN_01228dd0(s_Icon_Mask_Over_dds_01ad4b4c,&DAT_01bee88c,*(int *)(param_1 + 0xb0) + 0x78);
      pvVar4 = (void *)FUN_016830c0(0xec);
      local_8 = 6;
      if (pvVar4 == (void *)0x0) {
        local_54 = (void *)0x0;
      }
      else {
        memset(pvVar4,0,0xec);
        local_54 = (void *)FUN_01247d90();
      }
      local_8 = 0xffffffff;
      if (local_54 != (void *)0x0) {
        FUN_01248090(5,0,3,0,DAT_01bee88c,DAT_01bee890,0x10,0x87,0x10,0xf,DAT_01bee88c,DAT_01bee890,
                     0xda,0x62);
        FUN_0122a200(local_54);
      }
      FUN_012472c0(*(undefined4 *)(param_1 + 0xb0));
    }
  }
  ExceptionList = local_10;
  return;
}


