
/* ============================================================ */
/* Function: FUN_011d5350 */
/* Entry: 011d5350 */
/* Signature: undefined FUN_011d5350() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

undefined4 FUN_011d5350(int param_1)

{
  wchar_t *pwVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  undefined4 *******pppppppuVar5;
  undefined8 uVar6;
  tagRECT local_4c;
  undefined4 *******local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0198af1d;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if ((((param_1 == 0) || (*(int *)(param_1 + 4) == 0)) ||
      (local_24 = uVar2, iVar3 = FUN_012326b0(), iVar3 == 0)) ||
     (piVar4 = (int *)__RTDynamicCast(iVar3,0,&DAT_01bc4bb0,&DAT_01bc4e94,0,uVar2),
     piVar4 == (int *)0x0)) {
    ExceptionList = local_1c;
    return 0;
  }
  piVar4 = (int *)(**(code **)(*piVar4 + 0x4c))();
  local_4c.left = *piVar4;
  local_4c.top = piVar4[1];
  local_4c.right = piVar4[2];
  local_4c.bottom = piVar4[3];
  OffsetRect(&local_4c,*(int *)(param_1 + 0x20),*(int *)(param_1 + 0x24));
  iVar3 = FUN_00f83b00();
  if (iVar3 == 0) {
    ExceptionList = local_1c;
    return 0;
  }
  iVar3 = __RTDynamicCast(*(undefined4 *)(param_1 + 8),0,&DAT_01bc4a94,&DAT_01bc62a4,0);
  if (iVar3 == 0) {
    ExceptionList = local_1c;
    return 0;
  }
  if (*DAT_01bfe47c == 0) {
    ExceptionList = local_1c;
    return 0;
  }
  local_2c = 0;
  local_28 = 7;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_3c = (undefined4 *******)0x0;
  local_14 = 0;
  switch(*(undefined4 *)(param_1 + 0xc)) {
  case 0:
  case 4:
    uVar6 = CONCAT44(*(uint *)(iVar3 + 8) / 100,*(uint *)(iVar3 + 4) / 100);
    pwVar1 = u__d____d_01ace3b0;
    break;
  case 1:
    uVar6 = *(undefined8 *)(iVar3 + 4);
    pwVar1 = u__d______d___01ae2cb0;
    break;
  case 2:
  case 3:
    FUN_01500f80(&local_3c,u___2f_______2f___01ae2d00,
                 (double)((float)((double)*(int *)(iVar3 + 4) +
                                 (double)(&DAT_01b033e0)[-(*(int *)(iVar3 + 4) >> 0x1f)]) /
                         DAT_01b02a84),
                 (double)((float)((double)*(int *)(iVar3 + 8) +
                                 (double)(&DAT_01b033e0)[-(*(int *)(iVar3 + 8) >> 0x1f)]) /
                         DAT_01b02a84));
  default:
    goto switchD_011d546a_default;
  }
  FUN_01500f80(&local_3c,pwVar1,uVar6);
switchD_011d546a_default:
  pppppppuVar5 = &local_3c;
  if (7 < local_28) {
    pppppppuVar5 = local_3c;
  }
  FUN_013c1650(pppppppuVar5,9,local_4c.left,local_4c.top,local_4c.right - local_4c.left,
               local_4c.bottom - local_4c.top);
  FUN_00f1a330();
  ExceptionList = local_1c;
  return 1;
}


