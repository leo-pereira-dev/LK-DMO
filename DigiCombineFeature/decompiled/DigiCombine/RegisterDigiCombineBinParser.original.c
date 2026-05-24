
/* ============================================================ */
/* Function: FUN_0162e3a0 */
/* Entry: 0162e3a0 */
/* Signature: undefined FUN_0162e3a0() */
/* ============================================================ */


void __thiscall
FUN_0162e3a0(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            )

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c8394;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  puVar2 = operator_new(0x40);
  local_8 = 0;
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    memset(puVar2,0,0x40);
    *puVar2 = &DAT_01a1a768;
    puVar2[1] = param_3;
    puVar2[3] = param_5;
    local_8 = CONCAT31(local_8._1_3_,1);
    puVar2[2] = param_4;
    puVar2[4] = 0;
    puVar2[5] = 0;
    puVar2[6] = 0;
    puVar2[7] = 0;
    FUN_00f221a0(param_2);
    puVar2[0xe] = 0;
  }
  local_8 = 0xffffffff;
  if (puVar2 != (undefined4 *)0x0) {
    iVar1 = *(int *)(param_1 + 4);
    if (*(int *)(param_1 + 8) == 0x15555555) {
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8 = 2;
    piVar3 = operator_new(0xc);
    piVar3[2] = (int)puVar2;
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
    puVar2 = *(undefined4 **)(iVar1 + 4);
    *piVar3 = iVar1;
    piVar3[1] = (int)puVar2;
    *(int **)(iVar1 + 4) = piVar3;
    *puVar2 = piVar3;
  }
  ExceptionList = local_10;
  return;
}


