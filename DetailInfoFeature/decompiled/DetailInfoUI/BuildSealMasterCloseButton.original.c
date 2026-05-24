
/* ============================================================ */
/* Function: FUN_011da100 */
/* Entry: 011da100 */
/* Signature: undefined FUN_011da100() */
/* ============================================================ */


void __fastcall FUN_011da100(int param_1)

{
  int iVar1;
  undefined4 extraout_ECX;
  
  iVar1 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,
                       *(int *)(param_1 + 0x38) + -0x10,2,0x10,0x10,0,0x1c);
  *(int *)(param_1 + 0x7c) = iVar1;
  if (iVar1 != 0) {
    FUN_011da8e0(extraout_ECX,param_1,&LAB_010fba80,0);
  }
  return;
}


