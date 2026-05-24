
/* ============================================================ */
/* Function: FUN_011b41c0 */
/* Entry: 011b41c0 */
/* Signature: undefined FUN_011b41c0() */
/* ============================================================ */


bool __fastcall FUN_011b41c0(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_10;
  undefined1 local_c [8];
  int local_4;
  
  if (DAT_01bfe470 != 0) {
    piVar1 = (int *)(DAT_01bfe470 + 4);
    local_10 = 0x40;
    FUN_00f1d720(local_c,&local_10);
    if (((*(char *)(local_4 + 0xd) == '\0') && (*(int *)(local_4 + 0x10) < 0x41)) &&
       (local_4 != *piVar1)) {
      uVar2 = *(undefined4 *)(local_4 + 0x14);
    }
    else {
      uVar2 = 0;
    }
    iVar3 = __RTDynamicCast(uVar2,0,&DAT_01bc3cf8,&DAT_01bc5ee0,0);
    *(int *)(param_1 + 0x78) = iVar3;
    return iVar3 != 0;
  }
  return false;
}


