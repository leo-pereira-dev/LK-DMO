
/* ============================================================ */
/* Function: FUN_011b4d10 */
/* Entry: 011b4d10 */
/* Signature: undefined FUN_011b4d10() */
/* ============================================================ */


void __thiscall FUN_011b4d10(int param_1,int *param_2)

{
  char cVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 uVar7;
  int *piVar8;
  undefined1 local_50 [24];
  undefined1 local_38 [8];
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int *local_20;
  int *local_1c;
  int *local_18;
  int *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01990eef;
  local_10 = ExceptionList;
  local_1c = param_2;
  local_18 = (int *)param_2[1];
  if (*(int **)(param_1 + 0x80) == local_18) {
    param_2 = (int *)*param_2;
    local_18 = (int *)0x0;
    piVar8 = (int *)*param_2;
    if (piVar8 == param_2) {
      return;
    }
    local_20 = (int *)(param_1 + 0x7c);
    ExceptionList = &local_10;
    do {
      local_24 = piVar8[6];
      local_1c = local_18;
      local_14 = local_18;
      local_18 = (int *)((int)local_18 + 1);
      FUN_00f1d720(local_38,&local_1c);
      iVar5 = local_30;
      if (((*(char *)(local_30 + 0xd) == '\0') && (*(int *)(local_30 + 0x10) <= (int)local_14)) &&
         (local_30 != *local_20)) {
        iVar6 = *(int *)(local_30 + 0x18);
        if (iVar6 != 0) {
          *(undefined4 *)(iVar6 + 0x18) = 10;
          *(int *)(iVar6 + 0x1c) = local_24;
          *(undefined4 *)(iVar6 + 0x20) = 1;
          *(undefined4 *)(iVar6 + 0x24) = 0;
        }
        local_14 = *(int **)(local_30 + 0x1c);
        if (local_14 != (int *)0x0) {
          local_2c = piVar8[4];
          local_28 = piVar8[5];
          puVar4 = (undefined4 *)FUN_015027f0(local_50,&local_2c);
          local_8 = 0;
          if (7 < (uint)puVar4[5]) {
            puVar4 = (undefined4 *)*puVar4;
          }
          (**(code **)(local_14[5] + 0x10))(puVar4);
          local_8 = 0xffffffff;
          FUN_00f1a330();
        }
        if ((*(int *)(iVar5 + 0x20) != 0) &&
           (iVar5 = __RTDynamicCast(*(undefined4 *)(*(int *)(iVar5 + 0x20) + 8),0,&DAT_01bc4a94,
                                    &DAT_01bc5f04,0), iVar5 != 0)) {
          *(int *)(iVar5 + 0x10) = local_24;
          iVar6 = piVar8[5];
          *(int *)(iVar5 + 8) = piVar8[4];
          *(int *)(iVar5 + 0xc) = iVar6;
        }
      }
      piVar8 = (int *)*piVar8;
    } while (piVar8 != param_2);
    ExceptionList = local_10;
    return;
  }
  piVar8 = (int *)((int)local_18 * 200);
  iVar5 = *(int *)(param_1 + 0x38) + (int)local_18 * -200 >> 1;
  local_14 = piVar8;
  if (*(int **)(param_1 + 0x84) == (int *)0x0) {
    ExceptionList = &local_10;
    local_20 = (int *)FUN_016830c0(0xac);
    local_8 = 1;
    if (local_20 == (void *)0x0) {
      iVar6 = 0;
      uVar7 = extraout_ECX;
    }
    else {
      memset(local_20,0,0xac);
      iVar6 = FUN_01228950();
      uVar7 = extraout_ECX_00;
    }
    local_8 = 0xffffffff;
    *(int *)(param_1 + 0x84) = iVar6;
    if (iVar6 == 0) goto LAB_011b4f93;
    FUN_01228bd0(*(undefined4 *)(param_1 + 0x40),iVar5,0xf,local_14,0x50,0,0,200,0x50,0,2,uVar7,
                 local_18);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x84));
  }
  else {
    ExceptionList = &local_10;
    (**(code **)(**(int **)(param_1 + 0x84) + 0x34))
              (iVar5,0xf,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
    (**(code **)(**(int **)(param_1 + 0x84) + 0x38))(piVar8,0x50);
    (**(code **)(**(int **)(param_1 + 0x84) + 0x3c))();
  }
  (**(code **)(**(int **)(param_1 + 0x84) + 0x1c))(0);
  FUN_0122a180();
  local_18 = *(int **)(param_1 + 0x7c);
  cVar1 = *(char *)(local_18[1] + 0xd);
  piVar8 = (int *)local_18[1];
  while (cVar1 == '\0') {
    FUN_00f45df0(param_1 + 0x7c,piVar8[2]);
    piVar2 = (int *)*piVar8;
    FUN_017986fc(piVar8,0x24);
    piVar8 = piVar2;
    cVar1 = *(char *)((int)piVar2 + 0xd);
  }
  local_18[1] = (int)local_18;
  *local_18 = (int)local_18;
  local_18[2] = (int)local_18;
  *(undefined4 *)(param_1 + 0x80) = 0;
LAB_011b4f93:
  puVar4 = (undefined4 *)*local_1c;
  piVar2 = (int *)0x0;
  piVar8 = local_14;
  for (puVar3 = (undefined4 *)*puVar4; local_14 = piVar2, puVar3 != puVar4;
      puVar3 = (undefined4 *)*puVar3) {
    local_18 = (int *)puVar3[6];
    local_1c = (int *)((int)local_14 + 1);
    FUN_011b48c0(&local_14,&local_18,puVar3 + 4);
    piVar2 = local_1c;
    piVar8 = local_14;
  }
  if (*(int *)(param_1 + 0x84) != 0) {
    *(undefined4 *)(*(int *)(param_1 + 0x84) + 0x80) = 0;
    local_14 = piVar8;
    FUN_0122afb0();
  }
  ExceptionList = local_10;
  return;
}


