
/* ============================================================ */
/* Function: FUN_01283910 */
/* Entry: 01283910 */
/* Signature: undefined FUN_01283910() */
/* ============================================================ */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * __fastcall FUN_01283910(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  void *pvVar3;
  code *pcVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  char *pcVar10;
  undefined4 uVar11;
  undefined1 local_40 [8];
  int local_38;
  int *local_34;
  int *local_30;
  undefined4 local_2c;
  int *local_28;
  int local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199d41f;
  local_1c = ExceptionList;
  local_24 = param_2;
  switch(param_1) {
  case 1:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xf0);
    local_14 = 0x11;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xf0);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af42bc;
      piVar6[0x29] = (int)&DAT_01af42b0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      iVar1 = iRam01bee8a0;
      iVar7 = iRam01bee89c;
      iVar8 = iRam01bee898;
      piVar6[0x38] = _DAT_01bee894;
      piVar6[0x39] = iVar8;
      piVar6[0x3a] = iVar7;
      piVar6[0x3b] = iVar1;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_TamerWindow_01ae9834,0xb);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_013894f0) {
      FUN_013894f0();
      goto LAB_01283d3d;
    }
    break;
  case 2:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xf8);
    local_14 = 0x12;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xf8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af4040;
      piVar6[0x29] = (int)&DAT_01af4034;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      iVar2 = iRam01bee8a0;
      iVar1 = iRam01bee89c;
      iVar7 = iRam01bee898;
      iVar8 = _DAT_01bee894;
      *(undefined1 *)(piVar6 + 0x3c) = 0;
      piVar6[0x3d] = 0;
      piVar6[0x38] = iVar8;
      piVar6[0x39] = iVar7;
      piVar6[0x3a] = iVar1;
      piVar6[0x3b] = iVar2;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_MainDigimonWindow_01ae9820,0x11);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01386c40) {
      FUN_01386c40();
      goto LAB_01283d3d;
    }
    break;
  case 3:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xf8);
    local_14 = 0x13;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xf8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af6c2c;
      piVar6[0x29] = (int)&DAT_01af6c20;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      piVar6[0x38] = 0;
      piVar6[0x39] = 0;
      piVar6[0x3a] = 0;
      piVar6[0x3b] = 0;
      iVar8 = DAT_01bee890;
      piVar6[0x3c] = DAT_01bee88c;
      piVar6[0x3d] = iVar8;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_TargetUI_01ae9850,8);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_013bb1e0) {
      FUN_013bb1e0();
      goto LAB_01283d3d;
    }
    break;
  case 4:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xcc);
    local_14 = 0x14;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xcc);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af4178;
      piVar6[0x29] = (int)&DAT_01af416c;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      *(undefined2 *)(piVar6 + 0x2d) = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      *(undefined1 *)(piVar6 + 0x31) = 0;
      piVar6[0x32] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_TacticsWindow_01ae9840,0xd);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010b92b0) {
      FUN_010b92b0();
      goto LAB_01283d3d;
    }
    break;
  case 5:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xbc);
    local_14 = 0x15;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xbc);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x17);
      *piVar6 = (int)&DAT_01af5f3c;
      piVar6[0x29] = (int)&DAT_01af5f30;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      pvVar3 = operator_new(0x14);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x2d] = (int)pvVar3;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_XaiGuageWindow_01ae9868,0xe);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_013ae830) {
      FUN_013ae830();
      goto LAB_01283d3d;
    }
    break;
  case 6:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xc4);
    local_14 = 0x18;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xc4);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ae9414;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x1a);
      *piVar6 = (int)&DAT_01af44b4;
      piVar6[0x29] = (int)&DAT_01af4574;
      *(undefined2 *)(piVar6 + 0x2b) = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x2e] = (int)pvVar3;
      piVar6[0x30] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_QuickSlot_01adbb30,9);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_0138ad30) {
      FUN_0138ad30();
      goto LAB_01283d3d;
    }
    break;
  case 7:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x250);
    local_14 = 0x1b;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x250);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01aede44;
      piVar6[0x2a] = 0;
      piVar9 = piVar6 + 0x2b;
      *piVar6 = (int)&DAT_01af509c;
      piVar6[0x29] = (int)&DAT_01af5090;
      iVar8 = 9;
      piVar5 = piVar6 + 0x2e;
      do {
        *piVar9 = 0;
        piVar5[-2] = 0;
        piVar9 = piVar9 + 0xb;
        piVar5[-1] = 0;
        *piVar5 = 0;
        piVar5[1] = 0;
        piVar5[2] = 0;
        piVar5[3] = 0;
        piVar5[4] = 0;
        piVar5[5] = 0;
        piVar5[6] = 0;
        *(undefined2 *)(piVar5 + 7) = 0;
        *(undefined1 *)((int)piVar5 + 0x1e) = 0;
        iVar8 = iVar8 + -1;
        piVar5 = piVar5 + 0xb;
      } while (iVar8 != 0);
      piVar6[0x8e] = 0;
      *(undefined1 *)(piVar6 + 0x8f) = 0;
      iVar7 = DAT_01bee890;
      iVar8 = DAT_01bee88c;
      piVar6[0x92] = 0;
      piVar6[0x93] = 0;
      piVar6[0x90] = iVar8;
      piVar6[0x91] = iVar7;
      param_2 = local_24;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_QuickEvol_01ae985c,9);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_0139b6d0) {
      FUN_0139b6d0();
      goto LAB_01283d3d;
    }
    break;
  case 8:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01288bb0();
    ExceptionList = local_1c;
    return piVar6;
  case 9:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb0);
    local_14 = 0x1e;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb0);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad6530;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01ad6bf0;
      piVar6[0x29] = (int)&DAT_01ad6be4;
      piVar6[0x2b] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_ChatOption_01ae9880,10);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010df9e0) {
      FUN_010df9e0();
      goto LAB_01283d3d;
    }
    break;
  case 10:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01288050();
    ExceptionList = local_1c;
    return piVar6;
  case 0xb:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    local_34 = (int *)FUN_016830c0(0x160);
    local_14 = 0x44;
    if (local_34 == (void *)0x0) {
      uVar11 = 7;
      piVar6 = (int *)0x0;
      pcVar10 = s_ZoneMap_01ae993c;
    }
    else {
      memset(local_34,0,0x160);
      piVar6 = (int *)FUN_012ecab0();
      uVar11 = 7;
      pcVar10 = s_ZoneMap_01ae993c;
    }
    goto LAB_01285497;
  case 0xc:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xbc);
    local_14 = 0x1f;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      uVar11 = 0xd;
      piVar6 = (int *)0x0;
      pcVar10 = s_DigiviceQuest_01ae9898;
    }
    else {
      memset(piVar6,0,0xbc);
      FUN_01287ce0();
      *piVar6 = (int)&DAT_01af2cf4;
      piVar6[0x29] = 0;
      piVar6[0x2a] = 0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      iVar8 = DAT_01bee890;
      uVar11 = 0xd;
      piVar6[0x2d] = DAT_01bee88c;
      piVar6[0x2e] = iVar8;
      pcVar10 = s_DigiviceQuest_01ae9898;
    }
    goto LAB_01283d1b;
  case 0xd:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb8);
    local_14 = 0x20;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01af46ac;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x22);
      *piVar6 = (int)&DAT_01af45ec;
      piVar6[0x29] = (int)&DAT_01af45e0;
      piVar6[0x2b] = 0;
      piVar9 = piVar6 + 0x2c;
      *piVar9 = 0;
      piVar6[0x2d] = 0;
      local_34 = piVar9;
      pvVar3 = operator_new(0x54);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      *(void **)((int)pvVar3 + 8) = pvVar3;
      *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
      *piVar9 = (int)pvVar3;
      piVar6 = local_30;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_PartyWindow_01ae988c,0xb);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_0138e170) {
      FUN_0138e170();
      goto LAB_01283d3d;
    }
    break;
  case 0xe:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x114);
    local_14 = 0x23;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      uVar11 = 8;
      pcVar10 = s_BuffInfo_01ae98b4;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x114);
      FUN_01287ce0();
      *piVar6 = (int)&DAT_01af4a74;
      piVar6[0x29] = 0;
      iVar8 = 2;
      piVar6[0x2a] = 0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar9 = piVar6 + 0x34;
      do {
        *piVar9 = 0;
        piVar9[1] = 0;
        *(undefined1 *)(piVar9 + 2) = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 3;
      } while (iVar8 != 0);
      *(undefined2 *)(piVar6 + 0x3a) = 0;
      piVar6[0x3b] = 0;
      piVar6[0x3c] = 0;
      piVar6[0x3d] = 0;
      piVar6[0x3e] = 0;
      piVar6[0x3f] = 0;
      piVar6[0x40] = 0;
      piVar6[0x41] = 0;
      piVar6[0x42] = 0;
      uVar11 = 8;
      piVar6[0x43] = 0;
      piVar6[0x44] = 0;
      pcVar10 = s_BuffInfo_01ae98b4;
    }
    goto LAB_01283d1b;
  case 0xf:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01288930();
    ExceptionList = local_1c;
    return piVar6;
  case 0x10:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012889f0();
    ExceptionList = local_1c;
    return piVar6;
  default:
    return (int *)0x0;
  case 0x32:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x8c8);
    local_14 = 0x25;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x8c8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ae91a4;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01aedc4c;
      piVar6[0x29] = (int)&DAT_01aedd0c;
      iVar8 = 100;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar9 = piVar6 + 0x2d;
      do {
        *piVar9 = 0;
        piVar9[1] = 0;
        piVar9[2] = 0;
        piVar9[3] = 0;
        piVar9[4] = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 5;
      } while (iVar8 != 0);
      piVar6[0x221] = 0;
      piVar6[0x222] = 0;
      piVar6[0x223] = 0;
      piVar6[0x224] = 0;
      piVar6[0x225] = 0;
      piVar6[0x226] = 0;
      piVar6[0x227] = 0;
      piVar6[0x228] = 0;
      piVar6[0x229] = 0;
      *(undefined1 *)(piVar6 + 0x22a) = 0;
      piVar6[0x22b] = 0;
      piVar6[0x22c] = 0;
      piVar6[0x22d] = 0;
      piVar6[0x22e] = 0;
      piVar6[0x22f] = 0;
      piVar6[0x230] = 0;
      *(undefined1 *)(piVar6 + 0x231) = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Inventory_01ae98a8,9);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_012f70e0) {
      FUN_012f70e0();
      goto LAB_01283d3d;
    }
    break;
  case 0x34:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xf4);
    local_14 = 0x28;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xf4);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01afffbc;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x2a);
      *piVar6 = (int)&DAT_01affef0;
      piVar6[0x29] = (int)&DAT_01afffb0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      local_34 = piVar6 + 0x31;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x33] = (int)pvVar3;
      *(undefined1 *)(piVar6 + 0x35) = 1;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      piVar6[0x38] = 0;
      local_30[0x39] = 0;
      local_30[0x3a] = 0;
      local_30[0x3b] = 0;
      *(undefined2 *)(local_30 + 0x3c) = 0;
      *(undefined1 *)((int)local_30 + 0xf2) = 0;
      piVar6 = local_30;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(&DAT_01adbbb0,4);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_014647a0) {
      FUN_014647a0();
      goto LAB_01283d3d;
    }
    break;
  case 0x35:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xc4);
    local_14 = 0x2b;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xc4);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ae9560;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01aeb754;
      piVar6[0x29] = (int)&DAT_01aeb748;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Store_01adbb3c,5);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_012ba410) {
      FUN_012ba410();
      goto LAB_01283d3d;
    }
    break;
  case 0x37:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x17c);
    local_14 = 0x2c;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x17c);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ae87b0;
      piVar6[0x2a] = 0;
      local_30 = piVar6 + 0x2b;
      local_14 = CONCAT31(local_14._1_3_,0x2e);
      *piVar6 = (int)&DAT_01ae8874;
      piVar6[0x29] = (int)&DAT_01ae8934;
      _eh_vector_constructor_iterator_(local_30,0xc,0xe,FUN_00f2df90,FUN_00f199b0);
      piVar6[0x55] = 0;
      piVar6[0x56] = 0;
      piVar6[0x57] = 0;
      piVar6[0x58] = 0;
      piVar6[0x59] = 0;
      iVar2 = iRam01bee8a0;
      iVar1 = iRam01bee89c;
      iVar7 = iRam01bee898;
      iVar8 = _DAT_01bee894;
      piVar6[0x5e] = 0;
      piVar6[0x5a] = iVar8;
      piVar6[0x5b] = iVar7;
      piVar6[0x5c] = iVar1;
      piVar6[0x5d] = iVar2;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_DigitamaSell_01ae98d0,0xc);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01264d60) {
      FUN_01264d60();
      goto LAB_01283d3d;
    }
    break;
  case 0x38:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x1a4);
    local_14 = 0x2f;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x1a4);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ae87b0;
      piVar6[0x2a] = 0;
      local_30 = piVar6 + 0x2b;
      local_14._0_1_ = 0x31;
      *piVar6 = (int)&DAT_01ae86f0;
      piVar6[0x29] = (int)&DAT_01ae86e4;
      _eh_vector_constructor_iterator_(local_30,0xc,0xe,FUN_00f2df90,FUN_00f199b0);
      local_14 = CONCAT31(local_14._1_3_,0x32);
      piVar6[0x55] = 0;
      piVar6[0x56] = 0;
      pvVar3 = operator_new(0x18);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x55] = (int)pvVar3;
      piVar6[0x57] = 0;
      piVar6[0x58] = 0;
      piVar6[0x59] = 0;
      piVar6[0x5a] = 0;
      piVar6[0x5b] = 0;
      piVar6[0x5c] = 0;
      piVar6[0x5d] = 0;
      iVar2 = iRam01bee8a0;
      iVar1 = iRam01bee89c;
      iVar7 = iRam01bee898;
      iVar8 = _DAT_01bee894;
      piVar6[0x62] = 0;
      piVar6[99] = 0;
      piVar6[0x5e] = iVar8;
      piVar6[0x5f] = iVar7;
      piVar6[0x60] = iVar1;
      piVar6[0x61] = iVar2;
      piVar6[100] = 0;
      *(undefined1 *)(piVar6 + 0x65) = 0;
      piVar6[0x66] = 1;
      piVar6[0x67] = 0x42e00000;
      piVar6[0x68] = 0x425c0000;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_DigitamaScan_01ae98c0,0xc);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_012614e0) {
      FUN_012614e0();
      goto LAB_01283d3d;
    }
    break;
  case 0x39:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar9 = (int *)FUN_016830c0(0x1c0);
    local_14 = 0x33;
    local_30 = piVar9;
    if (piVar9 == (int *)0x0) {
      uVar11 = 8;
      pcVar10 = s_QuestRev_01ae98ec;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar9,0,0x1c0);
      FUN_01287ce0();
      local_14._0_1_ = 0x34;
      *piVar9 = (int)&DAT_01affe30;
      piVar9[0x29] = 0;
      piVar9[0x2a] = 0;
      piVar9[0x2b] = 0;
      piVar9[0x2c] = 0;
      piVar9[0x2d] = 0;
      piVar9[0x2e] = 0;
      local_34 = piVar9 + 0x2f;
      piVar9[0x30] = 0;
      piVar9[0x31] = 0;
      piVar9[0x32] = 0;
      pvVar3 = operator_new(0xc);
      piVar6 = local_30;
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar9[0x31] = (int)pvVar3;
      *(undefined1 *)(piVar9 + 0x33) = 1;
      piVar9[0x34] = 0;
      piVar9[0x35] = 0;
      piVar9[0x36] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x35);
      FUN_010baa10();
      piVar6[0x42] = 0;
      piVar6[0x43] = 0;
      iVar8 = 6;
      piVar6[0x44] = 0;
      piVar9 = piVar6 + 0x4b;
      do {
        *piVar9 = 0;
        piVar9[1] = 0;
        piVar9[2] = 0;
        piVar9[3] = 0;
        piVar9[4] = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 5;
      } while (iVar8 != 0);
      piVar6[0x69] = 0;
      piVar6[0x6a] = 0;
      *(undefined1 *)(piVar6 + 0x6b) = 0;
      piVar6[0x6c] = 0;
      piVar6[0x6d] = 0;
      piVar6[0x6e] = 0;
      piVar6[0x6f] = 0;
      uVar11 = 8;
      piVar6[0x45] = 0;
      piVar6[0x46] = 0;
      piVar6[0x47] = 0;
      piVar6[0x48] = 0;
      pcVar10 = s_QuestRev_01ae98ec;
      piVar6[0x49] = 0;
      piVar6[0x4a] = 0;
    }
    goto LAB_01283d1b;
  case 0x3a:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar9 = (int *)FUN_016830c0(0x1c4);
    local_14 = 0x37;
    local_30 = piVar9;
    if (piVar9 == (int *)0x0) {
      uVar11 = 9;
      pcVar10 = s_QuestComp_01ae98e0;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar9,0,0x1c4);
      FUN_01287ce0();
      local_14._0_1_ = 0x38;
      *piVar9 = (int)&DAT_01aed290;
      piVar9[0x29] = 0;
      piVar9[0x2a] = 0;
      piVar9[0x2b] = 0;
      piVar9[0x2c] = 0;
      piVar9[0x2d] = 0;
      piVar9[0x2e] = 0;
      local_34 = piVar9 + 0x2f;
      piVar9[0x30] = 0;
      piVar9[0x31] = 0;
      piVar9[0x32] = 0;
      pvVar3 = operator_new(0xc);
      piVar6 = local_30;
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar9[0x31] = (int)pvVar3;
      *(undefined1 *)(piVar9 + 0x33) = 1;
      piVar9[0x34] = 0;
      piVar9[0x35] = 0;
      piVar9[0x36] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x39);
      FUN_010baa10();
      piVar6[0x42] = 0;
      piVar6[0x43] = 0;
      iVar8 = 6;
      piVar6[0x44] = 0;
      piVar9 = piVar6 + 0x4b;
      do {
        *piVar9 = 0;
        piVar9[1] = 0;
        piVar9[2] = 0;
        piVar9[3] = 0;
        piVar9[4] = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 5;
      } while (iVar8 != 0);
      piVar6[0x69] = 0;
      piVar6[0x6a] = 0;
      piVar6[0x6b] = 0;
      *(undefined1 *)(piVar6 + 0x6c) = 0;
      piVar6[0x6d] = 0;
      piVar6[0x6e] = 0;
      piVar6[0x6f] = 0;
      piVar6[0x70] = 0;
      uVar11 = 9;
      piVar6[0x45] = 0;
      piVar6[0x46] = 0;
      piVar6[0x47] = 0;
      piVar6[0x48] = 0;
      pcVar10 = s_QuestComp_01ae98e0;
      piVar6[0x49] = 0;
      piVar6[0x4a] = 0;
    }
    goto LAB_01283d1b;
  case 0x3b:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    local_34 = (int *)FUN_016830c0(0x160);
    local_14 = 0x3b;
    if (local_34 == (void *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(local_34,0,0x160);
      piVar6 = (int *)FUN_012fc960();
    }
    uVar11 = 9;
    pcVar10 = s_QuestList_01ae9908;
    goto LAB_01285497;
  case 0x3c:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x344);
    local_14 = 0x3c;
    local_28 = piVar6;
    if (piVar6 == (int *)0x0) {
      uVar11 = 0xd;
      pcVar10 = s_QuestDiscript_01ae98f8;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x344);
      FUN_01287ce0();
      local_14._0_1_ = 0x3d;
      piVar9 = piVar6 + 0x30;
      *piVar6 = (int)&DAT_01aee210;
      piVar6[0x29] = 0;
      piVar6[0x2a] = 0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      iVar8 = 6;
      do {
        iVar7 = iVar8;
        memset(piVar9,0,0x45);
        param_2 = local_24;
        piVar5 = local_28;
        piVar9 = (int *)((int)piVar9 + 0x45);
        iVar8 = iVar7 + -1;
      } while (iVar8 != 0);
      iVar7 = iVar7 + 5;
      piVar6 = local_28 + 0x98;
      do {
        *piVar6 = 0;
        piVar6[1] = 0;
        piVar6[2] = 0;
        piVar6[3] = 0;
        piVar6[4] = 0;
        iVar7 = iVar7 + -1;
        piVar6 = piVar6 + 5;
      } while (iVar7 != 0);
      local_34 = local_28 + 0xb6;
      local_28[0xb7] = 0;
      local_28[0xb8] = 0;
      local_28[0xb9] = 0;
      pvVar3 = operator_new(0xc);
      piVar6 = local_28;
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar5[0xb8] = (int)pvVar3;
      *(undefined1 *)(piVar5 + 0xba) = 1;
      piVar5[0xbb] = 0;
      piVar5[0xbc] = 0;
      piVar5[0xbd] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x3e);
      local_28[0xbe] = 0;
      local_28[0xbf] = 0;
      local_28[0xc0] = 0;
      FUN_010baa10();
      uVar11 = 0xd;
      piVar6[0xcc] = 0;
      piVar6[0xcd] = 0;
      piVar6[0xce] = 0;
      piVar6[0xcf] = 0;
      piVar6[0xd0] = 0;
      pcVar10 = s_QuestDiscript_01ae98f8;
    }
    goto LAB_01283d1b;
  case 0x3d:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xe0);
    local_14 = 0x3f;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      uVar11 = 0xb;
      pcVar10 = s_QuestHelper_01adbb44;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xe0);
      FUN_01287ce0();
      local_14 = CONCAT31(local_14._1_3_,0x40);
      *piVar6 = (int)&DAT_01af4dc4;
      piVar6[0x29] = 0;
      piVar6[0x2a] = 0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      local_34 = piVar6 + 0x2e;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x30] = (int)pvVar3;
      *(undefined1 *)(piVar6 + 0x32) = 1;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      uVar11 = 0xb;
      pcVar10 = s_QuestHelper_01adbb44;
      *(undefined1 *)(local_30 + 0x36) = 0;
      local_30[0x37] = 0;
      piVar6 = local_30;
    }
    goto LAB_01283d1b;
  case 0x3e:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    local_34 = (int *)FUN_016830c0(0x164);
    local_14 = 0x41;
    if (local_34 == (void *)0x0) {
      uVar11 = 0xb;
      piVar6 = (int *)0x0;
      pcVar10 = s_MakeTactics_01ae9924;
    }
    else {
      memset(local_34,0,0x164);
      piVar6 = (int *)FUN_01123530();
      uVar11 = 0xb;
      pcVar10 = s_MakeTactics_01ae9924;
    }
    goto LAB_01285497;
  case 0x40:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x1a0);
    local_14 = 0x42;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      uVar11 = 0xe;
      pcVar10 = s_EnchantTactics_01ae9914;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x1a0);
      FUN_01287ce0();
      *piVar6 = (int)&DAT_01aebd54;
      piVar6[0x29] = 0;
      iVar8 = 5;
      piVar6[0x2a] = 0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar9 = piVar6 + 0x35;
      do {
        *piVar9 = 0;
        piVar9[1] = 0;
        piVar9[2] = 0;
        piVar9[3] = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 4;
      } while (iVar8 != 0);
      iVar8 = 3;
      piVar9 = piVar6 + 0x49;
      do {
        *piVar9 = 0;
        piVar9[1] = 0;
        piVar9[2] = 0;
        piVar9[3] = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 4;
      } while (iVar8 != 0);
      piVar6[0x55] = 0;
      piVar6[0x56] = 0;
      piVar6[0x57] = 0;
      piVar6[0x58] = 0;
      piVar6[0x59] = 0;
      piVar6[0x5a] = 0x7fffffff;
      piVar6[0x5b] = 0;
      piVar6[0x5c] = 0x7fffffff;
      piVar6[0x5d] = 0;
      piVar6[0x5e] = -1;
      piVar6[0x5f] = -1;
      *(undefined1 *)(piVar6 + 0x60) = 0;
      piVar6[0x61] = 0;
      piVar6[0x62] = 0;
      piVar6[99] = 0;
      piVar6[100] = 0;
      piVar6[0x65] = 0;
      uVar11 = 0xe;
      piVar6[0x66] = 0;
      piVar6[0x67] = 0;
      pcVar10 = s_EnchantTactics_01ae9914;
    }
    goto LAB_01283d1b;
  case 0x42:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xd4);
    local_14 = 8;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xd4);
      FUN_01304130();
      *piVar6 = (int)&DAT_01aefaf0;
      piVar6[0x34] = 0;
    }
    uVar11 = 0xb;
    pcVar10 = s_OptionSound_01ae97c4;
    goto LAB_01283d1b;
  case 0x43:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x420);
    local_14 = 9;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      uVar11 = 0xb;
      pcVar10 = s_OptionMacro_01ae97f4;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x420);
      FUN_01304130();
      *piVar6 = (int)&DAT_01aef914;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      piVar6[0x38] = 0;
      iVar8 = 0x42;
      piVar9 = piVar6 + 0x3b;
      do {
        piVar9[-2] = 0;
        piVar9[-1] = 0;
        *(undefined1 *)piVar9 = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 3;
      } while (iVar8 != 0);
      piVar6[0xff] = 0;
      piVar6[0x100] = 0;
      *(undefined1 *)(piVar6 + 0x101) = 0;
      uVar11 = 0xb;
      piVar6[0x102] = 0;
      piVar6[0x103] = 0;
      piVar6[0x104] = 0;
      piVar6[0x105] = 0;
      piVar6[0x106] = 0;
      piVar6[0x107] = 0;
      pcVar10 = s_OptionMacro_01ae97f4;
    }
    goto LAB_01283d1b;
  case 0x44:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xd8);
    local_14 = 0xb;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      uVar11 = 0xd;
      pcVar10 = s_OptionGraphic_01ae97e4;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xd8);
      FUN_01304130();
      uVar11 = 0xd;
      *piVar6 = (int)&DAT_01aeea68;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      pcVar10 = s_OptionGraphic_01ae97e4;
    }
    goto LAB_01283d1b;
  case 0x45:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xd8);
    local_14 = 0xc;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xd8);
      FUN_01304130();
      *piVar6 = (int)&DAT_01aef3f0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      local_14 = CONCAT31(local_14._1_3_,0xe);
      local_28 = operator_new(8);
      local_28[0] = 0;
      local_28[1] = 0;
      *local_28 = (int)&DAT_01af3160;
      local_28[1] = 0;
      if ((undefined4 *)piVar6[0x35] != (undefined4 *)0x0) {
        (*(code *)**(undefined4 **)piVar6[0x35])(1);
        piVar6[0x35] = 0;
      }
      piVar6[0x35] = (int)local_28;
      if (local_28 != (int *)0x0) {
        local_30 = (int *)(DAT_01bfe470 + 4);
        local_2c = 0x20;
        FUN_00f1d720(local_40,&local_2c);
        if ((((*(char *)(local_38 + 0xd) == '\0') && (*(int *)(local_38 + 0x10) < 0x21)) &&
            (local_38 != *local_30)) &&
           ((local_30 = *(int **)(local_38 + 0x14), local_30 != (int *)0x0 &&
            (iVar8 = (**(code **)(*local_30 + 0x20))(), iVar8 == 0x20)))) {
          uVar11 = 0xf;
          pcVar10 = s_OptionInterface_01ae9810;
          local_28[1] = (int)local_30;
          goto LAB_01283d1b;
        }
      }
    }
    uVar11 = 0xf;
    pcVar10 = s_OptionInterface_01ae9810;
    goto LAB_01283d1b;
  case 0x46:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xf8);
    local_14 = 0xf;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      uVar11 = 0xf;
      pcVar10 = s_OptionUserGrade_01ae9800;
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xf8);
      FUN_01304130();
      local_14 = CONCAT31(local_14._1_3_,0x10);
      *piVar6 = (int)&DAT_01aefc98;
      local_34 = piVar6 + 0x34;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x36] = (int)pvVar3;
      *(undefined1 *)(piVar6 + 0x38) = 1;
      piVar6[0x39] = 0;
      piVar6[0x3a] = 0;
      piVar6[0x3b] = 0;
      uVar11 = 0xf;
      pcVar10 = s_OptionUserGrade_01ae9800;
      local_30[0x3c] = 0;
      local_30[0x3d] = 0;
      piVar6 = local_30;
    }
