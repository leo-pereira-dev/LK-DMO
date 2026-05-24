
/* ============================================================ */
/* Function: FUN_011fb410 */
/* Entry: 011fb410 */
/* Signature: undefined FUN_011fb410() */
/* ============================================================ */


undefined4 FUN_011fb410(undefined4 param_1,undefined4 param_2)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined1 *puVar8;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_019959c6;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_8 = 0;
  local_14 = 0;
  uVar2 = 0;
  uVar3 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_DIGICOMBINE_GRADE_01ae21d4,0x11);
  FUN_015ce480(param_1,uVar2,uVar3,uVar4,uVar5,uVar6,uVar7);
  local_8 = 0;
  local_14 = 1;
  switch(param_2) {
  case 1:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 1;
    puVar8 = &DAT_01ad9e64;
    break;
  case 2:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 2;
    puVar8 = &DAT_01ae21d0;
    break;
  case 3:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 3;
    puVar8 = &DAT_01ae21ec;
    break;
  case 4:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 4;
    puVar8 = &DAT_01ae21e8;
    break;
  case 5:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 5;
    puVar8 = &DAT_01ae21f4;
    break;
  case 6:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 6;
    puVar8 = &DAT_01ae21f0;
    break;
  case 7:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 7;
    puVar8 = &DAT_01ae21fc;
    break;
  case 8:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 8;
    puVar8 = &DAT_01ae21f8;
    break;
  case 9:
    local_20 = 0;
    local_1c = 0;
    local_30 = 0;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8 = 9;
    puVar8 = &DAT_01ae2200;
    break;
  default:
    goto switchD_011fb496_default;
  }
  FUN_015019b0(param_1,&local_30,puVar8,uVar1);
  FUN_00f1a330();
switchD_011fb496_default:
  ExceptionList = local_10;
  return param_1;
}


