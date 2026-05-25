
/* ============================================================ */
/* Function: FUN_011d8ff0 */
/* Entry: 011d8ff0 */
/* Signature: undefined FUN_011d8ff0() */
/* ============================================================ */


undefined4 __thiscall
FUN_011d8ff0(int param_1,uint param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined8 *puVar3;
  undefined4 *puVar4;
  undefined1 local_48 [12];
  undefined8 local_3c;
  undefined1 local_30 [4];
  uint local_2c;
  undefined4 *local_28;
  int local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019800bd;
  local_1c = ExceptionList;
  ExceptionList = &local_1c;
  local_24 = param_1;
  puVar1 = (undefined4 *)FUN_016830c0(0x18,DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
  if (puVar1 != (undefined4 *)0x0) {
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
    puVar1[3] = 0;
    *(undefined8 *)(puVar1 + 4) = 0;
    puVar1[1] = param_3;
    *puVar1 = &DAT_01ae30c4;
    puVar1[2] = 0;
    puVar1[4] = param_4;
    puVar1[5] = param_5;
    puVar1[2] = param_1;
    local_28 = puVar1;
    FUN_00f1b390(local_30,&param_2);
    if (((*(char *)((int)local_28 + 0xd) == '\0') && ((uint)local_28[4] <= param_2)) &&
       (local_28 != *(undefined4 **)(param_1 + 4))) {
      if ((undefined4 *)local_28[5] != (undefined4 *)0x0) {
        (*(code *)**(undefined4 **)local_28[5])(1);
        local_28[5] = 0;
      }
      uVar2 = FUN_00f5d5c0(local_28);
      FUN_017986fc(uVar2,0x18);
    }
    local_2c = param_2;
    local_28 = puVar1;
    puVar3 = (undefined8 *)FUN_00f1b390(local_48,&local_2c);
    local_3c = *puVar3;
    if ((*(char *)(*(int *)(puVar3 + 1) + 0xd) != '\0') ||
       (param_2 < *(uint *)(*(int *)(puVar3 + 1) + 0x10))) {
      if (*(int *)(param_1 + 8) != 0xaaaaaaa) {
        uVar2 = *(undefined4 *)(param_1 + 4);
        local_2c = local_24 + 4;
        local_14 = 0;
        local_28 = (undefined4 *)0x0;
        puVar4 = operator_new(0x18);
        puVar4[4] = param_2;
        puVar4[5] = puVar1;
        *puVar4 = uVar2;
        puVar4[1] = uVar2;
        puVar4[2] = uVar2;
        *(undefined2 *)(puVar4 + 3) = 0;
        FUN_00f27390((undefined4)local_3c,local_3c._4_4_,puVar4);
        ExceptionList = local_1c;
        return 1;
      }
                    /* WARNING: Subroutine does not return */
      FUN_00f1b5f0();
    }
  }
  ExceptionList = local_1c;
  return 0;
}


