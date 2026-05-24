
/* ============================================================ */
/* Function: FUN_0162e920 */
/* Entry: 0162e920 */
/* Signature: undefined FUN_0162e920() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_0162e920(undefined4 *param_1,int *param_2,uint *param_3)

{
  uint *puVar1;
  ushort *puVar2;
  uint uVar3;
  char cVar4;
  uint uVar5;
  uint uVar6;
  void *pvVar7;
  int *piVar8;
  undefined8 *puVar9;
  undefined4 *puVar10;
  undefined4 uVar11;
  uint uVar12;
  ushort uVar13;
  undefined1 local_58 [12];
  undefined8 local_4c;
  undefined4 *local_40;
  undefined4 *local_3c;
  uint *local_38;
  int *local_34;
  uint local_30;
  uint local_2c;
  undefined4 *local_28;
  uint local_24;
  undefined8 local_20;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019cd81d;
  local_10 = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_28 = param_1;
  uVar6 = param_2[1] - *param_2;
  uVar3 = *param_3;
  uVar12 = uVar6 - uVar3;
  local_34 = param_2;
  local_38 = param_3;
  local_24 = 0;
  if (uVar12 < 4) {
LAB_0162eb3e:
    uVar11 = 0;
  }
  else {
    local_14 = uVar5;
    if (uVar6 <= uVar3) {
LAB_0162eb60:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar5);
    }
    puVar1 = (uint *)(*param_2 + uVar3);
    if (puVar1 == (uint *)0x0) {
      memset(&local_24,0,uVar12);
      piVar8 = _errno();
      *piVar8 = 0x16;
      _invalid_parameter_noinfo();
      local_30 = local_24;
    }
    else {
      local_30 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    local_2c = 0;
    if (local_30 != 0) {
      do {
        local_18 = 0;
        local_20 = 0;
        pvVar7 = operator_new(0x20);
        *(void **)pvVar7 = pvVar7;
        *(void **)((int)pvVar7 + 4) = pvVar7;
        *(void **)((int)pvVar7 + 8) = pvVar7;
        *(undefined2 *)((int)pvVar7 + 0xc) = 0x101;
        local_20 = CONCAT44(local_20._4_4_,pvVar7);
        uVar6 = param_2[1] - *param_2;
        uVar3 = *param_3;
        uVar12 = uVar6 - uVar3;
        local_8 = 0;
        if (uVar12 < 2) {
LAB_0162eb33:
          FUN_00f325c0(&local_20);
          goto LAB_0162eb3e;
        }
        if (uVar6 <= uVar3) goto LAB_0162eb60;
        puVar2 = (ushort *)(*param_2 + uVar3);
        if (puVar2 == (ushort *)0x0) {
          memset(&local_18,0,uVar12);
          piVar8 = _errno();
          *piVar8 = 0x16;
          _invalid_parameter_noinfo();
          uVar13 = (ushort)local_18;
        }
        else {
          uVar13 = *puVar2;
          local_18 = (uint)uVar13;
        }
        *param_3 = *param_3 + 2;
        cVar4 = FUN_0162ee00(&local_20,local_34,param_3);
        if (cVar4 == '\0') goto LAB_0162eb33;
        puVar9 = (undefined8 *)FUN_015e2b20(local_58,&local_18);
        local_4c = *puVar9;
        if ((*(char *)(*(int *)(puVar9 + 1) + 0xd) != '\0') ||
           ((short)uVar13 < *(short *)(*(int *)(puVar9 + 1) + 0x10))) {
          if (local_28[1] == 0x9249249) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          uVar11 = *local_28;
          local_40 = local_28;
          local_8._0_1_ = 1;
          local_3c = (undefined4 *)0x0;
          puVar10 = operator_new(0x1c);
          local_8 = CONCAT31(local_8._1_3_,2);
          *(ushort *)(puVar10 + 4) = uVar13;
          local_3c = puVar10;
          FUN_0162f040(&local_20);
          *puVar10 = uVar11;
          puVar10[1] = uVar11;
          puVar10[2] = uVar11;
          *(undefined2 *)(puVar10 + 3) = 0;
          local_3c = (undefined4 *)0x0;
          FUN_015b0610((undefined4)local_4c,local_4c._4_4_,puVar10);
          param_3 = local_38;
        }
        local_8 = 0xffffffff;
        FUN_00f325c0(&local_20);
        local_2c = local_2c + 1;
        param_2 = local_34;
      } while (local_2c < local_30);
    }
    uVar11 = 1;
  }
  ExceptionList = local_10;
  return uVar11;
}


