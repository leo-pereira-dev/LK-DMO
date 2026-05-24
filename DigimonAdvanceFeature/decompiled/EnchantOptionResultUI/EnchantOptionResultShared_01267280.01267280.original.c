
/* ============================================================ */
/* Function: FUN_01267280 */
/* Entry: 01267280 */
/* Signature: undefined FUN_01267280() */
/* ============================================================ */


void __thiscall
FUN_01267280(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  short sVar1;
  undefined4 *puVar2;
  longlong lVar3;
  char cVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  undefined4 local_28;
  undefined4 local_24;
  undefined8 local_20;
  int local_14;
  int local_10;
  int local_c;
  
  local_c = param_1;
  uVar5 = FUN_010324e0();
  FUN_01266c80((uVar5 & 0xff ^ 1) * 2);
  local_10 = *(int *)(param_1 + 0x164);
  if (local_10 != 0) {
    iVar6 = *(int *)(param_1 + 0xa8);
    local_20 = 0;
    iVar8 = 0xe;
    piVar7 = (int *)(iVar6 + 0x50);
    local_24 = 0;
    lVar3 = 0;
    local_14 = iVar6;
    do {
      local_28 = (undefined4)lVar3;
      puVar2 = (undefined4 *)*piVar7;
      if (puVar2 != (undefined4 *)0x0) {
        sVar1 = *(short *)(puVar2 + 1);
        if (*(int *)(iVar6 + 0x2c) == 0) {
          local_20 = 0;
          lVar3 = 0;
        }
        else {
          local_20 = CONCAT44(local_20._4_4_,*puVar2);
          iVar6 = FUN_015db910(&local_20);
          if (iVar6 == 0) {
            local_20 = 0;
            lVar3 = 0;
          }
          else {
            lVar3 = (ulonglong)(uint)(int)sVar1 * (ulonglong)*(uint *)(iVar6 + 0x50);
          }
        }
        lVar3 = lVar3 + CONCAT44(local_24,local_28);
        local_24 = (undefined4)((ulonglong)lVar3 >> 0x20);
        iVar6 = local_14;
      }
      piVar7 = piVar7 + 2;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
    iVar6 = iVar8;
    iVar9 = iVar8;
    iVar10 = iVar8;
    iVar11 = iVar8;
    iVar12 = iVar8;
    iVar13 = iVar8;
    FUN_00f65090(lVar3);
    FUN_013f3620(iVar8,iVar6,iVar9,iVar10,iVar11,iVar12,iVar13);
    param_1 = local_c;
  }
  iVar6 = *(int *)(param_1 + 0xb0 + param_2 * 0xc);
  param_1 = param_1 + param_2 * 0xc;
  if (iVar6 != 0) {
    *(undefined4 *)(iVar6 + 0x18) = 10;
    *(undefined4 *)(iVar6 + 0x1c) = param_5;
    *(undefined4 *)(iVar6 + 0x20) = param_4;
    *(undefined4 *)(iVar6 + 0x24) = 0;
    (**(code **)(**(int **)(param_1 + 0xb0) + 0x40))(1);
  }
  iVar6 = *(int *)(param_1 + 0xb4);
  if (iVar6 != 0) {
    iVar6 = __RTDynamicCast(*(undefined4 *)(iVar6 + 8),0,&DAT_01bc4a94,&DAT_01bc69c8,0);
    if (iVar6 != 0) {
      *(undefined4 *)(iVar6 + 4) = param_3;
      *(undefined4 *)(iVar6 + 8) = param_5;
    }
  }
  if (DAT_01bfb32c != (int *)0x0) {
    cVar4 = (**(code **)(*DAT_01bfb32c + 4))(s_System_interface_person_01_wav_01ae83c0);
    if (cVar4 != '\0') {
      (**(code **)(*DAT_01bfb32c + 8))(s_System_interface_person_01_wav_01ae83c0);
    }
    (**(code **)*DAT_01bfb32c)(s_System_interface_person_01_wav_01ae83c0);
  }
  return;
}


