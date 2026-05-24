
/* ============================================================ */
/* Function: FUN_016827f0 */
/* Entry: 016827f0 */
/* Signature: undefined FUN_016827f0() */
/* ============================================================ */


void __fastcall FUN_016827f0(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  uVar1 = *(uint *)(param_1 + 0x34);
  if (0xf < uVar1) {
    iVar2 = *(int *)(param_1 + 0x20);
    uVar4 = uVar1 + 1;
    iVar3 = iVar2;
    if (0xfff < uVar4) {
      iVar3 = *(int *)(iVar2 + -4);
      uVar4 = uVar1 + 0x24;
      if (0x1f < (iVar2 - iVar3) - 4U) goto LAB_01682879;
    }
    FUN_017986fc(iVar3,uVar4);
  }
  *(undefined4 *)(param_1 + 0x30) = 0;
  *(undefined4 *)(param_1 + 0x34) = 0xf;
  *(undefined1 *)(param_1 + 0x20) = 0;
  uVar1 = *(uint *)(param_1 + 0x1c);
  if (0xf < uVar1) {
    iVar2 = *(int *)(param_1 + 8);
    uVar4 = uVar1 + 1;
    iVar3 = iVar2;
    if (0xfff < uVar4) {
      iVar3 = *(int *)(iVar2 + -4);
      uVar4 = uVar1 + 0x24;
      if (0x1f < (iVar2 - iVar3) - 4U) {
LAB_01682879:
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
    }
    FUN_017986fc(iVar3,uVar4);
  }
  *(undefined4 *)(param_1 + 0x18) = 0;
  *(undefined4 *)(param_1 + 0x1c) = 0xf;
  *(undefined1 *)(param_1 + 8) = 0;
  return;
}


