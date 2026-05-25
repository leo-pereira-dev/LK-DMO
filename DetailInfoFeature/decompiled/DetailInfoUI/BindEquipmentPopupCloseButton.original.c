
/* ============================================================ */
/* Function: FUN_011d6580 */
/* Entry: 011d6580 */
/* Signature: undefined FUN_011d6580() */
/* ============================================================ */


void __fastcall FUN_011d6580(int param_1)

{
  int iVar1;
  undefined4 extraout_ECX;
  
  iVar1 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,
                       *(int *)(param_1 + 0x38) + -0x14,2,0x14,0x14,0,0x1c);
  *(int *)(param_1 + 0x90) = iVar1;
  if (iVar1 != 0) {
    FUN_011d7de0(extraout_ECX,param_1,&LAB_010fba80,0);
  }
  return;
}


