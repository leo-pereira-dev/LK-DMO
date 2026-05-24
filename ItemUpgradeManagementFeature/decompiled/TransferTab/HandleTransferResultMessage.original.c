
/* ============================================================ */
/* Function: FUN_01271060 */
/* Entry: 01271060 */
/* Signature: undefined FUN_01271060() */
/* ============================================================ */


void __thiscall FUN_01271060(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  char *pcVar7;
  undefined4 uVar8;
  
  if (param_2 == 0) {
    if (*(int **)(param_1 + 0x138) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x138) + 0x1c))(1);
      iVar1 = *(int *)(param_1 + 0x138);
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
    uVar8 = 0;
    FUN_00f19cc0(s_DATA_TRANSFER_SUCCESS_01ae8d24,0x15);
    FUN_010656d0(uVar2,uVar3,uVar4,uVar5,uVar6,uVar8);
    if (DAT_01bfb2a0 == 0) goto LAB_0127115c;
    pcVar7 = s_game_Upgrade_Success_wav_01ae4648;
  }
  else {
    if (param_2 != 1) goto LAB_0127115c;
    if (*(int **)(param_1 + 0x13c) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x13c) + 0x1c))(1);
      iVar1 = *(int *)(param_1 + 0x13c);
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
    uVar8 = 0;
    FUN_00f19cc0(s_DATA_TRANSFER_CONFLICT_01ae8d0c,0x16);
    FUN_010656d0(uVar2,uVar3,uVar4,uVar5,uVar6,uVar8);
    if (DAT_01bfb2a0 == 0) goto LAB_0127115c;
    pcVar7 = s_game_Upgrade_Fail_wav_01ae4630;
  }
  FUN_00f54e80(pcVar7,DAT_01bfb2a0);
LAB_0127115c:
  (**(code **)(**(int **)(param_1 + 0x88) + 0x18))(1);
  if ((*(int **)(param_1 + 0xcc) != (int *)0x0) && (*(int *)(param_1 + 0xd0) != 0)) {
    (**(code **)(**(int **)(param_1 + 0xcc) + 0x24))(1);
    (**(code **)(**(int **)(param_1 + 0xd0) + 0x24))(1);
  }
  return;
}


