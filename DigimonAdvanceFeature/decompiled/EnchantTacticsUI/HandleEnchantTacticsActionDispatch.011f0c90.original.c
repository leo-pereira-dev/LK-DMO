
/* ============================================================ */
/* Function: FUN_011f0c90 */
/* Entry: 011f0c90 */
/* Signature: undefined FUN_011f0c90() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall
FUN_011f0c90(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6,undefined4 param_7)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined1 local_2c [24];
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019841ad;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  *(undefined4 *)(param_1 + 0x88) = 0x20;
  local_14 = uVar1;
  FUN_012459a0(0,s_Breakthrough_Breakthrough_Digimo_01ae4040,param_7,6,5,0,param_3,param_4,param_5,
               param_6);
  uVar2 = 0;
  uVar3 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_BREAKTHROUGH_DIGIMON_INFO_01ae4024,0x19);
  FUN_015ce480(local_2c,uVar2,uVar3,uVar4,uVar5,uVar6,uVar7);
  local_8 = 0;
  FUN_011f0d50(uVar1);
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


