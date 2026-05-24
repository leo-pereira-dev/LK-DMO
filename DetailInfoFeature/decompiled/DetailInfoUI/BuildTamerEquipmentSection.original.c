
/* ============================================================ */
/* Function: FUN_011db440 */
/* Entry: 011db440 */
/* Signature: undefined FUN_011db440() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011db440(int param_1)

{
  short sVar1;
  undefined1 *puVar2;
  uint uVar3;
  int iVar4;
  void *pvVar5;
  int iVar6;
  int *piVar7;
  undefined4 *puVar8;
  int extraout_ECX;
  int extraout_ECX_00;
  undefined4 extraout_ECX_01;
  short *******pppppppsVar9;
  short *******pppppppsVar10;
  char *pcVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  short ******local_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 local_7c;
  uint local_78;
  int local_74;
  int local_70;
  short *local_6c;
  undefined4 *local_68;
  int local_64;
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
  puStack_18 = &LAB_019938cb;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  iVar6 = param_1;
  local_74 = param_1;
  local_24 = uVar3;
  puVar2 = &stack0xfffffffc;
  if (*(int *)(param_1 + 0x88) == 0) {
    local_68 = (undefined4 *)FUN_016830c0(0xac,uVar3);
    local_14 = 0;
    if (local_68 == (undefined4 *)0x0) {
      iVar4 = 0;
      iVar6 = extraout_ECX;
    }
    else {
      memset(local_68,0,0xac);
      iVar4 = FUN_01228950();
      iVar6 = extraout_ECX_00;
    }
    *(int *)(param_1 + 0x88) = iVar4;
    puVar2 = puStack_20;
    if (iVar4 == 0) {
      ExceptionList = local_1c;
      return;
    }
  }
  puStack_20 = puVar2;
  local_14 = 0xffffffff;
  FUN_01228bd0(*(undefined4 *)(param_1 + 0x40),0x13,0x20f,0xe1,0x28,5,0,0x6e,0x28,0,2,iVar6,2);
  FUN_011dd5b0(extraout_ECX_01,param_1,&LAB_011dae50,0);
  local_64 = 0;
  do {
    pvVar5 = (void *)FUN_016830c0(0x2c,uVar3);
    local_14 = 1;
    if (pvVar5 == (void *)0x0) {
      local_70 = 0;
    }
    else {
      memset(pvVar5,0,0x2c);
      local_70 = FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (local_70 == 0) {
      ExceptionList = local_1c;
      return;
    }
    pvVar5 = (void *)FUN_016830c0(0x1c0);
    local_14 = 2;
    if (pvVar5 == (void *)0x0) {
      iVar6 = 0;
    }
    else {
      memset(pvVar5,0,0x1c0);
      iVar6 = FUN_01222670();
    }
    local_14 = 0xffffffff;
    if (iVar6 == 0) {
      ExceptionList = local_1c;
      return;
    }
    FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0x6e,0x28,s_DMG_SKINS_Mini_Out_btn_a_tga_01ad83b4,0);
    FUN_01223e30(0,0x24);
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 3;
    local_34 = DAT_01bfe468 + 8;
    local_58 = &DAT_3f7ae148;
    local_60 = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_30 = 0xb;
    local_3c = 1;
    local_2c = 0;
    local_38 = 5;
    if (local_64 == 0) {
      uVar12 = 0x11;
      pcVar11 = s_DETAIL_INFO_EQUIP_01ae3460;
LAB_011db690:
      uVar13 = 0;
      uVar14 = 0;
      uVar15 = 0;
      uVar16 = 0;
      uVar17 = 0;
      uVar18 = 0;
      FUN_00f19cc0(pcVar11,uVar12);
      FUN_015ce480(&local_8c,uVar13,uVar14,uVar15,uVar16,uVar17,uVar18);
    }
    else {
      if (local_64 == 1) {
        uVar12 = 0x18;
        pcVar11 = s_MAINBAR_MEMU_SEAL_MASTER_01ae32f8;
        goto LAB_011db690;
      }
      local_7c = 0;
      uStack_88 = 0;
      uStack_84 = 0;
      uStack_80 = 0;
      local_78 = 7;
      local_8c = (short ******)0x0;
    }
    iVar4 = local_64;
    local_14._0_1_ = 4;
    pppppppsVar10 = &local_8c;
    if (7 < local_78) {
      pppppppsVar10 = (short *******)local_8c;
    }
    local_6c = (short *)((int)pppppppsVar10 + 2);
    pppppppsVar9 = pppppppsVar10;
    do {
      sVar1 = *(short *)pppppppsVar9;
      pppppppsVar9 = (short *******)((int)pppppppsVar9 + 2);
    } while (sVar1 != 0);
    FUN_00f1a040(pppppppsVar10,(int)pppppppsVar9 - ((int)pppppppsVar10 + 2) >> 1);
    local_14 = CONCAT31(local_14._1_3_,3);
    FUN_00f1a330();
    FUN_01224f40(&local_60);
    local_68 = operator_new(8);
    *local_68 = &DAT_01ae35b4;
    local_68[1] = iVar4;
    if (*(undefined4 **)(iVar6 + 0x34) != (undefined4 *)0x0) {
      (**(code **)**(undefined4 **)(iVar6 + 0x34))(1);
    }
    *(undefined4 **)(iVar6 + 0x34) = local_68;
    piVar7 = (int *)FUN_012317d0(iVar6,0,DAT_01bee88c,DAT_01bee890,0x6e,0x28,0x37,0x14);
    (**(code **)(*piVar7 + 0x54))(1);
    puVar8 = (undefined4 *)FUN_016830c0(0x28);
    iVar4 = local_64;
    iVar6 = local_74;
    local_6c = (short *)puVar8;
    if (puVar8 != (undefined4 *)0x0) {
      *puVar8 = 0;
      puVar8[1] = 0;
      puVar8[2] = 0;
      puVar8[3] = 0;
      puVar8[4] = 0;
      puVar8[5] = 0;
      puVar8[6] = 0;
      puVar8[7] = 0;
      *(undefined8 *)(puVar8 + 8) = 0;
      *puVar8 = &DAT_01ae66e0;
      puVar8[1] = 0;
      puVar8[2] = 0;
      puVar8[3] = local_64;
      *(undefined1 *)(puVar8 + 4) = 0;
      puVar8[5] = 0x6e;
      puVar8[6] = 0x28;
      *(undefined2 *)(puVar8 + 7) = 0x101;
      puVar8[8] = 0;
      puVar8[9] = 0;
      FUN_012288c0(local_70);
      iVar6 = local_74;
      FUN_0122a090(puVar8);
    }
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
    local_64 = iVar4 + 1;
    if (1 < local_64) {
      local_68 = (undefined4 *)0x0;
      FUN_0122a840(&local_68,1);
      FUN_012472c0(*(undefined4 *)(iVar6 + 0x88));
      ExceptionList = local_1c;
      return;
    }
  } while( true );
}


