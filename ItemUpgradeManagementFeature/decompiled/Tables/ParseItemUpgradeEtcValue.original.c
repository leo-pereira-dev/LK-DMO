
/* ============================================================ */
/* Function: FUN_015eacc0 */
/* Entry: 015eacc0 */
/* Signature: undefined FUN_015eacc0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_015eacc0(undefined4 param_1,int *param_2,uint *param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  int *piVar5;
  uint _Size;
  undefined1 local_2c [8];
  undefined4 local_24;
  uint local_20;
  uint local_1c;
  undefined *local_18;
  undefined4 local_14;
  uint local_10;
  undefined4 local_c;
  uint local_8;
  
  local_8 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  local_24 = param_1;
  iVar1 = *param_2;
  uVar3 = *param_3;
  uVar2 = (param_2[1] - iVar1) - uVar3;
  local_10 = 0;
  if (uVar2 < 4) {
    return 0;
  }
  if (uVar3 < (uint)(param_2[1] - iVar1)) {
    if ((uint *)(iVar1 + uVar3) == (uint *)0x0) {
      memset(&local_10,0,uVar2);
      piVar5 = _errno();
      *piVar5 = 0x16;
      _invalid_parameter_noinfo();
      local_20 = local_10;
    }
    else {
      local_20 = *(uint *)(iVar1 + uVar3);
    }
    *param_3 = *param_3 + 4;
    local_1c = 0;
    if (local_20 != 0) {
      do {
        iVar1 = *param_2;
        uVar3 = *param_3;
        uVar2 = (param_2[1] - iVar1) - uVar3;
        local_c = 0;
        local_18 = &DAT_01a18e78;
        local_14 = 0;
        if (uVar2 < 4) {
          return 0;
        }
        if ((uint)(param_2[1] - iVar1) <= uVar3) goto LAB_015eae48;
        if ((undefined4 *)(iVar1 + uVar3) == (undefined4 *)0x0) {
          memset(&local_c,0,uVar2);
          piVar5 = _errno();
          *piVar5 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_c = *(undefined4 *)(iVar1 + uVar3);
        }
        uVar3 = *param_3 + 4;
        *param_3 = uVar3;
        uVar2 = param_2[1] - *param_2;
        _Size = uVar2 - uVar3;
        if (3 < _Size) {
          if (uVar2 <= uVar3) goto LAB_015eae48;
          puVar4 = (undefined4 *)(uVar3 + *param_2);
          if (puVar4 == (undefined4 *)0x0) {
            memset(&local_14,0,_Size);
            piVar5 = _errno();
            *piVar5 = 0x16;
            _invalid_parameter_noinfo();
          }
          else {
            local_14 = *puVar4;
          }
          *param_3 = *param_3 + 4;
        }
        FUN_015ea300(local_2c,&local_c,&local_18);
        local_1c = local_1c + 1;
      } while (local_1c < local_20);
    }
    return 1;
  }
LAB_015eae48:
                    /* WARNING: Subroutine does not return */
  FUN_00f3fcb0();
}


