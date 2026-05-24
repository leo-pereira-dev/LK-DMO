
/* ============================================================ */
/* Function: FUN_01056780 */
/* Entry: 01056780 */
/* Signature: undefined FUN_01056780() */
/* ============================================================ */


undefined4 * FUN_01056780(undefined4 *param_1,undefined1 *param_2)

{
  char *pcVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  
  switch(*param_2) {
  case 0xb:
    uVar2 = 0x15;
    pcVar1 = s_EXTRA_INVEN_LIST_SEAL_01ad0d38;
    break;
  case 0xc:
    uVar2 = 0x17;
    pcVar1 = s_EXTRA_INVEN_LIST_TICKET_01ad0d50;
    break;
  case 0xd:
    uVar2 = 0x1a;
    pcVar1 = s_EXTRA_INVEN_LIST_Evolution_01ad0d68;
    break;
  case 0xe:
    uVar2 = 0x19;
    pcVar1 = s_EXTRA_INVEN_LIST_DIGITAMA_01ad0d84;
    break;
  case 0xf:
    uVar2 = 0x19;
    pcVar1 = s_EXTRA_INVEN_LIST_MATERIAL_01ad0da0;
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


