
/* ============================================================ */
/* Function: FUN_0167c050 */
/* Entry: 0167c050 */
/* Signature: undefined FUN_0167c050() */
/* ============================================================ */


void FUN_0167c050(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  wchar_t *pwVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  uint uVar7;
  wchar_t *pwVar8;
  wchar_t *local_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 local_24;
  uint local_20;
  undefined4 *local_1c;
  int local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019cc88c;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_14 = 0x27;
  local_34 = (wchar_t *)0x0;
  uStack_30 = 0;
  uStack_2c = 0;
  uStack_28 = 0;
  pwVar4 = (wchar_t *)FUN_00f1a1e0(&local_34,&local_14,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  uVar3 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._12_4_;
  uVar2 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._8_4_;
  uVar1 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._4_4_;
  local_24 = 0x21;
  local_20 = local_14;
  *(undefined4 *)pwVar4 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._0_4_;
  *(undefined4 *)(pwVar4 + 2) = uVar1;
  *(undefined4 *)(pwVar4 + 4) = uVar2;
  *(undefined4 *)(pwVar4 + 6) = uVar3;
  uVar3 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._28_4_;
  uVar2 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._24_4_;
  uVar1 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._20_4_;
  *(undefined4 *)(pwVar4 + 8) = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._16_4_;
  *(undefined4 *)(pwVar4 + 10) = uVar1;
  *(undefined4 *)(pwVar4 + 0xc) = uVar2;
  *(undefined4 *)(pwVar4 + 0xe) = uVar3;
  uVar3 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._44_4_;
  uVar2 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._40_4_;
  uVar1 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._36_4_;
  *(undefined4 *)(pwVar4 + 0x10) = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._32_4_;
  *(undefined4 *)(pwVar4 + 0x12) = uVar1;
  *(undefined4 *)(pwVar4 + 0x14) = uVar2;
  *(undefined4 *)(pwVar4 + 0x16) = uVar3;
  uVar3 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._60_4_;
  uVar2 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._56_4_;
  uVar1 = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._52_4_;
  *(undefined4 *)(pwVar4 + 0x18) = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580._48_4_;
  *(undefined4 *)(pwVar4 + 0x1a) = uVar1;
  *(undefined4 *)(pwVar4 + 0x1c) = uVar2;
  *(undefined4 *)(pwVar4 + 0x1e) = uVar3;
  pwVar4[0x20] = u_DM_Extra_Inven_EvoExInven_Str_bi_01a1d580[0x20];
  pwVar4[0x21] = L'\0';
  local_8 = 0;
  local_34 = pwVar4;
  puVar5 = operator_new(0x40);
  local_1c = puVar5;
  if (puVar5 == (undefined4 *)0x0) {
    puVar5 = (undefined4 *)0x0;
  }
  else {
    memset(puVar5,0,0x40);
    puVar5[1] = local_18;
    local_8 = CONCAT31(local_8._1_3_,2);
    *puVar5 = &DAT_01a1d5c8;
    puVar5[2] = FUN_0167c210;
    puVar5[3] = 0;
    puVar5[4] = 0;
    puVar5[5] = 0;
    puVar5[6] = 0;
    puVar5[7] = 0;
    FUN_00f221a0(&local_34);
    puVar5[0xe] = 0;
    local_14 = local_20;
    pwVar4 = local_34;
  }
  uVar7 = local_8;
  local_8 = local_8 & 0xffffff00;
  if (puVar5 != (undefined4 *)0x0) {
    local_1c = *(undefined4 **)(local_18 + 4);
    if (*(int *)(local_18 + 8) == 0x15555555) {
                    /* WARNING: Subroutine does not return */
      std::_Xlength_error(s_list_too_long_01abdaa4);
    }
    local_8._1_3_ = SUB43(uVar7,1);
    local_8 = CONCAT31(local_8._1_3_,3);
    puVar6 = operator_new(0xc);
    puVar6[2] = puVar5;
    *(int *)(local_18 + 8) = *(int *)(local_18 + 8) + 1;
    puVar5 = (undefined4 *)local_1c[1];
    *puVar6 = local_1c;
    puVar6[1] = puVar5;
    local_1c[1] = puVar6;
    *puVar5 = puVar6;
  }
  if (7 < local_14) {
    uVar7 = local_14 * 2 + 2;
    pwVar8 = pwVar4;
    if (0xfff < uVar7) {
      pwVar8 = *(wchar_t **)(pwVar4 + -2);
      uVar7 = local_14 * 2 + 0x25;
      if (0x1f < (uint)((int)pwVar4 + (-4 - (int)pwVar8))) {
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
    }
    FUN_017986fc(pwVar8,uVar7);
  }
  ExceptionList = local_10;
  return;
}


