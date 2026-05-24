
/* ============================================================ */
/* Function: FUN_011ba1e0 */
/* Entry: 011ba1e0 */
/* Signature: undefined FUN_011ba1e0() */
/* ============================================================ */


void __thiscall FUN_011ba1e0(int param_1,int param_2)

{
  int *piVar1;
  char cVar2;
  int iVar3;
  int iStack_c;
  
  (**(code **)(**(int **)(param_1 + 0x98) + 0x1c))(0);
  (**(code **)(**(int **)(param_1 + 0x9c) + 0x1c))(0);
  if (param_2 != 0) {
    iVar3 = __RTDynamicCast(*(undefined4 *)(param_2 + 0x34),0,&DAT_01bc4a94,&DAT_01bc5ff8,0);
    if (iVar3 != 0) {
      FUN_00f1d720(&stack0xffffffec,(int *)(iVar3 + 4));
      if (((*(char *)(iStack_c + 0xd) == '\0') && (*(int *)(iStack_c + 0x10) <= *(int *)(iVar3 + 4))
          ) && (iStack_c != *(int *)(param_1 + 0x7c))) {
        piVar1 = *(int **)(param_1 + 0x9c);
        if ((*(int *)(iStack_c + 0x2c) != 0) &&
           (*(char *)(*(int *)(iStack_c + 0x2c) + 0x1c) != '\0')) {
          if (*(int *)(iStack_c + 0x1c) != 0) {
            (**(code **)(**(int **)(param_1 + 0x98) + 0x1c))(0);
            (**(code **)(*piVar1 + 0x1c))(0);
            iVar3 = *(int *)(*(int *)(iStack_c + 0x1c) + 0x24);
            if (iVar3 != 0) {
              *(undefined1 *)(iVar3 + 0x78) = 1;
              *(undefined1 *)(iVar3 + 0x2d) = 1;
              *(undefined1 *)(iVar3 + 0x88) = 1;
              *(undefined4 *)(iVar3 + 0x6c) = 4;
              *(undefined4 *)(iVar3 + 0x68) = *(undefined4 *)(iVar3 + 100);
            }
            if (DAT_01bfb32c != (int *)0x0) {
              cVar2 = (**(code **)(*DAT_01bfb32c + 4))(s_System_DigiCombine_Gacha_Progres_01ae1b00);
              if (cVar2 != '\0') {
                (**(code **)(*DAT_01bfb32c + 8))(s_System_DigiCombine_Gacha_Progres_01ae1b00);
              }
              (**(code **)*DAT_01bfb32c)(s_System_DigiCombine_Gacha_Progres_01ae1b00);
            }
          }
          FUN_011ba9b0();
        }
      }
      if (*(char *)(*(int *)(param_1 + 0x90) + 0x2d) == '\0') {
        (**(code **)(**(int **)(param_1 + 0x94) + 0x1c))(1);
      }
    }
  }
  return;
}


