
/* ============================================================ */
/* Function: FUN_01267a60 */
/* Entry: 01267a60 */
/* Signature: undefined FUN_01267a60() */
/* ============================================================ */


void __fastcall FUN_01267a60(int *param_1)

{
  int iVar1;
  
  if ((char)param_1[0x1a] != '\0') {
    (**(code **)(*param_1 + 0x10))();
    if ((int *)param_1[0x2b] != (int *)0x0) {
      (**(code **)(*(int *)param_1[0x2b] + 0x10))();
    }
    if ((int *)param_1[0x2c] != (int *)0x0) {
      (**(code **)(*(int *)param_1[0x2c] + 0x10))();
    }
    iVar1 = param_1[0x2d];
    if ((iVar1 != 0) && (*(int *)(iVar1 + 0xf0) != 0)) {
      FUN_01250630(*(undefined4 *)(iVar1 + 0xdc),DAT_01bee88c,DAT_01bee890);
    }
    if ((int *)param_1[0x3e] != (int *)0x0) {
      (**(code **)(*(int *)param_1[0x3e] + 0x10))();
    }
    if (0 < param_1[0x31]) {
      FUN_012334d0(0,0,*(undefined4 *)(param_1[0x2b] + 0x10),*(undefined4 *)(param_1[0x2b] + 0x14));
    }
    if (0 < param_1[0x39]) {
      FUN_012334d0(0,0,*(undefined4 *)(param_1[0x2b] + 0x10),*(undefined4 *)(param_1[0x2b] + 0x14));
    }
  }
  return;
}


