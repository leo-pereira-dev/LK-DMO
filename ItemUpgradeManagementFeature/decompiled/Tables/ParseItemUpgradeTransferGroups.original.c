
/* ============================================================ */
/* Function: FUN_015eb710 */
/* Entry: 015eb710 */
/* Signature: undefined FUN_015eb710() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_015eb710(undefined4 *param_1,int *param_2,uint *param_3)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char cVar6;
  uint uVar7;
  uint uVar8;
  void *pvVar9;
  int *piVar10;
  undefined8 *puVar11;
  undefined4 *puVar12;
  undefined4 uVar13;
  uint uVar14;
  undefined1 local_60 [12];
  undefined8 local_54;
  undefined4 *local_48;
  undefined4 *local_44;
  uint *local_40;
  int *local_3c;
  uint local_38;
  uint local_34;
  int local_30;
  undefined4 *local_2c;
  uint local_28;
  int local_24;
  int local_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c904d;
  local_10 = ExceptionList;
  uVar7 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_2c = param_1;
  uVar8 = param_2[1] - *param_2;
  uVar2 = *param_3;
  uVar14 = uVar8 - uVar2;
  local_3c = param_2;
  local_40 = param_3;
  local_28 = 0;
  if (uVar14 < 4) {
LAB_015eb94b:
    uVar13 = 0;
  }
  else {
    local_14 = uVar7;
    if (uVar8 <= uVar2) {
LAB_015eb96d:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar7);
    }
    puVar1 = (uint *)(*param_2 + uVar2);
    if (puVar1 == (uint *)0x0) {
      memset(&local_28,0,uVar14);
      piVar10 = _errno();
      *piVar10 = 0x16;
      _invalid_parameter_noinfo();
      local_38 = local_28;
    }
    else {
      local_38 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    local_34 = 0;
    if (local_38 != 0) {
      do {
        local_24 = 0;
        local_20 = 0;
        local_1c = 0;
        pvVar9 = operator_new(0x38);
        *(void **)pvVar9 = pvVar9;
        *(void **)((int)pvVar9 + 4) = pvVar9;
        *(void **)((int)pvVar9 + 8) = pvVar9;
        *(undefined2 *)((int)pvVar9 + 0xc) = 0x101;
        local_1c = CONCAT44(local_1c._4_4_,pvVar9);
        uVar8 = param_2[1] - *param_2;
        uVar2 = *param_3;
        uVar14 = uVar8 - uVar2;
        local_8 = 0;
        if (uVar14 < 8) {
LAB_015eb940:
          FUN_01271d90(&local_1c);
          goto LAB_015eb94b;
        }
        if (uVar8 <= uVar2) goto LAB_015eb96d;
        piVar10 = (int *)(*param_2 + uVar2);
        if (piVar10 == (int *)0x0) {
          memset(&local_24,0,uVar14);
          piVar10 = _errno();
          *piVar10 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_24 = *piVar10;
          local_20 = piVar10[1];
        }
        iVar5 = local_20;
        iVar4 = local_24;
        *param_3 = *param_3 + 8;
        local_30 = local_20;
        cVar6 = FUN_015eb300(&local_1c,local_3c,param_3);
        if (cVar6 == '\0') goto LAB_015eb940;
        puVar11 = (undefined8 *)FUN_015ea250(local_60,&local_24);
        local_54 = *puVar11;
        iVar3 = *(int *)(puVar11 + 1);
        if (((*(char *)(iVar3 + 0xd) != '\0') || (iVar4 < *(int *)(iVar3 + 0x10))) ||
           ((iVar4 <= *(int *)(iVar3 + 0x10) && (iVar5 < *(int *)(iVar3 + 0x14))))) {
          if (local_2c[1] == 0x7ffffff) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          uVar13 = *local_2c;
          local_48 = local_2c;
          local_8._0_1_ = 1;
          local_44 = (undefined4 *)0x0;
          puVar12 = operator_new(0x20);
          puVar12[5] = local_30;
          local_8 = CONCAT31(local_8._1_3_,2);
          puVar12[4] = iVar4;
          local_44 = puVar12;
          FUN_015ebad0(&local_1c);
          *puVar12 = uVar13;
          puVar12[1] = uVar13;
          puVar12[2] = uVar13;
          *(undefined2 *)(puVar12 + 3) = 0;
          local_44 = (undefined4 *)0x0;
          FUN_015b0610((undefined4)local_54,local_54._4_4_,puVar12);
          param_3 = local_40;
        }
        local_8 = 0xffffffff;
        FUN_01271d90(&local_1c);
        local_34 = local_34 + 1;
        param_2 = local_3c;
      } while (local_34 < local_38);
    }
    uVar13 = 1;
  }
  ExceptionList = local_10;
  return uVar13;
}


