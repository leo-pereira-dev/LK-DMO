
/* ============================================================ */
/* Function: FUN_0128bd60 */
/* Entry: 0128bd60 */
/* Signature: undefined FUN_0128bd60() */
/* ============================================================ */


int * __fastcall FUN_0128bd60(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *pvVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199df67;
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
    _Dst[0x29] = (int)&DAT_01ad357c;
    _Dst[0x2a] = 0;
    local_8._0_1_ = 2;
    *_Dst = (int)&DAT_01af61bc;
    _Dst[0x29] = (int)&DAT_01af61b0;
    _Dst[0x2b] = 0;
    _Dst[0x2c] = 0;
    _Dst[0x2d] = 0;
    _Dst[0x2e] = 0;
    _Dst[0x2f] = 0;
    _Dst[0x30] = 0;
    _Dst[0x32] = 0;
    _Dst[0x33] = 0;
    _Dst[0x34] = 0;
    pvVar1 = operator_new(0xc);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    _Dst[0x33] = (int)pvVar1;
    *(undefined1 *)(_Dst + 0x35) = 1;
    _Dst[0x36] = 0;
    _Dst[0x37] = 0;
    _Dst[0x38] = 0;
    local_8._0_1_ = 3;
    _Dst[0x3a] = 0;
    _Dst[0x3b] = 0;
    _Dst[0x3c] = 0;
    pvVar1 = operator_new(0xc);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    _Dst[0x3b] = (int)pvVar1;
    *(undefined1 *)(_Dst + 0x3d) = 1;
    _Dst[0x3e] = 0;
    _Dst[0x3f] = 0;
    _Dst[0x40] = 0;
    local_8._0_1_ = 4;
    _Dst[0x42] = 0;
    _Dst[0x43] = 0;
    _Dst[0x44] = 0;
    pvVar1 = operator_new(0xc);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    _Dst[0x43] = (int)pvVar1;
    *(undefined1 *)(_Dst + 0x45) = 1;
    _Dst[0x46] = 0;
    _Dst[0x47] = 0;
    _Dst[0x48] = 0;
    local_8 = CONCAT31(local_8._1_3_,5);
    _Dst[0x4a] = 0;
    _Dst[0x4b] = 0;
    _Dst[0x4c] = 0;
    pvVar1 = operator_new(0xc);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    _Dst[0x4b] = (int)pvVar1;
    *(undefined1 *)(_Dst + 0x4d) = 1;
    _Dst[0x4e] = 0;
    _Dst[0x4f] = 0;
    _Dst[0x50] = 0;
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_DigimonStatusUI_Help_01ae9d40,0x14);
  if (*(code **)(*_Dst + 0x20) == FUN_010b92b0) {
    FUN_010b92b0();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


