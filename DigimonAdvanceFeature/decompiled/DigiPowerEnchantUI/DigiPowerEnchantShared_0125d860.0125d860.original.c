
/* ============================================================ */
/* Function: FUN_0125d860 */
/* Entry: 0125d860 */
/* Signature: undefined FUN_0125d860() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0125d860(int param_1)

{
  short sVar1;
  uint uVar2;
  short *psVar3;
  int iVar4;
  short *psVar5;
  undefined1 local_4c [31];
  undefined1 local_2d;
  undefined4 local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01990fb5;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar4 = *(int *)(param_1 + 0xac);
  if (iVar4 != 0) {
    psVar5 = (short *)(iVar4 + 0x114);
    if (7 < *(uint *)(iVar4 + 0x128)) {
      psVar5 = *(short **)psVar5;
    }
    local_1c = 0;
    local_18 = 0;
    local_2c = 0;
    uStack_28 = 0;
    uStack_24 = 0;
    uStack_20 = 0;
    psVar3 = psVar5;
    do {
      sVar1 = *psVar3;
      psVar3 = psVar3 + 1;
    } while (sVar1 != 0);
    local_14 = uVar2;
    FUN_00f1a200(psVar5,(int)psVar3 - (int)(psVar5 + 1) >> 1);
    local_8 = 0;
    FUN_00f221a0(&local_2c);
    local_8._0_1_ = 1;
    FUN_00f221a0(local_4c);
    iVar4 = FUN_013254a0();
    if (iVar4 == 0) {
      FUN_00fab0f0(uVar2);
      local_8 = (uint)local_8._1_3_ << 8;
      FUN_00f1a330();
    }
    else {
      FUN_013bdff0(0,0,0,0);
      local_8 = (uint)local_8._1_3_ << 8;
      FUN_00f1a330();
      local_2d = 0;
      FUN_0125d660(&local_2d);
    }
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


