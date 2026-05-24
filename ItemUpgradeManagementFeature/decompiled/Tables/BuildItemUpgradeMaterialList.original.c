
/* ============================================================ */
/* Function: FUN_015ec940 */
/* Entry: 015ec940 */
/* Signature: undefined FUN_015ec940() */
/* ============================================================ */


undefined4 FUN_015ec940(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  undefined1 local_44 [24];
  undefined4 *local_2c;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c91fd;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = param_1;
  iVar4 = FUN_015ec600(local_44,param_2,param_3);
  local_8 = 0;
  FUN_015eb980(iVar4 + 0x18);
  *(undefined4 *)local_2c[1] = 0;
  puVar2 = (undefined4 *)*local_2c;
  while (puVar2 != (undefined4 *)0x0) {
    puVar1 = (undefined4 *)*puVar2;
    FUN_017986fc(puVar2,0x18,uVar3);
    puVar2 = puVar1;
  }
  FUN_017986fc(local_2c,0x18,uVar3);
  ExceptionList = local_10;
  return param_1;
}


