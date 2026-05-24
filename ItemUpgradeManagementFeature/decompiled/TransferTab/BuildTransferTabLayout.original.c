
/* ============================================================ */
/* Function: FUN_0126d080 */
/* Entry: 0126d080 */
/* Signature: undefined FUN_0126d080() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_0126d080(int param_1,int *param_2,uint *param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  uint uVar4;
  int *piVar5;
  uint uVar6;
  uint uVar7;
  undefined1 *_Size;
  undefined1 *puVar8;
  undefined1 *extraout_ECX;
  float10 fVar9;
  uint local_48;
  uint local_44;
  int *local_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199bbcd;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar3;
  if (*param_2 == 0x1a) {
    local_20 = 0;
    local_1c = 0;
    uStack_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    local_2c = 0;
    local_28 = 7;
    local_3c = 0;
    local_8 = 0;
    uVar7 = param_3[2];
    local_18 = local_18 & 0xffffff00;
    if ((uVar7 != param_3[3]) && (uVar4 = *param_3, uVar4 < param_3[1])) {
      uVar6 = param_3[3] - uVar7;
      local_40 = (int *)(uVar6 - uVar4);
      if ((int *)0x3 < local_40) {
        if (uVar6 <= uVar4) goto LAB_0126d3f1;
        if ((undefined4 *)(uVar7 + uVar4) == (undefined4 *)0x0) {
          memset(&local_20,0,(size_t)local_40);
          piVar5 = _errno();
          *piVar5 = 0x16;
          _invalid_parameter_noinfo();
          uVar7 = param_3[2];
        }
        else {
          local_20 = *(undefined4 *)(uVar7 + uVar4);
        }
        *param_3 = *param_3 + 4;
      }
    }
    uVar4 = param_3[3];
    if (((uVar7 != uVar4) && (*param_3 < param_3[1])) && (3 < (uVar4 - *param_3) - uVar7)) {
      if (uVar4 - uVar7 <= *param_3) goto LAB_0126d3f1;
      if (*param_3 + uVar7 == 0) {
        piVar5 = _errno();
        *piVar5 = 0x16;
        _invalid_parameter_noinfo();
      }
      *param_3 = *param_3 + 4;
    }
    if ((param_3[2] != param_3[3]) && (*param_3 < param_3[1])) {
      FUN_0100f610(&local_3c,param_3 + 2,param_3,uVar3);
    }
    if ((param_3[2] != param_3[3]) && (uVar3 = *param_3, uVar3 < param_3[1])) {
      uVar7 = param_3[3] - param_3[2];
      uVar4 = uVar7 - uVar3;
      if (3 < uVar4) {
        if (uVar7 <= uVar3) goto LAB_0126d3f1;
        if ((undefined4 *)(param_3[2] + uVar3) == (undefined4 *)0x0) {
          memset(&local_1c,0,uVar4);
          piVar5 = _errno();
          *piVar5 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_1c = *(undefined4 *)(param_3[2] + uVar3);
        }
        *param_3 = *param_3 + 4;
      }
    }
    puVar8 = (undefined1 *)param_3[3];
    if ((((undefined1 *)param_3[2] != puVar8) && (uVar3 = *param_3, uVar3 < param_3[1])) &&
       (_Size = puVar8 + (-uVar3 - param_3[2]), _Size != (undefined1 *)0x0)) {
      if ((int)puVar8 - param_3[2] <= uVar3) {
LAB_0126d3f1:
                    /* WARNING: Subroutine does not return */
        FUN_00f3fcb0();
      }
      puVar8 = (undefined1 *)(param_3[2] + uVar3);
      if (puVar8 == (undefined1 *)0x0) {
        memset(&local_18,0,(size_t)_Size);
        piVar5 = _errno();
        *piVar5 = 0x16;
        _invalid_parameter_noinfo();
        puVar8 = extraout_ECX;
      }
      else {
        local_18 = CONCAT31(local_18._1_3_,*puVar8);
      }
      *param_3 = *param_3 + 1;
    }
    FUN_0126ef40(&local_20,&local_1c,puVar8,&local_3c,local_18);
    FUN_00f1a330();
  }
  else if (*param_2 == 0x1d) {
    uVar3 = param_3[3];
    if (((param_3[2] != uVar3) && (uVar7 = *param_3, uVar7 < param_3[1])) &&
       (uVar4 = (uVar3 - param_3[2]) - uVar7, 3 < uVar4)) {
      if (uVar3 - param_3[2] <= uVar7) {
                    /* WARNING: Subroutine does not return */
        FUN_00f3fcb0();
      }
      if ((undefined4 *)(param_3[2] + uVar7) == (undefined4 *)0x0) {
        memset(&local_24,0,uVar4);
        piVar5 = _errno();
        *piVar5 = 0x16;
        _invalid_parameter_noinfo();
        *param_3 = *param_3 + 4;
      }
      else {
        local_24 = *(undefined4 *)(param_3[2] + uVar7);
        *param_3 = *param_3 + 4;
      }
    }
    FUN_01271060(local_24);
    *(undefined4 *)(param_1 + 0xd4) = 0xffffffff;
    local_44 = 0;
    if (*(int *)(param_1 + 0xd0) != 0) {
      do {
        local_48 = local_44;
        piVar5 = (int *)FUN_0126c790(&local_48);
        puVar1 = (undefined4 *)*piVar5;
        for (puVar2 = (undefined4 *)*puVar1; puVar2 != puVar1; puVar2 = (undefined4 *)*puVar2) {
          *(undefined1 *)(puVar2[2] + 0xe4) = 0;
          *(undefined4 *)(puVar2[2] + 0xd4) = 0x3c23d70a;
          local_40 = (int *)puVar2[2];
          local_40[0x3b] = 0;
          local_40[0x3c] = 0;
          fVar9 = (float10)fmaxf(0,0x3f000000);
          local_40[0x32] = (int)(float)fVar9;
          FUN_01243010();
          (**(code **)(*local_40 + 0x48))();
          FUN_01242bf0();
        }
        local_44 = local_44 + 1;
      } while (local_44 < *(uint *)(param_1 + 0xd0));
    }
    FUN_01270a10();
  }
  ExceptionList = local_10;
  return;
}


