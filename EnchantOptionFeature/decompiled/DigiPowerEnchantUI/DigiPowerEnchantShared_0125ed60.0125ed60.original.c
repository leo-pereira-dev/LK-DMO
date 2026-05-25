
/* ============================================================ */
/* Function: FUN_0125ed60 */
/* Entry: 0125ed60 */
/* Signature: undefined FUN_0125ed60() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0125ed60(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  uint uVar3;
  void *pvVar4;
  undefined4 uVar5;
  int *piVar6;
  undefined4 *puVar7;
  int iVar8;
  short *psVar9;
  short *psVar10;
  float10 fVar11;
  undefined4 local_64;
  undefined8 local_60;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined1 local_3c;
  undefined4 local_38;
  int local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199afb1;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar3;
  FUN_012457d0(0,1,0,0,1,(DAT_01bc07ec + -0x13d) / 2,(DAT_01bc07f0 + -0xf7) / 2,0x13d,300);
  uVar2 = DAT_01bee890;
  local_64 = DAT_01bee88c;
  pvVar4 = (void *)FUN_016830c0(0xa0,uVar3);
  local_14 = 0;
  if (pvVar4 == (void *)0x0) {
    uVar5 = 0;
  }
  else {
    memset(pvVar4,0,0xa0);
    uVar5 = FUN_0123de10();
  }
  local_14 = 0xffffffff;
  FUN_0123e0d0(*(undefined4 *)(param_1 + 0x40),s_Accessory_Enchant_Enchant_BG_tga_01ae839c,0,
               DAT_01bb9b2c,DAT_01bb9b34,1,local_64,uVar2,0x13d,0xf7);
  local_64 = uVar5;
  FUN_00f55430(&local_64);
  FUN_00f19cc0(s_ACCESSORY_ENCHANT_ENCHANTMENT_01ae837c,0x1d);
  FUN_015ce480();
  local_14 = 1;
  FUN_012461a0();
  local_14 = 0xffffffff;
  FUN_00f1a330();
  pvVar4 = (void *)FUN_016830c0(0x1ac);
  local_14 = 2;
  if (pvVar4 == (void *)0x0) {
    piVar6 = (int *)0x0;
  }
  else {
    memset(pvVar4,0,0x1ac);
    piVar6 = (int *)FUN_012435e0();
  }
  local_14 = 0xffffffff;
  *(int **)(param_1 + 0xb0) = piVar6;
  if (piVar6 != (int *)0x0) {
    (**(code **)(*piVar6 + 0xc))
              (*(undefined4 *)(param_1 + 0x40),DAT_01bee88c,DAT_01bee890,0x13d,0xf7,0);
    FUN_012611d0(1,param_1,FUN_01260f60,0);
    iVar8 = *(int *)(param_1 + 0xb0);
    *(undefined4 *)(iVar8 + 0xc0) = 0;
    *(undefined4 *)(iVar8 + 0xdc) = 0;
    puVar7 = (undefined4 *)(iVar8 + 0xb0);
    if (7 < *(uint *)(iVar8 + 0xc4)) {
      puVar7 = (undefined4 *)*puVar7;
    }
    *(undefined2 *)puVar7 = 0;
    piVar6 = *(int **)(param_1 + 0xb0);
    piVar6[0x3b] = 1;
    piVar6[0x3c] = 0;
    fVar11 = (float10)fmaxf(0,0x3fd56042);
    piVar6[0x32] = (int)(float)fVar11;
    FUN_01243010();
    (**(code **)(*piVar6 + 0x48))();
    FUN_01242bf0();
    *(undefined1 *)(*(int *)(param_1 + 0xb0) + 0xe4) = 0;
    FUN_012472c0(*(undefined4 *)(param_1 + 0xb0));
  }
  iVar8 = FUN_012462c0(s_Accessory_Enchant_Enchant_Enchan_01ae83e0,0x17,1,0x73,0xbe,0x56,0x1a,0,0x1a
                      );
  *(int *)(param_1 + 0xd0) = iVar8;
  if (iVar8 != 0) {
    local_44 = 0;
    local_40 = 7;
    uStack_50 = 0;
    uStack_4c = 0;
    uStack_48 = 0;
    local_54 = 0;
    local_28 = 0;
    local_14 = 3;
    local_34 = DAT_01bfe468 + 8;
    local_60 = DAT_01bb9b2c;
    local_30 = 9;
    local_58 = DAT_01bb9b34;
    local_3c = 1;
    local_2c = 0;
    local_38 = 1;
    FUN_00f19cc0(s_ACCESSORY_ENCHANT_ENCHANTMENT_01ae837c,0x1d);
    psVar9 = (short *)FUN_015ce480();
    local_14._0_1_ = 4;
    if (7 < *(uint *)(psVar9 + 10)) {
      psVar9 = *(short **)psVar9;
    }
    psVar10 = psVar9;
    do {
      sVar1 = *psVar10;
      psVar10 = psVar10 + 1;
    } while (sVar1 != 0);
    FUN_00f1a040(psVar9,(int)psVar10 - (int)(psVar9 + 1) >> 1);
    local_14 = CONCAT31(local_14._1_3_,3);
    FUN_00f1a330();
    FUN_01224f40(&local_60);
    FUN_012611d0();
    (**(code **)(**(int **)(param_1 + 0xd0) + 0x18))(0);
    local_14 = 0xffffffff;
    local_34 = 0;
    FUN_00f1a330();
  }
  iVar8 = FUN_012462c0(s_System_Ch_Close_tga_01ad561c,0x17,1,0x122,0xc,0x10,0x10,0,0x10);
  *(int *)(param_1 + 0xd4) = iVar8;
  if (iVar8 != 0) {
    FUN_012611d0();
  }
  FUN_0125f1d0();
  FUN_0125f450();
  FUN_0125f9f0();
  ExceptionList = local_1c;
  return;
}


