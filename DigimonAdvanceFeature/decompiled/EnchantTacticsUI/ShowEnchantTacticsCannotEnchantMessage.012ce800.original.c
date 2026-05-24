
/* ============================================================ */
/* Function: FUN_012ce800 */
/* Entry: 012ce800 */
/* Signature: undefined FUN_012ce800() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_012ce800(int param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  code *pcVar8;
  code *pcVar9;
  code *pcVar10;
  float fVar11;
  undefined4 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined1 local_ac [12];
  undefined1 local_a0 [8];
  int local_98;
  int local_94;
  char local_8d;
  float local_8c;
  char local_85;
  code *local_84;
  uint local_80;
  int local_7c;
  code *local_78;
  uint local_74;
  code *local_70;
  int local_6c;
  undefined1 local_68 [68];
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019a1bc0;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_94 = param_1;
  local_6c = FUN_012ce4f0(local_24);
  local_6c = local_6c + 1;
  if (0xf < local_6c) {
    uVar3 = 0;
    uVar12 = 0;
    uVar13 = 0;
    local_70 = *(code **)(**(int **)(param_1 + 0x188) + 0x10);
    uVar14 = 0;
    FUN_00f19cc0(s_ENCHANTTACTICS_CANNOT_ENCHANT_01aebcd8,0x1d);
    FUN_015ce480(local_ac,uVar3,uVar12,uVar13,uVar14);
    local_14 = 0;
    (*local_70)();
    FUN_00f1a330();
    ExceptionList = local_1c;
    return;
  }
  memset(local_68,0,0x40);
  iVar4 = *(int *)(DAT_01bfb2a8 + 0x14);
  local_8c = *(float *)(iVar4 + 0xf8);
  if (*(int *)(iVar4 + 0x168) == 0) {
    local_80 = 0;
  }
  else {
    local_80 = FUN_015b1930();
  }
  local_84 = *(code **)(iVar4 + 0x60);
  uVar14 = 0x1bc2bb0;
  local_80 = local_80 & 0xffff;
  uVar13 = 0x1bc2a2000000000;
  uVar3 = FUN_015cea10(0xe);
  iVar4 = __RTDynamicCast(uVar3,uVar13,uVar14);
  if (iVar4 == 0) {
    local_78 = (code *)0x0;
  }
  else {
    local_78 = (code *)FUN_015d6b70();
  }
  pcVar8 = local_78;
  iVar4 = 0;
  local_84 = (code *)FUN_012ce390();
  if (*(undefined4 **)(local_94 + 0x164) != (undefined4 *)0x0) {
    if (*(int *)(local_94 + 0x198) == 0) {
      iVar5 = 0;
    }
    else {
      local_7c = **(undefined4 **)(local_94 + 0x164);
      iVar5 = FUN_015db910();
    }
    if (0 < (int)local_84) {
      local_74 = 100;
      do {
        pcVar8 = local_78;
        if ((iVar5 != 0) &&
           ((uint)*(ushort *)(iVar5 + 0x1a) ==
            *(uint *)(*(int *)(local_94 + 0x18c) + iVar4 * 4) % 100)) break;
        iVar4 = iVar4 + 1;
      } while (iVar4 < (int)local_84);
    }
  }
  local_78 = (code *)0x0;
  local_84 = (code *)0x0;
  local_85 = '\0';
  local_7c = *(int *)(*(int *)(DAT_01bfb2a8 + 0x14) + 0x344);
  switch(*(undefined4 *)(*(int *)(local_94 + 0x18c) + iVar4 * 4)) {
  case 0x1587:
  case 0x1588:
  case 0x1589:
  case 0x158a:
  case 0x158b:
  case 0x1591:
  case 0x1592:
  case 0x1593:
  case 0x1594:
  case 0x1595:
    local_85 = '\x01';
  }
  puVar2 = (undefined4 *)(local_94 + 0x178);
  if (*(int *)(local_94 + 0x19c) != 0) {
    local_8d = FUN_012cf6a0();
    local_8d = local_8d + '\x01';
    iVar4 = FUN_0161e740(&local_8d,&local_6c);
    if (iVar4 != 0) {
      if (local_85 == '\0') {
        local_78 = *(code **)(iVar4 + 0x14);
        local_84 = *(code **)(iVar4 + 0x18);
      }
      else {
        local_84 = *(code **)(iVar4 + 0x1c);
        local_78 = local_84;
      }
    }
  }
  pcVar9 = local_70;
  pcVar10 = local_70;
  switch(*puVar2) {
  case 0:
    if (pcVar8 == (code *)0x0) {
      iVar4 = 0;
      iVar5 = 0;
    }
    else {
      uVar14 = 0x1bc2b4c;
      uVar13 = 0x1bc2a2000000000;
      uVar3 = FUN_015cea10(4);
      iVar4 = __RTDynamicCast(uVar3,uVar13,uVar14);
      if (iVar4 == 0) {
        iVar4 = 0;
      }
      else {
        local_74 = local_80 & 0xffff;
        local_70 = (code *)(uint)*(ushort *)((int)pcVar8 + 0x2c);
        iVar4 = FUN_015e33f0(&local_70,&local_74);
        if (iVar4 == 0) {
          iVar4 = 0;
        }
        else {
          iVar4 = (int)((float)*(ushort *)((int)pcVar8 + 0x14) * local_8c) + *(int *)(iVar4 + 0x18);
        }
      }
      uVar14 = 0x1bc2b4c;
      uVar13 = 0x1bc2a2000000000;
      uVar3 = FUN_015cea10(4);
      iVar6 = __RTDynamicCast(uVar3,uVar13,uVar14);
      iVar5 = 0;
      if (iVar6 != 0) {
        local_70 = (code *)(local_80 & 0xffff);
        local_74 = (uint)*(ushort *)((int)pcVar8 + 0x2c);
        local_80 = FUN_015e33f0(&local_74,&local_70);
        iVar5 = 0;
        if (local_80 != 0) {
          uVar14 = 0x1bc2b28;
          uVar13 = 0x1bc2a2000000000;
          uVar3 = FUN_015cea10(0xd);
          iVar5 = __RTDynamicCast(uVar3,uVar13,uVar14);
          if (iVar5 == 0) {
            fVar11 = 0.0;
          }
          else {
            local_8c = (float)(uint)*(byte *)((int)pcVar8 + 0x36);
            iVar5 = FUN_015e5990();
            fVar11 = (float)(iVar5 + -100) * DAT_01b02528;
          }
          iVar5 = (int)((float)*(int *)(local_80 + 0x18) * fVar11);
        }
      }
    }
    iVar6 = local_7c;
    local_74 = iVar5 + iVar4;
    local_70 = (code *)0x0;
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (iVar4 = local_98, 0 < *(int *)(local_98 + 0x10))) {
      iVar4 = *(int *)(iVar6 + 4);
    }
    iVar5 = *(int *)(iVar6 + 4);
    iVar6 = 0;
    if (iVar4 != iVar5) {
      iVar6 = iVar4 + 0x14;
    }
    if (iVar6 == 0) {
      iVar4 = 0;
    }
    else {
      iVar4 = *(int *)(iVar6 + 4);
    }
    local_70 = (code *)0x0;
    fVar11 = (float)(int)(local_78 + iVar4);
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (0 < *(int *)(local_98 + 0x10))) {
      local_98 = iVar5;
    }
    iVar4 = 0;
    if (local_98 != iVar5) {
      iVar4 = local_98 + 0x14;
    }
    if (iVar4 == 0) {
      iVar4 = 0;
    }
    else {
LAB_012cf199:
      iVar4 = *(int *)(iVar4 + 4);
    }
    goto LAB_012cf19c;
  case 1:
    local_70 = (code *)0x1;
    piVar1 = (int *)(local_7c + 4);
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (iVar4 = local_98, 1 < *(int *)(local_98 + 0x10))) {
      iVar4 = *piVar1;
    }
    iVar5 = *piVar1;
    iVar6 = 0;
    if (iVar4 != iVar5) {
      iVar6 = iVar4 + 0x14;
    }
    if (iVar6 == 0) {
      iVar4 = 0;
    }
    else {
      iVar4 = *(int *)(iVar6 + 4);
    }
    local_70 = (code *)0x1;
    fVar11 = (float)(int)(local_78 + iVar4);
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (1 < *(int *)(local_98 + 0x10))) {
      local_98 = iVar5;
    }
    iVar4 = 0;
    if (local_98 != iVar5) {
      iVar4 = local_98 + 0x14;
    }
    if (iVar4 == 0) {
      iVar4 = 0;
    }
    else {
      iVar4 = *(int *)(iVar4 + 4);
    }
    FUN_00f222a0(local_68,0x20,u__d______d___01aebcac,(int)fVar11,
                 (int)(float)(int)(local_84 + iVar4));
    goto LAB_012cf216;
  case 2:
    local_70 = (code *)0x2;
    piVar1 = (int *)(local_7c + 4);
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (iVar4 = local_98, 2 < *(int *)(local_98 + 0x10))) {
      iVar4 = *piVar1;
    }
    iVar5 = 0;
    if (iVar4 != *piVar1) {
      iVar5 = iVar4 + 0x14;
    }
    if (iVar5 == 0) {
      local_7c = 0;
    }
    else {
      local_7c = *(int *)(iVar5 + 4);
    }
    if (pcVar8 == (code *)0x0) {
      iVar4 = 0;
    }
    else {
      uVar14 = 0x1bc2b4c;
      uVar13 = 0x1bc2a2000000000;
      uVar3 = FUN_015cea10(4);
      iVar5 = __RTDynamicCast(uVar3,uVar13,uVar14);
      iVar4 = 0;
      if (iVar5 != 0) {
        local_70 = (code *)(local_80 & 0xffff);
        local_74 = (uint)*(ushort *)((int)pcVar8 + 0x2c);
        iVar4 = FUN_015e33f0(&local_74,&local_70);
        if (iVar4 == 0) {
          iVar4 = 0;
        }
        else {
          iVar4 = (int)((float)*(ushort *)((int)pcVar8 + 0x12) * local_8c) + *(int *)(iVar4 + 0x14);
        }
      }
    }
    local_70 = (code *)0x2;
    pcVar10 = (code *)((float)((int)(local_78 + local_7c) * iVar4) / DAT_01b02a84);
    local_78 = pcVar10;
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (2 < *(int *)(local_98 + 0x10))) {
      local_98 = *piVar1;
    }
    iVar4 = 0;
    if (local_98 != *piVar1) {
      iVar4 = local_98 + 0x14;
    }
    if (iVar4 == 0) {
      iVar4 = 0;
    }
    else {
      iVar4 = *(int *)(iVar4 + 4);
    }
    if (pcVar8 == (code *)0x0) {
      iVar5 = 0;
    }
    else {
      uVar14 = 0x1bc2b4c;
      uVar13 = 0x1bc2a2000000000;
      uVar3 = FUN_015cea10(4);
      iVar5 = __RTDynamicCast(uVar3,uVar13,uVar14);
      if (iVar5 != 0) {
        local_70 = (code *)(local_80 & 0xffff);
        local_74 = (uint)*(ushort *)((int)pcVar8 + 0x2c);
        iVar5 = FUN_015e33f0(&local_74,&local_70);
        if (iVar5 != 0) {
          iVar5 = (int)((float)*(ushort *)((int)pcVar8 + 0x12) * local_8c) + *(int *)(iVar5 + 0x14);
          pcVar10 = local_78;
          goto LAB_012cee24;
        }
      }
      iVar5 = 0;
      pcVar10 = local_78;
    }
LAB_012cee24:
    pcVar9 = (code *)((float)((int)(local_84 + iVar4) * iVar5) / DAT_01b02a84);
    break;
  case 3:
    local_8c = 5.60519e-45;
    if (pcVar8 == (code *)0x0) {
      iVar4 = 0;
    }
    else {
      uVar14 = 0x1bc2b4c;
      uVar13 = 0x1bc2a2000000000;
      uVar3 = FUN_015cea10(4);
      iVar4 = __RTDynamicCast(uVar3,uVar13,uVar14);
      if (iVar4 == 0) {
        iVar4 = 0;
      }
      else {
        local_70 = (code *)(local_80 & 0xffff);
        local_74 = (uint)*(ushort *)((int)pcVar8 + 0x2c);
        iVar4 = FUN_015e33f0(&local_74,&local_70);
        if (iVar4 == 0) {
          iVar4 = 0;
        }
        else {
          iVar4 = (uint)*(ushort *)((int)pcVar8 + 0xe) + *(int *)(iVar4 + 0x10);
        }
      }
    }
    FUN_00f1d720(local_a0,&local_8c);
    iVar5 = local_7c;
    if ((*(char *)(local_98 + 0xd) != '\0') || (iVar6 = local_98, 4 < *(int *)(local_98 + 0x10))) {
      iVar6 = *(int *)(local_7c + 4);
    }
    iVar7 = 0;
    if (iVar6 != *(int *)(local_7c + 4)) {
      iVar7 = iVar6 + 0x14;
    }
    if (iVar7 == 0) {
      iVar6 = 0;
    }
    else {
      iVar6 = *(int *)(iVar7 + 4);
    }
    local_70 = (code *)0x4;
    pcVar10 = (code *)((float)((int)(local_78 + iVar6) * iVar4) / DAT_01b02a84);
    local_78 = pcVar10;
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (4 < *(int *)(local_98 + 0x10))) {
      local_98 = *(int *)(iVar5 + 4);
    }
    iVar4 = 0;
    if (local_98 != *(int *)(iVar5 + 4)) {
      iVar4 = local_98 + 0x14;
    }
    if (iVar4 == 0) {
      iVar4 = 0;
    }
    else {
      iVar4 = *(int *)(iVar4 + 4);
    }
    if (pcVar8 == (code *)0x0) {
      iVar5 = 0;
    }
    else {
      uVar14 = 0x1bc2b4c;
      uVar13 = 0x1bc2a2000000000;
      uVar3 = FUN_015cea10(4);
      iVar5 = __RTDynamicCast(uVar3,uVar13,uVar14);
      if (iVar5 != 0) {
        local_70 = (code *)(local_80 & 0xffff);
        local_74 = (uint)*(ushort *)((int)pcVar8 + 0x2c);
        iVar5 = FUN_015e33f0(&local_74,&local_70);
        if (iVar5 != 0) {
          iVar5 = (uint)*(ushort *)((int)pcVar8 + 0xe) + *(int *)(iVar5 + 0x10);
          pcVar10 = local_78;
          goto LAB_012cefb5;
        }
      }
      iVar5 = 0;
      pcVar10 = local_78;
    }
LAB_012cefb5:
    pcVar9 = (code *)((float)((int)(local_84 + iVar4) * iVar5) / DAT_01b02a84);
    break;
  case 4:
    if (pcVar8 == (code *)0x0) {
      iVar4 = 0;
      iVar5 = 0;
    }
    else {
      uVar14 = 0x1bc2b4c;
      uVar13 = 0x1bc2a2000000000;
      uVar3 = FUN_015cea10(4);
      iVar4 = __RTDynamicCast(uVar3,uVar13,uVar14);
      if (iVar4 == 0) {
        iVar4 = 0;
      }
      else {
        local_70 = (code *)(local_80 & 0xffff);
        local_74 = (uint)*(ushort *)((int)pcVar8 + 0x2c);
        iVar4 = FUN_015e33f0(&local_74,&local_70);
        if (iVar4 == 0) {
          iVar4 = 0;
        }
        else {
          iVar4 = (int)((float)*(ushort *)((int)pcVar8 + 8) * local_8c) + *(int *)(iVar4 + 4);
        }
      }
      uVar14 = 0x1bc2b4c;
      uVar13 = 0x1bc2a2000000000;
      uVar3 = FUN_015cea10(4);
      iVar6 = __RTDynamicCast(uVar3,uVar13,uVar14);
      iVar5 = 0;
      if (iVar6 != 0) {
        local_70 = (code *)(local_80 & 0xffff);
        local_74 = (uint)*(ushort *)((int)pcVar8 + 0x2c);
        local_80 = FUN_015e33f0(&local_74,&local_70);
        iVar5 = 0;
        if (local_80 != 0) {
          uVar14 = 0x1bc2b28;
          uVar13 = 0x1bc2a2000000000;
          uVar3 = FUN_015cea10(0xd);
          iVar5 = __RTDynamicCast(uVar3,uVar13,uVar14);
          if (iVar5 == 0) {
            fVar11 = 0.0;
          }
          else {
            local_8c = (float)(uint)*(byte *)((int)pcVar8 + 0x36);
            iVar5 = FUN_015e5990();
            fVar11 = (float)(iVar5 + -100) * DAT_01b02528;
          }
          iVar5 = (int)((float)*(int *)(local_80 + 4) * fVar11);
        }
      }
    }
    iVar6 = local_7c;
    local_74 = iVar5 + iVar4;
    local_70 = (code *)0x6;
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (iVar4 = local_98, 6 < *(int *)(local_98 + 0x10))) {
      iVar4 = *(int *)(iVar6 + 4);
    }
    iVar5 = *(int *)(iVar6 + 4);
    iVar6 = 0;
    if (iVar4 != iVar5) {
      iVar6 = iVar4 + 0x14;
    }
    if (iVar6 == 0) {
      iVar4 = 0;
    }
    else {
      iVar4 = *(int *)(iVar6 + 4);
    }
    local_70 = (code *)0x6;
    fVar11 = (float)(int)(local_78 + iVar4);
    FUN_00f1d720(local_a0,&local_70);
    if ((*(char *)(local_98 + 0xd) != '\0') || (6 < *(int *)(local_98 + 0x10))) {
      local_98 = iVar5;
    }
    iVar4 = 0;
    if (local_98 != iVar5) {
      iVar4 = local_98 + 0x14;
    }
    if (iVar4 != 0) goto LAB_012cf199;
    iVar4 = 0;
LAB_012cf19c:
    FUN_00f222a0(local_68,0x20,u__d____d_01aebc98,
                 (int)((fVar11 / DAT_01b02970) * (float)(int)local_74),
                 (int)(((float)(int)(local_84 + iVar4) / DAT_01b02970) * (float)(int)local_74));
    goto LAB_012cf216;
  }
  FUN_00f222a0(local_68,0x20,u___2f_______2f___01aebc58,(double)(float)pcVar10,(double)(float)pcVar9
              );
LAB_012cf216:
  (**(code **)(**(int **)(local_94 + 0x188) + 0x10))();
  ExceptionList = local_1c;
  return;
}


