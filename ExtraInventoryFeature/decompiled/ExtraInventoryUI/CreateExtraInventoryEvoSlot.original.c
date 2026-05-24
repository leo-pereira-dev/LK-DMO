
/* ============================================================ */
/* Function: FUN_011a5b40 */
/* Entry: 011a5b40 */
/* Signature: undefined FUN_011a5b40() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011a5b40(int param_1,undefined4 param_2,short *param_3,undefined4 param_4)

{
  int iVar1;
  char cVar2;
  uint uVar3;
  undefined8 *puVar4;
  int *piVar5;
  undefined8 local_64;
  int local_5c;
  undefined1 local_58 [4];
  int *local_54;
  undefined4 local_50;
  short *local_4c;
  undefined4 local_48;
  int local_44;
  int iStack_40;
  int iStack_3c;
  int iStack_38;
  int local_34;
  int iStack_30;
  int iStack_2c;
  int iStack_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198fead;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  piVar5 = (int *)(param_1 + 0x80);
  local_50 = param_2;
  local_48 = param_4;
  local_4c = param_3;
  local_24 = uVar3;
  FUN_00f41dd0(&local_64,param_3);
  if (((*(char *)(local_5c + 0xd) == '\0') && (*(short *)(local_5c + 0x10) <= *param_3)) &&
     (local_5c != *piVar5)) {
    FUN_011a7b60(local_48);
  }
  else {
    local_44 = 0;
    iStack_40 = 0;
    iStack_3c = 0;
    iStack_38 = 0;
    local_34 = 0;
    iStack_30 = 0;
    iStack_2c = 0;
    iStack_28 = 0;
    cVar2 = FUN_011a72c0(local_48,param_3);
    if (cVar2 != '\0') {
      FUN_0122a090(iStack_28);
      puVar4 = (undefined8 *)FUN_00f41dd0(local_58,param_3);
      local_64 = *puVar4;
      if ((*(char *)(*(int *)(puVar4 + 1) + 0xd) != '\0') ||
         (*param_3 < *(short *)(*(int *)(puVar4 + 1) + 0x10))) {
        if (*(int *)(param_1 + 0x84) == 0x4ec4ec4) {
                    /* WARNING: Subroutine does not return */
          FUN_00f1b5f0(uVar3);
        }
        iVar1 = *piVar5;
        local_14 = 0;
        local_50 = 0;
        local_54 = piVar5;
        piVar5 = operator_new(0x34);
        *(short *)(piVar5 + 4) = *local_4c;
        piVar5[5] = local_44;
        piVar5[6] = iStack_40;
        piVar5[7] = iStack_3c;
        piVar5[8] = iStack_38;
        piVar5[9] = local_34;
        piVar5[10] = iStack_30;
        piVar5[0xb] = iStack_2c;
        piVar5[0xc] = iStack_28;
        *piVar5 = iVar1;
        piVar5[1] = iVar1;
        piVar5[2] = iVar1;
        *(undefined2 *)(piVar5 + 3) = 0;
        FUN_00f1b3e0((undefined4)local_64,local_64._4_4_,piVar5);
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


