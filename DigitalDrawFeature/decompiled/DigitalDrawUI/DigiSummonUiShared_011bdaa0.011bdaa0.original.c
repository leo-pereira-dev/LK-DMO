
/* ============================================================ */
/* Function: FUN_011bdaa0 */
/* Entry: 011bdaa0 */
/* Signature: undefined FUN_011bdaa0() */
/* ============================================================ */


int * __thiscall FUN_011bdaa0(undefined4 *param_1,int *param_2,int *param_3,undefined4 *param_4)

{
  undefined8 uVar1;
  undefined4 uVar2;
  undefined1 uVar3;
  uint uVar4;
  undefined8 *puVar5;
  undefined4 *puVar6;
  int iVar7;
  undefined4 local_34;
  undefined4 uStack_30;
  undefined1 local_28 [4];
  undefined4 *local_24;
  undefined4 *local_20;
  undefined4 *local_1c;
  int *local_18;
  undefined4 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0197b785;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = param_4;
  local_18 = param_3;
  local_1c = param_1;
  puVar5 = (undefined8 *)FUN_00f1d720(local_28,param_3);
  iVar7 = *(int *)(puVar5 + 1);
  uVar1 = *puVar5;
  if ((*(char *)(iVar7 + 0xd) == '\0') && (*(int *)(iVar7 + 0x10) <= *param_3)) {
    uVar3 = 0;
  }
  else {
    if (param_1[1] == 0x4444444) {
                    /* WARNING: Subroutine does not return */
      FUN_00f1b5f0(uVar4);
    }
    uVar2 = *param_1;
    local_8 = 0;
    local_20 = (undefined4 *)0x0;
    local_24 = param_1;
    puVar6 = operator_new(0x3c);
    local_8 = 1;
    puVar6[4] = *local_18;
    puVar6[5] = *local_14;
    puVar6[6] = local_14[1];
    puVar6[7] = local_14[2];
    puVar6[8] = local_14[3];
    puVar6[9] = local_14[4];
    puVar6[10] = local_14[5];
    puVar6[0xb] = local_14[6];
    local_20 = puVar6;
    FUN_010c0bb0(local_14 + 7);
    uStack_30 = (undefined4)((ulonglong)uVar1 >> 0x20);
    local_34 = (undefined4)uVar1;
    puVar6[0xe] = local_14[9];
    *puVar6 = uVar2;
    puVar6[1] = uVar2;
    puVar6[2] = uVar2;
    *(undefined2 *)(puVar6 + 3) = 0;
    iVar7 = FUN_00f1b3e0(local_34,uStack_30,puVar6);
    uVar3 = 1;
  }
  *(undefined1 *)(param_2 + 1) = uVar3;
  *param_2 = iVar7;
  ExceptionList = local_10;
  return param_2;
}


