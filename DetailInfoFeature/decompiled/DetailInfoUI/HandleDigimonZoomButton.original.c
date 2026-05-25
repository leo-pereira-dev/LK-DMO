
/* ============================================================ */
/* Function: FUN_011d5d40 */
/* Entry: 011d5d40 */
/* Signature: undefined FUN_011d5d40() */
/* ============================================================ */


void __fastcall FUN_011d5d40(int param_1)

{
  undefined4 local_10;
  undefined1 local_c [8];
  int local_4;
  
  local_10 = 0;
  FUN_00f1d720(local_c,&local_10);
  if (((*(char *)(local_4 + 0xd) == '\0') && (*(int *)(local_4 + 0x10) < 1)) &&
     (local_4 != *(int *)(param_1 + 0x8c))) {
    *(bool *)(*(int *)(local_4 + 0x14) + 0x68) = *(char *)(*(int *)(local_4 + 0x14) + 0x68) == '\0';
  }
  return;
}


