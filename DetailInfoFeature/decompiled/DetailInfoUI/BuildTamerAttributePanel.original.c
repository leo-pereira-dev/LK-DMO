
/* ============================================================ */
/* Function: FUN_011db840 */
/* Entry: 011db840 */
/* Signature: undefined FUN_011db840() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011db840(int param_1)

{
  undefined4 uVar1;
  void *pvVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 extraout_ECX;
  undefined4 uVar6;
  undefined8 uVar7;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01993943;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  pvVar2 = (void *)FUN_016830c0(0xa0,local_24);
  local_14 = 0;
  if (pvVar2 == (void *)0x0) {
    iVar3 = 0;
  }
  else {
    memset(pvVar2,0,0xa0);
    iVar3 = FUN_0123de10();
  }
  uVar5 = DAT_01bb9b34;
  uVar7 = DAT_01bb9b2c;
  local_14 = 0xffffffff;
  if (iVar3 != 0) {
    uVar1 = *(undefined4 *)(param_1 + 0x40);
    FUN_00f19f10();
    local_2c = 0;
    uStack_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    local_28 = 0xf;
    local_3c = 0;
    local_14 = 1;
    FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_DetailInfo_TamerStatus_win_02_bg_01ae3438);
    FUN_0124ef30(&local_3c);
    uVar6 = 0;
    uVar4 = FUN_0124f150(0,uVar7,uVar5);
    FUN_0123e330(uVar1,0x1a,0xaa,0xd2,0x143,uVar4,uVar6,uVar7,uVar5);
    local_14 = 0xffffffff;
    FUN_00f1a420();
    FUN_012472c0(iVar3);
    pvVar2 = (void *)FUN_016830c0(0xac);
    local_14 = 2;
    if (pvVar2 == (void *)0x0) {
      uVar5 = 0;
    }
    else {
      memset(pvVar2,0,0xac);
      uVar5 = FUN_01228950();
    }
    local_14 = 0xffffffff;
    *(undefined4 *)(param_1 + 0x7c) = uVar5;
    pvVar2 = (void *)FUN_016830c0(0xac);
    local_14 = 3;
    if (pvVar2 == (void *)0x0) {
      iVar3 = 0;
    }
    else {
      memset(pvVar2,0,0xac);
      iVar3 = FUN_01228950();
    }
    local_14 = 0xffffffff;
    *(int *)(param_1 + 0x80) = iVar3;
    if ((*(int *)(param_1 + 0x7c) != 0) && (iVar3 != 0)) {
      FUN_01228c50(*(undefined4 *)(param_1 + 0x40),0x1a,0xaa,0x69,0x143,0xe,0xc,0x22,0x22,6,3,0,
                   *(int *)(param_1 + 0x7c),7);
      iVar3 = *(int *)(param_1 + 0x7c);
      *(undefined4 *)(iVar3 + 0xa0) = 6;
      *(undefined4 *)(iVar3 + 0xa4) = 7;
      FUN_01228c50(*(undefined4 *)(param_1 + 0x40),0x83,0xaa,0x69,0x143,0xe,0xc,0x22,0x22,7,1,0,
                   extraout_ECX,7);
      iVar3 = *(int *)(param_1 + 0x80);
      *(undefined4 *)(iVar3 + 0xa0) = 7;
      *(undefined4 *)(iVar3 + 0xa4) = 7;
      FUN_01228dd0(s_Icon_Mask_Over_dds_01ad4b4c,&DAT_01bee88c,*(int *)(param_1 + 0x7c) + 0x78);
      FUN_01228dd0(s_Icon_Mask_Over_dds_01ad4b4c,&DAT_01bee88c,*(int *)(param_1 + 0x80) + 0x78);
      *(undefined1 *)(*(int *)(param_1 + 0x7c) + 0x94) = 0;
      iVar3 = *(int *)(param_1 + 0x7c);
      *(undefined1 *)(iVar3 + 0xa9) = 0;
      if (*(int *)(iVar3 + 0x6c) != 0) {
        *(undefined1 *)(*(int *)(iVar3 + 0x6c) + 0x10) = 0;
        *(undefined4 *)(iVar3 + 0x6c) = 0;
      }
      *(undefined1 *)(*(int *)(param_1 + 0x80) + 0x94) = 0;
      iVar3 = *(int *)(param_1 + 0x80);
      *(undefined1 *)(iVar3 + 0xa9) = 0;
      if (*(int *)(iVar3 + 0x6c) != 0) {
        *(undefined1 *)(*(int *)(iVar3 + 0x6c) + 0x10) = 0;
        *(undefined4 *)(iVar3 + 0x6c) = 0;
      }
      FUN_012472c0(*(undefined4 *)(param_1 + 0x7c));
      FUN_012472c0(*(undefined4 *)(param_1 + 0x80));
      FUN_0122a180();
      FUN_0122a180();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
      FUN_011dcbe0();
    }
  }
  ExceptionList = local_1c;
  return;
}


