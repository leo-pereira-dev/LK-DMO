
/* ============================================================ */
/* Function: FUN_0125df00 */
/* Entry: 0125df00 */
/* Signature: undefined FUN_0125df00() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_0125df00(int param_1,undefined4 *param_2,uint *param_3)

{
  int iVar1;
  code *pcVar2;
  char cVar3;
  byte bVar4;
  uint uVar5;
  uint uVar6;
  undefined4 *puVar7;
  uint uVar8;
  uint uVar9;
  int *piVar10;
  size_t _Size;
  int iVar11;
  float10 fVar12;
  undefined1 auStack_44 [24];
  int iStack_2c;
  int iStack_28;
  int iStack_24;
  int iStack_20;
  int iStack_1c;
  char cStack_15;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 uStack_8;
  
  uStack_8 = 0xffffffff;
  puStack_c = &LAB_0199aeed;
  local_10 = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar5;
  switch(*param_2) {
  case 0:
    iVar11 = *(int *)(param_1 + 0xc);
    if (iVar11 != 0) {
      *(undefined1 *)(iVar11 + 0xe4) = 1;
      *(undefined4 *)(iVar11 + 0xe8) = 0;
    }
    break;
  case 1:
    iVar11 = 0;
    iStack_1c = 0;
    if ((param_3[2] != param_3[3]) && (uVar5 = *param_3, uVar5 < param_3[1])) {
      uVar6 = param_3[3] - param_3[2];
      uVar8 = uVar6 - uVar5;
      if (3 < uVar8) {
        if (uVar6 <= uVar5) goto LAB_0125e415;
        if ((int *)(param_3[2] + uVar5) == (int *)0x0) {
          memset(&iStack_1c,0,uVar8);
          piVar10 = _errno();
          *piVar10 = 0x16;
          _invalid_parameter_noinfo();
          iVar11 = iStack_1c;
        }
        else {
          iVar11 = *(int *)(param_3[2] + uVar5);
        }
        *param_3 = *param_3 + 4;
      }
    }
    if (DAT_01bfb32c != (int *)0x0) {
      cVar3 = (**(code **)(*DAT_01bfb32c + 4))(s_System_interface_person_01_wav_01ae83c0);
      if (cVar3 != '\0') {
        (**(code **)(*DAT_01bfb32c + 8))(s_System_interface_person_01_wav_01ae83c0);
      }
      (**(code **)*DAT_01bfb32c)(s_System_interface_person_01_wav_01ae83c0);
    }
    iVar1 = *(int *)(param_1 + 0x20);
    if (iVar1 != 0) {
      *(undefined4 *)(iVar1 + 0x18) = 10;
      *(int *)(iVar1 + 0x1c) = iVar11;
      *(undefined4 *)(iVar1 + 0x20) = 1;
      *(undefined4 *)(iVar1 + 0x24) = 0;
      *(bool *)(param_1 + 0x24) = iVar11 != 0;
    }
    if (*(int **)(param_1 + 0x3c) != (int *)0x0) {
      pcVar2 = *(code **)(**(int **)(param_1 + 0x3c) + 0x10);
      puVar7 = (undefined4 *)FUN_0137ac80(auStack_44);
      uStack_8 = 0;
      if (7 < (uint)puVar7[5]) {
        puVar7 = (undefined4 *)*puVar7;
      }
      (*pcVar2)(puVar7);
      uStack_8 = 0xffffffff;
      FUN_00f1a330();
    }
    iVar11 = *(int *)(param_1 + 4);
    if ((((*(int *)(iVar11 + 0xb8) != 0) || (*(short *)(iVar11 + 0xbc) != 0)) &&
        (*(int *)(iVar11 + 0x14c) != 0x7fffffff)) && (*(int *)(iVar11 + 0x144) == 4)) {
      FUN_0125fda0();
    }
    piVar10 = *(int **)(param_1 + 0x2c);
    if (piVar10 == (int *)0x0) {
      ExceptionList = local_10;
      return;
    }
    bVar4 = *(byte *)(param_1 + 0x24) & *(byte *)(param_1 + 0x1c);
    goto LAB_0125e3ee;
  case 2:
    uVar5 = param_3[2];
    iStack_2c = 0;
    iStack_24 = 0;
    iStack_28 = 0;
    iStack_20 = 0;
    if ((uVar5 != param_3[3]) && (uVar6 = *param_3, uVar6 < param_3[1])) {
      uVar8 = param_3[3] - uVar5;
      uVar9 = uVar8 - uVar6;
      if (3 < uVar9) {
        if (uVar8 <= uVar6) goto LAB_0125e415;
        if ((int *)(uVar6 + uVar5) == (int *)0x0) {
          memset(&iStack_20,0,uVar9);
          piVar10 = _errno();
          *piVar10 = 0x16;
          _invalid_parameter_noinfo();
          iStack_28 = iStack_20;
        }
        else {
          iStack_28 = *(int *)(uVar6 + uVar5);
        }
        *param_3 = *param_3 + 4;
      }
    }
    uVar5 = param_3[2];
    uVar6 = param_3[3];
    if (((uVar5 != uVar6) && (uVar8 = *param_3, uVar8 < param_3[1])) &&
       (uVar9 = (uVar6 - uVar8) - uVar5, 3 < uVar9)) {
      if (uVar6 - uVar5 <= uVar8) goto LAB_0125e415;
      if ((int *)(uVar8 + uVar5) == (int *)0x0) {
        memset(&iStack_24,0,uVar9);
        piVar10 = _errno();
        *piVar10 = 0x16;
        _invalid_parameter_noinfo();
        iStack_2c = iStack_24;
      }
      else {
        iStack_2c = *(int *)(uVar8 + uVar5);
      }
      *param_3 = *param_3 + 4;
    }
    uVar5 = param_3[2];
    if (((uVar5 != param_3[3]) && (uVar6 = *param_3, uVar6 < param_3[1])) &&
       (uVar8 = param_3[3] - uVar5, uVar8 != uVar6)) {
      if (uVar8 <= uVar6) {
LAB_0125e415:
                    /* WARNING: Subroutine does not return */
        FUN_00f3fcb0();
      }
      if (uVar6 + uVar5 == 0) {
        piVar10 = _errno();
        *piVar10 = 0x16;
        _invalid_parameter_noinfo();
      }
      *param_3 = *param_3 + 1;
    }
    if (DAT_01bfb32c != (int *)0x0) {
      cVar3 = (**(code **)(*DAT_01bfb32c + 4))(s_System_interface_person_01_wav_01ae83c0);
      if (cVar3 != '\0') {
        (**(code **)(*DAT_01bfb32c + 8))(s_System_interface_person_01_wav_01ae83c0);
      }
      (**(code **)*DAT_01bfb32c)(s_System_interface_person_01_wav_01ae83c0);
    }
    iVar11 = *(int *)(param_1 + 0x18);
    if (iVar11 != 0) {
      *(undefined4 *)(iVar11 + 0x18) = 10;
      *(int *)(iVar11 + 0x1c) = iStack_28;
      *(undefined4 *)(iVar11 + 0x20) = 1;
      *(undefined4 *)(iVar11 + 0x24) = 0;
      *(bool *)(param_1 + 0x1c) = iStack_28 != 0;
    }
    iVar11 = *(int *)(param_1 + 0x20);
    if (iVar11 != 0) {
      *(undefined4 *)(iVar11 + 0x18) = 10;
      *(undefined4 *)(iVar11 + 0x1c) = 0;
      *(undefined4 *)(iVar11 + 0x20) = 1;
      *(undefined4 *)(iVar11 + 0x24) = 0;
      *(undefined1 *)(param_1 + 0x24) = 0;
    }
    FUN_0137ab70();
    piVar10 = *(int **)(param_1 + 0xc);
    if (piVar10 != (int *)0x0) {
      piVar10[0x3b] = 1;
      piVar10[0x3c] = 0;
      fVar12 = (float10)fmaxf(0,0x3fd56042);
      piVar10[0x32] = (int)(float)fVar12;
      FUN_01243010();
      (**(code **)(*piVar10 + 0x48))();
      FUN_01242bf0();
      *(undefined1 *)(*(int *)(param_1 + 0xc) + 0xe4) = 0;
    }
    if (*(int **)(param_1 + 0x2c) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x2c) + 0x18))
                (*(byte *)(param_1 + 0x24) & *(byte *)(param_1 + 0x1c));
    }
    if (*(char *)(param_1 + 0x24) != '\0') {
      if (iStack_2c == 4) {
        FUN_0125fda0();
      }
      else if (*(int **)(param_1 + 0x28) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x28) + 0x1c))(0);
      }
    }
    break;
  case 3:
    FUN_0125f6e0();
    break;
  case 4:
    uVar6 = param_3[2];
    cStack_15 = '\x01';
    if ((uVar6 == param_3[3]) || (uVar8 = *param_3, param_3[1] <= uVar8)) {
LAB_0125e396:
      iVar11 = *(int *)(param_1 + 0x18);
      if (iVar11 != 0) {
        *(undefined4 *)(iVar11 + 0x18) = 10;
        *(undefined4 *)(iVar11 + 0x1c) = 0;
        *(undefined4 *)(iVar11 + 0x20) = 1;
        *(undefined4 *)(iVar11 + 0x24) = 0;
        *(undefined1 *)(param_1 + 0x1c) = 0;
      }
      if (*(int **)(param_1 + 0x28) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x28) + 0x1c))(0,uVar5);
      }
    }
    else {
      uVar9 = param_3[3] - uVar6;
      _Size = uVar9 - uVar8;
      if (_Size == 0) goto LAB_0125e396;
      if (uVar9 <= uVar8) goto LAB_0125e415;
      if ((char *)(uVar8 + uVar6) == (char *)0x0) {
        memset(&cStack_15,0,_Size);
        piVar10 = _errno();
        *piVar10 = 0x16;
        _invalid_parameter_noinfo();
        cVar3 = cStack_15;
      }
      else {
        cVar3 = *(char *)(uVar8 + uVar6);
      }
      *param_3 = *param_3 + 1;
      if (cVar3 == '\x01') goto LAB_0125e396;
    }
    *(undefined4 *)(param_1 + 0x38) = 0;
    if (*(int *)(param_1 + 0xc) != 0) {
      FUN_01242bf0();
    }
    piVar10 = *(int **)(param_1 + 0x2c);
    if (piVar10 == (int *)0x0) {
      ExceptionList = local_10;
      return;
    }
    bVar4 = *(byte *)(param_1 + 0x24) & *(byte *)(param_1 + 0x1c);
LAB_0125e3ee:
    (**(code **)(*piVar10 + 0x18))(bVar4);
  }
  ExceptionList = local_10;
  return;
}


