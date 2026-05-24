
/* ============================================================ */
/* Function: FUN_01290160 */
/* Entry: 01290160 */
/* Signature: undefined FUN_01290160() */
/* ============================================================ */


int * __fastcall FUN_01290160(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199e8cd;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (int *)FUN_016830c0(0x144,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (int *)0x0) {
    _Dst = (int *)0x0;
  }
  else {
    memset(_Dst,0,0x144);
    FUN_01287ce0();
    _Dst[0x29] = (int)&DAT_01ae489c;
    _Dst[0x2a] = 0;
    local_8 = CONCAT31(local_8._1_3_,2);
    *_Dst = (int)&DAT_01aea1b4;
    _Dst[0x29] = (int)&DAT_01aea274;
    _Dst[0x2b] = 0;
    _Dst[0x2c] = 0;
    _Dst[0x2d] = 0;
    _Dst[0x2e] = 0;
    _Dst[0x2f] = 0;
    _Dst[0x30] = 0;
    _Dst[0x31] = 0;
    _Dst[0x32] = 0;
    _Dst[0x33] = 0;
    FUN_01245550();
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_UnionCombineUI_01aea15c,0xe);
  if (*(code **)(*_Dst + 0x20) == FUN_011f99e0) {
    FUN_011f99e0();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


