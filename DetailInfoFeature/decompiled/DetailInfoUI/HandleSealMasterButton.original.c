
/* ============================================================ */
/* Function: FUN_011dd770 */
/* Entry: 011dd770 */
/* Signature: undefined FUN_011dd770() */
/* ============================================================ */


int * __thiscall FUN_011dd770(int *param_1,uint param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  uint uVar4;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_019683e0;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_8 = 0;
  piVar1 = param_1 + 0x1d;
  *param_1 = (int)&DAT_01ae36a8;
  *piVar1 = (int)&DAT_01ae36e8;
  if (param_1[0x1e] != 0) {
    FUN_00ff8af0(piVar1);
  }
  (**(code **)(*param_1 + 0x28))(uVar4);
  puVar2 = (undefined4 *)param_1[0x29];
  *(undefined4 *)puVar2[1] = 0;
  puVar2 = (undefined4 *)*puVar2;
  while (puVar2 != (undefined4 *)0x0) {
    puVar3 = (undefined4 *)*puVar2;
    FUN_017986fc(puVar2,0xc);
    puVar2 = puVar3;
  }
  FUN_017986fc(param_1[0x29],0xc);
  *param_1 = (int)&DAT_01ae3e4c;
  *piVar1 = (int)&DAT_01ae3e40;
  if (param_1[0x1e] != 0) {
    FUN_00ff8af0(piVar1);
  }
  *piVar1 = (int)&DAT_01ae3acc;
  FUN_012456d0();
  if ((param_2 & 1) != 0) {
    if ((param_2 & 4) == 0) {
      FUN_01683120();
    }
    else {
      FUN_00f199b0(param_1,0xb0);
    }
  }
  ExceptionList = local_10;
  return param_1;
}


