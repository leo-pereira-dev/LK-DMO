
/* ============================================================ */
/* Function: FUN_012b6bf0 */
/* Entry: 012b6bf0 */
/* Signature: undefined FUN_012b6bf0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 __thiscall FUN_012b6bf0(int param_1,int *param_2)

{
  uint uVar1;
  undefined1 *puVar2;
  char cVar3;
  undefined1 *puVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  int iVar9;
  char *pcVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined8 local_38;
  undefined4 local_30;
  int local_2c;
  int local_28;
  undefined1 local_21;
  undefined8 local_20;
  undefined1 *local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019a06bd;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  uVar1 = *(uint *)(param_1 + 0x48);
  if ((((DAT_01bfb2b4 == 0) || (DAT_01bfb2b4 == -0x28)) || (*(uint *)(DAT_01bfb2b4 + 0x44) <= uVar1)
      ) || (((199 < uVar1 ||
             (piVar7 = (int *)(DAT_01bfb2b4 + 0x58 + uVar1 * 0x45), piVar7 == (int *)0x0)) ||
            ((*piVar7 == 0 && ((short)piVar7[1] == 0)))))) {
    piVar7 = (int *)0x0;
  }
  uVar1 = *(uint *)(param_1 + 0x78);
  if ((((DAT_01bfb2b4 == 0) || (DAT_01bfb2b4 == -0x28)) ||
      ((*(uint *)(DAT_01bfb2b4 + 0x44) <= uVar1 ||
       ((199 < uVar1 || (piVar8 = (int *)(DAT_01bfb2b4 + 0x58 + uVar1 * 0x45), piVar8 == (int *)0x0)
        ))))) || ((*piVar8 == 0 && ((short)piVar8[1] == 0)))) {
    piVar8 = (int *)0x0;
  }
  local_20 = 0;
  iVar9 = *(int *)(param_1 + 0x148);
  local_28 = param_1;
  if (iVar9 == 0) {
    local_20._0_4_ = (undefined1 *)iVar9;
    local_20._4_4_ = iVar9;
    puVar4 = operator_new(0x38);
    *(undefined1 **)puVar4 = puVar4;
    *(undefined1 **)(puVar4 + 4) = puVar4;
    *(undefined1 **)(puVar4 + 8) = puVar4;
    *(undefined2 *)(puVar4 + 0xc) = 0x101;
    local_20 = CONCAT44(local_20._4_4_,puVar4);
  }
  else {
    FUN_015ec8b0(&local_20,*piVar7,*(undefined1 *)((int)piVar7 + 9));
    puVar4 = (undefined1 *)local_20;
  }
  local_8 = 0;
  local_2c = *piVar8;
  FUN_01271bc0(&local_18,&local_2c);
  puVar2 = local_18;
  if (local_18 != puVar4) {
    FUN_012b5f30(&local_48);
    cVar3 = FUN_012b8610(*(int *)(puVar2 + 0x24),*(int *)(puVar2 + 0x24) >> 0x1f);
    local_30 = 0;
    local_38 = 0;
    local_48 = 0;
    uStack_44 = 0;
    uStack_40 = 0;
    uStack_3c = 0;
    if (cVar3 == '\0') {
      if ((DAT_01bfb2b4 == 0) || (local_2c = DAT_01bfb2b4 + 0x28, local_2c == 0)) goto LAB_012b6d4e;
      local_18 = *(undefined1 **)(puVar2 + 0x28);
      local_21 = 0;
      iVar5 = FUN_00f62070(&local_18,&local_21);
      local_18 = *(undefined1 **)(puVar2 + 0x2c);
      local_21 = 0;
      iVar6 = FUN_00f62070(&local_18,&local_21);
      iVar9 = DAT_01bfb2b4;
      if (*param_2 <= iVar6 + iVar5) {
        if (((*(int *)(*(int *)(local_28 + 0x154) + 4) != 0) && (DAT_01bfb2b4 != 0)) &&
           (DAT_01bfb2b4 != -0x28)) {
          if (*DAT_01bfe460 != 0) {
            FUN_014e3cc0();
          }
          local_18 = *(undefined1 **)(iVar9 + 0x44);
          iVar9 = 0;
          if (0 < (int)local_18) {
            do {
              FUN_00f63330(iVar9);
              iVar9 = iVar9 + 1;
            } while (iVar9 < (int)local_18);
          }
        }
        local_18 = &DAT_00000015;
        FUN_00ffcb30(&local_18,&DAT_01bfe980);
        local_21 = 1;
        goto LAB_012b6d52;
      }
      uVar11 = 0xe;
      pcVar10 = s_NEED_ITEM_LACK_01aeb260;
    }
    else {
      uVar11 = 0x1e;
      pcVar10 = s_EQUIPSYSTEM_UPGRADE_MONEY_LACK_01ace3dc;
    }
    uVar17 = 0;
    uVar16 = 0;
    uVar15 = 0;
    uVar14 = 0;
    uVar13 = 0;
    uVar12 = 0;
    FUN_00f19cc0(pcVar10,uVar11);
    FUN_010052f0(uVar12,uVar13,uVar14,uVar15,uVar16,uVar17);
  }
LAB_012b6d4e:
  local_21 = 0;
LAB_012b6d52:
  FUN_0115a360();
  ExceptionList = local_10;
  return local_21;
}


