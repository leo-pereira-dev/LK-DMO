
/* ============================================================ */
/* Function: FUN_012b5c70 */
/* Entry: 012b5c70 */
/* Signature: undefined FUN_012b5c70() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_012b5c70(int *param_1,undefined4 *param_2)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  short *psVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  uint in_stack_00000018;
  char in_stack_0000001c;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined1 local_d8 [24];
  undefined4 local_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  short *local_8c;
  undefined4 local_88;
  undefined1 local_84 [88];
  undefined1 local_2c [24];
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_019a05d4;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_8 = 0;
  local_14 = uVar2;
  if (DAT_01bfb2b4 != -0x28) {
    if (in_stack_0000001c == '\0') {
      local_88 = 0x15;
      FUN_00ffcb30(&local_88,&DAT_01bfe980);
      local_88 = 0x16;
      FUN_00ffcb30(&local_88,&DAT_01bfe980);
    }
    else {
      local_b0 = 0;
      local_c0 = 0;
      uStack_bc = 0;
      uStack_b8 = 0;
      uStack_b4 = 0;
      local_ac = 0;
      FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_DESTROY_REMI_01aeb1e8,0x22);
      local_8._0_1_ = 1;
      FUN_015ce180(local_2c,&local_c0);
      local_8._0_1_ = 3;
      FUN_00f1a420();
      puVar6 = &param_2;
      if (7 < in_stack_00000018) {
        puVar6 = param_2;
      }
      psVar3 = (short *)FUN_0112e960();
      local_8._0_1_ = 4;
      if (7 < *(uint *)(psVar3 + 10)) {
        psVar3 = *(short **)psVar3;
      }
      local_98 = 0;
      local_94 = 0;
      local_a8 = 0;
      uStack_a4 = 0;
      uStack_a0 = 0;
      uStack_9c = 0;
      local_8c = psVar3 + 1;
      psVar4 = psVar3;
      do {
        sVar1 = *psVar4;
        psVar4 = psVar4 + 1;
      } while (sVar1 != 0);
      FUN_00f1a200(psVar3,(int)psVar4 - (int)local_8c >> 1);
      local_8._0_1_ = 5;
      FUN_01502130(local_2c,&local_a8,puVar6);
      FUN_00f1a330();
      local_8._0_1_ = 3;
      FUN_00f1a330();
      local_8c = (short *)0x1;
      local_90 = 6;
      uVar5 = 0;
      uVar7 = 0;
      uVar8 = 0;
      uVar9 = 0;
      uVar10 = 0;
      uVar11 = 0;
      FUN_00f19cc0(s_MESSAGE_BOX_CAPTION_NOTICE_01ace3c0,0x1a);
      uVar5 = FUN_015ce480(local_d8,uVar5,uVar7,uVar8,uVar9,uVar10,uVar11);
      local_8._0_1_ = 6;
      local_98 = 0;
      local_a8 = 0;
      uStack_a4 = 0;
      uStack_a0 = 0;
      uStack_9c = 0;
      local_94 = 0;
      FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_DESTROY_REMI_01aeb1e8,0x22);
      puVar6 = &local_a8;
      local_8._0_1_ = 7;
      local_88 = FUN_01504150(puVar6);
      FUN_01003fa0(&local_88,uVar5,local_2c,&local_90,&local_8c,puVar6);
      FUN_00f1a420();
      local_8 = CONCAT31(local_8._1_3_,10);
      FUN_00f1a330();
      (**(code **)(*param_1 + 0x14))(local_84);
      FUN_00f1e050();
      FUN_00f1a330();
      FUN_00f1a330();
      FUN_00f1a330();
    }
  }
  FUN_00f1a330(uVar2);
  ExceptionList = local_10;
  return;
}


