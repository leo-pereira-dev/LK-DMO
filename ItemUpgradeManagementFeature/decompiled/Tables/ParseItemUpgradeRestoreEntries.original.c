
/* ============================================================ */
/* Function: FUN_015eb0e0 */
/* Entry: 015eb0e0 */
/* Signature: undefined FUN_015eb0e0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_015eb0e0(undefined4 *param_1,int *param_2,uint *param_3)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  char cVar4;
  uint uVar5;
  undefined8 *puVar6;
  undefined4 *puVar7;
  undefined4 uVar8;
  uint uVar9;
  uint _Size;
  uint uVar10;
  int *piVar11;
  undefined1 local_64 [12];
  undefined8 local_58;
  undefined4 *local_4c;
  undefined4 local_48;
  uint local_44;
  uint local_40;
  int *local_3c;
  undefined4 *local_38;
  undefined *local_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 local_20;
  uint local_1c;
  int local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019c8f7d;
  local_10 = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_38 = param_1;
  local_3c = param_2;
  uVar2 = *param_3;
  local_1c = 0;
  uVar9 = param_2[1] - *param_2;
  uVar10 = uVar9 - uVar2;
  if (uVar10 < 4) {
LAB_015eb2c9:
    uVar8 = 0;
  }
  else {
    local_14 = uVar5;
    if (uVar9 <= uVar2) {
LAB_015eb2ec:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar5);
    }
    puVar1 = (uint *)(*param_2 + uVar2);
    if (puVar1 == (uint *)0x0) {
      memset(&local_1c,0,uVar10);
      piVar11 = _errno();
      *piVar11 = 0x16;
      _invalid_parameter_noinfo();
      local_44 = local_1c;
    }
    else {
      local_44 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    local_40 = 0;
    if (local_44 != 0) {
      do {
        uVar9 = local_40;
        uVar2 = *param_3;
        uVar10 = local_3c[1] - *local_3c;
        local_18 = 0;
        _Size = uVar10 - uVar2;
        local_34 = &DAT_01a18e98;
        local_20 = 0;
        local_30 = 0;
        uStack_2c = 0;
        uStack_28 = 0;
        uStack_24 = 0;
        if (_Size < 4) goto LAB_015eb2c9;
        if (uVar10 <= uVar2) goto LAB_015eb2ec;
        piVar11 = (int *)(*local_3c + uVar2);
        if (piVar11 == (int *)0x0) {
          memset(&local_18,0,_Size);
          piVar11 = _errno();
          *piVar11 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_18 = *piVar11;
        }
        iVar3 = local_18;
        *param_3 = *param_3 + 4;
        cVar4 = FUN_012b80c0(local_3c,param_3);
        if (cVar4 == '\0') goto LAB_015eb2c9;
        puVar6 = (undefined8 *)FUN_015ae4e0(local_64,&local_18);
        local_58 = *puVar6;
        if ((*(char *)(*(int *)(puVar6 + 1) + 0xd) != '\0') ||
           (iVar3 < *(int *)(*(int *)(puVar6 + 1) + 0x10))) {
          if (local_38[1] == 0x5d1745d) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          uVar8 = *local_38;
          local_4c = local_38;
          local_8 = 0;
          local_48 = 0;
          puVar7 = operator_new(0x2c);
          local_8 = 0xffffffff;
          local_48 = 0;
          puVar7[8] = uStack_28;
          puVar7[6] = local_30;
          puVar7[9] = uStack_24;
          puVar7[7] = uStack_2c;
          puVar7[4] = iVar3;
          puVar7[5] = &DAT_01a18e98;
          puVar7[10] = local_20;
          *puVar7 = uVar8;
          puVar7[1] = uVar8;
          puVar7[2] = uVar8;
          *(undefined2 *)(puVar7 + 3) = 0;
          FUN_015b0610((undefined4)local_58,local_58._4_4_,puVar7);
          uVar9 = local_40;
        }
        local_40 = uVar9 + 1;
      } while (local_40 < local_44);
    }
    uVar8 = 1;
  }
  ExceptionList = local_10;
  return uVar8;
}


