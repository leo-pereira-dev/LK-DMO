
/* ============================================================ */
/* Function: FUN_01202700 */
/* Entry: 01202700 */
/* Signature: undefined FUN_01202700() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01202700(int param_1,undefined1 *param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  char *pcVar4;
  undefined4 ******ppppppuVar5;
  undefined4 uVar6;
  undefined4 *****local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_0199147d;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  if (*(int *)(param_1 + 0x1c) == 0) {
    return;
  }
  local_2c = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_28 = 0xf;
  local_3c = (undefined4 ******)0x0;
  local_14 = 0;
  switch(*param_2) {
  default:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1b98;
    goto LAB_012027ea;
  case 2:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1b28;
    goto LAB_012027ea;
  case 3:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1c10;
    uVar6 = 0x2b;
    break;
  case 4:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1be8;
    goto LAB_012027ea;
  case 5:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1c68;
    uVar6 = 0x2b;
    break;
  case 6:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1c3c;
    uVar6 = 0x28;
    break;
  case 7:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1cc0;
    uVar6 = 0x2c;
    break;
  case 8:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1c94;
    uVar6 = 0x29;
    break;
  case 9:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1d18;
    uVar6 = 0x2d;
    break;
  case 10:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1cf0;
LAB_012027ea:
    uVar6 = 0x27;
    break;
  case 0xb:
    pcVar4 = s_DigiCombine_DigiCombine_Gacha_En_01ae1d70;
    uVar6 = 0x2b;
  }
  ExceptionList = &local_1c;
  local_24 = uVar3;
  FUN_00f264c0(pcVar4,uVar6);
  ppppppuVar5 = &local_3c;
  if (0xf < local_28) {
    ppppppuVar5 = (undefined4 ******)local_3c;
  }
  FUN_0122d970(ppppppuVar5);
  iVar1 = *(int *)(param_1 + 0x1c);
  iVar2 = *(int *)(iVar1 + 0x24);
  if (iVar2 != 0) {
    *(undefined1 *)(iVar2 + 0x78) = 0;
    iVar1 = *(int *)(iVar1 + 0x24);
    *(undefined4 *)(iVar1 + 0x6c) = 0;
    *(undefined4 *)(iVar1 + 0x68) = *(undefined4 *)(iVar1 + 100);
  }
  FUN_00f1a420(uVar3);
  ExceptionList = local_1c;
  return;
}


