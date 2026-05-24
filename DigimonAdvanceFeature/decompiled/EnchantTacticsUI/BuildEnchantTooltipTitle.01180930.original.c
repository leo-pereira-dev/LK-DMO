
/* ============================================================ */
/* Function: FUN_01180930 */
/* Entry: 01180930 */
/* Signature: undefined FUN_01180930() */
/* ============================================================ */


undefined4 * FUN_01180930(undefined4 *param_1,undefined2 param_2)

{
  char *pcVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  
  switch(param_2) {
  case 2:
    uVar2 = 0x14;
    pcVar1 = s_COMMON_TXT_ATTRIBUTE_01ade03c;
    break;
  case 3:
    uVar2 = 0x14;
    pcVar1 = s_COMMON_TXT_MAX_LEVEL_01ade0b0;
    break;
  case 4:
    uVar2 = 0x20;
    pcVar1 = s_TOOLTIP_ENCHANT_EXPLAIN_TITLE_AT_01ade08c;
    break;
  case 5:
    uVar2 = 0x11;
    pcVar1 = s_TOOLTIP_COOL_TIME_01ade0c8;
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
  uVar3 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar8 = 0;
  uVar7 = 0;
  FUN_00f19cc0(pcVar1,uVar2);
  FUN_015ce480(param_1,uVar3,uVar4,uVar5,uVar6,uVar7,uVar8);
  return param_1;
}


