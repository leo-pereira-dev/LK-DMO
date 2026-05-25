
/* ============================================================ */
/* Function: FUN_011d86e0 */
/* Entry: 011d86e0 */
/* Signature: undefined FUN_011d86e0() */
/* ============================================================ */


void __fastcall FUN_011d86e0(int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int *piVar3;
  undefined4 *puVar4;
  undefined4 *local_20;
  undefined4 local_1c;
  int *local_18;
  int *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01993629;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  FUN_00f6e780(DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_14 = (int *)FUN_016830c0(0x9c);
  local_8 = 0;
  if (local_14 == (int *)0x0) {
    piVar3 = (int *)0x0;
  }
  else {
    memset(local_14,0,0x9c);
    piVar3 = (int *)FUN_011dab40();
  }
  local_8 = 0xffffffff;
  local_14 = piVar3;
  if (piVar3 != (int *)0x0) {
    (**(code **)(*piVar3 + 0x2c))(*(undefined4 *)(param_1 + 0x40),0,0x3f,0x1cf,0x25d);
    piVar3[0x12] = param_1;
    local_18 = *(int **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_011d8863;
    local_8 = 1;
    local_1c = 0;
    local_20 = (undefined4 *)(param_1 + 0x58);
    puVar4 = operator_new(0xc);
    piVar2 = local_18;
    local_8 = 0xffffffff;
    local_18 = (int *)0x0;
    puVar4[2] = piVar3;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar1 = (undefined4 *)piVar2[1];
    *puVar4 = piVar2;
    puVar4[1] = puVar1;
    piVar2[1] = (int)puVar4;
    *puVar1 = puVar4;
    FUN_00f31350(&local_20,&local_18,&local_14);
  }
  local_14 = (int *)FUN_016830c0(0x98);
  local_8 = 2;
  if (local_14 == (int *)0x0) {
    piVar3 = (int *)0x0;
  }
  else {
    memset(local_14,0,0x98);
    piVar3 = (int *)FUN_011d2710();
  }
  local_8 = 0xffffffff;
  if (piVar3 != (int *)0x0) {
    local_18 = piVar3;
    (**(code **)(*piVar3 + 0x2c))(*(undefined4 *)(param_1 + 0x40),0,0x3f,0x1cf,0x25d);
    piVar3[0x12] = param_1;
    local_14 = *(int **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) {
LAB_011d8863:
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8 = 3;
    local_1c = 0;
    local_20 = (undefined4 *)(param_1 + 0x58);
    puVar4 = operator_new(0xc);
    piVar2 = local_14;
    local_8 = 0xffffffff;
    local_14 = (int *)0x1;
    puVar4[2] = piVar3;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar1 = (undefined4 *)piVar2[1];
    *puVar4 = piVar2;
    puVar4[1] = puVar1;
    piVar2[1] = (int)puVar4;
    *puVar1 = puVar4;
    FUN_00f31350(&local_20,&local_14,&local_18);
  }
  ExceptionList = local_10;
  return;
}


