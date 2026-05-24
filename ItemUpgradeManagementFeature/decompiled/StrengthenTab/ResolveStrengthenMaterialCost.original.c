
/* ============================================================ */
/* Function: FUN_01276a00 */
/* Entry: 01276a00 */
/* Signature: undefined FUN_01276a00() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01276a00(int param_1)

{
  short sVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  byte *pbVar8;
  int iVar9;
  short sVar10;
  int local_74;
  int local_70;
  undefined1 local_69;
  undefined1 local_68 [24];
  undefined4 *local_50;
  undefined4 local_44;
  undefined1 local_3c [4];
  int local_38;
  int local_34;
  undefined *local_30;
  undefined2 local_2c;
  undefined2 local_28;
  undefined4 *local_26;
  int local_22;
  undefined2 local_1e;
  short local_1c;
  int local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199c33d;
  local_10 = ExceptionList;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if ((((*(int *)(param_1 + 0xa4) != 0) &&
       (local_14 = uVar5,
       local_70 = __RTDynamicCast(*(undefined4 *)(*(int *)(param_1 + 0xa4) + 8),0,&DAT_01bc4a94,
                                  &DAT_01bc6a44,0,uVar5), iVar7 = DAT_01bfb2b4, local_70 != 0)) &&
      (*(int *)(local_70 + 4) != 0)) && (DAT_01bfb2b4 != -0x28)) {
    iVar9 = 0;
    local_74 = 0;
    sVar1 = 0;
    iVar2 = *(int *)(*(int *)(param_1 + 0x78) + 0x48);
LAB_01276a93:
    do {
      iVar6 = *(int *)(*(int *)(param_1 + (iVar9 + 7) * 0x1c) + 0x24);
      if (iVar6 != 0) {
        iVar6 = __RTDynamicCast(*(undefined4 *)(iVar6 + 0x34),0,&DAT_01bc4a94,&DAT_01bc6a70,0,uVar5)
        ;
        if (iVar6 == 0) {
          ExceptionList = local_10;
          return;
        }
        if (iVar9 == 0) {
          sVar1 = *(short *)(iVar6 + 8);
          iVar9 = 1;
          goto LAB_01276a93;
        }
        if (iVar9 == 1) {
          sVar10 = *(short *)(iVar6 + 8);
          break;
        }
      }
      sVar10 = 0;
      iVar9 = iVar9 + 1;
    } while (iVar9 < 2);
    if (sVar1 != 0) {
      if ((iVar2 < 0) || (199 < iVar2)) {
        pbVar8 = &DAT_00000009;
      }
      else {
        pbVar8 = (byte *)(iVar7 + 0x61 + iVar2 * 0x45);
      }
      FUN_015ec600(local_68,*(undefined4 *)(local_70 + 4),*pbVar8 + 1);
      local_8 = 0;
      FUN_015ec470(local_3c,local_44);
      local_70 = local_34;
      local_69 = 0;
      iVar7 = FUN_00f62070(&local_70,&local_69);
      local_74 = local_34;
      if (iVar7 == 0) {
        local_74 = local_38;
      }
      local_8 = 0xffffffff;
      *(undefined4 *)local_50[1] = 0;
      puVar4 = (undefined4 *)*local_50;
      while (puVar4 != (undefined4 *)0x0) {
        puVar3 = (undefined4 *)*puVar4;
        FUN_017986fc(puVar4,0x18);
        puVar4 = puVar3;
      }
      FUN_017986fc(local_50,0x18);
    }
    iVar7 = *(int *)(param_1 + 0x78);
    if (*(char *)(iVar7 + 0x130) == '\0') {
      *(undefined1 *)(iVar7 + 0x130) = 1;
      iVar9 = DAT_01bfb2b4;
      if (((*(int *)(*(int *)(iVar7 + 0x154) + 4) != 0) && (DAT_01bfb2b4 != 0)) &&
         (DAT_01bfb2b4 != -0x28)) {
        if (*DAT_01bfe460 != 0) {
          FUN_014e3cc0();
        }
        iVar7 = *(int *)(iVar9 + 0x44);
        iVar9 = 0;
        if (0 < iVar7) {
          do {
            FUN_00f63330(iVar9);
            iVar9 = iVar9 + 1;
          } while (iVar9 < iVar7);
        }
      }
      local_30 = &DAT_01aeb2dc;
      local_2c = 0xf9b;
      local_28 = (undefined2)iVar2;
      local_22 = local_74;
      local_1e = 0;
      local_18 = (int)sVar10;
      local_26 = &DAT_01ac6564;
      iVar7 = *DAT_01bfe460;
      if ((((iVar7 != 0) && (*(char *)(iVar7 + 0x4f0) != '\x01')) &&
          (*(char *)(iVar7 + 0x544) == '\x03')) && (*(char *)(iVar7 + 0x545) == '\x02')) {
        local_1c = sVar1;
        FUN_014e3b40(&local_30,0x200,0);
      }
    }
  }
  ExceptionList = local_10;
  return;
}


