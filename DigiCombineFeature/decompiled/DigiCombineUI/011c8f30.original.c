
/* ============================================================ */
/* Function: FUN_011c8f30 */
/* Entry: 011c8f30 */
/* Signature: undefined FUN_011c8f30() */
/* ============================================================ */


void __fastcall FUN_011c8f30(int *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piStack_4;
  
  if ((param_1[2] == 0) || (*(char *)(param_1[2] + 0x1c) == '\0')) {
    return;
  }
  piVar1 = (int *)param_1[5];
  piStack_4 = param_1;
  if (((piVar1 == (int *)0x0) || (piVar1[9] == 0)) || (*(char *)(piVar1[9] + 0x78) == '\0')) {
    if (((param_1[4] == 0) || (iVar2 = *(int *)(param_1[4] + 0x24), iVar2 == 0)) ||
       (*(char *)(iVar2 + 0x78) == '\0')) {
      if (param_1[3] == 0) {
        return;
      }
      iVar2 = *(int *)(param_1[3] + 0x1c);
      if (iVar2 == 0) {
        return;
      }
      if (*(int *)(iVar2 + 0x80) != 2) {
        return;
      }
      if (*(int *)(iVar2 + 0x84) < 1) {
        return;
      }
    }
    if (piVar1 == (int *)0x0) goto LAB_011c8fde;
  }
  (**(code **)(*piVar1 + 0x40))(0);
  iVar2 = param_1[5];
  iVar3 = *(int *)(iVar2 + 0x24);
  if (iVar3 != 0) {
    *(undefined1 *)(iVar3 + 0x78) = 0;
    iVar2 = *(int *)(iVar2 + 0x24);
    *(undefined4 *)(iVar2 + 0x6c) = 0;
    *(undefined4 *)(iVar2 + 0x68) = *(undefined4 *)(iVar2 + 100);
  }
LAB_011c8fde:
  piVar1 = (int *)param_1[4];
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x40))(0);
    iVar2 = param_1[4];
    iVar3 = *(int *)(iVar2 + 0x24);
    if (iVar3 != 0) {
      *(undefined1 *)(iVar3 + 0x78) = 0;
      iVar2 = *(int *)(iVar2 + 0x24);
      *(undefined4 *)(iVar2 + 0x6c) = 0;
      *(undefined4 *)(iVar2 + 0x68) = *(undefined4 *)(iVar2 + 100);
    }
  }
  if ((int *)*param_1 != (int *)0x0) {
    (**(code **)(*(int *)*param_1 + 0x58))(s_DigiCombine_DigiCombine_CardFron_01ae106c,1);
  }
  if ((int *)param_1[3] != (int *)0x0) {
    (**(code **)(*(int *)param_1[3] + 0x40))(1);
    iVar2 = *(int *)(param_1[3] + 0x1c);
    if (iVar2 != 0) {
      piStack_4 = (int *)((*(int *)(iVar2 + 0x54) - *(int *)(iVar2 + 0x50)) / 0x24);
      FUN_0124bd00(&piStack_4);
    }
  }
  FUN_011c9100();
  return;
}


