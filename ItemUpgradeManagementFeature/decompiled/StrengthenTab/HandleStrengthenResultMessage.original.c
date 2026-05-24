
/* ============================================================ */
/* Function: FUN_01277870 */
/* Entry: 01277870 */
/* Signature: undefined FUN_01277870() */
/* ============================================================ */


void __thiscall FUN_01277870(int param_1,undefined4 param_2)

{
  int iVar1;
  char *pcVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  
  switch(param_2) {
  case 0:
    if (*(int **)(param_1 + 0x17c) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x17c) + 0x1c))(1);
      iVar1 = *(int *)(param_1 + 0x17c);
      *(undefined1 *)(iVar1 + 0x78) = 1;
      *(undefined1 *)(iVar1 + 0x2d) = 1;
      *(undefined1 *)(iVar1 + 0x88) = 1;
      *(undefined4 *)(iVar1 + 0x6c) = 0;
      *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
    }
    uVar3 = 0;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar8 = 0;
    uVar7 = 0;
    FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_SUCCESS_01ae8da8,0x1b);
    FUN_010656d0(uVar3,uVar4,uVar5,uVar6,uVar8,uVar7);
    if (DAT_01bfb2a0 == 0) goto switchD_01277886_default;
    pcVar2 = s_game_Upgrade_Success_wav_01ae4648;
    break;
  case 1:
    if (*(int **)(param_1 + 0x180) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x180) + 0x1c))(1);
      iVar1 = *(int *)(param_1 + 0x180);
      *(undefined1 *)(iVar1 + 0x78) = 1;
      *(undefined1 *)(iVar1 + 0x2d) = 1;
      *(undefined1 *)(iVar1 + 0x88) = 1;
      *(undefined4 *)(iVar1 + 0x6c) = 0;
      *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
    }
    uVar3 = 0x18;
    pcVar2 = s_EQUIPSYSTEM_UPGRADE_FAIL_01ae8e0c;
    goto LAB_01277942;
  case 2:
    if (*(int **)(param_1 + 0x184) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x184) + 0x1c))(1);
      iVar1 = *(int *)(param_1 + 0x184);
      *(undefined1 *)(iVar1 + 0x78) = 1;
      *(undefined1 *)(iVar1 + 0x2d) = 1;
      *(undefined1 *)(iVar1 + 0x88) = 1;
      *(undefined4 *)(iVar1 + 0x6c) = 0;
      *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
    }
    uVar3 = 0;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar8 = 0;
    uVar7 = 0;
    FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_DESTROY_01ae8e44,0x1b);
    FUN_010656d0(uVar3,uVar4,uVar5,uVar6,uVar8,uVar7);
    if (DAT_01bfb2a0 == 0) goto switchD_01277886_default;
    pcVar2 = s_game_Upgrade_Destory_wav_01ae8e28;
    break;
  case 3:
    if (*(int **)(param_1 + 0x180) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x180) + 0x1c))(1);
      iVar1 = *(int *)(param_1 + 0x180);
      *(undefined1 *)(iVar1 + 0x78) = 1;
      *(undefined1 *)(iVar1 + 0x2d) = 1;
      *(undefined1 *)(iVar1 + 0x88) = 1;
      *(undefined4 *)(iVar1 + 0x6c) = 0;
      *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
    }
    uVar3 = 0x1e;
    pcVar2 = s_EQUIPSYSTEM_UPGRADE_BACKUPDISK_01ae8dec;
LAB_01277942:
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar7 = 0;
    FUN_00f19cc0(pcVar2,uVar3);
    FUN_010656d0(uVar4,uVar5,uVar6,uVar8,uVar7,uVar9);
    if (DAT_01bfb2a0 == 0) goto switchD_01277886_default;
    pcVar2 = s_game_Upgrade_Fail_wav_01ae4630;
    break;
  default:
    goto switchD_01277886_default;
  }
  FUN_00f54e80(pcVar2,DAT_01bfb2a0);
switchD_01277886_default:
  *(undefined1 *)(param_1 + 0x15c) = 0;
  FUN_01277a60(1);
  return;
}


