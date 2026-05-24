
/* ============================================================ */
/* Function: FUN_01682430 */
/* Entry: 01682430 */
/* Signature: undefined FUN_01682430() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_01682430(int *param_1,int *param_2,uint *param_3)

{
  uint uVar1;
  undefined4 *puVar2;
  bool bVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  char cVar6;
  uint uVar7;
  uint uVar8;
  uint _Size;
  undefined4 *puVar9;
  int *piVar10;
  undefined4 uVar11;
  undefined4 *puVar12;
  int iVar13;
  uint local_70;
  undefined4 *local_5c;
  int local_58;
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 local_28;
  int local_24;
  undefined1 local_20;
  int local_1c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019d354b;
  local_10 = ExceptionList;
  uVar7 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar13 = *param_2;
  uVar8 = (param_2[1] - *param_3) - iVar13;
  local_18 = 0;
  if (uVar8 < 4) {
LAB_016826ba:
    uVar11 = 0;
  }
  else {
    local_14 = uVar7;
    if ((uint)(param_2[1] - iVar13) <= *param_3) {
LAB_016826db:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar7);
    }
    if ((uint *)(*param_3 + iVar13) == (uint *)0x0) {
      memset(&local_18,0,uVar8);
      piVar10 = _errno();
      *piVar10 = 0x16;
      _invalid_parameter_noinfo();
      uVar8 = local_18;
    }
    else {
      uVar8 = *(uint *)(*param_3 + iVar13);
    }
    *param_3 = *param_3 + 4;
    local_70 = 0;
    if (uVar8 != 0) {
      do {
        local_1c = 0;
        memset(&local_5c,0,0x40);
        local_5c = &DAT_01a1dc20;
        uStack_50 = 0;
        uStack_4c = 0;
        uStack_48 = 0;
        local_44 = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_30 = 0;
        local_40 = 0xf;
        local_54 = 0;
        local_2c = 0;
        local_28 = 0xf;
        local_3c = 0;
        uVar1 = *param_3;
        local_8 = 0;
        iVar13 = *param_2;
        _Size = (param_2[1] - uVar1) - iVar13;
        if (_Size < 4) {
LAB_016826b2:
          FUN_016827f0();
          goto LAB_016826ba;
        }
        if ((uint)(param_2[1] - iVar13) <= uVar1) goto LAB_016826db;
        if ((int *)(uVar1 + iVar13) == (int *)0x0) {
          memset(&local_1c,0,_Size);
          piVar10 = _errno();
          *piVar10 = 0x16;
          _invalid_parameter_noinfo();
          iVar13 = local_1c;
        }
        else {
          iVar13 = *(int *)(uVar1 + iVar13);
        }
        *param_3 = *param_3 + 4;
        if ((code *)*local_5c == FUN_01682ad0) {
          cVar6 = FUN_01682ad0(param_2,param_3);
        }
        else {
          cVar6 = (*(code *)*local_5c)(param_2,param_3);
        }
        if (cVar6 == '\0') goto LAB_016826b2;
        bVar3 = false;
        puVar2 = (undefined4 *)*param_1;
        puVar9 = (undefined4 *)puVar2[1];
        cVar6 = *(char *)((int)puVar9 + 0xd);
        puVar12 = puVar2;
        puVar5 = puVar9;
        while (puVar4 = puVar9, cVar6 == '\0') {
          bVar3 = iVar13 <= (int)puVar4[4];
          if (bVar3) {
            puVar9 = (undefined4 *)*puVar4;
            puVar12 = puVar4;
          }
          else {
            puVar9 = (undefined4 *)puVar4[2];
          }
          cVar6 = *(char *)((int)puVar9 + 0xd);
          puVar5 = puVar4;
        }
        if ((*(char *)((int)puVar12 + 0xd) != '\0') || (iVar13 < (int)puVar12[4])) {
          if (param_1[1] == 0x30c30c3) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          local_8._0_1_ = 1;
          piVar10 = operator_new(0x54);
          piVar10[4] = iVar13;
          local_8._0_1_ = 2;
          piVar10[5] = (int)&DAT_01a1dc20;
          piVar10[6] = local_58;
          FUN_00f1a6c0(&local_54);
          local_8 = CONCAT31(local_8._1_3_,3);
          FUN_00f1a6c0(&local_3c);
          piVar10[0x13] = local_24;
          *(undefined1 *)(piVar10 + 0x14) = local_20;
          *piVar10 = (int)puVar2;
          piVar10[1] = (int)puVar2;
          piVar10[2] = (int)puVar2;
          *(undefined2 *)(piVar10 + 3) = 0;
          FUN_015b0610(puVar5,bVar3,piVar10);
        }
        local_8 = 0xffffffff;
        FUN_016827f0();
        local_70 = local_70 + 1;
      } while (local_70 < uVar8);
    }
    uVar11 = 1;
  }
  ExceptionList = local_10;
  return uVar11;
}


