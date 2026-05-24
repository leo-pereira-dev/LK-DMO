
/* ============================================================ */
/* Function: FUN_012b7900 */
/* Entry: 012b7900 */
/* Signature: undefined FUN_012b7900() */
/* ============================================================ */


undefined4 * __thiscall
FUN_012b7900(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4)

{
  int iVar1;
  
  if (*(int *)(param_1 + 0x148) == 0) {
    *param_2 = 0;
    param_2[1] = 0;
    param_2[2] = 0;
    param_2[3] = 0;
    param_2[4] = 0;
    param_2[5] = 0;
    param_2[6] = 0;
    return param_2;
  }
  iVar1 = FUN_015ec540(*param_3,*param_4);
  *param_2 = 0;
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[4] = 0;
  param_2[5] = 0;
  param_2[6] = 0;
  FUN_00f65090(iVar1,iVar1 >> 0x1f);
  return param_2;
}


