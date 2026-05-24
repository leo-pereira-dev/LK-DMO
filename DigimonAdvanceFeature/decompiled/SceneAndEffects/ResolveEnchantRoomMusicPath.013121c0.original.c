
/* ============================================================ */
/* Function: FUN_013121c0 */
/* Entry: 013121c0 */
/* Signature: undefined FUN_013121c0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

char * FUN_013121c0(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 ***pppuVar3;
  undefined4 uVar4;
  undefined *puVar5;
  undefined *puVar6;
  undefined4 uVar7;
  int iStack_48;
  int iStack_44;
  undefined8 uStack_40;
  undefined8 uStack_34;
  undefined4 local_28;
  undefined4 **appuStack_24 [4];
  int iStack_14;
  uint uStack_10;
  uint local_c;
  
  local_c = DAT_01bbb1c0 ^ (uint)&iStack_48;
  if (*(int *)(DAT_01bfb2d8 + 0x58) == 3) {
    return s_Enchant_Room_mp3_01ac8de8;
  }
  uVar7 = 0;
  puVar6 = &DAT_01bc357c;
  local_28 = *(undefined4 *)(DAT_01bfe478 + 0x4c0);
  puVar5 = &DAT_01bc2a20;
  uVar4 = 0;
  uVar1 = FUN_015cea10(0x21);
  iVar2 = __RTDynamicCast(uVar1,uVar4,puVar5,puVar6,uVar7);
  if (iVar2 != 0) {
    uVar7 = 0;
    puVar6 = &DAT_01bc3558;
    puVar5 = &DAT_01bc2a20;
    uVar4 = 0;
    uVar1 = FUN_015cea10(0x24);
    iVar2 = __RTDynamicCast(uVar1,uVar4,puVar5,puVar6,uVar7);
    if (iVar2 != 0) {
      iVar2 = FUN_015d17e0(&local_28);
      if (iVar2 != 0) {
        uStack_34 = *(undefined8 *)(*(int *)(DAT_01bfb2a8 + 8) + 0xd4);
        uStack_40._4_4_ = (float)((ulonglong)uStack_34 >> 0x20);
        iStack_48 = (int)uStack_40._4_4_;
        iStack_44 = (int)(float)uStack_34;
        uStack_40 = uStack_34;
        FUN_0160b740(appuStack_24,&local_28,&iStack_44,&iStack_48);
        if (iStack_14 != 0) {
          pppuVar3 = appuStack_24;
          if (0xf < uStack_10) {
            pppuVar3 = (undefined4 ***)appuStack_24[0];
          }
          FUN_00f1a420();
          return (char *)pppuVar3;
        }
        if (*(int *)(iVar2 + 0x60) == 0) {
          pppuVar3 = (undefined4 ***)0x0;
        }
        else {
          pppuVar3 = (undefined4 ***)(iVar2 + 0x50);
          if (0xf < *(uint *)(iVar2 + 100)) {
            pppuVar3 = (undefined4 ***)*pppuVar3;
            FUN_00f1a420();
            return (char *)pppuVar3;
          }
        }
        FUN_00f1a420();
        return (char *)pppuVar3;
      }
    }
  }
  return (char *)(undefined4 ***)0x0;
}


