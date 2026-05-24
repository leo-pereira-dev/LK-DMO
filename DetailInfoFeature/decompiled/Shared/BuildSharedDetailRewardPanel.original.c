
/* ============================================================ */
/* Function: FUN_011ed0a0 */
/* Entry: 011ed0a0 */
/* Signature: undefined FUN_011ed0a0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011ed0a0(int param_1,undefined4 param_2,int param_3,int param_4)

{
  uint uVar1;
  undefined4 ***pppuVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 **local_3c [5];
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0197b59d;
  local_1c = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  *(undefined4 *)(param_1 + 0x88) = 0x1c;
  local_24 = uVar1;
  FUN_012459a0(0,s_DetailInfo_DetailInfo_bg_b_tga_01ae2e20,1,0,0,1,param_3 + -0x136,param_4 + -0x118
               ,0x26c,0x230);
  uVar3 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_UNION_REWARD_01ae3eb4,0xc);
  FUN_015ce480(local_3c,uVar3,uVar4,uVar5,uVar6,uVar7);
  local_14 = 0;
  pppuVar2 = local_3c;
  if (7 < local_28) {
    pppuVar2 = (undefined4 ***)local_3c[0];
  }
  FUN_012461a0(pppuVar2,0xd,CONCAT44(DAT_01b026f8,DAT_01b0260c),0x3f76f6f7,DAT_01bee88c,DAT_01bee890
              );
  FUN_011ed1e0(uVar1);
  FUN_011ed230();
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


