
/* ============================================================ */
/* Function: FUN_01267760 */
/* Entry: 01267760 */
/* Signature: undefined FUN_01267760() */
/* ============================================================ */


undefined4 __fastcall FUN_01267760(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_10;
  undefined1 local_c [8];
  int local_4;
  
  if (DAT_01bfe470 != 0) {
    piVar1 = (int *)(DAT_01bfe470 + 4);
    local_10 = 0x2c;
    iVar3 = param_1 + 0xa4;
    FUN_00f1d720(local_c,&local_10);
    if (((*(char *)(local_4 + 0xd) == '\0') && (*(int *)(local_4 + 0x10) < 0x2d)) &&
       (local_4 != *piVar1)) {
      uVar2 = *(undefined4 *)(local_4 + 0x14);
    }
    else {
      uVar2 = 0;
    }
    uVar2 = __RTDynamicCast(uVar2,0,&DAT_01bc3cf8,&DAT_01bc6948,0);
    *(undefined4 *)(param_1 + 0xa8) = uVar2;
    if (*(int *)(param_1 + 0xa8) != 0) {
      local_10 = 6;
      if (param_1 == 0) {
        iVar3 = 0;
      }
      FUN_00ff8bc0(&local_10,iVar3);
      return 1;
    }
  }
  return 0;
}


