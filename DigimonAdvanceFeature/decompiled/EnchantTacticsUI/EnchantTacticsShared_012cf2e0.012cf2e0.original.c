
/* ============================================================ */
/* Function: FUN_012cf2e0 */
/* Entry: 012cf2e0 */
/* Signature: undefined FUN_012cf2e0() */
/* ============================================================ */


void __thiscall FUN_012cf2e0(int param_1,uint param_2)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  uint *puVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 local_24;
  int local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  undefined8 local_10;
  
  if (*(char *)(param_1 + 0x180) == '\x01') {
    FUN_013be010(0,0,0,0);
    return;
  }
  if (DAT_01bfb2b4 == 0) {
    return;
  }
  if (DAT_01bfb2b4 == -0x28) {
    return;
  }
  if (199 < param_2) {
    return;
  }
  puVar7 = (undefined4 *)(DAT_01bfb2b4 + 0x58 + param_2 * 0x45);
  if (puVar7 == (undefined4 *)0x0) {
    return;
  }
  iVar3 = *(int *)(*(int *)(param_1 + 0xb8) + 0x104);
  if (iVar3 == 0) {
    if (*(int *)(param_1 + 0x198) != 0) {
      local_1c = *puVar7;
      iVar4 = FUN_015db910(&local_1c);
      iVar3 = DAT_01bfb2b4;
      if (iVar4 != 0) {
        if (*(short *)(iVar4 + 0x18) == 0x37) {
          if (*(int *)(param_1 + 0x19c) == 0) {
            local_10 = 0;
            uVar8 = 0;
          }
          else {
            local_20 = *(ushort *)(iVar4 + 0x1a) + 0x157c;
            uVar8 = FUN_0161e6e0(&local_20);
          }
          local_24 = (undefined4)((ulonglong)uVar8 >> 0x20);
          local_20 = (int)uVar8;
          cVar1 = FUN_012ce2b0();
          if (cVar1 != '\x01') goto LAB_012cf659;
          iVar3 = FUN_012ce4f0();
          iVar3 = FUN_012ce390(iVar3 + 1);
          iVar6 = 0;
          local_10 = CONCAT44(local_10._4_4_,iVar3);
          if (0 < iVar3) {
            local_14 = (uint)*(ushort *)(iVar4 + 0x1a);
            puVar5 = *(uint **)(param_1 + 0x18c);
            local_18 = 100;
            do {
              if (local_14 == *puVar5 % 100) {
                uVar9 = 0;
                uVar10 = 0;
                uVar11 = 0;
                uVar12 = 0;
                uVar13 = 0;
                uVar14 = 0;
                uVar15 = 0;
                FUN_00f65090(local_20,local_24);
                FUN_013f3620(uVar9,uVar10,uVar11,uVar12,uVar13,uVar14,uVar15);
                iVar3 = DAT_01bfb2b4;
                if (*(int *)(param_1 + 0x164) != 0) {
                  FUN_00f70840(*(undefined4 *)(param_1 + 0x168));
                }
                *(uint *)(param_1 + 0x168) = param_2;
                iVar4 = *(int *)(iVar3 + 4);
                iVar6 = 0;
                local_10 = CONCAT44(local_10._4_4_,(int *)(iVar3 + 4));
                if (iVar4 < 1) goto LAB_012cf587;
                puVar5 = *(uint **)(iVar3 + 0x14);
                goto LAB_012cf577;
              }
              iVar6 = iVar6 + 1;
              puVar5 = puVar5 + 1;
            } while (iVar6 < iVar3);
          }
        }
        else if (*(short *)(iVar4 + 0x18) == 0x38) {
          if (*(int *)(param_1 + 0x16c) != 0) {
            FUN_00f70840(*(undefined4 *)(param_1 + 0x170));
          }
          *(uint *)(param_1 + 0x170) = param_2;
          iVar4 = 0;
          if (0 < *(int *)(iVar3 + 4)) {
            puVar5 = *(uint **)(iVar3 + 0x14);
            do {
              if (*puVar5 == param_2) goto LAB_012cf61b;
              iVar4 = iVar4 + 1;
              puVar5 = puVar5 + 1;
            } while (iVar4 < *(int *)(iVar3 + 4));
          }
          goto LAB_012cf60f;
        }
      }
    }
  }
  else {
    if (iVar3 != 1) goto LAB_012cf659;
    if (0 < **(int **)(*(int *)(DAT_01bfb2a8 + 0x14) + 0x344)) {
      piVar2 = (int *)(DAT_01bfb2b4 + 0x58 + param_2 * 0x45);
      if (piVar2 == (int *)0x0) {
        iVar3 = 0;
      }
      else {
        iVar3 = *piVar2;
      }
      if (((*(int *)(param_1 + 0x198) != 0) &&
          (local_20 = iVar3, iVar4 = FUN_015db910(&local_20), iVar3 = DAT_01bfb2b4, iVar4 != 0)) &&
         (*(short *)(iVar4 + 0x18) == 0x39)) {
        if (*(int *)(param_1 + 0x16c) != 0) {
          FUN_00f70840(*(undefined4 *)(param_1 + 0x170));
        }
        *(uint *)(param_1 + 0x170) = param_2;
        iVar4 = 0;
        if (0 < *(int *)(iVar3 + 4)) {
          puVar5 = *(uint **)(iVar3 + 0x14);
          do {
            if (*puVar5 == param_2) goto LAB_012cf61b;
            iVar4 = iVar4 + 1;
            puVar5 = puVar5 + 1;
          } while (iVar4 < *(int *)(iVar3 + 4));
        }
LAB_012cf60f:
        FUN_00f70890(param_2);
        iVar3 = DAT_01bfb2b4;
LAB_012cf61b:
        if (*(uint *)(param_1 + 0x170) < 200) {
          *(uint *)(param_1 + 0x16c) = *(uint *)(param_1 + 0x170) * 0x45 + 0x58 + iVar3;
        }
        else {
          *(undefined4 *)(param_1 + 0x16c) = 0;
        }
        goto LAB_012cf659;
      }
    }
  }
  FUN_013be010(0,0,0,0);
LAB_012cf659:
  if (DAT_01bfb32c != (int *)0x0) {
    cVar1 = (**(code **)(*DAT_01bfb32c + 4))(s_System_interface_person_01_wav_01ae83c0);
    if (cVar1 != '\0') {
      (**(code **)(*DAT_01bfb32c + 8))(s_System_interface_person_01_wav_01ae83c0);
    }
    (**(code **)*DAT_01bfb32c)(s_System_interface_person_01_wav_01ae83c0);
  }
  return;
  while( true ) {
    iVar6 = iVar6 + 1;
    puVar5 = puVar5 + 1;
    if (iVar4 <= iVar6) break;
LAB_012cf577:
    if (*puVar5 == param_2) goto LAB_012cf595;
  }
LAB_012cf587:
  FUN_00f70890(param_2);
  iVar3 = DAT_01bfb2b4;
LAB_012cf595:
  if (*(uint *)(param_1 + 0x168) < 200) {
    iVar3 = *(uint *)(param_1 + 0x168) * 0x45 + 0x58 + iVar3;
  }
  else {
    iVar3 = 0;
  }
  *(int *)(param_1 + 0x164) = iVar3;
  FUN_012ce6a0();
  FUN_012ce800();
  goto LAB_012cf659;
}


