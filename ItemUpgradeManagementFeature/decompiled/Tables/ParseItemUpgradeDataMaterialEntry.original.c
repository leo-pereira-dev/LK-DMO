
/* ============================================================ */
/* Function: FUN_015ecd40 */
/* Entry: 015ecd40 */
/* Signature: undefined FUN_015ecd40() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 __thiscall FUN_015ecd40(int param_1,int *param_2,uint *param_3)

{
  int iVar1;
  code *pcVar2;
  char cVar3;
  uint uVar4;
  undefined4 *puVar5;
  uint uVar6;
  int *piVar7;
  undefined4 *puVar8;
  uint uVar9;
  uint uVar10;
  code *pcVar11;
  code *pcVar12;
  uint local_2c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  pcVar11 = _invalid_parameter_noinfo_exref;
  pcVar12 = _errno_exref;
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c88ad;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  puVar8 = (undefined4 *)(param_1 + 4);
  uVar9 = param_2[1] - *param_2;
  uVar6 = *param_3;
  uVar10 = uVar9 - uVar6;
  local_14 = uVar4;
  if (3 < uVar10) {
    if (uVar9 <= uVar6) goto LAB_015ed160;
    puVar5 = (undefined4 *)(uVar6 + *param_2);
    if (puVar8 == (undefined4 *)0x0) {
LAB_015ecdc9:
      piVar7 = _errno();
      *piVar7 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if (puVar5 == (undefined4 *)0x0) {
        memset(puVar8,0,uVar10);
        goto LAB_015ecdc9;
      }
      *puVar8 = *puVar5;
    }
    *param_3 = *param_3 + 4;
    uVar6 = *param_3;
  }
  pcVar2 = _errno_exref;
  uVar9 = (param_2[1] - *param_2) - uVar6;
  if (3 < uVar9) {
    if ((uint)(param_2[1] - *param_2) <= uVar6) goto LAB_015ed160;
    puVar8 = (undefined4 *)(param_1 + 8);
    if (puVar8 == (undefined4 *)0x0) {
LAB_015ece1c:
      piVar7 = _errno();
      *piVar7 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar6 + *param_2) == (undefined4 *)0x0) {
        memset(puVar8,0,uVar9);
        goto LAB_015ece1c;
      }
      *puVar8 = *(undefined4 *)(uVar6 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar6 = *param_3;
    pcVar12 = pcVar2;
  }
  pcVar2 = _errno_exref;
  uVar9 = (param_2[1] - *param_2) - uVar6;
  if (3 < uVar9) {
    if ((uint)(param_2[1] - *param_2) <= uVar6) goto LAB_015ed160;
    puVar8 = (undefined4 *)(param_1 + 0xc);
    if (puVar8 == (undefined4 *)0x0) {
LAB_015ece6f:
      piVar7 = _errno();
      *piVar7 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar6 + *param_2) == (undefined4 *)0x0) {
        memset(puVar8,0,uVar9);
        goto LAB_015ece6f;
      }
      *puVar8 = *(undefined4 *)(uVar6 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar6 = *param_3;
    pcVar12 = pcVar2;
  }
  pcVar2 = _errno_exref;
  uVar9 = (param_2[1] - *param_2) - uVar6;
  if (3 < uVar9) {
    if ((uint)(param_2[1] - *param_2) <= uVar6) goto LAB_015ed160;
    puVar8 = (undefined4 *)(param_1 + 0x10);
    if (puVar8 == (undefined4 *)0x0) {
LAB_015ecec2:
      piVar7 = _errno();
      *piVar7 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar6 + *param_2) == (undefined4 *)0x0) {
        memset(puVar8,0,uVar9);
        goto LAB_015ecec2;
      }
      *puVar8 = *(undefined4 *)(uVar6 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar6 = *param_3;
    pcVar12 = pcVar2;
  }
  pcVar2 = _errno_exref;
  uVar9 = (param_2[1] - *param_2) - uVar6;
  if (3 < uVar9) {
    if ((uint)(param_2[1] - *param_2) <= uVar6) goto LAB_015ed160;
    puVar8 = (undefined4 *)(param_1 + 0x14);
    if (puVar8 == (undefined4 *)0x0) {
LAB_015ecf15:
      piVar7 = _errno();
      *piVar7 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar6 + *param_2) == (undefined4 *)0x0) {
        memset(puVar8,0,uVar9);
        goto LAB_015ecf15;
      }
      *puVar8 = *(undefined4 *)(uVar6 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar6 = *param_3;
    pcVar12 = pcVar2;
  }
  uVar9 = (param_2[1] - *param_2) - uVar6;
  local_18 = 0;
  if (3 < uVar9) {
    if ((uint)(param_2[1] - *param_2) <= uVar6) goto LAB_015ed160;
    if ((uint *)(uVar6 + *param_2) == (uint *)0x0) {
      memset(&local_18,0,uVar9);
      puVar8 = (undefined4 *)(*pcVar12)();
      *puVar8 = 0x16;
      _invalid_parameter_noinfo();
      uVar6 = local_18;
    }
    else {
      uVar6 = *(uint *)(uVar6 + *param_2);
    }
    *param_3 = *param_3 + 4;
    local_2c = 0;
    if (uVar6 != 0) {
      do {
        cVar3 = FUN_015d7bc0(param_2,param_3);
        if (cVar3 == '\0') break;
        iVar1 = *(int *)(param_1 + 0x18);
        if (*(int *)(param_1 + 0x1c) == 0xaaaaaaa) {
                    /* WARNING: Subroutine does not return */
          std::_Xlength_error(s_list_too_long_01abdaa4);
        }
        local_8 = 0;
        piVar7 = operator_new(0x18);
        local_8 = 0xffffffff;
        piVar7[3] = 0;
        piVar7[4] = 0;
        piVar7[5] = 0;
        piVar7[2] = (int)&DAT_01a18e58;
        *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + 1;
        puVar8 = *(undefined4 **)(iVar1 + 4);
        *piVar7 = iVar1;
        piVar7[1] = (int)puVar8;
        *(int **)(iVar1 + 4) = piVar7;
        *puVar8 = piVar7;
        local_2c = local_2c + 1;
      } while (local_2c < uVar6);
    }
  }
  pcVar2 = _invalid_parameter_noinfo_exref;
  uVar9 = *param_3;
  uVar6 = (param_2[1] - *param_2) - uVar9;
  if (3 < uVar6) {
    if ((uint)(param_2[1] - *param_2) <= uVar9) goto LAB_015ed160;
    puVar8 = (undefined4 *)(param_1 + 0x20);
    if (puVar8 == (undefined4 *)0x0) {
LAB_015ed089:
      puVar8 = (undefined4 *)(*pcVar12)();
      *puVar8 = 0x16;
      _invalid_parameter_noinfo();
    }
    else {
      if ((undefined4 *)(uVar9 + *param_2) == (undefined4 *)0x0) {
        memset(puVar8,0,uVar6);
        goto LAB_015ed089;
      }
      *puVar8 = *(undefined4 *)(uVar9 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar9 = *param_3;
    pcVar11 = pcVar2;
  }
  uVar6 = (param_2[1] - *param_2) - uVar9;
  if (3 < uVar6) {
    if ((uint)(param_2[1] - *param_2) <= uVar9) goto LAB_015ed160;
    puVar8 = (undefined4 *)(param_1 + 0x24);
    if (puVar8 == (undefined4 *)0x0) {
LAB_015ed0dc:
      piVar7 = _errno();
      *piVar7 = 0x16;
      (*pcVar11)();
    }
    else {
      if ((undefined4 *)(uVar9 + *param_2) == (undefined4 *)0x0) {
        memset(puVar8,0,uVar6);
        goto LAB_015ed0dc;
      }
      *puVar8 = *(undefined4 *)(uVar9 + *param_2);
    }
    *param_3 = *param_3 + 4;
    uVar9 = *param_3;
  }
  uVar10 = param_2[1] - *param_2;
  puVar8 = (undefined4 *)(param_1 + 0x28);
  uVar6 = uVar10 - uVar9;
  if (uVar6 < 4) {
    ExceptionList = local_10;
    return 1;
  }
  if (uVar10 <= uVar9) {
LAB_015ed160:
                    /* WARNING: Subroutine does not return */
    FUN_00f3fcb0(uVar4);
  }
  puVar5 = (undefined4 *)(uVar9 + *param_2);
  if (puVar8 != (undefined4 *)0x0) {
    if (puVar5 != (undefined4 *)0x0) {
      *puVar8 = *puVar5;
      goto LAB_015ed13d;
    }
    memset(puVar8,0,uVar6);
  }
  piVar7 = _errno();
  *piVar7 = 0x16;
  (*pcVar11)();
LAB_015ed13d:
  *param_3 = *param_3 + 4;
  ExceptionList = local_10;
  return 1;
}


