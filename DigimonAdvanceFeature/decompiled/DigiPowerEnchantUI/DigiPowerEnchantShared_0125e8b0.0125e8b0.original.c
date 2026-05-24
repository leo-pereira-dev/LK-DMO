
/* ============================================================ */
/* Function: FUN_0125e8b0 */
/* Entry: 0125e8b0 */
/* Signature: undefined FUN_0125e8b0() */
/* ============================================================ */


void __fastcall FUN_0125e8b0(int *param_1)

{
  int iVar1;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  (**(code **)(*param_1 + 0x10))();
  iVar1 = param_1[0x36];
  if ((iVar1 != 0) && (param_1[0x37] != 0)) {
    FUN_00f1db60();
    uStack_18 = 0;
    uStack_14 = 0;
    uStack_10 = 0;
    uStack_c = 0;
    (**(code **)(**(int **)(DAT_01bfb28c + 0xbc) + 0x58))(&uStack_18);
    FUN_00f1dbf0(*(undefined4 *)(iVar1 + 4),7);
    FUN_0125c060(*(undefined4 *)(DAT_01bfc3b4 + 0x1c0),param_1[0x37]);
    FUN_0125c360(*(undefined4 *)(DAT_01bfc3b4 + 0x1c0),param_1[0xc] + -0xaa,param_1[0xd] + -0x140);
  }
  return;
}


