
/* ============================================================ */
/* Function: FUN_011b7390 */
/* Entry: 011b7390 */
/* Signature: undefined FUN_011b7390() */
/* ============================================================ */


void __fastcall FUN_011b7390(int param_1)

{
  void *pvVar1;
  int *piVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 extraout_ECX;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01991199;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  pvVar1 = (void *)FUN_016830c0(0x94,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  local_8 = 0;
  if (pvVar1 == (void *)0x0) {
    piVar2 = (int *)0x0;
  }
  else {
    memset(pvVar1,0,0x94);
    piVar2 = (int *)FUN_0122b460();
  }
  local_8 = 0xffffffff;
  *(int **)(param_1 + 0x7c) = piVar2;
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 0xc))(*(undefined4 *)(param_1 + 0x40),0x14,0xaa,0x2f8,0x19f,0);
    FUN_0122b730(s_Random_box_Rate_bg_5x5_png_01adab04,&DAT_01bee88c,extraout_ECX);
    iVar4 = *(int *)(param_1 + 0x7c);
    *(undefined4 *)(iVar4 + 0x7c) = 10;
    *(undefined4 *)(iVar4 + 0x80) = 10;
    puVar3 = operator_new(8);
    *puVar3 = &DAT_01ae1a40;
    puVar3[1] = 0;
    FUN_0145efe0(puVar3);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x7c));
    pvVar1 = (void *)FUN_016830c0(0xec);
    local_8 = 1;
    if (pvVar1 == (void *)0x0) {
      iVar4 = 0;
    }
    else {
      memset(pvVar1,0,0xec);
      iVar4 = FUN_01247d90();
    }
    local_8 = 0xffffffff;
    if (iVar4 != 0) {
      FUN_01248090(2,0,7,0,DAT_01bee88c,DAT_01bee890,0x17,0x19f,0x17,0x17,DAT_01bee88c,DAT_01bee890,
                   0x2d7,0x19f);
      *(undefined1 *)(iVar4 + 0xdc) = 1;
      FUN_0122c3a0(iVar4,1);
    }
  }
  ExceptionList = local_10;
  return;
}


