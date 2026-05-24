
/* ============================================================ */
/* Function: FUN_011bc7d0 */
/* Entry: 011bc7d0 */
/* Signature: undefined FUN_011bc7d0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011bc7d0(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  void *pvVar4;
  undefined4 *puVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined1 local_6c [28];
  undefined1 local_50 [8];
  int local_48;
  int local_44;
  int *local_40;
  int *local_3c;
  int *local_38;
  int local_34;
  undefined1 local_2d;
  undefined8 local_2c;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  int local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01991645;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_2c = 0;
  local_24 = uVar3;
  pvVar4 = operator_new(0x4c);
  *(void **)pvVar4 = pvVar4;
  *(void **)((int)pvVar4 + 4) = pvVar4;
  local_2c = CONCAT44(local_2c._4_4_,pvVar4);
  local_14 = 0;
  FUN_0102d940(param_2,param_3,&local_2c);
  local_40 = (int *)local_2c;
  piVar8 = (int *)*(int *)local_2c;
  if (piVar8 != (int *)local_2c) {
    local_38 = (int *)(param_1 + 0x7c);
    iVar6 = 0;
    do {
      local_44 = iVar6 + 1;
      local_34 = iVar6;
      FUN_00f1d720(local_50,&local_34);
      iVar2 = local_48;
      if (((*(char *)(local_48 + 0xd) != '\0') || (iVar6 < *(int *)(local_48 + 0x10))) ||
         (local_48 == *local_38)) break;
      piVar7 = (int *)(local_48 + 0x14);
      local_2d = 0;
      local_3c = piVar7;
      FUN_01160050(&local_2d);
      if (piVar8[3] == 0) {
        if ((int *)*piVar7 != (int *)0x0) {
          (**(code **)(*(int *)*piVar7 + 0x40))(1);
        }
        if (*(int **)(iVar2 + 0x20) != (int *)0x0) {
          (**(code **)(**(int **)(iVar2 + 0x20) + 0x40))(1);
        }
        if (*(int **)(iVar2 + 0x24) != (int *)0x0) {
          (**(code **)(**(int **)(iVar2 + 0x24) + 0x40))(1);
          local_34 = *(int *)(iVar2 + 0x24);
          uVar9 = 0;
          uVar10 = 0;
          uVar11 = 0;
          uVar12 = 0;
          uVar13 = 0;
          uVar14 = 0;
          FUN_00f19cc0(s_DIGI_SUMMON_TXT_READY_01ae1fcc,0x15);
          puVar5 = (undefined4 *)FUN_015ce480(local_6c,uVar9,uVar10,uVar11,uVar12,uVar13,uVar14);
          local_14._0_1_ = 1;
          if (7 < (uint)puVar5[5]) {
            puVar5 = (undefined4 *)*puVar5;
          }
          (**(code **)(*(int *)(local_34 + 0x14) + 0x10))(puVar5);
          local_14 = (uint)local_14._1_3_ << 8;
          FUN_00f1a330();
          piVar7 = local_3c;
        }
        if ((piVar7[9] != 0) &&
           (iVar6 = __RTDynamicCast(*(undefined4 *)(piVar7[9] + 8),0,&DAT_01bc4a94,&DAT_01bc6058,0),
           iVar6 != 0)) {
          *(undefined4 *)(iVar6 + 4) = 0;
          *(undefined1 *)(iVar6 + 8) = 0;
        }
      }
      else {
        if ((*(int *)(iVar2 + 0x38) != 0) &&
           (iVar6 = __RTDynamicCast(*(undefined4 *)(*(int *)(iVar2 + 0x38) + 8),0,&DAT_01bc4a94,
                                    &DAT_01bc6058,0), iVar6 != 0)) {
          *(int *)(iVar6 + 4) = piVar8[3];
          *(char *)(iVar6 + 8) = (char)piVar8[4];
        }
        if ((int *)*piVar7 != (int *)0x0) {
          (**(code **)(*(int *)*piVar7 + 0x40))(1);
        }
        if (*(int **)(iVar2 + 0x18) != (int *)0x0) {
          (**(code **)(**(int **)(iVar2 + 0x18) + 0x40))(1);
          piVar7 = piVar8 + 0xb;
          if (0xf < (uint)piVar8[0x10]) {
            piVar7 = (int *)*piVar7;
          }
          (**(code **)(**(int **)(iVar2 + 0x18) + 0x58))(piVar7,1);
        }
        if (*(int **)(local_48 + 0x1c) != (int *)0x0) {
          (**(code **)(**(int **)(local_48 + 0x1c) + 0x40))(1);
          piVar7 = piVar8 + 5;
          if (7 < (uint)piVar8[10]) {
            piVar7 = (int *)*piVar7;
          }
          (**(code **)(*(int *)(*(int *)(local_48 + 0x1c) + 0x14) + 0x10))(piVar7);
        }
        if (*(int **)(local_48 + 0x28) != (int *)0x0) {
          (**(code **)(**(int **)(local_48 + 0x28) + 0x40))(1);
        }
        if (*(int **)(local_48 + 0x2c) != (int *)0x0) {
          (**(code **)(**(int **)(local_48 + 0x2c) + 0x40))(1);
        }
        FUN_011bd690(piVar8 + 3,piVar8 + 0x11);
      }
      piVar8 = (int *)*piVar8;
      iVar6 = local_44;
    } while (piVar8 != local_40);
  }
  *(undefined4 *)((int *)local_2c)[1] = 0;
  puVar5 = (undefined4 *)*(int *)local_2c;
  while (puVar5 != (undefined4 *)0x0) {
    puVar1 = (undefined4 *)*puVar5;
    FUN_0102fd10(puVar5 + 0x11);
    FUN_00f1a420();
    FUN_00f1a330();
    FUN_017986fc(puVar5,0x4c);
    puVar5 = puVar1;
  }
  FUN_017986fc((int *)local_2c,0x4c,uVar3);
  ExceptionList = local_1c;
  return;
}


