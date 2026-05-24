
/* ============================================================ */
/* Function: FUN_015ea9c0 */
/* Entry: 015ea9c0 */
/* Signature: undefined FUN_015ea9c0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_015ea9c0(undefined4 *param_1,int *param_2,uint *param_3)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  char cVar4;
  uint uVar5;
  uint uVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 uVar9;
  uint uVar10;
  int *piVar11;
  undefined1 local_7c [12];
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 *local_64;
  undefined4 *local_60;
  uint *local_5c;
  uint local_58;
  uint local_54;
  int *local_50;
  undefined4 *local_4c;
  undefined *local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 *local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  uint local_1c;
  int local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c8f3d;
  local_10 = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_4c = param_1;
  uVar6 = param_2[1] - *param_2;
  uVar2 = *param_3;
  uVar10 = uVar6 - uVar2;
  local_50 = param_2;
  local_5c = param_3;
  local_1c = 0;
  if (uVar10 < 4) {
LAB_015eac86:
    uVar9 = 0;
  }
  else {
    local_14 = uVar5;
    if (uVar6 <= uVar2) {
LAB_015eaca8:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar5);
    }
    puVar1 = (uint *)(*param_2 + uVar2);
    if (puVar1 == (uint *)0x0) {
      memset(&local_1c,0,uVar10);
      piVar11 = _errno();
      *piVar11 = 0x16;
      _invalid_parameter_noinfo();
      local_58 = local_1c;
    }
    else {
      local_58 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    local_54 = 0;
    if (local_58 != 0) {
      do {
        local_18 = 0;
        memset(&local_48,0,0x2c);
        local_48 = &DAT_01a18e68;
        local_44 = 0;
        uStack_40 = 0;
        uStack_3c = 0;
        uStack_38 = 0;
        local_34 = 0;
        local_30 = (undefined4 *)0x0;
        local_2c = 0;
        local_30 = operator_new(0x18);
        *local_30 = local_30;
        local_30[1] = local_30;
        local_28 = 0;
        local_24 = 0;
        local_20 = 0;
        uVar6 = param_2[1] - *param_2;
        uVar2 = *param_3;
        uVar10 = uVar6 - uVar2;
        local_8 = 0;
        if (uVar10 < 4) {
LAB_015eac57:
          *(undefined4 *)local_30[1] = 0;
          puVar8 = (undefined4 *)*local_30;
          while (puVar8 != (undefined4 *)0x0) {
            puVar7 = (undefined4 *)*puVar8;
            FUN_017986fc(puVar8,0x18);
            puVar8 = puVar7;
          }
          FUN_017986fc(local_30,0x18);
          goto LAB_015eac86;
        }
        if (uVar6 <= uVar2) goto LAB_015eaca8;
        piVar11 = (int *)(*param_2 + uVar2);
        if (piVar11 == (int *)0x0) {
          memset(&local_18,0,uVar10);
          piVar11 = _errno();
          *piVar11 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_18 = *piVar11;
        }
        iVar3 = local_18;
        param_2 = local_50;
        *param_3 = *param_3 + 4;
        cVar4 = FUN_015ecd40(local_50,param_3);
        puVar8 = local_4c;
        if (cVar4 == '\0') goto LAB_015eac57;
        puVar7 = (undefined4 *)FUN_015ae4e0(local_7c,&local_18);
        local_70 = *puVar7;
        uStack_6c = puVar7[1];
        if ((*(char *)(puVar7[2] + 0xd) != '\0') || (iVar3 < *(int *)(puVar7[2] + 0x10))) {
          if (puVar8[1] == 0x3ffffff) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          uVar9 = *puVar8;
          local_64 = puVar8;
          local_8._0_1_ = 1;
          local_60 = (undefined4 *)0x0;
          puVar8 = operator_new(0x40);
          puVar8[4] = iVar3;
          puVar8[5] = &DAT_01a18e68;
          puVar8[6] = local_44;
          puVar8[7] = uStack_40;
          puVar8[8] = uStack_3c;
          puVar8[9] = uStack_38;
          puVar8[10] = local_34;
          local_8 = CONCAT31(local_8._1_3_,2);
          local_60 = puVar8;
          FUN_015eb980(&local_30);
          puVar8[0xd] = local_28;
          puVar8[0xe] = local_24;
          puVar8[0xf] = local_20;
          *puVar8 = uVar9;
          puVar8[1] = uVar9;
          puVar8[2] = uVar9;
          *(undefined2 *)(puVar8 + 3) = 0;
          local_60 = (undefined4 *)0x0;
          FUN_015b0610(local_70,uStack_6c,puVar8);
          param_2 = local_50;
        }
        local_8 = 0xffffffff;
        *(undefined4 *)local_30[1] = 0;
        puVar8 = (undefined4 *)*local_30;
        while (puVar8 != (undefined4 *)0x0) {
          puVar7 = (undefined4 *)*puVar8;
          FUN_017986fc(puVar8,0x18);
          puVar8 = puVar7;
        }
        FUN_017986fc(local_30,0x18);
        local_54 = local_54 + 1;
        param_3 = local_5c;
      } while (local_54 < local_58);
    }
    uVar9 = 1;
  }
  ExceptionList = local_10;
  return uVar9;
}


