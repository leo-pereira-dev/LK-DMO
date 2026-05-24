
/* ============================================================ */
/* Function: FUN_01118d40 */
/* Entry: 01118d40 */
/* Signature: undefined FUN_01118d40() */
/* ============================================================ */


bool __fastcall FUN_01118d40(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_10;
  undefined1 local_c [8];
  int local_4;
  
  if (DAT_01bfe470 != 0) {
    piVar1 = (int *)(DAT_01bfe470 + 4);
    local_10 = 0x24;
    FUN_00f1d720(local_c,&local_10);
    if (((*(char *)(local_4 + 0xd) == '\0') && (*(int *)(local_4 + 0x10) < 0x25)) &&
       (local_4 != *piVar1)) {
      uVar2 = *(undefined4 *)(local_4 + 0x14);
    }
    else {
      uVar2 = 0;
    }
    iVar3 = __RTDynamicCast(uVar2,0,&DAT_01bc3cf8,&DAT_01bc4dd8,0);
    *(int *)(param_1 + 0xa8) = iVar3;
    return iVar3 != 0;
  }
  return false;
}


