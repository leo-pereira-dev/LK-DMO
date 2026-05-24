
/* ============================================================ */
/* Function: FUN_011be4e0 */
/* Entry: 011be4e0 */
/* Signature: undefined FUN_011be4e0() */
/* ============================================================ */


int * __thiscall FUN_011be4e0(int param_1,undefined4 *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined1 **ppuVar4;
  undefined4 *puVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  undefined1 local_7c [32];
  int local_5c;
  undefined1 *local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined1 *local_4c;
  undefined1 *puStack_48;
  undefined1 *local_44;
  undefined1 *local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined1 *local_34;
  undefined1 *puStack_30;
  undefined1 *local_2c;
  int local_28;
  int local_24;
  int *local_20;
  char local_19;
  code *local_18;
  code *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199181d;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_20 = (int *)param_3[1];
  if (*(int **)(param_1 + 0xa8) != local_20) {
    local_20 = (int *)FUN_011bdf80(&local_20);
  }
  param_3 = (int *)*param_3;
  local_14 = (code *)0x0;
  piVar8 = (int *)*param_3;
  if (piVar8 != param_3) {
    local_20 = (int *)(param_1 + 0xa4);
    do {
      local_18 = local_14;
      FUN_00f1d720();
      iVar6 = local_5c;
      if (((*(char *)(local_5c + 0xd) == '\0') && (*(int *)(local_5c + 0x10) <= (int)local_18)) &&
         (local_5c != *local_20)) {
        local_19 = (char)piVar8[0xd];
        iVar1 = *(int *)(local_5c + 0x18);
        iVar2 = piVar8[6];
        if (iVar1 != 0) {
          *(undefined4 *)(iVar1 + 0x18) = 10;
          *(int *)(iVar1 + 0x1c) = iVar2;
          *(undefined4 *)(iVar1 + 0x20) = 1;
          *(undefined4 *)(iVar1 + 0x24) = 0;
        }
        if (*(int **)(local_5c + 0x20) != (int *)0x0) {
          local_18 = *(code **)(**(int **)(local_5c + 0x20) + 0x1c);
          if (local_19 == '\0') {
            local_40 = &DAT_3f7ae148;
            ppuVar4 = &local_40;
            local_3c = 0x3dcccccd;
            local_38 = 0x3dcccccd;
          }
          else {
            local_34 = &DAT_3f7ae148;
            ppuVar4 = &local_34;
            puStack_30 = &DAT_3f7ae148;
            local_2c = &DAT_3f7ae148;
          }
          (*local_18)(*(undefined8 *)ppuVar4,ppuVar4[2],uVar3);
          piVar7 = piVar8 + 7;
          if (7 < (uint)piVar8[0xc]) {
            piVar7 = (int *)*piVar7;
          }
          (**(code **)(*(int *)(*(int *)(iVar6 + 0x20) + 0x14) + 0x10))(piVar7);
        }
        if (*(int **)(iVar6 + 0x1c) != (int *)0x0) {
          local_18 = *(code **)(**(int **)(iVar6 + 0x1c) + 0x1c);
          if (local_19 == '\0') {
            local_58 = &DAT_3f7ae148;
            ppuVar4 = &local_58;
            local_54 = 0x3dcccccd;
            local_50 = 0x3dcccccd;
          }
          else {
            local_4c = &DAT_3f7ae148;
            ppuVar4 = &local_4c;
            puStack_48 = &DAT_3f7ae148;
            local_44 = &DAT_3f7ae148;
          }
          (*local_18)(*(undefined8 *)ppuVar4,ppuVar4[2]);
          local_18 = *(code **)(iVar6 + 0x1c);
          local_24 = piVar8[5];
          local_28 = piVar8[4];
          puVar5 = (undefined4 *)FUN_015027f0(local_7c,&local_28);
          local_8 = 0;
          if (7 < (uint)puVar5[5]) {
            puVar5 = (undefined4 *)*puVar5;
          }
          (**(code **)(*(int *)(local_18 + 0x14) + 0x10))(puVar5);
          local_8 = 0xffffffff;
          FUN_00f1a330();
        }
        if (((*(int *)(iVar6 + 0x14) != 0) &&
            (iVar6 = *(int *)(*(int *)(iVar6 + 0x14) + 0x24), iVar6 != 0)) &&
           (iVar6 = __RTDynamicCast(*(undefined4 *)(iVar6 + 0x34),0,&DAT_01bc4a94,&DAT_01bc6088,0),
           iVar6 != 0)) {
          *(undefined4 *)(iVar6 + 4) = *param_2;
          *(code **)(iVar6 + 8) = local_14;
        }
      }
      piVar8 = (int *)*piVar8;
      local_14 = local_14 + 1;
    } while (piVar8 != param_3);
  }
  ExceptionList = local_10;
  return local_20;
}


