
/* ============================================================ */
/* Function: FUN_012b5f30 */
/* Entry: 012b5f30 */
/* Signature: undefined FUN_012b5f30() */
/* ============================================================ */


void FUN_012b5f30(undefined4 *param_1)

{
  undefined4 local_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined8 local_c;
  undefined4 local_4;
  
  if ((DAT_01bfb2b4 == 0) || ((undefined4 *)(DAT_01bfb2b4 + 0x28) == (undefined4 *)0x0)) {
    local_1c = 0;
    uStack_18 = 0;
    uStack_14 = 0;
    uStack_10 = 0;
    local_c = 0;
    local_4 = 0;
  }
  else {
    local_1c = *(undefined4 *)(DAT_01bfb2b4 + 0x28);
    uStack_18 = *(undefined4 *)(DAT_01bfb2b4 + 0x2c);
    uStack_14 = *(undefined4 *)(DAT_01bfb2b4 + 0x30);
    uStack_10 = *(undefined4 *)(DAT_01bfb2b4 + 0x34);
    local_c = *(undefined8 *)(DAT_01bfb2b4 + 0x38);
    local_4 = *(undefined4 *)(DAT_01bfb2b4 + 0x40);
  }
  *param_1 = local_1c;
  param_1[1] = uStack_18;
  param_1[2] = uStack_14;
  param_1[3] = uStack_10;
  *(undefined8 *)(param_1 + 4) = local_c;
  param_1[6] = local_4;
  return;
}


