
/* ============================================================ */
/* Function: FUN_010c9870 */
/* Entry: 010c9870 */
/* Signature: undefined FUN_010c9870() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_010c9870(int param_1,ushort *param_2)

{
  ushort uVar1;
  short sVar2;
  code *pcVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  int iVar7;
  void *pvVar8;
  int *piVar9;
  uint uVar10;
  short *psVar11;
  undefined4 *puVar12;
  int iVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  undefined8 uStack_f4;
  undefined4 ***pppuStack_ec;
  undefined4 uStack_e8;
  undefined8 uStack_e4;
  undefined4 uStack_dc;
  int *piStack_d8;
  undefined4 *puStack_d4;
  int *local_a8;
  int local_a4;
  undefined8 local_a0;
  undefined1 *puStack_9c;
  int local_98;
  undefined4 ***local_54 [4];
  int local_44;
  uint local_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  int local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01980d73;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if (*(int *)(param_1 + 0xac) != 0) {
    iVar7 = *(int *)(param_1 + 0xa8);
    uVar1 = *param_2;
    FUN_00f48220();
    if ((((*(char *)(local_98 + 0xd) == '\0') && (*(ushort *)(local_98 + 0x10) <= uVar1)) &&
        (local_98 != *(int *)(iVar7 + 0xc))) &&
       ((*(int *)(iVar7 + 0x98) != 0 && (iVar7 = FUN_015d17e0(), iVar7 != 0)))) {
      pvVar8 = (void *)FUN_016830c0();
      local_14 = 0;
      if (pvVar8 == (void *)0x0) {
        local_a4 = 0;
      }
      else {
        puStack_d4 = (undefined4 *)0x10c995f;
        memset(pvVar8,0,0x2c);
        local_a4 = FUN_010baa10();
      }
      local_14 = 0xffffffff;
      if (local_a4 != 0) {
        pvVar8 = (void *)FUN_016830c0();
        local_14 = 1;
        if (pvVar8 == (void *)0x0) {
          local_a8 = (int *)0x0;
        }
        else {
          puStack_d4 = (undefined4 *)0x10c99b7;
          memset(pvVar8,0,0xa0);
          local_a8 = (int *)FUN_0123de10();
        }
        piVar9 = DAT_01bee890;
        uVar4 = DAT_01bee88c;
        local_14 = 0xffffffff;
        if (local_a8 != (int *)0x0) {
          puStack_9c = (undefined1 *)(DAT_01bb9b2c >> 0x20);
          FUN_00f19f10();
          local_2c = 0;
          uStack_38 = 0;
          uStack_34 = 0;
          uStack_30 = 0;
          local_28 = 0xf;
          local_3c = 0;
          puStack_d4 = &local_3c;
          local_14 = 2;
          piStack_d8 = (int *)0x10c9a50;
          FUN_015010a0();
          FUN_0124ef30();
          puStack_d4 = (undefined4 *)0x0;
          piStack_d8 = (int *)0x10c9a85;
          piStack_d8 = (int *)FUN_0124f150();
          uStack_dc = 0x47;
          uStack_e4 = CONCAT44(0xde,piVar9);
          uStack_e8 = uVar4;
          pppuStack_ec = (undefined4 ****)0x0;
          uStack_f4 = CONCAT44(0x10c9a9d,(undefined4)uStack_f4);
          FUN_0123e330();
          local_14 = 0xffffffff;
          FUN_00f1a420();
          puStack_d4 = (undefined4 *)0xffffffff;
          piStack_d8 = local_a8;
          uStack_dc = 0x10c9aca;
          piVar9 = (int *)FUN_01230f40();
          if (piVar9 != (int *)0x0) {
            (**(code **)(*piVar9 + 0x54))();
          }
        }
        pvVar8 = (void *)FUN_016830c0();
        local_14 = 3;
        if (pvVar8 == (void *)0x0) {
          piVar9 = (int *)0x0;
        }
        else {
          puStack_d4 = (undefined4 *)0x10c9b06;
          memset(pvVar8,0,0xa0);
          piVar9 = (int *)FUN_0123de10();
        }
        local_14 = 0xffffffff;
        if (piVar9 != (int *)0x0) {
          puVar12 = (undefined4 *)(iVar7 + 0xc);
          puStack_9c = &DAT_3f7ae148;
          if (0xf < *(uint *)(iVar7 + 0x20)) {
            puVar12 = (undefined4 *)*puVar12;
          }
          piStack_d8 = (int *)0x15d;
          uStack_dc = 0x243;
          puStack_d4 = (undefined4 *)_DAT_01b03150;
          uStack_e4 = CONCAT44(DAT_01bee890,DAT_01bee88c);
          uStack_e8 = 1;
          uStack_f4 = 0x3f7ae1483f7ae148;
          pppuStack_ec = (undefined4 ***)&DAT_3f7ae148;
          FUN_0123e200(0,puVar12,&uStack_f4);
          (**(code **)(*piVar9 + 0x38))();
          puStack_d4 = (undefined4 *)DAT_01bee88c;
          uStack_dc = 0x10c9bc5;
          piStack_d8 = piVar9;
          piVar9 = (int *)FUN_01230f40();
          if (piVar9 != (int *)0x0) {
            (**(code **)(*piVar9 + 0x54))();
          }
        }
        pvVar8 = (void *)FUN_016830c0();
        local_14 = 4;
        if (pvVar8 == (void *)0x0) {
          uVar10 = 0;
        }
        else {
          puStack_d4 = (undefined4 *)0x10c9c01;
          memset(pvVar8,0,0x1c0);
          uVar10 = FUN_01222670();
        }
        local_14 = 0xffffffff;
        if (uVar10 != 0) {
          puStack_d4 = (undefined4 *)0xdc;
          piStack_d8 = DAT_01bee890;
          uStack_dc = DAT_01bee88c;
          uStack_e4 = 0x10c9c43;
          FUN_012228b0();
          FUN_0124eff0();
          uVar4 = DAT_01b02768;
          *(undefined4 *)(uVar10 + 0xe0) = 0;
          *(undefined4 *)(uVar10 + 0xe4) = 0;
          *(undefined4 *)(uVar10 + 0xe8) = uVar4;
          *(undefined4 *)(uVar10 + 0x118) = 0x3f800000;
          *(undefined4 *)(uVar10 + 0x11c) = 0x3f800000;
          *(undefined4 *)(uVar10 + 0x140) = 0;
          *(undefined4 *)(uVar10 + 0x144) = 0;
          fVar20 = (float)puStack_9c * 0.0;
          *(float *)(uVar10 + 0xf0) = fVar20;
          fVar21 = (float)pvVar8 * 69.0;
          fVar16 = fVar20 + fVar20;
          *(undefined4 *)(uVar10 + 0x108) = uVar4;
          fVar15 = fVar20 * DAT_01b02858;
          *(float *)(uVar10 + 0xf4) = fVar21;
          *(float *)(uVar10 + 0xec) = fVar21;
          fVar19 = fVar21 + fVar21;
          *(float *)(uVar10 + 0x100) = fVar16;
          *(undefined4 *)(uVar10 + 0xf8) = uVar4;
          fVar18 = fVar21 * DAT_01b02858;
          fVar14 = fVar20 * DAT_01b02890;
          *(float *)(uVar10 + 0x130) = fVar16;
          fVar17 = fVar21 * DAT_01b02890;
          *(float *)(uVar10 + 0x160) = fVar14;
          fVar14 = fVar20 * DAT_01b02898;
          *(float *)(uVar10 + 0x15c) = fVar17;
          *(float *)(uVar10 + 0x164) = fVar17;
          fVar17 = fVar21 * DAT_01b02898;
          *(float *)(uVar10 + 0xfc) = fVar19;
          *(float *)(uVar10 + 0x104) = fVar19;
          *(float *)(uVar10 + 0x10c) = fVar18;
          *(float *)(uVar10 + 0x110) = fVar15;
          *(float *)(uVar10 + 0x114) = fVar18;
          *(undefined4 *)(uVar10 + 0x148) = uVar4;
          *(float *)(uVar10 + 0x14c) = fVar21;
          *(float *)(uVar10 + 0x120) = fVar20;
          *(float *)(uVar10 + 0x124) = fVar21;
          *(undefined4 *)(uVar10 + 0x128) = uVar4;
          *(float *)(uVar10 + 300) = fVar19;
          *(float *)(uVar10 + 0x134) = fVar19;
          *(undefined4 *)(uVar10 + 0x138) = uVar4;
          *(float *)(uVar10 + 0x13c) = fVar18;
          *(float *)(uVar10 + 0x150) = fVar15;
          *(float *)(uVar10 + 0x154) = fVar18;
          *(undefined4 *)(uVar10 + 0x158) = uVar4;
          *(undefined4 *)(uVar10 + 0x168) = uVar4;
          *(float *)(uVar10 + 0x16c) = fVar17;
          *(float *)(uVar10 + 0x170) = fVar14;
          *(float *)(uVar10 + 0x174) = fVar17;
          *(undefined4 *)(uVar10 + 0x178) = uVar4;
          *(undefined4 *)(uVar10 + 0x188) = uVar4;
          fVar14 = fVar20 * DAT_01b0289c;
          *(undefined4 *)(uVar10 + 0x1a0) = 0;
          fVar17 = fVar21 * DAT_01b0289c;
          *(undefined4 *)(uVar10 + 0x1a4) = 0;
          fVar15 = fVar21 * DAT_01b028b0;
          fVar20 = fVar20 * DAT_01b028b0;
          *(float *)(uVar10 + 0x180) = fVar14;
          fVar14 = fVar21 * DAT_01b028b4;
          *(float *)(uVar10 + 0x17c) = fVar17;
          *(float *)(uVar10 + 0x184) = fVar17;
          *(float *)(uVar10 + 0x19c) = fVar14;
          *(float *)(uVar10 + 0x18c) = fVar15;
          *(float *)(uVar10 + 400) = fVar20;
          *(float *)(uVar10 + 0x194) = fVar15;
          *(undefined4 *)(uVar10 + 0x198) = uVar4;
          *(undefined4 *)(uVar10 + 0x1a8) = uVar4;
          *(float *)(uVar10 + 0x1ac) = fVar21;
          *(float *)(uVar10 + 0x1b0) = fVar16;
          *(float *)(uVar10 + 0x1b4) = fVar19;
          *(undefined4 *)(uVar10 + 0x1b8) = uVar4;
          *(float *)(uVar10 + 0x1bc) = fVar18;
          puStack_d4 = (undefined4 *)DAT_01bee88c;
          piStack_d8 = DAT_01bee890;
          uStack_dc = DAT_01bee88c;
          uStack_e4 = (ulonglong)uVar10;
          uStack_e8 = 0x10c9eee;
          piVar9 = (int *)FUN_012317d0();
          if (piVar9 != (int *)0x0) {
            pcVar3 = *(code **)(*piVar9 + 0x24);
            iVar13 = *(int *)(param_1 + 0xa8);
            uVar1 = *param_2;
            FUN_00f48220();
            if (((*(char *)(local_28 + 0xd) == '\0') && (*(ushort *)(local_28 + 0x10) <= uVar1)) &&
               (local_28 != *(int *)(iVar13 + 0xc))) {
              FUN_01036690();
            }
            (*pcVar3)();
            (**(code **)(*piVar9 + 0x54))();
          }
        }
        local_14 = 5;
        psVar11 = (short *)(iVar7 + 0x3c);
        local_28 = 0x3f63e3e4;
        if (7 < *(uint *)(iVar7 + 0x50)) {
          psVar11 = *(short **)psVar11;
        }
        do {
          sVar2 = *psVar11;
          psVar11 = psVar11 + 1;
        } while (sVar2 != 0);
        FUN_00f1a040();
        puStack_d4 = (undefined4 *)0x10ca040;
        piVar9 = (int *)FUN_01230c40();
        if (piVar9 != (int *)0x0) {
          (**(code **)(*piVar9 + 0x54))();
        }
        local_14 = 0xffffffff;
        FUN_00f1a330();
        FUN_010ccc80();
        local_14 = 6;
        if (local_44 != 0) {
          pvVar8 = (void *)FUN_016830c0();
          local_14._0_1_ = 7;
          if (pvVar8 == (void *)0x0) {
            piVar9 = (int *)0x0;
          }
          else {
            puStack_d4 = (undefined4 *)0x10ca0ba;
            memset(pvVar8,0,0xa0);
            piVar9 = (int *)FUN_0123de10();
          }
          local_14 = CONCAT31(local_14._1_3_,6);
          if (piVar9 != (int *)0x0) {
            pppuStack_ec = local_54;
            if (0xf < local_40) {
              pppuStack_ec = local_54[0];
            }
            puStack_d4 = (undefined4 *)DAT_01bee88c;
            piStack_d8 = (int *)0x1;
            uStack_e8 = 0;
            uStack_e4 = DAT_01bb9b2c;
            uStack_dc = DAT_01bb9b34;
            uStack_f4 = 0x10ca114;
            FUN_0123e0d0();
            puStack_d4 = (undefined4 *)DAT_01bee88c;
            uStack_dc = 0x10ca12b;
            piStack_d8 = piVar9;
            piVar9 = (int *)FUN_01230f40();
            if ((piVar9 != (int *)0x0) && ((**(code **)(*piVar9 + 0x54))(), local_44 == 0)) {
              (**(code **)(*piVar9 + 0x40))();
            }
          }
        }
        puVar12 = (undefined4 *)FUN_016830c0();
        if (puVar12 != (undefined4 *)0x0) {
          *puVar12 = 0;
          puVar12[1] = 0;
          puVar12[2] = 0;
          puVar12[3] = 0;
          *(undefined8 *)(puVar12 + 4) = 0;
          uVar4 = *(undefined4 *)param_2;
          puVar12[6] = 0;
          *puVar12 = &DAT_01ae673c;
          puVar12[1] = 0;
          puVar12[2] = 0;
          puVar12[3] = uVar4;
          *(undefined2 *)(puVar12 + 4) = 0;
          puVar12[5] = 0;
          *(undefined2 *)(puVar12 + 6) = 1;
          FUN_0122b3e0();
          iVar13 = 0;
          iVar7 = *(int *)(param_1 + 0xac);
          puVar5 = *(undefined4 **)(iVar7 + 100);
          for (puVar6 = (undefined4 *)*puVar5; puVar6 != puVar5; puVar6 = (undefined4 *)*puVar6) {
            if ((undefined4 *)puVar6[2] == puVar12) {
              if (iVar13 != -1) goto LAB_010ca22c;
              break;
            }
            iVar13 = iVar13 + 1;
          }
          if (*(int *)(iVar7 + 0x78) < (int)puVar12[5]) {
            *(undefined4 *)(iVar7 + 0x78) = puVar12[5];
            puVar12 = (undefined4 *)*puVar5;
            if (puVar12 != puVar5) {
              do {
                *(undefined4 *)(puVar12[2] + 0x14) = *(undefined4 *)(iVar7 + 0x78);
                puVar12 = (undefined4 *)*puVar12;
              } while (puVar12 != *(undefined4 **)(iVar7 + 100));
            }
          }
          else {
            puVar12[5] = *(int *)(iVar7 + 0x78);
          }
          FUN_00f55430();
          FUN_0122c760();
        }
LAB_010ca22c:
        FUN_00f1a420();
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


