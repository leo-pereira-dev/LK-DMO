
/* ============================================================ */
/* Function: FUN_015ed170 */
/* Entry: 015ed170 */
/* Signature: undefined FUN_015ed170() */
/* ============================================================ */


undefined4 __thiscall FUN_015ed170(int param_1,int *param_2,uint *param_3)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  undefined4 *puVar4;
  uint uVar5;
  
  puVar4 = (undefined4 *)(param_1 + 4);
  uVar2 = *param_3;
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_015ed436;
    if (puVar4 == (undefined4 *)0x0) {
LAB_015ed1ce:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar2 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_015ed1ce;
      }
      *puVar4 = *(undefined4 *)(uVar2 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_015ed436;
    puVar4 = (undefined4 *)(param_1 + 8);
    if (puVar4 == (undefined4 *)0x0) {
LAB_015ed21b:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar2 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_015ed21b;
      }
      *puVar4 = *(undefined4 *)(uVar2 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_015ed436;
    puVar4 = (undefined4 *)(param_1 + 0xc);
    if (puVar4 == (undefined4 *)0x0) {
LAB_015ed26b:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar2 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_015ed26b;
      }
      *puVar4 = *(undefined4 *)(uVar2 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_015ed436;
    puVar4 = (undefined4 *)(param_1 + 0x10);
    if (puVar4 == (undefined4 *)0x0) {
LAB_015ed2bb:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar2 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_015ed2bb;
      }
      *puVar4 = *(undefined4 *)(uVar2 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_015ed436;
    puVar4 = (undefined4 *)(uVar2 + *param_2);
    if ((void *)(param_1 + 0x14) == (void *)0x0) {
LAB_015ed318:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if (puVar4 == (undefined4 *)0x0) {
        memset((void *)(param_1 + 0x14),0,uVar5);
        goto LAB_015ed318;
      }
      *(undefined4 *)(param_1 + 0x14) = *puVar4;
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_015ed436;
    puVar4 = (undefined4 *)(param_1 + 0x18);
    if (puVar4 == (undefined4 *)0x0) {
LAB_015ed368:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar2 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_015ed368;
      }
      *puVar4 = *(undefined4 *)(uVar2 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  uVar5 = (param_2[1] - *param_2) - uVar2;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - *param_2) <= uVar2) goto LAB_015ed436;
    puVar4 = (undefined4 *)(param_1 + 0x1c);
    if (puVar4 == (undefined4 *)0x0) {
LAB_015ed3b8:
      piVar3 = _errno();
      *piVar3 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar2 + *param_2) == (undefined4 *)0x0) {
        memset(puVar4,0,uVar5);
        goto LAB_015ed3b8;
      }
      *puVar4 = *(undefined4 *)(uVar2 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar2 = *param_3;
  }
  iVar1 = *param_2;
  uVar5 = (param_2[1] - uVar2) - iVar1;
  if (3 < uVar5) {
    if ((uint)(param_2[1] - iVar1) <= uVar2) {
LAB_015ed436:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0();
    }
    puVar4 = (undefined4 *)(param_1 + 0x20);
    if (puVar4 != (undefined4 *)0x0) {
      if ((undefined4 *)(uVar2 + iVar1) != (undefined4 *)0x0) {
        *puVar4 = *(undefined4 *)(uVar2 + iVar1);
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


