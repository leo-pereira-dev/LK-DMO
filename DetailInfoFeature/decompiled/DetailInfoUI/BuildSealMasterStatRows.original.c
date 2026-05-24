
/* ============================================================ */
/* Function: FUN_011da150 */
/* Entry: 011da150 */
/* Signature: undefined FUN_011da150() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011da150(int param_1)

{
  short sVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  int *piVar7;
  undefined4 *puVar8;
  short *psVar9;
  undefined1 local_94 [32];
  undefined4 local_74;
  undefined1 *local_68;
  void *local_64;
  int local_60;
  short *local_5c;
  uint local_58;
  int local_54;
  longlong local_50;
  undefined1 *local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01993714;
  local_10 = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar5 = *(int *)(param_1 + 0x38);
  local_60 = (iVar5 + -0x20) / 3;
  local_54 = param_1;
  local_14 = uVar3;
  local_5c = (short *)FUN_016830c0(0xac,uVar3);
  local_8 = 0;
  if (local_5c == (short *)0x0) {
    iVar4 = 0;
  }
  else {
    memset(local_5c,0,0xac);
    iVar4 = FUN_01228950();
  }
  local_8 = 0xffffffff;
  *(int *)(local_54 + 0x80) = iVar4;
  if (iVar4 != 0) {
    FUN_01228c50(*(undefined4 *)(local_54 + 0x40),5,0x29,iVar5 + -10,0x42,0xb,6,local_60,0x12,0,2,0,
                 local_54,3);
    local_58 = 0;
    do {
      local_64 = (void *)FUN_016830c0(0x2c,uVar3);
      local_8 = 1;
      if (local_64 == (void *)0x0) {
        iVar5 = 0;
      }
      else {
        memset(local_64,0,0x2c);
        iVar5 = FUN_010baa10();
      }
      uVar2 = local_58;
      if (iVar5 == 0) {
        ExceptionList = local_10;
        return;
      }
      local_34 = 0;
      local_30 = 7;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_44 = 0;
      local_18 = 0;
      local_8 = 2;
      local_24 = DAT_01bfe468 + 8;
      local_68 = &DAT_3f7ae148;
      local_48 = &DAT_3f7ae148;
      local_20 = 8;
      local_50 = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_2c = 1;
      local_1c = 0;
      local_28 = 5;
      psVar6 = (short *)FUN_011da4b0(local_94,local_58);
      local_8._0_1_ = 3;
      if (7 < *(uint *)(psVar6 + 10)) {
        psVar6 = *(short **)psVar6;
      }
      local_5c = psVar6 + 1;
      psVar9 = psVar6;
      do {
        sVar1 = *psVar9;
        psVar9 = psVar9 + 1;
      } while (sVar1 != 0);
      FUN_00f1a040(psVar6,(int)psVar9 - (int)local_5c >> 1);
      local_8 = CONCAT31(local_8._1_3_,2);
      FUN_00f1a330();
      piVar7 = (int *)FUN_01230c40(&local_50,0x19,9);
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))(1);
      }
      local_24 = DAT_01bfe468 + 8;
      local_74 = 0;
      local_48 = (undefined1 *)0x0;
      local_20 = 8;
      local_50 = (ulonglong)DAT_01b02744 << 0x20;
      local_2c = 1;
      local_1c = 0;
      local_28 = 4;
      psVar6 = (short *)FUN_011da5e0(local_94,uVar2);
      local_8._0_1_ = 4;
      if (7 < *(uint *)(psVar6 + 10)) {
        psVar6 = *(short **)psVar6;
      }
      psVar9 = psVar6;
      do {
        sVar1 = *psVar9;
        psVar9 = psVar9 + 1;
      } while (sVar1 != 0);
      FUN_00f1a040(psVar6,(int)psVar9 - (int)(psVar6 + 1) >> 1);
      local_8 = CONCAT31(local_8._1_3_,2);
      FUN_00f1a330();
      piVar7 = (int *)FUN_01230c40(&local_50,0x37,9);
      if (piVar7 != (int *)0x0) {
        (**(code **)(*piVar7 + 0x54))(1);
      }
      puVar8 = (undefined4 *)FUN_016830c0(0x28);
      iVar4 = local_54;
      local_5c = (short *)puVar8;
      if (puVar8 != (undefined4 *)0x0) {
        *puVar8 = 0;
        puVar8[1] = 0;
        puVar8[2] = 0;
        puVar8[3] = 0;
        puVar8[4] = 0;
        puVar8[5] = 0;
        puVar8[6] = 0;
        puVar8[7] = 0;
        *(undefined8 *)(puVar8 + 8) = 0;
        puVar8[3] = local_58;
        *puVar8 = &DAT_01ae66e0;
        puVar8[1] = 0;
        puVar8[2] = 0;
        *(undefined1 *)(puVar8 + 4) = 0;
        puVar8[5] = local_60;
        puVar8[6] = 0x12;
        *(undefined2 *)(puVar8 + 7) = 0x101;
        puVar8[8] = 0;
        puVar8[9] = 0;
        FUN_012288c0(iVar5);
        iVar4 = local_54;
        FUN_0122a090(puVar8);
      }
      local_8 = 0xffffffff;
      local_24 = 0;
      FUN_00f1a330();
      local_58 = local_58 + 1;
    } while (local_58 < 9);
    FUN_012472c0(*(undefined4 *)(iVar4 + 0x80));
  }
  ExceptionList = local_10;
  return;
}


