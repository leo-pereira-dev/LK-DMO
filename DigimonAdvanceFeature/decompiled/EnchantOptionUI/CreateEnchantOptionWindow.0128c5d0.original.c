
/* ============================================================ */
/* Function: FUN_0128c5d0 */
/* Entry: 0128c5d0 */
/* Signature: undefined FUN_0128c5d0() */
/* ============================================================ */


int * __fastcall FUN_0128c5d0(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199e0a7;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (int *)FUN_016830c0(0x13c,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (int *)0x0) {
    _Dst = (int *)0x0;
  }
  else {
    memset(_Dst,0,0x13c);
    FUN_01287ce0();
    _Dst[0x29] = (int)&DAT_01ae8554;
    _Dst[0x2a] = 0;
    *_Dst = (int)&DAT_01af3544;
    _Dst[0x29] = (int)&DAT_01af3538;
    _Dst[0x2b] = 0;
    _Dst[0x2c] = 0;
    _Dst[0x2d] = 0;
    _Dst[0x2e] = 0;
    _Dst[0x2f] = 0;
    *(undefined1 *)(_Dst + 0x30) = 0;
    _Dst[0x31] = 0;
    *(undefined1 *)(_Dst + 0x32) = 0;
    _Dst[0x34] = 0;
    _Dst[0x35] = 0;
    _Dst[0x36] = 0;
    _Dst[0x37] = 0;
    _Dst[0x38] = 0;
    _Dst[0x39] = 0;
    _Dst[0x3b] = 0;
    _Dst[0x3c] = 0;
    _Dst[0x3d] = 0;
    _Dst[0x3e] = 0;
    _Dst[0x3f] = 0;
    _Dst[0x40] = 0;
    _Dst[0x41] = 0;
    _Dst[0x42] = 0;
    _Dst[0x43] = 0;
    _Dst[0x44] = 0;
    _Dst[0x45] = 0;
    _Dst[0x46] = 0;
    _Dst[0x47] = 0;
    _Dst[0x48] = 0;
    _Dst[0x49] = 0;
    _Dst[0x4a] = 0;
    _Dst[0x4b] = 0;
    _Dst[0x4c] = 0;
    _Dst[0x4d] = 0;
    _Dst[0x4e] = 0;
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_EnchantOption_01ae9dc0,0xd);
  if (*(code **)(*_Dst + 0x20) == FUN_01371e40) {
    FUN_01371e40();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


