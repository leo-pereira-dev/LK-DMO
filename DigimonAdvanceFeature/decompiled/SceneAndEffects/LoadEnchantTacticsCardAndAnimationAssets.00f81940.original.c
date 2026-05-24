
/* ============================================================ */
/* Function: FUN_00f81940 */
/* Entry: 00f81940 */
/* Signature: undefined FUN_00f81940() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00f81940(void)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  undefined4 uVar4;
  void *pvVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  float fVar8;
  float fVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 local_58;
  undefined4 uStack_54;
  float fStack_50;
  float fStack_4c;
  longlong local_48;
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
  
  iVar2 = DAT_01bfb2b8;
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0196a703;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  FUN_00f808f0(local_24);
  fVar8 = (float)DAT_01bc07f0 * _DAT_01b025bc;
  pvVar5 = (void *)FUN_016830c0(0xa0);
  local_14 = 0;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0xa0);
    uVar6 = FUN_0123de10();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x70) = uVar6;
  uVar4 = DAT_01bc07ec;
  uVar6 = DAT_01bb9b34;
  lVar3 = DAT_01bb9b2c;
  FUN_00f19f10(s_EnchantTactics_card_base_tga_01ac8ed4,0x1c);
  uStack_54 = 0;
  fStack_50 = 0.0;
  fStack_4c = 0.0;
  local_48 = 0xf00000000;
  local_58 = 0;
  local_14 = 1;
  FUN_015010a0(&local_58,&DAT_01ae7290,&DAT_01bfbd40,s_EnchantTactics_card_base_tga_01ac8ed4);
  FUN_0124ef30(&local_58);
  uVar12 = 0;
  uVar10 = (undefined4)lVar3;
  uVar11 = (undefined4)((ulonglong)lVar3 >> 0x20);
  uVar7 = FUN_0124f150(0,uVar10,uVar11,uVar6);
  FUN_0123e330(0,0,10,uVar4,(int)fVar8,uVar7,uVar12,uVar10,uVar11,uVar6);
  local_14 = 0xffffffff;
  FUN_00f1a420();
  (**(code **)(**(int **)(iVar2 + 0x70) + 0x4c))(0x3f4ccccd);
  fVar8 = (float)DAT_01bc07f0 * _DAT_01b025ac;
  pvVar5 = (void *)FUN_016830c0(0xa0);
  local_14 = 2;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0xa0);
    uVar6 = FUN_0123de10();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x74) = uVar6;
  uVar4 = DAT_01bc07ec;
  uVar6 = DAT_01bb9b34;
  fVar9 = ((float)DAT_01bc07f0 - fVar8) - DAT_01b028bc;
  local_48 = DAT_01bb9b2c;
  FUN_00f19f10(s_EnchantTactics_card_base2_tga_01ac8ef4,0x1d);
  local_2c = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_28 = 0xf;
  local_3c = 0;
  local_14 = 3;
  FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_EnchantTactics_card_base2_tga_01ac8ef4);
  FUN_0124ef30(&local_3c);
  uVar12 = 0;
  uVar10 = (undefined4)local_48;
  uVar11 = (undefined4)((ulonglong)local_48 >> 0x20);
  uVar7 = FUN_0124f150(0,uVar10,uVar11,uVar6);
  FUN_0123e330(0,0,(int)fVar9,uVar4,(int)fVar8,uVar7,uVar12,uVar10,uVar11,uVar6);
  local_14 = 0xffffffff;
  FUN_00f1a420();
  (**(code **)(**(int **)(iVar2 + 0x74) + 0x4c))(0x3f4ccccd);
  pvVar5 = (void *)FUN_016830c0(0xa0);
  local_14 = 4;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0xa0);
    uVar6 = FUN_0123de10();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x6c) = uVar6;
  uVar7 = DAT_01bee890;
  uVar4 = DAT_01bee88c;
  uVar6 = DAT_01bb9b34;
  local_48 = DAT_01bb9b2c;
  FUN_00f19f10(s_EnchantTactics_NO_tga_01ac8f14,0x15);
  local_2c = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_28 = 0xf;
  local_3c = 0;
  local_14 = 5;
  FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_EnchantTactics_NO_tga_01ac8f14);
  FUN_0124ef30(&local_3c);
  uVar10 = 0;
  uVar11 = (undefined4)local_48;
  uVar13 = (undefined4)((ulonglong)local_48 >> 0x20);
  uVar12 = FUN_0124f150(0,uVar11,uVar13,uVar6);
  FUN_0123e330(0,uVar4,uVar7,0x177,0xb2,uVar12,uVar10,uVar11,uVar13,uVar6);
  local_14 = 0xffffffff;
  FUN_00f1a420();
  fVar9 = DAT_01b02768;
  iVar1 = *(int *)(iVar2 + 0x6c);
  fVar8 = *(float *)(iVar1 + 0x74);
  if ((((*(float *)(iVar1 + 0x68) != DAT_01b02768) || (*(float *)(iVar1 + 0x6c) != DAT_01b02768)) ||
      (*(float *)(iVar1 + 0x70) != 0.0)) || (*(float *)(iVar1 + 0x74) != fVar8)) {
    fStack_50 = DAT_01b02768;
    fStack_4c = DAT_01b02768;
    local_48 = (ulonglong)(uint)fVar8 << 0x20;
    *(float *)(iVar1 + 0x68) = DAT_01b02768;
    *(float *)(iVar1 + 0x6c) = fVar9;
    *(undefined4 *)(iVar1 + 0x70) = 0;
    *(float *)(iVar1 + 0x74) = fVar8;
    if (*(int *)(iVar1 + 0x50) != 0) {
      FUN_016a9740(0,(float *)(iVar1 + 0x68));
    }
  }
  pvVar5 = (void *)FUN_016830c0(0xa0);
  local_14 = 6;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0xa0);
    uVar6 = FUN_0123de10();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x78) = uVar6;
  uVar7 = DAT_01bee890;
  uVar4 = DAT_01bee88c;
  uVar6 = DAT_01bb9b34;
  local_48 = DAT_01bb9b2c;
  FUN_00f19f10(s_EnchantTactics_CardSuccess_Succe_01ac8f2c,0x2a);
  local_2c = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_28 = 0xf;
  local_3c = 0;
  local_14 = 7;
  FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_EnchantTactics_CardSuccess_Succe_01ac8f2c);
  FUN_0124ef30(&local_3c);
  uVar10 = 0;
  uVar11 = (undefined4)local_48;
  uVar13 = (undefined4)((ulonglong)local_48 >> 0x20);
  uVar12 = FUN_0124f150(0,uVar11,uVar13,uVar6);
  FUN_0123e330(0,uVar4,uVar7,0x82,0xbe,uVar12,uVar10,uVar11,uVar13,uVar6);
  local_14 = 0xffffffff;
  FUN_00f1a420();
  pvVar5 = (void *)FUN_016830c0(0xa0);
  local_14 = 8;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0xa0);
    uVar6 = FUN_0123de10();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x7c) = uVar6;
  uVar7 = DAT_01bee890;
  uVar4 = DAT_01bee88c;
  uVar6 = DAT_01bb9b34;
  local_48 = DAT_01bb9b2c;
  FUN_00f19f10(s_EnchantTactics_CardFail_Fail_R_2_01ac8f58,0x25);
  local_2c = 0;
  uStack_38 = 0;
  uStack_34 = 0;
  uStack_30 = 0;
  local_28 = 0xf;
  local_3c = 0;
  local_14 = 9;
  FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,s_EnchantTactics_CardFail_Fail_R_2_01ac8f58);
  FUN_0124ef30(&local_3c);
  uVar10 = 0;
  uVar11 = (undefined4)local_48;
  uVar13 = (undefined4)((ulonglong)local_48 >> 0x20);
  uVar12 = FUN_0124f150(0,uVar11,uVar13,uVar6);
  FUN_0123e330(0,uVar4,uVar7,0x82,0xbe,uVar12,uVar10,uVar11,uVar13,uVar6);
  local_14 = 0xffffffff;
  FUN_00f1a420();
  pvVar5 = (void *)FUN_016830c0(0x8c);
  local_14 = 10;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0x8c);
    uVar6 = FUN_012404a0();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x80) = uVar6;
  FUN_01240640(0,0,DAT_01bee88c,DAT_01bee890,0x82,0xbe,s_EnchantTactics_CardAni_Card_R__01ac8f84,
               &DAT_01ac8f80,0x14,DAT_01bee890,DAT_01bee88c,DAT_01bee890,0,0);
  iVar1 = *(int *)(iVar2 + 0x80);
  *(undefined4 *)(iVar1 + 100) = 0x3d4ccccd;
  *(undefined4 *)(iVar1 + 0x68) = 0x3d4ccccd;
  pvVar5 = (void *)FUN_016830c0(0x8c);
  local_14 = 0xb;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0x8c);
    uVar6 = FUN_012404a0();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x8c) = uVar6;
  FUN_01240640(0,0,DAT_01bee88c,DAT_01bee890,0x82,0xbe,s_EnchantTactics_SelectAni_Card_C__01ac8fa4,
               &DAT_01ac8f80,0x14,DAT_01bee890,DAT_01bee88c,DAT_01bee890,0,0);
  iVar1 = *(int *)(iVar2 + 0x8c);
  *(undefined4 *)(iVar1 + 100) = 0x3d4ccccd;
  *(undefined4 *)(iVar1 + 0x68) = 0x3d4ccccd;
  pvVar5 = (void *)FUN_016830c0(0x8c);
  local_14 = 0xc;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0x8c);
    uVar6 = FUN_012404a0();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x84) = uVar6;
  FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,0x82,0xbe,s_EnchantTactics_CardSuccess_Succe_01ac8fc8,
               &DAT_01ac8f80,0x14,DAT_01bee890,DAT_01bee88c,DAT_01bee890,0,0);
  iVar1 = *(int *)(iVar2 + 0x84);
  *(undefined4 *)(iVar1 + 100) = 0x3cf5c28f;
  *(undefined4 *)(iVar1 + 0x68) = 0x3cf5c28f;
  pvVar5 = (void *)FUN_016830c0(0x8c);
  local_14 = 0xd;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0x8c);
    uVar6 = FUN_012404a0();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x88) = uVar6;
  FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,0x82,0xbe,s_EnchantTactics_CardFail_Fail_R__01ac8ff0,
               &DAT_01ac8f80,0x14,DAT_01bee890,DAT_01bee88c,DAT_01bee890,0,0);
  iVar1 = *(int *)(iVar2 + 0x88);
  *(undefined4 *)(iVar1 + 100) = 0x3cf5c28f;
  *(undefined4 *)(iVar1 + 0x68) = 0x3cf5c28f;
  pvVar5 = (void *)FUN_016830c0(0x8c);
  local_14 = 0xe;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0x8c);
    uVar6 = FUN_012404a0();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x90) = uVar6;
  FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,0x436,300,s_EnchantTactics_SuccessAni_Succes_01ac9010,
               &DAT_01ac8f80,0x1e,DAT_01bee890,DAT_01bee88c,DAT_01bee890,0,0);
  iVar1 = *(int *)(iVar2 + 0x90);
  *(undefined4 *)(iVar1 + 100) = 0x3e19999a;
  *(undefined4 *)(iVar1 + 0x68) = 0x3e19999a;
  pvVar5 = (void *)FUN_016830c0(0x8c);
  local_14 = 0xf;
  if (pvVar5 == (void *)0x0) {
    uVar6 = 0;
  }
  else {
    memset(pvVar5,0,0x8c);
    uVar6 = FUN_012404a0();
  }
  local_14 = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x94) = uVar6;
  FUN_01240640(1,0,DAT_01bee88c,DAT_01bee890,0x436,300,s_EnchantTactics_FailAni_Fail__01ac9034,
               &DAT_01ac8f80,0x1e,DAT_01bee890,DAT_01bee88c,DAT_01bee890,0,0);
  iVar2 = *(int *)(iVar2 + 0x94);
  *(undefined4 *)(iVar2 + 100) = 0x3e19999a;
  *(undefined4 *)(iVar2 + 0x68) = 0x3e19999a;
  ExceptionList = local_1c;
  return;
}


