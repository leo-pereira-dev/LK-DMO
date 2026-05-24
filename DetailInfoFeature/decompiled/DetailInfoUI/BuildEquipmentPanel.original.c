
/* ============================================================ */
/* Function: FUN_011d61d0 */
/* Entry: 011d61d0 */
/* Signature: undefined FUN_011d61d0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_011d61d0(undefined4 param_1)

{
  uint uVar1;
  undefined4 ***pppuVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 **local_2c [5];
  uint local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01984dad;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar1;
  FUN_012459a0(param_1,s_DetailInfo_DetailInfo_bg_b_tga_01ae2e20,1,0,0,1,0x1cf,0,0x157,0x223);
  uVar3 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  FUN_00f19cc0(s_DETAIL_INFO_EQUIP_TOP_01ae2e08,0x15);
  FUN_015ce480(local_2c,uVar3,uVar4,uVar5,uVar6,uVar7);
  local_8 = 0;
  pppuVar2 = local_2c;
  if (7 < local_18) {
    pppuVar2 = (undefined4 ***)local_2c[0];
  }
  FUN_012461a0(pppuVar2,0xb,CONCAT44(DAT_01b02744,DAT_01b02744),&DAT_3f7ae148,DAT_01bee88c,
               DAT_01bee890);
  FUN_011d6580(uVar1);
  FUN_011d65d0();
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


