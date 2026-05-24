
/* ============================================================ */
/* Function: FUN_011dd050 */
/* Entry: 011dd050 */
/* Signature: undefined FUN_011dd050() */
/* ============================================================ */


undefined4 * FUN_011dd050(undefined4 *param_1,undefined4 param_2)

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
  case 0:
    uVar2 = 0x20;
    pcVar1 = s_TOOLTIP_ENCHANT_EXPLAIN_TITLE_HP_01ae34ac;
    break;
  case 1:
    uVar2 = 0xd;
    pcVar1 = s_COMMON_TXT_DS_01ac50e0;
    break;
  case 2:
    uVar2 = 0x20;
    pcVar1 = s_TOOLTIP_ENCHANT_EXPLAIN_TITLE_AT_01ade08c;
    break;
  case 3:
    uVar2 = 0x17;
    pcVar1 = s_COMMON_TXT_ATTACK_SPEED_01ac5194;
    break;
  case 4:
    uVar2 = 0x20;
    pcVar1 = s_TOOLTIP_ENCHANT_EXPLAIN_TITLE_CR_01ae3488;
    break;
  case 5:
    uVar2 = 0x18;
    pcVar1 = s_COMMON_TXT_STAT_HIT_RATE_01ac51c4;
    break;
  case 6:
    uVar2 = 0xf;
    pcVar1 = s_DETAIL_INFO_SCD_01ae34e8;
    break;
  case 7:
    uVar2 = 0x15;
    pcVar1 = s_DETAIL_INFO_CRIDAMAGE_01ae34d0;
    break;
  case 8:
    uVar2 = 0xe;
    pcVar1 = s_DETAIL_INFO_SD_01ae351c;
    break;
  case 9:
    uVar2 = 0x1b;
    pcVar1 = s_DETAIL_INFO_ELEDMENT_DAMAGE_01ac58e4;
    break;
  case 10:
    uVar2 = 0x17;
    pcVar1 = s_COMMON_TXT_STAT_DEFENSE_01ac5158;
    break;
  case 0xb:
    uVar2 = 0x20;
    pcVar1 = s_TOOLTIP_ENCHANT_EXPLAIN_TITLE_BL_01ae34f8;
    break;
  case 0xc:
    uVar2 = 0x20;
    pcVar1 = s_TOOLTIP_ENCHANT_EXPLAIN_TITLE_EV_01ae352c;
    break;
  default:
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    param_1[4] = 0;
    param_1[5] = 7;
    *(undefined2 *)param_1 = 0;
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


