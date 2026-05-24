
/* ============================================================ */
/* Function: FUN_01119160 */
/* Entry: 01119160 */
/* Signature: undefined FUN_01119160() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01119160(undefined1 *param_1)

{
  short sVar1;
  int iVar2;
  short *psVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined1 *puStack_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined8 *puStack_8c;
  short *local_88;
  undefined1 local_74 [24];
  int local_5c;
  undefined1 *local_58;
  undefined4 *local_54;
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
  puStack_c = &LAB_019866c5;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_88 = (short *)0x0;
  puStack_8c = (undefined8 *)0x18;
  uStack_90 = 0x66;
  uStack_94 = 0x20;
  local_98 = 5;
  puStack_9c = param_1;
  local_58 = param_1;
  iVar2 = FUN_01246da0(s_DigimonStatus_New_Digimon_Status_01ad92a8);
  *(int *)(param_1 + 0xb0) = iVar2;
  if (iVar2 != 0) {
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
    local_20 = 0xb;
    local_48 = DAT_01bb9b34;
    local_2c = 1;
    local_1c = 0;
    local_28 = 1;
    puStack_9c = (undefined1 *)0x27;
    local_98 = 0;
    uStack_94 = 0;
    uStack_90 = 0;
    puStack_8c = (undefined8 *)0x0;
    local_88 = (short *)0x0;
    FUN_00f19cc0(s_DIGIMONTRANSCENDHELP_EXPLAIN_TAB_01ad931c);
    puStack_9c = local_74;
    psVar3 = (short *)FUN_015ce480();
    local_8._0_1_ = 1;
    local_88 = psVar3;
    if (7 < *(uint *)(psVar3 + 10)) {
      psVar3 = *(short **)psVar3;
      local_88 = psVar3;
    }
    do {
      sVar1 = *psVar3;
      psVar3 = psVar3 + 1;
    } while (sVar1 != 0);
    puStack_8c = (undefined8 *)0x1119270;
    FUN_00f1a040();
    local_8 = (uint)local_8._1_3_ << 8;
    FUN_00f1a330();
    puStack_8c = &local_50;
    local_88 = (short *)DAT_01bee88c;
    uStack_90 = 0x1119297;
    FUN_01226310();
    local_5c = *(int *)(param_1 + 0xb0);
    local_88 = (short *)0x11192a7;
    puVar4 = operator_new(8);
    *puVar4 = &DAT_01ad943c;
    puVar4[1] = 0;
    local_54 = puVar4;
    if (*(undefined4 **)(local_5c + 0x34) != (undefined4 *)0x0) {
      local_88 = (short *)0x11192cc;
      (**(code **)**(undefined4 **)(local_5c + 0x34))();
    }
    local_88 = (short *)&LAB_011198d0;
    *(undefined4 **)(local_5c + 0x34) = puVar4;
    uStack_90 = 0;
    uStack_94 = 0x11192e7;
    puStack_8c = (undefined8 *)param_1;
    FUN_01119930();
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_DIGIMONTRANSCENDHELP_EXPLAIN_TAB_01ad92f4,0x26);
    FUN_015ce480(&puStack_9c,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
    FUN_01119420(0x6b,0x20,0x66,0x18,0,0x18);
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_DIGIMONTRANSCENDHELP_EXPLAIN_TAB_01ad936c,0x29);
    FUN_015ce480(&puStack_9c,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
    FUN_01119420(0xd1,0x20,0x66,0x18,0,0x18);
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_DIGIMONTRANSCENDHELP_EXPLAIN_TAB_01ad9344,0x27);
    FUN_015ce480(&puStack_9c,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
    FUN_01119420(0x137,0x20,0x66,0x18,0,0x18);
    local_88 = (short *)0x0;
    puStack_8c = (undefined8 *)0x11193e8;
    FUN_01227420();
    local_24 = 0;
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


