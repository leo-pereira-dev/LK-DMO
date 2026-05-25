
/* ============================================================ */
/* Function: FUN_011dcbe0 */
/* Entry: 011dcbe0 */
/* Signature: undefined FUN_011dcbe0() */
/* ============================================================ */


void FUN_011dcbe0(int param_1,undefined4 *param_2)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int *piVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  int *piVar7;
  int iVar8;
  int *piVar9;
  undefined1 local_3c [4];
  undefined1 local_38 [4];
  undefined4 *local_34;
  int local_30;
  undefined4 *local_2c;
  undefined4 *local_28;
  int local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01993b64;
  local_1c = ExceptionList;
  local_30 = param_1;
  if (param_1 == 0) {
    return;
  }
  ExceptionList = &local_1c;
  if (param_2 == (undefined4 *)0xffffffff) {
    puStack_20 = &stack0xfffffffc;
    puVar6 = (undefined4 *)FUN_016830c0(0x28,DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
    if (puVar6 == (undefined4 *)0x0) {
      ExceptionList = local_1c;
      return;
    }
    *puVar6 = 0;
    puVar6[1] = 0;
    puVar6[2] = 0;
    puVar6[3] = 0;
    puVar6[4] = 0;
    puVar6[5] = 0;
    puVar6[6] = 0;
    puVar6[7] = 0;
    *(undefined8 *)(puVar6 + 8) = 0;
    puVar6[3] = *(undefined4 *)(param_1 + 0x68);
    *puVar6 = &DAT_01ae66e0;
    puVar6[1] = 0;
    puVar6[2] = 0;
    *(undefined1 *)(puVar6 + 4) = 0;
    puVar6[5] = 0x22;
    puVar6[6] = 0x22;
    *(undefined2 *)(puVar6 + 7) = 0x101;
    puVar6[8] = 0;
    puVar6[9] = 0;
    *(undefined1 *)((int)puVar6 + 0x1d) = 0;
    local_34 = puVar6;
    if (puVar6[1] != 0) {
      puVar2 = *(undefined4 **)(puVar6[1] + 4);
      for (puVar3 = (undefined4 *)*puVar2; puVar3 != puVar2; puVar3 = (undefined4 *)*puVar3) {
        local_28 = puVar6;
        (**(code **)(*(int *)puVar3[2] + 0x24))(0);
        puVar6 = local_28;
      }
    }
  }
  else {
    local_34 = (undefined4 *)FUN_016830c0(0x2c,DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
    local_14 = 0;
    if (local_34 == (void *)0x0) {
      local_28 = (undefined4 *)0x0;
    }
    else {
      memset(local_34,0,0x2c);
      local_28 = (undefined4 *)FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (local_28 == (undefined4 *)0x0) {
      ExceptionList = local_1c;
      return;
    }
    piVar9 = (int *)(local_24 + 0x94);
    local_2c = (undefined4 *)*piVar9;
    cVar1 = *(char *)((int)local_2c[1] + 0xd);
    piVar7 = (int *)local_2c[1];
    while (cVar1 == '\0') {
      FUN_00f1d770(piVar9,piVar7[2]);
      piVar4 = (int *)*piVar7;
      FUN_017986fc(piVar7,0x18);
      piVar7 = piVar4;
      cVar1 = *(char *)((int)piVar4 + 0xd);
    }
    local_2c[1] = local_2c;
    *local_2c = local_2c;
    local_2c[2] = local_2c;
    *(undefined4 *)(local_24 + 0x98) = 0;
    piVar7 = (int *)FUN_010102c0(local_38);
    if (piVar9 != piVar7) {
      FUN_00f6e780();
      iVar8 = *piVar9;
      *piVar9 = *piVar7;
      *piVar7 = iVar8;
      iVar8 = *(int *)(local_24 + 0x98);
      *(int *)(local_24 + 0x98) = piVar7[1];
      piVar7[1] = iVar8;
    }
    FUN_00f31c20(local_38);
    iVar8 = 0;
    local_2c = param_2;
    local_24 = 0;
    FUN_00f1b390(local_3c,&local_2c);
    if ((((*(char *)((int)local_34 + 0xd) == '\0') &&
         (*(undefined4 **)((int)local_34 + 0x10) <= param_2)) && (local_34 != (void *)*piVar9)) &&
       (*(int *)((int)local_34 + 0x14) != 0)) {
      iVar8 = *(int *)(*(int *)((int)local_34 + 0x14) + 4);
      local_24 = iVar8;
    }
    piVar7 = (int *)FUN_01231410(10,iVar8,1,0x22,0x22,DAT_01bee88c,DAT_01bee890);
    if (piVar7 != (int *)0x0) {
      (**(code **)(*piVar7 + 0x54))(1);
    }
    puVar6 = (undefined4 *)FUN_016830c0(0x28);
    if (puVar6 == (undefined4 *)0x0) {
      ExceptionList = local_1c;
      return;
    }
    *puVar6 = 0;
    puVar6[1] = 0;
    puVar6[2] = 0;
    puVar6[3] = 0;
    puVar6[4] = 0;
    puVar6[5] = 0;
    puVar6[6] = 0;
    puVar6[7] = 0;
    *(undefined8 *)(puVar6 + 8) = 0;
    uVar5 = *(undefined4 *)(local_30 + 0x68);
    *puVar6 = &DAT_01ae66e0;
    puVar6[1] = 0;
    puVar6[2] = 0;
    puVar6[3] = uVar5;
    *(undefined1 *)(puVar6 + 4) = 0;
    puVar6[5] = 0x22;
    puVar6[6] = 0x22;
    *(undefined2 *)(puVar6 + 7) = 0x101;
    puVar6[8] = 0;
    puVar6[9] = 0;
    local_34 = puVar6;
    local_2c = operator_new(0xc);
    iVar8 = local_24;
    *local_2c = &DAT_01ae35bc;
    local_2c[1] = local_24;
    local_2c[2] = param_2;
    if ((undefined4 *)puVar6[2] != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)puVar6[2])(1);
    }
    puVar6[2] = local_2c;
    FUN_012288c0(local_28);
    if (iVar8 == 0) {
      *(undefined1 *)((int)puVar6 + 0x1d) = 0;
      if (puVar6[1] != 0) {
        puVar2 = *(undefined4 **)(puVar6[1] + 4);
        puVar6 = local_34;
        for (puVar3 = (undefined4 *)*puVar2; local_34 = puVar6, puVar3 != puVar2;
            puVar3 = (undefined4 *)*puVar3) {
          (**(code **)(*(int *)puVar3[2] + 0x24))(0);
          puVar6 = local_34;
        }
      }
    }
  }
  FUN_0122a090(puVar6);
  ExceptionList = local_1c;
  return;
}


