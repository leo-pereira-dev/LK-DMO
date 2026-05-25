
/* ============================================================ */
/* Function: FUN_011d9b80 */
/* Entry: 011d9b80 */
/* Signature: undefined FUN_011d9b80() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011d9b80(int param_1,undefined4 param_2,int param_3)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int local_1c;
  int local_18;
  undefined1 auStack_14 [8];
  int iStack_c;
  int local_8;
  uint local_4;
  
  local_4 = DAT_01bbb1c0 ^ (uint)&local_1c;
  if (param_3 != 0) {
    local_1c = *(int *)(param_3 + 0xc);
    local_8 = 0;
    local_18 = *(int *)(*(int *)(param_1 + 0xb0) + 0x68);
    if (0 < local_18) {
      do {
        puVar5 = *(undefined4 **)(*(int *)(param_1 + 0xb0) + 100);
        for (puVar1 = (undefined4 *)*puVar5; puVar1 != puVar5; puVar1 = (undefined4 *)*puVar1) {
          iVar3 = puVar1[2];
          if (*(int *)(iVar3 + 0xc) == local_8) {
            if ((iVar3 != 0) && (*(int *)(iVar3 + 4) != 0)) {
              puVar5 = *(undefined4 **)(*(int *)(iVar3 + 4) + 4);
              puVar1 = (undefined4 *)*puVar5;
              goto joined_r0x011d9c02;
            }
            break;
          }
        }
LAB_011d9cb6:
        local_8 = local_8 + 1;
      } while (local_8 < local_18);
    }
  }
  return;
joined_r0x011d9c02:
  if (puVar1 == puVar5) goto LAB_011d9cb6;
  piVar2 = (int *)puVar1[2];
  iVar3 = (**(code **)(*piVar2 + 4))();
  if (iVar3 == 8) {
    if (piVar2 != (int *)0x0) {
      iVar4 = __RTDynamicCast(piVar2,0,&DAT_01bc4bb0,&DAT_01bc4fcc,0);
      iVar3 = local_1c;
      if (iVar4 != 0) {
        uVar6 = 0;
        if (local_8 == local_1c) {
          uVar6 = 3;
        }
        *(undefined4 *)(*(int *)(iVar4 + 0x24) + 0xd8) = uVar6;
      }
      FUN_00f1d720(auStack_14,&local_8);
      if (((*(char *)(iStack_c + 0xd) == '\0') && (*(int *)(iStack_c + 0x10) <= local_8)) &&
         (iStack_c != *(int *)(param_1 + 0xb4))) {
        puVar5 = (undefined4 *)FUN_00fcd2e0(&local_8);
        (**(code **)(*(int *)*puVar5 + 0x38))(local_8 == iVar3);
      }
    }
    goto LAB_011d9cb6;
  }
  puVar1 = (undefined4 *)*puVar1;
  goto joined_r0x011d9c02;
}


