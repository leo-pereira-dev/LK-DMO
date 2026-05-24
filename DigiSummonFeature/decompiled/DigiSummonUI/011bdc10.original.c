
/* ============================================================ */
/* Function: FUN_011bdc10 */
/* Entry: 011bdc10 */
/* Signature: undefined FUN_011bdc10() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_011bdc10(int *param_1,uint *param_2)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  void *pvVar5;
  uint _Size;
  int *piVar6;
  undefined4 ***pppuVar7;
  int iVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 local_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 local_98;
  undefined1 *local_94;
  int local_90;
  undefined8 local_8c;
  undefined1 *local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined1 local_68;
  undefined4 local_64;
  int local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 **local_50 [5];
  uint local_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  void *local_28;
  undefined4 local_24;
  void *local_20;
  void *pvStack_1c;
  void *local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  void *local_8;
  
  puStack_c = &LAB_01991783;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_28 = (void *)(*param_1 + -2);
  if (local_28 == (void *)0x0) {
    uStack_34 = 0;
    uStack_30 = 0;
    uStack_2c = 0;
    local_24 = 7;
    local_38 = 0;
    local_18 = local_28;
    local_14 = uVar4;
    local_8 = local_28;
    local_20 = local_28;
    pvStack_1c = local_28;
    pvVar5 = operator_new(0x38);
    *(void **)pvVar5 = pvVar5;
    *(void **)((int)pvVar5 + 4) = pvVar5;
    _local_20 = CONCAT44(pvStack_1c,pvVar5);
    puVar1 = param_2 + 2;
    local_8 = (void *)CONCAT31(local_8._1_3_,1);
    uVar2 = param_2[3];
    if (((*puVar1 != uVar2) && (uVar3 = *param_2, uVar3 < param_2[1])) &&
       (_Size = (uVar2 - *puVar1) - uVar3, 3 < _Size)) {
      if (uVar2 - *puVar1 <= uVar3) {
                    /* WARNING: Subroutine does not return */
        FUN_00f3fcb0(uVar4);
      }
      if ((int *)(*puVar1 + uVar3) == (int *)0x0) {
        memset(&local_18,0,_Size);
        piVar6 = _errno();
        *piVar6 = 0x16;
        _invalid_parameter_noinfo();
      }
      else {
        local_18 = *(void **)(*puVar1 + uVar3);
      }
      *param_2 = *param_2 + 4;
    }
    if ((*puVar1 != param_2[3]) && (*param_2 < param_2[1])) {
      FUN_0100f610(&local_38,puVar1,param_2);
    }
    FUN_011b5760(&local_20);
    iVar8 = local_90 + 8;
    FUN_01232880(1);
    local_70 = 0;
    local_6c = 7;
    uStack_7c = 0;
    uStack_78 = 0;
    uStack_74 = 0;
    local_80 = 0;
    local_54 = 0;
    local_8._0_1_ = 2;
    local_60 = DAT_01bfe468 + 8;
    local_8c = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_5c = 0xc;
    local_68 = 1;
    local_58 = 0;
    local_64 = 1;
    local_94 = &DAT_3f7ae148;
    local_84 = &DAT_3f7ae148;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    uVar14 = 0;
    FUN_00f19cc0(s_DIGISUMMON_BUY_DESC_01ae1ff8,0x13);
    FUN_015ce480(local_50,uVar9,uVar10,uVar11,uVar12,uVar13,uVar14);
    local_8._0_1_ = 3;
    local_98 = 0;
    local_a8 = 0;
    uStack_a4 = 0;
    uStack_a0 = 0;
    uStack_9c = 0;
    local_94 = (undefined1 *)0x0;
    FUN_00f1a200(u__value__01ae17ec,7);
    local_8._0_1_ = 4;
    FUN_01501620(local_50,&local_a8,&local_38);
    local_8._0_1_ = 3;
    FUN_00f1a330();
    pppuVar7 = local_50;
    if (7 < local_3c) {
      pppuVar7 = (undefined4 ***)local_50[0];
    }
    FUN_012543f0(iVar8,0x15e,pppuVar7,&local_8c);
    FUN_00f1a330();
    local_8 = (void *)CONCAT31(local_8._1_3_,1);
    local_60 = 0;
    FUN_00f1a330();
    FUN_011be4e0(&local_18,&local_20);
    *(undefined1 *)(local_90 + -0xc) = 1;
    FUN_0102f760();
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


