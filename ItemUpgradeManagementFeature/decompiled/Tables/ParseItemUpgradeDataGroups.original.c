
/* ============================================================ */
/* Function: FUN_015eb540 */
/* Entry: 015eb540 */
/* Signature: undefined FUN_015eb540() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_015eb540(undefined4 param_1,int *param_2,uint *param_3)

{
  uint *puVar1;
  uint uVar2;
  char cVar3;
  uint uVar4;
  uint uVar5;
  void *pvVar6;
  int *piVar7;
  undefined4 uVar8;
  uint _Size;
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
  puStack_c = &LAB_019c8ffd;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_30 = param_1;
  uVar5 = param_2[1] - *param_2;
  uVar2 = *param_3;
  _Size = uVar5 - uVar2;
  local_24 = 0;
  if (_Size < 4) {
LAB_015eb6e9:
    uVar8 = 0;
  }
  else {
    local_14 = uVar4;
    if (uVar5 <= uVar2) {
LAB_015eb707:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar4);
    }
    puVar1 = (uint *)(*param_2 + uVar2);
    if (puVar1 == (uint *)0x0) {
      memset(&local_24,0,_Size);
      piVar7 = _errno();
      *piVar7 = 0x16;
      _invalid_parameter_noinfo();
      local_2c = local_24;
    }
    else {
      local_2c = *puVar1;
    }
    *param_3 = *param_3 + 4;
    local_28 = 0;
    if (local_2c != 0) {
      do {
        local_20 = 0;
        local_1c = 0;
        pvVar6 = operator_new(0x40);
        *(void **)pvVar6 = pvVar6;
        *(void **)((int)pvVar6 + 4) = pvVar6;
        *(void **)((int)pvVar6 + 8) = pvVar6;
        *(undefined2 *)((int)pvVar6 + 0xc) = 0x101;
        local_1c = CONCAT44(local_1c._4_4_,pvVar6);
        uVar2 = *param_3;
        uVar5 = (param_2[1] - *param_2) - uVar2;
        local_8 = 0;
        if (uVar5 < 4) {
LAB_015eb6ce:
          FUN_015ea100(&local_1c,*(undefined4 *)((int)pvVar6 + 4));
          FUN_017986fc((void *)local_1c,0x40);
          goto LAB_015eb6e9;
        }
        if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_015eb707;
        if ((undefined4 *)(*param_2 + uVar2) == (undefined4 *)0x0) {
          memset(&local_20,0,uVar5);
          piVar7 = _errno();
          *piVar7 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_20 = *(undefined4 *)(*param_2 + uVar2);
        }
        *param_3 = *param_3 + 4;
        cVar3 = FUN_015ea9c0(&local_1c,param_2,param_3);
        if (cVar3 == '\0') {
          pvVar6 = (void *)local_1c;
          goto LAB_015eb6ce;
        }
        FUN_015ea4e0(local_38,&local_20,&local_1c);
        local_8 = 0xffffffff;
        FUN_015ea100(&local_1c,*(undefined4 *)((int)(void *)local_1c + 4));
        FUN_017986fc((void *)local_1c,0x40);
        local_28 = local_28 + 1;
      } while (local_28 < local_2c);
    }
    uVar8 = 1;
  }
  ExceptionList = local_10;
  return uVar8;
}


