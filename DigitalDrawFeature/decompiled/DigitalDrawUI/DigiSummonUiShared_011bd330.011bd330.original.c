
/* ============================================================ */
/* Function: FUN_011bd330 */
/* Entry: 011bd330 */
/* Signature: undefined FUN_011bd330() */
/* ============================================================ */


void __fastcall FUN_011bd330(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(*(int *)(param_1 + 0x84) + 0x6c);
  if (iVar1 != 0) {
    iVar1 = __RTDynamicCast(*(undefined4 *)(iVar1 + 8),0,&DAT_01bc4a94,&DAT_01bc6058,0);
    if (iVar1 != 0) {
      FUN_0102e1f0(iVar1 + 4);
    }
  }
  return;
}


