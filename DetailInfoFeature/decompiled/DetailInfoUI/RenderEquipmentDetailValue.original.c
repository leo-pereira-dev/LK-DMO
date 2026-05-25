
/* ============================================================ */
/* Function: FUN_011d7cf0 */
/* Entry: 011d7cf0 */
/* Signature: undefined FUN_011d7cf0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_011d7cf0(undefined4 param_1,undefined4 param_2,short *param_3)

{
  short sVar1;
  uint uVar2;
  undefined4 uVar3;
  short *psVar4;
  undefined8 local_50;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_0198f26d;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_34 = 0;
  uStack_40 = 0;
  uStack_3c = 0;
  uStack_38 = 0;
  local_30 = 7;
  local_44 = 0;
  local_18 = 0;
  local_8 = 0;
  local_24 = DAT_01bfe468 + 8;
  local_20 = 0xb;
  local_50 = DAT_01bb9b2c;
  local_48 = DAT_01bb9b34;
  local_1c = 0;
  local_2c = 1;
  local_28 = 5;
  psVar4 = param_3;
  do {
    sVar1 = *psVar4;
    psVar4 = psVar4 + 1;
  } while (sVar1 != 0);
  local_14 = uVar2;
  FUN_00f1a040(param_3,(int)psVar4 - (int)(param_3 + 1) >> 1);
  uVar3 = FUN_01246bb0(&local_50,1,param_1,param_2);
  local_24 = 0;
  FUN_00f1a330(uVar2);
  ExceptionList = local_10;
  return uVar3;
}


