
/* ============================================================ */
/* Function: FUN_011da4b0 */
/* Entry: 011da4b0 */
/* Signature: undefined FUN_011da4b0() */
/* ============================================================ */


undefined4 * FUN_011da4b0(undefined4 *param_1,undefined4 param_2)

{
  char *pcVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  switch(param_2) {
  case 0:
    pcVar1 = s_SEALMASTER_STATE_HP_01ad7500;
    break;
  case 1:
    pcVar1 = s_SEALMASTER_STATE_DS_01ae32e4;
    break;
  case 2:
    pcVar1 = s_SEALMASTER_STATE_AT_01ad7490;
    break;
  case 3:
    pcVar1 = s_SEALMASTER_STATE_AS_01ae3328;
    break;
  case 4:
    pcVar1 = s_SEALMASTER_STATE_CT_01ad74d8;
    break;
  case 5:
    pcVar1 = s_SEALMASTER_STATE_HT_01ae3314;
    break;
  case 6:
    pcVar1 = s_SEALMASTER_STATE_DE_01ae333c;
    break;
  case 7:
    pcVar1 = s_SEALMASTER_STATE_BL_01ad74ec;
    break;
  case 8:
    pcVar1 = s_SEALMASTER_STATE_EV_01ad7514;
    break;
  default:
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    FUN_00f1a200(&DAT_01ac5dd8,0);
    return param_1;
  }
  uVar2 = 0;
  uVar3 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar7 = 0;
  uVar6 = 0;
  FUN_00f19cc0(pcVar1,0x13);
  FUN_015ce480(param_1,uVar2,uVar3,uVar4,uVar5,uVar6,uVar7);
  return param_1;
}


