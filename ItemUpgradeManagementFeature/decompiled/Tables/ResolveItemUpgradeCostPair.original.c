
/* ============================================================ */
/* Function: FUN_015ec470 */
/* Entry: 015ec470 */
/* Signature: undefined FUN_015ec470() */
/* ============================================================ */


undefined4 * __thiscall FUN_015ec470(int param_1,undefined4 *param_2,int param_3)

{
  undefined1 local_10 [8];
  int local_8;
  
  FUN_015ae4e0(local_10,&param_3);
  if (((*(char *)(local_8 + 0xd) == '\0') && (*(int *)(local_8 + 0x10) <= param_3)) &&
     (local_8 != *(int *)(param_1 + 0x7c))) {
    param_2[1] = *(undefined4 *)(local_8 + 0x18);
    param_2[2] = *(undefined4 *)(local_8 + 0x1c);
    *param_2 = &DAT_01a18ea8;
    return param_2;
  }
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[1] = 0;
  param_2[2] = 0;
  *param_2 = &DAT_01a18ea8;
  return param_2;
}


