
/* ============================================================ */
/* Function: FUN_01267ef0 */
/* Entry: 01267ef0 */
/* Signature: undefined FUN_01267ef0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01267ef0(int param_1)

{
  short sVar1;
  short *psVar2;
  short ****ppppsVar3;
  short ****ppppsVar4;
  int iVar5;
  int iVar6;
  undefined1 local_d8 [24];
  void *local_c0;
  void *local_bc;
  undefined1 *local_b8;
  short *local_b4;
  int local_b0;
  int local_ac;
  short *local_a8;
  undefined8 local_a4;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined1 local_80;
  undefined4 local_7c;
  int local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined8 local_68;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined1 local_44;
  undefined4 local_40;
  int local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  short ***local_2c;
  short **ppsStack_28;
  short **ppsStack_24;
  short **ppsStack_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199b6a4;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  *(undefined1 *)(param_1 + 0x68) = 1;
  local_b0 = param_1;
  if (0 < *(int *)(param_1 + 0xc4)) {
    FUN_01232880(1);
  }
  if (0 < *(int *)(param_1 + 0xe4)) {
    FUN_01232880(1);
  }
  iVar6 = *(int *)(param_1 + 0xa8);
  if ((((*(int *)(iVar6 + 0xfd) != 0) || (*(short *)(iVar6 + 0x101) != 0)) &&
      (*(int *)(iVar6 + 0x150) != 0x7fffffff)) && (iVar6 + 0xfd != 0)) {
    local_b8 = &stack0x00000004 + -(iVar6 + 0xfd);
    local_ac = 0x32;
    local_b4 = (short *)&stack0x00000019;
    iVar6 = iVar6 + 0x122;
    iVar5 = local_b0;
    do {
      if ((*(short *)(iVar6 + -0x10) != 0) && (*local_b4 != 0)) {
        FUN_0125ffe0(&local_2c,*(short *)(iVar6 + -0x10));
        local_8 = 0;
        local_bc = (void *)FUN_016830c0(0x2c);
        local_8._0_1_ = 1;
        if (local_bc == (void *)0x0) {
          psVar2 = (short *)0x0;
        }
        else {
          memset(local_bc,0,0x2c);
          psVar2 = (short *)FUN_010baa10();
        }
        local_88 = 0;
        local_84 = 7;
        uStack_94 = 0;
        uStack_90 = 0;
        uStack_8c = 0;
        local_98 = 0;
        local_6c = 0;
        local_8 = CONCAT31(local_8._1_3_,2);
        local_78 = DAT_01bfe468 + 8;
        ppppsVar3 = &local_2c;
        if (7 < local_1c._4_4_) {
          ppppsVar3 = (short ****)local_2c;
        }
        local_9c = DAT_01bb9b34;
        local_74 = 9;
        local_a4 = DAT_01bb9b2c;
        local_a8 = (short *)((int)ppppsVar3 + 2);
        local_80 = 1;
        local_70 = 0;
        local_7c = 1;
        ppppsVar4 = ppppsVar3;
        do {
          sVar1 = *(short *)ppppsVar4;
          ppppsVar4 = (short ****)((int)ppppsVar4 + 2);
        } while (sVar1 != 0);
        FUN_00f1a040(ppppsVar3,(int)ppppsVar4 - (int)local_a8 >> 1);
        FUN_01230c40(&local_a4,0x15e,local_ac);
        local_a8 = psVar2;
        FUN_00f58160(*(undefined4 *)(local_b0 + 0xe0),&local_a8);
        ppppsVar3 = (short ****)FUN_0125ffe0(local_d8,*local_b4);
        if (&local_2c != ppppsVar3) {
          FUN_00f1a330();
          local_2c = *ppppsVar3;
          ppsStack_28 = (short **)ppppsVar3[1];
          ppsStack_24 = (short **)ppppsVar3[2];
          ppsStack_20 = (short **)ppppsVar3[3];
          local_1c = *(undefined8 *)(ppppsVar3 + 4);
          ppppsVar3[4] = (short ***)0x0;
          ppppsVar3[5] = (short ***)0x7;
          *(undefined2 *)ppppsVar3 = 0;
        }
        FUN_00f1a330();
        local_c0 = (void *)FUN_016830c0(0x2c);
        local_8._0_1_ = 3;
        if (local_c0 == (void *)0x0) {
          psVar2 = (short *)0x0;
        }
        else {
          memset(local_c0,0,0x2c);
          psVar2 = (short *)FUN_010baa10();
        }
        local_4c = 0;
        local_48 = 7;
        uStack_58 = 0;
        uStack_54 = 0;
        uStack_50 = 0;
        local_5c = 0;
        local_30 = 0;
        local_8 = CONCAT31(local_8._1_3_,4);
        local_3c = DAT_01bfe468 + 8;
        ppppsVar3 = &local_2c;
        if (7 < local_1c._4_4_) {
          ppppsVar3 = (short ****)local_2c;
        }
        local_60 = DAT_01bb9b34;
        local_38 = 9;
        local_68 = DAT_01bb9b2c;
        local_a8 = (short *)((int)ppppsVar3 + 2);
        local_44 = 1;
        local_34 = 0;
        local_40 = 1;
        ppppsVar4 = ppppsVar3;
        do {
          sVar1 = *(short *)ppppsVar4;
          ppppsVar4 = (short ****)((int)ppppsVar4 + 2);
        } while (sVar1 != 0);
        FUN_00f1a040(ppppsVar3,(int)ppppsVar4 - (int)local_a8 >> 1);
        FUN_01230c40(&local_68,0x96,local_ac);
        iVar5 = local_b0;
        local_a8 = psVar2;
        FUN_00f58160(*(undefined4 *)(local_b0 + 0xc0),&local_a8);
        local_3c = 0;
        FUN_00f1a330();
        local_78 = 0;
        FUN_00f1a330();
        local_8 = 0xffffffff;
        FUN_00f1a330();
      }
      local_ac = local_ac + 0xd;
      local_b4 = local_b4 + 1;
      iVar6 = iVar6 + 2;
    } while (local_ac < 0x9a);
    FUN_012944f0(iVar5,1);
  }
  ExceptionList = local_10;
  return;
}


