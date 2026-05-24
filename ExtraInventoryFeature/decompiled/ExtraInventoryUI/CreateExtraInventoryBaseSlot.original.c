
/* ============================================================ */
/* Function: FUN_011a3290 */
/* Entry: 011a3290 */
/* Signature: undefined FUN_011a3290() */
/* ============================================================ */


void __thiscall FUN_011a3290(int param_1,undefined4 param_2,short *param_3,undefined4 param_4)

{
  int iVar1;
  char cVar2;
  uint uVar3;
  undefined8 *puVar4;
  int *piVar5;
  undefined8 local_40;
  int local_38;
  undefined1 local_34 [4];
  int *local_30;
  undefined4 local_2c;
  int *local_28;
  undefined4 local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198fbcd;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  piVar5 = (int *)(param_1 + 0x80);
  local_2c = param_2;
  local_24 = param_4;
  local_28 = piVar5;
  FUN_00f41dd0(&local_40,param_3);
  if (((*(char *)(local_38 + 0xd) == '\0') && (*(short *)(local_38 + 0x10) <= *param_3)) &&
     (local_38 != *piVar5)) {
    FUN_011a3ea0(local_24);
  }
  else {
    cVar2 = FUN_011a36a0(local_24,param_3);
    if (cVar2 != '\0') {
      FUN_0122a090(0);
      puVar4 = (undefined8 *)FUN_00f41dd0(local_34,param_3);
      local_40 = *puVar4;
      if ((*(char *)(*(int *)(puVar4 + 1) + 0xd) != '\0') ||
         (*param_3 < *(short *)(*(int *)(puVar4 + 1) + 0x10))) {
        if (*(int *)(param_1 + 0x84) == 0x5d1745d) {
                    /* WARNING: Subroutine does not return */
          FUN_00f1b5f0(uVar3);
        }
        iVar1 = *piVar5;
        local_30 = local_28;
        local_14 = 0;
        local_2c = 0;
        piVar5 = operator_new(0x2c);
        *(short *)(piVar5 + 4) = *param_3;
        piVar5[5] = 0;
        piVar5[6] = 0;
        piVar5[7] = 0;
        piVar5[8] = 0;
        piVar5[9] = 0;
        piVar5[10] = 0;
        *piVar5 = iVar1;
        piVar5[1] = iVar1;
        piVar5[2] = iVar1;
        *(undefined2 *)(piVar5 + 3) = 0;
        FUN_00f1b3e0((undefined4)local_40,local_40._4_4_,piVar5);
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


