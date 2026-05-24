
/* ============================================================ */
/* Function: FUN_01202880 */
/* Entry: 01202880 */
/* Signature: undefined FUN_01202880() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_01202880(int param_1,undefined1 *param_2)

{
  uint uVar1;
  char *pcVar2;
  undefined4 ******ppppppuVar3;
  undefined4 uVar4;
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
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  if (*(int *)(param_1 + 0x18) == 0) {
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
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1d48;
    goto LAB_0120296a;
  case 2:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1aac;
    goto LAB_0120296a;
  case 3:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1dc4;
    uVar4 = 0x2a;
    break;
  case 4:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1d9c;
    goto LAB_0120296a;
  case 5:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1e18;
    uVar4 = 0x2a;
    break;
  case 6:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1df0;
    uVar4 = 0x27;
    break;
  case 7:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1e70;
    uVar4 = 0x2b;
    break;
  case 8:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1e44;
    uVar4 = 0x28;
    break;
  case 9:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1ec4;
    uVar4 = 0x2c;
    break;
  case 10:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1e9c;
LAB_0120296a:
    uVar4 = 0x26;
    break;
  case 0xb:
    pcVar2 = s_DigiCombine_DigiCombine_Gacha_Bg_01ae1ef4;
    uVar4 = 0x2a;
  }
  ExceptionList = &local_1c;
  local_24 = uVar1;
  FUN_00f264c0(pcVar2,uVar4);
  ppppppuVar3 = &local_3c;
  if (0xf < local_28) {
    ppppppuVar3 = (undefined4 ******)local_3c;
  }
  FUN_0122d970(ppppppuVar3);
  FUN_00f1a420(uVar1);
  ExceptionList = local_1c;
  return;
}


