
/* ============================================================ */
/* Function: FUN_011baa40 */
/* Entry: 011baa40 */
/* Signature: undefined FUN_011baa40() */
/* ============================================================ */


void __fastcall FUN_011baa40(int param_1)

{
  int iVar1;
  char cVar2;
  char *pcVar3;
  
  if (((*(int *)(param_1 + 0xc) == 0) ||
      (iVar1 = *(int *)(*(int *)(param_1 + 0xc) + 0x24), iVar1 == 0)) ||
     (*(char *)(iVar1 + 0x88) == '\0')) {
    if (DAT_01bfb32c == (int *)0x0) goto LAB_011baac5;
    cVar2 = (**(code **)(*DAT_01bfb32c + 4))(s_System_DigiCombine_Gacha_Default_01ae1bc0);
    if (cVar2 != '\0') {
      (**(code **)(*DAT_01bfb32c + 8))(s_System_DigiCombine_Gacha_Default_01ae1bc0);
    }
    pcVar3 = s_System_DigiCombine_Gacha_Default_01ae1bc0;
  }
  else {
    if (DAT_01bfb32c == (int *)0x0) goto LAB_011baac5;
    cVar2 = (**(code **)(*DAT_01bfb32c + 4))(s_System_DigiCombine_Gacha_Rare_wa_01ae1b50);
    if (cVar2 != '\0') {
      (**(code **)(*DAT_01bfb32c + 8))(s_System_DigiCombine_Gacha_Rare_wa_01ae1b50);
    }
    pcVar3 = s_System_DigiCombine_Gacha_Rare_wa_01ae1b50;
  }
  (**(code **)*DAT_01bfb32c)(pcVar3);
LAB_011baac5:
  if ((*(int *)(param_1 + 0x14) != 0) &&
     (iVar1 = *(int *)(*(int *)(param_1 + 0x14) + 0x24), iVar1 != 0)) {
    *(undefined1 *)(iVar1 + 0x78) = 1;
    *(undefined1 *)(iVar1 + 0x2d) = 1;
    *(undefined1 *)(iVar1 + 0x88) = 1;
    *(undefined4 *)(iVar1 + 0x6c) = 0;
    *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
  }
  if (*(int **)(param_1 + 0x10) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x10) + 0x40))(1);
  }
  return;
}


