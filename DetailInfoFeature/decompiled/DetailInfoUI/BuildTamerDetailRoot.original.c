
/* ============================================================ */
/* Function: FUN_011daec0 */
/* Entry: 011daec0 */
/* Signature: undefined FUN_011daec0() */
/* ============================================================ */


void FUN_011daec0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5)

{
  undefined8 local_30;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  uStack_14 = param_5;
  uStack_18 = param_4;
  uStack_1c = param_3;
  uStack_20 = param_2;
  uStack_24 = 1;
  local_28 = 0;
  local_30 = 1;
  FUN_012459a0(param_1,0);
  uStack_14 = param_1;
  uStack_18 = 0x11daef2;
  FUN_011daf90();
  uStack_14 = 0x263;
  uStack_18 = 0xfd;
  uStack_1c = 0xfffffff8;
  uStack_20 = 4;
  uStack_24 = 1;
  local_30 = DAT_01bb9b2c;
  local_28 = DAT_01bb9b34;
  FUN_01246800(s_DetailInfo_TamerStatus_Win_03_bg_01ae2b68,&local_30);
  uStack_14 = 0x16d;
  uStack_18 = 0xc5;
  uStack_1c = 0x84;
  uStack_20 = 0x102;
  uStack_24 = 1;
  local_30 = DAT_01bb9b2c;
  local_28 = DAT_01bb9b34;
  FUN_01246800(s_DetailInfo_TamerStatus_Win_04_bg_01ae33fc,&local_30);
  uStack_14 = 0x11daf6b;
  FUN_011db440();
  uStack_14 = 0x11daf72;
  FUN_011db840();
  uStack_14 = 0x11daf79;
  FUN_011dbca0();
  uStack_14 = 0x11daf80;
  FUN_011dc210();
  uStack_14 = 0x11daf87;
  FUN_011dc7f0();
  return;
}


