
/* ============================================================ */
/* Function: FUN_012b1e10 */
/* Entry: 012b1e10 */
/* Signature: undefined FUN_012b1e10() */
/* ============================================================ */


undefined4 * __thiscall FUN_012b1e10(int param_1,undefined4 *param_2)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  char *pcVar4;
  undefined4 uVar5;
  int local_8 [2];
  
  if (((((DAT_01bfb2b4 == 0) || (DAT_01bfb2b4 + 0x28 == 0)) ||
       (uVar1 = *(uint *)(param_1 + 0x48), *(uint *)(DAT_01bfb2b4 + 0x44) <= uVar1)) ||
      ((199 < uVar1 ||
       (piVar2 = (int *)(uVar1 * 0x45 + 0x30 + DAT_01bfb2b4 + 0x28), piVar2 == (int *)0x0)))) ||
     ((*piVar2 == 0 && ((short)piVar2[1] == 0)))) {
    iVar3 = 0;
  }
  else {
    iVar3 = *piVar2;
  }
  if ((*(int *)(param_1 + 0x140) == 0) ||
     (local_8[0] = iVar3, iVar3 = FUN_015db910(local_8), iVar3 == 0)) {
switchD_012b1e7e_caseD_1a:
    uVar5 = 0;
    pcVar4 = &DAT_01abd9ec;
  }
  else {
    switch(*(undefined2 *)(iVar3 + 0x18)) {
    case 0x15:
    case 0x1b:
    case 0x23:
    case 0x24:
      uVar5 = 0x14;
      pcVar4 = s_EI_Item_Outbox_1_tga_01ad6ee8;
      break;
    case 0x16:
      uVar5 = 0x14;
      pcVar4 = s_EI_Item_Outbox_2_tga_01aeb184;
      break;
    case 0x17:
      uVar5 = 0x14;
      pcVar4 = s_EI_Item_Outbox_3_tga_01aeb1d0;
      break;
    case 0x18:
      uVar5 = 0x14;
      pcVar4 = s_EI_Item_Outbox_4_tga_01aeb1b8;
      break;
    case 0x19:
      uVar5 = 0x14;
      pcVar4 = s_EI_Item_Outbox_5_tga_01ad8674;
      break;
    default:
      goto switchD_012b1e7e_caseD_1a;
    }
  }
  *param_2 = 0;
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[4] = 0;
  param_2[5] = 0;
  FUN_00f19cc0(pcVar4,uVar5);
  return param_2;
}


