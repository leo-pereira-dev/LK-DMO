
/* ============================================================ */
/* Function: FUN_010ce810 */
/* Entry: 010ce810 */
/* Signature: undefined FUN_010ce810() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_010ce810(int param_1)

{
  short sVar1;
  void *pvVar2;
  short *psVar3;
  int iVar4;
  short *psVar5;
  undefined4 extraout_ECX;
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
  puStack_c = &LAB_019815f3;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_54 = (void *)FUN_016830c0(0xa0,local_14);
  local_8 = 0;
  if (local_54 == (void *)0x0) {
    pvVar2 = (void *)0x0;
  }
  else {
    memset(local_54,0,0xa0);
    pvVar2 = (void *)FUN_0123de10();
  }
  local_8 = 0xffffffff;
  FUN_0123e0d0(*(undefined4 *)(param_1 + 0x40),s_DungeonUI_Dungeon_list_L_png_01ad505c,0,
               DAT_01bb9b2c,DAT_01bb9b34,1,0x169,0x23,0xd1,0x1e);
  local_54 = pvVar2;
  FUN_00f55430(&local_54);
  local_34 = 0;
  local_30 = 7;
  uStack_40 = 0;
  uStack_3c = 0;
  uStack_38 = 0;
  local_44 = 0;
  local_18 = 0;
  local_8 = 1;
  local_24 = DAT_01bfe468 + 8;
  local_50 = DAT_01bb9b2c;
  local_20 = 0xb;
  local_48 = DAT_01bb9b34;
  local_2c = 1;
  local_1c = 0;
  local_28 = 5;
  FUN_00f19cc0(s_DUNGEON_ENTER_CLEAR_REWARD_ITEM__01ad50bc,0x2a);
  psVar3 = (short *)FUN_015ce480();
  local_8._0_1_ = 2;
  if (7 < *(uint *)(psVar3 + 10)) {
    psVar3 = *(short **)psVar3;
  }
  psVar5 = psVar3;
  do {
    sVar1 = *psVar5;
    psVar5 = psVar5 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar3,(int)psVar5 - (int)(psVar3 + 1) >> 1);
  local_8 = CONCAT31(local_8._1_3_,1);
  FUN_00f1a330();
  FUN_01246bb0(&local_50,1,0x1d1,0x32);
  local_24 = 0;
  FUN_00f1a330();
  local_34 = 0;
  local_30 = 7;
  uStack_40 = 0;
  uStack_3c = 0;
  uStack_38 = 0;
  local_44 = 0;
  local_18 = 0;
  local_8 = 3;
  local_24 = DAT_01bfe468 + 8;
  local_20 = 9;
  local_50 = DAT_01bb9b2c;
  local_48 = DAT_01bb9b34;
  local_2c = 1;
  local_1c = 0;
  local_28 = 5;
  FUN_00f1a040(&DAT_01ac59a8,1);
  iVar4 = FUN_01246bb0(&local_50,1,0x1d1,0x61);
  *(int *)(param_1 + 0x88) = iVar4;
  if (iVar4 != 0) {
    (**(code **)(*(int *)(iVar4 + 0x14) + 0x1c))(0);
  }
  local_8 = 0xffffffff;
  local_24 = 0;
  FUN_00f1a330();
  local_54 = (void *)FUN_016830c0(0xac);
  local_8 = 4;
  if (local_54 == (void *)0x0) {
    iVar4 = 0;
  }
  else {
    memset(local_54,0,0xac);
    iVar4 = FUN_01228950();
  }
  local_8 = 0xffffffff;
  *(int *)(param_1 + 0x84) = iVar4;
  if (iVar4 != 0) {
    FUN_012472c0(iVar4);
    FUN_01228bd0(*(undefined4 *)(param_1 + 0x40),0x16d,0x78,0xc9,0xd6,5,5,0x20,0x20,0,2,extraout_ECX
                 ,5);
    local_54 = (void *)FUN_016830c0(0xec);
    local_8 = 5;
    if (local_54 == (void *)0x0) {
      iVar4 = 0;
    }
    else {
      memset(local_54,0,0xec);
      iVar4 = FUN_01247d90();
    }
    local_8 = 0xffffffff;
    if (iVar4 != 0) {
      FUN_01248090(5,0,4,0,DAT_01bee88c,DAT_01bee890,0x10,0xfb,0x10,0xf,DAT_01bee88c,DAT_01bee890,
                   0xc9,0xd6);
      FUN_0122a200(iVar4);
    }
  }
  ExceptionList = local_10;
  return;
}


