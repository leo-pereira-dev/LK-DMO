
/* ============================================================ */
/* Function: FUN_01276140 */
/* Entry: 01276140 */
/* Signature: undefined FUN_01276140() */
/* ============================================================ */


void __fastcall FUN_01276140(int param_1)

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
  puStack_c = &LAB_0199c30f;
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
  *(int **)(param_1 + 0x170) = piVar4;
  if (piVar4 != (int *)0x0) {
    (**(code **)(*piVar4 + 0xc))(*(undefined4 *)(param_1 + 0x40),0xae,0xaf,0x71,2,0);
    iVar1 = *(int *)(param_1 + 0x170);
    *(undefined4 *)(iVar1 + 0xc0) = 0;
    *(undefined4 *)(iVar1 + 0xdc) = 0;
    puVar5 = (undefined4 *)(iVar1 + 0xb0);
    if (7 < *(uint *)(iVar1 + 0xc4)) {
      puVar5 = (undefined4 *)*puVar5;
    }
    *(undefined2 *)puVar5 = 0;
    FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
    FUN_01242970(s_EquipSystem_Upgrade_Empty_Line_t_01ae8b88);
    *(undefined4 *)(*(int *)(param_1 + 0x170) + 0xd4) = 0x3c23d70a;
    piVar4 = *(int **)(param_1 + 0x170);
    piVar4[0x3b] = 0;
    piVar4[0x3c] = 0;
    fVar6 = (float10)fmaxf(0,0x3f000000);
    piVar4[0x32] = (int)(float)fVar6;
    FUN_01243010();
    (**(code **)(*piVar4 + 0x48))();
    FUN_01242bf0();
    (**(code **)(**(int **)(param_1 + 0x170) + 0x1c))(1);
    FUN_012787a0(1,param_1,FUN_012768e0,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x170));
    FUN_01273690(3,*(undefined4 *)(param_1 + 0x170));
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
  *(int **)(param_1 + 0x174) = piVar4;
  if (piVar4 != (int *)0x0) {
    (**(code **)(*piVar4 + 0xc))(*(undefined4 *)(param_1 + 0x40),0x152,0xaf,0x72,2,0);
    iVar1 = *(int *)(param_1 + 0x174);
    *(undefined4 *)(iVar1 + 0xc0) = 0;
    *(undefined4 *)(iVar1 + 0xdc) = 0;
    puVar5 = (undefined4 *)(iVar1 + 0xb0);
    if (7 < *(uint *)(iVar1 + 0xc4)) {
      puVar5 = (undefined4 *)*puVar5;
    }
    *(undefined2 *)puVar5 = 0;
    FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
    FUN_01242970(s_EquipSystem_Upgrade_Empty_Line_t_01ae8b88);
    *(undefined4 *)(*(int *)(param_1 + 0x174) + 0xd4) = 0x3c23d70a;
    piVar4 = *(int **)(param_1 + 0x174);
    piVar4[0x3b] = 0;
    piVar4[0x3c] = 0;
    fVar6 = (float10)fmaxf(0,0x3f000000);
    piVar4[0x32] = (int)(float)fVar6;
    FUN_01243010();
    (**(code **)(*piVar4 + 0x48))();
    FUN_01242bf0();
    *(undefined4 *)(*(int *)(param_1 + 0x174) + 0xd8) = 1;
    (**(code **)(**(int **)(param_1 + 0x174) + 0x1c))(1);
    FUN_012787a0(1,param_1,FUN_012768e0,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x174));
    FUN_01273690(3,*(undefined4 *)(param_1 + 0x174));
  }
  pvVar3 = (void *)FUN_016830c0(0x1ac);
  local_8 = 2;
  if (pvVar3 == (void *)0x0) {
    piVar4 = (int *)0x0;
  }
  else {
    memset(pvVar3,0,0x1ac);
    piVar4 = (int *)FUN_012435e0();
  }
  local_8 = 0xffffffff;
  *(int **)(param_1 + 0x164) = piVar4;
  if (piVar4 != (int *)0x0) {
    (**(code **)(*piVar4 + 0xc))(*(undefined4 *)(param_1 + 0x40),0xe5,0x101,0x3a,2,0);
    iVar1 = *(int *)(param_1 + 0x164);
    *(undefined4 *)(iVar1 + 0xc0) = 0;
    *(undefined4 *)(iVar1 + 0xdc) = 0;
    puVar5 = (undefined4 *)(iVar1 + 0xb0);
    if (7 < *(uint *)(iVar1 + 0xc4)) {
      puVar5 = (undefined4 *)*puVar5;
    }
    *(undefined2 *)puVar5 = 0;
    FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
    FUN_01242970(s_EquipSystem_Upgrade_Empty_Line_t_01ae8b88);
    *(undefined4 *)(*(int *)(param_1 + 0x164) + 0xd4) = 0x3c23d70a;
    piVar4 = *(int **)(param_1 + 0x164);
    piVar4[0x3b] = 0;
    piVar4[0x3c] = 0;
    fVar6 = (float10)fmaxf(0,0x3f000000);
    piVar4[0x32] = (int)(float)fVar6;
    FUN_01243010();
    (**(code **)(*piVar4 + 0x48))();
    FUN_01242bf0();
    (**(code **)(**(int **)(param_1 + 0x164) + 0x1c))(1);
    FUN_012787a0(1,param_1,FUN_012768e0,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x164));
    FUN_01273690(1,*(undefined4 *)(param_1 + 0x164));
  }
  pvVar3 = (void *)FUN_016830c0(0x1ac);
  local_8 = 3;
  if (pvVar3 == (void *)0x0) {
    piVar4 = (int *)0x0;
  }
  else {
    memset(pvVar3,0,0x1ac);
    piVar4 = (int *)FUN_012435e0();
  }
  local_8 = 0xffffffff;
  *(int **)(param_1 + 0x168) = piVar4;
  if (piVar4 != (int *)0x0) {
    (**(code **)(*piVar4 + 0xc))(*(undefined4 *)(param_1 + 0x40),0x152,0x101,0x3c,2,0);
    iVar1 = *(int *)(param_1 + 0x168);
    *(undefined4 *)(iVar1 + 0xc0) = 0;
    *(undefined4 *)(iVar1 + 0xdc) = 0;
    puVar5 = (undefined4 *)(iVar1 + 0xb0);
    if (7 < *(uint *)(iVar1 + 0xc4)) {
      puVar5 = (undefined4 *)*puVar5;
    }
    *(undefined2 *)puVar5 = 0;
    FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
    FUN_01242970(s_EquipSystem_Upgrade_Empty_Line_t_01ae8b88);
    *(undefined4 *)(*(int *)(param_1 + 0x168) + 0xd4) = 0x3c23d70a;
    piVar4 = *(int **)(param_1 + 0x168);
    piVar4[0x3b] = 0;
    piVar4[0x3c] = 0;
    fVar6 = (float10)fmaxf(0,0x3f000000);
    piVar4[0x32] = (int)(float)fVar6;
    FUN_01243010();
    (**(code **)(*piVar4 + 0x48))();
    FUN_01242bf0();
    *(undefined4 *)(*(int *)(param_1 + 0x168) + 0xd8) = 1;
    (**(code **)(**(int **)(param_1 + 0x168) + 0x1c))(1);
    FUN_012787a0(1,param_1,FUN_012768e0,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x168));
    FUN_01273690(1,*(undefined4 *)(param_1 + 0x168));
  }
  pvVar3 = (void *)FUN_016830c0(0x1ac);
  local_8 = 4;
  if (pvVar3 == (void *)0x0) {
    piVar4 = (int *)0x0;
  }
  else {
    memset(pvVar3,0,0x1ac);
    piVar4 = (int *)FUN_012435e0();
  }
  local_8 = 0xffffffff;
  *(int **)(param_1 + 0x16c) = piVar4;
  if (piVar4 != (int *)0x0) {
    (**(code **)(*piVar4 + 0xc))(*(undefined4 *)(param_1 + 0x40),0x138,0xca,2,0x1c,0);
    iVar1 = *(int *)(param_1 + 0x16c);
    *(undefined4 *)(iVar1 + 0xc0) = 0;
    *(undefined4 *)(iVar1 + 0xdc) = 0;
    puVar5 = (undefined4 *)(iVar1 + 0xb0);
    if (7 < *(uint *)(iVar1 + 0xc4)) {
      puVar5 = (undefined4 *)*puVar5;
    }
    *(undefined2 *)puVar5 = 0;
    FUN_012427e0(s_Item_Make_Make_ani_gauge_pixel_t_01ae8aac);
    FUN_01242970(s_EquipSystem_Upgrade_Empty_Line_t_01ae8b88);
    *(undefined4 *)(*(int *)(param_1 + 0x16c) + 0xd4) = 0x3c23d70a;
    piVar4 = *(int **)(param_1 + 0x16c);
    piVar4[0x3b] = 0;
    piVar4[0x3c] = 0;
    fVar6 = (float10)fmaxf(0,0x3f000000);
    piVar4[0x32] = (int)(float)fVar6;
    FUN_01243010();
    (**(code **)(*piVar4 + 0x48))();
    FUN_01242bf0();
    *(undefined4 *)(*(int *)(param_1 + 0x16c) + 0xd8) = 4;
    (**(code **)(**(int **)(param_1 + 0x16c) + 0x1c))(1);
    FUN_012787a0(1,param_1,FUN_012768e0,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x16c));
    FUN_01273690(3,*(undefined4 *)(param_1 + 0x16c));
  }
  FUN_01246720(s_EquipSystem_Upgrade_Line_horizon_01ae8bec,1,0xa7,0xaf,0x76,2);
  FUN_01246720(s_EquipSystem_Upgrade_Line_horizon_01ae8bec,1,0x155,0xaf,0x72,2);
  FUN_01246720(s_EquipSystem_Upgrade_Line_horizon_01ae8bec,1,0xe2,0x101,0x3f,2);
  FUN_01246720(s_EquipSystem_Upgrade_Line_horizon_01ae8bec,1,0x150,0x101,0x41,2);
  FUN_01246720(s_EquipSystem_Upgrade_Line_vertica_01ae8dc4,1,0x138,0xcc,2,0x1f);
  ExceptionList = local_10;
  return;
}


