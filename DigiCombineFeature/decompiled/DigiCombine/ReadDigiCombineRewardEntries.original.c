
/* ============================================================ */
/* Function: FUN_0162ee00 */
/* Entry: 0162ee00 */
/* Signature: undefined FUN_0162ee00() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_0162ee00(int *param_1,int *param_2,uint *param_3)

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
  undefined4 uVar10;
  uint uVar11;
  uint _Size;
  undefined4 *puVar12;
  int *piVar13;
  int iVar14;
  bool bVar15;
  uint local_34;
  int local_28;
  uint local_24;
  undefined4 *local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019caa9d;
  local_10 = ExceptionList;
  uVar6 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  uVar7 = param_2[1] - *param_2;
  uVar8 = *param_3;
  uVar11 = uVar7 - uVar8;
  local_24 = 0;
  if (uVar11 < 4) {
LAB_0162f017:
    uVar10 = 0;
  }
  else {
    local_14 = uVar6;
    if (uVar7 <= uVar8) {
LAB_0162f035:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar6);
    }
    puVar1 = (uint *)(*param_2 + uVar8);
    if (puVar1 == (uint *)0x0) {
      memset(&local_24,0,uVar11);
      piVar13 = _errno();
      *piVar13 = 0x16;
      _invalid_parameter_noinfo();
      uVar8 = local_24;
    }
    else {
      uVar8 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    local_34 = 0;
    if (uVar8 != 0) {
      do {
        uVar7 = *param_3;
        local_1c = 0;
        local_28 = 0;
        local_18 = 0;
        local_20 = &DAT_01a1a670;
        uVar11 = param_2[1] - *param_2;
        _Size = uVar11 - uVar7;
        if (_Size < 4) goto LAB_0162f017;
        if (uVar11 <= uVar7) goto LAB_0162f035;
        piVar13 = (int *)(*param_2 + uVar7);
        if (piVar13 == (int *)0x0) {
          memset(&local_28,0,_Size);
          piVar13 = _errno();
          *piVar13 = 0x16;
          _invalid_parameter_noinfo();
          iVar14 = local_28;
        }
        else {
          iVar14 = *piVar13;
        }
        *param_3 = *param_3 + 4;
        if ((code *)*local_20 == FUN_01630370) {
          cVar5 = FUN_01630370(param_2,param_3);
        }
        else {
          cVar5 = (*(code *)*local_20)(param_2,param_3);
        }
        if (cVar5 == '\0') goto LAB_0162f017;
        bVar15 = false;
        puVar2 = (undefined4 *)*param_1;
        puVar9 = (undefined4 *)puVar2[1];
        cVar5 = *(char *)((int)puVar9 + 0xd);
        puVar12 = puVar2;
        puVar4 = puVar9;
        while (puVar3 = puVar9, cVar5 == '\0') {
          bVar15 = iVar14 <= (int)puVar3[4];
          if (bVar15) {
            puVar9 = (undefined4 *)*puVar3;
            puVar12 = puVar3;
          }
          else {
            puVar9 = (undefined4 *)puVar3[2];
          }
          cVar5 = *(char *)((int)puVar9 + 0xd);
          puVar4 = puVar3;
        }
        if ((*(char *)((int)puVar12 + 0xd) != '\0') || (iVar14 < (int)puVar12[4])) {
          if (param_1[1] == 0x7ffffff) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          local_8 = 0;
          piVar13 = operator_new(0x20);
          local_8 = 0xffffffff;
          piVar13[4] = iVar14;
          piVar13[5] = (int)&DAT_01a1a670;
          *(undefined2 *)(piVar13 + 6) = (undefined2)local_1c;
          *(undefined2 *)((int)piVar13 + 0x1a) = local_1c._2_2_;
          *(undefined1 *)(piVar13 + 7) = (undefined1)local_18;
          *(undefined1 *)((int)piVar13 + 0x1d) = local_18._1_1_;
          *(undefined1 *)((int)piVar13 + 0x1e) = local_18._2_1_;
          *piVar13 = (int)puVar2;
          piVar13[1] = (int)puVar2;
          piVar13[2] = (int)puVar2;
          *(undefined2 *)(piVar13 + 3) = 0;
          FUN_015b0610(puVar4,bVar15,piVar13);
        }
        local_34 = local_34 + 1;
      } while (local_34 < uVar8);
    }
    uVar10 = 1;
  }
  ExceptionList = local_10;
  return uVar10;
}


