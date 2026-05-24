
/* ============================================================ */
/* Function: FUN_015e9a60 */
/* Entry: 015e9a60 */
/* Signature: undefined FUN_015e9a60() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

uint * __thiscall FUN_015e9a60(uint *param_1,undefined4 param_2)

{
  uint *puVar1;
  uint *puVar2;
  undefined4 *puVar3;
  uint uVar4;
  char cVar5;
  uint uVar6;
  uint uVar7;
  void *pvVar8;
  int *piVar9;
  uint uVar10;
  undefined1 local_38 [8];
  undefined4 local_30;
  uint local_2c;
  uint local_28;
  uint local_24;
  undefined4 local_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c8d9d;
  local_10 = ExceptionList;
  uVar6 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  puVar1 = param_1 + 1;
  uVar4 = *param_1;
  local_30 = param_2;
  uVar7 = param_1[2] - *puVar1;
  local_24 = 0;
  uVar10 = uVar7 - uVar4;
  if (3 < uVar10) {
    local_14 = uVar6;
    if (uVar7 <= uVar4) {
LAB_015e9c05:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar6);
    }
    puVar2 = (uint *)(*puVar1 + uVar4);
    if (puVar2 == (uint *)0x0) {
      memset(&local_24,0,uVar10);
      piVar9 = _errno();
      *piVar9 = 0x16;
      _invalid_parameter_noinfo();
      local_2c = local_24;
    }
    else {
      local_2c = *puVar2;
    }
    *param_1 = *param_1 + 4;
    local_28 = 0;
    if (local_2c != 0) {
      do {
        local_20 = 0;
        local_1c = 0;
        pvVar8 = operator_new(0x2c);
        *(void **)pvVar8 = pvVar8;
        *(void **)((int)pvVar8 + 4) = pvVar8;
        *(void **)((int)pvVar8 + 8) = pvVar8;
        *(undefined2 *)((int)pvVar8 + 0xc) = 0x101;
        local_1c = CONCAT44(local_1c._4_4_,pvVar8);
        uVar7 = param_1[2] - *puVar1;
        uVar4 = *param_1;
        uVar10 = uVar7 - uVar4;
        local_8 = 0;
        if (uVar10 < 4) {
LAB_015e9bdd:
          FUN_015ea0b0(&local_1c);
          ExceptionList = local_10;
          return param_1;
        }
        if (uVar7 <= uVar4) goto LAB_015e9c05;
        puVar3 = (undefined4 *)(*puVar1 + uVar4);
        if (puVar3 == (undefined4 *)0x0) {
          memset(&local_20,0,uVar10);
          piVar9 = _errno();
          *piVar9 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_20 = *puVar3;
        }
        *param_1 = *param_1 + 4;
        cVar5 = FUN_015eb0e0(&local_1c,puVar1,param_1);
        if (cVar5 == '\0') goto LAB_015e9bdd;
        FUN_015ea750(local_38,&local_20,&local_1c);
        local_8 = 0xffffffff;
        FUN_015ea0b0(&local_1c);
        local_28 = local_28 + 1;
      } while (local_28 < local_2c);
    }
  }
  ExceptionList = local_10;
  return param_1;
}


