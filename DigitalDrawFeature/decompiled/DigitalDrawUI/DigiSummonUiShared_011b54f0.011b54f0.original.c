
/* ============================================================ */
/* Function: FUN_011b54f0 */
/* Entry: 011b54f0 */
/* Signature: undefined FUN_011b54f0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011b54f0(int param_1,char *param_2,undefined4 param_3)

{
  undefined4 ***pppuVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 **local_2c [5];
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01990fb5;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_34 = param_3;
  if (*(int *)(param_1 + 0x90) != 0) {
    local_30 = CONCAT31(local_30._1_3_,*param_2);
    (**(code **)(*(int *)(*(int *)(param_1 + 0x90) + 0x14) + 0x1c))(local_30,local_14);
    if (*param_2 != '\0') {
      uVar2 = 0;
      uVar3 = 0;
      uVar4 = 0;
      uVar5 = 0;
      uVar6 = 0;
      uVar7 = 0;
      FUN_00f19cc0(s_DIGISUMMON_COUNT_REMAINING_01ae17b4,0x1a);
      FUN_015ce480(local_2c,uVar2,uVar3,uVar4,uVar5,uVar6,uVar7);
      local_8 = 0;
      local_3c = 0;
      local_4c = 0;
      uStack_48 = 0;
      uStack_44 = 0;
      uStack_40 = 0;
      local_38 = 0;
      FUN_00f1a200(u__value__01ae17ec,7);
      local_8._0_1_ = 1;
      FUN_01501660(local_2c,&local_4c,local_34);
      local_8 = (uint)local_8._1_3_ << 8;
      FUN_00f1a330();
      pppuVar1 = local_2c;
      if (7 < local_18) {
        pppuVar1 = (undefined4 ***)local_2c[0];
      }
      (**(code **)(**(int **)(param_1 + 0x90) + 0x10))(pppuVar1);
      FUN_00f1a330();
    }
  }
  ExceptionList = local_10;
  return;
}


