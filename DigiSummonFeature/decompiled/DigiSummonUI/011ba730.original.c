
/* ============================================================ */
/* Function: FUN_011ba730 */
/* Entry: 011ba730 */
/* Signature: undefined FUN_011ba730() */
/* ============================================================ */


void __thiscall FUN_011ba730(int *param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  char cVar3;
  int iVar4;
  
  if ((int *)*param_1 != (int *)0x0) {
    (**(code **)(*(int *)*param_1 + 0x58))(s_DigiCombine_DigiCombine_CardBack_01ae1a84,1);
  }
  if (param_1[1] != 0) {
    if (*(int *)(param_1[1] + 0x1c) != 0) {
      FUN_0124b170();
    }
    (**(code **)(*(int *)param_1[1] + 0x40))(0);
  }
  puVar1 = *(undefined4 **)(param_2 + 0x10);
  for (puVar2 = (undefined4 *)*puVar1; puVar2 != puVar1; puVar2 = (undefined4 *)*puVar2) {
    if ((param_1[1] != 0) && (*(int *)(param_1[1] + 0x1c) != 0)) {
      FUN_0124af60(puVar2 + 3,puVar2[9],puVar2[10],puVar2[0xb],puVar2[0xc]);
    }
  }
  if (((param_1[1] != 0) && (iVar4 = *(int *)(param_1[1] + 0x1c), iVar4 != 0)) &&
     (iVar4 = __RTDynamicCast(*(undefined4 *)(iVar4 + 0x34),0,&DAT_01bc4a94,&DAT_01bc6024,0),
     iVar4 != 0)) {
    *(undefined4 *)(iVar4 + 8) = *(undefined4 *)(param_2 + 4);
    *(undefined4 *)(iVar4 + 0xc) = *(undefined4 *)(param_2 + 8);
    *(undefined1 *)(iVar4 + 0x10) = *(undefined1 *)(param_2 + 0x18);
  }
  if (param_1[3] != 0) {
    iVar4 = *(int *)(param_1[3] + 0x24);
    if (iVar4 != 0) {
      *(undefined1 *)(iVar4 + 0x78) = 1;
      *(undefined1 *)(iVar4 + 0x2d) = 1;
      *(undefined1 *)(iVar4 + 0x88) = 1;
      *(undefined4 *)(iVar4 + 0x6c) = 0;
      *(undefined4 *)(iVar4 + 0x68) = *(undefined4 *)(iVar4 + 100);
    }
    if (DAT_01bfb32c != (int *)0x0) {
      cVar3 = (**(code **)(*DAT_01bfb32c + 4))(s_System_DigiCombine_Gacha_Change__01ae1b74);
      if (cVar3 != '\0') {
        (**(code **)(*DAT_01bfb32c + 8))(s_System_DigiCombine_Gacha_Change__01ae1b74);
      }
      (**(code **)*DAT_01bfb32c)(s_System_DigiCombine_Gacha_Change__01ae1b74);
    }
  }
  if ((int *)param_1[4] != (int *)0x0) {
    (**(code **)(*(int *)param_1[4] + 0x40))(0);
  }
  FUN_011bab00(param_2 + 0xc);
  FUN_011bac80(param_2 + 0xc);
  return;
}


