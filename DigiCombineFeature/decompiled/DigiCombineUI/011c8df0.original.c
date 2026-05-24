
/* ============================================================ */
/* Function: FUN_011c8df0 */
/* Entry: 011c8df0 */
/* Signature: undefined FUN_011c8df0() */
/* ============================================================ */


void __thiscall FUN_011c8df0(int *param_1,int param_2)

{
  char *pcVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  char cVar4;
  int iVar5;
  
  if ((int *)*param_1 != (int *)0x0) {
    (**(code **)(*(int *)*param_1 + 0x58))(s_DigiCombine_DigiCombine_CardBack_01ae1a84,1);
  }
  if (param_1[3] != 0) {
    if (*(int *)(param_1[3] + 0x1c) != 0) {
      FUN_0124b170();
    }
    (**(code **)(*(int *)param_1[3] + 0x40))(0);
  }
  puVar2 = *(undefined4 **)(param_2 + 0x10);
  for (puVar3 = (undefined4 *)*puVar2; puVar3 != puVar2; puVar3 = (undefined4 *)*puVar3) {
    if ((param_1[3] != 0) && (*(int *)(param_1[3] + 0x1c) != 0)) {
      FUN_0124af60(puVar3 + 3,puVar3[9],puVar3[10],puVar3[0xb],puVar3[0xc]);
    }
  }
  pcVar1 = (char *)(param_2 + 0xc);
  cVar4 = *pcVar1;
  if (((param_1[3] != 0) && (iVar5 = *(int *)(param_1[3] + 0x1c), iVar5 != 0)) &&
     (iVar5 = __RTDynamicCast(*(undefined4 *)(iVar5 + 0x34),0,&DAT_01bc4a94,&DAT_01bc617c,0),
     iVar5 != 0)) {
    *(undefined4 *)(iVar5 + 8) = *(undefined4 *)(param_2 + 4);
    *(int *)(iVar5 + 0xc) = (int)cVar4;
    *(undefined1 *)(iVar5 + 0x10) = *(undefined1 *)(param_2 + 0x18);
  }
  if (param_1[5] != 0) {
    iVar5 = *(int *)(param_1[5] + 0x24);
    if (iVar5 != 0) {
      *(undefined1 *)(iVar5 + 0x78) = 1;
      *(undefined1 *)(iVar5 + 0x2d) = 1;
      *(undefined1 *)(iVar5 + 0x88) = 1;
      *(undefined4 *)(iVar5 + 0x6c) = 0;
      *(undefined4 *)(iVar5 + 0x68) = *(undefined4 *)(iVar5 + 100);
    }
    if (DAT_01bfb32c != (int *)0x0) {
      cVar4 = (**(code **)(*DAT_01bfb32c + 4))(s_System_DigiCombine_Gacha_Change__01ae1b74);
      if (cVar4 != '\0') {
        (**(code **)(*DAT_01bfb32c + 8))(s_System_DigiCombine_Gacha_Change__01ae1b74);
      }
      (**(code **)*DAT_01bfb32c)(s_System_DigiCombine_Gacha_Change__01ae1b74);
    }
  }
  if ((int *)param_1[6] != (int *)0x0) {
    (**(code **)(*(int *)param_1[6] + 0x40))(0);
  }
  FUN_011c91c0(pcVar1);
  FUN_011c9340(pcVar1);
  return;
}


