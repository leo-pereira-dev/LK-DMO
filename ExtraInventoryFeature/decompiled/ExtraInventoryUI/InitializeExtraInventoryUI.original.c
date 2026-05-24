
/* ============================================================ */
/* Function: FUN_011a90d0 */
/* Entry: 011a90d0 */
/* Signature: undefined FUN_011a90d0() */
/* ============================================================ */


undefined4 __fastcall FUN_011a90d0(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_18;
  undefined1 local_14 [8];
  int local_c;
  
  if (DAT_01bfe470 != 0) {
    piVar1 = (int *)(DAT_01bfe470 + 4);
    local_18 = 0x3e;
    iVar3 = param_1 + 0xa4;
    FUN_00f1d720(local_14,&local_18);
    if (((*(char *)(local_c + 0xd) == '\0') && (*(int *)(local_c + 0x10) < 0x3f)) &&
       (local_c != *piVar1)) {
      uVar2 = *(undefined4 *)(local_c + 0x14);
    }
    else {
      uVar2 = 0;
    }
    uVar2 = __RTDynamicCast(uVar2,0,&DAT_01bc3cf8,&DAT_01bc4500,0);
    *(undefined4 *)(param_1 + 0xa8) = uVar2;
    if (*(int *)(param_1 + 0xa8) != 0) {
      local_18 = 1;
      if (param_1 == 0) {
        iVar3 = 0;
      }
      FUN_010d4490(&local_18,iVar3);
      local_18 = 2;
      FUN_010d4490(&local_18,iVar3);
      local_18 = 5;
      FUN_010d4490(&local_18,iVar3);
      return 1;
    }
  }
  return 0;
}


