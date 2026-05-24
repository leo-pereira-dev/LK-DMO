
/* ============================================================ */
/* Function: FUN_011d82a0 */
/* Entry: 011d82a0 */
/* Signature: undefined FUN_011d82a0() */
/* ============================================================ */


void __fastcall FUN_011d82a0(int param_1)

{
  char *pcVar1;
  int iVar2;
  
  if (*(char *)(*(int *)(param_1 + 0xa8) + 0xfd) != '\0') {
    pcVar1 = s_DetailInfo_Autocrop_ON_Btn_bmp_01ae2fe4;
    if (*(char *)(*(int *)(param_1 + 0xa8) + 0xfc) == '\0') {
      pcVar1 = s_DetailInfo_Autocrop_OFF_Btn_bmp_01ae2fc4;
    }
    iVar2 = FUN_012462c0(pcVar1,0x17,1,3,3,0x36,0x1a,0,0x1a);
    *(int *)(param_1 + 0xb0) = iVar2;
    if (iVar2 != 0) {
      FUN_011d8ff0(1,param_1,&LAB_011d8d50,0);
      FUN_012254b0(*(int *)(param_1 + 0xa8) + 0x20);
    }
  }
  return;
}


