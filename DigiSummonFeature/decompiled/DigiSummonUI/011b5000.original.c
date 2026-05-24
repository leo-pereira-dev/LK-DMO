
/* ============================================================ */
/* Function: FUN_011b5000 */
/* Entry: 011b5000 */
/* Signature: undefined FUN_011b5000() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011b5000(int param_1)

{
  undefined4 *puVar1;
  void *pvVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 ***pppuVar5;
  undefined4 uVar6;
  undefined4 extraout_ECX;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 local_fc [20];
  undefined4 local_e8;
  undefined4 **local_e0;
  undefined4 uStack_dc;
  undefined4 uStack_d8;
  undefined4 uStack_d4;
  undefined4 local_d0;
  uint local_cc;
  undefined4 **local_c8;
  undefined4 uStack_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  undefined4 local_b8;
  uint local_b4;
  undefined4 **local_b0;
  undefined4 uStack_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  undefined4 local_a0;
  uint local_9c;
  undefined4 **local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 local_88;
  uint local_84;
  undefined4 **local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 local_70;
  uint local_6c;
  void *local_68;
  undefined1 local_61;
  undefined8 local_60;
  undefined4 local_58;
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
  puStack_18 = &LAB_01990f76;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_68 = (void *)FUN_016830c0(0x1ac,local_24);
  local_14 = 0;
  if (local_68 == (void *)0x0) {
    iVar3 = 0;
  }
  else {
    memset(local_68,0,0x1ac);
    iVar3 = FUN_012435e0();
  }
  uVar6 = DAT_01b0273c;
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0x8c) = iVar3;
  if (iVar3 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 1;
    local_34 = DAT_01bfe468 + 8;
    local_e8 = 0x3f19999a;
    local_30 = 0xb;
    local_60 = CONCAT44(DAT_01b026d8,uVar6);
    local_58 = 0x3f19999a;
    local_3c = 1;
    local_2c = 0;
    local_38 = 4;
    FUN_00f1a040(&DAT_01ac5dd8,0);
    (**(code **)(**(int **)(param_1 + 0x8c) + 0xc))
              (*(undefined4 *)(param_1 + 0x40),0x32,0x1e,0x78,0x18,0);
    FUN_01242a50(&local_60);
    FUN_01242f20(1);
    local_70 = 0;
    *(undefined4 *)(*(int *)(param_1 + 0x8c) + 0xfc) = 1;
    local_80 = (undefined4 ***)0x0;
    uStack_7c = 0;
    uStack_78 = 0;
    uStack_74 = 0;
    local_6c = 0;
    *(undefined4 *)(*(int *)(param_1 + 0x8c) + 0xf8) = 1;
    local_68 = *(void **)(param_1 + 0x8c);
    FUN_00f1a200(&DAT_01ac5dd8,0);
    local_14._0_1_ = 2;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    FUN_00f19cc0(s_DIGISUMMON_TIME_REMAINING_01ae17d0,0x19);
    puVar4 = (undefined4 *)FUN_015ce480(local_fc,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    pvVar2 = local_68;
    local_14 = CONCAT31(local_14._1_3_,3);
    if ((undefined4 *)((int)local_68 + 0x100) != puVar4) {
      puVar1 = puVar4 + 4;
      if (7 < (uint)puVar4[5]) {
        puVar4 = (undefined4 *)*puVar4;
      }
      FUN_00f1a040(puVar4,*puVar1);
    }
    if ((undefined4 ***)((int)pvVar2 + 0x118) != &local_80) {
      pppuVar5 = &local_80;
      if (7 < local_6c) {
        pppuVar5 = (undefined4 ***)local_80;
      }
      FUN_00f1a040(pppuVar5,local_70);
    }
    FUN_00f1a330();
    local_14._0_1_ = 1;
    FUN_00f1a330();
    iVar3 = *(int *)(param_1 + 0x8c);
    local_d0 = 0;
    local_e0 = (undefined4 ***)0x0;
    uStack_dc = 0;
    uStack_d8 = 0;
    uStack_d4 = 0;
    local_cc = 0;
    FUN_00f1a200(&DAT_01ac5dd8,0);
    local_14._0_1_ = 4;
    local_b8 = 0;
    local_c8 = (undefined4 ***)0x0;
    uStack_c4 = 0;
    uStack_c0 = 0;
    uStack_bc = 0;
    local_b4 = 0;
    FUN_00f1a200(&DAT_01acbba0,3);
    local_14._0_1_ = 5;
    local_a0 = 0;
    local_b0 = (undefined4 ***)0x0;
    uStack_ac = 0;
    uStack_a8 = 0;
    uStack_a4 = 0;
    local_9c = 0;
    FUN_00f1a200(&DAT_01acbba0,3);
    local_14._0_1_ = 6;
    local_88 = 0;
    local_98 = (undefined4 ***)0x0;
    uStack_94 = 0;
    uStack_90 = 0;
    uStack_8c = 0;
    local_84 = 0;
    FUN_00f1a200(&DAT_01ac5dd8,0);
    local_14 = CONCAT31(local_14._1_3_,7);
    if ((undefined4 ***)(iVar3 + 400) != &local_98) {
      pppuVar5 = &local_98;
      if (7 < local_84) {
        pppuVar5 = (undefined4 ***)local_98;
      }
      FUN_00f1a040(pppuVar5,local_88);
    }
    if ((undefined4 ***)(iVar3 + 0x148) != &local_b0) {
      pppuVar5 = &local_b0;
      if (7 < local_9c) {
        pppuVar5 = (undefined4 ***)local_b0;
      }
      FUN_00f1a040(pppuVar5,local_a0);
    }
    if ((undefined4 ***)(iVar3 + 0x160) != &local_c8) {
      pppuVar5 = &local_c8;
      if (7 < local_b4) {
        pppuVar5 = (undefined4 ***)local_c8;
      }
      FUN_00f1a040(pppuVar5,local_b8);
    }
    if ((undefined4 ***)(iVar3 + 0x178) != &local_e0) {
      pppuVar5 = &local_e0;
      if (7 < local_cc) {
        pppuVar5 = (undefined4 ***)local_e0;
      }
      FUN_00f1a040(pppuVar5,local_d0);
    }
    FUN_00f1a330();
    FUN_00f1a330();
    FUN_00f1a330();
    local_14 = CONCAT31(local_14._1_3_,1);
    FUN_00f1a330();
    FUN_011b58b0(extraout_ECX,param_1,&LAB_011b56d0,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x8c));
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  local_68 = (void *)0x0;
  local_61 = 0;
  FUN_011b5610(&local_61,&local_68);
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = 0;
  local_28 = 0;
  local_14 = 8;
  local_34 = DAT_01bfe468 + 8;
  local_e8 = 0x3f19999a;
  local_30 = 0xb;
  local_60 = CONCAT44(DAT_01b026d8,uVar6);
  local_58 = 0x3f19999a;
  local_3c = 1;
  local_2c = 0;
  local_38 = 4;
  FUN_00f1a040(&DAT_01ac5dd8,0);
  uVar6 = FUN_01246bb0(&local_60,1,0x32,0x41);
  *(undefined4 *)(param_1 + 0x90) = uVar6;
  local_34 = 0;
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


