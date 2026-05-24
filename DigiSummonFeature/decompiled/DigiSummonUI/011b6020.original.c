
/* ============================================================ */
/* Function: FUN_011b6020 */
/* Entry: 011b6020 */
/* Signature: undefined FUN_011b6020() */
/* ============================================================ */


void __fastcall FUN_011b6020(int param_1)

{
  char cVar1;
  undefined8 uVar2;
  uint uVar3;
  undefined4 *puVar4;
  void *pvVar5;
  int *piVar6;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01991109;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  cVar1 = *(char *)(param_1 + 0x74);
  *(undefined2 *)(param_1 + 0x74) = 0x101;
  if (cVar1 == '\0') {
    FUN_012457d0(s_Random_box_bg_png_01adb0b4,0,4,4,1,DAT_01bee88c,DAT_01bee890,DAT_01bc07ec,
                 DAT_01bc07f0);
    puVar4 = (undefined4 *)FUN_016830c0(0x94,uVar3);
    local_8 = 0;
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = (undefined4 *)0x0;
    }
    else {
      memset(puVar4,0,0x94);
      FUN_01245550();
      puVar4[0x1d] = &DAT_01ae17ac;
      puVar4[0x1e] = 0;
      local_8 = CONCAT31(local_8._1_3_,2);
      *puVar4 = &DAT_01ae1820;
      puVar4[0x1d] = &DAT_01ae1814;
      puVar4[0x1f] = 0;
      puVar4[0x20] = 0;
      pvVar5 = operator_new(0x24);
      *(void **)pvVar5 = pvVar5;
      *(void **)((int)pvVar5 + 4) = pvVar5;
      *(void **)((int)pvVar5 + 8) = pvVar5;
      *(undefined2 *)((int)pvVar5 + 0xc) = 0x101;
      puVar4[0x1f] = pvVar5;
      puVar4[0x21] = 0;
      puVar4[0x22] = 0;
      puVar4[0x23] = 0;
      puVar4[0x24] = 0;
    }
    local_8 = 0xffffffff;
    *(undefined4 **)(param_1 + 0xac) = puVar4;
    if (puVar4 != (undefined4 *)0x0) {
      FUN_011b4710(*(undefined4 *)(param_1 + 0x40));
    }
    puVar4 = (undefined4 *)FUN_016830c0(0x1e8,uVar3);
    local_8 = 3;
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = (undefined4 *)0x0;
    }
    else {
      memset(puVar4,0,0x1e8);
      FUN_01245550();
      puVar4[0x1d] = &DAT_01ae17ac;
      puVar4[0x1e] = 0;
      puVar4[0x1d] = &DAT_01ae175c;
      local_8._0_1_ = 4;
      *puVar4 = &DAT_01ae19b8;
      puVar4[0x1d] = &DAT_01ae1944;
      puVar4[0x1f] = 0;
      puVar4[0x20] = 0;
      pvVar5 = operator_new(0x3c);
      *(void **)pvVar5 = pvVar5;
      *(void **)((int)pvVar5 + 4) = pvVar5;
      *(void **)((int)pvVar5 + 8) = pvVar5;
      *(undefined2 *)((int)pvVar5 + 0xc) = 0x101;
      puVar4[0x1f] = pvVar5;
      local_8._0_1_ = 5;
      puVar4[0x21] = 0;
      puVar4[0x22] = 0;
      puVar4[0x23] = 0;
      puVar4[0x24] = 0;
      puVar4[0x25] = 0;
      puVar4[0x26] = 0;
      FUN_01245550();
      local_8._0_1_ = 6;
      FUN_01245550();
      local_8._0_1_ = 7;
      puVar4[0x44] = &DAT_01ae1994;
      puVar4[0x61] = 0;
      puVar4[0x62] = 0;
      puVar4[99] = 0;
      puVar4[0x65] = 0;
      puVar4[0x66] = 0;
      puVar4[0x67] = 0;
      pvVar5 = operator_new(0xc);
      *(void **)pvVar5 = pvVar5;
      *(void **)((int)pvVar5 + 4) = pvVar5;
      puVar4[0x66] = pvVar5;
      *(undefined1 *)(puVar4 + 0x68) = 1;
      puVar4[0x69] = 0;
      puVar4[0x6a] = 0;
      puVar4[0x6b] = 0;
      local_8 = CONCAT31(local_8._1_3_,8);
      puVar4[0x6d] = 0;
      puVar4[0x6e] = 0;
      puVar4[0x6f] = 0;
      pvVar5 = operator_new(0xc);
      uVar2 = DAT_01b02b10;
      *(void **)pvVar5 = pvVar5;
      *(void **)((int)pvVar5 + 4) = pvVar5;
      puVar4[0x6e] = pvVar5;
      *(undefined1 *)(puVar4 + 0x70) = 1;
      puVar4[0x71] = 0;
      puVar4[0x72] = 0;
      puVar4[0x73] = 0;
      *(undefined8 *)(puVar4 + 0x74) = uVar2;
      *(undefined8 *)(puVar4 + 0x76) = 0;
      *(undefined1 *)(puVar4 + 0x79) = 0;
    }
    local_8 = 0xffffffff;
    *(undefined4 **)(param_1 + 0xb4) = puVar4;
    if (puVar4 != (undefined4 *)0x0) {
      FUN_011bb4b0(0);
    }
    piVar6 = (int *)FUN_016830c0(0x80);
    local_8 = 9;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x80);
      FUN_01245550();
      piVar6[0x1d] = (int)&DAT_01ae17ac;
      piVar6[0x1e] = 0;
      piVar6[0x1d] = (int)&DAT_01ae175c;
      *piVar6 = (int)&DAT_01ae1950;
      piVar6[0x1d] = (int)&DAT_01ae18f4;
      piVar6[0x1f] = 0;
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xb8) = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x24))(0);
    }
    piVar6 = (int *)FUN_016830c0(0x11c);
    local_8 = 10;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0x11c);
      FUN_01245550();
      piVar6[0x1d] = (int)&DAT_01ae17ac;
      piVar6[0x1e] = 0;
      piVar6[0x1d] = (int)&DAT_01ae175c;
      local_8._0_1_ = 0xb;
      *piVar6 = (int)&DAT_01ae1900;
      piVar6[0x1d] = (int)&DAT_01ae18a4;
      piVar6[0x1f] = 0;
      piVar6[0x20] = 0;
      pvVar5 = operator_new(0x30);
      *(void **)pvVar5 = pvVar5;
      *(void **)((int)pvVar5 + 4) = pvVar5;
      *(void **)((int)pvVar5 + 8) = pvVar5;
      *(undefined2 *)((int)pvVar5 + 0xc) = 0x101;
      piVar6[0x1f] = (int)pvVar5;
      local_8 = CONCAT31(local_8._1_3_,0xc);
      piVar6[0x21] = 0;
      piVar6[0x22] = 0;
      piVar6[0x23] = 0;
      piVar6[0x24] = 0;
      piVar6[0x25] = 0;
      piVar6[0x26] = 0;
      piVar6[0x27] = 0;
      piVar6[0x28] = 0;
      FUN_01245550();
      *(undefined1 *)(piVar6 + 0x46) = 0;
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xbc) = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x24))(0);
    }
    piVar6 = (int *)FUN_016830c0(0xa8);
    local_8 = 0xd;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xa8);
      FUN_01245550();
      piVar6[0x1d] = (int)&DAT_01ae17ac;
      piVar6[0x1e] = 0;
      piVar6[0x1d] = (int)&DAT_01ae175c;
      local_8 = CONCAT31(local_8._1_3_,0xe);
      *piVar6 = (int)&DAT_01ae18b0;
      piVar6[0x1d] = (int)&DAT_01ae1854;
      piVar6[0x1f] = 0;
      piVar6[0x20] = 0;
      piVar6[0x21] = 0;
      piVar6[0x23] = 0;
      piVar6[0x24] = 0;
      piVar6[0x25] = 0;
      pvVar5 = operator_new(0xc);
      *(void **)pvVar5 = pvVar5;
      *(void **)((int)pvVar5 + 4) = pvVar5;
      piVar6[0x24] = (int)pvVar5;
      *(undefined1 *)(piVar6 + 0x26) = 1;
      piVar6[0x27] = 0;
      piVar6[0x28] = 0;
      piVar6[0x29] = 0;
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xc0) = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x24))(0);
      *(undefined1 *)(*(int *)(param_1 + 0xc0) + 0x68) = 0;
    }
    piVar6 = (int *)FUN_016830c0(0xac);
    local_8 = 0xf;
    if (piVar6 == (int *)0x0) {
      piVar6 = (int *)0x0;
    }
    else {
      memset(piVar6,0,0xac);
      FUN_01245550();
      piVar6[0x1d] = (int)&DAT_01ae17ac;
      piVar6[0x1e] = 0;
      piVar6[0x1d] = (int)&DAT_01ae175c;
      local_8._0_1_ = 0x10;
      *piVar6 = (int)&DAT_01ae1860;
      piVar6[0x1d] = (int)&DAT_01ae1848;
      piVar6[0x20] = 0;
      piVar6[0x21] = 0;
      piVar6[0x22] = 0;
      pvVar5 = operator_new(0xc);
      *(void **)pvVar5 = pvVar5;
      *(void **)((int)pvVar5 + 4) = pvVar5;
      piVar6[0x21] = (int)pvVar5;
      *(undefined1 *)(piVar6 + 0x23) = 1;
      piVar6[0x24] = 0;
      piVar6[0x25] = 0;
      piVar6[0x26] = 0;
      local_8 = CONCAT31(local_8._1_3_,0x11);
      piVar6[0x27] = 0;
      piVar6[0x28] = 0;
      piVar6[0x29] = 0;
      piVar6[0x2a] = 0;
      pvVar5 = operator_new(0x24);
      *(void **)pvVar5 = pvVar5;
      *(void **)((int)pvVar5 + 4) = pvVar5;
      *(void **)((int)pvVar5 + 8) = pvVar5;
      *(undefined2 *)((int)pvVar5 + 0xc) = 0x101;
      piVar6[0x29] = (int)pvVar5;
    }
    local_8 = 0xffffffff;
    *(int **)(param_1 + 0xc4) = piVar6;
    if (piVar6 != (int *)0x0) {
      (**(code **)(*piVar6 + 0x24))(0);
      *(undefined1 *)(*(int *)(param_1 + 0xc4) + 0x68) = 0;
    }
    *(undefined4 *)(param_1 + 0xb0) = *(undefined4 *)(param_1 + 0xb4);
  }
  ExceptionList = local_10;
  return;
}


