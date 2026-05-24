
/* ============================================================ */
/* Function: FUN_015ec4f0 */
/* Entry: 015ec4f0 */
/* Signature: undefined FUN_015ec4f0() */
/* ============================================================ */


undefined4 __fastcall FUN_015ec4f0(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 local_c [8];
  
  uVar1 = FUN_00f64ef0(&stack0x00000004);
  iVar2 = _Find_last<>(local_c,&stack0x00000004,uVar1);
  iVar2 = *(int *)(iVar2 + 4);
  if (iVar2 == 0) {
    iVar2 = *(int *)(param_1 + 0x18);
  }
  if (iVar2 == *(int *)(param_1 + 0x18)) {
    return 0;
  }
  return *(undefined4 *)(iVar2 + 0x10);
}


