
/* ============================================================ */
/* Function: FUN_01270660 */
/* Entry: 01270660 */
/* Signature: undefined FUN_01270660() */
/* ============================================================ */


void __fastcall FUN_01270660(int param_1)

{
  int iVar1;
  uint uVar2;
  void *pvVar3;
  int *piVar4;
  undefined4 *puVar5;
  float10 fVar6;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199bb39;
  local_10 = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  pvVar3 = (void *)FUN_016830c0(0x1ac,uVar2);
  local_8 = 0;
  if (pvVar3 == (void *)0x0) {
    piVar4 = (int *)0x0;
  }
  else {
    memset(pvVar3,0,0x1ac);
    piVar4 = (int *)FUN_012435e0();
  }
  local_8 = 0xffffffff;
  *(int **)(param_1 + 300) = piVar4;
  if (piVar4 != (int *)0x0) {
    (**(code **)(*piVar4 + 0xc))(*(undefined4 *)(param_1 + 0x40),0xaf,0x6a,0x70,2,0);
    iVar1 = *(int *)(param_1 + 300);
    *(undefined4 *)(iVar1 + 0xc0) = 0;
    *(undefined4 *)(iVar1 + 0xdc) = 0;
    puVar5 = (undefined4 *)(iVar1 + 0xb0);
    if (7 < *(uint *)(iVar1 + 0xc4)) {
      puVar5 = (undefined4 *)*puVar5;
    }
    *(undefined2 *)puVar5 = 0;
    FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
    FUN_01242970(s_EquipSystem_Upgrade_Empty_Line_t_01ae8b88);
    *(undefined4 *)(*(int *)(param_1 + 300) + 0xd4) = 0x3c23d70a;
    piVar4 = *(int **)(param_1 + 300);
    piVar4[0x3b] = 1;
    piVar4[0x3c] = 0;
    fVar6 = (float10)fmaxf(0,0x3f800000);
    piVar4[0x32] = (int)(float)fVar6;
    FUN_01243010();
    (**(code **)(*piVar4 + 0x48))();
    FUN_01242bf0();
    (**(code **)(**(int **)(param_1 + 300) + 0x1c))(1);
    FUN_01271c10(1,param_1,FUN_01271aa0,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 300));
    FUN_0126de20(1,*(undefined4 *)(param_1 + 300));
  }
  pvVar3 = (void *)FUN_016830c0(0x1ac,uVar2);
  local_8 = 1;
  if (pvVar3 == (void *)0x0) {
    piVar4 = (int *)0x0;
  }
  else {
    memset(pvVar3,0,0x1ac);
    piVar4 = (int *)FUN_012435e0();
  }
  local_8 = 0xffffffff;
  *(int **)(param_1 + 0x130) = piVar4;
  if (piVar4 != (int *)0x0) {
    (**(code **)(*piVar4 + 0xc))(*(undefined4 *)(param_1 + 0x40),0x154,0x6a,0x6e,2,0);
    iVar1 = *(int *)(param_1 + 0x130);
    *(undefined4 *)(iVar1 + 0xc0) = 0;
    *(undefined4 *)(iVar1 + 0xdc) = 0;
    puVar5 = (undefined4 *)(iVar1 + 0xb0);
    if (7 < *(uint *)(iVar1 + 0xc4)) {
      puVar5 = (undefined4 *)*puVar5;
    }
    *(undefined2 *)puVar5 = 0;
    FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
    FUN_01242970(s_EquipSystem_Upgrade_Empty_Line_t_01ae8b88);
    *(undefined4 *)(*(int *)(param_1 + 0x130) + 0xd4) = 0x3c23d70a;
    piVar4 = *(int **)(param_1 + 0x130);
    piVar4[0x3b] = 1;
    piVar4[0x3c] = 0;
    fVar6 = (float10)fmaxf(0,0x3f800000);
    piVar4[0x32] = (int)(float)fVar6;
    FUN_01243010();
    (**(code **)(*piVar4 + 0x48))();
    FUN_01242bf0();
    (**(code **)(**(int **)(param_1 + 0x130) + 0x1c))(1);
    FUN_01271c10(1,param_1,FUN_01271aa0,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x130));
    FUN_0126de20(2,*(undefined4 *)(param_1 + 0x130));
  }
  FUN_01246720(s_EquipSystem_Upgrade_Line_horizon_01ae8bec,1,0xa7,0x6a,0x76,2);
  FUN_01246720(s_EquipSystem_Upgrade_Line_horizon_01ae8bec,1,0x155,0x6a,0x76,2);
  ExceptionList = local_10;
  return;
}


