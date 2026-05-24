
/* ============================================================ */
/* Function: FUN_0126c670 */
/* Entry: 0126c670 */
/* Signature: undefined FUN_0126c670() */
/* ============================================================ */


void __fastcall FUN_0126c670(int param_1)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  uint uVar4;
  uint local_18;
  undefined1 local_14 [8];
  int local_c;
  
  local_18 = *(uint *)(param_1 + 0x13c);
  *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + 1;
  iVar1 = FUN_0126c790(&local_18);
  if (*(int *)(param_1 + 0x140) == *(int *)(iVar1 + 4)) {
    *(undefined4 *)(param_1 + 0x140) = 0;
    uVar4 = *(int *)(param_1 + 0x13c) + 1;
    *(uint *)(param_1 + 0x13c) = uVar4;
    local_18 = uVar4;
    FUN_00f1b390(local_14,&local_18);
    if (((*(char *)(local_c + 0xd) == '\0') && (*(uint *)(local_c + 0x10) <= uVar4)) &&
       (local_c != *(int *)(param_1 + 0x134))) {
      local_18 = uVar4;
      piVar2 = (int *)FUN_0126c790(&local_18);
      piVar2 = (int *)*piVar2;
      piVar3 = (int *)*piVar2;
      if (piVar3 != piVar2) {
        do {
          iVar1 = piVar3[2];
          *(undefined1 *)(iVar1 + 0xe4) = 1;
          *(undefined4 *)(iVar1 + 0xe8) = 0;
          piVar3 = (int *)*piVar3;
        } while (piVar3 != piVar2);
        return;
      }
    }
    else {
      iVar1 = *(int *)(param_1 + 0x128);
      *(undefined4 *)(param_1 + 0x13c) = 0xffffffff;
      if (iVar1 != 0) {
        *(undefined1 *)(iVar1 + 0x78) = 1;
        *(undefined1 *)(iVar1 + 0x2d) = 1;
        *(undefined1 *)(iVar1 + 0x88) = 1;
        *(undefined4 *)(iVar1 + 0x6c) = 0;
        *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
      }
      if (DAT_01bfb2a0 != 0) {
        FUN_00f54e80(s_game_Upgrade_Process_wav_01ae8c14,DAT_01bfb2a0);
      }
    }
  }
  return;
}


