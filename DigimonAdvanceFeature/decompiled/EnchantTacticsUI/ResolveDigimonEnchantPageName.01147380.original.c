
/* ============================================================ */
/* Function: FUN_01147380 */
/* Entry: 01147380 */
/* Signature: undefined FUN_01147380() */
/* ============================================================ */


uint __fastcall FUN_01147380(int param_1)

{
  char *_Str1;
  int iVar1;
  uint uVar2;
  
  _Str1 = (char *)FUN_01505ad0(s_PageName_01adb9ec);
  uVar2 = 0;
  if (_Str1 != (char *)0x0) {
    iVar1 = _stricmp(_Str1,s_DigimonEnchantPage_01adba18);
    if (iVar1 == 0) {
      *(undefined4 *)(param_1 + 8) = 10;
      return 1;
    }
    uVar2 = _stricmp(_Str1,s_DigimonDataTradePage_01adba00);
    if (uVar2 == 0) {
      *(undefined4 *)(param_1 + 8) = 0xb;
      return 1;
    }
  }
  return uVar2 & 0xffffff00;
}


