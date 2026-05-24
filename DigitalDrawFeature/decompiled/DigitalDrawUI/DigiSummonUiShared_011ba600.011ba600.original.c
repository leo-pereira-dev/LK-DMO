
/* ============================================================ */
/* Function: FUN_011ba600 */
/* Entry: 011ba600 */
/* Signature: undefined FUN_011ba600() */
/* ============================================================ */


undefined4 __thiscall FUN_011ba600(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  char cVar3;
  
  if ((*(int *)(param_1 + 0x18) == 0) || (*(char *)(*(int *)(param_1 + 0x18) + 0x1c) == '\0')) {
    return 0;
  }
  if ((((*(int *)(param_1 + 0xc) == 0) ||
       ((iVar1 = *(int *)(*(int *)(param_1 + 0xc) + 0x24), iVar1 == 0 ||
        (*(char *)(iVar1 + 0x78) == '\0')))) &&
      ((iVar1 = *(int *)(param_1 + 8), iVar1 == 0 ||
       ((*(int *)(iVar1 + 0x24) == 0 || (*(char *)(*(int *)(iVar1 + 0x24) + 0x78) == '\0')))))) &&
     ((*(int *)(param_1 + 4) == 0 ||
      (((iVar2 = *(int *)(*(int *)(param_1 + 4) + 0x1c), iVar2 == 0 || (*(int *)(iVar2 + 0x80) != 2)
        ) || (*(int *)(iVar2 + 0x84) < 1)))))) {
    if (iVar1 == 0) {
      return 0;
    }
    if ((*(int *)(iVar1 + 0x24) != 0) && (*(char *)(*(int *)(iVar1 + 0x24) + 0x88) != '\0')) {
      return 0;
    }
    iVar1 = *param_2;
    *(int *)(iVar1 + 0x6c) = *(int *)(iVar1 + 0x6c) + 1;
    if (*(int *)(iVar1 + 0x70) <= *(int *)(iVar1 + 0x6c)) {
      param_2 = (int *)0xa;
      FUN_00ffcb30(&param_2,&DAT_01bfe74c);
    }
    iVar1 = *(int *)(*(int *)(param_1 + 8) + 0x24);
    if (iVar1 != 0) {
      *(undefined1 *)(iVar1 + 0x78) = 1;
      *(undefined1 *)(iVar1 + 0x2d) = 1;
      *(undefined1 *)(iVar1 + 0x88) = 1;
      *(undefined4 *)(iVar1 + 0x6c) = 4;
      *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
    }
    FUN_011ba9b0();
    if (DAT_01bfb32c != (int *)0x0) {
      cVar3 = (**(code **)(*DAT_01bfb32c + 4))(s_System_DigiCombine_Gacha_Progres_01ae1b00);
      if (cVar3 != '\0') {
        (**(code **)(*DAT_01bfb32c + 8))(s_System_DigiCombine_Gacha_Progres_01ae1b00);
      }
      (**(code **)*DAT_01bfb32c)(s_System_DigiCombine_Gacha_Progres_01ae1b00);
    }
  }
  return 1;
}


