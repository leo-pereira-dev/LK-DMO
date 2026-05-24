
/* ============================================================ */
/* Function: FUN_0128bcc0 */
/* Entry: 0128bcc0 */
/* Signature: undefined FUN_0128bcc0() */
/* ============================================================ */


int * __fastcall FUN_0128bcc0(undefined4 param_1,int *param_2)

{
  void *_Dst;
  int *piVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199dee7;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (void *)FUN_016830c0(0x174,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (void *)0x0) {
    piVar1 = (int *)0x0;
  }
  else {
    memset(_Dst,0,0x174);
    piVar1 = (int *)FUN_013b06c0();
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_DigimonStatusUI_01ae9d58,0xf);
  (**(code **)(*piVar1 + 0x20))();
  piVar1[0x1e] = *param_2;
  ExceptionList = local_10;
  return piVar1;
}


