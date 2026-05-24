
/* ============================================================ */
/* Function: FUN_012faae0 */
/* Entry: 012faae0 */
/* Signature: undefined FUN_012faae0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_012faae0(int param_1)

{
  short sVar1;
  int iVar2;
  undefined4 *puVar3;
  short ****ppppsVar4;
  short ****ppppsVar5;
  float fVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  short **local_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 local_7c;
  undefined4 uStack_78;
  int local_74;
  float local_70;
  undefined4 *local_6c;
  undefined8 local_68;
  undefined4 local_60;
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
  short ***local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019a42b5;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar2 = FUN_01246da0(s_NewInventory_new_inventory_btn_t_01aedbf4,param_1,5,0x20,0x40,0x17,0,0x17);
  *(int *)(param_1 + 0x89c) = iVar2;
  if (iVar2 != 0) {
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    FUN_00f19cc0(s_INVENTORY_FILTER_TEXT_NONE_01ae2310,0x1a);
    FUN_015ce480(&local_2c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    local_4c = 0;
    uStack_58 = 0;
    uStack_54 = 0;
    uStack_50 = 0;
    local_48 = 7;
    local_5c = 0;
    local_30 = 0;
    local_8 = 1;
    local_3c = DAT_01bfe468 + 8;
    ppppsVar5 = &local_2c;
    if (7 < local_1c._4_4_) {
      ppppsVar5 = (short ****)local_2c;
    }
    local_38 = 9;
    local_68 = DAT_01bb9b2c;
    local_60 = DAT_01bb9b34;
    local_44 = 1;
    local_34 = 0;
    local_40 = 0;
    ppppsVar4 = ppppsVar5;
    do {
      sVar1 = *(short *)ppppsVar4;
      ppppsVar4 = (short ****)((int)ppppsVar4 + 2);
    } while (sVar1 != 0);
    FUN_00f1a040(ppppsVar5,(int)ppppsVar4 - (int)((int)ppppsVar5 + 2) >> 1);
    local_40 = 1;
    FUN_01226310(&local_68,DAT_01bee88c,DAT_01bee890);
    FUN_012fb940(0,param_1,FUN_012fb210,0);
    local_74 = *(int *)(param_1 + 0x89c);
    puVar3 = operator_new(8);
    *puVar3 = &DAT_01aedc44;
    puVar3[1] = 0;
    local_6c = puVar3;
    if (*(undefined4 **)(local_74 + 0x34) != (undefined4 *)0x0) {
      (**(code **)**(undefined4 **)(local_74 + 0x34))(1);
    }
    *(undefined4 **)(local_74 + 0x34) = puVar3;
    iVar2 = *(int *)(param_1 + 0x89c);
    FUN_0124eff0(&local_70);
    local_70 = local_70 * DAT_01b024f4;
    *(undefined4 *)(iVar2 + 0xec) = 0x3f800000;
    fVar6 = (float)local_6c * _DAT_01b028e4;
    *(float *)(iVar2 + 0xe4) = local_70 + local_70;
    *(float *)(iVar2 + 0xf0) = fVar6 * DAT_01b02858;
    *(float *)(iVar2 + 0xe8) = fVar6 + fVar6;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    FUN_00f19cc0(s_INVENTORY_FILTER_TEXT_EQUIP_01ae2e58,0x1b);
    FUN_015ce480(&local_8c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    FUN_00f1a330();
    local_2c = (short ***)local_8c;
    local_8c = (short **)((uint)local_8c & 0xffff0000);
    uStack_28 = uStack_88;
    uStack_24 = uStack_84;
    uStack_20 = uStack_80;
    local_7c = 0;
    uStack_78 = 7;
    uVar12 = 0x12fad43;
    FUN_00f1a330();
    FUN_00f221a0(&local_2c);
    FUN_012faf80(1,0x46,0x20,0x40,0x17,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    FUN_00f19cc0(s_INVENTORY_FILTER_TEXT_CONSUME_01aea9f8,0x1d);
    FUN_015ce480(&local_8c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    FUN_00f1a330();
    local_2c = (short ***)local_8c;
    local_8c = (short **)((uint)local_8c & 0xffff0000);
    uStack_28 = uStack_88;
    uStack_24 = uStack_84;
    uStack_20 = uStack_80;
    local_1c = CONCAT44(uStack_78,local_7c);
    local_7c = 0;
    uStack_78 = 7;
    uVar12 = 0x12fadda;
    FUN_00f1a330();
    FUN_00f221a0(&local_2c);
    FUN_012faf80(2,0x87,0x20,0x40,0x17,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    FUN_00f19cc0(s_INVENTORY_FILTER_TEXT_DIGIMON_01aeaa50,0x1d);
    FUN_015ce480(&local_8c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    FUN_00f1a330();
    local_2c = (short ***)local_8c;
    local_8c = (short **)((uint)local_8c & 0xffff0000);
    uStack_28 = uStack_88;
    uStack_24 = uStack_84;
    uStack_20 = uStack_80;
    local_1c = CONCAT44(uStack_78,local_7c);
    local_7c = 0;
    uStack_78 = 7;
    uVar12 = 0x12fae74;
    FUN_00f1a330();
    FUN_00f221a0(&local_2c);
    FUN_012faf80(3,200,0x20,0x40,0x17,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    FUN_00f19cc0(s_INVENTORY_FILTER_TEXT_ETC_01aeaa34,0x19);
    FUN_015ce480(&local_8c,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    FUN_00f1a330();
    local_2c = (short ***)local_8c;
    local_8c = (short **)((uint)local_8c & 0xffff0000);
    uStack_28 = uStack_88;
    uStack_24 = uStack_84;
    uStack_20 = uStack_80;
    local_1c = CONCAT44(uStack_78,local_7c);
    local_7c = 0;
    uStack_78 = 7;
    uVar12 = 0x12faf0e;
    FUN_00f1a330();
    FUN_00f221a0(&local_2c);
    FUN_012faf80(4,0x109,0x20,0x40,0x17,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    FUN_01227420(0,0);
    local_3c = 0;
    FUN_00f1a330();
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


