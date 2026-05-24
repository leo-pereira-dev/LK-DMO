
/* ============================================================ */
/* Function: FUN_0128ac10 */
/* Entry: 0128ac10 */
/* Signature: undefined FUN_0128ac10() */
/* ============================================================ */


int * __fastcall FUN_0128ac10(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *pvVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199dced;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (int *)FUN_016830c0(0x434,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (int *)0x0) {
    _Dst = (int *)0x0;
  }
  else {
    memset(_Dst,0,0x434);
    FUN_010e8700();
    local_8._0_1_ = 1;
    *_Dst = (int)&DAT_01ad7538;
    _Dst[0x29] = (int)&DAT_01ad752c;
    _Dst[0x107] = 0;
    _Dst[0x108] = 0;
    pvVar1 = operator_new(0x18);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    *(void **)((int)pvVar1 + 8) = pvVar1;
    *(undefined2 *)((int)pvVar1 + 0xc) = 0x101;
    _Dst[0x107] = (int)pvVar1;
    local_8 = CONCAT31(local_8._1_3_,2);
    _Dst[0x109] = 0;
    _Dst[0x10a] = 0;
    pvVar1 = operator_new(0x18);
    *(void **)pvVar1 = pvVar1;
    *(void **)((int)pvVar1 + 4) = pvVar1;
    *(void **)((int)pvVar1 + 8) = pvVar1;
    *(undefined2 *)((int)pvVar1 + 0xc) = 0x101;
    _Dst[0x109] = (int)pvVar1;
    _Dst[0x10b] = 0;
    _Dst[0x10c] = 0;
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_DataTradeDigimonUpgrade_01ae9be4,0x17);
  if (*(code **)(*_Dst + 0x20) == FUN_010f2b50) {
    FUN_010f2b50();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


