
/* ============================================================ */
/* Function: FUN_011b7bb0 */
/* Entry: 011b7bb0 */
/* Signature: undefined FUN_011b7bb0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011b7bb0(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  void *pvVar5;
  int *piVar6;
  int local_60;
  undefined8 local_5c;
  undefined1 *local_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined1 local_38;
  undefined4 local_34;
  int local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01991265;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if ((*(int *)(param_1 + 0x7c) != 0) &&
     (iVar4 = __RTDynamicCast(*(undefined4 *)(*(int *)(param_1 + 0x7c) + 0x34),0,&DAT_01bc4a94,
                              &DAT_01bc5f68,0,local_14), iVar4 != 0)) {
    *(undefined4 *)(iVar4 + 4) = *param_2;
    FUN_01232880(1);
    if (param_3[4] != 0) {
      local_40 = 0;
      local_3c = 7;
      uStack_4c = 0;
      uStack_48 = 0;
      uStack_44 = 0;
      local_50 = 0;
      local_24 = 0;
      local_8 = 0;
      local_30 = DAT_01bfe468 + 8;
      local_1c = CONCAT44(&DAT_3f7ae148,(int *)local_1c);
      local_2c = 0xd;
      local_5c = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_54 = &DAT_3f7ae148;
      local_38 = 1;
      local_28 = 0;
      local_34 = 5;
      if (7 < (uint)param_3[5]) {
        param_3 = (undefined4 *)*param_3;
      }
      FUN_012543f0(param_1 + 0x88,400,param_3,&local_5c);
      local_8 = 0xffffffff;
      local_30 = 0;
      FUN_00f1a330();
    }
  }
  if (*(int *)(param_1 + 0x7c) != 0) {
    FUN_0122c320();
    local_1c = 0;
    pvVar5 = operator_new(0x28);
    *(void **)pvVar5 = pvVar5;
    *(void **)((int)pvVar5 + 4) = pvVar5;
    local_1c = CONCAT44(local_1c._4_4_,pvVar5);
    local_8 = 1;
    FUN_0102f490(param_2,&local_1c);
    piVar3 = (int *)local_1c;
    piVar6 = (int *)*(int *)local_1c;
    if (piVar6 != (int *)local_1c) {
      do {
        local_60 = piVar6[2];
        FUN_011b7530(&local_60,piVar6 + 3,(int)piVar6 + 0x26,piVar6 + 9);
        piVar6 = (int *)*piVar6;
      } while (piVar6 != piVar3);
    }
    *(undefined4 *)((int *)local_1c)[1] = 0;
    puVar2 = (undefined4 *)*(int *)local_1c;
    while (puVar2 != (undefined4 *)0x0) {
      puVar1 = (undefined4 *)*puVar2;
      FUN_00f1a330();
      FUN_017986fc(puVar2,0x28);
      puVar2 = puVar1;
    }
    FUN_017986fc((int *)local_1c,0x28);
  }
  ExceptionList = local_10;
  return;
}


