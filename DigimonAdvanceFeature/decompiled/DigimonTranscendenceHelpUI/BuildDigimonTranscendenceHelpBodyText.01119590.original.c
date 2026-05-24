
/* ============================================================ */
/* Function: FUN_01119590 */
/* Entry: 01119590 */
/* Signature: undefined FUN_01119590() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01119590(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined4 ***pppuVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined1 auStack_d8 [24];
  undefined1 auStack_c0 [24];
  undefined1 auStack_a8 [24];
  undefined1 auStack_90 [32];
  undefined1 *local_70;
  undefined8 local_68;
  undefined1 *local_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined1 local_44;
  undefined4 local_40;
  int local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 **local_2c;
  undefined4 *puStack_28;
  undefined4 *puStack_24;
  undefined4 *puStack_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01986745;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar3;
  FUN_01232880(1);
  local_1c = 0x700000000;
  puStack_28 = (undefined4 **)0x0;
  puStack_24 = (undefined4 **)0x0;
  puStack_20 = (undefined4 **)0x0;
  local_2c = (undefined4 ***)0x0;
  local_8 = 0;
  switch(param_2) {
  case 0:
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_DIGIMONTRANSCENDHELP_EXPLAIN_SYS_01ad93bc,0x23);
    pppuVar4 = (undefined4 ***)FUN_015ce480(auStack_90,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
    if (&local_2c != pppuVar4) {
      FUN_00f1a330(uVar3);
      local_2c = *pppuVar4;
      puStack_28 = pppuVar4[1];
      puStack_24 = pppuVar4[2];
      puStack_20 = pppuVar4[3];
      local_1c = *(undefined8 *)(pppuVar4 + 4);
      pppuVar4[4] = (undefined4 **)0x0;
      pppuVar4[5] = (undefined4 **)0x7;
      *(undefined2 *)pppuVar4 = 0;
    }
    break;
  case 1:
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_DIGIMONTRANSCENDHELP_EXPLAIN_TER_01ad9398,0x22);
    pppuVar4 = (undefined4 ***)FUN_015ce480(auStack_a8,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
    if (&local_2c != pppuVar4) {
      FUN_00f1a330(uVar3);
      local_2c = *pppuVar4;
      puStack_28 = pppuVar4[1];
      puStack_24 = pppuVar4[2];
      puStack_20 = pppuVar4[3];
      local_1c = *(undefined8 *)(pppuVar4 + 4);
      pppuVar4[4] = (undefined4 **)0x0;
      pppuVar4[5] = (undefined4 **)0x7;
      *(undefined2 *)pppuVar4 = 0;
    }
    break;
  case 2:
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_DIGIMONTRANSCENDHELP_EXPLAIN_PRO_01ad9404,0x25);
    pppuVar4 = (undefined4 ***)FUN_015ce480(auStack_c0,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
    if (&local_2c != pppuVar4) {
      FUN_00f1a330(uVar3);
      local_2c = *pppuVar4;
      puStack_28 = pppuVar4[1];
      puStack_24 = pppuVar4[2];
      puStack_20 = pppuVar4[3];
      local_1c = *(undefined8 *)(pppuVar4 + 4);
      pppuVar4[4] = (undefined4 **)0x0;
      pppuVar4[5] = (undefined4 **)0x7;
      *(undefined2 *)pppuVar4 = 0;
    }
    break;
  case 3:
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_DIGIMONTRANSCENDHELP_EXPLAIN_CHA_01ad93e0,0x23);
    pppuVar4 = (undefined4 ***)FUN_015ce480(auStack_d8,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
    if (&local_2c != pppuVar4) {
      FUN_00f1a330(uVar3);
      local_2c = *pppuVar4;
      puStack_28 = pppuVar4[1];
      puStack_24 = pppuVar4[2];
      puStack_20 = pppuVar4[3];
      local_1c = *(undefined8 *)(pppuVar4 + 4);
      pppuVar4[4] = (undefined4 **)0x0;
      pppuVar4[5] = (undefined4 **)0x7;
      *(undefined2 *)pppuVar4 = 0;
    }
    break;
  default:
    goto switchD_011195f6_default;
  }
  FUN_00f1a330();
switchD_011195f6_default:
  local_4c = 0;
  local_48 = 7;
  uStack_58 = 0;
  uStack_54 = 0;
  uStack_50 = 0;
  local_5c = 0;
  local_30 = 0;
  local_8 = CONCAT31(local_8._1_3_,1);
  local_3c = DAT_01bfe468 + 8;
  local_70 = &DAT_3f7ae148;
  local_60 = &DAT_3f7ae148;
  pppuVar4 = &local_2c;
  if (7 < local_1c._4_4_) {
    pppuVar4 = (undefined4 ***)local_2c;
  }
  local_68 = CONCAT44(DAT_01b02744,DAT_01b02744);
  local_38 = 0xb;
  local_44 = 1;
  local_34 = 0;
  local_40 = 0;
  FUN_012543f0(param_1 + 0xb8,0x15e,pppuVar4,&local_68);
  iVar1 = *(int *)(param_1 + 0xb4);
  if (iVar1 != 0) {
    *(int *)(param_1 + 0xcc) = iVar1;
    iVar2 = *(int *)(param_1 + 0xc4);
    if ((*(int *)(iVar1 + 0x78) != 0) || (*(int *)(iVar1 + 0x7c) != iVar2)) {
      *(undefined4 *)(iVar1 + 0x78) = 0;
      *(int *)(iVar1 + 0x7c) = iVar2;
      FUN_0124a620();
    }
  }
  local_3c = 0;
  FUN_00f1a330(uVar3);
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


