
/* ============================================================ */
/* Function: FUN_01682ad0 */
/* Entry: 01682ad0 */
/* Signature: undefined FUN_01682ad0() */
/* ============================================================ */


undefined4 __thiscall FUN_01682ad0(int param_1,int *param_2,uint *param_3)

{
  size_t _Size;
  uint uVar1;
  undefined1 *puVar2;
  int *piVar3;
  undefined4 *puVar4;
  undefined1 *_Dst;
  uint uVar5;
  
  FUN_0102b8e0(param_1 + 8,param_2,param_3);
  FUN_0102b8e0(param_1 + 0x20,param_2,param_3);
  uVar1 = *param_3;
  puVar4 = (undefined4 *)(param_1 + 4);
  uVar5 = (param_2[1] - *param_2) - uVar1;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar1) goto LAB_01682c13;
    if (puVar4 == (undefined4 *)0x0) {
LAB_01682b49:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar1 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_01682b49;
      }
      *puVar4 = *(undefined4 *)(uVar1 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar1 = *param_3;
  }
  uVar5 = (param_2[1] - *param_2) - uVar1;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar1) goto LAB_01682c13;
    puVar4 = (undefined4 *)(param_1 + 0x38);
    if (puVar4 == (undefined4 *)0x0) {
LAB_01682b97:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar1 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_01682b97;
      }
      *puVar4 = *(undefined4 *)(uVar1 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar1 = *param_3;
  }
  _Dst = (undefined1 *)(param_1 + 0x3c);
  uVar5 = param_2[1] - *param_2;
  _Size = uVar5 - uVar1;
  if (_Size != 0) {
    if (uVar5 <= uVar1) {
LAB_01682c13:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0();
    }
    puVar2 = (undefined1 *)(uVar1 + *param_2);
    if (_Dst != (undefined1 *)0x0) {
      if (puVar2 != (undefined1 *)0x0) {
        *_Dst = *puVar2;
        *param_3 = *param_3 + 1;
        return 1;
      }
      memset(_Dst,0,_Size);
    }
    piVar3 = _errno();
    *piVar3 = 0x16;
    _invalid_parameter_noinfo();
    *param_3 = *param_3 + 1;
  }
  return 1;
}


