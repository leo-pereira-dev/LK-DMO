
/* ============================================================ */
/* Function: FUN_01057d70 */
/* Entry: 01057d70 */
/* Signature: undefined FUN_01057d70() */
/* ============================================================ */


undefined4 FUN_01057d70(int *param_1)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined1 local_50 [24];
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019762e5;
  local_1c = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if (*param_1 == 0) {
    uVar2 = 0;
  }
  else {
    local_28 = 0;
    local_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    uStack_2c = 0;
    local_24 = 0;
    FUN_00f19cc0(s_EXTRA_INVEN_DURING_FILTER_FAIL_01ad0e10,0x1e);
    local_14 = 0;
    puVar3 = (undefined4 *)FUN_015ce180(local_50,&local_38);
    local_14 = CONCAT31(local_14._1_3_,1);
    if (DAT_01bfbcf4 != 0) {
      if (7 < (uint)puVar3[5]) {
        puVar3 = (undefined4 *)*puVar3;
      }
      FUN_013bfad0(puVar3);
    }
    FUN_00f1a330(uVar1);
    FUN_00f1a420();
    uVar2 = 1;
  }
  ExceptionList = local_1c;
  return uVar2;
}


