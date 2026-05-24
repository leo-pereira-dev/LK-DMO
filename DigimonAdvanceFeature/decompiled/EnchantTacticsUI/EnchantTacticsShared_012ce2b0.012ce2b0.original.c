
/* ============================================================ */
/* Function: FUN_012ce2b0 */
/* Entry: 012ce2b0 */
/* Signature: undefined FUN_012ce2b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 __fastcall FUN_012ce2b0(int param_1)

{
  char cVar1;
  int iVar2;
  undefined1 auStack_18 [3];
  char local_15;
  int local_14;
  uint local_10;
  uint local_c;
  
  local_c = DAT_01bbb1c0 ^ (uint)auStack_18;
  if ((*(int *)(param_1 + 0x178) != -1) &&
     (**(int **)(*(int *)(DAT_01bfb2a8 + 0x14) + 0x344) != 0x4b)) {
    iVar2 = FUN_012ce4f0();
    if (iVar2 + 1 < 0x10) {
      if (DAT_01bfb2b4 == -0x70fa) {
        return 0;
      }
      local_10 = (uint)*(byte *)(DAT_01bfb2b4 + 0x7185);
      if (*(int *)(param_1 + 0x19c) == 0) {
        return 1;
      }
      local_14 = iVar2 + 1;
      local_15 = FUN_012cf6a0(param_1 + 0x178);
      local_15 = local_15 + '\x01';
      cVar1 = FUN_0161f040(&local_15,&local_14,&local_10);
      if (cVar1 != '\0') {
        return 1;
      }
    }
  }
  FUN_013be010(0,0,0,0);
  return 0;
}


