
/* ============================================================ */
/* Function: FUN_01267910 */
/* Entry: 01267910 */
/* Signature: undefined FUN_01267910() */
/* ============================================================ */


void __fastcall FUN_01267910(int param_1)

{
  char cVar1;
  
  cVar1 = *(char *)(param_1 + 0x74);
  *(undefined2 *)(param_1 + 0x74) = 0x101;
  if (cVar1 == '\0') {
    FUN_01267bd0();
    *(undefined1 *)(param_1 + 0x68) = 0;
  }
  return;
}


