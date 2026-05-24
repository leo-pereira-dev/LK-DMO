
/* ============================================================ */
/* Function: FUN_012af150 */
/* Entry: 012af150 */
/* Signature: undefined FUN_012af150() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 __thiscall FUN_012af150(int *param_1,int *param_2,uint *param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int *piVar5;
  undefined4 uVar6;
  undefined4 ****ppppuVar7;
  undefined4 extraout_ECX;
  uint _Size;
  int iVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined1 local_dc [24];
  undefined4 local_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  int *local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  int *local_a0;
  undefined1 local_99;
  undefined4 local_98 [7];
  undefined4 ***local_7c;
  undefined4 **ppuStack_78;
  undefined4 **ppuStack_74;
  undefined4 **ppuStack_70;
  undefined8 local_6c;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_01974aae;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  uStack_3c = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  local_a0 = param_2;
  local_30 = 0;
  local_2c = 0xf;
  local_40 = 0;
  local_14 = 0;
  iVar8 = 0;
  local_28 = 0;
  local_ac = param_1;
  local_24 = uVar3;
  switch(*param_2) {
  case 0:
  case 1:
    local_99 = 1;
    puStack_20 = &stack0xfffffffc;
    break;
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
    FUN_00f19f10(s_DATA_TRANSFER_ERROR_CODE_01aeb0dc,0x18);
    if ((param_3[2] != param_3[3]) && (uVar1 = *param_3, uVar1 < param_3[1])) {
      uVar4 = param_3[3] - param_3[2];
      _Size = uVar4 - uVar1;
      if (3 < _Size) {
        if (uVar4 <= uVar1) {
                    /* WARNING: Subroutine does not return */
          FUN_00f3fcb0();
        }
        if ((int *)(param_3[2] + uVar1) == (int *)0x0) {
          memset(&local_28,0,_Size);
          piVar5 = _errno();
          *piVar5 = 0x16;
          _invalid_parameter_noinfo();
          iVar8 = local_28;
        }
        else {
          iVar8 = *(int *)(param_3[2] + uVar1);
        }
        *param_3 = *param_3 + 4;
      }
    }
    piVar5 = local_a0;
    local_a0 = (int *)0x0;
    local_a8 = 2;
    iVar2 = *piVar5;
    local_c4 = 0;
    uStack_c0 = 0;
    uStack_bc = 0;
    uStack_b8 = 0;
    local_b4 = 0;
    local_b0 = 0;
    FUN_00f1a200(&DAT_01ac5dd8,0);
    local_14._0_1_ = 1;
    uVar6 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    FUN_00f19cc0(s_MESSAGE_BOX_CAPTION_NOTICE_01ace3c0,0x1a);
    uVar6 = FUN_015ce480(local_dc,uVar6,uVar9,uVar10,uVar11,uVar12,uVar13);
    local_14._0_1_ = 2;
    local_a4 = 0;
    FUN_01003fa0(&local_a4,uVar6,&local_c4,&local_a8,&local_a0,extraout_ECX);
    FUN_00f1a330();
    local_14 = CONCAT31(local_14._1_3_,5);
    FUN_00f1a330();
    local_98[0] = FUN_01504150(&local_40);
    ppppuVar7 = (undefined4 ****)FUN_015ce180(&local_c4,&local_40);
    if (&local_7c != ppppuVar7) {
      FUN_00f1a330();
      local_7c = *ppppuVar7;
      ppuStack_78 = ppppuVar7[1];
      ppuStack_74 = ppppuVar7[2];
      ppuStack_70 = ppppuVar7[3];
      local_6c = *(undefined8 *)(ppppuVar7 + 4);
      ppppuVar7[4] = (undefined4 ***)0x0;
      ppppuVar7[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar7 = 0;
    }
    FUN_00f1a330();
    ppppuVar7 = &local_7c;
    if (7 < local_6c._4_4_) {
      ppppuVar7 = (undefined4 ****)local_7c;
    }
    FUN_01500f80(&local_7c,ppppuVar7,iVar8 * 100 + iVar2);
    (**(code **)(*local_ac + 0x14))(local_98);
    local_99 = 0;
    FUN_00f1e050();
    FUN_00f1a330();
    FUN_00f1a330();
    break;
  default:
    local_99 = 0;
    puStack_20 = &stack0xfffffffc;
  }
  FUN_00f1a420(uVar3);
  ExceptionList = local_1c;
  return local_99;
}


