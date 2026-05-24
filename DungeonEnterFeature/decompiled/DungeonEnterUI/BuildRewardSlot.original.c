
/* ============================================================ */
/* Function: FUN_010ca670 */
/* Entry: 010ca670 */
/* Signature: undefined FUN_010ca670() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall
FUN_010ca670(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  uint uVar3;
  void *pvVar4;
  int iVar5;
  undefined4 uVar6;
  int *piVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined4 uVar10;
  undefined8 uVar11;
  undefined4 uVar12;
  int local_40;
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
  puStack_18 = &LAB_01980e5e;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if (*(int *)(param_1 + 0xb0) != 0) {
    local_24 = uVar3;
    pvVar4 = (void *)FUN_016830c0(0x2c,uVar3);
    local_14 = 0;
    if (pvVar4 == (void *)0x0) {
      iVar5 = 0;
    }
    else {
      memset(pvVar4,0,0x2c);
      iVar5 = FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (iVar5 != 0) {
      pvVar4 = (void *)FUN_016830c0(0xa0,uVar3);
      local_14 = 1;
      if (pvVar4 == (void *)0x0) {
        local_40 = 0;
      }
      else {
        memset(pvVar4,0,0xa0);
        local_40 = FUN_0123de10();
      }
      uVar2 = DAT_01bee890;
      uVar1 = DAT_01bee88c;
      uVar12 = DAT_01bb9b34;
      uVar11 = DAT_01bb9b2c;
      local_14 = 0xffffffff;
      if (local_40 != 0) {
        FUN_00f19f10();
        local_2c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_30 = 0;
        local_28 = 0xf;
        local_3c = 0;
        local_14 = 2;
        FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_DungeonUI_Dungeon_Slot_tga_01ad4b60);
        FUN_0124ef30(&local_3c);
        uVar10 = 0;
        uVar6 = FUN_0124f150(0,uVar11,uVar12);
        FUN_0123e330(0,uVar1,uVar2,0x2c,0x2c,uVar6,uVar10,uVar11,uVar12);
        local_14 = 0xffffffff;
        FUN_00f1a420();
        piVar7 = (int *)FUN_01230f40(local_40,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
        if (piVar7 != (int *)0x0) {
          (**(code **)(*piVar7 + 0x54))(1);
        }
      }
      piVar7 = (int *)FUN_01231410(10,*param_3,*param_4,0x28,0x28,2,2);
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))(1);
      }
      puVar8 = (undefined4 *)FUN_016830c0(0x28);
      if (puVar8 != (undefined4 *)0x0) {
        *puVar8 = 0;
        puVar8[1] = 0;
        puVar8[2] = 0;
        puVar8[3] = 0;
        puVar8[4] = 0;
        puVar8[5] = 0;
        puVar8[6] = 0;
        puVar8[7] = 0;
        uVar12 = *param_2;
        *(undefined8 *)(puVar8 + 8) = 0;
        *puVar8 = &DAT_01ae66e0;
        puVar8[1] = 0;
        puVar8[2] = 0;
        puVar8[3] = uVar12;
        *(undefined1 *)(puVar8 + 4) = 0;
        puVar8[5] = 0x2c;
        puVar8[6] = 0x2c;
        *(undefined2 *)(puVar8 + 7) = 0x101;
        puVar8[8] = 0;
        puVar8[9] = 0;
        FUN_012288c0(iVar5);
        puVar9 = operator_new(8);
        uVar12 = *param_3;
        *puVar9 = &DAT_01ad4eb4;
        puVar9[1] = uVar12;
        if ((undefined4 *)puVar8[2] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar8[2])(1);
        }
        puVar8[2] = puVar9;
        FUN_0122a090(puVar8);
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


