
/* ============================================================ */
/* Function: FUN_011d5590 */
/* Entry: 011d5590 */
/* Signature: undefined FUN_011d5590() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_011d5590(int param_1)

{
  short sVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  short *******pppppppsVar6;
  short *******pppppppsVar7;
  undefined4 uVar8;
  short *******pppppppsVar9;
  float local_70;
  int local_6c;
  short *******local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined1 local_4d;
  tagRECT local_4c;
  short *******local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined8 local_2c;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199339d;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if ((((param_1 == 0) || (*(int *)(param_1 + 4) == 0)) ||
      (local_24 = uVar3, iVar4 = FUN_012326b0(1), iVar4 == 0)) ||
     (piVar5 = (int *)__RTDynamicCast(iVar4,0,&DAT_01bc4bb0,&DAT_01bc4e94,0,uVar3),
     piVar5 == (int *)0x0)) {
    ExceptionList = local_1c;
    return 0;
  }
  piVar5 = (int *)(**(code **)(*piVar5 + 0x4c))(&uStack_60);
  local_4c.left = *piVar5;
  local_4c.top = piVar5[1];
  local_4c.right = piVar5[2];
  local_4c.bottom = piVar5[3];
  OffsetRect(&local_4c,*(int *)(param_1 + 0x20),*(int *)(param_1 + 0x24));
  iVar4 = FUN_00f83b00(&stack0x00000008);
  if (iVar4 == 0) {
    ExceptionList = local_1c;
    return 0;
  }
  iVar4 = __RTDynamicCast(*(undefined4 *)(param_1 + 8),0,&DAT_01bc4a94,&DAT_01bc6270,0);
  if (iVar4 == 0) {
    ExceptionList = local_1c;
    return 0;
  }
  local_6c = *DAT_01bfe47c;
  if (local_6c == 0) {
    ExceptionList = local_1c;
    return 0;
  }
  local_2c._0_4_ = 0;
  uVar3 = (uint)local_2c;
  local_2c._0_4_ = 0;
  local_2c._4_4_ = 7;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_3c = (short *******)0x0;
  local_14 = 0;
  iVar2 = *(int *)(iVar4 + 0xc);
  if (iVar2 != 0) {
    if (iVar2 == 1) {
      local_2c._0_4_ = uVar3;
      FUN_015011b0(&local_3c,iVar4 + 8);
      pppppppsVar9 = (short *******)&local_3c;
      if (7 < local_2c._4_4_) {
        pppppppsVar9 = local_3c;
      }
      if ((uint)local_2c == 0) {
LAB_011d5792:
        uVar3 = 0xffffffff;
      }
      else {
        pppppppsVar7 = (short *******)0x0;
        if ((uint)local_2c != 0) {
          sVar1 = *(short *)pppppppsVar9;
          pppppppsVar6 = pppppppsVar9;
          for (uVar3 = (uint)local_2c;
              (pppppppsVar7 = pppppppsVar6, sVar1 != 0x2e &&
              (pppppppsVar7 = (short *******)0x0, uVar3 != 1)); uVar3 = uVar3 - 1) {
            pppppppsVar6 = (short *******)((int)pppppppsVar6 + 2);
            sVar1 = *(short *)pppppppsVar6;
          }
        }
        if (pppppppsVar7 == (short *******)0x0) goto LAB_011d5792;
        uVar3 = (int)pppppppsVar7 - (int)pppppppsVar9 >> 1;
      }
      if ((uint)local_2c < uVar3) {
        uVar3 = (uint)local_2c;
      }
    }
    else {
      if (iVar2 != 2) {
        local_4d = 0;
        FUN_00f1a330();
        ExceptionList = local_1c;
        return local_4d;
      }
      local_70 = (float)*(int *)(iVar4 + 8) * _DAT_01b0252c;
      FUN_01501290(&local_3c,&local_70);
      pppppppsVar9 = (short *******)&local_3c;
      if (7 < local_2c._4_4_) {
        pppppppsVar9 = local_3c;
      }
      if ((uint)local_2c != 0) {
        pppppppsVar7 = (short *******)0x0;
        if ((uint)local_2c != 0) {
          sVar1 = *(short *)pppppppsVar9;
          pppppppsVar6 = pppppppsVar9;
          for (uVar3 = (uint)local_2c;
              (pppppppsVar7 = pppppppsVar6, sVar1 != 0x2e &&
              (pppppppsVar7 = (short *******)0x0, uVar3 != 1)); uVar3 = uVar3 - 1) {
            pppppppsVar6 = (short *******)((int)pppppppsVar6 + 2);
            sVar1 = *(short *)pppppppsVar6;
          }
        }
        if (pppppppsVar7 != (short *******)0x0) {
          uVar3 = ((int)pppppppsVar7 - (int)pppppppsVar9 >> 1) + 3;
          if ((uint)local_2c < uVar3) {
            uVar3 = (uint)local_2c;
          }
          goto LAB_011d579d;
        }
      }
      uVar3 = 2;
      if ((uint)local_2c < 2) {
        uVar3 = (uint)local_2c;
      }
    }
LAB_011d579d:
    uStack_54 = 0;
    pppppppsVar9 = (short *******)&local_3c;
    if (7 < local_2c._4_4_) {
      pppppppsVar9 = local_3c;
    }
    local_58 = 0;
    local_68 = (short *******)0x0;
    uStack_64 = 0;
    uStack_60 = 0;
    uStack_5c = 0;
    FUN_00f1a200(pppppppsVar9,uVar3);
    FUN_00f1a330();
    local_3c = local_68;
    local_68 = (short *******)((uint)local_68 & 0xffff0000);
    uStack_38 = uStack_64;
    uStack_34 = uStack_60;
    uStack_30 = uStack_5c;
    local_2c = CONCAT44(uStack_54,local_58);
    local_58 = 0;
    uStack_54 = 7;
    FUN_00f1a330();
    pppppppsVar9 = (short *******)&DAT_01ad878c;
    uVar8 = 0x11d580b;
    FUN_00f53390(&DAT_01ad878c,1);
    goto LAB_011d58c5;
  }
  local_2c._0_4_ = uVar3;
  FUN_015011b0(&local_3c,iVar4 + 8);
  pppppppsVar9 = (short *******)&local_3c;
  if (7 < local_2c._4_4_) {
    pppppppsVar9 = local_3c;
  }
  if ((uint)local_2c == 0) {
LAB_011d585b:
    uVar3 = 0xffffffff;
  }
  else {
    pppppppsVar7 = (short *******)0x0;
    if ((uint)local_2c != 0) {
      sVar1 = *(short *)pppppppsVar9;
      pppppppsVar6 = pppppppsVar9;
      for (uVar3 = (uint)local_2c;
          (pppppppsVar7 = pppppppsVar6, sVar1 != 0x2e &&
          (pppppppsVar7 = (short *******)0x0, uVar3 != 1)); uVar3 = uVar3 - 1) {
        pppppppsVar6 = (short *******)((int)pppppppsVar6 + 2);
        sVar1 = *(short *)pppppppsVar6;
      }
    }
    if (pppppppsVar7 == (short *******)0x0) goto LAB_011d585b;
    uVar3 = (int)pppppppsVar7 - (int)pppppppsVar9 >> 1;
  }
  local_58 = 0;
  if ((uint)local_2c < uVar3) {
    uVar3 = (uint)local_2c;
  }
  pppppppsVar9 = (short *******)&local_3c;
  if (7 < local_2c._4_4_) {
    pppppppsVar9 = local_3c;
  }
  local_68 = (short *******)0x0;
  uStack_64 = 0;
  uStack_60 = 0;
  uStack_5c = 0;
  uStack_54 = 0;
  uVar8 = 0x11d588f;
  FUN_00f1a200(pppppppsVar9,uVar3);
  FUN_00f1a330();
  local_3c = local_68;
  local_68 = (short *******)((uint)local_68 & 0xffff0000);
  uStack_38 = uStack_64;
  uStack_34 = uStack_60;
  uStack_30 = uStack_5c;
  local_2c = CONCAT44(uStack_54,local_58);
  local_58 = 0;
  uStack_54 = 7;
  FUN_00f1a330();
LAB_011d58c5:
  FUN_011d5f90(uVar8,pppppppsVar9,(*(int *)(iVar4 + 4) != 0) * '\x02' + '+');
  pppppppsVar9 = (short *******)&local_3c;
  if (7 < local_2c._4_4_) {
    pppppppsVar9 = local_3c;
  }
  FUN_013c1650(pppppppsVar9,9,local_4c.left,local_4c.top,local_4c.right - local_4c.left,
               local_4c.bottom - local_4c.top);
  local_4d = 1;
  FUN_00f1a330();
  ExceptionList = local_1c;
  return local_4d;
}


