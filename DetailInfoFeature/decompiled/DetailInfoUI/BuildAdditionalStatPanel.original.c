
/* ============================================================ */
/* Function: FUN_011d12b0 */
/* Entry: 011d12b0 */
/* Signature: undefined FUN_011d12b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011d12b0(int param_1,undefined4 param_2)

{
  uint uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 extraout_ECX;
  undefined4 ***pppuVar4;
  undefined8 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 local_4c [20];
  undefined1 *local_38;
  undefined4 *local_34;
  int local_30;
  undefined4 **local_2c [5];
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01990fb5;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar1;
  FUN_012459a0(param_2,s_DetailInfo_skin_bg_a_tga_01ae2a3c,1,0,0,1,0x1cf,0,300,0x1e9);
  uVar3 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_DETAIL_INFO_ADDITION_01ae2a24,0x14);
  FUN_015ce480(local_2c,uVar3,uVar5,uVar6,uVar7,uVar8);
  local_8 = 0;
  pppuVar4 = local_2c;
  if (7 < local_18) {
    pppuVar4 = (undefined4 ***)local_2c[0];
  }
  local_38 = &DAT_3f7ae148;
  FUN_012461a0(pppuVar4,0xb,CONCAT44(DAT_01b02744,DAT_01b02744),&DAT_3f7ae148,DAT_01bee88c,
               DAT_01bee890);
  FUN_011d14a0(uVar1);
  FUN_011d14f0();
  uVar3 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_COMMON_ATTRIBUTE_STAT_01ae2a58,0x15);
  puVar2 = (undefined4 *)FUN_015ce480(local_4c,uVar3,uVar5,uVar6,uVar7,uVar8);
  local_8._0_1_ = 1;
  if (7 < (uint)puVar2[5]) {
    puVar2 = (undefined4 *)*puVar2;
  }
  uVar3 = FUN_01246e70(0x73,0x26,0x46,0x17,6,puVar2,extraout_ECX);
  *(undefined4 *)(param_1 + 0x80) = uVar3;
  local_8 = (uint)local_8._1_3_ << 8;
  FUN_00f1a330();
  local_30 = *(int *)(param_1 + 0x80);
  if (local_30 != 0) {
    puVar2 = operator_new(8);
    *puVar2 = &DAT_01ae2acc;
    puVar2[1] = 0;
    local_34 = puVar2;
    if (*(undefined4 **)(local_30 + 0x34) != (undefined4 *)0x0) {
      (**(code **)**(undefined4 **)(local_30 + 0x34))(1);
    }
    *(undefined4 **)(local_30 + 0x34) = puVar2;
    FUN_011d1890();
    FUN_01227420(0,0);
    local_30 = 0;
    FUN_011d17c0(&local_30);
  }
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


