
/* ============================================================ */
/* Function: FUN_0125d660 */
/* Entry: 0125d660 */
/* Signature: undefined FUN_0125d660() */
/* ============================================================ */


void __thiscall FUN_0125d660(int param_1,char *param_2)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 local_50 [24];
  undefined1 local_38 [12];
  undefined1 *local_2c;
  undefined1 *puStack_28;
  undefined4 local_24;
  code *local_20;
  int *local_1c;
  uint local_18;
  code *local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199aeb7;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_18 = 0;
  if (*(int **)(param_1 + 0xbc) != (int *)0x0) {
    local_14 = *(code **)(**(int **)(param_1 + 0xbc) + 4);
    if (*param_2 == '\0') {
      local_2c = &DAT_3f7ae148;
      puStack_28 = (undefined1 *)0x3dcccccd;
      local_24 = 0x3dcccccd;
    }
    else {
      local_2c = (undefined1 *)0x0;
      puStack_28 = &DAT_3f7ae148;
      local_24 = 0;
    }
    (*local_14)(CONCAT44(puStack_28,local_2c),local_24,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
    local_1c = *(int **)(param_1 + 0xbc);
    cVar1 = *param_2;
    uVar3 = 0;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    local_20 = *(code **)(*local_1c + 0x10);
    if (cVar1 == '\0') {
      FUN_00f19cc0(s_CHANGE_GUILD_NAME_UNUSABLE_01ae81a8,0x1a);
      puVar2 = (undefined4 *)FUN_015ce480(local_38,uVar3,uVar4,uVar5,uVar6,uVar7,uVar8);
      local_18 = 2;
    }
    else {
      FUN_00f19cc0(s_CHANGE_GUILD_NAME_USABLE_01ae81c4,0x18);
      puVar2 = (undefined4 *)FUN_015ce480(local_50,uVar3,uVar4,uVar5,uVar6,uVar7,uVar8);
      local_18 = 1;
    }
    local_8 = (uint)(cVar1 == '\0');
    if (7 < (uint)puVar2[5]) {
      puVar2 = (undefined4 *)*puVar2;
    }
    local_14 = (code *)local_18;
    (*local_20)(puVar2);
    if (((uint)local_14 & 2) != 0) {
      local_14 = (code *)((uint)local_14 & 0xfffffffd);
      FUN_00f1a330();
    }
    local_8 = 0xffffffff;
    if (((uint)local_14 & 1) != 0) {
      FUN_00f1a330();
    }
  }
  if (*(int **)(param_1 + 0xb8) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xb8) + 0x18))(*param_2);
  }
  ExceptionList = local_10;
  return;
}


