
/* ============================================================ */
/* Function: FUN_011d8320 */
/* Entry: 011d8320 */
/* Signature: undefined FUN_011d8320() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011d8320(int param_1)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  void *pvVar4;
  int iVar5;
  int *piVar6;
  undefined4 *puVar7;
  int extraout_ECX;
  int extraout_ECX_00;
  short *******pppppppsVar8;
  short *******pppppppsVar9;
  short *psVar10;
  char *pcVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  short ******local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  uint local_64;
  undefined4 *local_60;
  int local_5c;
  int local_58;
  short *local_54;
  undefined8 local_50;
  undefined1 *local_48;
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
  puStack_c = &LAB_019935b8;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar5 = param_1;
  local_58 = param_1;
  local_14 = uVar2;
  if (*(int *)(param_1 + 0xb4) == 0) {
    local_54 = (short *)FUN_016830c0(0xac,uVar2);
    local_8 = 0;
    if (local_54 == (short *)0x0) {
      iVar3 = 0;
      iVar5 = extraout_ECX;
    }
    else {
      memset(local_54,0,0xac);
      iVar3 = FUN_01228950();
      iVar5 = extraout_ECX_00;
    }
    *(int *)(param_1 + 0xb4) = iVar3;
    if (iVar3 == 0) {
      ExceptionList = local_10;
      return;
    }
  }
  local_8 = 0xffffffff;
  FUN_01228bd0(*(undefined4 *)(param_1 + 0x40),0x80,0x27,0xcf,0x18,3,0,0x66,0x18,0,2,iVar5,2);
  FUN_011d8ff0(3,param_1,FUN_011d8e30,0);
  local_5c = 0;
  do {
    pvVar4 = (void *)FUN_016830c0(0x2c,uVar2);
    local_8 = 1;
    if (pvVar4 == (void *)0x0) {
      iVar5 = 0;
    }
    else {
      memset(pvVar4,0,0x2c);
      iVar5 = FUN_010baa10();
    }
    local_8 = 0xffffffff;
    if (iVar5 == 0) {
      ExceptionList = local_10;
      return;
    }
    pvVar4 = (void *)FUN_016830c0(0x1c0);
    local_8 = 2;
    if (pvVar4 == (void *)0x0) {
      local_54 = (short *)0x0;
    }
    else {
      memset(pvVar4,0,0x1c0);
      local_54 = (short *)FUN_01222670();
    }
    psVar10 = local_54;
    local_8 = 0xffffffff;
    if (local_54 == (short *)0x0) {
      ExceptionList = local_10;
      return;
    }
    FUN_012228b0(0,DAT_01bee88c,DAT_01bee890,0x66,0x18,s_System_Btn_Radio1_tga_01ae3078,0);
    FUN_01223e30(0,0x17);
    local_34 = 0;
    local_30 = 7;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    local_44 = 0;
    local_18 = 0;
    local_8 = 3;
    local_24 = DAT_01bfe468 + 8;
    local_48 = &DAT_3f7ae148;
    local_50 = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_20 = 0xb;
    local_2c = 1;
    local_1c = 0;
    local_28 = 5;
    if (local_5c == 0) {
      uVar12 = 0x10;
      pcVar11 = s_COMMON_TXT_TAMER_01ae3064;
LAB_011d855a:
      uVar13 = 0;
      uVar14 = 0;
      uVar15 = 0;
      uVar16 = 0;
      uVar17 = 0;
      uVar18 = 0;
      FUN_00f19cc0(pcVar11,uVar12);
      FUN_015ce480(&local_78,uVar13,uVar14,uVar15,uVar16,uVar17,uVar18);
      psVar10 = local_54;
    }
    else {
      if (local_5c == 1) {
        uVar12 = 0x12;
        pcVar11 = s_COMMON_TXT_DIGIMON_01ad9b10;
        goto LAB_011d855a;
      }
      local_68 = 0;
      uStack_74 = 0;
      uStack_70 = 0;
      uStack_6c = 0;
      local_64 = 7;
      local_78 = (short ******)0x0;
    }
    local_8._0_1_ = 4;
    pppppppsVar9 = &local_78;
    if (7 < local_64) {
      pppppppsVar9 = (short *******)local_78;
    }
    local_54 = (short *)((int)pppppppsVar9 + 2);
    pppppppsVar8 = pppppppsVar9;
    do {
      sVar1 = *(short *)pppppppsVar8;
      pppppppsVar8 = (short *******)((int)pppppppsVar8 + 2);
    } while (sVar1 != 0);
    FUN_00f1a040(pppppppsVar9,(int)pppppppsVar8 - ((int)pppppppsVar9 + 2) >> 1);
    local_8 = CONCAT31(local_8._1_3_,3);
    FUN_00f1a330();
    FUN_01224f40(&local_50);
    piVar6 = (int *)FUN_012317d0(psVar10,0,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890,0x33,
                                 0xc);
    (**(code **)(*piVar6 + 0x54))(1);
    puVar7 = (undefined4 *)FUN_016830c0(0x28);
    iVar3 = local_58;
    local_60 = puVar7;
    if (puVar7 != (undefined4 *)0x0) {
      *puVar7 = 0;
      puVar7[1] = 0;
      puVar7[2] = 0;
      puVar7[3] = 0;
      puVar7[4] = 0;
      puVar7[5] = 0;
      puVar7[6] = 0;
      puVar7[7] = 0;
      *(undefined8 *)(puVar7 + 8) = 0;
      *puVar7 = &DAT_01ae66e0;
      puVar7[1] = 0;
      puVar7[2] = 0;
      puVar7[3] = local_5c;
      *(undefined1 *)(puVar7 + 4) = 0;
      puVar7[5] = 0x66;
      puVar7[6] = 0x18;
      *(undefined2 *)(puVar7 + 7) = 0x101;
      puVar7[8] = 0;
      puVar7[9] = 0;
      FUN_012288c0(iVar5);
      iVar3 = local_58;
      FUN_0122a090(puVar7);
    }
    local_8 = 0xffffffff;
    local_24 = 0;
    FUN_00f1a330();
    local_5c = local_5c + 1;
    if (1 < local_5c) {
      local_58 = 0;
      FUN_0122a840(&local_58,1);
      FUN_012472c0(*(undefined4 *)(iVar3 + 0xb4));
      ExceptionList = local_10;
      return;
    }
  } while( true );
}


