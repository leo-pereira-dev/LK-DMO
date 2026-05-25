
/* ============================================================ */
/* Function: FUN_011d14a0 */
/* Entry: 011d14a0 */
/* Signature: undefined FUN_011d14a0() */
/* ============================================================ */


void __fastcall FUN_011d14a0(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,
                       *(int *)(param_1 + 0x38) + -0x14,2,0x14,0x14,0,0x1c);
  *(int *)(param_1 + 0x7c) = iVar1;
  if (iVar1 != 0) {
    FUN_011d1890(1,param_1,&LAB_010fba80,0);
  }
  return;
}


