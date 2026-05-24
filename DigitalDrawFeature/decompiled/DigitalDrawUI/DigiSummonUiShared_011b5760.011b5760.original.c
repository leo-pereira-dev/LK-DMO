
/* ============================================================ */
/* Function: FUN_011b5760 */
/* Entry: 011b5760 */
/* Signature: undefined FUN_011b5760() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

uint * __fastcall FUN_011b5760(uint *param_1)

{
  char cVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  uint _Size;
  uint uVar5;
  undefined *local_48 [2];
  undefined8 local_40;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_1c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01990fed;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  uVar3 = param_1[2];
  if ((uVar3 != param_1[3]) && (uVar5 = *param_1, uVar5 < param_1[1])) {
    uVar4 = param_1[3] - uVar3;
    local_18 = 0;
    _Size = uVar4 - uVar5;
    if (3 < _Size) {
      if (uVar4 <= uVar5) {
                    /* WARNING: Subroutine does not return */
        FUN_00f3fcb0(local_14);
      }
      if ((uint *)(uVar5 + uVar3) == (uint *)0x0) {
        memset(&local_18,0,_Size);
        piVar2 = _errno();
        *piVar2 = 0x16;
        _invalid_parameter_noinfo();
        uVar3 = local_18;
      }
      else {
        uVar3 = *(uint *)(uVar5 + uVar3);
      }
      *param_1 = *param_1 + 4;
      uVar5 = 0;
      if (uVar3 != 0) {
        do {
          memset(local_48,0,0x30);
          local_48[0] = &DAT_01acfaf8;
          local_40 = 0;
          uStack_30 = 0;
          uStack_2c = 0;
          uStack_28 = 0;
          local_38 = 0;
          local_24 = 0;
          local_20 = 7;
          local_34 = 0;
          local_1c = 0;
          local_8 = 0;
          cVar1 = FUN_010302a0(param_1 + 2,param_1);
          if (cVar1 == '\0') {
            FUN_00f1a330();
            ExceptionList = local_10;
            return param_1;
          }
          FUN_0102d580(local_48);
          local_8 = 0xffffffff;
          FUN_00f1a330();
          uVar5 = uVar5 + 1;
        } while (uVar5 < uVar3);
      }
    }
  }
  ExceptionList = local_10;
  return param_1;
}


