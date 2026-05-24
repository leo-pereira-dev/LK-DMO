
/* ============================================================ */
/* Function: FUN_012b80c0 */
/* Entry: 012b80c0 */
/* Signature: undefined FUN_012b80c0() */
/* ============================================================ */


undefined4 __thiscall FUN_012b80c0(int param_1,int *param_2,uint *param_3)

{
  undefined4 *puVar1;
  uint uVar2;
  int *piVar3;
  undefined4 *puVar4;
  uint uVar5;
  uint uVar6;
  
  puVar4 = (undefined4 *)(param_1 + 4);
  uVar2 = *param_3;
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_012b82ac;
    if (puVar4 == (undefined4 *)0x0) {
LAB_012b811f:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar2 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_012b811f;
      }
      *puVar4 = *(undefined4 *)(uVar2 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  uVar5 = param_2[1] - *param_2;
  puVar4 = (undefined4 *)(param_1 + 8);
  uVar6 = uVar5 - uVar2;
  if (3 < uVar6) {
    if (uVar5 <= uVar2) goto LAB_012b82ac;
    puVar1 = (undefined4 *)(uVar2 + *param_2);
    if (puVar4 == (undefined4 *)0x0) {
LAB_012b8173:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if (puVar1 == (undefined4 *)0x0) {
        memset(puVar4,0,uVar6);
        goto LAB_012b8173;
      }
      *puVar4 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  puVar4 = (undefined4 *)(param_1 + 0xc);
  uVar5 = param_2[1] - *param_2;
  uVar6 = uVar5 - uVar2;
  if (3 < uVar6) {
    if (uVar5 <= uVar2) goto LAB_012b82ac;
    puVar1 = (undefined4 *)(uVar2 + *param_2);
    if (puVar4 == (undefined4 *)0x0) {
LAB_012b81c7:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if (puVar1 == (undefined4 *)0x0) {
        memset(puVar4,0,uVar6);
        goto LAB_012b81c7;
      }
      *puVar4 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  puVar4 = (undefined4 *)(param_1 + 0x10);
  uVar5 = param_2[1] - *param_2;
  uVar6 = uVar5 - uVar2;
  if (3 < uVar6) {
    if (uVar5 <= uVar2) goto LAB_012b82ac;
    puVar1 = (undefined4 *)(uVar2 + *param_2);
    if (puVar4 == (undefined4 *)0x0) {
LAB_012b821b:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if (puVar1 == (undefined4 *)0x0) {
        memset(puVar4,0,uVar6);
        goto LAB_012b821b;
      }
      *puVar4 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  puVar4 = (undefined4 *)(param_1 + 0x14);
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) {
LAB_012b82ac:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0();
    }
    if (puVar4 != (undefined4 *)0x0) {
      if ((undefined4 *)(uVar2 + *param_2) != (undefined4 *)0x0) {
        *puVar4 = *(undefined4 *)(uVar2 + *param_2);
        *param_3 = *param_3 + 4;
        return 1;
      }
      memset(puVar4,0,uVar5);
    }
    piVar3 = _errno();
    *piVar3 = 0x16;
    _invalid_parameter_noinfo();
    *param_3 = *param_3 + 4;
  }
  return 1;
}


