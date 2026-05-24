
/* ============================================================ */
/* Function: FUN_01054c10 */
/* Entry: 01054c10 */
/* Signature: undefined FUN_01054c10() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 FUN_01054c10(int *param_1)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 extraout_ECX;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 local_f0 [24];
  undefined1 local_d8 [24];
  undefined4 local_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined1 local_99;
  undefined4 local_98 [7];
  undefined4 local_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined8 local_6c;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019788be;
  local_1c = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_a4 = 0;
  local_a8 = 2;
  local_c0 = 0;
  uStack_bc = 0;
  uStack_b8 = 0;
  uStack_b4 = 0;
  local_b0 = 0;
  local_ac = 0;
  local_24 = uVar1;
  FUN_00f1a200(&DAT_01ac5dd8,0);
  local_14 = 0;
  uVar2 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_MESSAGE_BOX_CAPTION_NOTICE_01ace3c0,0x1a);
  uVar2 = FUN_015ce480(local_d8,uVar2,uVar4,uVar5,uVar6,uVar7,uVar8);
  local_14._0_1_ = 1;
  local_a0 = 0;
  FUN_01003fa0(&local_a0,uVar2,&local_c0,&local_a8,&local_a4,extraout_ECX);
  FUN_00f1a330(uVar1);
  FUN_00f1a330();
  local_2c = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_28 = 0xf;
  local_3c = 0;
  local_14 = CONCAT31(local_14._1_3_,5);
  if (*param_1 == 0) {
    local_99 = 1;
  }
  else {
    if (*param_1 == 1) {
      FUN_00f19f10(s_FAILURE_EXTRA_INVENTORY_01ad0cd8,0x17);
      local_98[0] = FUN_01504150(&local_3c);
      puVar3 = (undefined4 *)FUN_015ce180(local_f0,&local_3c);
      if (&local_7c != puVar3) {
        FUN_00f1a330();
        local_7c = *puVar3;
        uStack_78 = puVar3[1];
        uStack_74 = puVar3[2];
        uStack_70 = puVar3[3];
        local_6c = *(undefined8 *)(puVar3 + 4);
        puVar3[4] = 0;
        puVar3[5] = 7;
        *(undefined2 *)puVar3 = 0;
      }
      FUN_00f1a330();
      FUN_01441e20(local_98);
    }
    local_99 = 0;
  }
  FUN_00f1a420();
  FUN_00f1e050();
  FUN_00f1a330();
  FUN_00f1a330();
  ExceptionList = local_1c;
  return local_99;
}