LAB_01283d1b:
    local_14 = 0xffffffff;
    FUN_00f19f10(pcVar10,uVar11);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_00f505a0) goto LAB_01283d3d;
    break;
  case 0x47:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x78c);
    local_14 = 0x45;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x78c);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01aeab4c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01aeaa8c;
      piVar6[0x29] = (int)&DAT_01aeaa80;
      piVar6[0x2b] = 0;
      *(undefined1 *)(piVar6 + 0x2c) = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x36] = 0;
      iVar8 = DAT_01beeea0;
      piVar6[0x37] = DAT_01beee9c;
      piVar6[0x38] = iVar8;
      iVar8 = 0x46;
      piVar9 = piVar6 + 0x39;
      do {
        *piVar9 = 0;
        piVar9[1] = 0;
        piVar9[2] = 0;
        piVar9[3] = 0;
        piVar9[4] = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 5;
      } while (iVar8 != 0);
      iVar8 = 0xe;
      piVar9 = piVar6 + 0x197;
      do {
        *piVar9 = 0;
        piVar9[1] = 0;
        piVar9[2] = 0;
        piVar9[3] = 0;
        piVar9[4] = 0;
        iVar8 = iVar8 + -1;
        piVar9 = piVar9 + 5;
      } while (iVar8 != 0);
      piVar6[0x1dd] = 0;
      piVar6[0x1de] = 0;
      piVar6[0x1df] = 0;
      piVar6[0x1e0] = 0;
      piVar6[0x1e1] = 0;
      *(undefined1 *)(piVar6 + 0x1e2) = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Warehouse_01ae9930,9);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_0129b3c0) {
      FUN_0129b3c0();
      goto LAB_01283d3d;
    }
    break;
  case 0x49:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x5ac);
    local_14 = 0x48;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x5ac);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01aefe8c;
      piVar6[0x2a] = 0;
      local_14._0_1_ = 0x4a;
      *piVar6 = (int)&DAT_01aefdcc;
      piVar6[0x29] = (int)&DAT_01aefdc0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      _eh_vector_constructor_iterator_(piVar6 + 0x35,0x7c,5,FUN_01314e90,FUN_01314f70);
      local_14 = CONCAT31(local_14._1_3_,0x4b);
      _eh_vector_constructor_iterator_(piVar6 + 0xd0,0x7c,5,FUN_01314e90,FUN_01314f70);
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Trade_01ae994c,5);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01314760) {
      FUN_01314760();
      goto LAB_01283d3d;
    }
    break;
  case 0x4a:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb8);
    local_14 = 0x4c;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01aed4d4;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01aeffdc;
      piVar6[0x29] = (int)&DAT_01af009c;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_NpcFind_01ae9944,7);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01317d30) {
      FUN_01317d30();
      goto LAB_01283d3d;
    }
    break;
  case 0x4b:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012882d0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x4c:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xbc);
    local_14 = 0x4d;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xbc);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ae82c4;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af09e4;
      piVar6[0x29] = (int)&DAT_01af0aa4;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_GuildSet_01ae9954,8);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_0132ab80) {
      FUN_0132ab80();
      goto LAB_01283d3d;
    }
    break;
  case 0x4d:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb0);
    local_14 = 0x4e;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb0);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ae82c4;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af0ba0;
      piVar6[0x29] = (int)&DAT_01af0b94;
      piVar6[0x2b] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_RankView_01ae997c,8);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_0132ab80) {
      FUN_0132ab80();
      goto LAB_01283d3d;
    }
    break;
  case 0x4e:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb0);
    local_14 = 0x4f;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb0);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad391c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01ad385c;
      piVar6[0x29] = (int)&DAT_01ad3850;
      piVar6[0x2b] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_CashShopButton_01ae996c,0xe);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010bcba0) {
      FUN_010bcba0();
      goto LAB_01283d3d;
    }
    break;
  case 0x4f:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x110);
    local_14 = 0x52;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x110);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad7e4c;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x54);
      *piVar6 = (int)&DAT_01ad7d80;
      piVar6[0x29] = (int)&DAT_01ad7e40;
      iVar8 = DAT_01beee9c;
      piVar6[0x2c] = DAT_01beeea0;
      piVar6[0x2b] = iVar8;
      iVar8 = DAT_01bee88c;
      piVar6[0x2e] = DAT_01bee890;
      piVar6[0x2d] = iVar8;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      piVar6[0x38] = 0;
      piVar6[0x39] = 0;
      piVar6[0x3a] = 0;
      piVar6[0x3b] = 0;
      piVar6[0x3c] = 0;
      *(undefined1 *)(piVar6 + 0x3d) = 0;
      piVar6[0x3e] = 0;
      piVar6[0x3f] = 0;
      piVar6[0x40] = 0;
      piVar6[0x41] = 0;
      FUN_010fa620(0x18d,piVar6,FUN_010fa370,0);
      FUN_010fa620(0x18e,piVar6,FUN_010fa370,0);
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_TacticsHouse_01ae99b4,0xc);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010f49d0) {
      FUN_010f49d0();
      goto LAB_01283d3d;
    }
    break;
  case 0x50:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01288570();
    ExceptionList = local_1c;
    return piVar6;
  case 0x51:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xc0);
    local_14 = 0x55;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xc0);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01aed4d4;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af2f54;
      piVar6[0x29] = (int)&DAT_01af3014;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_JumpBuster_01ae99d0,10);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_0136b6a0) {
      FUN_0136b6a0();
      goto LAB_01283d3d;
    }
    break;
  case 0x52:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(200);
    local_14 = 0x56;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,200);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ae91a4;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af8e04;
      piVar6[0x29] = (int)&DAT_01af8df8;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_ChangeTamer_01ae99c4,0xb);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_013f5560) {
      FUN_013f5560();
      goto LAB_01283d3d;
    }
    break;
  case 0x53:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xc4);
    local_14 = 0x57;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xc4);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad3240;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af3c3c;
      piVar6[0x29] = (int)&DAT_01af3c30;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Tutorial_01ae99e8,8);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01383070) {
      FUN_01383070();
      goto LAB_01283d3d;
    }
    break;
  case 0x58:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xe8);
    local_14 = 0x59;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xe8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01af2c8c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af2bcc;
      piVar6[0x29] = (int)&DAT_01af2bc0;
      piVar6[0x2b] = 0;
      *(undefined1 *)(piVar6 + 0x2c) = 0;
      piVar6[0x2d] = 0x40c00000;
      piVar6[0x2e] = 0;
      *(undefined1 *)(piVar6 + 0x2f) = 1;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 7;
      *(undefined2 *)(piVar6 + 0x32) = 0;
      local_14 = CONCAT31(local_14._1_3_,0x5c);
      piVar6[0x38] = 0;
      piVar6[0x39] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x38] = (int)pvVar3;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Publicity_01ae99dc,9);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01369790) {
      FUN_01369790();
      goto LAB_01283d3d;
    }
    break;
  case 0x59:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb4);
    local_14 = 0x5d;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb4);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01af4c48;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af4b88;
      piVar6[0x29] = (int)&DAT_01af4b7c;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Channel_01ae9a00,7);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01395f50) {
      FUN_01395f50();
      goto LAB_01283d3d;
    }
    break;
  case 0x5a:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x104);
    local_14 = 0x5e;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x104);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01aed4d4;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x60);
      *piVar6 = (int)&DAT_01aed404;
      piVar6[0x29] = (int)&DAT_01aed4c8;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      *(undefined1 *)(piVar6 + 0x32) = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      *(undefined1 *)(piVar6 + 0x38) = 0;
      iVar8 = DAT_01bee890;
      piVar6[0x39] = DAT_01bee88c;
      piVar6[0x3a] = iVar8;
      piVar6[0x3b] = 0;
      piVar6[0x3c] = 0;
      piVar6[0x3d] = 0;
      piVar6[0x3e] = 0;
      piVar9 = piVar6 + 0x3f;
      *piVar9 = 0;
      piVar6[0x40] = 0;
      local_34 = piVar9;
      pvVar3 = operator_new(0x24);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      *(void **)((int)pvVar3 + 8) = pvVar3;
      *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
      *piVar9 = (int)pvVar3;
      piVar6 = local_30;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_SmallMap_01ae99f4,8);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_012eb5e0) {
      FUN_012eb5e0();
      goto LAB_01283d3d;
    }
    break;
  case 0x5b:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb4);
    local_14 = 0x61;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb4);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01aed974;
      piVar6[0x29] = (int)&DAT_01aeda34;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_WebWin_01ae9a14,6);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_012f5dd0) {
      FUN_012f5dd0();
      goto LAB_01283d3d;
    }
    break;
  case 0x5c:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xe8);
    local_14 = 0x62;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xe8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,100);
      *piVar6 = (int)&DAT_01aea88c;
      piVar6[0x29] = (int)&DAT_01aea94c;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x2f] = (int)pvVar3;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      piVar6[0x38] = 0;
      piVar6[0x39] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_DigimonTalk_01ae9a08,0xb);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010b92b0) {
      FUN_010b92b0();
      goto LAB_01283d3d;
    }
    break;
  case 0x5d:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0x124);
    local_14 = 0x65;
    local_28 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x124);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01af33fc;
      piVar6[0x2a] = 0;
      local_14._0_1_ = 0x67;
      *piVar6 = (int)&DAT_01af3330;
      piVar6[0x29] = (int)&DAT_01af33f0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 5;
      local_34 = piVar6 + 0x30;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      pvVar3 = operator_new(0xc);
      piVar9 = local_28;
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x32] = (int)pvVar3;
      *(undefined1 *)(piVar6 + 0x34) = 1;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      local_14 = CONCAT31(local_14._1_3_,0x68);
      local_34 = local_28 + 0x38;
      local_28[0x39] = 0;
      local_28[0x3a] = 0;
      local_28[0x3b] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar9[0x3a] = (int)pvVar3;
      *(undefined1 *)(piVar9 + 0x3c) = 1;
      piVar9[0x3d] = 0;
      piVar9[0x3e] = 0;
      piVar9[0x3f] = 0;
      local_28[0x40] = 0;
      local_28[0x41] = 0;
      local_28[0x42] = 0;
      local_28[0x43] = 0;
      piVar6 = local_28;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Protect_01ae9a24,7);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01371690) {
      FUN_01371690();
      goto LAB_01283d3d;
    }
    break;
  case 0x5e:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xe8);
    local_14 = 0x69;
    local_28 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xe8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01af1d70;
      piVar6[0x2a] = 0;
      local_14._0_1_ = 0x6b;
      *piVar6 = (int)&DAT_01af1ca4;
      piVar6[0x29] = (int)&DAT_01af1d64;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0;
      piVar6[0x33] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x32] = (int)pvVar3;
      local_14 = CONCAT31(local_14._1_3_,0x6c);
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      pvVar3 = operator_new(0xc);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      piVar6[0x34] = (int)pvVar3;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      piVar6[0x38] = 0;
      piVar6[0x39] = 0;
      FUN_00f5d420();
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_Achieve_01ae9a1c,7);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01349910) {
      FUN_01349910();
      goto LAB_01283d3d;
    }
    break;
  case 0x5f:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012887d0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x60:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01288ab0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x6e:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01288d00();
    ExceptionList = local_1c;
    return piVar6;
  case 0x6f:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01288ef0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x70:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012890e0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x72:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012891c0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x73:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012894f0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x74:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012895f0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x75:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01289780();
    ExceptionList = local_1c;
    return piVar6;
  case 0x76:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012899a0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x77:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01289ac0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x78:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01289be0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x79:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01289de0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x7a:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01289ef0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x7c:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128a000();
    ExceptionList = local_1c;
    return piVar6;
  case 0x7d:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128a220();
    ExceptionList = local_1c;
    return piVar6;
  case 0x7e:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128a370();
    ExceptionList = local_1c;
    return piVar6;
  case 0x83:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128a4a0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x84:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128a710();
    ExceptionList = local_1c;
    return piVar6;
  case 0x85:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128a830();
    ExceptionList = local_1c;
    return piVar6;
  case 0x86:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128a900();
    ExceptionList = local_1c;
    return piVar6;
  case 0x87:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128aa10();
    ExceptionList = local_1c;
    return piVar6;
  case 0x88:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ab40();
    ExceptionList = local_1c;
    return piVar6;
  case 0x89:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ac10();
    ExceptionList = local_1c;
    return piVar6;
  case 0x8a:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ad50();
    ExceptionList = local_1c;
    return piVar6;
  case 0x8b:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ae90();
    ExceptionList = local_1c;
    return piVar6;
  case 0x8c:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012892a0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x8d:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012893b0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x8e:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128af80();
    ExceptionList = local_1c;
    return piVar6;
  case 0x8f:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b0c0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x90:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b1b0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x91:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b2d0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x92:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b3e0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x93:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b4c0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x94:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b5c0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x95:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b6c0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x96:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    local_34 = (int *)FUN_016830c0(0x1e0);
    local_14 = 0x50;
    if (local_34 == (void *)0x0) {
      uVar11 = 0xb;
      piVar6 = (int *)0x0;
      pcVar10 = s_NewCashShop_01ae999c;
    }
    else {
      memset(local_34,0,0x1e0);
      piVar6 = (int *)FUN_0133c0b0();
      uVar11 = 0xb;
      pcVar10 = s_NewCashShop_01ae999c;
    }
