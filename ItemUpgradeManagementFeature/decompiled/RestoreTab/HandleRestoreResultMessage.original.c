
/* ============================================================ */
/* Function: FUN_0126c260 */
/* Entry: 0126c260 */
/* Signature: undefined FUN_0126c260() */
/* ============================================================ */


void __thiscall FUN_0126c260(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  if (param_2 == 0) {
    if (*(int **)(param_1 + 300) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 300) + 0x1c))(1);
      iVar1 = *(int *)(param_1 + 300);
      *(undefined1 *)(iVar1 + 0x78) = 1;
      *(undefined1 *)(iVar1 + 0x2d) = 1;
      *(undefined1 *)(iVar1 + 0x88) = 1;
      *(undefined4 *)(iVar1 + 0x6c) = 0;
      *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
    }
    uVar2 = 0;
    uVar3 = 0;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    FUN_00f19cc0(s_DATA_RESTORE_SUCCESS_01ae8bd4,0x14);
    FUN_010656d0(uVar2,uVar3,uVar4,uVar5,uVar6,uVar7);
    if (DAT_01bfb2a0 != 0) {
      FUN_00f54e80(s_game_Upgrade_Success_wav_01ae4648,DAT_01bfb2a0);
    }
  }
  else if (param_2 == 1) {
    if (*(int **)(param_1 + 0x130) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x130) + 0x1c))(1);
      iVar1 = *(int *)(param_1 + 0x130);
      *(undefined1 *)(iVar1 + 0x78) = 1;
      *(undefined1 *)(iVar1 + 0x2d) = 1;
      *(undefined1 *)(iVar1 + 0x88) = 1;
      *(undefined4 *)(iVar1 + 0x6c) = 0;
      *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
    }
    uVar2 = 0;
    uVar3 = 0;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    FUN_00f19cc0(s_DATA_RESTORE_FAIL_01ae8c30,0x11);
    FUN_010656d0(uVar2,uVar3,uVar4,uVar5,uVar6,uVar7);
    if (DAT_01bfb2a0 != 0) {
      FUN_00f54e80(s_game_Upgrade_Fail_wav_01ae4630,DAT_01bfb2a0);
      return;
    }
  }
  return;
}


