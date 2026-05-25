
/* ============================================================ */
/* Function: FUN_011d0110 */
/* Entry: 011d0110 */
/* Signature: undefined FUN_011d0110() */
/* ============================================================ */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __thiscall FUN_011d0110(int param_1,int param_2)

{
  char cVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  uint uVar8;
  void *pvVar9;
  int iVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  undefined4 uVar13;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 *puVar14;
  int *piVar15;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01992c99;
  local_10 = ExceptionList;
  uVar8 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if (*(int *)(param_1 + 0xb4) != *(int *)(param_1 + 0xb8)) {
    *(int *)(param_1 + 0xb8) = *(int *)(param_1 + 0xb4);
  }
  pvVar9 = (void *)FUN_016830c0(0x94,uVar8);
  local_8 = 0;
  if (pvVar9 == (void *)0x0) {
    iVar10 = 0;
    uVar13 = extraout_ECX;
  }
  else {
    memset(pvVar9,0,0x94);
    iVar10 = FUN_0122b460();
    uVar13 = extraout_ECX_00;
  }
  local_8 = 0xffffffff;
  *(int *)(param_1 + 0xb0) = iVar10;
  if (iVar10 == 0) {
    ExceptionList = local_10;
    return 0;
  }
  FUN_0122b6f0(*(undefined4 *)(param_1 + 0x40),7,0x21,0x11e,0x1a7,s_Mail_Mail_List_Box_tga_01ad56c4,
               uVar13);
  *(undefined4 *)(*(int *)(param_1 + 0xb0) + 0x78) = 0x2d;
  iVar10 = *(int *)(param_1 + 0xb0);
  *(undefined4 *)(iVar10 + 0x7c) = 3;
  *(undefined4 *)(iVar10 + 0x80) = 7;
  iVar10 = *(int *)(param_1 + 0xb0);
  *(undefined4 *)(iVar10 + 0x84) = 0;
  *(undefined4 *)(iVar10 + 0x88) = 7;
  FUN_012472c0(*(undefined4 *)(param_1 + 0xb0));
  puVar2 = *(undefined4 **)(*(int *)(param_1 + 0xa8) + 0x30);
  cVar1 = *(char *)((int)puVar2[1] + 0xd);
  puVar12 = puVar2;
  puVar11 = (undefined4 *)puVar2[1];
  while (cVar1 == '\0') {
    if ((int)puVar11[4] < param_2) {
      puVar14 = (undefined4 *)puVar11[2];
      puVar11 = puVar12;
    }
    else {
      puVar14 = (undefined4 *)*puVar11;
    }
    puVar12 = puVar11;
    puVar11 = puVar14;
    cVar1 = *(char *)((int)puVar14 + 0xd);
  }
  if ((*(char *)((int)puVar12 + 0xd) != '\0') || (param_2 < (int)puVar12[4])) {
    puVar12 = puVar2;
  }
  piVar15 = puVar12 + 5;
  if (puVar12 == puVar2) {
    piVar15 = (int *)0x0;
  }
  if (piVar15 != (int *)0x0) {
    puVar2 = (undefined4 *)*piVar15;
    for (puVar12 = (undefined4 *)*puVar2; puVar12 != puVar2; puVar12 = (undefined4 *)*puVar12) {
      FUN_011d0390(param_2,puVar12[2]);
    }
  }
  local_14 = 0;
  if (*(int *)(param_1 + 0xc0) != 0) {
    local_14 = *(undefined4 *)(*(int *)(param_1 + 0xc0) + 0x88);
  }
  pvVar9 = (void *)FUN_016830c0(0xec);
  local_8 = 1;
  if (pvVar9 == (void *)0x0) {
    uVar13 = 0;
  }
  else {
    memset(pvVar9,0,0xec);
    uVar13 = FUN_01247d90();
  }
  uVar7 = uRam01b031ec;
  uVar6 = uRam01b031e8;
  uVar5 = uRam01b031e4;
  uVar4 = _DAT_01b031e0;
  local_8 = 0xffffffff;
  *(undefined4 *)(param_1 + 0xc0) = uVar13;
  FUN_01248090(0,*(undefined4 *)(param_1 + 0x40),5,0,0x113,0x28,0x10,0x194,0x10,0xf,uVar4,uVar5,
               uVar6,uVar7);
  iVar10 = *(int *)(param_1 + 0xc0);
  iVar3 = *(int *)(*(int *)(param_1 + 0xb0) + 0x68);
  if ((*(int *)(iVar10 + 0x78) != 0) || (*(int *)(iVar10 + 0x7c) != iVar3)) {
    *(undefined4 *)(iVar10 + 0x78) = 0;
    *(int *)(iVar10 + 0x7c) = iVar3;
    FUN_0124a620();
  }
  FUN_012473e0(*(undefined4 *)(param_1 + 0xc0));
  FUN_0122c520(*(undefined4 *)(param_1 + 0xc0));
  FUN_0124a8a0(local_14);
  iVar10 = *(int *)(param_1 + 0xb0);
  *(undefined1 *)(iVar10 + 0x90) = 1;
  ExceptionList = local_10;
  return CONCAT31((int3)((uint)iVar10 >> 8),1);
}


