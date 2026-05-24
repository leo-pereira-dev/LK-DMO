
/* ============================================================ */
/* Function: FUN_011befd0 */
/* Entry: 011befd0 */
/* Signature: undefined FUN_011befd0() */
/* ============================================================ */


void __fastcall FUN_011befd0(int param_1)

{
  int *piVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  int *piVar6;
  void *pvVar7;
  int *piVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined1 local_34 [24];
  int *local_1c;
  int local_18;
  int *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019918d7;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  cVar2 = *(char *)(param_1 + 0x74);
  *(undefined2 *)(param_1 + 0x74) = 0x101;
  if (cVar2 == '\0') {
    local_18 = param_1;
    FUN_012457d0(s_Random_box_bg_png_01adb0b4,0,4,4,1,DAT_01bee88c,DAT_01bee890,DAT_01bc07ec,
                 DAT_01bc07f0);
    uVar5 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
    FUN_00f19cc0(s_DIGICOMBINE_TITLE_01ae207c,0x11);
    uVar5 = FUN_015ce480(local_34,uVar5,uVar9,uVar10,uVar11,uVar12,uVar13);
    local_8 = 0;
    FUN_011bfa80(uVar5);
    local_8 = 0xffffffff;
    FUN_00f1a330(uVar4);
    piVar6 = (int *)FUN_016830c0(0x140);
    local_8 = 1;
    local_14 = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x140);
      FUN_01245550();
      piVar6[0x1d] = (int)&DAT_01ae2074;
      piVar6[0x1e] = 0;
      piVar6[0x1d] = (int)&DAT_01ae2068;
      local_8._0_1_ = 2;
      *piVar6 = (int)&DAT_01ae2184;
      piVar6[0x1d] = (int)&DAT_01ae21c8;
      piVar8 = piVar6 + 0x1f;
      *piVar8 = 0;
      piVar6[0x20] = 0;
      local_1c = piVar8;
      pvVar7 = operator_new(0x24);
      piVar1 = local_14;
      *(void **)pvVar7 = pvVar7;
      *(void **)((int)pvVar7 + 4) = pvVar7;
      *(void **)((int)pvVar7 + 8) = pvVar7;
      *(undefined2 *)((int)pvVar7 + 0xc) = 0x101;
      *piVar8 = (int)pvVar7;
      local_8._0_1_ = 3;
      piVar6 = local_14 + 0x22;
      local_14[0x21] = 0;
      *piVar6 = 0;
      local_14[0x23] = 0;
      local_1c = piVar6;
      pvVar7 = operator_new(0x38);
      piVar8 = local_14;
      *(void **)pvVar7 = pvVar7;
      *(void **)((int)pvVar7 + 4) = pvVar7;
      *(void **)((int)pvVar7 + 8) = pvVar7;
      *(undefined2 *)((int)pvVar7 + 0xc) = 0x101;
      *piVar6 = (int)pvVar7;
      piVar1[0x24] = 0;
      local_8._0_1_ = 4;
      piVar6 = local_14 + 0x25;
      *piVar6 = 0;
      local_14[0x26] = 0;
      local_1c = piVar6;
      pvVar7 = operator_new(0x34);
      *(void **)pvVar7 = pvVar7;
      *(void **)((int)pvVar7 + 4) = pvVar7;
      *(void **)((int)pvVar7 + 8) = pvVar7;
      *(undefined2 *)((int)pvVar7 + 0xc) = 0x101;
      *piVar6 = (int)pvVar7;
      piVar8[0x27] = 0;
      piVar8[0x28] = 0;
      piVar8[0x29] = 0;
      piVar8[0x2a] = 0;
      piVar8[0x2b] = 0;
      piVar8[0x2c] = 0;
      piVar8[0x2d] = 0;
      piVar8[0x2e] = 0;
      piVar8[0x2f] = 0;
      piVar8[0x30] = 0;
      piVar8[0x31] = 0;
      *(undefined1 *)(piVar8 + 0x32) = 1;
      local_8 = CONCAT31(local_8._1_3_,5);
      FUN_01245550();
      piVar6 = local_14;
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xb0) = piVar6;
    if (piVar6 != (int *)0x0) {
      FUN_011c1420(0);
    }
    piVar6 = (int *)FUN_016830c0(0x80);
    local_8 = 6;
    local_1c = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x80);
      FUN_01245550();
      piVar6[0x1d] = (int)&DAT_01ae2074;
      piVar6[0x1e] = 0;
      piVar6[0x1d] = (int)&DAT_01ae2068;
      *piVar6 = (int)&DAT_01ae2134;
      piVar6[0x1d] = (int)&DAT_01ae2178;
      piVar6[0x1f] = 0;
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xb4) = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x24))(0);
    }
    piVar6 = (int *)FUN_016830c0(0x114);
    local_8 = 7;
    local_1c = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x114);
      FUN_01245550();
      piVar6[0x1d] = (int)&DAT_01ae2074;
      piVar6[0x1e] = 0;
      piVar6[0x1d] = (int)&DAT_01ae2068;
      local_8._0_1_ = 8;
      piVar1 = piVar6 + 0x1f;
      *piVar6 = (int)&DAT_01ae20e4;
      piVar6[0x1d] = (int)&DAT_01ae2128;
      *piVar1 = 0;
      piVar6[0x20] = 0;
      local_14 = piVar1;
      pvVar7 = operator_new(0x34);
      *(void **)pvVar7 = pvVar7;
      *(void **)((int)pvVar7 + 4) = pvVar7;
      *(void **)((int)pvVar7 + 8) = pvVar7;
      *(undefined2 *)((int)pvVar7 + 0xc) = 0x101;
      *piVar1 = (int)pvVar7;
      local_8 = CONCAT31(local_8._1_3_,9);
      piVar6[0x21] = 0;
      piVar6[0x22] = 0;
      piVar6[0x23] = 0;
      piVar6[0x24] = 0;
      piVar6[0x25] = 0;
      piVar6[0x26] = 0;
      *(undefined1 *)(piVar6 + 0x27) = 0;
      FUN_01245550();
    }
    local_8 = 0xffffffff;
    *(int **)(local_18 + 0xb8) = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x24))(0);
    }
    piVar6 = (int *)FUN_016830c0(0x88);
    local_8 = 10;
    local_1c = piVar6;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x88);
      FUN_01245550();
      piVar6[0x1d] = (int)&DAT_01ae2074;
      piVar6[0x1e] = 0;
      piVar6[0x1d] = (int)&DAT_01ae2068;
      *piVar6 = (int)&DAT_01ae2094;
      piVar6[0x1d] = (int)&DAT_01ae20d8;
      piVar6[0x1f] = 0;
      piVar6[0x20] = 0;
      piVar6[0x21] = 0;
    }
    iVar3 = local_18;
    local_8 = 0xffffffff;
    *(int **)(local_18 + 0xbc) = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x24))(0);
      *(undefined1 *)(*(int *)(iVar3 + 0xbc) + 0x68) = 0;
    }
    *(undefined4 *)(iVar3 + 0xac) = *(undefined4 *)(iVar3 + 0xb0);
  }
  ExceptionList = local_10;
  return;
}


