
/* ============================================================ */
/* Function: FUN_011daf90 */
/* Entry: 011daf90 */
/* Signature: undefined FUN_011daf90() */
/* ============================================================ */


void __thiscall FUN_011daf90(int param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  void *pvVar3;
  int iVar4;
  int *piVar5;
  undefined8 *puVar6;
  int *piVar7;
  undefined4 uVar8;
  int *piVar9;
  undefined8 local_38;
  int local_30;
  undefined1 local_2c [4];
  int *local_28;
  int *local_24;
  int *local_20;
  undefined4 local_1c;
  int *local_18 [2];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01993860;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_1c = param_2;
  local_20 = (int *)(param_1 + 0x8c);
  FUN_00f6e780(DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  piVar2 = (int *)FUN_016830c0(0xa4);
  local_8 = 0;
  local_24 = piVar2;
  if (piVar2 == (int *)0x0) {
    piVar2 = (int *)0x0;
    piVar9 = (int *)(param_1 + 0x8c);
  }
  else {
    memset(piVar2,0,0xa4);
    FUN_01245550();
    piVar2[0x1d] = (int)&DAT_01ae319c;
    piVar2[0x1e] = 0;
    piVar2[0x1d] = (int)&DAT_01ae3280;
    local_8._0_1_ = 1;
    piVar9 = piVar2 + 0x1f;
    *piVar2 = (int)&DAT_01ae2f54;
    piVar2[0x1d] = (int)&DAT_01ae2f94;
    *piVar9 = 0;
    piVar2[0x20] = 0;
    local_18[0] = piVar9;
    pvVar3 = operator_new(0x18);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    *(void **)((int)pvVar3 + 8) = pvVar3;
    *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
    *piVar9 = (int)pvVar3;
    piVar9 = piVar2 + 0x21;
    *piVar9 = 0;
    piVar2[0x22] = 0;
    piVar2[0x23] = 0;
    local_8._0_1_ = 3;
    piVar7 = piVar2 + 0x27;
    piVar2[0x24] = 0;
    piVar2[0x25] = 0;
    *piVar7 = 0;
    piVar2[0x28] = 0;
    local_18[0] = piVar7;
    pvVar3 = operator_new(0x18);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    *(void **)((int)pvVar3 + 8) = pvVar3;
    *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
    *piVar7 = (int)pvVar3;
    local_8 = CONCAT31(local_8._1_3_,4);
    FUN_011d62e0();
    FUN_00f2ed20(3);
    iVar4 = DAT_01bfe470;
    *(undefined4 *)*piVar9 = 9;
    *(undefined4 *)(*piVar9 + 4) = 0xd;
    *(undefined4 *)(*piVar9 + 8) = 0x11;
    piVar9 = local_20;
    if (iVar4 != 0) {
      local_18[0] = (int *)0x44;
      FUN_00f1d720(&local_38,local_18);
      if (((*(char *)(local_30 + 0xd) == '\0') && (*(int *)(local_30 + 0x10) < 0x45)) &&
         (local_30 != *(int *)(iVar4 + 4))) {
        uVar8 = *(undefined4 *)(local_30 + 0x14);
      }
      else {
        uVar8 = 0;
      }
      iVar4 = __RTDynamicCast(uVar8,0,&DAT_01bc3cf8,&DAT_01bd38e4,0);
      piVar2[0x1e] = iVar4;
      piVar9 = local_20;
    }
  }
  local_8 = 0xffffffff;
  if (piVar2 != (int *)0x0) {
    if (*(code **)(*piVar2 + 0x2c) == FUN_011d61d0) {
      FUN_011d61d0(local_1c);
    }
    else {
      (**(code **)(*piVar2 + 0x2c))(local_1c);
    }
    piVar2[0x12] = param_1;
    local_18[0] = *(int **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) goto LAB_011db35b;
    local_8 = 5;
    local_24 = (int *)0x0;
    local_28 = (int *)(param_1 + 0x58);
    piVar5 = operator_new(0xc);
    piVar7 = local_18[0];
    local_8 = 0xffffffff;
    local_18[0] = (int *)0x0;
    piVar5[2] = (int)piVar2;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar1 = (undefined4 *)piVar7[1];
    *piVar5 = (int)piVar7;
    piVar5[1] = (int)puVar1;
    piVar7[1] = (int)piVar5;
    *puVar1 = piVar5;
    puVar6 = (undefined8 *)FUN_00f1d720(local_2c,local_18);
    local_38 = *puVar6;
    if ((*(char *)(*(int *)(puVar6 + 1) + 0xd) != '\0') ||
       (0 < *(int *)(*(int *)(puVar6 + 1) + 0x10))) {
      if (piVar9[1] == 0xaaaaaaa) goto LAB_011db42f;
      iVar4 = *piVar9;
      local_28 = local_20;
      local_8 = 6;
      local_24 = (int *)0x0;
      piVar7 = operator_new(0x18);
      piVar9 = local_20;
      local_8 = 0xffffffff;
      piVar7[4] = 0;
      piVar7[5] = (int)piVar2;
      *piVar7 = iVar4;
      piVar7[1] = iVar4;
      piVar7[2] = iVar4;
      *(undefined2 *)(piVar7 + 3) = 0;
      FUN_00f1b3e0((undefined4)local_38,local_38._4_4_,piVar7);
    }
  }
  piVar2 = (int *)FUN_016830c0(0x84);
  local_8 = 7;
  local_24 = piVar2;
  if (piVar2 == (int *)0x0) {
    piVar2 = (int *)0x0;
  }
  else {
    memset(piVar2,0,0x84);
    FUN_01245550();
    piVar2[0x1d] = (int)&DAT_01ae319c;
    piVar2[0x1e] = 0;
    piVar2[0x1d] = (int)&DAT_01ae3280;
    iVar4 = DAT_01bfe470;
    local_8 = CONCAT31(local_8._1_3_,8);
    *piVar2 = (int)&DAT_01ae3360;
    piVar2[0x1d] = (int)&DAT_01ae33a0;
    piVar2[0x1f] = 0;
    piVar2[0x20] = 0;
    if (iVar4 != 0) {
      local_18[0] = (int *)0x44;
      FUN_00f1d720(&local_38,local_18);
      if (((*(char *)(local_30 + 0xd) == '\0') && (*(int *)(local_30 + 0x10) < 0x45)) &&
         (local_30 != *(int *)(iVar4 + 4))) {
        uVar8 = *(undefined4 *)(local_30 + 0x14);
      }
      else {
        uVar8 = 0;
      }
      iVar4 = __RTDynamicCast(uVar8,0,&DAT_01bc3cf8,&DAT_01bd38e4,0);
      piVar2[0x1e] = iVar4;
    }
  }
  local_8 = 0xffffffff;
  if (piVar2 != (int *)0x0) {
    if (*(code **)(*piVar2 + 0x2c) == FUN_011da000) {
      FUN_011da000(local_1c);
    }
    else {
      (**(code **)(*piVar2 + 0x2c))(local_1c);
    }
    piVar2[0x12] = param_1;
    local_18[0] = *(int **)(param_1 + 0x58);
    if (*(int *)(param_1 + 0x5c) == 0x15555555) {
LAB_011db35b:
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8 = 9;
    local_24 = (int *)0x0;
    local_28 = (int *)(param_1 + 0x58);
    piVar5 = operator_new(0xc);
    piVar7 = local_18[0];
    local_8 = 0xffffffff;
    local_18[0] = (int *)0x1;
    piVar5[2] = (int)piVar2;
    *(int *)(param_1 + 0x5c) = *(int *)(param_1 + 0x5c) + 1;
    puVar1 = (undefined4 *)piVar7[1];
    *piVar5 = (int)piVar7;
    piVar5[1] = (int)puVar1;
    piVar7[1] = (int)piVar5;
    *puVar1 = piVar5;
    puVar6 = (undefined8 *)FUN_00f1d720(local_2c,local_18);
    local_38 = *puVar6;
    if ((*(char *)(*(int *)(puVar6 + 1) + 0xd) != '\0') ||
       (1 < *(int *)(*(int *)(puVar6 + 1) + 0x10))) {
      if (piVar9[1] == 0xaaaaaaa) {
LAB_011db42f:
                    /* WARNING: Subroutine does not return */
        FUN_00f1b5f0();
      }
      iVar4 = *piVar9;
      local_28 = local_20;
      local_8 = 10;
      local_24 = (int *)0x0;
      piVar9 = operator_new(0x18);
      piVar9[4] = 1;
      piVar9[5] = (int)piVar2;
      *piVar9 = iVar4;
      piVar9[1] = iVar4;
      piVar9[2] = iVar4;
      *(undefined2 *)(piVar9 + 3) = 0;
      FUN_00f1b3e0((undefined4)local_38,local_38._4_4_,piVar9);
    }
  }
  ExceptionList = local_10;
  return;
}


