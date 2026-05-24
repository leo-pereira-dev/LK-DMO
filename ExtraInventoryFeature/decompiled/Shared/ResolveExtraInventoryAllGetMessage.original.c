
/* ============================================================ */
/* Function: FUN_01057640 */
/* Entry: 01057640 */
/* Signature: undefined FUN_01057640() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

code * FUN_01057640(code *param_1,byte *param_2)

{
  int *piVar1;
  int *piVar2;
  char cVar3;
  int iVar4;
  int *piVar5;
  char *pcVar6;
  undefined4 uVar7;
  undefined1 local_48 [8];
  int local_40;
  int local_38;
  int local_34;
  code *local_30;
  undefined8 local_2c;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01978cad;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_30 = param_1;
  cVar3 = FUN_010575a0(local_24);
  if (cVar3 != '\0') {
    FUN_00f489b0(local_48,param_2);
    if ((((*(char *)(local_40 + 0xd) != '\0') || (*param_2 < *(byte *)(local_40 + 0x10))) ||
        (local_40 == *(int *)(local_38 + 0x10))) ||
       (piVar5 = *(int **)(local_40 + 0x14), piVar5 == (int *)0x0)) {
      uVar7 = 0;
      pcVar6 = &DAT_01abd9ec;
      goto LAB_01057853;
    }
    cVar3 = (**(code **)(*piVar5 + 0x4c))();
    if (cVar3 != '\0') {
      local_30 = *(code **)(*piVar5 + 0x48);
      if (*DAT_01bfe460 == 0) {
        local_34 = 0;
      }
      else {
        local_34 = FUN_014e3cc0();
      }
      cVar3 = (*local_30)(&local_34);
      if (cVar3 != '\0') {
        local_2c = 0;
        (**(code **)(*piVar5 + 0x58))(&local_2c);
        local_14 = 0;
        piVar5 = (int *)*(int *)local_2c;
        if (*(char *)((int)piVar5 + 0xd) == '\0') {
          local_34 = 100;
          do {
            iVar4 = *(int *)(*(int *)(local_38 + 0x18) + 4);
            if (*param_2 == 0xd) {
              local_30 = (code *)(piVar5[4] / 100);
              if (iVar4 != 0) {
                iVar4 = FUN_01064df0(&local_30);
LAB_010577bd:
                if (iVar4 != 0x7fffffff) {
                  uVar7 = 0;
                  *param_1 = (code)0x1;
                  pcVar6 = &DAT_01abd9ec;
                  goto LAB_01057820;
                }
              }
            }
            else if (((iVar4 != 0) && (DAT_01bfb2b4 != 0)) && (DAT_01bfb2b4 != -0x28)) {
              iVar4 = FUN_00f61ff0(piVar5[4] / 100,piVar5[4] % local_34);
              goto LAB_010577bd;
            }
            piVar1 = (int *)piVar5[2];
            if (*(char *)((int)piVar1 + 0xd) == '\0') {
              cVar3 = *(char *)(*piVar1 + 0xd);
              piVar5 = piVar1;
              piVar1 = (int *)*piVar1;
              while (cVar3 == '\0') {
                cVar3 = *(char *)(*piVar1 + 0xd);
                piVar5 = piVar1;
                piVar1 = (int *)*piVar1;
              }
            }
            else {
              cVar3 = *(char *)(piVar5[1] + 0xd);
              piVar2 = (int *)piVar5[1];
              piVar1 = piVar5;
              while ((piVar5 = piVar2, cVar3 == '\0' && (piVar1 == (int *)piVar5[2]))) {
                cVar3 = *(char *)(piVar5[1] + 0xd);
                piVar2 = (int *)piVar5[1];
                piVar1 = piVar5;
              }
            }
          } while (*(char *)((int)piVar5 + 0xd) == '\0');
        }
        uVar7 = 0x1b;
        *param_1 = (code)0x0;
        pcVar6 = s_EXTRA_INVEN_NO_ITEMS_TO_GET_01ad0dd8;
LAB_01057820:
        *(undefined4 *)(param_1 + 4) = 0;
        *(undefined4 *)(param_1 + 8) = 0;
        *(undefined4 *)(param_1 + 0xc) = 0;
        *(undefined4 *)(param_1 + 0x10) = 0;
        *(undefined4 *)(param_1 + 0x14) = 0;
        *(undefined4 *)(param_1 + 0x18) = 0;
        FUN_00f19cc0(pcVar6,uVar7);
        FUN_00f27550(&local_2c);
        ExceptionList = local_1c;
        return param_1;
      }
      uVar7 = 0x1a;
      pcVar6 = s_EXTRA_INVEN_INVENTORY_LACK_01ad0dbc;
      goto LAB_01057853;
    }
  }
  uVar7 = 0x12;
  pcVar6 = s_COMMON_NOT_USE_NOW_01ad0cf0;
LAB_01057853:
  *param_1 = (code)0x0;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined4 *)(param_1 + 0x14) = 0;
  *(undefined4 *)(param_1 + 0x18) = 0;
  FUN_00f19cc0(pcVar6,uVar7);
  ExceptionList = local_1c;
  return param_1;
}


