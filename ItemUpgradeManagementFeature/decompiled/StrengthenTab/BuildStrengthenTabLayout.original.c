
/* ============================================================ */
/* Function: FUN_01272360 */
/* Entry: 01272360 */
/* Signature: undefined FUN_01272360() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01272360(int param_1,undefined4 *param_2,uint *param_3)

{
  uint uVar1;
  uint uVar2;
  int *piVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  int iStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 uStack_8;
  
  uStack_8 = 0xffffffff;
  puStack_c = &LAB_0199be95;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar1;
  switch(*param_2) {
  case 0x14:
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_44 = 0;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_24 = 0;
    uStack_38 = 0;
    uStack_34 = 7;
    uStack_48 = 0;
    uStack_8 = 1;
    if ((param_3[2] != param_3[3]) && (uVar2 = *param_3, uVar2 < param_3[1])) {
      uVar5 = param_3[3] - param_3[2];
      uVar6 = uVar5 - uVar2;
      if (3 < uVar6) {
        if (uVar5 <= uVar2) goto LAB_01272803;
        if ((undefined4 *)(param_3[2] + uVar2) == (undefined4 *)0x0) {
          memset(&uStack_2c,0,uVar6);
          piVar3 = _errno();
          *piVar3 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          uStack_2c = *(undefined4 *)(param_3[2] + uVar2);
        }
        *param_3 = *param_3 + 4;
      }
    }
    uVar2 = param_3[3];
    if (((param_3[2] != uVar2) && (uVar5 = *param_3, uVar5 < param_3[1])) &&
       (uVar6 = (uVar2 - uVar5) - param_3[2], 3 < uVar6)) {
      if (uVar2 - param_3[2] <= uVar5) goto LAB_01272803;
      if ((undefined4 *)(param_3[2] + uVar5) == (undefined4 *)0x0) {
        memset(&uStack_24,0,uVar6);
        piVar3 = _errno();
        *piVar3 = 0x16;
        _invalid_parameter_noinfo();
      }
      else {
        uStack_24 = *(undefined4 *)(param_3[2] + uVar5);
      }
      *param_3 = *param_3 + 4;
    }
    if ((param_3[2] != param_3[3]) && (*param_3 < param_3[1])) {
      FUN_0100f610(&uStack_48,param_3 + 2,param_3,uVar1);
    }
    if ((param_3[2] != param_3[3]) && (uVar1 = *param_3, uVar1 < param_3[1])) {
      uVar2 = param_3[3] - param_3[2];
      uVar5 = uVar2 - uVar1;
      if (3 < uVar5) {
        if (uVar2 <= uVar1) goto LAB_01272803;
        if ((undefined4 *)(param_3[2] + uVar1) == (undefined4 *)0x0) {
          memset(&uStack_28,0,uVar5);
          piVar3 = _errno();
          *piVar3 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          uStack_28 = *(undefined4 *)(param_3[2] + uVar1);
        }
        *param_3 = *param_3 + 4;
      }
    }
    uVar1 = param_3[3];
    if (((param_3[2] != uVar1) && (uVar2 = *param_3, uVar2 < param_3[1])) &&
       (uVar5 = (uVar1 - uVar2) - param_3[2], 3 < uVar5)) {
      if (uVar1 - param_3[2] <= uVar2) {
LAB_01272803:
                    /* WARNING: Subroutine does not return */
        FUN_00f3fcb0();
      }
      if ((int *)(param_3[2] + uVar2) == (int *)0x0) {
        memset(&iStack_30,0,uVar5);
        piVar3 = _errno();
        *piVar3 = 0x16;
        _invalid_parameter_noinfo();
        *param_3 = *param_3 + 4;
      }
      else {
        iStack_30 = *(int *)(param_3[2] + uVar2);
        *param_3 = *param_3 + 4;
      }
    }
    FUN_01277870(iStack_30);
    FUN_012741c0();
    if (iStack_30 == 2) {
      FUN_01276eb0();
      FUN_00f1a330();
    }
    else {
      FUN_012742c0(&uStack_2c,&uStack_28,&uStack_24,&uStack_48);
      FUN_00f1a330();
    }
    break;
  case 0x16:
    FUN_01277140();
    break;
  case 0x17:
    *(undefined1 *)(param_1 + 0xe8) = 0;
    FUN_01277a60(1);
    *(undefined4 *)(param_1 + 0x11c) = 0xffffffff;
    *(undefined1 *)(param_1 + 0xe9) = 0;
    *(undefined4 *)(param_1 + 0xc) = 0;
    *(undefined4 *)(param_1 + 8) = 0;
    FUN_012741c0();
    FUN_01276eb0();
    if (*(int **)(param_1 + 0x104) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x104) + 0x1c))(0);
    }
    if (*(int **)(param_1 + 0x108) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x108) + 0x1c))(0);
    }
    if (*(int **)(param_1 + 0x10c) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x10c) + 0x1c))(0);
    }
    if (*(int **)(param_1 + 0x110) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x110) + 0x1c))(0);
    }
    break;
  case 0x18:
    *(undefined1 *)(param_1 + 0xe8) = 0;
    FUN_01277a60(1);
    break;
  case 0x19:
    uStack_20 = 0;
    uStack_1c = 0;
    uStack_44 = 0;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_18 = 0;
    uStack_38 = 0;
    uStack_34 = 7;
    uStack_48 = 0;
    uStack_8 = 0;
    uVar2 = param_3[2];
    if ((uVar2 != param_3[3]) && (uVar5 = *param_3, uVar5 < param_3[1])) {
      uVar4 = param_3[3] - uVar2;
      uVar6 = uVar4 - uVar5;
      if (3 < uVar6) {
        if (uVar4 <= uVar5) goto LAB_012727fe;
        if ((undefined4 *)(uVar5 + uVar2) == (undefined4 *)0x0) {
          memset(&uStack_20,0,uVar6);
          piVar3 = _errno();
          *piVar3 = 0x16;
          _invalid_parameter_noinfo();
          uVar2 = param_3[2];
        }
        else {
          uStack_20 = *(undefined4 *)(uVar5 + uVar2);
        }
        *param_3 = *param_3 + 4;
      }
    }
    if ((uVar2 != param_3[3]) && (uVar5 = *param_3, uVar5 < param_3[1])) {
      uVar4 = param_3[3] - uVar2;
      uVar6 = uVar4 - uVar5;
      if (3 < uVar6) {
        if (uVar4 <= uVar5) goto LAB_012727fe;
        if ((undefined4 *)(uVar2 + uVar5) == (undefined4 *)0x0) {
          memset(&uStack_18,0,uVar6);
          piVar3 = _errno();
          *piVar3 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          uStack_18 = *(undefined4 *)(uVar2 + uVar5);
        }
        *param_3 = *param_3 + 4;
      }
    }
    if ((param_3[2] != param_3[3]) && (*param_3 < param_3[1])) {
      FUN_0100f610(&uStack_48,param_3 + 2,param_3,uVar1);
    }
    if ((param_3[2] != param_3[3]) && (uVar1 = *param_3, uVar1 < param_3[1])) {
      uVar2 = param_3[3] - param_3[2];
      uVar5 = uVar2 - uVar1;
      if (3 < uVar5) {
        if (uVar2 <= uVar1) {
LAB_012727fe:
                    /* WARNING: Subroutine does not return */
          FUN_00f3fcb0();
        }
        if ((undefined4 *)(param_3[2] + uVar1) == (undefined4 *)0x0) {
          memset(&uStack_1c,0,uVar5);
          piVar3 = _errno();
          *piVar3 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          uStack_1c = *(undefined4 *)(param_3[2] + uVar1);
        }
        *param_3 = *param_3 + 4;
      }
    }
    FUN_012742c0(&uStack_20,&uStack_1c,&uStack_18,&uStack_48);
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


