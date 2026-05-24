
/* ============================================================ */
/* Function: FUN_011bd3b0 */
/* Entry: 011bd3b0 */
/* Signature: undefined FUN_011bd3b0() */
/* ============================================================ */


void __fastcall FUN_011bd3b0(int param_1)

{
  int iVar1;
  
  if ((*(int *)(param_1 + 0x84) != 0) &&
     (iVar1 = *(int *)(*(int *)(param_1 + 0x84) + 0x6c), iVar1 != 0)) {
    iVar1 = __RTDynamicCast(*(undefined4 *)(iVar1 + 8),0,&DAT_01bc4a94,&DAT_01bc6058,0);
    if (iVar1 != 0) {
      FUN_0102e350(iVar1 + 4);
    }
  }
  return;
}


