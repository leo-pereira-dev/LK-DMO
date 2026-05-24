
/* ============================================================ */
/* Function: FUN_011e3360 */
/* Entry: 011e3360 */
/* Signature: undefined FUN_011e3360() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011e3360(int param_1,undefined4 param_2,int param_3,int param_4)

{
  short sVar1;
  char cVar2;
  uint uVar3;
  undefined4 *puVar4;
  void *pvVar5;
  short *psVar6;
  undefined4 uVar7;
  int *piVar8;
  int iVar9;
  short ****ppppsVar10;
  short *psVar11;
  undefined4 extraout_ECX;
  short ****ppppsVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined1 local_28c [60];
  undefined1 local_250 [24];
  undefined4 local_238;
  undefined4 uStack_234;
  undefined4 uStack_230;
  undefined4 uStack_22c;
  undefined4 local_228;
  undefined4 local_224;
  undefined4 local_220;
  undefined4 uStack_21c;
  undefined4 uStack_218;
  undefined4 uStack_214;
  int local_208;
  undefined1 local_201;
  undefined4 local_200;
  undefined4 *local_1fc;
  undefined8 local_1f8;
  undefined1 *local_1f0;
  undefined4 local_1ec;
  undefined4 uStack_1e8;
  undefined4 uStack_1e4;
  undefined4 uStack_1e0;
  undefined4 local_1dc;
  undefined4 local_1d8;
  undefined1 local_1d4;
  undefined4 local_1d0;
  int local_1cc;
  undefined4 local_1c8;
  undefined4 local_1c4;
  undefined4 local_1c0;
  undefined8 local_1bc;
  undefined1 *local_1b4;
  undefined4 local_1b0;
  undefined4 uStack_1ac;
  undefined4 uStack_1a8;
  undefined4 uStack_1a4;
  undefined4 local_1a0;
  undefined4 local_19c;
  undefined1 local_198;
  undefined4 local_194;
  int local_190;
  undefined4 local_18c;
  undefined4 local_188;
  undefined4 local_184;
  undefined8 local_180;
  undefined1 *local_178;
  undefined4 local_174;
  undefined4 uStack_170;
  undefined4 uStack_16c;
  undefined4 uStack_168;
  undefined4 local_164;
  undefined4 local_160;
  undefined1 local_15c;
  undefined4 local_158;
  int local_154;
  undefined4 local_150;
  undefined4 local_14c;
  undefined4 local_148;
  undefined8 local_144;
  undefined1 *local_13c;
  undefined4 local_138;
  undefined4 uStack_134;
  undefined4 uStack_130;
  undefined4 uStack_12c;
  undefined4 local_128;
  undefined4 local_124;
  undefined1 local_120;
  undefined4 local_11c;
  int local_118;
  undefined4 local_114;
  undefined4 local_110;
  undefined4 local_10c;
  undefined8 local_108;
  undefined1 *local_100;
  undefined4 local_fc;
  undefined4 uStack_f8;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined1 local_e4;
  undefined4 local_e0;
  int local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined8 local_cc;
  undefined1 *local_c4;
  undefined4 local_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined1 local_a8;
  undefined4 local_a4;
  int local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined8 local_78;
  undefined1 *local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined1 local_54;
  undefined4 local_50;
  int local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  short ***local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined1 *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019943e4;
  local_1c = ExceptionList;
  uVar3 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  *(undefined4 *)(param_1 + 0x88) = 0x1c;
  local_24 = uVar3;
  FUN_012459a0(0,s_EventPopup_Event_popup_Backgroun_01ae0c38,0,0,0,1,param_3 + -0x119,
               param_4 + -0xcf,0x232,0x19f);
  uStack_30 = 0;
  uStack_2c = 0x3f800000;
  local_28 = (undefined1 *)0x3f800000;
  uVar7 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar15 = 0;
  uVar16 = 0;
  uVar17 = 0;
  FUN_00f19cc0(s_MAINOPTION_MSGBOX_ALERT_01ace1c0,0x17);
  puVar4 = (undefined4 *)FUN_015ce480(&local_238,uVar7,uVar13,uVar14,uVar15,uVar16,uVar17);
  local_14 = 0;
  if (7 < (uint)puVar4[5]) {
    puVar4 = (undefined4 *)*puVar4;
  }
  FUN_012461a0(puVar4,0xd,uStack_30,uStack_2c,local_28,0,0);
  FUN_00f1a330(uVar3);
  local_80 = 0;
  local_7c = 7;
  uStack_8c = 0;
  uStack_88 = 0;
  uStack_84 = 0;
  local_90 = 0;
  local_14 = 1;
  FUN_01500f80(&local_90,&DAT_01acc21c,0);
  puVar4 = (undefined4 *)FUN_016830c0(0x20);
  local_14._0_1_ = 2;
  local_1fc = puVar4;
  if (puVar4 == (undefined4 *)0x0) {
    puVar4 = (undefined4 *)0x0;
  }
  else {
    *puVar4 = 0;
    puVar4[1] = 0;
    puVar4[2] = 0;
    puVar4[3] = 0;
    puVar4[4] = 0;
    puVar4[5] = 0;
    puVar4[6] = 0;
    puVar4[7] = 0;
    puVar4[1] = 0;
    puVar4[2] = 0;
    puVar4[3] = 0;
    pvVar5 = operator_new(0xc);
    *(void **)pvVar5 = pvVar5;
    *(void **)((int)pvVar5 + 4) = pvVar5;
    puVar4[2] = pvVar5;
    *(undefined1 *)(puVar4 + 4) = 1;
    puVar4[5] = 0;
    puVar4[6] = 0;
    puVar4[7] = 0;
  }
  local_220 = DAT_01b02744;
  local_14._0_1_ = 1;
  *(undefined4 **)(param_1 + 0xd0) = puVar4;
  uStack_21c = 0;
  uStack_218 = 0;
  uStack_214 = 0;
  if (puVar4 != (undefined4 *)0x0) {
    local_5c = 0;
    local_58 = 7;
    uStack_68 = 0;
    uStack_64 = 0;
    uStack_60 = 0;
    local_6c = 0;
    local_40 = 0;
    local_14._0_1_ = 3;
    local_78 = CONCAT44(local_220,local_220);
    local_4c = DAT_01bfe468 + 8;
    uVar7 = 0;
    uVar13 = 0;
    uVar14 = 0;
    uVar15 = 0;
    local_28 = &DAT_3f7ae148;
    local_48 = 0xb;
    local_70 = &DAT_3f7ae148;
    local_54 = 1;
    local_44 = 0;
    local_50 = 5;
    uVar16 = 0;
    uVar17 = 0;
    FUN_00f19cc0(s_UNION_UNLOCK_NOTICE2_01ae38b8,0x14);
    psVar6 = (short *)FUN_015ce480(local_250,uVar7,uVar13,uVar14,uVar15,uVar16,uVar17);
    local_14._0_1_ = 4;
    if (7 < *(uint *)(psVar6 + 10)) {
      psVar6 = *(short **)psVar6;
    }
    uStack_2c = 0;
    local_28 = (undefined1 *)0x0;
    local_3c = (short ***)0x0;
    uStack_38 = 0;
    uStack_34 = 0;
    uStack_30 = 0;
    psVar11 = psVar6;
    do {
      sVar1 = *psVar11;
      psVar11 = psVar11 + 1;
    } while (sVar1 != 0);
    FUN_00f1a200(psVar6,(int)psVar11 - (int)(psVar6 + 1) >> 1);
    local_14._0_1_ = 6;
    FUN_00f1a330();
    local_201 = 0;
    ppppsVar12 = &local_3c;
    if (7 < local_28) {
      ppppsVar12 = (short ****)local_3c;
    }
    local_208 = 0x154;
    local_238 = 0;
    uStack_234 = 0;
    uStack_230 = 0;
    uStack_22c = 0;
    local_228 = 0;
    local_224 = 0;
    ppppsVar10 = ppppsVar12;
    do {
      sVar1 = *(short *)ppppsVar10;
      ppppsVar10 = (short ****)((int)ppppsVar10 + 2);
    } while (sVar1 != 0);
    FUN_00f1a200(ppppsVar12,(int)ppppsVar10 - (int)((int)ppppsVar12 + 2) >> 1);
    local_14._0_1_ = 7;
    FUN_0114d430(&local_78,&local_208,&local_201);
    FUN_00f1a330();
    FUN_00f1a330();
    local_14._0_1_ = 1;
    local_4c = 0;
    FUN_00f1a330();
  }
  FUN_01500f80(&local_90,&DAT_01ae094c,0);
  FUN_01246720(s_Mail_Mail_Item_Icon_tga_01ad588c,1,0x108,0x10a,0x20,0x20);
  FUN_01246720(s_Union_Unlock_bg_tga_01ae3980,1,0x29,0x3a,0x66,0x80);
  local_1fc = (undefined4 *)FUN_016830c0(0xa0);
  local_14._0_1_ = 8;
  if (local_1fc == (undefined4 *)0x0) {
    uVar7 = 0;
  }
  else {
    memset(local_1fc,0,0xa0);
    uVar7 = FUN_0123de10();
  }
  local_14._0_1_ = 1;
  *(undefined4 *)(param_1 + 0xdc) = uVar7;
  FUN_0123e0d0(*(undefined4 *)(param_1 + 0x40),&DAT_01abd9ec,0,(int)DAT_01bb9b2c,
               (int)((ulonglong)DAT_01bb9b2c >> 0x20),DAT_01bb9b34,0,0x42,0x52,0x30,0x2d);
  (**(code **)(**(int **)(param_1 + 0xdc) + 0x1c))(0);
  FUN_012472c0(*(undefined4 *)(param_1 + 0xdc));
  FUN_01246720(s_Union_Unlock_frame_tga_01ae39a8,1,0x1b,0x32,0x7d,0x96);
  FUN_01246720(s_Encyclopedia_NewEncyclopedia_rat_01addbe4,1,0x2b,0x87,0x5f,0x16);
  puVar4 = (undefined4 *)FUN_016830c0(0xbc);
  local_14._0_1_ = 9;
  local_1fc = puVar4;
  if (puVar4 == (undefined4 *)0x0) {
    puVar4 = (undefined4 *)0x0;
  }
  else {
    memset(puVar4,0,0xbc);
    FUN_0123de10();
    *puVar4 = &DAT_01ae721c;
    *(undefined2 *)(puVar4 + 0x28) = 0;
    *(undefined1 *)((int)puVar4 + 0xa2) = 0;
    puVar4[0x29] = 1;
    puVar4[0x2a] = 1;
    puVar4[0x2b] = 0xffffffff;
    puVar4[0x2c] = 0x3f800000;
    puVar4[0x2d] = 0x3f800000;
    puVar4[0x2e] = 0;
  }
  local_14._0_1_ = 1;
  *(undefined4 **)(param_1 + 0xd8) = puVar4;
  if (puVar4 != (undefined4 *)0x0) {
    FUN_0123d7c0();
    FUN_0123d9a0(0x66,0x66);
    if (*(int **)(param_1 + 0xd8) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0xd8) + 0x1c))(0);
    }
  }
  FUN_012472c0(*(undefined4 *)(param_1 + 0xd8));
  local_128 = 0;
  local_124 = 7;
  uStack_134 = 0;
  uStack_130 = 0;
  uStack_12c = 0;
  local_138 = 0;
  local_10c = 0;
  local_14._0_1_ = 10;
  local_118 = DAT_01bfe468 + 8;
  local_144 = CONCAT44(local_220,local_220);
  local_114 = 0xb;
  local_120 = 1;
  local_110 = 0;
  local_11c = 0;
  local_28 = &DAT_3f7ae148;
  local_13c = &DAT_3f7ae148;
  uVar7 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar15 = 0;
  uVar16 = 0;
  uVar17 = 0;
  FUN_00f19cc0();
  psVar6 = (short *)FUN_015ce480(local_250,uVar7,uVar13,uVar14,uVar15,uVar16,uVar17);
  local_14._0_1_ = 0xb;
  if (7 < *(uint *)(psVar6 + 10)) {
    psVar6 = *(short **)psVar6;
  }
  psVar11 = psVar6;
  do {
    sVar1 = *psVar11;
    psVar11 = psVar11 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar6,(int)psVar11 - (int)(psVar6 + 1) >> 1);
  local_14._0_1_ = 10;
  FUN_00f1a330();
  local_11c = 5;
  FUN_01246bb0(&local_144,1,0xd2,100);
  local_ec = 0;
  local_e8 = 7;
  uStack_f8 = 0;
  uStack_f4 = 0;
  uStack_f0 = 0;
  local_fc = 0;
  local_d0 = 0;
  local_14._0_1_ = 0xc;
  local_dc = DAT_01bfe468 + 8;
  local_108 = CONCAT44(local_220,local_220);
  local_d8 = 0xb;
  local_e4 = 1;
  local_d4 = 0;
  local_e0 = 0;
  local_28 = &DAT_3f7ae148;
  local_100 = &DAT_3f7ae148;
  uVar7 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar15 = 0;
  uVar16 = 0;
  uVar17 = 0;
  FUN_00f19cc0();
  psVar6 = (short *)FUN_015ce480(local_250,uVar7,uVar13,uVar14,uVar15,uVar16,uVar17);
  local_14._0_1_ = 0xd;
  if (7 < *(uint *)(psVar6 + 10)) {
    psVar6 = *(short **)psVar6;
  }
  psVar11 = psVar6;
  do {
    sVar1 = *psVar11;
    psVar11 = psVar11 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar6,(int)psVar11 - (int)(psVar6 + 1) >> 1);
  local_14._0_1_ = 0xc;
  FUN_00f1a330();
  local_e0 = 5;
  FUN_01246bb0(&local_108,1,0xd2,0xb6);
  local_b0 = 0;
  local_ac = 7;
  uStack_bc = 0;
  uStack_b8 = 0;
  uStack_b4 = 0;
  local_c0 = 0;
  local_94 = 0;
  local_14._0_1_ = 0xe;
  local_a0 = DAT_01bfe468 + 8;
  local_cc = CONCAT44(local_220,local_220);
  local_9c = 0xb;
  local_a8 = 1;
  local_98 = 0;
  local_a4 = 0;
  local_28 = &DAT_3f7ae148;
  local_c4 = &DAT_3f7ae148;
  uVar7 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar15 = 0;
  uVar16 = 0;
  uVar17 = 0;
  FUN_00f19cc0();
  psVar6 = (short *)FUN_015ce480(local_250,uVar7,uVar13,uVar14,uVar15,uVar16,uVar17);
  local_14._0_1_ = 0xf;
  if (7 < *(uint *)(psVar6 + 10)) {
    psVar6 = *(short **)psVar6;
  }
  psVar11 = psVar6;
  do {
    sVar1 = *psVar11;
    psVar11 = psVar11 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar6,(int)psVar11 - (int)(psVar6 + 1) >> 1);
  local_14._0_1_ = 0xe;
  FUN_00f1a330();
  local_a4 = 5;
  FUN_01246bb0(&local_cc,1,0xdc,0x11c);
  local_1fc = (undefined4 *)FUN_016830c0(0xfc);
  local_14._0_1_ = 0x10;
  if (local_1fc == (undefined4 *)0x0) {
    piVar8 = (int *)0x0;
  }
  else {
    memset(local_1fc,0,0xfc);
    piVar8 = (int *)FUN_01250410();
  }
  *(int **)(param_1 + 0xec) = piVar8;
  local_1dc = 0;
  uStack_1e8 = 0;
  uStack_1e4 = 0;
  uStack_1e0 = 0;
  local_1d8 = 7;
  local_1ec = 0;
  local_1c0 = 0;
  local_14._0_1_ = 0x11;
  local_1cc = DAT_01bfe468 + 8;
  local_1f8 = CONCAT44(local_220,local_220);
  local_1f0 = &DAT_3f7ae148;
  local_28 = &DAT_3f7ae148;
  local_1c8 = 9;
  local_1d4 = 1;
  local_1c4 = 0;
  local_1d0 = 5;
  (**(code **)(*piVar8 + 4))(local_220,local_220,&DAT_3f7ae148);
  uVar7 = FUN_01246bb0(&local_1f8,1,0x5a,0x91);
  *(undefined4 *)(param_1 + 0xec) = uVar7;
  local_1fc = (undefined4 *)FUN_016830c0(0xfc);
  local_14._0_1_ = 0x12;
  if (local_1fc == (undefined4 *)0x0) {
    piVar8 = (int *)0x0;
  }
  else {
    memset(local_1fc,0,0xfc);
    piVar8 = (int *)FUN_01250410();
  }
  *(int **)(param_1 + 0xf0) = piVar8;
  local_1a0 = 0;
  uStack_1ac = 0;
  uStack_1a8 = 0;
  uStack_1a4 = 0;
  local_19c = 7;
  local_1b0 = 0;
  local_184 = 0;
  local_14._0_1_ = 0x13;
  local_190 = DAT_01bfe468 + 8;
  local_1bc = CONCAT44(local_220,local_220);
  local_1b4 = &DAT_3f7ae148;
  local_28 = &DAT_3f7ae148;
  local_18c = 10;
  local_198 = 1;
  local_188 = 0;
  local_194 = 4;
  (**(code **)(*piVar8 + 4))(local_220,local_220,&DAT_3f7ae148);
  uVar7 = FUN_01246bb0(&local_1bc,1,0x1c7,0x4b);
  *(undefined4 *)(param_1 + 0xf0) = uVar7;
  local_1fc = (undefined4 *)FUN_016830c0(0xfc);
  local_14._0_1_ = 0x14;
  if (local_1fc == (undefined4 *)0x0) {
    piVar8 = (int *)0x0;
  }
  else {
    memset(local_1fc,0,0xfc);
    piVar8 = (int *)FUN_01250410();
  }
  *(int **)(param_1 + 0xf4) = piVar8;
  local_164 = 0;
  uStack_170 = 0;
  uStack_16c = 0;
  uStack_168 = 0;
  local_160 = 7;
  local_174 = 0;
  local_148 = 0;
  local_14._0_1_ = 0x15;
  local_154 = DAT_01bfe468 + 8;
  local_180 = CONCAT44(local_220,local_220);
  local_178 = &DAT_3f7ae148;
  local_28 = &DAT_3f7ae148;
  local_150 = 10;
  local_15c = 1;
  local_14c = 0;
  local_158 = 4;
  (**(code **)(*piVar8 + 4))(CONCAT44(local_220,local_220),&DAT_3f7ae148);
  uVar7 = FUN_01246bb0(&local_180,1,0x1c7,0x9d);
  *(undefined4 *)(param_1 + 0xf4) = uVar7;
  local_1fc = (undefined4 *)FUN_016830c0(0xfc);
  local_14._0_1_ = 0x16;
  if (local_1fc == (undefined4 *)0x0) {
    piVar8 = (int *)0x0;
  }
  else {
    memset(local_1fc,0,0xfc);
    piVar8 = (int *)FUN_01250410();
  }
  *(int **)(param_1 + 0xf8) = piVar8;
  local_5c = 0;
  uStack_68 = 0;
  uStack_64 = 0;
  uStack_60 = 0;
  local_58 = 7;
  local_6c = 0;
  local_40 = 0;
  local_14 = CONCAT31(local_14._1_3_,0x17);
  local_4c = DAT_01bfe468 + 8;
  local_70 = &DAT_3f7ae148;
  local_78 = CONCAT44(local_220,local_220);
  local_28 = &DAT_3f7ae148;
  local_48 = 10;
  local_54 = 1;
  local_44 = 0;
  local_50 = 5;
  (**(code **)(*piVar8 + 4))(CONCAT44(local_220,local_220),&DAT_3f7ae148);
  uVar7 = FUN_01246bb0(&local_78,1,0x15e,0x11c);
  *(undefined4 *)(param_1 + 0xf8) = uVar7;
  iVar9 = FUN_012468c0(extraout_ECX,0x108,0x10a,0x20,0x20);
  *(int *)(param_1 + 0xd4) = iVar9;
  if (iVar9 != 0) {
    local_200 = 0x20;
    local_1fc = (undefined4 *)0x20;
    FUN_0124d5e0(&local_200,&local_200);
    local_208 = *(int *)(param_1 + 0xd4);
    puVar4 = operator_new(8);
    *puVar4 = &DAT_01ae39e8;
    puVar4[1] = 0;
    local_1fc = puVar4;
    if (*(undefined4 **)(local_208 + 0x34) != (undefined4 *)0x0) {
      (**(code **)**(undefined4 **)(local_208 + 0x34))(1);
    }
    iVar9 = DAT_01bfe640;
    *(undefined4 **)(local_208 + 0x34) = puVar4;
    if (iVar9 != 0) {
      iVar9 = *(int *)(param_1 + 0xd4);
      local_208 = 0;
      local_1fc = (undefined4 *)0x0;
      uVar7 = FUN_0123a800(local_28c,&local_1fc,&local_208);
      local_14._0_1_ = 0x18;
      if (*(int *)(iVar9 + 0x5c) != 0) {
        cVar2 = FUN_0123ff00(uVar7);
        if (cVar2 != '\0') {
          (**(code **)(**(int **)(iVar9 + 0x5c) + 0x1c))(1);
        }
      }
      local_14 = CONCAT31(local_14._1_3_,0x17);
      FUN_00f1a420();
      iVar9 = *(int *)(*(int *)(param_1 + 0xd4) + 0x34);
      if (iVar9 != 0) {
        *(undefined4 *)(iVar9 + 4) = 0;
      }
    }
  }
  FUN_011e4890();
  FUN_011e4700();
  FUN_011e44b0();
  local_4c = 0;
  FUN_00f1a330();
  local_154 = 0;
  FUN_00f1a330();
  local_190 = 0;
  FUN_00f1a330();
  local_1cc = 0;
  FUN_00f1a330();
  local_a0 = 0;
  FUN_00f1a330();
  local_dc = 0;
  FUN_00f1a330();
  local_118 = 0;
  FUN_00f1a330();
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


