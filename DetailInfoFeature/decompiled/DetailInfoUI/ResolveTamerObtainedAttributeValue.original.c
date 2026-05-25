
/* ============================================================ */
/* Function: FUN_011dcf20 */
/* Entry: 011dcf20 */
/* Signature: undefined FUN_011dcf20() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_011dcf20(int param_1)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_88 [8];
  int local_80;
  undefined1 auStack_7c [8];
  int iStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined1 uStack_2c;
  uint local_14;
  
  local_14 = DAT_01bbb1c0 ^ (uint)auStack_88;
  if (param_1 != 0) {
    iVar2 = __RTDynamicCast(*(undefined4 *)(param_1 + 8),0,&DAT_01bc4a94,&DAT_01bc6300,0);
    if ((iVar2 != 0) && (*(int *)(iVar2 + 4) != 0)) {
      FUN_00f1b390(auStack_7c,iVar2 + 8);
      if ((*(char *)(iStack_74 + 0xd) != '\0') ||
         (*(uint *)(iVar2 + 8) < *(uint *)(iStack_74 + 0x10))) {
        iStack_74 = *(int *)(local_80 + 0x94);
      }
      iVar1 = *(int *)(iStack_74 + 0x14);
      uStack_70 = *(undefined4 *)(iVar1 + 4);
      uStack_6c = *(undefined4 *)(iVar1 + 8);
      uStack_68 = *(undefined4 *)(iVar1 + 0xc);
      uStack_64 = *(undefined4 *)(iVar1 + 0x10);
      uStack_60 = *(undefined4 *)(iVar1 + 0x14);
      uStack_5c = *(undefined4 *)(iVar1 + 0x18);
      uStack_58 = *(undefined4 *)(iVar1 + 0x1c);
      uStack_54 = *(undefined4 *)(iVar1 + 0x20);
      uStack_50 = *(undefined4 *)(iVar1 + 0x24);
      uStack_4c = *(undefined4 *)(iVar1 + 0x28);
      uStack_48 = *(undefined4 *)(iVar1 + 0x2c);
      uStack_44 = *(undefined4 *)(iVar1 + 0x30);
      uStack_40 = *(undefined4 *)(iVar1 + 0x34);
      uStack_3c = *(undefined4 *)(iVar1 + 0x38);
      uStack_38 = *(undefined4 *)(iVar1 + 0x3c);
      uStack_34 = *(undefined4 *)(iVar1 + 0x40);
      uStack_30 = *(undefined4 *)(iVar1 + 0x44);
      uStack_2c = *(undefined1 *)(iVar1 + 0x48);
      FUN_013c19c0(*(undefined4 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x24),0x20,0x20,0x168,1
                   ,*(undefined4 *)(iVar2 + 4),0,0x9b,0,0,0,0,0,&uStack_70,0,0);
      FUN_013e32b0();
      return 1;
    }
  }
  return 0;
}


