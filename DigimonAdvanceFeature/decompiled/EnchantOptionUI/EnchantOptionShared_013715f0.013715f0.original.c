
/* ============================================================ */
/* Function: FUN_013715f0 */
/* Entry: 013715f0 */
/* Signature: undefined FUN_013715f0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_013715f0(int param_1)

{
  int iVar1;
  undefined1 local_2c [16];
  int local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0197c65d;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if (param_1 != 0) {
    iVar1 = __RTDynamicCast(*(undefined4 *)(param_1 + 0x34),0,&DAT_01bc4a94,&DAT_01bc742c,0,local_14
                           );
    if (iVar1 != 0) {
      FUN_0105cb70();
      local_8 = 0;
      if (local_1c != 0) {
        FUN_0136f850(local_2c);
      }
      FUN_00f1a330();
    }
  }
  ExceptionList = local_10;
  return;
}


