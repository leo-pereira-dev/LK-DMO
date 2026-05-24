
/* ============================================================ */
/* Function: FUN_011d50f0 */
/* Entry: 011d50f0 */
/* Signature: undefined FUN_011d50f0() */
/* ============================================================ */


void __thiscall FUN_011d50f0(int param_1,undefined4 param_2)

{
  int iVar1;
  int *piVar2;
  void *pvVar3;
  undefined4 uVar4;
  int iVar5;
  int *piVar6;
  undefined8 *puVar7;
  undefined4 *puVar8;
  undefined8 local_38;
  int local_30;
  undefined1 local_2c [4];
  int *local_28;
  int *local_24;
  int *local_20;
  undefined4 local_1c;
  int local_18 [2];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199336d;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_1c = param_2;
  local_18[0] = param_1;
  FUN_00f6e780(DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  piVar2 = (int *)FUN_016830c0(0x8c);
  local_8 = 0;
  local_24 = piVar2;
  if (piVar2 == (int *)0x0) {
    iVar5 = param_1;
    piVar2 = (int *)0x0;
  }
  else {
    memset(piVar2,0,0x8c);
    FUN_01245550();
    piVar2[0x1d] = (int)&DAT_01ae319c;
    piVar2[0x1e] = 0;
    piVar2[0x1d] = (int)&DAT_01ae3280;
    local_8._0_1_ = 1;
    piVar6 = piVar2 + 0x21;
    *piVar2 = (int)&DAT_01ae2a8c;
    piVar2[0x1d] = (int)&DAT_01ae2a80;
    piVar2[0x1f] = 0;
    piVar2[0x20] = 0;
    *piVar6 = 0;
    piVar2[0x22] = 0;
    local_20 = piVar6;
    pvVar3 = operator_new(0x18);
    *(void **)pvVar3 = pvVar3;
    *(void **)((int)pvVar3 + 4) = pvVar3;
    *(void **)((int)pvVar3 + 8) = pvVar3;
    *(undefined2 *)((int)pvVar3 + 0xc) = 0x101;
    *piVar6 = (int)pvVar3;
    iVar1 = DAT_01bfe470;
    local_8 = CONCAT31(local_8._1_3_,2);
    iVar5 = local_18[0];
    if (DAT_01bfe470 != 0) {
      local_20 = (int *)0x44;
      FUN_00f1d720(&local_38,&local_20);
      if (((*(char *)(local_30 + 0xd) == '\0') && (*(int *)(local_30 + 0x10) < 0x45)) &&
         (local_30 != *(int *)(iVar1 + 4))) {
        uVar4 = *(undefined4 *)(local_30 + 0x14);
      }
      else {
        uVar4 = 0;
      }
      iVar5 = __RTDynamicCast(uVar4,0,&DAT_01bc3cf8,&DAT_01bd38e4,0);
      piVar2[0x1e] = iVar5;
      iVar5 = local_18[0];
    }
  }
  local_8 = 0xffffffff;
  if (piVar2 != (int *)0x0) {
    if (*(code **)(*piVar2 + 0x2c) == FUN_011d12b0) {
      FUN_011d12b0(local_1c);
    }
    else {
      (**(code **)(*piVar2 + 0x2c))(local_1c);
    }
    piVar2[0x12] = iVar5;
    local_28 = (int *)(iVar5 + 0x58);
    local_18[0] = *local_28;
    if (*(int *)(iVar5 + 0x5c) == 0x15555555) {
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8 = 3;
    local_24 = (int *)0x0;
    piVar6 = operator_new(0xc);
    iVar1 = local_18[0];
    local_8 = 0xffffffff;
    local_18[0] = 0;
    piVar6[2] = (int)piVar2;
    *(int *)(iVar5 + 0x5c) = *(int *)(iVar5 + 0x5c) + 1;
    puVar8 = *(undefined4 **)(iVar1 + 4);
    *piVar6 = iVar1;
    piVar6[1] = (int)puVar8;
    *(int **)(iVar1 + 4) = piVar6;
    *puVar8 = piVar6;
    puVar7 = (undefined8 *)FUN_00f1d720(local_2c,local_18);
    local_38 = *puVar7;
    if ((*(char *)(*(int *)(puVar7 + 1) + 0xd) != '\0') ||
       (0 < *(int *)(*(int *)(puVar7 + 1) + 0x10))) {
      if (*(int *)(param_1 + 0x90) == 0xaaaaaaa) {
                    /* WARNING: Subroutine does not return */
        FUN_00f1b5f0();
      }
      uVar4 = *(undefined4 *)(param_1 + 0x8c);
      local_8 = 4;
      local_24 = (int *)0x0;
      local_28 = (undefined4 *)(param_1 + 0x8c);
      puVar8 = operator_new(0x18);
      puVar8[4] = 0;
      puVar8[5] = piVar2;
      *puVar8 = uVar4;
      puVar8[1] = uVar4;
      puVar8[2] = uVar4;
      *(undefined2 *)(puVar8 + 3) = 0;
      FUN_00f1b3e0((undefined4)local_38,local_38._4_4_,puVar8);
    }
  }
  ExceptionList = local_10;
  return;
}


