
/* ============================================================ */
/* Function: FUN_011b6d30 */
/* Entry: 011b6d30 */
/* Signature: undefined FUN_011b6d30() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011b6d30(int param_1,int *param_2,uint *param_3)

{
  uint _Size;
  uint uVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_01975fbd;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if (*param_2 == 4) {
    local_18 = 0;
    local_20 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    local_1c = 7;
    local_30 = 0;
    local_8 = 0;
    uVar3 = param_3[2];
    if ((uVar3 != param_3[3]) && (uVar1 = *param_3, uVar1 < param_3[1])) {
      uVar4 = param_3[3] - uVar3;
      _Size = uVar4 - uVar1;
      if (3 < _Size) {
        if (uVar4 <= uVar1) {
                    /* WARNING: Subroutine does not return */
          FUN_00f3fcb0(local_14);
        }
        if ((undefined4 *)(uVar1 + uVar3) == (undefined4 *)0x0) {
          memset(&local_18,0,_Size);
          piVar2 = _errno();
          *piVar2 = 0x16;
          _invalid_parameter_noinfo();
          uVar3 = param_3[2];
        }
        else {
          local_18 = *(undefined4 *)(uVar1 + uVar3);
        }
        *param_3 = *param_3 + 4;
      }
    }
    if ((uVar3 != param_3[3]) && (*param_3 < param_3[1])) {
      FUN_0100f610(&local_30,param_3 + 2,param_3);
    }
    FUN_011b7bb0(&local_18,&local_30);
    *(undefined1 *)(param_1 + -0xc) = 1;
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


