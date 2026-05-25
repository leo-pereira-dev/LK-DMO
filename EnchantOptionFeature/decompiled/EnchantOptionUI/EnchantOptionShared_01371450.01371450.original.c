
/* ============================================================ */
/* Function: FUN_01371450 */
/* Entry: 01371450 */
/* Signature: undefined FUN_01371450() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01371450(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined8 local_50;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019abb85;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_14 = uVar1;
  if (param_2 < 3) {
    iVar2 = FUN_012462c0(s_System_Btn_Radio5_tga_01adb7f4,0x17,1,param_2 * 0x82 + 8,0x195,0x6e,0x20,
                         0,0x1d);
    *(int *)(param_1 + 0x110 + param_2 * 4) = iVar2;
    if (iVar2 == 0) {
      ExceptionList = local_10;
      return;
    }
    local_8 = 0;
  }
  else {
    iVar2 = FUN_012462c0(s_System_Btn_Radio5_tga_01adb7f4,0x17,1,param_2 * 0x82 + -0x17e,0x1bd,0x6e,
                         0x20,0,0x1d);
    *(int *)(param_1 + 0x110 + param_2 * 4) = iVar2;
    if (iVar2 == 0) {
      ExceptionList = local_10;
      return;
    }
    local_8 = 1;
  }
  local_18 = 0;
  local_30 = 7;
  local_34 = 0;
  uStack_38 = 0;
  uStack_3c = 0;
  uStack_40 = 0;
  local_44 = 0;
  local_24 = DAT_01bfe468 + 8;
  local_48 = DAT_01bb9b34;
  local_20 = 0xc;
  local_50 = DAT_01bb9b2c;
  local_2c = 1;
  local_1c = 0;
  local_28 = 1;
  FUN_01224f40(&local_50);
  iVar2 = *(int *)(param_1 + 0x110 + param_2 * 4);
  puVar3 = operator_new(8);
  *puVar3 = &DAT_01af3328;
  puVar3[1] = param_2;
  if (*(undefined4 **)(iVar2 + 0x34) != (undefined4 *)0x0) {
    (**(code **)**(undefined4 **)(iVar2 + 0x34))(1,uVar1);
  }
  *(undefined4 **)(iVar2 + 0x34) = puVar3;
  FUN_01371a00(iVar2,param_1,FUN_013715f0,0);
  local_24 = 0;
  FUN_00f1a330();
  ExceptionList = local_10;
  return;
}


