
/* ============================================================ */
/* Function: FUN_012faf80 */
/* Entry: 012faf80 */
/* Signature: undefined FUN_012faf80() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_012faf80(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,short *param_6)

{
  short sVar1;
  uint uVar2;
  int *piVar3;
  undefined4 *puVar4;
  short *psVar5;
  short *psVar6;
  float fVar7;
  uint in_stack_0000002c;
  float local_58;
  undefined4 *local_54;
  undefined8 local_50;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_019a42f5;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_8 = 0;
  local_14 = uVar2;
  piVar3 = (int *)FUN_01226f60(s_NewInventory_new_inventory_btn_t_01aedbf4,param_2,param_3,param_4,
                               param_5,0,0x17);
  if (piVar3 != (int *)0x0) {
    local_34 = 0;
    local_30 = 7;
    uStack_40 = 0;
    uStack_3c = 0;
    uStack_38 = 0;
    local_44 = 0;
    local_18 = 0;
    local_8 = CONCAT31(local_8._1_3_,1);
    local_24 = DAT_01bfe468 + 8;
    psVar6 = (short *)&param_6;
    if (7 < in_stack_0000002c) {
      psVar6 = param_6;
    }
    local_20 = 9;
    local_50 = DAT_01bb9b2c;
    local_48 = DAT_01bb9b34;
    local_2c = 1;
    local_1c = 0;
    local_28 = 0;
    psVar5 = psVar6;
    do {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar6,(int)psVar5 - (int)(psVar6 + 1) >> 1);
    local_28 = 1;
    FUN_01226310(&local_50,DAT_01bee88c,DAT_01bee890);
    (**(code **)(*piVar3 + 0x18))(1,uVar2);
    puVar4 = operator_new(8);
    *puVar4 = &DAT_01aedc44;
    puVar4[1] = param_1;
    local_54 = puVar4;
    if ((undefined4 *)piVar3[0xd] != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)piVar3[0xd])(1);
    }
    piVar3[0xd] = (int)puVar4;
    FUN_0124eff0(&local_58);
    local_58 = local_58 * DAT_01b024f4;
    piVar3[0x3b] = 0x3f800000;
    fVar7 = (float)local_54 * _DAT_01b028e4;
    local_24 = 0;
    piVar3[0x39] = (int)(local_58 + local_58);
    piVar3[0x3c] = (int)(fVar7 * DAT_01b02858);
    piVar3[0x3a] = (int)(fVar7 + fVar7);
    FUN_00f1a330();
  }
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


