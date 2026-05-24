
/* ============================================================ */
/* Function: FUN_0128bfa0 */
/* Entry: 0128bfa0 */
/* Signature: undefined FUN_0128bfa0() */
/* ============================================================ */


int * __fastcall FUN_0128bfa0(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *pvVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199dfbd;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (int *)FUN_016830c0(0xd8,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (int *)0x0) {
    _Dst = (int *)0x0;
  }
  else {
    memset(_Dst,0,0xd8);
    FUN_01287ce0();
    _Dst[0x29] = (int)&DAT_01ad7e4c;
    _Dst[0x2a] = 0;
    local_8 = CONCAT31(local_8._1_3_,2);
    *_Dst = (int)&DAT_01ad9444;
    _Dst[0x29] = (int)&DAT_01ad9504;
    _Dst[0x2b] = 0;
    _Dst[0x2c] = 0;
    _Dst[0x2d] = 0;
    _Dst[0x2f] = 0;
    _Dst[0x30] = 0;
    _Dst[0x31] = 0;
    pvVar1 = operator_new(0xc);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    _Dst[0x30] = (int)pvVar1;
    *(undefined1 *)(_Dst + 0x32) = 1;
    _Dst[0x33] = 0;
    _Dst[0x34] = 0;
    _Dst[0x35] = 0;
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_DigimonTranscendenceHelp_01ae9d7c,0x18);
  if (*(code **)(*_Dst + 0x20) == FUN_01118d40) {
    FUN_01118d40();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


