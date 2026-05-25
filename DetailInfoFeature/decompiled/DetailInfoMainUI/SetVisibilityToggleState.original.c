
/* ============================================================ */
/* Function: FUN_011d8250 */
/* Entry: 011d8250 */
/* Signature: undefined FUN_011d8250() */
/* ============================================================ */


void __fastcall FUN_011d8250(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,0x1b3,2,0x1c,0x1c,0,0x1c);
  *(int *)(param_1 + 0xac) = iVar1;
  if (iVar1 != 0) {
    FUN_011d8ff0(1,param_1,&LAB_010db1a0,0);
  }
  return;
}


