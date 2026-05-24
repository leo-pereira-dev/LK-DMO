
/* ============================================================ */
/* Function: FUN_0128bab0 */
/* Entry: 0128bab0 */
/* Signature: undefined FUN_0128bab0() */
/* ============================================================ */


int * __fastcall FUN_0128bab0(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199dea7;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (int *)FUN_016830c0(0xcc,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (int *)0x0) {
    _Dst = (int *)0x0;
  }
  else {
    memset(_Dst,0,0xcc);
    FUN_01287ce0();
    _Dst[0x29] = (int)&DAT_01adb678;
    _Dst[0x2a] = 0;
    *_Dst = (int)&DAT_01adb5ac;
    _Dst[0x29] = (int)&DAT_01adb66c;
    _Dst[0x2b] = 0;
    _Dst[0x2c] = 0;
    _Dst[0x2d] = 0;
    _Dst[0x2e] = 0;
    _Dst[0x2f] = 0;
    _Dst[0x30] = 0;
    _Dst[0x31] = 0;
    _Dst[0x32] = 0;
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_TamerStatusUI_01ae9d30,0xd);
  if (*(code **)(*_Dst + 0x20) == FUN_0113f8a0) {
    FUN_0113f8a0();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


