
/* ============================================================ */
/* Function: FUN_011b9e30 */
/* Entry: 011b9e30 */
/* Signature: undefined FUN_011b9e30() */
/* ============================================================ */


void __fastcall FUN_011b9e30(int param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 0x9c) != 0) {
    iVar1 = __RTDynamicCast(*(undefined4 *)(*(int *)(param_1 + 0x9c) + 0x34),0,&DAT_01bc4a94,
                            &DAT_01bc5fc4,0);
    if (iVar1 != 0) {
      FUN_0102e1f0(iVar1 + 4);
      *(undefined1 *)(*(int *)(param_1 + 0x78) + 0x56) = 1;
      iVar1 = *(int *)(param_1 + 0x78);
      *(undefined4 *)(iVar1 + 0x68) = 0;
      *(undefined4 *)(iVar1 + 0x6c) = 0;
      *(undefined4 *)(iVar1 + 0x74) = 0;
    }
  }
  return;
}


