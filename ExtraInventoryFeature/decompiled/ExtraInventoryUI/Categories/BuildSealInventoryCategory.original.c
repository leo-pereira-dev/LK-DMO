
/* ============================================================ */
/* Function: FUN_011ad850 */
/* Entry: 011ad850 */
/* Signature: undefined FUN_011ad850() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 __thiscall FUN_011ad850(int param_1,int param_2,undefined4 *param_3,undefined4 *param_4)

{
  short sVar1;
  char cVar2;
  undefined1 uVar3;
  int iVar4;
  short *psVar5;
  short *psVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 local_70 [12];
  undefined1 local_64 [8];
  undefined1 *local_5c;
  undefined1 local_51;
  undefined8 local_50;
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
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0198b7e5;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  cVar2 = FUN_011a2420(local_14);
  uVar3 = 0;
  if (cVar2 != '\0') {
    FUN_012459a0(*(undefined4 *)(param_2 + 0x40),0,0,0,0,1,*param_3,param_3[1],*param_4,param_4[1]);
    iVar4 = FUN_012462c0(s_inventory_Invensort_tga_01ae0ee8,0x17,1,*(int *)(param_1 + 0x38) + -0x1e,
                         5,0x14,0x14,0,0x14);
    *(int *)(param_1 + 0x8c) = iVar4;
    if (iVar4 != 0) {
      FUN_011afe70(1,param_1,FUN_011af160,0);
      iVar4 = *(int *)(param_1 + 0x78);
      local_51 = 0xb;
      FUN_00f489b0(local_64,&local_51);
      if (((local_5c[0xd] == '\0') && ((byte)local_5c[0x10] < 0xc)) &&
         (local_5c != *(undefined1 **)(iVar4 + 0x10))) {
        iVar4 = *(int *)(local_5c + 0x14);
      }
      else {
        iVar4 = 0;
      }
      FUN_012254b0(-(uint)(iVar4 != 0) & iVar4 + 8U);
    }
    iVar4 = FUN_012462c0(s_CommonUI_CommonUI_btn_s_tga_01adfeb4,2,1,
                         *(int *)(param_1 + 0x38) + -0x80 >> 1,*(int *)(param_1 + 0x3c) + -0x3c,0x80
                         ,0x30,0,0x30);
    *(int *)(param_1 + 0x90) = iVar4;
    if (iVar4 != 0) {
      local_34 = 0;
      local_30 = 7;
      uStack_40 = 0;
      uStack_3c = 0;
      uStack_38 = 0;
      local_44 = 0;
      local_18 = 0;
      local_8 = 0;
      local_24 = DAT_01bfe468 + 8;
      local_5c = &DAT_3f7ae148;
      local_50 = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_20 = 9;
      local_48 = &DAT_3f7ae148;
      local_2c = 1;
      local_1c = 0;
      local_28 = 0;
      uVar7 = 0;
      uVar8 = 0;
      uVar9 = 0;
      uVar10 = 0;
      uVar11 = 0;
      uVar12 = 0;
      FUN_00f19cc0(s_EXTRA_INVEN_SEAL_BTN_ALLGET_01ae13c8,0x1b);
      psVar5 = (short *)FUN_015ce480(local_70,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
      local_8._0_1_ = 1;
      if (7 < *(uint *)(psVar5 + 10)) {
        psVar5 = *(short **)psVar5;
      }
      psVar6 = psVar5;
      do {
        sVar1 = *psVar6;
        psVar6 = psVar6 + 1;
      } while (sVar1 != 0);
      FUN_00f1a040(psVar5,(int)psVar6 - (int)(psVar5 + 1) >> 1);
      local_8 = (uint)local_8._1_3_ << 8;
      FUN_00f1a330();
      local_28 = 1;
      FUN_01224f40(&local_50);
      FUN_011afe70(1,param_1,FUN_011af1c0,0);
      local_8 = 0xffffffff;
      local_24 = 0;
      FUN_00f1a330();
    }
    FUN_011adee0();
    FUN_011ae540();
    uVar3 = 1;
  }
  ExceptionList = local_10;
  return uVar3;
}


