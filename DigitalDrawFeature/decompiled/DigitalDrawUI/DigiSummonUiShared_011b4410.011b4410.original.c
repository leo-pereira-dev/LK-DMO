
/* ============================================================ */
/* Function: FUN_011b4410 */
/* Entry: 011b4410 */
/* Signature: undefined FUN_011b4410() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011b4410(int param_1,int *param_2)

{
  int iVar1;
  undefined4 ***pppuVar2;
  void *pvVar3;
  uint *puVar4;
  size_t _Size;
  uint uVar5;
  uint uVar6;
  int *piVar7;
  uint uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  int iVar13;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 local_50;
  undefined4 local_4c;
  uint local_48;
  undefined1 local_41;
  undefined4 **local_40 [5];
  uint local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined8 local_20;
  undefined1 local_15;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01990e2d;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar1 = *param_2;
  if (iVar1 == 7) {
    local_20 = 0;
    pvVar3 = operator_new(0x38);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    local_20 = CONCAT44(local_20._4_4_,pvVar3);
    local_8 = 0;
    local_15 = 0;
    local_24 = 0;
    local_28 = 0;
    puVar4 = (uint *)FUN_011b5760(&local_20);
    local_48 = puVar4[3];
    uVar8 = puVar4[2];
    if (((uVar8 != local_48) && (uVar6 = *puVar4, uVar6 < puVar4[1])) &&
       (_Size = (local_48 - uVar8) - uVar6, _Size != 0)) {
      local_48 = local_48 - uVar8;
      if (local_48 <= uVar6) goto LAB_011b4707;
      if ((undefined1 *)(uVar6 + uVar8) == (undefined1 *)0x0) {
        memset(&local_15,0,_Size);
        piVar7 = _errno();
        *piVar7 = 0x16;
        _invalid_parameter_noinfo();
        uVar8 = puVar4[2];
      }
      else {
        local_15 = *(undefined1 *)(uVar6 + uVar8);
      }
      *puVar4 = *puVar4 + 1;
    }
    uVar6 = puVar4[3];
    if (((uVar8 != uVar6) && (*puVar4 < puVar4[1])) &&
       (uVar5 = (uVar6 - *puVar4) - uVar8, 3 < uVar5)) {
      if (uVar6 - uVar8 <= *puVar4) goto LAB_011b4707;
      if ((undefined4 *)(uVar8 + *puVar4) == (undefined4 *)0x0) {
        memset(&local_24,0,uVar5);
        piVar7 = _errno();
        *piVar7 = 0x16;
        _invalid_parameter_noinfo();
      }
      else {
        local_24 = *(undefined4 *)(uVar8 + *puVar4);
      }
      *puVar4 = *puVar4 + 4;
    }
    if ((puVar4[2] != puVar4[3]) && (uVar8 = *puVar4, uVar8 < puVar4[1])) {
      uVar6 = puVar4[3] - puVar4[2];
      uVar5 = uVar6 - uVar8;
      if (3 < uVar5) {
        if (uVar6 <= uVar8) {
LAB_011b4707:
                    /* WARNING: Subroutine does not return */
          FUN_00f3fcb0();
        }
        if ((undefined4 *)(puVar4[2] + uVar8) == (undefined4 *)0x0) {
          memset(&local_28,0,uVar5);
          piVar7 = _errno();
          *piVar7 = 0x16;
          _invalid_parameter_noinfo();
        }
        else {
          local_28 = *(undefined4 *)(puVar4[2] + uVar8);
        }
        *puVar4 = *puVar4 + 4;
      }
    }
    FUN_011b4d10(&local_20);
    if (*(int **)(param_1 + 0x10) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x10) + 0x1c))(1);
    }
    FUN_011b5610(&local_15,&local_24);
    FUN_011b54f0(&local_15,&local_28);
    FUN_0102f760();
  }
  else if (iVar1 == 8) {
    if (*(int **)(param_1 + 0x10) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x10) + 0x1c))(0,local_14);
    }
    local_20 = (ulonglong)(uint)local_20;
    local_41 = 0;
    FUN_011b5610(&local_41,(int)&local_20 + 4);
    local_20 = local_20 & 0xffffffff;
    local_41 = 0;
    FUN_011b54f0(&local_41,(int)&local_20 + 4);
  }
  else {
    iVar1 = iVar1 + -0xb;
    if ((iVar1 == 0) && (local_48 = 1, *(int *)(param_1 + 0x1c) != 0)) {
      uVar9 = 0;
      uVar10 = 0;
      uVar11 = 0;
      uVar12 = 0;
      iVar13 = iVar1;
      FUN_00f19cc0(s_DIGISUMMON_COUNT_REMAINING_01ae17b4,0x1a);
      FUN_015ce480(local_40,uVar9,uVar10,uVar11,uVar12,iVar1,iVar13);
      local_8 = 1;
      local_50 = 0;
      local_60 = 0;
      uStack_5c = 0;
      uStack_58 = 0;
      uStack_54 = 0;
      local_4c = 0;
      FUN_00f1a200(u__value__01ae17ec,7);
      local_8._0_1_ = 2;
      FUN_01501660(local_40,&local_60,&local_48);
      local_8 = CONCAT31(local_8._1_3_,1);
      FUN_00f1a330();
      pppuVar2 = local_40;
      if (7 < local_2c) {
        pppuVar2 = (undefined4 ***)local_40[0];
      }
      (**(code **)(**(int **)(param_1 + 0x1c) + 0x10))(pppuVar2);
      FUN_00f1a330();
    }
  }
  ExceptionList = local_10;
  return;
}


