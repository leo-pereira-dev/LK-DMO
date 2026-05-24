
/* ============================================================ */
/* Function: FUN_011c1da0 */
/* Entry: 011c1da0 */
/* Signature: undefined FUN_011c1da0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011c1da0(int param_1,undefined4 param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *******pppppppuVar4;
  float10 fVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int iVar12;
  undefined1 local_68 [12];
  undefined1 local_5c [4];
  undefined4 local_58;
  int local_54;
  int local_50;
  int local_4c;
  byte local_46;
  byte local_45;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 ******local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01991bad;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  if (*(char *)(param_1 + 200) == '\0') {
    return;
  }
  if (param_3 == 0) {
    return;
  }
  ExceptionList = &local_10;
  iVar2 = __RTDynamicCast(*(undefined4 *)(param_3 + 0x34),0,&DAT_01bc4a94,&DAT_01bc6148,0,local_14);
  if (iVar2 == 0) {
    ExceptionList = local_10;
    return;
  }
  FUN_011c5f60(*(undefined1 *)(iVar2 + 4),1);
  local_4c = FUN_01028810(*(undefined1 *)(iVar2 + 4));
  iVar12 = *(int *)(param_1 + 0x78);
  local_45 = *(byte *)(iVar2 + 4);
  if (local_45 != 0) {
    local_46 = local_45;
    FUN_01012bc0(local_5c,&local_46);
    if (((*(char *)(local_54 + 0xd) == '\0') && (*(byte *)(local_54 + 0xe) <= local_45)) &&
       (local_54 != *(int *)(iVar12 + 0x50))) {
      iVar2 = (int)*(short *)(local_54 + 0x14);
      goto LAB_011c1e69;
    }
  }
  iVar2 = 0;
LAB_011c1e69:
  local_50 = iVar2;
  if (local_4c == 0) {
    (**(code **)(*(int *)(*(int *)(param_1 + 0xc4) + 0x14) + 0x1c))(0);
    (**(code **)(**(int **)(param_1 + 0xbc) + 0x1c))(0);
    (**(code **)(**(int **)(param_1 + 0xc0) + 0x1c))(0);
    (**(code **)(**(int **)(param_1 + 0xb4) + 0x1c))(0);
  }
  else {
    local_46 = local_45;
    FUN_01012bc0(local_5c,&local_46);
    if (((*(char *)(local_54 + 0xd) == '\0') && (*(byte *)(local_54 + 0xe) <= local_45)) &&
       (local_54 != *(int *)(*(int *)(param_1 + 0x78) + 0x50))) {
      local_58 = *(undefined4 *)(local_54 + 0x10);
      local_54 = CONCAT22((short)((uint)local_58 >> 0x10),*(undefined2 *)(local_54 + 0x14));
    }
    else {
      local_54 = 0;
    }
    (**(code **)(*(int *)(*(int *)(param_1 + 0xc4) + 0x14) + 0x1c))(1);
    (**(code **)(**(int **)(param_1 + 0xbc) + 0x1c))(1);
    (**(code **)(**(int **)(param_1 + 0xc0) + 0x1c))(1);
    FUN_01242bf0();
    (**(code **)(**(int **)(param_1 + 0xb4) + 0x1c))(1);
    (**(code **)(**(int **)(param_1 + 0xb4) + 0x18))(local_4c <= iVar2);
    local_34 = 0;
    local_30 = 7;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    local_44 = 0;
    local_8 = 0;
    local_2c = (undefined4 *******)0x0;
    uStack_28 = 0;
    uStack_24 = 0;
    uStack_20 = 0;
    local_1c = 0;
    local_18 = 0;
    FUN_00f1a200(&DAT_01ac5dd8,0);
    local_8._0_1_ = 1;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    iVar2 = local_50;
    iVar12 = local_4c;
    FUN_00f19cc0(s_DIGICOMBINE_CEILING_COUNT_01ae22b4,0x19);
    puVar3 = (undefined4 *)FUN_015ce480(local_68,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11);
    local_8._0_1_ = 2;
    if (7 < (uint)puVar3[5]) {
      puVar3 = (undefined4 *)*puVar3;
    }
    FUN_01500f80(&local_2c,u__s__d____d_01ad3d14,puVar3,iVar2,iVar12);
    local_8 = CONCAT31(local_8._1_3_,1);
    FUN_00f1a330();
    pppppppuVar4 = &local_2c;
    if (7 < local_18) {
      pppppppuVar4 = (undefined4 *******)local_2c;
    }
    (**(code **)(**(int **)(param_1 + 0xc4) + 0x10))(pppppppuVar4);
    piVar1 = *(int **)(param_1 + 0xc0);
    fVar5 = (float10)fmaxf(0,(float)local_4c);
    piVar1[0x32] = (int)(float)fVar5;
    FUN_01243010();
    (**(code **)(*piVar1 + 0x48))();
    FUN_01242bf0();
    FUN_00f1a330();
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


