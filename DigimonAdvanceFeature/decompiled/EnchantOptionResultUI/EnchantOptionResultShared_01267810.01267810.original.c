
/* ============================================================ */
/* Function: FUN_01267810 */
/* Entry: 01267810 */
/* Signature: undefined FUN_01267810() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_01267810(int *param_1,uint *param_2)

{
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 local_c;
  undefined1 local_8;
  uint local_4;
  
  local_4 = DAT_01bbb1c0 ^ (uint)&local_4c;
  if (*param_1 == 6) {
    memset(&local_4c,0,0x45);
    if ((param_2[2] != param_2[3]) && (*param_2 < param_2[1])) {
      FUN_012684b0(param_2);
    }
    FUN_01267ef0(local_4c,uStack_48,uStack_44,uStack_40,local_3c,uStack_38,uStack_34,uStack_30,
                 local_2c,uStack_28,uStack_24,uStack_20,local_1c,uStack_18,uStack_14,uStack_10,
                 local_c,local_8);
  }
  return;
}


