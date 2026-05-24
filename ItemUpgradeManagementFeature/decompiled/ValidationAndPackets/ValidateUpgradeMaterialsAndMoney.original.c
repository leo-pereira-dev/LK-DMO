
/* ============================================================ */
/* Function: FUN_012b5fb0 */
/* Entry: 012b5fb0 */
/* Signature: undefined FUN_012b5fb0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 __thiscall
FUN_012b5fb0(int param_1,uint *param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
            undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  bool bVar9;
  bool bVar10;
  char *pcVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 local_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined8 local_9c;
  undefined4 local_94;
  undefined1 local_90 [16];
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 *puStack_78;
  undefined4 uStack_74;
  undefined8 local_70;
  undefined4 local_68;
  byte *local_64;
  int local_60;
  int local_5c;
  int *local_58;
  int local_54;
  undefined1 local_4d;
  int local_4c;
  undefined1 local_45;
  int local_44;
  int iStack_40;
  int iStack_3c;
  int iStack_38;
  undefined8 local_34;
  undefined1 local_28 [4];
  int local_24;
  int local_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019a0615;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  if ((((*(int *)(param_1 + 0x148) == 0) || (local_54 = DAT_01bfb2b4, DAT_01bfb2b4 == 0)) ||
      (DAT_01bfb2b4 == -0x28)) || (199 < *param_2)) {
    return 0;
  }
  piVar8 = (int *)(DAT_01bfb2b4 + 0x58 + *param_2 * 0x45);
  if (piVar8 == (int *)0x0) {
    return 0;
  }
  ExceptionList = &local_10;
  local_60 = param_1;
  local_58 = piVar8;
  uVar3 = FUN_015ec4f0(*piVar8);
  if (uVar3 == 0) {
    ExceptionList = local_10;
    return 0;
  }
  local_64 = (byte *)((int)piVar8 + 9);
  if (*(byte *)((int)piVar8 + 9) == uVar3) {
    uVar13 = 0;
    uVar14 = 0;
    uVar15 = 0;
    uVar16 = 0;
    uVar17 = 0;
    uVar18 = 0;
    FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_MAX_GRADE_01aeb164,0x1d);
    FUN_010052f0(uVar13,uVar14,uVar15,uVar16,uVar17,uVar18);
    ExceptionList = local_10;
    return 0;
  }
  local_1c = 0;
  FUN_015ec940(&local_1c,*piVar8,*local_64 + 1);
  local_8 = 0;
  if (local_1c._4_4_ != 0) {
    piVar7 = (int *)local_1c;
    piVar8 = (int *)*(int *)local_1c;
    if (piVar8 != (int *)local_1c) {
      local_4d = 0;
      local_45 = 0;
      do {
        local_5c = piVar8[3];
        local_4c = FUN_00f62070(&local_5c,&local_4d);
        local_5c = piVar8[4];
        iVar4 = FUN_00f62070(&local_5c,&local_45);
        if (local_4c + iVar4 < piVar8[5]) {
          uVar13 = 0;
          uVar14 = 0;
          uVar15 = 0;
          uVar16 = 0;
          uVar17 = 0;
          uVar18 = 0;
          FUN_00f19cc0(s_EQUIPSYSTEM_UPGRADE_MATERIAL_LAC_01aeb23c,0x21);
          goto LAB_012b62b3;
        }
        piVar8 = (int *)*piVar8;
      } while (piVar8 != piVar7);
    }
    iVar4 = local_60;
    if (param_5._2_2_ == 0) {
LAB_012b61cc:
      if (param_8._2_2_ != 0) {
        iVar5 = FUN_015ec600(local_90,*local_58,*local_64 + 1);
        local_4c = *(int *)(iVar5 + 0x28);
        *(undefined4 *)puStack_78[1] = 0;
        puVar2 = (undefined4 *)*puStack_78;
        while (puVar2 != (undefined4 *)0x0) {
          puVar1 = (undefined4 *)*puVar2;
          FUN_017986fc(puVar2,0x18);
          puVar2 = puVar1;
        }
        FUN_017986fc(puStack_78,0x18);
        FUN_015ec470(local_28,local_4c);
        local_4c = local_24;
        local_45 = 0;
        iVar5 = FUN_00f62070(&local_4c,&local_45);
        local_45 = 0;
        local_4c = local_20;
        iVar6 = FUN_00f62070(&local_4c,&local_45);
        if (iVar5 + iVar6 < (int)param_8._2_2_) goto LAB_012b628c;
      }
      piVar8 = (int *)FUN_012b5f30(&local_80);
      local_8._0_1_ = 1;
      local_4c = *local_64 + 1;
      local_5c = *local_58;
      piVar7 = (int *)FUN_012b7900(&local_ac,&local_5c,&local_4c);
      iVar6 = DAT_01bfb2b4;
      local_44 = *piVar7;
      iStack_40 = piVar7[1];
      iStack_3c = piVar7[2];
      iStack_38 = piVar7[3];
      local_34 = *(undefined8 *)(piVar7 + 4);
      iVar5 = *piVar8;
      bVar10 = SBORROW4(iVar5,local_44);
      bVar9 = iVar5 - local_44 < 0;
      if (iVar5 == local_44) {
        iVar5 = piVar8[1];
        bVar10 = SBORROW4(iVar5,iStack_40);
        bVar9 = iVar5 - iStack_40 < 0;
        if (iVar5 == iStack_40) {
          bVar10 = SBORROW4(piVar8[2],iStack_3c);
          bVar9 = piVar8[2] - iStack_3c < 0;
        }
      }
      local_8 = (uint)local_8._1_3_ << 8;
      local_9c = 0;
      local_94 = 0;
      local_70 = 0;
      local_68 = 0;
      local_ac = 0;
      uStack_a8 = 0;
      uStack_a4 = 0;
      uStack_a0 = 0;
      local_80 = 0;
      uStack_7c = 0;
      puStack_78 = (undefined4 *)0x0;
      uStack_74 = 0;
      if (bVar10 == bVar9) {
        if (((*(int *)(*(int *)(iVar4 + 0x154) + 4) != 0) && (DAT_01bfb2b4 != 0)) &&
           (DAT_01bfb2b4 != -0x28)) {
          if (*DAT_01bfe460 != 0) {
            FUN_014e3cc0();
          }
          local_4c = *(int *)(iVar6 + 0x44);
          iVar4 = 0;
          if (0 < local_4c) {
            do {
              FUN_00f63330(iVar4);
              iVar4 = iVar4 + 1;
            } while (iVar4 < local_4c);
          }
        }
        local_4d = 1;
        goto LAB_012b62bc;
      }
      uVar12 = 0x1e;
      pcVar11 = s_EQUIPSYSTEM_UPGRADE_MONEY_LACK_01ace3dc;
    }
    else {
      iVar5 = FUN_015ec600(local_90,*local_58,*local_64 + 1);
      local_4c = *(int *)(iVar5 + 0x24);
      *(undefined4 *)puStack_78[1] = 0;
      puVar2 = (undefined4 *)*puStack_78;
      while (puVar2 != (undefined4 *)0x0) {
        puVar1 = (undefined4 *)*puVar2;
        FUN_017986fc(puVar2,0x18);
        puVar2 = puVar1;
      }
      FUN_017986fc(puStack_78,0x18);
      FUN_015ec470(local_28,local_4c);
      local_4c = local_24;
      local_45 = 0;
      iVar5 = FUN_00f62070(&local_4c,&local_45);
      local_4c = local_20;
      local_45 = 0;
      iVar6 = FUN_00f62070(&local_4c,&local_45);
      if ((int)param_5._2_2_ <= iVar5 + iVar6) goto LAB_012b61cc;
LAB_012b628c:
      uVar12 = 0x2c;
      pcVar11 = s_EQUIPSYSTEM_UPGRADE_ADDITIONAL_M_01aeb20c;
    }
    uVar13 = 0;
    uVar14 = 0;
    uVar15 = 0;
    uVar16 = 0;
    uVar17 = 0;
    uVar18 = 0;
    FUN_00f19cc0(pcVar11,uVar12);
LAB_012b62b3:
    FUN_010052f0(uVar13,uVar14,uVar15,uVar16,uVar17,uVar18);
  }
  local_4d = 0;
LAB_012b62bc:
  *(undefined4 *)((int *)local_1c)[1] = 0;
  puVar2 = (undefined4 *)*(int *)local_1c;
  while (puVar2 != (undefined4 *)0x0) {
    puVar1 = (undefined4 *)*puVar2;
    FUN_017986fc(puVar2,0x18);
    puVar2 = puVar1;
  }
  FUN_017986fc((int *)local_1c,0x18);
  ExceptionList = local_10;
  return local_4d;
}


