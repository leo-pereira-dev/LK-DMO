
/* ============================================================ */
/* Function: FUN_012aee10 */
/* Entry: 012aee10 */
/* Signature: undefined FUN_012aee10() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 __thiscall FUN_012aee10(int *param_1,int *param_2,uint *param_3)

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
  undefined1 auStack_cc [24];
  undefined4 uStack_b4;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  int *local_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  int *local_90;
  undefined1 local_89;
  undefined4 auStack_88 [7];
  undefined4 ***pppuStack_6c;
  undefined4 **ppuStack_68;
  undefined4 **ppuStack_64;
  undefined4 **ppuStack_60;
  undefined8 uStack_5c;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 local_20;
  undefined4 local_1c;
  int local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_0197c9ee;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  uStack_2c = 0;
  uStack_28 = 0;
  uStack_24 = 0;
  local_90 = param_2;
  local_20 = 0;
  local_1c = 0xf;
  local_30 = 0;
  local_8 = 0;
  iVar8 = 0;
  local_18 = 0;
  local_9c = param_1;
  local_14 = uVar3;
  switch(*param_2) {
  case 0:
    local_89 = 1;
    break;
  case 1:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 0xc:
  case 0xd:
  case 0xe:
    FUN_00f19f10(s_EQUIPSYSTEM_UPGRADE_ERROR_CODE_01aeb0f8,0x1e);
    if ((param_3[2] != param_3[3]) && (uVar1 = *param_3, uVar1 < param_3[1])) {
      uVar4 = param_3[3] - param_3[2];
      _Size = uVar4 - uVar1;
      if (3 < _Size) {
        if (uVar4 <= uVar1) {
                    /* WARNING: Subroutine does not return */
          FUN_00f3fcb0();
        }
        if ((int *)(param_3[2] + uVar1) == (int *)0x0) {
          memset(&local_18,0,_Size);
          piVar5 = _errno();
          *piVar5 = 0x16;
          _invalid_parameter_noinfo();
          iVar8 = local_18;
        }
        else {
          iVar8 = *(int *)(param_3[2] + uVar1);
        }
        *param_3 = *param_3 + 4;
      }
    }
    piVar5 = local_90;
    local_90 = (int *)0x0;
    uStack_98 = 2;
    iVar2 = *piVar5;
    uStack_b4 = 0;
    uStack_b0 = 0;
    uStack_ac = 0;
    uStack_a8 = 0;
    uStack_a4 = 0;
    uStack_a0 = 0;
    FUN_00f1a200(&DAT_01ac5dd8,0);
    local_8._0_1_ = 1;
    uVar6 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    FUN_00f19cc0(s_MESSAGE_BOX_CAPTION_NOTICE_01ace3c0,0x1a);
    uVar6 = FUN_015ce480(auStack_cc,uVar6,uVar9,uVar10,uVar11,uVar12,uVar13);
    local_8._0_1_ = 2;
    uStack_94 = 0;
    FUN_01003fa0(&uStack_94,uVar6,&uStack_b4,&uStack_98,&local_90,extraout_ECX);
    FUN_00f1a330();
    local_8 = CONCAT31(local_8._1_3_,5);
    FUN_00f1a330();
    auStack_88[0] = FUN_01504150(&local_30);
    ppppuVar7 = (undefined4 ****)FUN_015ce180(&uStack_b4,&local_30);
    if (&pppuStack_6c != ppppuVar7) {
      FUN_00f1a330();
      pppuStack_6c = *ppppuVar7;
      ppuStack_68 = ppppuVar7[1];
      ppuStack_64 = ppppuVar7[2];
      ppuStack_60 = ppppuVar7[3];
      uStack_5c = *(undefined8 *)(ppppuVar7 + 4);
      ppppuVar7[4] = (undefined4 ***)0x0;
      ppppuVar7[5] = (undefined4 ***)0x7;
      *(undefined2 *)ppppuVar7 = 0;
    }
    FUN_00f1a330();
    ppppuVar7 = &pppuStack_6c;
    if (7 < uStack_5c._4_4_) {
      ppppuVar7 = (undefined4 ****)pppuStack_6c;
    }
    FUN_01500f80(&pppuStack_6c,ppppuVar7,iVar8 * 100 + iVar2);
    (**(code **)(*local_9c + 0x14))(auStack_88);
    local_89 = 0;
    FUN_00f1e050();
    FUN_00f1a330();
    FUN_00f1a330();
    break;
  default:
    local_89 = 0;
  }
  FUN_00f1a420(uVar3);
  ExceptionList = local_10;
  return local_89;
}


