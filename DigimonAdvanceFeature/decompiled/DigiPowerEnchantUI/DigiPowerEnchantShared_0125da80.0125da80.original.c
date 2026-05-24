
/* ============================================================ */
/* Function: FUN_0125da80 */
/* Entry: 0125da80 */
/* Signature: undefined FUN_0125da80() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_0125da80(int param_1,int param_2)

{
  short sVar1;
  int iVar2;
  short *psVar3;
  short *psVar4;
  undefined4 local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019841ad;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if (param_2 != 0) {
    iVar2 = __RTDynamicCast(*(undefined4 *)(param_2 + 0x34),0,&DAT_01bc4a94,&DAT_01bc691c,0,local_14
                           );
    if ((iVar2 != 0) && (iVar2 = *(int *)(param_1 + 0xac), iVar2 != 0)) {
      psVar4 = (short *)(iVar2 + 0x114);
      if (7 < *(uint *)(iVar2 + 0x128)) {
        psVar4 = *(short **)psVar4;
      }
      local_1c = 0;
      local_18 = 0;
      local_2c = 0;
      uStack_28 = 0;
      uStack_24 = 0;
      uStack_20 = 0;
      psVar3 = psVar4;
      do {
        sVar1 = *psVar3;
        psVar3 = psVar3 + 1;
      } while (sVar1 != 0);
      FUN_00f1a200(psVar4,(int)psVar3 - (int)(psVar4 + 1) >> 1);
      local_8 = 0;
      FUN_00f221a0(&local_2c);
      FUN_01325390();
      FUN_00f1a330();
    }
  }
  ExceptionList = local_10;
  return;
}


