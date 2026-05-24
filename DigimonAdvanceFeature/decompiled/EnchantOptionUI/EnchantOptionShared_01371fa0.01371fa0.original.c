
/* ============================================================ */
/* Function: FUN_01371fa0 */
/* Entry: 01371fa0 */
/* Signature: undefined FUN_01371fa0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01371fa0(int param_1,undefined4 *param_2,uint *param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  char cVar4;
  uint uVar5;
  uint uVar6;
  int *piVar7;
  undefined4 *puVar8;
  size_t sVar9;
  uint uVar10;
  undefined1 auStack_2c [4];
  undefined4 uStack_28;
  undefined4 uStack_24;
  char cStack_1d;
  uint uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  uint local_c;
  
  local_c = DAT_01bbb1c0 ^ (uint)auStack_2c;
  switch(*param_2) {
  case 0:
    iVar2 = *(int *)(param_1 + 0xc);
    if (iVar2 != 0) {
      *(undefined1 *)(iVar2 + 0xe4) = 1;
      *(undefined4 *)(iVar2 + 0xe8) = 0;
      return;
    }
    break;
  case 1:
    uVar3 = param_3[3];
    uStack_18 = 0;
    if (((param_3[2] != uVar3) && (uVar5 = *param_3, uVar5 < param_3[1])) &&
       (uVar6 = (uVar3 - param_3[2]) - uVar5, 3 < uVar6)) {
      if (uVar3 - param_3[2] <= uVar5) {
LAB_0137239d:
                    /* WARNING: Subroutine does not return */
        FUN_00f3fcb0();
      }
      puVar8 = (undefined4 *)(param_3[2] + uVar5);
      if (puVar8 != (undefined4 *)0x0) {
        uVar1 = *puVar8;
        *param_3 = *param_3 + 4;
        FUN_01374ce0(uVar1);
        return;
      }
      memset(&uStack_18,0,uVar6);
      piVar7 = _errno();
      *piVar7 = 0x16;
      _invalid_parameter_noinfo();
      *param_3 = *param_3 + 4;
    }
    FUN_01374ce0(uStack_18);
    return;
  case 2:
    uVar3 = param_3[3];
    uStack_28 = 0;
    uStack_10 = 0;
    uStack_1c = uStack_1c & 0xffffff00;
    uVar5 = param_3[2];
    uStack_24 = 0;
    uStack_14 = 0;
    if (((uVar5 != uVar3) && (uVar6 = *param_3, uVar6 < param_3[1])) &&
       (uVar10 = (uVar3 - uVar5) - uVar6, 3 < uVar10)) {
      if (uVar3 - uVar5 <= uVar6) goto LAB_0137239d;
      if ((undefined4 *)(uVar5 + uVar6) == (undefined4 *)0x0) {
        memset(&uStack_14,0,uVar10);
        piVar7 = _errno();
        *piVar7 = 0x16;
        _invalid_parameter_noinfo();
        uVar5 = param_3[2];
        uStack_24 = uStack_14;
      }
      else {
        uStack_24 = *(undefined4 *)(uVar5 + uVar6);
      }
      *param_3 = *param_3 + 4;
    }
    uVar3 = param_3[3];
    if (((uVar5 != uVar3) && (uVar6 = *param_3, uVar6 < param_3[1])) &&
       (uVar10 = (uVar3 - uVar5) - uVar6, 3 < uVar10)) {
      if (uVar3 - uVar5 <= uVar6) goto LAB_0137239d;
      if ((undefined4 *)(uVar5 + uVar6) == (undefined4 *)0x0) {
        memset(&uStack_10,0,uVar10);
        piVar7 = _errno();
        *piVar7 = 0x16;
        _invalid_parameter_noinfo();
        uStack_28 = uStack_10;
      }
      else {
        uStack_28 = *(undefined4 *)(uVar5 + uVar6);
      }
      *param_3 = *param_3 + 4;
    }
    uVar3 = param_3[2];
    if ((uVar3 != param_3[3]) && (uVar5 = *param_3, uVar5 < param_3[1])) {
      uVar6 = param_3[3] - uVar3;
      sVar9 = uVar6 - uVar5;
      if (sVar9 != 0) {
        if (uVar6 <= uVar5) goto LAB_0137239d;
        if ((undefined1 *)(uVar3 + uVar5) == (undefined1 *)0x0) {
          memset(&uStack_1c,0,sVar9);
          piVar7 = _errno();
          *piVar7 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          uStack_1c = CONCAT31(uStack_1c._1_3_,*(undefined1 *)(uVar3 + uVar5));
        }
        *param_3 = *param_3 + 1;
      }
    }
    FUN_013740f0(uStack_24,uStack_28,uStack_1c);
    return;
  case 3:
    FUN_0137a3e0();
    if (*(int **)(param_1 + 0x18) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x18) + 0x40))(1);
    }
    if (*(int **)(param_1 + 0x20) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x20) + 0x40))(1);
      return;
    }
    break;
  case 4:
    uVar3 = param_3[2];
    cStack_1d = '\x01';
    if ((uVar3 == param_3[3]) || (uVar5 = *param_3, param_3[1] <= uVar5)) {
LAB_0137232d:
      iVar2 = *(int *)(param_1 + 0x18);
      if (iVar2 != 0) {
        *(undefined4 *)(iVar2 + 0x18) = 10;
        *(undefined4 *)(iVar2 + 0x1c) = 0;
        *(undefined4 *)(iVar2 + 0x20) = 1;
        *(undefined4 *)(iVar2 + 0x24) = 0;
        *(undefined1 *)(param_1 + 0x1c) = 0;
      }
    }
    else {
      uVar6 = param_3[3] - uVar3;
      sVar9 = uVar6 - uVar5;
      if (sVar9 == 0) goto LAB_0137232d;
      if (uVar6 <= uVar5) goto LAB_0137239d;
      if ((char *)(uVar5 + uVar3) == (char *)0x0) {
        memset(&cStack_1d,0,sVar9);
        piVar7 = _errno();
        *piVar7 = 0x16;
        _invalid_parameter_noinfo();
        cVar4 = cStack_1d;
      }
      else {
        cVar4 = *(char *)(uVar5 + uVar3);
      }
      *param_3 = *param_3 + 1;
      if (cVar4 == '\x01') goto LAB_0137232d;
    }
    *(undefined4 *)(param_1 + 0x38) = 0;
    if (*(int *)(param_1 + 0xc) != 0) {
      FUN_01242bf0();
      return;
    }
    break;
  case 5:
    FUN_01375f10();
    break;
  case 6:
    piVar7 = *(int **)(param_1 + 0x88);
    if (piVar7 != (int *)0x0) {
      if (*(char *)((int)piVar7 + 0x2d) != '\0') {
        (**(code **)(*piVar7 + 0x1c))(1);
      }
      iVar2 = *(int *)(param_1 + 0x88);
      *(undefined1 *)(iVar2 + 0x78) = 1;
      *(undefined1 *)(iVar2 + 0x2d) = 1;
      *(undefined1 *)(iVar2 + 0x88) = 1;
      *(undefined4 *)(iVar2 + 0x6c) = 0;
      *(undefined4 *)(iVar2 + 0x68) = *(undefined4 *)(iVar2 + 100);
    }
    iVar2 = *(int *)(param_1 + 0x90);
    if (iVar2 != 0) {
      *(undefined1 *)(iVar2 + 0x78) = 1;
      *(undefined1 *)(iVar2 + 0x2d) = 1;
      *(undefined1 *)(iVar2 + 0x88) = 1;
      *(undefined4 *)(iVar2 + 0x6c) = 0;
      *(undefined4 *)(iVar2 + 0x68) = *(undefined4 *)(iVar2 + 100);
    }
    iVar2 = *(int *)(param_1 + 0x8c);
    if (iVar2 != 0) {
      *(undefined1 *)(iVar2 + 0x78) = 1;
      *(undefined1 *)(iVar2 + 0x2d) = 1;
      *(undefined1 *)(iVar2 + 0x88) = 1;
      *(undefined4 *)(iVar2 + 0x6c) = 0;
      *(undefined4 *)(iVar2 + 0x68) = *(undefined4 *)(iVar2 + 100);
      return;
    }
  }
  return;
}


