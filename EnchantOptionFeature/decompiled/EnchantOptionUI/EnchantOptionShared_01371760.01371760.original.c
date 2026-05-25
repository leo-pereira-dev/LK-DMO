
/* ============================================================ */
/* Function: FUN_01371760 */
/* Entry: 01371760 */
/* Signature: undefined FUN_01371760() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01371760(int param_1,int *param_2)

{
  int iVar1;
  uint uVar2;
  int local_7c;
  int local_78;
  int local_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  int iStack_68;
  int iStack_64;
  uint uStack_60;
  undefined4 local_4c;
  undefined4 local_48;
  undefined1 *local_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  int local_20 [4];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_019abbd5;
  local_10 = ExceptionList;
  uStack_60 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if (*param_2 == 0) {
    local_30 = 0;
    local_2c = 7;
    uStack_3c = 0;
    uStack_38 = 0;
    uStack_34 = 0;
    local_40 = 0;
    local_20[0] = 0;
    local_20[1] = 0;
    local_20[2] = 0;
    local_8 = 1;
    iVar1 = *(int *)(param_1 + 4);
    local_4c = *(undefined4 *)(iVar1 + 0xc);
    local_48 = *(undefined4 *)(iVar1 + 0x10);
    local_28 = *(undefined4 *)(iVar1 + 0x14);
    local_24 = *(undefined4 *)(iVar1 + 0xc05c);
    iStack_68 = iVar1 + 0xc060;
    local_20[3] = uStack_60;
    if (&local_40 != (undefined4 *)iStack_68) {
      iStack_64 = *(undefined4 *)(iVar1 + 0xc070);
      if (7 < *(uint *)(iVar1 + 0xc074)) {
        iStack_68 = *(int *)iStack_68;
      }
      uStack_6c = 0x137180b;
      FUN_00f1a040();
    }
    if (local_20 != (int *)(iVar1 + 0xc078)) {
      iStack_68 = *(int *)(iVar1 + 0xc078);
      iStack_64 = *(int *)(iVar1 + 0xc07c) - iStack_68 >> 5;
      uStack_6c = 0x137182c;
      FUN_0136fd10();
    }
    local_70 = *(undefined4 *)(iVar1 + 0xc084);
    uStack_6c = *(undefined4 *)(iVar1 + 0xc088);
    iStack_68 = *(undefined4 *)(iVar1 + 0xc08c);
    iStack_64 = *(undefined4 *)(iVar1 + 0xc090);
    local_7c = 0;
    local_78 = 0;
    local_74 = 0;
    uVar2 = local_20[1] - local_20[0] >> 5;
    if (uVar2 != 0) {
      if (0x7ffffff < uVar2) {
                    /* WARNING: Subroutine does not return */
        FUN_00f19c70();
      }
      local_44 = (undefined1 *)uVar2;
      local_7c = FUN_00fce380();
      local_74 = uVar2 * 0x20 + local_7c;
      local_8 = CONCAT31(local_8._1_3_,2);
      local_78 = local_7c;
      local_44 = (undefined1 *)&local_7c;
      local_78 = FUN_00fce3d0(local_7c,&local_7c);
    }
    local_8._0_1_ = 3;
    FUN_00f221a0(&local_40);
    local_8 = CONCAT31(local_8._1_3_,1);
    FUN_01370c90(&local_4c,&local_48,&local_28,&local_24);
    iStack_64 = 0x13718e1;
    FUN_00fcec10();
    iStack_64 = 0x13718e9;
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


