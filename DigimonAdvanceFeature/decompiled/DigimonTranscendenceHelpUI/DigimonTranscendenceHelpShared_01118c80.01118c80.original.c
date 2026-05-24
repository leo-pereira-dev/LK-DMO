
/* ============================================================ */
/* Function: FUN_01118c80 */
/* Entry: 01118c80 */
/* Signature: undefined FUN_01118c80() */
/* ============================================================ */


int __thiscall FUN_01118c80(int param_1,uint param_2)

{
  *(undefined **)(param_1 + 0x74) = &DAT_01ad7e4c;
  FUN_012456d0();
  if ((param_2 & 1) != 0) {
    if ((param_2 & 4) == 0) {
      FUN_01683120();
      return param_1;
    }
    FUN_00f199b0(param_1,0x7c);
  }
  return param_1;
}


