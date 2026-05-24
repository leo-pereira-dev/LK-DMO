
/* ============================================================ */
/* Function: FUN_012cfc40 */
/* Entry: 012cfc40 */
/* Signature: undefined FUN_012cfc40() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_012cfc40(int param_1)

{
  undefined4 *****pppppuVar1;
  uint uVar2;
  int iVar3;
  undefined4 *****pppppuVar4;
  undefined4 ****ppppuVar5;
  undefined4 *****pppppuVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined2 *puVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 ******ppppppuVar12;
  undefined4 uVar13;
  undefined4 ******ppppppuVar14;
  undefined4 uVar15;
  undefined1 local_70 [8];
  undefined4 ****local_68;
  undefined4 *****local_64;
  undefined4 *****local_60;
  undefined4 *****local_5c;
  undefined4 *****local_58;
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 *****local_3c [5];
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019a1c05;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  if (DAT_01bfb2a8 == 0) {
    return;
  }
  if (*(int *)(DAT_01bfb2a8 + 0x14) == 0) {
    return;
  }
  local_5c = *(undefined4 ******)(*(int *)(DAT_01bfb2a8 + 0x14) + 0x344);
  if ((undefined4 ******)local_5c == (undefined4 ******)0x0) {
    return;
  }
  if (*(undefined4 **)(param_1 + 0x16c) == (undefined4 *)0x0) {
    return;
  }
  if (*(int *)(param_1 + 0x198) == 0) {
    return;
  }
  local_60 = (undefined4 *****)**(undefined4 **)(param_1 + 0x16c);
  ExceptionList = &local_1c;
  local_24 = uVar2;
  iVar3 = FUN_015db910(&local_60);
  if (iVar3 == 0) {
    ExceptionList = local_1c;
    return;
  }
  if (*(short *)(iVar3 + 0x18) != 1) {
    switch(*(undefined4 *)(param_1 + 0x178)) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      ExceptionList = local_1c;
      return;
    }
    FUN_013be010(0,0,0,0,uVar2);
    ExceptionList = local_1c;
    return;
  }
  uVar7 = 0;
  uVar8 = 0;
  uVar10 = 0;
  uVar11 = 0;
  uVar13 = 0;
  uVar15 = 0;
  FUN_00f19cc0(s_ENCHANTTACTICS_ENCHANT_LEVEL_01aebd1c,0x1c);
  FUN_015ce480(local_3c,uVar7,uVar8,uVar10,uVar11,uVar13,uVar15);
  pppppuVar1 = local_5c;
  local_44 = 0;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_40 = 7;
  local_54 = 0;
  local_14 = 1;
  switch(*(undefined4 *)(param_1 + 0x178)) {
  case 0:
    local_64 = local_3c;
    if (7 < local_28) {
      local_64 = local_3c[0];
    }
    local_60 = (undefined4 *****)0x0;
    FUN_00f1d720(local_70,&local_60);
    if ((*(char *)((int)local_68 + 0xd) != '\0') ||
       (pppppuVar4 = (undefined4 *****)local_68, 0 < (int)local_68[4])) {
      pppppuVar4 = (undefined4 *****)pppppuVar1[1];
    }
    pppppuVar1 = (undefined4 *****)pppppuVar1[1];
    pppppuVar6 = (undefined4 *****)0x0;
    if (pppppuVar4 != pppppuVar1) {
      pppppuVar6 = pppppuVar4 + 5;
    }
    if (pppppuVar6 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
    }
    else {
      ppppuVar5 = *pppppuVar6;
    }
    local_5c = (undefined4 *****)((int)ppppuVar5 + -1);
    local_60 = (undefined4 ******)0x0;
    local_58 = local_3c;
    if (7 < local_28) {
      local_58 = local_3c[0];
    }
    FUN_00f1d720(local_70,&local_60);
    if ((*(char *)((int)local_68 + 0xd) != '\0') || (0 < (int)local_68[4])) {
      local_68 = pppppuVar1;
    }
    pppppuVar4 = (undefined4 *****)0x0;
    if ((undefined4 *****)local_68 != pppppuVar1) {
      pppppuVar4 = (undefined4 *****)(local_68 + 5);
    }
    ppppppuVar12 = (undefined4 ******)local_58;
    ppppppuVar14 = (undefined4 ******)local_5c;
    if (pppppuVar4 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
      puVar9 = &DAT_01add8a0;
    }
    else {
      ppppuVar5 = *pppppuVar4;
      puVar9 = &DAT_01add8a0;
    }
    break;
  case 1:
    local_64 = local_3c;
    if (7 < local_28) {
      local_64 = local_3c[0];
    }
    local_58 = (undefined4 *****)0x1;
    FUN_00f1d720(local_70,&local_58);
    if ((*(char *)((int)local_68 + 0xd) != '\0') ||
       (pppppuVar4 = (undefined4 *****)local_68, 1 < (int)local_68[4])) {
      pppppuVar4 = (undefined4 *****)pppppuVar1[1];
    }
    pppppuVar1 = (undefined4 *****)pppppuVar1[1];
    pppppuVar6 = (undefined4 *****)0x0;
    if (pppppuVar4 != pppppuVar1) {
      pppppuVar6 = pppppuVar4 + 5;
    }
    if (pppppuVar6 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
    }
    else {
      ppppuVar5 = *pppppuVar6;
    }
    local_60 = (undefined4 *****)((int)ppppuVar5 + -1);
    local_58 = (undefined4 ******)0x1;
    local_5c = local_3c;
    if (7 < local_28) {
      local_5c = local_3c[0];
    }
    FUN_00f1d720(local_70,&local_58);
    if ((*(char *)((int)local_68 + 0xd) != '\0') || (1 < (int)local_68[4])) {
      local_68 = pppppuVar1;
    }
    pppppuVar4 = (undefined4 *****)0x0;
    if ((undefined4 *****)local_68 != pppppuVar1) {
      pppppuVar4 = (undefined4 *****)(local_68 + 5);
    }
    ppppppuVar12 = (undefined4 ******)local_5c;
    ppppppuVar14 = (undefined4 ******)local_60;
    if (pppppuVar4 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
      puVar9 = &DAT_01add8c4;
    }
    else {
      ppppuVar5 = *pppppuVar4;
      puVar9 = &DAT_01add8c4;
    }
    break;
  case 2:
    local_64 = local_3c;
    if (7 < local_28) {
      local_64 = local_3c[0];
    }
    local_58 = (undefined4 *****)0x2;
    FUN_00f1d720(local_70,&local_58);
    if ((*(char *)((int)local_68 + 0xd) != '\0') ||
       (pppppuVar4 = (undefined4 *****)local_68, 2 < (int)local_68[4])) {
      pppppuVar4 = (undefined4 *****)pppppuVar1[1];
    }
    pppppuVar1 = (undefined4 *****)pppppuVar1[1];
    pppppuVar6 = (undefined4 *****)0x0;
    if (pppppuVar4 != pppppuVar1) {
      pppppuVar6 = pppppuVar4 + 5;
    }
    if (pppppuVar6 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
    }
    else {
      ppppuVar5 = *pppppuVar6;
    }
    local_60 = (undefined4 *****)((int)ppppuVar5 + -1);
    local_58 = (undefined4 ******)0x2;
    local_5c = local_3c;
    if (7 < local_28) {
      local_5c = local_3c[0];
    }
    FUN_00f1d720(local_70,&local_58);
    if ((*(char *)((int)local_68 + 0xd) != '\0') || (2 < (int)local_68[4])) {
      local_68 = pppppuVar1;
    }
    pppppuVar4 = (undefined4 *****)0x0;
    if ((undefined4 *****)local_68 != pppppuVar1) {
      pppppuVar4 = (undefined4 *****)(local_68 + 5);
    }
    ppppppuVar12 = (undefined4 ******)local_5c;
    ppppppuVar14 = (undefined4 ******)local_60;
    if (pppppuVar4 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
      puVar9 = &DAT_01add8b4;
    }
    else {
      ppppuVar5 = *pppppuVar4;
      puVar9 = &DAT_01add8b4;
    }
    break;
  case 3:
    local_64 = local_3c;
    if (7 < local_28) {
      local_64 = local_3c[0];
    }
    local_58 = (undefined4 *****)0x4;
    FUN_00f1d720(local_70,&local_58);
    if ((*(char *)((int)local_68 + 0xd) != '\0') ||
       (pppppuVar4 = (undefined4 *****)local_68, 4 < (int)local_68[4])) {
      pppppuVar4 = (undefined4 *****)pppppuVar1[1];
    }
    pppppuVar1 = (undefined4 *****)pppppuVar1[1];
    pppppuVar6 = (undefined4 *****)0x0;
    if (pppppuVar4 != pppppuVar1) {
      pppppuVar6 = pppppuVar4 + 5;
    }
    if (pppppuVar6 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
    }
    else {
      ppppuVar5 = *pppppuVar6;
    }
    local_60 = (undefined4 *****)((int)ppppuVar5 + -1);
    local_58 = (undefined4 *****)&DAT_00000004;
    local_5c = local_3c;
    if (7 < local_28) {
      local_5c = local_3c[0];
    }
    FUN_00f1d720(local_70,&local_58);
    if ((*(char *)((int)local_68 + 0xd) != '\0') || (4 < (int)local_68[4])) {
      local_68 = pppppuVar1;
    }
    pppppuVar4 = (undefined4 *****)0x0;
    if ((undefined4 *****)local_68 != pppppuVar1) {
      pppppuVar4 = (undefined4 *****)(local_68 + 5);
    }
    ppppppuVar12 = (undefined4 ******)local_5c;
    ppppppuVar14 = (undefined4 ******)local_60;
    if (pppppuVar4 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
      puVar9 = &DAT_01add8bc;
    }
    else {
      ppppuVar5 = *pppppuVar4;
      puVar9 = &DAT_01add8bc;
    }
    break;
  case 4:
    local_64 = local_3c;
    if (7 < local_28) {
      local_64 = local_3c[0];
    }
    local_58 = (undefined4 *****)0x6;
    FUN_00f1d720(local_70,&local_58);
    if ((*(char *)((int)local_68 + 0xd) != '\0') ||
       (pppppuVar4 = (undefined4 *****)local_68, 6 < (int)local_68[4])) {
      pppppuVar4 = (undefined4 *****)pppppuVar1[1];
    }
    pppppuVar1 = (undefined4 *****)pppppuVar1[1];
    pppppuVar6 = (undefined4 *****)0x0;
    if (pppppuVar4 != pppppuVar1) {
      pppppuVar6 = pppppuVar4 + 5;
    }
    if (pppppuVar6 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
    }
    else {
      ppppuVar5 = *pppppuVar6;
    }
    local_60 = (undefined4 *****)((int)ppppuVar5 + -1);
    local_58 = (undefined4 ******)0x6;
    local_5c = local_3c;
    if (7 < local_28) {
      local_5c = local_3c[0];
    }
    FUN_00f1d720(local_70,&local_58);
    if ((*(char *)((int)local_68 + 0xd) != '\0') || (6 < (int)local_68[4])) {
      local_68 = pppppuVar1;
    }
    pppppuVar4 = (undefined4 *****)0x0;
    if ((undefined4 *****)local_68 != pppppuVar1) {
      pppppuVar4 = (undefined4 *****)(local_68 + 5);
    }
    if (pppppuVar4 == (undefined4 *****)0x0) {
      ppppuVar5 = (undefined4 ****)0x0;
    }
    else {
      ppppuVar5 = *pppppuVar4;
    }
    puVar9 = (undefined2 *)&DAT_01aebd3c;
    ppppppuVar12 = (undefined4 ******)local_5c;
    ppppppuVar14 = (undefined4 ******)local_60;
    break;
  default:
    goto switchD_012cfd51_default;
  }
  FUN_01500f80(&local_54,u__s___d_s__>__d_s__01aebcf8,puVar9,ppppuVar5,ppppppuVar12,ppppppuVar14,
               local_64);
  FUN_013bdff0(0,0,0,0);
switchD_012cfd51_default:
  FUN_00f1a330();
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


