
/* ============================================================ */
/* Function: FUN_01275e30 */
/* Entry: 01275e30 */
/* Signature: undefined FUN_01275e30() */
/* ============================================================ */


void __fastcall FUN_01275e30(int param_1)

{
  uint uVar1;
  void *pvVar2;
  int iVar3;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  undefined4 extraout_ECX_03;
  undefined4 extraout_ECX_04;
  undefined4 extraout_ECX_05;
  undefined4 extraout_ECX_06;
  undefined4 uVar4;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0199c28d;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  pvVar2 = (void *)FUN_016830c0(0x8c,uVar1);
  local_8 = 0;
  if (pvVar2 == (void *)0x0) {
    iVar3 = 0;
    uVar4 = extraout_ECX;
  }
  else {
    memset(pvVar2,0,0x8c);
    iVar3 = FUN_012404a0();
    uVar4 = extraout_ECX_00;
  }
  local_8 = 0xffffffff;
  *(int *)(param_1 + 0x178) = iVar3;
  if (iVar3 != 0) {
    FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,200,200,s_EquipSystem_Upgrade_Progress_Eff_01ae8b4c,0
                 ,0x14,uVar4,200,200,1,5);
    iVar3 = *(int *)(param_1 + 0x178);
    *(undefined4 *)(iVar3 + 100) = 0x3d23d70a;
    *(undefined4 *)(iVar3 + 0x68) = 0x3d23d70a;
    *(undefined1 *)(*(int *)(param_1 + 0x178) + 0x78) = 0;
    (**(code **)(**(int **)(param_1 + 0x178) + 0x1c))(0);
    FUN_012787a0(0,param_1,FUN_01276a00,0);
  }
  pvVar2 = (void *)FUN_016830c0(0x8c,uVar1);
  local_8 = 1;
  if (pvVar2 == (void *)0x0) {
    iVar3 = 0;
    uVar4 = extraout_ECX_01;
  }
  else {
    memset(pvVar2,0,0x8c);
    iVar3 = FUN_012404a0();
    uVar4 = extraout_ECX_02;
  }
  local_8 = 0xffffffff;
  *(int *)(param_1 + 0x17c) = iVar3;
  if (iVar3 != 0) {
    FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,200,200,s_EquipSystem_Upgrade_Success_Effe_01ae4608,0
                 ,0x14,uVar4,200,200,1,5);
    iVar3 = *(int *)(param_1 + 0x17c);
    *(undefined4 *)(iVar3 + 100) = 0x3d23d70a;
    *(undefined4 *)(iVar3 + 0x68) = 0x3d23d70a;
    *(undefined1 *)(*(int *)(param_1 + 0x17c) + 0x78) = 0;
    (**(code **)(**(int **)(param_1 + 0x17c) + 0x1c))(0);
  }
  pvVar2 = (void *)FUN_016830c0(0x8c);
  local_8 = 2;
  if (pvVar2 == (void *)0x0) {
    iVar3 = 0;
    uVar4 = extraout_ECX_03;
  }
  else {
    memset(pvVar2,0,0x8c);
    iVar3 = FUN_012404a0();
    uVar4 = extraout_ECX_04;
  }
  local_8 = 0xffffffff;
  *(int *)(param_1 + 0x180) = iVar3;
  if (iVar3 != 0) {
    FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,200,200,s_EquipSystem_Upgrade_Fail_Effect__01ae45e4,0
                 ,0x14,uVar4,200,200,1,5);
    iVar3 = *(int *)(param_1 + 0x180);
    *(undefined4 *)(iVar3 + 100) = 0x3d23d70a;
    *(undefined4 *)(iVar3 + 0x68) = 0x3d23d70a;
    *(undefined1 *)(*(int *)(param_1 + 0x180) + 0x78) = 0;
    (**(code **)(**(int **)(param_1 + 0x180) + 0x1c))(0);
  }
  pvVar2 = (void *)FUN_016830c0(0x8c);
  local_8 = 3;
  if (pvVar2 == (void *)0x0) {
    iVar3 = 0;
    uVar4 = extraout_ECX_05;
  }
  else {
    memset(pvVar2,0,0x8c);
    iVar3 = FUN_012404a0();
    uVar4 = extraout_ECX_06;
  }
  local_8 = 0xffffffff;
  *(int *)(param_1 + 0x184) = iVar3;
  if (iVar3 != 0) {
    FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,200,200,s_EquipSystem_Upgrade_Destroy_Effe_01ae8bac,0
                 ,0x14,uVar4,200,200,1,5);
    iVar3 = *(int *)(param_1 + 0x184);
    *(undefined4 *)(iVar3 + 100) = 0x3d23d70a;
    *(undefined4 *)(iVar3 + 0x68) = 0x3d23d70a;
    *(undefined1 *)(*(int *)(param_1 + 0x184) + 0x78) = 0;
    (**(code **)(**(int **)(param_1 + 0x184) + 0x1c))(0);
  }
  ExceptionList = local_10;
  return;
}


