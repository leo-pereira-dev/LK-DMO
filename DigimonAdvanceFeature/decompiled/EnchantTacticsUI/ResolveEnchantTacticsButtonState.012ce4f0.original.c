
/* ============================================================ */
/* Function: FUN_012ce4f0 */
/* Entry: 012ce4f0 */
/* Signature: undefined FUN_012ce4f0() */
/* ============================================================ */


undefined4 __fastcall FUN_012ce4f0(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 local_10;
  undefined1 local_c [8];
  int local_4;
  
  iVar1 = *(int *)(*(int *)(DAT_01bfb2a8 + 0x14) + 0x344);
  switch(*(undefined4 *)(param_1 + 0x178)) {
  case 0:
    local_10 = 0;
    FUN_00f1d720(local_c,&local_10);
    if ((*(char *)(local_4 + 0xd) != '\0') || (0 < *(int *)(local_4 + 0x10))) {
      local_4 = *(int *)(iVar1 + 4);
    }
    puVar2 = (undefined4 *)0x0;
    if (local_4 != *(int *)(iVar1 + 4)) {
      puVar2 = (undefined4 *)(local_4 + 0x14);
    }
    if (puVar2 == (undefined4 *)0x0) {
      return 0;
    }
    break;
  case 1:
    local_10 = 1;
    FUN_00f1d720(local_c,&local_10);
    if ((*(char *)(local_4 + 0xd) != '\0') || (1 < *(int *)(local_4 + 0x10))) {
      local_4 = *(int *)(iVar1 + 4);
    }
    puVar2 = (undefined4 *)0x0;
    if (local_4 != *(int *)(iVar1 + 4)) {
      puVar2 = (undefined4 *)(local_4 + 0x14);
    }
    if (puVar2 == (undefined4 *)0x0) {
      return 0;
    }
    break;
  case 2:
    local_10 = 2;
    FUN_00f1d720(local_c,&local_10);
    if ((*(char *)(local_4 + 0xd) != '\0') || (2 < *(int *)(local_4 + 0x10))) {
      local_4 = *(int *)(iVar1 + 4);
    }
    puVar2 = (undefined4 *)0x0;
    if (local_4 != *(int *)(iVar1 + 4)) {
      puVar2 = (undefined4 *)(local_4 + 0x14);
    }
    if (puVar2 == (undefined4 *)0x0) {
      return 0;
    }
    break;
  case 3:
    local_10 = 4;
    FUN_00f1d720(local_c,&local_10);
    if ((*(char *)(local_4 + 0xd) != '\0') || (4 < *(int *)(local_4 + 0x10))) {
      local_4 = *(int *)(iVar1 + 4);
    }
    puVar2 = (undefined4 *)0x0;
    if (local_4 != *(int *)(iVar1 + 4)) {
      puVar2 = (undefined4 *)(local_4 + 0x14);
    }
    if (puVar2 == (undefined4 *)0x0) {
      return 0;
    }
    break;
  case 4:
    local_10 = 6;
    FUN_00f1d720(local_c,&local_10);
    if ((*(char *)(local_4 + 0xd) != '\0') || (6 < *(int *)(local_4 + 0x10))) {
      local_4 = *(int *)(iVar1 + 4);
    }
    puVar2 = (undefined4 *)0x0;
    if (local_4 != *(int *)(iVar1 + 4)) {
      puVar2 = (undefined4 *)(local_4 + 0x14);
    }
    if (puVar2 == (undefined4 *)0x0) {
      return 0;
    }
    break;
  default:
    return 0xffffffff;
  }
  return *puVar2;
}


