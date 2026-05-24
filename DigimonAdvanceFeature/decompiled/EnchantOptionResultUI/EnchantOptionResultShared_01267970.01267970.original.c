
/* ============================================================ */
/* Function: FUN_01267970 */
/* Entry: 01267970 */
/* Signature: undefined FUN_01267970() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int __fastcall FUN_01267970(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 auStack_38 [4];
  int local_34;
  int local_30;
  int iStack_2c;
  int iStack_28;
  int iStack_24;
  uint local_14;
  
  local_14 = DAT_01bbb1c0 ^ (uint)auStack_38;
  iVar2 = FUN_01287d50();
  if ((iVar2 == 0) || (*(char *)(param_1 + 0x68) == '\0')) {
    return 0;
  }
  if ((*(int **)(param_1 + 0xf8) != (int *)0x0) &&
     (iVar3 = (**(code **)(**(int **)(param_1 + 0xf8) + 0x44))(), iVar3 == 4)) {
    return iVar2;
  }
  iStack_2c = *(int *)(param_1 + 0x34);
  local_30 = *(int *)(param_1 + 0x30);
  iStack_28 = local_30 + 500;
  iStack_24 = iStack_2c + 0x2e;
  local_34 = DAT_01bfe474;
  iVar4 = FUN_00f83b00(DAT_01bfe474 + 8);
  iVar3 = DAT_01bfe474;
  if ((((iVar4 == 1) && (*(int *)(local_34 + 4) != 10)) && (*(char *)(param_1 + 0x1d) != '\0')) &&
     (*(int *)(DAT_01bfe474 + 4) == 1)) {
    *(undefined1 *)(param_1 + 0x7d) = 1;
    uVar1 = *(undefined4 *)(iVar3 + 0xc);
    *(undefined4 *)(param_1 + 0x80) = *(undefined4 *)(iVar3 + 8);
    *(undefined4 *)(param_1 + 0x84) = uVar1;
  }
  return iVar2;
}


