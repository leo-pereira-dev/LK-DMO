
/* ============================================================ */
/* Function: FUN_015eae50 */
/* Entry: 015eae50 */
/* Signature: undefined FUN_015eae50() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_015eae50(undefined4 *param_1,int *param_2,uint *param_3)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int *piVar7;
  int *piVar8;
  undefined8 *puVar9;
  undefined4 *puVar10;
  undefined4 uVar11;
  undefined1 local_5c [12];
  undefined8 local_50;
  undefined4 *local_44;
  undefined4 local_40;
  uint local_3c;
  uint local_38;
  int *local_34;
  undefined4 *local_30;
  int local_2c;
  undefined4 local_24;
  undefined4 local_20;
  uint local_1c;
  int local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c83cd;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_30 = param_1;
  local_34 = param_2;
  uVar6 = *param_3;
  local_1c = 0;
  iVar1 = *param_2;
  uVar5 = (param_2[1] - uVar6) - iVar1;
  if (uVar5 < 4) {
LAB_015eb0ac:
    uVar11 = 0;
  }
  else {
    local_14 = uVar4;
    if ((uint)(param_2[1] - iVar1) <= uVar6) {
LAB_015eb0cf:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar4);
    }
    if ((uint *)(iVar1 + uVar6) == (uint *)0x0) {
      memset(&local_1c,0,uVar5);
      piVar8 = _errno();
      *piVar8 = 0x16;
      _invalid_parameter_noinfo();
      local_3c = local_1c;
    }
    else {
      local_3c = *(uint *)(iVar1 + uVar6);
    }
    *param_3 = *param_3 + 4;
    local_38 = 0;
    if (local_3c != 0) {
      do {
        uVar11 = 0;
        uVar6 = *param_3;
        local_18 = 0;
        local_24 = 0;
        iVar1 = *local_34;
        local_20 = 0;
        uVar5 = (local_34[1] - uVar6) - iVar1;
        if (uVar5 < 4) goto LAB_015eb0ac;
        if ((uint)(local_34[1] - iVar1) <= uVar6) goto LAB_015eb0cf;
        if ((int *)(uVar6 + iVar1) == (int *)0x0) {
          memset(&local_18,0,uVar5);
          piVar8 = _errno();
          *piVar8 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_18 = *(int *)(uVar6 + iVar1);
        }
        piVar8 = local_34;
        uVar6 = *param_3 + 4;
        *param_3 = uVar6;
        uVar5 = (local_34[1] - *local_34) - uVar6;
        local_2c = local_18;
        if (3 < uVar5) {
          if ((uint)(local_34[1] - *local_34) <= uVar6) goto LAB_015eb0cf;
          if ((undefined4 *)(uVar6 + *local_34) == (undefined4 *)0x0) {
            memset(&local_24,0,uVar5);
            piVar7 = _errno();
            *piVar7 = 0x16;
            _invalid_parameter_noinfo();
            uVar11 = local_20;
          }
          else {
            local_24 = *(undefined4 *)(uVar6 + *local_34);
          }
          *param_3 = *param_3 + 4;
          uVar6 = *param_3;
        }
        iVar1 = piVar8[1];
        iVar2 = *piVar8;
        uVar5 = (iVar1 - uVar6) - iVar2;
        if (3 < uVar5) {
          if ((uint)(iVar1 - iVar2) <= uVar6) goto LAB_015eb0cf;
          if ((undefined4 *)(uVar6 + iVar2) == (undefined4 *)0x0) {
            memset(&local_20,0,uVar5);
            piVar8 = _errno();
            *piVar8 = 0x16;
            _invalid_parameter_noinfo();
            uVar11 = local_20;
          }
          else {
            uVar11 = *(undefined4 *)(uVar6 + iVar2);
          }
          *param_3 = *param_3 + 4;
        }
        puVar9 = (undefined8 *)FUN_015ae4e0(local_5c,&local_18);
        local_50 = *puVar9;
        if ((*(char *)(*(int *)(puVar9 + 1) + 0xd) != '\0') ||
           (local_2c < *(int *)(*(int *)(puVar9 + 1) + 0x10))) {
          if (local_30[1] == 0x7ffffff) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          uVar3 = *local_30;
          local_44 = local_30;
          local_8 = 0;
          local_40 = 0;
          puVar10 = operator_new(0x20);
          local_8 = 0xffffffff;
          local_40 = 0;
          puVar10[4] = local_2c;
          puVar10[6] = local_24;
          puVar10[5] = &DAT_01a18ea8;
          puVar10[7] = uVar11;
          *puVar10 = uVar3;
          puVar10[1] = uVar3;
          puVar10[2] = uVar3;
          *(undefined2 *)(puVar10 + 3) = 0;
          FUN_015b0610((undefined4)local_50,local_50._4_4_,puVar10);
        }
        local_38 = local_38 + 1;
      } while (local_38 < local_3c);
    }
    uVar11 = 1;
  }
  ExceptionList = local_10;
  return uVar11;
}


