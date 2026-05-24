
/* ============================================================ */
/* Function: FUN_011bcad0 */
/* Entry: 011bcad0 */
/* Signature: undefined FUN_011bcad0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011bcad0(int param_1,int *param_2)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  undefined4 ***pppuVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 local_50;
  int local_4c;
  undefined4 local_48;
  undefined4 **local_44 [5];
  uint local_30;
  int local_2c;
  int local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  int local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01991685;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  iVar1 = *(int *)(param_1 + 0x78);
  local_24 = uVar3;
  FUN_00f1d720(&uStack_54,param_2);
  if (((*(char *)(local_4c + 0xd) == '\0') && (*(int *)(local_4c + 0x10) <= *param_2)) &&
     (local_4c != *(int *)(iVar1 + 0x44))) {
    if (*(int *)(local_4c + 0x14) == 0) {
      local_28 = 0;
      local_2c = CONCAT31(local_2c._1_3_,1);
    }
    else {
      local_28 = (int)*(short *)(*(int *)(local_4c + 0x14) + 0x2a);
      local_2c = CONCAT31(local_2c._1_3_,1);
    }
  }
  else {
    local_2c = (uint)local_2c._1_3_ << 8;
    local_28 = 0;
  }
  local_48 = CONCAT31(local_48._1_3_,(char)local_2c);
  if (*(int **)(param_1 + 0x90) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x90) + 0x1c))(local_48,uVar3);
  }
  if (*(int **)(param_1 + 0x94) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x94) + 0x1c))(local_2c);
  }
  if (*(int *)(param_1 + 0x98) != 0) {
    (**(code **)(*(int *)(*(int *)(param_1 + 0x98) + 0x14) + 0x1c))(local_2c);
  }
  if (*(int *)(param_1 + 0x90) != 0) {
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_DIGISUMMON_BUY_01ae1fac,0xe);
    FUN_015ce480(local_44,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10);
    local_14 = 0;
    local_50 = 0;
    local_60 = 0;
    uStack_5c = 0;
    uStack_58 = 0;
    uStack_54 = 0;
    local_4c = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_14._0_1_ = 1;
    FUN_01501660(local_44,&local_60,&local_28);
    local_14 = (uint)local_14._1_3_ << 8;
    FUN_00f1a330();
    pppuVar4 = local_44;
    if (7 < local_30) {
      pppuVar4 = (undefined4 ***)local_44[0];
    }
    piVar2 = *(int **)(*(int *)(param_1 + 0x90) + 200);
    if (piVar2 != (int *)0x0) {
      (**(code **)(*piVar2 + 0x10))(pppuVar4);
    }
    FUN_00f1a330();
  }
  ExceptionList = local_1c;
  return;
}


