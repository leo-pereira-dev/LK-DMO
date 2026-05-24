
/* ============================================================ */
/* Function: FUN_015eb300 */
/* Entry: 015eb300 */
/* Signature: undefined FUN_015eb300() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_015eb300(int *param_1,int *param_2,uint *param_3)

{
  uint *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  char cVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined4 *puVar9;
  int *piVar10;
  undefined4 uVar11;
  uint uVar12;
  uint _Size;
  undefined4 *puVar13;
  int iVar14;
  bool bVar15;
  uint local_48;
  int local_1c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c8fbd;
  local_10 = ExceptionList;
  uVar6 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  uVar7 = param_2[1] - *param_2;
  uVar8 = *param_3;
  uVar12 = uVar7 - uVar8;
  local_18 = 0;
  if (uVar12 < 4) {
LAB_015eb515:
    uVar11 = 0;
  }
  else {
    local_14 = uVar6;
    if (uVar7 <= uVar8) {
LAB_015eb538:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar6);
    }
    puVar1 = (uint *)(*param_2 + uVar8);
    if (puVar1 == (uint *)0x0) {
      memset(&local_18,0,uVar12);
      piVar10 = _errno();
      *piVar10 = 0x16;
      _invalid_parameter_noinfo();
      uVar8 = local_18;
    }
    else {
      uVar8 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    local_48 = 0;
    if (uVar8 != 0) {
      do {
        uVar12 = param_2[1] - *param_2;
        uVar7 = *param_3;
        _Size = uVar12 - uVar7;
        local_1c = 0;
        if (_Size < 4) goto LAB_015eb515;
        if (uVar12 <= uVar7) goto LAB_015eb538;
        piVar10 = (int *)(*param_2 + uVar7);
        if (piVar10 == (int *)0x0) {
          memset(&local_1c,0,_Size);
          piVar10 = _errno();
          *piVar10 = 0x16;
          _invalid_parameter_noinfo();
          iVar14 = local_1c;
        }
        else {
          iVar14 = *piVar10;
        }
        *param_3 = *param_3 + 4;
        cVar5 = FUN_015ed170(param_2,param_3);
        if (cVar5 == '\0') goto LAB_015eb515;
        bVar15 = false;
        puVar2 = (undefined4 *)*param_1;
        puVar9 = (undefined4 *)puVar2[1];
        cVar5 = *(char *)((int)puVar9 + 0xd);
        puVar13 = puVar2;
        puVar4 = puVar9;
        while (puVar3 = puVar9, cVar5 == '\0') {
          bVar15 = iVar14 <= (int)puVar3[4];
          if (bVar15) {
            puVar9 = (undefined4 *)*puVar3;
            puVar13 = puVar3;
          }
          else {
            puVar9 = (undefined4 *)puVar3[2];
          }
          cVar5 = *(char *)((int)puVar9 + 0xd);
          puVar4 = puVar3;
        }
        if ((*(char *)((int)puVar13 + 0xd) != '\0') || (iVar14 < (int)puVar13[4])) {
          if (param_1[1] == 0x4924924) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          local_8 = 0;
          piVar10 = operator_new(0x38);
          local_8 = 0xffffffff;
          piVar10[8] = 0;
          piVar10[9] = 0;
          piVar10[10] = 0;
          piVar10[0xb] = 0;
          piVar10[0xc] = 0;
          piVar10[6] = 0;
          piVar10[0xd] = 0;
          piVar10[7] = 0;
          piVar10[4] = iVar14;
          piVar10[5] = (int)&DAT_01a18e88;
          *piVar10 = (int)puVar2;
          piVar10[1] = (int)puVar2;
          piVar10[2] = (int)puVar2;
          *(undefined2 *)(piVar10 + 3) = 0;
          FUN_015b0610(puVar4,bVar15,piVar10);
        }
        local_48 = local_48 + 1;
      } while (local_48 < uVar8);
    }
    uVar11 = 1;
  }
  ExceptionList = local_10;
  return uVar11;
}


