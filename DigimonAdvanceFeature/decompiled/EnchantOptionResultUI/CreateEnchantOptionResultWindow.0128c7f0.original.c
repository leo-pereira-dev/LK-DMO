
/* ============================================================ */
/* Function: FUN_0128c7f0 */
/* Entry: 0128c7f0 */
/* Signature: undefined FUN_0128c7f0() */
/* ============================================================ */


int * __fastcall FUN_0128c7f0(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *pvVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199e10b;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (int *)FUN_016830c0(0xfc,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (int *)0x0) {
    _Dst = (int *)0x0;
  }
  else {
    memset(_Dst,0,0xfc);
    FUN_01287ce0();
    _Dst[0x29] = (int)&DAT_01ae8554;
    _Dst[0x2a] = 0;
    local_8._0_1_ = 2;
    *_Dst = (int)&DAT_01ae89a4;
    _Dst[0x29] = (int)&DAT_01ae8a64;
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
    local_8 = CONCAT31(local_8._1_3_,3);
    _Dst[0x37] = 0;
    _Dst[0x38] = 0;
    _Dst[0x39] = 0;
    pvVar1 = operator_new(0xc);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    _Dst[0x38] = (int)pvVar1;
    *(undefined1 *)(_Dst + 0x3a) = 1;
    _Dst[0x3b] = 0;
    _Dst[0x3c] = 0;
    _Dst[0x3d] = 0;
    _Dst[0x3e] = 0;
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_EnchantOptionResultUI_01ae9df8,0x15);
  if (*(code **)(*_Dst + 0x20) == FUN_01267760) {
    FUN_01267760();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


