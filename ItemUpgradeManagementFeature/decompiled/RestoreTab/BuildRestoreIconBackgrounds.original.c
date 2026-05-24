
/* ============================================================ */
/* Function: FUN_0126a270 */
/* Entry: 0126a270 */
/* Signature: undefined FUN_0126a270() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0126a270(int param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  undefined4 *puVar6;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 uVar7;
  int iVar8;
  undefined4 uVar9;
  undefined8 uVar10;
  int local_88;
  undefined4 *local_80 [2];
  undefined8 local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined1 local_54;
  undefined4 local_50;
  int local_4c;
  int local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199b8f8;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_80[0] = (undefined4 *)FUN_016830c0(0x94,local_24);
  local_14 = 0;
  if (local_80[0] == (undefined4 *)0x0) {
    iVar3 = 0;
    uVar7 = extraout_ECX;
  }
  else {
    memset(local_80[0],0,0x94);
    iVar3 = FUN_0122b460();
    uVar7 = extraout_ECX_00;
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0x84) = iVar3;
  if (iVar3 != 0) {
    FUN_0122b6f0(*(undefined4 *)(param_1 + 0x40),0xee,0x1f,0x96,0x96,0,uVar7);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x84));
    FUN_0122c320();
    local_80[0] = (undefined4 *)FUN_016830c0(0x2c);
    local_14 = 1;
    if (local_80[0] == (undefined4 *)0x0) {
      iVar3 = 0;
    }
    else {
      memset(local_80[0],0,0x2c);
      iVar3 = FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (iVar3 != 0) {
      local_80[0] = (undefined4 *)FUN_016830c0(0xa0);
      local_14 = 2;
      if (local_80[0] == (undefined4 *)0x0) {
        local_88 = 0;
      }
      else {
        memset(local_80[0],0,0xa0);
        local_88 = FUN_0123de10();
      }
      uVar2 = DAT_01bee890;
      uVar7 = DAT_01bb9b34;
      uVar10 = DAT_01bb9b2c;
      local_14 = 0xffffffff;
      if (local_88 != 0) {
        local_80[0] = DAT_01bee88c;
        FUN_00f19f10();
        local_2c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_30 = 0;
        local_28 = 0xf;
        local_3c = 0;
        local_14 = 3;
        FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_EquipSystem_Upgrade_IC_BG_tga_01ae8b0c)
        ;
        FUN_0124ef30(&local_3c);
        uVar9 = 0;
        uVar4 = FUN_0124f150(0,uVar10,uVar7);
        FUN_0123e330(0,local_80[0],uVar2,0x96,0x96,uVar4,uVar9,uVar10,uVar7);
        local_14 = 0xffffffff;
        FUN_00f1a420();
        piVar5 = (int *)FUN_01230f40(local_88,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        if (piVar5 != (int *)0x0) {
          (**(code **)(*piVar5 + 0x54))(1);
        }
      }
      local_5c = 0;
      local_58 = 7;
      uStack_68 = 0;
      uStack_64 = 0;
      uStack_60 = 0;
      local_6c = 0;
      local_40 = 0;
      local_14 = 4;
      local_4c = DAT_01bfe468 + 8;
      local_48 = 8;
      local_78 = DAT_01bb9b2c;
      local_70 = DAT_01bb9b34;
      local_54 = 1;
      local_44 = 0;
      local_50 = 1;
      FUN_00f1a040();
      piVar5 = (int *)FUN_01230c40(&local_78,0x4b,0x4c - local_48);
      *(int **)(param_1 + 0x8c) = piVar5;
      if (piVar5 != (int *)0x0) {
        (**(code **)(*piVar5 + 0x54))(1);
      }
      local_80[0] = (undefined4 *)FUN_016830c0(0x1c);
      if (local_80[0] == (undefined4 *)0x0) {
        *(undefined4 *)(param_1 + 0x90) = 0;
        local_80[0] = (undefined4 *)0x0;
      }
      else {
        *local_80[0] = 0;
        local_80[0][1] = 0;
        local_80[0][2] = 0;
        local_80[0][3] = 0;
        *(undefined8 *)(local_80[0] + 4) = 0;
        local_80[0][6] = 0;
        *local_80[0] = &DAT_01ae673c;
        local_80[0][1] = 0;
        local_80[0][2] = 0;
        local_80[0][3] = 0xffffffff;
        *(undefined2 *)(local_80[0] + 4) = 0;
        local_80[0][5] = 0;
        *(undefined2 *)(local_80[0] + 6) = 1;
        *(undefined4 **)(param_1 + 0x90) = local_80[0];
        FUN_0122b3e0(iVar3);
        iVar3 = *(int *)(param_1 + 0x90);
        puVar6 = operator_new(0xc);
        *puVar6 = &DAT_01ae8cac;
        puVar6[1] = 0;
        puVar6[2] = 0;
        puVar1 = *(undefined4 **)(iVar3 + 8);
        if (puVar1 != (undefined4 *)0x0) {
          local_80[0] = puVar6;
          (**(code **)*puVar1)(1);
        }
        *(undefined4 **)(iVar3 + 8) = puVar6;
        iVar3 = *(int *)(param_1 + 0x84);
        local_80[0] = *(undefined4 **)(param_1 + 0x90);
        if (local_80[0] != (undefined4 *)0x0) {
          puVar1 = *(undefined4 **)(iVar3 + 100);
          iVar8 = 0;
          for (puVar6 = (undefined4 *)*puVar1; puVar6 != puVar1; puVar6 = (undefined4 *)*puVar6) {
            if ((undefined4 *)puVar6[2] == local_80[0]) {
              if (iVar8 != -1) goto LAB_0126a675;
              break;
            }
            iVar8 = iVar8 + 1;
          }
          if (*(int *)(iVar3 + 0x78) < (int)local_80[0][5]) {
            *(undefined4 *)(iVar3 + 0x78) = local_80[0][5];
            puVar6 = (undefined4 *)*puVar1;
            if (puVar6 != puVar1) {
              do {
                *(undefined4 *)(puVar6[2] + 0x14) = *(undefined4 *)(iVar3 + 0x78);
                puVar6 = (undefined4 *)*puVar6;
              } while (puVar6 != *(undefined4 **)(iVar3 + 100));
            }
          }
          else {
            local_80[0][5] = *(int *)(iVar3 + 0x78);
          }
          FUN_00f55430(local_80);
          FUN_0122c760();
        }
      }
LAB_0126a675:
      local_4c = 0;
      FUN_00f1a330();
    }
  }
  ExceptionList = local_1c;
  return;
}


