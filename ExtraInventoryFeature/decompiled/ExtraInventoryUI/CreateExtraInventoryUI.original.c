
/* ============================================================ */
/* Function: FUN_0128ef50 */
/* Entry: 0128ef50 */
/* Signature: undefined FUN_0128ef50() */
/* ============================================================ */


int * __fastcall FUN_0128ef50(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *pvVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199e56d;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (int *)FUN_016830c0(0xbc,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (int *)0x0) {
    _Dst = (int *)0x0;
  }
  else {
    memset(_Dst,0,0xbc);
    FUN_01287ce0();
    _Dst[0x29] = (int)&DAT_01ae1118;
    _Dst[0x2a] = 0;
    local_8 = CONCAT31(local_8._1_3_,2);
    *_Dst = (int)&DAT_01aea418;
    _Dst[0x29] = (int)&DAT_01aea4d8;
    _Dst[0x2b] = 0;
    _Dst[0x2c] = 0;
    _Dst[0x2d] = 0;
    _Dst[0x2e] = 0;
    pvVar1 = operator_new(0x18);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    *(void **)((int)pvVar1 + 8) = pvVar1;
    *(undefined2 *)((int)pvVar1 + 0xc) = 0x101;
    _Dst[0x2d] = (int)pvVar1;
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_ExtraInventoryUI_01aea07c,0x10);
  if (*(code **)(*_Dst + 0x20) == FUN_011a90d0) {
    FUN_011a90d0();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


