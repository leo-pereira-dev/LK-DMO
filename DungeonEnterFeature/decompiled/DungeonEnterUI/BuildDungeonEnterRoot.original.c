
/* ============================================================ */
/* Function: FUN_010c8fa0 */
/* Entry: 010c8fa0 */
/* Signature: undefined FUN_010c8fa0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_010c8fa0(int param_1)

{
  char cVar1;
  short sVar2;
  int iVar3;
  short *psVar4;
  short *psVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined1 local_6c [24];
  undefined4 local_54;
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
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0197f785;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  cVar1 = *(char *)(param_1 + 0x74);
  *(undefined2 *)(param_1 + 0x74) = 0x101;
  if (cVar1 == '\0') {
    FUN_012457d0(s_DungeonUI_Dungeon_Bg_tga_01ad49f0,0,3,3,0,DAT_01bc07ec + -900 >> 1,
                 DAT_01bc07f0 + -0x2cb >> 1,900,0x2cb);
    iVar3 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,0x35e,0x24,0x1e,0x1c,0,0x1c);
    *(int *)(param_1 + 0xd4) = iVar3;
    if (iVar3 != 0) {
      FUN_010ccf80(1,param_1,&LAB_010ccd00,0);
    }
    iVar3 = FUN_012462c0(s_DungeonUI_Dungeon_Entrance_Btn_t_01ad438c,0x17,1,0x2d0,0x28a,0xa6,0x32,0,
                         0x32);
    *(int *)(param_1 + 0xd0) = iVar3;
    if (iVar3 != 0) {
      local_34 = 0;
      local_30 = 7;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_44 = 0;
      local_18 = 0;
      local_8 = 0;
      local_24 = DAT_01bfe468 + 8;
      local_50 = DAT_01bb9b2c;
      local_20 = 0xf;
      local_48 = DAT_01bb9b34;
      local_2c = 1;
      local_1c = 0;
      local_28 = 1;
      uVar6 = 0;
      uVar7 = 0;
      uVar8 = 0;
      uVar9 = 0;
      uVar10 = 0;
      uVar11 = 0;
      FUN_00f19cc0(s_DUNGEON_ENTER_BTN_ENTER_01ad4a0c,0x17);
      psVar4 = (short *)FUN_015ce480(local_6c,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
      local_8._0_1_ = 1;
      if (7 < *(uint *)(psVar4 + 10)) {
        psVar4 = *(short **)psVar4;
      }
      psVar5 = psVar4;
      do {
        sVar2 = *psVar5;
        psVar5 = psVar5 + 1;
      } while (sVar2 != 0);
      FUN_00f1a040(psVar4,(int)psVar5 - (int)(psVar4 + 1) >> 1);
      local_8 = (uint)local_8._1_3_ << 8;
      FUN_00f1a330();
      FUN_01224f40(&local_50);
      FUN_010ccf80(1,param_1,&LAB_010ccd10,0);
      local_8 = 0xffffffff;
      local_24 = 0;
      FUN_00f1a330();
    }
    FUN_010c93b0();
    FUN_010ca260();
    FUN_010ca940();
    FUN_010cbd20();
    if (*(int *)(param_1 + 0xac) != 0) {
      local_54 = 0;
      FUN_0122c9a0(&local_54,1);
    }
  }
  ExceptionList = local_10;
  return;
}


