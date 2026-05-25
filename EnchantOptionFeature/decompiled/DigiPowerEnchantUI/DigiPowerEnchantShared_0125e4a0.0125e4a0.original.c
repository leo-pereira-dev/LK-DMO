
/* ============================================================ */
/* Function: FUN_0125e4a0 */
/* Entry: 0125e4a0 */
/* Signature: undefined FUN_0125e4a0() */
/* ============================================================ */


void __fastcall FUN_0125e4a0(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = *(char *)(param_1 + 0x74);
  *(undefined2 *)(param_1 + 0x74) = 0x101;
  if (cVar1 == '\0') {
    uVar2 = *(undefined4 *)(*(int *)(param_1 + 0xa8) + 0xc);
    *(undefined4 *)(param_1 + 0xac) = uVar2;
    switch(uVar2) {
    case 1:
      FUN_0125ea10();
      break;
    case 2:
    case 3:
      FUN_0125ed60(param_1);
      return;
    }
  }
  return;
}


