
/* ============================================================ */
/* Function: FUN_0128c370 */
/* Entry: 0128c370 */
/* Signature: undefined FUN_0128c370() */
/* ============================================================ */


int * __fastcall FUN_0128c370(undefined4 param_1,int *param_2)

{
  int *_Dst;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199de17;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _Dst = (int *)FUN_016830c0(200,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (_Dst == (int *)0x0) {
    _Dst = (int *)0x0;
  }
  else {
    memset(_Dst,0,200);
    FUN_01287ce0();
    _Dst[0x29] = (int)&DAT_01ae9024;
    _Dst[0x2a] = 0;
    *_Dst = (int)&DAT_01ae8f64;
    _Dst[0x29] = (int)&DAT_01ae8f58;
    _Dst[0x2b] = 0;
    _Dst[0x2c] = 0;
    _Dst[0x2d] = 0;
    _Dst[0x2e] = 0;
    *(undefined1 *)(_Dst + 0x2f) = 0;
    _Dst[0x30] = 0;
    _Dst[0x31] = 0;
  }
  local_8 = 0xffffffff;
  FUN_00f19f10(s_EquipmentUpgradeSystem_01ae9d98,0x16);
  if (*(code **)(*_Dst + 0x20) == FUN_01278990) {
    FUN_01278990();
  }
  else {
    (**(code **)(*_Dst + 0x20))();
  }
  _Dst[0x1e] = *param_2;
  ExceptionList = local_10;
  return _Dst;
}


