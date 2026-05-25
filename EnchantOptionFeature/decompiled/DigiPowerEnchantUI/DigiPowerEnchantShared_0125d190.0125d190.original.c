
/* ============================================================ */
/* Function: FUN_0125d190 */
/* Entry: 0125d190 */
/* Signature: undefined FUN_0125d190() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0125d190(int param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  void *pvVar5;
  int iVar6;
  int *piVar7;
  undefined4 *puVar8;
  int iVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  int local_d0;
  undefined4 *local_c8;
  undefined4 local_c4;
  undefined4 uStack_c0;
  undefined4 local_bc;
  undefined1 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined8 local_88;
  undefined1 *local_80;
  undefined4 local_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined1 local_64;
  undefined4 local_60;
  int local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 *puStack_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined1 local_2c [16];
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199ae55;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if (*(int *)(param_1 + 0xc0) != 0) {
    FUN_0122c320(local_14);
    local_6c = 0;
    local_68 = 7;
    uStack_78 = 0;
    uStack_74 = 0;
    uStack_70 = 0;
    local_7c = 0;
    local_50 = 0;
    local_8 = 0;
    local_88 = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_5c = DAT_01bfe468 + 8;
    local_4c = 0;
    local_1c = CONCAT44(&DAT_3f7ae148,(undefined4 *)local_1c);
    local_58 = 0xb;
    local_80 = &DAT_3f7ae148;
    local_64 = 1;
    local_54 = 0;
    local_60 = 0;
    local_3c = 0;
    uStack_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    uStack_48 = 0;
    puStack_44 = (undefined4 *)0x0;
    uStack_40 = 0;
    puStack_44 = operator_new(0xc);
    *puStack_44 = puStack_44;
    puStack_44[1] = puStack_44;
    local_3c = CONCAT31(local_3c._1_3_,1);
    uStack_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    local_8._0_1_ = 1;
    if (DAT_01bfb8c4 != 0) {
      uVar10 = 0;
      uVar11 = 0;
      uVar12 = 0;
      uVar13 = 0;
      uVar14 = 0;
      uVar15 = 0;
      FUN_00f19cc0(s_CHANGE_GUILD_NAME_MSG_01ae817c,0x15);
      puVar3 = (undefined4 *)FUN_015ce480(local_2c,uVar10,uVar11,uVar12,uVar13,uVar14,uVar15);
      local_8._0_1_ = 2;
      if (7 < (uint)puVar3[5]) {
        puVar3 = (undefined4 *)*puVar3;
      }
      FUN_012543f0(&local_4c,400,puVar3,&local_88);
      local_8._0_1_ = 1;
      FUN_00f1a330();
      puVar2 = puStack_44;
      for (puVar3 = (undefined4 *)*puStack_44; puVar3 != puVar2; puVar3 = (undefined4 *)*puVar3) {
        if ((puVar3[2] != 0) &&
           (puVar4 = (undefined4 *)FUN_016830c0(0x1c), local_c8 = puVar4,
           puVar4 != (undefined4 *)0x0)) {
          *puVar4 = 0;
          puVar4[1] = 0;
          puVar4[2] = 0;
          puVar4[3] = 0;
          *(undefined8 *)(puVar4 + 4) = 0;
          puVar4[6] = 0;
          *puVar4 = &DAT_01ae673c;
          puVar4[1] = 0;
          puVar4[2] = 0;
          puVar4[3] = 0xffffffff;
          *(undefined2 *)(puVar4 + 4) = 0;
          puVar4[5] = 0;
          *(undefined2 *)(puVar4 + 6) = 1;
          pvVar5 = (void *)FUN_016830c0(0x2c);
          local_8._0_1_ = 3;
          if (pvVar5 == (void *)0x0) {
            iVar6 = 0;
          }
          else {
            memset(pvVar5,0,0x2c);
            iVar6 = FUN_010baa10();
          }
          local_8._0_1_ = 1;
          if (iVar6 == 0) {
            (**(code **)*puVar4)(1);
          }
          else {
            local_d0 = 400 - *(int *)(puVar3[2] + 0xc) >> 1;
            local_1c = 0;
            pvVar5 = operator_new(0xc);
            *(void **)pvVar5 = pvVar5;
            *(void **)((int)pvVar5 + 4) = pvVar5;
            local_1c = CONCAT44(local_1c._4_4_,pvVar5);
            local_8 = CONCAT31(local_8._1_3_,4);
            FUN_01232730(0,&local_1c);
            puVar8 = (undefined4 *)local_1c;
            for (puVar1 = (undefined4 *)*(undefined4 *)local_1c; puVar1 != puVar8;
                puVar1 = (undefined4 *)*puVar1) {
              if (puVar1[2] != 0) {
                local_c8 = (undefined4 *)__RTDynamicCast(puVar1[2],0,&DAT_01bc4bb0,&DAT_01bc4bd0,0);
                local_bc = local_c8[0x34];
                local_c4 = local_c8[0x32];
                uStack_c0 = local_c8[0x33];
                FUN_00f221a0(local_c8 + 0x35);
                local_a0 = *(undefined1 *)(local_c8 + 0x3b);
                local_9c = local_c8[0x3c];
                local_98 = local_c8[0x3d];
                local_94 = local_c8[0x3e];
                local_90 = local_c8[0x3f];
                local_8c = local_c8[0x40];
                local_8 = CONCAT31(local_8._1_3_,5);
                piVar7 = (int *)FUN_01230c40(&local_c4,local_d0,0);
                if (piVar7 != (int *)0x0) {
                  (**(code **)(*piVar7 + 0x54))(1);
                  local_d0 = local_d0 + local_c8[7];
                }
                local_8 = CONCAT31(local_8._1_3_,4);
                local_98 = 0;
                FUN_00f1a330();
              }
            }
            FUN_0122b3e0(iVar6);
            iVar9 = 0;
            iVar6 = *(int *)(param_1 + 0xc0);
            puVar1 = *(undefined4 **)(iVar6 + 100);
            for (puVar8 = (undefined4 *)*puVar1; puVar8 != puVar1; puVar8 = (undefined4 *)*puVar8) {
              if ((undefined4 *)puVar8[2] == puVar4) {
                local_c8 = puVar4;
                if (iVar9 != -1) goto LAB_0125d5dc;
                break;
              }
              iVar9 = iVar9 + 1;
            }
            if (*(int *)(iVar6 + 0x78) < (int)puVar4[5]) {
              *(undefined4 *)(iVar6 + 0x78) = puVar4[5];
              puVar8 = (undefined4 *)*puVar1;
              if (puVar8 != puVar1) {
                do {
                  *(undefined4 *)(puVar8[2] + 0x14) = *(undefined4 *)(iVar6 + 0x78);
                  puVar8 = (undefined4 *)*puVar8;
                } while (puVar8 != *(undefined4 **)(iVar6 + 100));
              }
            }
            else {
              puVar4[5] = *(int *)(iVar6 + 0x78);
            }
            local_c8 = puVar4;
            FUN_00f55430(&local_c8);
            FUN_0122c760();
LAB_0125d5dc:
            local_8._0_1_ = 1;
            *(undefined4 *)((undefined4 *)local_1c)[1] = 0;
            puVar4 = (undefined4 *)*(undefined4 *)local_1c;
            while (puVar4 != (undefined4 *)0x0) {
              puVar1 = (undefined4 *)*puVar4;
              FUN_017986fc(puVar4,0xc);
              puVar4 = puVar1;
            }
            FUN_017986fc((undefined4 *)local_1c,0xc);
          }
        }
      }
    }
    FUN_012327f0();
    local_5c = 0;
    FUN_00f1a330();
  }
  ExceptionList = local_10;
  return;
}


