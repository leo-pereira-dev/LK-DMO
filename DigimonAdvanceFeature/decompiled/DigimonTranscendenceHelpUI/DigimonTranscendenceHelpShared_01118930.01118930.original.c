
/* ============================================================ */
/* Function: FUN_01118930 */
/* Entry: 01118930 */
/* Signature: undefined FUN_01118930() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01118930(int *param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 ***pppuVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 local_58 [28];
  undefined4 **local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  int local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01986685;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if (param_2 < 1) {
    puStack_20 = &stack0xfffffffc;
    (**(code **)(*(int *)param_1[1] + 0x40))(0);
    (**(code **)(*(int *)*param_1 + 0x40))(0);
  }
  else {
    (**(code **)(*(int *)param_1[1] + 0x40))(1,local_24);
    (**(code **)(*(int *)*param_1 + 0x40))(1);
    local_2c = 0;
    local_28 = 7;
    uStack_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    local_3c = (undefined4 ***)0x0;
    local_14 = 0;
    uVar3 = 0;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    FUN_00f19cc0(s_EvoOpenNew_Selected_Text_01ad919c,0x18);
    puVar1 = (undefined4 *)FUN_015ce480(local_58,uVar3,uVar4,uVar5,uVar6,uVar7,uVar8);
    local_14._0_1_ = 1;
    if (7 < (uint)puVar1[5]) {
      puVar1 = (undefined4 *)*puVar1;
    }
    FUN_01500f80(&local_3c,u_x_2d__s_01ad918c,param_2,puVar1);
    local_14 = (uint)local_14._1_3_ << 8;
    FUN_00f1a330();
    pppuVar2 = &local_3c;
    if (7 < local_28) {
      pppuVar2 = (undefined4 ***)local_3c;
    }
    (**(code **)(*(int *)(*param_1 + 0x14) + 0x10))(pppuVar2);
    FUN_00f1a330();
  }
  ExceptionList = local_1c;
  return;
}