LAB_01285497:
    local_14 = 0xffffffff;
    FUN_00f19f10(pcVar10,uVar11);
    (**(code **)(*piVar6 + 0x20))();
    goto LAB_01283d3d;
  case 0x97:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xbc);
    local_14 = 0x51;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xbc);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad391c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01af1a60;
      piVar6[0x29] = (int)&DAT_01af1b20;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_NewCashWarehouse_01ae9988,0x10);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_01349580) {
      FUN_01349580();
      goto LAB_01283d3d;
    }
    break;
  case 0x98:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b7b0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x99:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b8a0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x9a:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128b980();
    ExceptionList = local_1c;
    return piVar6;
  case 0x9b:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128bab0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x9c:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128bbd0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x9d:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128bcc0();
    ExceptionList = local_1c;
    return piVar6;
  case 0x9e:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128bd60();
    ExceptionList = local_1c;
    return piVar6;
  case 0x9f:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128bfa0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xa0:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128c0e0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xa1:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128c260();
    ExceptionList = local_1c;
    return piVar6;
  case 0xa2:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128c370();
    ExceptionList = local_1c;
    return piVar6;
  case 0xa3:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128c490();
    ExceptionList = local_1c;
    return piVar6;
  case 0xa4:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128c5d0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xa5:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128c7f0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xa6:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128c990();
    ExceptionList = local_1c;
    return piVar6;
  case 0xb5:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128caf0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xb6:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128cbd0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xb7:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ccb0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xb8:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128cd60();
    ExceptionList = local_1c;
    return piVar6;
  case 0xb9:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ced0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xba:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128cfd0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xbb:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128d0d0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xbc:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128d1f0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xbd:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128d2d0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xbe:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128d3d0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xbf:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128d4e0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xc0:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128d5c0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xc1:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128d710();
    ExceptionList = local_1c;
    return piVar6;
  case 0xc2:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128d890();
    ExceptionList = local_1c;
    return piVar6;
  case 0xc3:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128da20();
    ExceptionList = local_1c;
    return piVar6;
  case 0xc4:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128db50();
    ExceptionList = local_1c;
    return piVar6;
  case 0xc5:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128dc90();
    ExceptionList = local_1c;
    return piVar6;
  case 0xc6:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128dd80();
    ExceptionList = local_1c;
    return piVar6;
  case 199:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xc0);
    local_14 = 0;
    local_28 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xc0);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,2);
      *piVar6 = (int)&DAT_01ad34bc;
      piVar6[0x29] = (int)&DAT_01ad34b0;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar9 = piVar6 + 0x2d;
      *piVar9 = 0;
      piVar6[0x2e] = 0;
      local_30 = piVar9;
      pvVar3 = operator_new(0x1c);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      *(void **)((int)pvVar3 + 8) = pvVar3;
      *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
      *piVar9 = (int)pvVar3;
      local_28[0x2f] = 0;
      piVar6 = local_28;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_MainMenuUI_01ae979c,10);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010b92b0) {
      FUN_010b92b0();
      piVar6[0x1e] = param_2;
      ExceptionList = local_1c;
      return piVar6;
    }
    break;
  case 200:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xbc);
    local_14 = 3;
    local_28 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xbc);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      local_14 = CONCAT31(local_14._1_3_,5);
      *piVar6 = (int)&DAT_01ad35d0;
      piVar6[0x29] = (int)&DAT_01ad35c4;
      piVar6[0x2b] = 0;
      piVar9 = piVar6 + 0x2c;
      *piVar9 = 0;
      piVar6[0x2d] = 0;
      local_30 = piVar9;
      pvVar3 = operator_new(0x1c);
      *(void **)pvVar3 = pvVar3;
      *(void **)((int)pvVar3 + 4) = pvVar3;
      *(void **)((int)pvVar3 + 8) = pvVar3;
      *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
      *piVar9 = (int)pvVar3;
      *(undefined1 *)(local_28 + 0x2e) = 0;
      piVar6 = local_28;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_QuickMenuUI_01ae97b8,0xb);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010bab40) {
      FUN_010bab40();
      piVar6[0x1e] = param_2;
      ExceptionList = local_1c;
      return piVar6;
    }
    break;
  case 0xc9:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb0);
    local_14 = 6;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb0);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad357c;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01ad371c;
      piVar6[0x29] = (int)&DAT_01ad3710;
      piVar6[0x2b] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_QuickSubMenuUI_01ae97a8,0xe);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010bba60) {
      FUN_010bba60();
      piVar6[0x1e] = param_2;
      ExceptionList = local_1c;
      return piVar6;
    }
    break;
  case 0xca:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xb8);
    local_14 = 7;
    local_30 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xb8);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01ad3ff0;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01ad3f30;
      piVar6[0x29] = (int)&DAT_01ad3f24;
      piVar6[0x2b] = 0;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_VipMemberShipToolUI_01ae97d0,0x13);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_010c0da0) {
      FUN_010c0da0();
      piVar6[0x1e] = param_2;
      ExceptionList = local_1c;
      return piVar6;
    }
    break;
  case 0xcb:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_016830c0(0xec);
    local_14 = 0x58;
    local_34 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xec);
      FUN_01287ce0();
      piVar6[0x29] = (int)&DAT_01adb908;
      piVar6[0x2a] = 0;
      *piVar6 = (int)&DAT_01adbd10;
      piVar6[0x29] = (int)&DAT_01adbd04;
      piVar6[0x2b] = (int)&DAT_01ad1314;
      piVar6[0x2c] = 0;
      piVar6[0x2d] = 0;
      piVar6[0x2e] = 0;
      piVar6[0x2f] = 0;
      piVar6[0x30] = 0;
      piVar6[0x31] = 0;
      piVar6[0x32] = 0xf;
      *(undefined1 *)(piVar6 + 0x2d) = 0;
      piVar6[0x33] = 0;
      piVar6[0x34] = 0;
      piVar6[0x35] = 0;
      piVar6[0x36] = 0;
      piVar6[0x37] = 0;
      piVar6[0x38] = 0xf;
      *(undefined1 *)(piVar6 + 0x33) = 0;
      piVar6[0x39] = 0;
      piVar6[0x3a] = 0;
    }
    local_14 = 0xffffffff;
    FUN_00f19f10(s_TraceUI_01adbb04,7);
    pcVar4 = *(code **)(*piVar6 + 0x20);
    if (pcVar4 == FUN_0114a210) {
      FUN_0114a210();
      goto LAB_01283d3d;
    }
    break;
  case 0xcc:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e000();
    ExceptionList = local_1c;
    return piVar6;
  case 0xcd:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128dec0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xce:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e150();
    ExceptionList = local_1c;
    return piVar6;
  case 0xcf:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e280();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd0:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e370();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd1:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e470();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd2:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e580();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd3:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e670();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd4:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e760();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd5:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e870();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd6:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128e950();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd7:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ea50();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd8:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128eb50();
    ExceptionList = local_1c;
    return piVar6;
  case 0xd9:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ed20();
    ExceptionList = local_1c;
    return piVar6;
  case 0xda:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ee70();
    ExceptionList = local_1c;
    return piVar6;
  case 0xdb:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ef50();
    ExceptionList = local_1c;
    return piVar6;
  case 0xdc:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128f070();
    ExceptionList = local_1c;
    return piVar6;
  case 0xdd:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128f1b0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xde:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128f2f0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xdf:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128f3d0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe0:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128f590();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe1:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128f6f0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe2:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128f820();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe3:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128f980();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe4:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128fad0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe5:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128fcd0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe6:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128fe40();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe7:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_0128ffd0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe8:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01290620();
    ExceptionList = local_1c;
    return piVar6;
  case 0xe9:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01290160();
    ExceptionList = local_1c;
    return piVar6;
  case 0xea:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012902a0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xeb:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_01290490();
    ExceptionList = local_1c;
    return piVar6;
  case 0xed:
    ExceptionList = &local_1c;
    puStack_20 = &stack0xfffffffc;
    piVar6 = (int *)FUN_012907b0();
    ExceptionList = local_1c;
    return piVar6;
  case 0xee:
    ExceptionList = &local_1c;
    piVar6 = (int *)FUN_01290a20(DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
    ExceptionList = local_1c;
    return piVar6;
  }
  (*pcVar4)();
LAB_01283d3d:
  piVar6[0x1e] = param_2;
  ExceptionList = local_1c;
  return piVar6;
}


