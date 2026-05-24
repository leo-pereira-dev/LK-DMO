
/* ============================================================ */
/* Function: FUN_012022e0 */
/* Entry: 012022e0 */
/* Signature: undefined FUN_012022e0() */
/* ============================================================ */


void __thiscall FUN_012022e0(int *param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  undefined4 uStack_4;
  
  if ((param_1[2] != 0) && (*(char *)(param_1[2] + 0x1c) != '\0')) {
    uStack_4 = param_1;
    if ((int *)*param_1 != (int *)0x0) {
      (**(code **)(*(int *)*param_1 + 0x58))(s_DigiCombine_DigiCombine_CardBack_01ae1a84,1);
    }
    if (param_1[3] != 0) {
      if (*(int *)(param_1[3] + 0x1c) != 0) {
        FUN_0124b170();
      }
      (**(code **)(*(int *)param_1[3] + 0x40))(0);
    }
    puVar1 = *(undefined4 **)(param_2 + 0x10);
    for (puVar2 = (undefined4 *)*puVar1; puVar2 != puVar1; puVar2 = (undefined4 *)*puVar2) {
      if ((param_1[3] != 0) && (*(int *)(param_1[3] + 0x1c) != 0)) {
        FUN_0124af60(puVar2 + 3,puVar2[9],puVar2[10],puVar2[0xb],puVar2[0xc]);
      }
    }
    cVar5 = *(char *)(param_2 + 0xc);
    if (((param_1[3] != 0) && (iVar4 = *(int *)(param_1[3] + 0x1c), iVar4 != 0)) &&
       (iVar4 = __RTDynamicCast(*(undefined4 *)(iVar4 + 0x34),0,&DAT_01bc4a94,&DAT_01bc65d0,0),
       iVar4 != 0)) {
      *(undefined4 *)(iVar4 + 8) = *(undefined4 *)(param_2 + 4);
      *(int *)(iVar4 + 0xc) = (int)cVar5;
      *(undefined1 *)(iVar4 + 0x10) = *(undefined1 *)(param_2 + 0x18);
    }
    if ((int *)param_1[4] != (int *)0x0) {
      (**(code **)(*(int *)param_1[4] + 0x40))(0);
      if (*(int *)(param_1[4] + 0x24) != 0) {
        *(undefined1 *)(*(int *)(param_1[4] + 0x24) + 0x88) = 0;
      }
      iVar4 = param_1[4];
      iVar3 = *(int *)(iVar4 + 0x24);
      if (iVar3 != 0) {
        *(undefined1 *)(iVar3 + 0x78) = 0;
        iVar4 = *(int *)(iVar4 + 0x24);
        *(undefined4 *)(iVar4 + 0x6c) = 0;
        *(undefined4 *)(iVar4 + 0x68) = *(undefined4 *)(iVar4 + 100);
      }
    }
    if ((int *)param_1[5] != (int *)0x0) {
      (**(code **)(*(int *)param_1[5] + 0x40))(0);
      if (*(int *)(param_1[5] + 0x24) != 0) {
        *(undefined1 *)(*(int *)(param_1[5] + 0x24) + 0x88) = 0;
      }
    }
    if ((int *)param_1[6] != (int *)0x0) {
      (**(code **)(*(int *)param_1[6] + 0x40))(0);
    }
    if ((int *)param_1[7] != (int *)0x0) {
      (**(code **)(*(int *)param_1[7] + 0x40))(0);
      iVar4 = param_1[7];
      iVar3 = *(int *)(iVar4 + 0x24);
      if (iVar3 != 0) {
        *(undefined1 *)(iVar3 + 0x78) = 0;
        iVar4 = *(int *)(iVar4 + 0x24);
        *(undefined4 *)(iVar4 + 0x6c) = 0;
        *(undefined4 *)(iVar4 + 0x68) = *(undefined4 *)(iVar4 + 100);
      }
    }
    cVar5 = *(char *)(param_2 + 0xc) + -1;
    if (*(char *)(param_2 + 0x18) == '\0') {
      cVar5 = *(char *)(param_2 + 0xc);
    }
    uStack_4 = (int *)CONCAT13(cVar5,(undefined3)uStack_4);
    FUN_01202700((int)&uStack_4 + 3);
    cVar5 = *(char *)(param_2 + 0xc) + -1;
    if (*(char *)(param_2 + 0x18) == '\0') {
      cVar5 = *(char *)(param_2 + 0xc);
    }
    uStack_4 = (int *)CONCAT13(cVar5,(undefined3)uStack_4);
    FUN_01202880((int)&uStack_4 + 3);
  }
  return;
}


