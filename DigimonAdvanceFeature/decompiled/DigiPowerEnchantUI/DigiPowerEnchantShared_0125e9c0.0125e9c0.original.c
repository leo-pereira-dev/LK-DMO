
/* ============================================================ */
/* Function: FUN_0125e9c0 */
/* Entry: 0125e9c0 */
/* Signature: undefined FUN_0125e9c0() */
/* ============================================================ */


undefined4 __thiscall FUN_0125e9c0(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  
  if (*(int *)(param_1 + 0xac) != 1) {
    if (param_2 == 1) {
      cVar1 = FUN_0137ac10(param_3,param_4);
      if (cVar1 != '\0') {
        FUN_01378790(&param_3);
        FUN_00ff2740();
        return 1;
      }
    }
  }
  return 0;
}


