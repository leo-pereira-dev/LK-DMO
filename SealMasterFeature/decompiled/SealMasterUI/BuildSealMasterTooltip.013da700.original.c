
/* ============================================================ */
/* Function: FUN_013da700 */
/* Entry: 013da700 */
/* Signature: undefined FUN_013da700() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_013da700(int param_1,char param_2)

{
  int iVar1;
  wchar_t *pwVar2;
  char cVar3;
  undefined1 uVar4;
  short sVar5;
  uint uVar6;
  int iVar7;
  undefined8 *puVar8;
  undefined4 uVar9;
  undefined8 *puVar10;
  int *piVar11;
  int iVar12;
  undefined8 *puVar13;
  undefined4 *puVar14;
  int *piVar15;
  short *psVar16;
  uint *puVar17;
  uint *puVar18;
  undefined8 uVar19;
  char *pcVar20;
  undefined4 in_stack_fffffbec;
  undefined4 uVar21;
  undefined4 uVar22;
  undefined1 *puVar23;
  undefined4 uVar24;
  undefined4 uVar25;
  ulonglong uVar26;
  undefined4 uVar27;
  short *psVar28;
  undefined4 local_3f0;
  undefined4 local_3ec;
  undefined1 *local_3e8;
  undefined1 *puStack_3e4;
  undefined1 *local_3e0;
  undefined1 local_3d8 [24];
  undefined8 local_3c0;
  undefined1 *local_3b8;
  short *local_3b4;
  undefined8 *local_3b0;
  int local_3ac;
  undefined1 *local_3a0;
  void *local_39c;
  int local_398;
  uint *local_394;
  uint *local_390;
  uint *local_38c;
  undefined8 local_388;
  undefined4 local_380;
  undefined4 local_35c;
  undefined4 local_358;
  undefined1 local_34c [24];
  undefined1 local_334 [48];
  undefined1 local_304 [24];
  int local_2ec [2];
  undefined8 local_2e4;
  undefined8 local_2d8 [3];
  undefined8 local_2c0;
  undefined1 local_2b8 [4];
  byte local_2b4;
  ushort local_2aa;
  undefined8 *local_2a0;
  uint *local_29c;
  uint *local_298;
  undefined1 local_294 [27];
  char local_279;
  uint *local_278;
  undefined8 local_274;
  undefined1 *local_26c;
  undefined4 local_248;
  undefined4 local_244;
  undefined1 local_238 [3];
  undefined1 uStack_235;
  uint *local_234;
  int *local_230;
  uint local_22c;
  undefined1 local_228 [516];
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019b3a22;
  local_1c = ExceptionList;
  uVar6 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  local_394 = (uint *)0x0;
  if (*(int *)(param_1 + 300) == 0) {
    return;
  }
  local_3ec = *(undefined4 *)(param_1 + 0x68);
  ExceptionList = &local_1c;
  local_3ac = param_1;
  local_24 = uVar6;
  iVar7 = FUN_015db910(&local_3ec);
  if (iVar7 == 0) {
    ExceptionList = local_1c;
    return;
  }
  local_398 = iVar7;
  FUN_0124fea0(uVar6);
  local_14 = 0;
  puVar8 = (undefined8 *)FUN_00f4eeb0();
  uVar22 = 0xc;
  uVar21 = 0x13da7d4;
  FUN_0124fef0(0xc,*puVar8,*(undefined4 *)(puVar8 + 1));
  FUN_00f1a3a0();
  local_14._0_1_ = 1;
  local_230 = *(int **)(param_1 + 0x88);
  local_244 = 0xb;
  uVar27 = 0x13da815;
  FUN_00f22020();
  if (local_230 == (int *)0x0) {
    uVar21 = FUN_00f19ea0();
    FUN_01254af0(param_1 + 0x30,9999,uVar21,&local_274);
  }
  else {
    _local_238 = (short *)(iVar7 + 0xd4);
    uVar9 = FUN_00f19ea0();
    FUN_01250240(uVar9);
    FUN_01234810();
    local_2e4 = CONCAT44(0x20,(undefined4)local_2e4);
    uVar19 = CONCAT44(0x13da868,uVar27);
    FUN_013e3290(&local_2e4);
    cVar3 = FUN_00f24470();
    if (param_2 == '\0') {
      if (cVar3 == '\0') {
        uVar27 = 0x13daa9c;
        cVar3 = FUN_00f24460();
        if (cVar3 != '\0') {
          FUN_00f19d60(s_TOOLTIP_CONFLICT_ITEM_01af7ba8);
          FUN_015ce480(local_294,in_stack_fffffbec,uVar21,uVar22,uVar19,uVar27);
          local_14._0_1_ = 8;
          goto LAB_013daac7;
        }
        uVar21 = FUN_00f19ea0();
        FUN_01500f80(local_34c,&DAT_01aec624,uVar21);
        param_1 = param_1;
      }
      else {
        uVar27 = 0x13dab2f;
        cVar3 = FUN_00f24460();
        if (cVar3 == '\0') {
          uVar4 = FUN_00f24470();
          uVar21 = FUN_00f19ea0();
          FUN_01500f80(local_34c,u__s____d_01adc768,uVar21,uVar4);
        }
        else {
          FUN_00f19d60(s_TOOLTIP_CONFLICT_ITEM_01af7ba8);
          FUN_015ce480(local_294,in_stack_fffffbec,uVar21,uVar22,uVar19,uVar27);
          local_14._0_1_ = 9;
          uVar21 = FUN_00f19ea0();
          uVar4 = FUN_00f24470();
          uVar22 = FUN_00f19ea0();
          FUN_01500f80(local_34c,u__s____d___s__01af7c08,uVar22,uVar4,uVar21);
          local_14._0_1_ = 1;
          thunk_FUN_00f1a330();
          param_1 = local_3ac;
        }
      }
    }
    else if (cVar3 == '\0') {
      uVar27 = 0x13da88e;
      cVar3 = FUN_00f24460();
      if (cVar3 == '\0') {
        FUN_00f19d60(s_TOOLTIP_EQUIP_ITEM_01af7b60);
        FUN_015ce480(local_294,in_stack_fffffbec,uVar21,uVar22,uVar19,uVar27);
        local_14._0_1_ = 4;
LAB_013daac7:
        uVar21 = FUN_00f19ea0();
        uVar22 = FUN_00f19ea0();
        FUN_01500f80(local_34c,u__s___s__01af7b74,uVar22,uVar21);
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        param_1 = param_1;
      }
      else {
        FUN_00f19d60(s_TOOLTIP_EQUIP_ITEM_01af7b60);
        FUN_015ce480(local_334,in_stack_fffffbec,uVar21,uVar22,uVar19,uVar27);
        local_14._0_1_ = 2;
        uVar9 = 0x13da8c4;
        uVar27 = FUN_00f19ea0();
        FUN_00f19d60(s_TOOLTIP_CONFLICT_ITEM_01af7ba8);
        FUN_015ce480(local_294,in_stack_fffffbec,uVar21,uVar22,uVar19,uVar9);
        local_14._0_1_ = 3;
        uVar21 = FUN_00f19ea0();
        uVar22 = FUN_00f19ea0();
        FUN_01500f80(local_34c,u__s___s____s__01af7bc0,uVar22,uVar21,uVar27);
        thunk_FUN_00f1a330();
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        param_1 = local_3ac;
      }
    }
    else {
      uVar27 = 0x13da95e;
      cVar3 = FUN_00f24460();
      if (cVar3 == '\0') {
        FUN_00f19d60(s_TOOLTIP_EQUIP_ITEM_01af7b60);
        FUN_015ce480(local_294,in_stack_fffffbec,uVar21,uVar22,uVar19,uVar27);
        local_14._0_1_ = 7;
        uVar21 = FUN_00f19ea0();
        uVar4 = FUN_00f24470();
        uVar22 = FUN_00f19ea0();
        FUN_01500f80(local_34c,u__s____d___s__01af7c08,uVar22,uVar4,uVar21);
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        param_1 = local_3ac;
      }
      else {
        FUN_00f19d60(s_TOOLTIP_EQUIP_ITEM_01af7b60);
        FUN_015ce480(local_334,in_stack_fffffbec,uVar21,uVar22,uVar19,uVar27);
        local_14._0_1_ = 5;
        uVar27 = 0x13da994;
        local_38c = (uint *)FUN_00f19ea0();
        FUN_00f19d60(s_TOOLTIP_CONFLICT_ITEM_01af7ba8);
        FUN_015ce480(local_294,in_stack_fffffbec,uVar21,uVar22,uVar19,uVar27);
        local_14._0_1_ = 6;
        uVar21 = FUN_00f19ea0();
        uVar4 = FUN_00f24470();
        uVar22 = FUN_00f19ea0();
        FUN_01500f80(local_34c,u__s____d___s____s__01af7b84,uVar22,uVar4,uVar21,local_38c);
        thunk_FUN_00f1a330();
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        param_1 = local_3ac;
      }
    }
  }
  uVar21 = FUN_00f19ea0();
  FUN_01254af0(param_1 + 0x30,9999,uVar21,&local_274);
  local_38c = (uint *)0x20;
  local_278 = (uint *)0x20;
  uVar22 = FUN_00f582a0();
  puVar10 = (undefined8 *)0x4c;
  puVar8 = *(undefined8 **)(param_1 + 0x68);
  uVar21 = 0x13dac72;
  uVar27 = FUN_013c25c0(puVar8,uVar22,0x4c,&DAT_01bee88c,0);
  FUN_012338a0(uVar27);
  FUN_013c2850(0);
  local_22c = *(uint *)(param_1 + 0x88);
  if (local_22c != 0) {
    local_38c = (uint *)FUN_00f22260();
    iVar7 = FUN_015db910(&local_38c);
    if (((iVar7 != 0) && (cVar3 = FUN_00f24480(), cVar3 != '\0')) &&
       (*(char *)(local_22c + 6) != '\0')) {
      uVar19 = CONCAT44(0x13dace7,puVar10);
      local_390 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 10;
      if (local_390 == (uint *)0x0) {
        local_234 = (uint *)0x0;
      }
      else {
        uVar19 = CONCAT44(0x13dad03,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_234 = (uint *)FUN_010baa10();
      }
      local_14._0_1_ = 1;
      local_244 = 0xd;
      uVar27 = 0x13dad3e;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_COMMON_TXT_DIGITARY_POWER_01af7c24);
      FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
      local_14._0_1_ = 0xb;
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14 = CONCAT31(local_14._1_3_,1);
      thunk_FUN_00f1a330();
      local_38c = (uint *)FUN_00f79280();
      puVar18 = local_234;
      if ((int)local_38c < 100) {
        FUN_01232490(3);
      }
      FUN_00f1a3a0();
      local_14._0_1_ = 0xc;
      FUN_01500f80(local_2d8,u__d___01af7bdc,local_38c);
      uVar22 = FUN_00f19ea0();
      FUN_012502e0(uVar22);
      puVar10 = &local_274;
      uVar22 = 0x13dae1b;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
      piVar11 = (int *)FUN_01232610(&local_2e4);
      FUN_012324c0(0x118 - *piVar11);
      FUN_012338a0(puVar18);
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
    }
  }
  iVar7 = *(int *)(param_1 + 0x74);
  uVar6 = *(uint *)(param_1 + 0x70);
  if ((((-1 < iVar7) && (iVar7 < 1)) && ((0x31 < uVar6 && (iVar7 < 1)))) &&
     ((iVar7 < 0 || (uVar6 < 0xc2)))) {
                    /* WARNING: Could not recover jumptable at 0x013dae89. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(&DAT_013e0f0c + (uint)*(byte *)(uVar6 + 0x13e0ee6) * 4))();
    return;
  }
  uVar19 = CONCAT44(0x13db1b6,puVar10);
  local_390 = (uint *)FUN_016830c0(0x2c);
  local_14._0_1_ = 0x15;
  if (local_390 == (uint *)0x0) {
    _local_238 = (short *)0x0;
  }
  else {
    uVar19 = CONCAT44(0x13db1d2,(int)uVar19);
    FUN_010b9e20(0x2c);
    _local_238 = (short *)FUN_010baa10();
  }
  local_14._0_1_ = 1;
  local_244 = 9;
  uVar27 = 0x13db21c;
  puVar10 = (undefined8 *)FUN_00f4eeb0();
  local_274 = *puVar10;
  local_26c = *(undefined1 **)(puVar10 + 1);
  FUN_00f19d60(s_TOOLTIP_ITEM_KIND_01af7c40);
  FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
  local_14._0_1_ = 0x16;
  uVar22 = FUN_00f19ea0();
  FUN_01250240(uVar22);
  local_14._0_1_ = 1;
  thunk_FUN_00f1a330();
  iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
  *(undefined4 *)(iVar7 + 8) = 0x72;
  local_244 = 9;
  puVar10 = (undefined8 *)FUN_00f4eeb0();
  iVar7 = local_398;
  local_274 = *puVar10;
  local_26c = *(undefined1 **)(puVar10 + 1);
  uVar22 = FUN_00f19ea0();
  FUN_01250240(uVar22);
  puVar10 = &local_274;
  uVar22 = 0x13db315;
  FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
  FUN_012338a0(_local_238);
  _local_238 = *(short **)(param_1 + 0x88);
  if (_local_238 != (short *)0x0) {
    if ((*(int *)(param_1 + 0x70) == 0x97) && (*(int *)(param_1 + 0x74) == 0)) {
      cVar3 = FUN_00f794a0();
      if (cVar3 == '\0') {
        if (*(int *)(iVar7 + 0x80) == 0) goto LAB_013db8aa;
        uVar19 = CONCAT44(0x13db458,puVar10);
        local_390 = (uint *)FUN_016830c0(0x2c);
        local_14._0_1_ = 0x19;
        if (local_390 == (uint *)0x0) {
          local_22c = 0;
        }
        else {
          uVar19 = CONCAT44(0x13db474,(int)uVar19);
          FUN_010b9e20(0x2c);
          local_22c = FUN_010baa10();
        }
        local_14._0_1_ = 1;
        uVar27 = 0x13db4ac;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f19d60(s_TOOLTIP_ITEM_REMAINING_01af7c54);
        FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
        local_14._0_1_ = 0x1a;
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        puVar18 = (uint *)local_22c;
        iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x72;
LAB_013db859:
        uVar22 = 0;
      }
      else {
        uVar19 = CONCAT44(0x13db364,puVar10);
        local_390 = (uint *)FUN_016830c0(0x2c);
        local_14._0_1_ = 0x17;
        if (local_390 == (uint *)0x0) {
          local_22c = 0;
        }
        else {
          uVar19 = CONCAT44(0x13db380,(int)uVar19);
          FUN_010b9e20(0x2c);
          local_22c = FUN_010baa10();
        }
        local_14._0_1_ = 1;
        uVar27 = 0x13db3b8;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f19d60(s_TOOLTIP_ITEM_REMAINING_01af7c54);
        FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
        local_14._0_1_ = 0x18;
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        puVar18 = (uint *)local_22c;
        iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        uVar22 = 1;
        *(undefined4 *)(iVar7 + 8) = 0x72;
      }
      uVar22 = FUN_013ce610(uVar22);
      FUN_01250240(uVar22);
      puVar10 = &local_274;
      uVar22 = 0x13db8a1;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    }
    else {
      cVar3 = FUN_00f794a0();
      if (cVar3 == '\0') {
        if (*(int *)(iVar7 + 0x80) != 0) {
          uVar19 = CONCAT44(0x13db74f,puVar10);
          local_390 = (uint *)FUN_016830c0(0x2c);
          local_14._0_1_ = 0x1e;
          if (local_390 == (uint *)0x0) {
            local_22c = 0;
          }
          else {
            uVar19 = CONCAT44(0x13db76b,(int)uVar19);
            FUN_010b9e20(0x2c);
            local_22c = FUN_010baa10();
          }
          local_14._0_1_ = 1;
          uVar27 = 0x13db7a3;
          puVar10 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          FUN_00f19d60(s_TOOLTIP_ITEM_REMAINING_01af7c54);
          FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
          local_14._0_1_ = 0x1f;
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          local_14._0_1_ = 1;
          thunk_FUN_00f1a330();
          puVar18 = (uint *)local_22c;
          iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
          *(undefined4 *)(iVar7 + 8) = 0x72;
          puVar10 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          goto LAB_013db859;
        }
        goto LAB_013db8aa;
      }
      uVar19 = CONCAT44(0x13db54c,puVar10);
      local_390 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x1b;
      if (local_390 == (uint *)0x0) {
        local_234 = (uint *)0x0;
      }
      else {
        uVar19 = CONCAT44(0x13db568,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_234 = (uint *)FUN_010baa10();
      }
      local_14._0_1_ = 1;
      uVar27 = 0x13db5a0;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_TOOLTIP_ITEM_REMAINING_01af7c54);
      FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
      uVar9 = (undefined4)uVar19;
      local_14._0_1_ = 0x1c;
      uVar27 = FUN_00f19ea0();
      uVar19 = CONCAT44(0x13db5ef,uVar9);
      FUN_01250240(uVar27);
      local_14._0_1_ = 1;
      uVar27 = 0x13db5fe;
      thunk_FUN_00f1a330();
      puVar18 = local_234;
      if (*(char *)(local_398 + 0x88) == '\x01') {
        if (*(int *)((int)_local_238 + 0x3d) == -0x10) {
          FUN_00f19d60(s_TOOLTIP_UNUSED_01af7ce4);
          FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
          local_14._0_1_ = 0x1d;
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          local_14._0_1_ = 1;
          thunk_FUN_00f1a330();
        }
        puVar18 = local_234;
        puVar10 = &local_274;
        uVar22 = 0x13db686;
        iVar7 = FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x72;
        if (*(int *)((int)_local_238 + 0x3d) == -0x10) goto LAB_013db8a1;
      }
      else {
        iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x72;
      }
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_01250240(&DAT_01ac59a8);
      uVar22 = 0x13db721;
      FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      puVar10 = (undefined8 *)0x13db736;
      FUN_013e2200();
    }
LAB_013db8a1:
    FUN_012338a0(puVar18);
  }
LAB_013db8aa:
  uVar19 = CONCAT44(0x13db8b1,puVar10);
  local_390 = (uint *)FUN_016830c0(0x2c);
  local_14._0_1_ = 0x20;
  if (local_390 == (uint *)0x0) {
    _local_238 = (short *)0x0;
  }
  else {
    uVar19 = CONCAT44(0x13db8cd,(int)uVar19);
    FUN_010b9e20(0x2c);
    _local_238 = (short *)FUN_010baa10();
  }
  local_14._0_1_ = 1;
  local_244 = 9;
  uVar27 = 0x13db917;
  puVar10 = (undefined8 *)FUN_00f4eeb0();
  local_274 = *puVar10;
  local_26c = *(undefined1 **)(puVar10 + 1);
  FUN_00f19d60(s_TOOLTIP_ITEM_ITEM_CLASS_01af7cf4);
  FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
  local_14._0_1_ = 0x21;
  uVar22 = FUN_00f19ea0();
  FUN_01250240(uVar22);
  local_14._0_1_ = 1;
  thunk_FUN_00f1a330();
  uVar19 = CONCAT44(DAT_01bee88c,&local_274);
  uVar22 = 0x13db993;
  iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
  *(undefined4 *)(iVar7 + 8) = 0x72;
  local_244 = 9;
  uVar27 = 0x13db9c0;
  puVar10 = (undefined8 *)FUN_00f4eeb0();
  iVar7 = local_398;
  local_274 = *puVar10;
  local_26c = *(undefined1 **)(puVar10 + 1);
  switch(*(undefined2 *)(local_398 + 0x16)) {
  case 1:
    FUN_00f19d60(s_TOOLTIP_ITEM_MISC_ITEM_01af7cb0);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x22;
    break;
  case 2:
    FUN_00f19d60(s_TOOLTIP_ITEM_NORMAL_ITEM_01af7cc8);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x23;
    break;
  case 3:
    FUN_00f19d60(s_TOOLTIP_ITEM_MAGIC_ITEM_01af7d40);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x24;
    break;
  case 4:
    FUN_00f19d60(s_TOOLTIP_ITEM_RARE_ITEM_01af7d58);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x25;
    break;
  case 5:
    FUN_00f19d60(s_TOOLTIP_ITEM_EPIC_ITEM_01af7d0c);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x26;
    break;
  case 6:
    FUN_00f19d60(s_TOOLTIP_ITEM_UNIQUE_ITEM_01af7d24);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x27;
    break;
  case 7:
    FUN_00f19d60(s_TOOLTIP_ITEM_LEGEND_ITEM_01af7da0);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x28;
    break;
  case 8:
    FUN_00f19d60(s_COMMON_TXT_EVENT_ITEM_01af7dbc);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x29;
    break;
  default:
    goto switchD_013db9e9_caseD_9;
  case 10:
    FUN_00f19d60(s_TOOLTIP_ITEM_CASH_ITEM_01af7d70);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x2a;
    break;
  case 0xb:
    FUN_01250240(u_Admin_Item_01af7d88);
    goto switchD_013db9e9_caseD_9;
  case 0xc:
    FUN_01250240(&DAT_01af7e04);
    goto switchD_013db9e9_caseD_9;
  case 0xd:
    FUN_00f19d60(s_TOOLTIP_ITEM_CASH_ITEM_SECOND_01af7e18);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x2b;
  }
  uVar22 = FUN_00f19ea0();
  FUN_01250240(uVar22);
  local_14._0_1_ = 1;
  thunk_FUN_00f1a330();
  iVar7 = local_398;
switchD_013db9e9_caseD_9:
  puVar10 = &local_274;
  piVar11 = (int *)0x13dbc07;
  FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
  FUN_012338a0(_local_238);
  if ((*(short *)(iVar7 + 0x18) == 0x3d) || (*(short *)(iVar7 + 0x18) == 0x48)) {
    uVar19 = CONCAT44(0x13dbc2e,puVar10);
    local_390 = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x2c;
    if (local_390 == (uint *)0x0) {
      _local_238 = (short *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13dbc4a,(int)uVar19);
      FUN_010b9e20(0x2c);
      _local_238 = (short *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar22 = 0x13dbc94;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_TARGET_01af7dd4);
    FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x2d;
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    uVar19 = CONCAT44(DAT_01bee88c,&local_274);
    uVar22 = 0x13dbd10;
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    local_244 = 9;
    uVar27 = 0x13dbd3d;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    switch(*(undefined2 *)(local_398 + 0x40)) {
    case 0:
    case 1:
      FUN_00f19d60(s_TOOLTIP_ITEM_APPLY_TO_ALL_01af7de8);
      FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
      local_14._0_1_ = 0x2e;
      break;
    case 2:
      FUN_00f19d60(s_COMMON_TXT_DIGIMON_01ad9b10);
      FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
      local_14._0_1_ = 0x2f;
      break;
    case 3:
      FUN_00f19d60(s_COMMON_TXT_TAMER_01ae3064);
      FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
      local_14._0_1_ = 0x30;
      break;
    default:
      goto switchD_013dbd65_default;
    }
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
switchD_013dbd65_default:
    iVar7 = local_398;
    puVar10 = &local_274;
    piVar11 = (int *)0x13dbe2b;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(_local_238);
  }
  switch(*(undefined2 *)(iVar7 + 0x18)) {
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1a:
  case 0x1b:
  case 0x1c:
  case 0x1d:
  case 0x1e:
  case 0x1f:
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
    cVar3 = FUN_00f79970();
    if (cVar3 == '\0') goto switchD_013dbe4c_caseD_20;
    break;
  default:
switchD_013dbe4c_caseD_20:
    cVar3 = FUN_00f23a90();
    if ((((cVar3 != '\0') || (*(int *)(param_1 + 0x144) == 0)) ||
        (iVar12 = FUN_015d17e0(iVar7 + 0x10), iVar12 == 0)) ||
       ((*(short *)(iVar12 + 0x9c) == 0 || (*(short *)(iVar12 + 0x9c) == 0x70)))) {
      puVar10 = (undefined8 *)(local_238 + 2);
      piVar11 = (int *)0x13dbeae;
      FUN_013e1a50(puVar10,iVar7,&local_274);
    }
    break;
  case 0x34:
  case 0x65:
    break;
  }
  if ((*(int *)(param_1 + 0x168) != 0) && (local_230 != (int *)0x0)) {
    FUN_01274fc0(8);
    uVar22 = FUN_00f22260();
    puVar10 = &local_2c0;
    piVar11 = (int *)0x13dbef7;
    FUN_015ec940(puVar10,uVar22,1);
    local_14._0_1_ = 0x31;
    cVar3 = FUN_013e21e0();
    if (cVar3 == '\0') {
      uVar19 = CONCAT44(0x13dbf15,puVar10);
      local_390 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x32;
      if (local_390 == (uint *)0x0) {
        local_234 = (uint *)0x0;
      }
      else {
        uVar19 = CONCAT44(0x13dbf31,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_234 = (uint *)FUN_010baa10();
      }
      local_14._0_1_ = 0x31;
      local_244 = 9;
      uVar22 = 0x13dbf7b;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_EQUIPSYSTEM_UPGRADE_GRADE_01ae8af0);
      FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
      local_14._0_1_ = 0x33;
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14._0_1_ = 0x31;
      thunk_FUN_00f1a330();
      uVar19 = CONCAT44(DAT_01bee88c,&local_274);
      uVar27 = 0x13dbff7;
      uVar22 = DAT_01bee890;
      iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      *(undefined4 *)(iVar7 + 8) = 0x72;
      FUN_00f19d60(s_TOOLTIP_EQUIPSYSTEM_UPGRADE_GRAD_01af7e74);
      FUN_015ce480(local_294,uVar21,puVar8,uVar27,uVar19,uVar22);
      local_14._0_1_ = 0x34;
      uVar22 = FUN_00f19ea0();
      FUN_00f1a2b0(uVar22);
      local_14._0_1_ = 0x36;
      thunk_FUN_00f1a330();
      uVar6 = FUN_00f24470();
      local_38c = (uint *)(uVar6 & 0xff);
      FUN_00f1a2b0(u__value1__01ad2720);
      local_14._0_1_ = 0x37;
      uVar19 = CONCAT44(local_2d8,local_2b8);
      uVar22 = 0x13dc089;
      FUN_01501660(local_2b8,local_2d8,&local_38c);
      local_14._0_1_ = 0x36;
      thunk_FUN_00f1a330();
      cVar3 = FUN_00f24460();
      if (cVar3 == '\0') {
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
      }
      else {
        uVar27 = 0x13dc0b9;
        FUN_00f1a3a0();
        local_14._0_1_ = 0x38;
        FUN_00f19d60(s_TOOLTIP_CONFLICT_ITEM_01af7ba8);
        FUN_015ce480(local_334,uVar21,puVar8,uVar22,uVar19,uVar27);
        local_14._0_1_ = 0x39;
        uVar22 = FUN_00f19ea0();
        uVar27 = FUN_00f19ea0();
        puVar8 = (undefined8 *)0x13dc10b;
        FUN_01500f80(local_2d8,u__s___s__01af7b74,uVar27,uVar22);
        local_14._0_1_ = 0x38;
        thunk_FUN_00f1a330();
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        local_14._0_1_ = 0x36;
        thunk_FUN_00f1a330();
      }
      puVar18 = local_234;
      puVar10 = &local_274;
      piVar11 = (int *)0x13dc1ad;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(puVar18);
      thunk_FUN_00f1a330();
    }
    local_14._0_1_ = 1;
    FUN_010b2530();
    iVar7 = local_398;
  }
  if (*(int *)(iVar7 + 0x10) != 0) {
    uVar19 = CONCAT44(0x13dc1e7,puVar10);
    local_38c = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x3a;
    if (local_38c == (uint *)0x0) {
      local_230 = (int *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13dc201,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_230 = (int *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    sVar5 = *(short *)(iVar7 + 0x86);
    if (sVar5 == 0) {
      uVar22 = 0x13dc6b8;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_TOOLTIP_ITEM_LIMIT_OBTAIN_01af7e98);
      FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
      local_14._0_1_ = 0x3b;
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
      uVar19 = CONCAT44(DAT_01bee88c,&local_274);
      uVar22 = 0x13dc734;
      iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      *(undefined4 *)(iVar7 + 8) = 0x72;
      uVar27 = 0x13dc757;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_TOOLTIP_ITEM_NO_LIMIT_OBTAIN_01af7e38);
      FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
      local_14 = CONCAT31(local_14._1_3_,0x3c);
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
LAB_013dc7ac:
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
      iVar7 = local_398;
    }
    else {
      if (sVar5 == 1) {
        uVar22 = 0x13dc492;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f19d60(s_TOOLTIP_ITEM_LIMIT_OBTAIN_01af7e98);
        FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
        local_14._0_1_ = 0x3d;
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x72;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f1a3a0();
        local_14 = CONCAT31(local_14._1_3_,0x3e);
        if (*(int *)(param_1 + 0x148) != 0) {
          uVar6 = FUN_00f22980();
          local_38c = (uint *)(uVar6 & 0xff);
          uVar22 = FUN_015e5a20();
          FUN_00f19c80(uVar22);
          thunk_FUN_00f1a330();
          cVar3 = FUN_00f22190();
          if (cVar3 == '\0') {
            FUN_00f533f0(&DAT_01af3600);
          }
          uVar6 = FUN_00f22980();
          local_38c = (uint *)(uVar6 & 0xff);
          uVar22 = FUN_015e5a20();
          local_14._0_1_ = 0x3f;
          FUN_00fa8420(uVar22);
          local_14 = CONCAT31(local_14._1_3_,0x3e);
          thunk_FUN_00f1a330();
          cVar3 = FUN_00f22190();
          if (cVar3 == '\0') {
            FUN_00f533f0(&DAT_01af3600);
          }
          uVar6 = FUN_00f22980();
          local_38c = (uint *)(uVar6 & 0xff);
          uVar22 = FUN_015e5a20();
          local_14._0_1_ = 0x40;
          FUN_00fa8420(uVar22);
          local_14 = CONCAT31(local_14._1_3_,0x3e);
          thunk_FUN_00f1a330();
        }
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        goto LAB_013dc7ac;
      }
      if (sVar5 == 2) {
        uVar22 = 0x13dc271;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f19d60(s_TOOLTIP_ITEM_UNLIMIT_OBTAIN_01af7e58);
        FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
        local_14._0_1_ = 0x41;
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x72;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f1a3a0();
        local_14 = CONCAT31(local_14._1_3_,0x42);
        if (*(int *)(param_1 + 0x148) != 0) {
          uVar6 = FUN_00f22980();
          local_38c = (uint *)(uVar6 & 0xff);
          uVar22 = FUN_015e5a20();
          FUN_00f19c80(uVar22);
          thunk_FUN_00f1a330();
          cVar3 = FUN_00f22190();
          if (cVar3 == '\0') {
            FUN_00f533f0(&DAT_01af3600);
          }
          uVar6 = FUN_00f22980();
          local_38c = (uint *)(uVar6 & 0xff);
          uVar22 = FUN_015e5a20();
          local_14._0_1_ = 0x43;
          FUN_00fa8420(uVar22);
          local_14 = CONCAT31(local_14._1_3_,0x42);
          thunk_FUN_00f1a330();
          cVar3 = FUN_00f22190();
          if (cVar3 == '\0') {
            FUN_00f533f0(&DAT_01af3600);
          }
          uVar6 = FUN_00f22980();
          local_38c = (uint *)(uVar6 & 0xff);
          uVar22 = FUN_015e5a20();
          local_14._0_1_ = 0x44;
          FUN_00fa8420(uVar22);
          local_14 = CONCAT31(local_14._1_3_,0x42);
          thunk_FUN_00f1a330();
        }
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        goto LAB_013dc7ac;
      }
    }
    puVar10 = &local_274;
    piVar11 = (int *)0x13dc7d9;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(local_230);
  }
  if (*(int *)(iVar7 + 0x78) != 0) {
    FUN_00f22250();
    local_279 = FUN_00f6c6f0(*(undefined4 *)(iVar7 + 0x78));
    if (*(int *)(param_1 + 0x130) == 0) {
      uVar22 = FUN_00f1a2b0(&DAT_01ac5dd8);
      local_230 = (int *)0x2;
    }
    else {
      puVar10 = (undefined8 *)0x13dc826;
      uVar22 = FUN_015ef010();
      local_230 = (int *)0x1;
    }
    FUN_00f19bd0(uVar22);
    local_14 = 0x46;
    if (((uint)local_230 & 2) != 0) {
      local_230 = (int *)((uint)local_230 & 0xfffffffd);
      thunk_FUN_00f1a330();
    }
    local_14._0_1_ = 0x47;
    if (((uint)local_230 & 1) != 0) {
      thunk_FUN_00f1a330();
    }
    cVar3 = FUN_00f22190();
    if (cVar3 == '\0') {
      uVar19 = CONCAT44(0x13dc8ac,puVar10);
      local_394 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x48;
      if (local_394 == (uint *)0x0) {
        local_22c = 0;
      }
      else {
        uVar19 = CONCAT44(0x13dc8c8,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_22c = FUN_010baa10();
      }
      local_14._0_1_ = 0x47;
      local_244 = 9;
      uVar22 = 0x13dc90a;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_COMMON_TXT_REQUIRE_QUEST_01af79e0);
      FUN_015ce480(local_304,uVar21,puVar8,piVar11,uVar19,uVar22);
      local_14._0_1_ = 0x49;
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14._0_1_ = 0x47;
      thunk_FUN_00f1a330();
      uVar6 = local_22c;
      iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      *(undefined4 *)(iVar7 + 8) = 0x72;
      local_244 = 9;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      puVar10 = &local_274;
      piVar11 = (int *)0x13dca1f;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(uVar6);
      iVar7 = local_398;
    }
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
  }
  local_234 = *(uint **)(param_1 + 0x88);
  if (*(short *)(iVar7 + 0x18) == 0x58) {
    if (local_234 != (uint *)0x0) {
      uVar19 = CONCAT44(0x13dcdc7,puVar10);
      local_394 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x4a;
      if (local_394 == (uint *)0x0) {
        local_298 = (uint *)0x0;
      }
      else {
        uVar19 = CONCAT44(0x13dcde3,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_298 = (uint *)FUN_010baa10();
      }
      local_14._0_1_ = 1;
      local_244 = 9;
      uVar22 = 0x13dce2d;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_TOOLTIP_ITEM_REWARDED_MONEY_01af7ed4);
      FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
      local_14._0_1_ = 0x4b;
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
      uVar19 = CONCAT44(DAT_01bee88c,&local_274);
      uVar22 = 0x13dcea9;
      iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      *(undefined4 *)(iVar7 + 8) = 0x72;
      local_244 = 9;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      uVar27 = 0x13dcef6;
      FUN_00f1a3a0();
      local_14._0_1_ = 0x4c;
      FUN_00f19d60(s_TOOLTIP_ITEM_MEGA_01af7ef0);
      FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
      local_14._0_1_ = 0x4d;
      uVar22 = FUN_00f19ea0();
      sVar5 = FUN_00f24450();
      puVar8 = (undefined8 *)0x13dcf4b;
      FUN_01500f80(local_2d8,u__d__s_01ad05f8,(int)sVar5,uVar22);
      local_14._0_1_ = 0x4c;
      thunk_FUN_00f1a330();
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      puVar18 = local_298;
      puVar10 = &local_274;
      piVar11 = (int *)0x13dcf94;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(puVar18);
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
      iVar7 = local_398;
    }
  }
  else if (*(short *)(iVar7 + 0x18) == 0xac) {
    if (local_234 != (uint *)0x0) {
      uVar19 = CONCAT44(0x13dcbe6,puVar10);
      local_394 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x4e;
      if (local_394 == (uint *)0x0) {
        local_230 = (int *)0x0;
      }
      else {
        uVar19 = CONCAT44(0x13dcc02,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_230 = (int *)FUN_010baa10();
      }
      local_14._0_1_ = 1;
      local_244 = 9;
      uVar22 = 0x13dcc4c;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_TOOLTIP_ITEM_REWARDED_MONEY_01af7ed4);
      FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
      local_14._0_1_ = 0x4f;
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
      iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      *(undefined4 *)(iVar7 + 8) = 0x72;
      local_244 = 9;
      puVar8 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar8;
      local_26c = *(undefined1 **)(puVar8 + 1);
      sVar5 = FUN_00f24450();
      if (sVar5 < 1000) {
        sVar5 = FUN_00f24450();
        uVar6 = *(int *)(local_398 + 0x50) * (int)sVar5;
        pwVar2 = u__d_Bit_01af7eb4;
      }
      else {
        sVar5 = FUN_00f24450();
        uVar6 = (uint)(*(int *)(local_398 + 0x50) * (int)sVar5) / 1000;
        pwVar2 = u__d_Mega_01af7ec4;
      }
      iVar7 = local_398;
      puVar8 = (undefined8 *)0x13dcd72;
      FUN_00f222a0(local_228,0x100,pwVar2,uVar6);
      FUN_01250240(local_228);
      puVar10 = &local_274;
      piVar11 = (int *)0x13dcda5;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(local_230);
    }
  }
  else if ((local_234 != (uint *)0x0) && (1 < *(ushort *)(iVar7 + 0x2a))) {
    uVar19 = CONCAT44(0x13dca79,puVar10);
    local_394 = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x50;
    if (local_394 == (uint *)0x0) {
      local_22c = 0;
    }
    else {
      uVar19 = CONCAT44(0x13dca95,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_22c = FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar22 = 0x13dcad7;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_COUNT_01af73cc);
    FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x51;
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    uVar6 = local_22c;
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    local_244 = 9;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    sVar5 = FUN_00f24450();
    FUN_01250270((int)sVar5);
    puVar10 = &local_274;
    piVar11 = (int *)0x13dcbc9;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(uVar6);
    iVar7 = local_398;
  }
  if ((*(short *)(iVar7 + 0x18) == 0x3d) || (*(short *)(iVar7 + 0x18) == 0x48)) {
    uVar19 = CONCAT44(0x13dcfcb,puVar10);
    local_394 = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x52;
    if (local_394 == (uint *)0x0) {
      local_230 = (int *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13dcfe7,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_230 = (int *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar22 = 0x13dd031;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_LIMITED_USE_01af7f54);
    FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x53;
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    uVar19 = CONCAT44(DAT_01bee88c,&local_274);
    uVar22 = 0x13dd0ad;
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    local_244 = 9;
    uVar27 = 0x13dd0da;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    cVar3 = *(char *)(local_398 + 0x26);
    if (cVar3 == '\0') {
      FUN_00f19d60(s_TOOLTIP_ITEM_USABLE_AT_ALL_TIMES_01af7f70);
      FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
      local_14._0_1_ = 0x54;
LAB_013dd185:
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
    }
    else {
      if (cVar3 == '\x01') {
        FUN_00f19d60(s_TOOLTIP_ITEM_NOT_USABLE_DURING_B_01af7f04);
        FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
        local_14._0_1_ = 0x55;
        goto LAB_013dd185;
      }
      if (cVar3 == '\x02') {
        FUN_00f19d60(s_TOOLTIP_ITEM_ONLY_USABLE_DURING__01af7f2c);
        FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
        local_14._0_1_ = 0x56;
        goto LAB_013dd185;
      }
    }
    iVar7 = local_398;
    puVar10 = &local_274;
    piVar11 = (int *)0x13dd1cb;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(local_230);
  }
  switch(*(undefined2 *)(iVar7 + 0x18)) {
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1b:
  case 0x1d:
    if (((*(char *)(iVar7 + 0xc) == '\x03') || (cVar3 = FUN_00f24480(), cVar3 != '\0')) &&
       (*(int *)(param_1 + 0x88) != 0)) {
      if (*(char *)(*(int *)(param_1 + 0x88) + 6) == '\0') {
        uVar19 = CONCAT44(0x13dd46f,puVar10);
        local_394 = (uint *)FUN_016830c0(0x2c);
        local_14._0_1_ = 0x5d;
        if (local_394 == (uint *)0x0) {
          local_22c = 0;
        }
        else {
          uVar19 = CONCAT44(0x13dd48b,(int)uVar19);
          FUN_010b9e20(0x2c);
          local_22c = FUN_010baa10();
        }
        local_14._0_1_ = 1;
        local_244 = 9;
        uVar22 = 0x13dd4d0;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f19d60(s_TOOLTIP_ITEM_OPTION_UNIDENTIFIED_01af7f94);
        FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
        local_14._0_1_ = 0x5e;
      }
      else {
        uVar19 = CONCAT44(0x13dd516,puVar10);
        local_394 = (uint *)FUN_016830c0(0x2c);
        local_14._0_1_ = 0x5f;
        if (local_394 == (uint *)0x0) {
          local_22c = 0;
        }
        else {
          uVar19 = CONCAT44(0x13dd532,(int)uVar19);
          FUN_010b9e20(0x2c);
          local_22c = FUN_010baa10();
        }
        local_14._0_1_ = 1;
        local_244 = 9;
        uVar22 = 0x13dd572;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f19d60(s_TOOLTIP_ITEM_OPTION_IDENTIFIED_01af7fb8);
        FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
        local_14._0_1_ = 0x60;
      }
      goto LAB_013dd5ae;
    }
    break;
  case 0x78:
  case 0x7a:
  case 0x7b:
  case 0x7c:
  case 0x7d:
  case 0x7f:
    iVar7 = *(int *)(param_1 + 0x74);
    uVar6 = *(uint *)(param_1 + 0x70);
    if ((((-1 < iVar7) && (iVar7 < 1)) && (0x31 < uVar6)) &&
       ((iVar7 < 1 && ((iVar7 < 0 || (uVar6 < 0xc2)))))) {
                    /* WARNING: Could not recover jumptable at 0x013dd230. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(&DAT_013e10c4 + (uint)(&BYTE_013e109a)[uVar6] * 4))();
      return;
    }
    uVar19 = CONCAT44(0x13dd399,puVar10);
    local_394 = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x5b;
    if (local_394 == (uint *)0x0) {
      local_22c = 0;
    }
    else {
      uVar19 = CONCAT44(0x13dd3b5,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_22c = FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar22 = 0x13dd3fa;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_MERGE_NOT_POSSIBLE_01af7fd8);
    FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x5c;
LAB_013dd5ae:
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    uVar6 = local_22c;
    puVar10 = &local_274;
    piVar11 = (int *)0x13dd5f0;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(uVar6);
    iVar7 = local_398;
  }
  if ((ushort)(*(short *)(iVar7 + 0x28) - 1U) < 0x22) {
    FUN_013c2850(1);
    uVar19 = CONCAT44(0x13dd61f,puVar10);
    local_394 = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x61;
    if (local_394 == (uint *)0x0) {
      local_230 = (int *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13dd63b,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_230 = (int *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar22 = 0x13dd685;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_COOL_TIME_01ade0c8);
    FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x62;
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    puVar10 = &local_274;
    piVar11 = (int *)0x13dd701;
    iVar7 = FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    puVar13 = (undefined8 *)FUN_00f4eeb0();
    iVar7 = local_398;
    local_274 = *puVar13;
    local_26c = *(undefined1 **)(puVar13 + 1);
    local_38c = (uint *)(uint)*(ushort *)(local_398 + 0x28);
    local_22c = FUN_00f1b240(&local_38c);
    if (local_22c != 0) {
      FUN_00f685a0(0);
      uVar22 = FUN_013ce610();
      FUN_01250240(uVar22);
      puVar10 = &local_274;
      piVar11 = (int *)0x13dd7aa;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(local_230);
      cVar3 = FUN_00f1b600();
      if (cVar3 != '\0') {
        uVar19 = CONCAT44(0x13dd7d2,puVar10);
        local_394 = (uint *)FUN_016830c0(0x2c);
        local_14._0_1_ = 99;
        if (local_394 == (uint *)0x0) {
          local_234 = (uint *)0x0;
        }
        else {
          uVar19 = CONCAT44(0x13dd7ee,(int)uVar19);
          FUN_010b9e20(0x2c);
          local_234 = (uint *)FUN_010baa10();
        }
        local_14._0_1_ = 1;
        local_244 = 9;
        uVar22 = 0x13dd838;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_00f19d60(s_TOOLTIP_TIME_REMAINING_01af32e4);
        FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
        local_14._0_1_ = 100;
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        local_14._0_1_ = 1;
        thunk_FUN_00f1a330();
        iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x72;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        FUN_01250240(&DAT_01ac59a8);
        piVar11 = (int *)0x13dd912;
        local_278 = (uint *)FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        FUN_01018b40();
        FUN_00f1f200();
        param_1 = local_3ac;
        puVar10 = (undefined8 *)0x13dd94e;
        FUN_013e2200();
        FUN_012338a0(local_234);
        iVar7 = local_398;
      }
    }
  }
  if ((*(short *)(iVar7 + 0x18) == 0x34) || (cVar3 = FUN_00f23a90(), cVar3 == '\0')) {
    puVar10 = (undefined8 *)0x13dd983;
    FUN_013e1240();
  }
  if (((*(int *)(iVar7 + 0x10) != 0) && (*(short *)(iVar7 + 0x18) != 0xb4)) ||
     (*(char *)(iVar7 + 0xc) == '\x03')) {
    cVar3 = FUN_00f24480();
    if (((cVar3 != '\0') || (cVar3 = FUN_00f79990(), cVar3 != '\0')) ||
       ((*(int *)(param_1 + 0x144) != 0 &&
        ((iVar12 = FUN_015d17e0(iVar7 + 0x10), iVar12 != 0 && (*(short *)(iVar12 + 0xdc) == 0))))))
    {
      FUN_013c2850(1);
    }
    if ((*(int *)(param_1 + 0x70) == 0x97) && (*(int *)(param_1 + 0x74) == 0)) {
switchD_013dd9fe_caseD_3:
      puVar10 = *(undefined8 **)(iVar7 + 4);
    }
    else {
      switch(*(undefined1 *)(iVar7 + 0x14)) {
      case 0:
      case 1:
        if ((*(short *)(iVar7 + 0x18) == 0x3e) && (*(short *)(iVar7 + 0x1a) == 0x1e)) {
          local_38c = (uint *)FUN_016830c0(0x2c);
          local_14._0_1_ = 0x65;
          if (local_38c == (uint *)0x0) {
            local_234 = (uint *)0x0;
          }
          else {
            FUN_010b9e20(0x2c);
            local_234 = (uint *)FUN_010baa10();
          }
          local_14._0_1_ = 1;
          local_244 = 9;
          puVar10 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          piVar11 = (int *)0x13ddacd;
          FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
          FUN_012338a0(local_234);
        }
        break;
      case 2:
        break;
      case 3:
        goto switchD_013dd9fe_caseD_3;
      default:
        goto switchD_013dd9fe_default;
      }
      if (*(int *)(param_1 + 0x88) != 0) {
        puVar10 = (undefined8 *)0x13ddaf5;
        FUN_01255040();
        goto switchD_013dd9fe_default;
      }
      puVar10 = *(undefined8 **)(iVar7 + 4);
    }
    piVar11 = (int *)(param_1 + 0x10);
    puVar8 = (undefined8 *)0x13ddb17;
    FUN_01258150(piVar11,puVar10,0);
  }
switchD_013dd9fe_default:
  switch(*(undefined2 *)(iVar7 + 0x18)) {
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1b:
  case 0x1f:
  case 0x22:
  case 0x23:
  case 0x24:
    if (*(short *)(iVar7 + 0x5c) != 0) {
      if (*(int *)(param_1 + 0x88) == 0) {
        uVar22 = FUN_00f791e0();
      }
      else if (*(char *)(iVar7 + 0xc) == '\x03') {
        if ((*(int *)(param_1 + 0x170) == 0) || (iVar7 = FUN_015d1a30(iVar7 + 4), iVar7 == 0))
        goto LAB_013e0eca;
        uVar22 = *(undefined4 *)(iVar7 + 8);
      }
      else {
        uVar22 = FUN_00f79280();
      }
      puVar23 = local_228;
      uVar27 = 0x100;
      puVar8 = (undefined8 *)0x13dde50;
      FUN_00f222a0(puVar23,0x100,u__d___01aecad8,uVar22);
      FUN_013c2850(1);
      uVar19 = CONCAT44(0x13dde63,uVar27);
      local_394 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x6a;
      if (local_394 == (uint *)0x0) {
        local_230 = (int *)0x0;
      }
      else {
        uVar19 = CONCAT44(0x13dde7f,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_230 = (int *)FUN_010baa10();
      }
      local_14._0_1_ = 1;
      local_244 = 9;
      uVar22 = 0x13ddec9;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_COMMON_TXT_APPLY_ATTRIBUTE_01ac5220);
      FUN_015ce480(local_294,uVar21,puVar8,puVar23,uVar19,uVar22);
      local_14._0_1_ = 0x6b;
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
      iVar12 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      iVar7 = local_398;
      local_22c = local_22c & 0xffffff00;
      *(undefined4 *)(iVar12 + 8) = 0x72;
      local_244 = 9;
      if ((*(int *)(param_1 + 0x88) != 0) && (cVar3 = FUN_00f79440(local_398), cVar3 != '\0')) {
        local_22c = CONCAT31(local_22c._1_3_,1);
      }
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_01250240(local_228);
      puVar10 = &local_274;
      piVar11 = (int *)0x13ddff4;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(local_230);
      local_3b0 = *(undefined8 **)(param_1 + 0x88);
      if ((local_3b0 != (undefined8 *)0x0) &&
         (local_234 = (uint *)0x0, *(short *)(iVar7 + 0x5c) != 0)) {
        local_2a0 = (undefined8 *)(param_1 + 0x10);
        _local_238 = (short *)((int)local_3b0 + 0xc);
        do {
          local_394 = (uint *)FUN_016830c0(0x2c);
          local_14._0_1_ = 0x6c;
          if (local_394 == (uint *)0x0) {
            local_230 = (int *)0x0;
          }
          else {
            FUN_010b9e20(0x2c);
            local_230 = (int *)FUN_010baa10();
          }
          local_14._0_1_ = 1;
          local_244 = 9;
          puVar8 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar8;
          local_26c = *(undefined1 **)(puVar8 + 1);
          FUN_00f1a3a0();
          local_14._0_1_ = 0x6d;
          puVar18 = (uint *)((int)local_234 + 1);
          local_298 = puVar18;
          FUN_00f19d60();
          FUN_015ce480();
          local_14._0_1_ = 0x6e;
          FUN_00f19ea0(puVar18);
          FUN_01500f80(local_2d8,u__s__d_01adb7d4);
          local_14._0_1_ = 0x6d;
          thunk_FUN_00f1a330();
          uVar21 = FUN_00f19ea0();
          FUN_01250240(uVar21);
          iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
          *(undefined4 *)(iVar7 + 8) = 0x72;
          uVar22 = DAT_01bee890;
          uVar21 = DAT_01bee88c;
          if (*_local_238 == 0) {
            local_244 = 9;
            puVar8 = (undefined8 *)FUN_00f4eeb0();
            local_274 = *puVar8;
            local_26c = *(undefined1 **)(puVar8 + 1);
            uVar19 = CONCAT44(DAT_01bee88c,DAT_01bee890);
            puVar8 = *(undefined8 **)(param_1 + 0x58);
            uVar21 = 0x13de1d3;
            uVar22 = DAT_01bee88c;
            uVar27 = DAT_01bee890;
            iVar7 = FUN_01230f40(puVar8,DAT_01bee88c,DAT_01bee890,DAT_01bee88c,DAT_01bee890);
            *(undefined4 *)(iVar7 + 8) = 0x12;
            FUN_00f19d60(s_TOOLTIP_ITEM_EMPTY_SLOT_01af8014);
            FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
            local_14._0_1_ = 0x6f;
            uVar22 = FUN_00f19ea0();
            FUN_01250240(uVar22);
            local_14._0_1_ = 0x6d;
            thunk_FUN_00f1a330();
            piVar15 = local_230;
            puVar10 = &local_274;
            piVar11 = (int *)0x13de243;
            FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
            FUN_012338a0(piVar15);
          }
          else {
            local_38c = (uint *)0xf;
            local_278 = (uint *)0xf;
            puVar14 = (undefined4 *)FUN_00f582a0();
            piVar11 = local_230;
            iVar7 = FUN_01231410(1,*_local_238,1,*puVar14,puVar14[1],uVar21,uVar22);
            local_29c = (uint *)0xc;
            local_3b4 = (short *)0x12;
            uVar21 = FUN_00f582a0();
            FUN_00f866f0(uVar21);
            *(undefined4 *)(iVar7 + 0x24) = 1;
            uVar21 = 0x13de30f;
            puVar8 = local_2a0;
            puVar10 = local_3b0;
            FUN_012598c0(local_2a0,piVar11,local_3b0,local_234,local_22c);
            param_1 = local_3ac;
          }
          local_14._0_1_ = 1;
          thunk_FUN_00f1a330();
          local_234 = local_298;
          _local_238 = _local_238 + 1;
        } while ((int)local_298 < (int)(uint)*(ushort *)(local_398 + 0x5c));
      }
    }
    break;
  case 0x35:
    FUN_013c2850(1);
    uVar19 = CONCAT44(0x13ddb45,puVar10);
    local_394 = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x66;
    if (local_394 == (uint *)0x0) {
      local_22c = 0;
    }
    else {
      uVar19 = CONCAT44(0x13ddb61,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_22c = FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar22 = 0x13ddba3;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_EVO_SLOT_01af8040);
    FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x67;
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    uVar6 = local_22c;
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_01250270(*(undefined1 *)(local_398 + 0x74));
    puVar10 = &local_274;
    uVar22 = 0x13ddc89;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(uVar6);
    uVar19 = CONCAT44(0x13ddc99,puVar10);
    local_394 = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x68;
    if (local_394 == (uint *)0x0) {
      local_22c = 0;
    }
    else {
      uVar19 = CONCAT44(0x13ddcb5,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_22c = FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar27 = 0x13ddcf7;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_CHIPSET_SLOT_01af8058);
    FUN_015ce480(local_294,uVar21,puVar8,uVar22,uVar19,uVar27);
    local_14._0_1_ = 0x69;
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    uVar6 = local_22c;
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_01250270(*(undefined1 *)(local_398 + 0x75));
    puVar10 = &local_274;
    piVar11 = (int *)0x13ddddd;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(uVar6);
  }
  cVar3 = FUN_00f681c0();
  if ((((cVar3 != '\0') && (cVar3 = FUN_00f681c0(), cVar3 != '\0')) &&
      (cVar3 = FUN_00f681c0(), cVar3 != '\0')) && (*(int *)(param_1 + 0x16c) != 0)) {
    FUN_01274fc0(8);
    uVar22 = FUN_015cd180();
    FUN_013e2100(uVar22);
    local_14 = CONCAT31(local_14._1_3_,0x70);
    FUN_013e11e0(4);
    local_38c = *(uint **)(param_1 + 0x68);
    puVar10 = (undefined8 *)0x13de3e6;
    FUN_00f85fa0();
    uVar22 = FUN_013e20a0(&local_394);
    cVar3 = FUN_013e2070(uVar22);
    if (cVar3 == '\0') {
      FUN_013c2850(1);
      FUN_00f22260();
      local_38c = *(uint **)(param_1 + 0x68);
      local_3b0 = (undefined8 *)FUN_0121bcc0(&local_38c);
      FUN_00f22260();
      local_38c = *(uint **)(param_1 + 0x68);
      local_2a0 = (undefined8 *)FUN_0121cbd0(&local_38c);
      FUN_00f22260();
      local_38c = *(uint **)(param_1 + 0x68);
      local_298 = (uint *)FUN_0121d4c0(&local_38c);
      FUN_00f22260();
      local_278 = (uint *)FUN_0121d4b0(*(undefined4 *)(param_1 + 0x68));
      local_244 = 9;
      FUN_013e20e0();
      FUN_013e11e0(4);
      FUN_010c3d00(&local_230);
      FUN_013e21f0(local_238);
      uVar19 = CONCAT44(0x13de4fb,puVar10);
      cVar3 = FUN_013e20f0(_local_238);
      if (cVar3 == '\0') {
        local_38c = (uint *)(param_1 + 0x10);
        do {
          uVar27 = (undefined4)uVar19;
          uVar22 = FUN_013e20e0();
          FUN_013e11f0(uVar22);
          local_14._0_1_ = 0x71;
          uVar19 = CONCAT44(0x13de532,uVar27);
          local_394 = (uint *)FUN_016830c0(0x2c);
          local_14._0_1_ = 0x72;
          if (local_394 == (uint *)0x0) {
            local_22c = 0;
          }
          else {
            uVar19 = CONCAT44(0x13de54e,(int)uVar19);
            FUN_010b9e20(0x2c);
            local_22c = FUN_010baa10();
          }
          local_38c = (uint *)(uint)local_2aa;
          piVar15 = (int *)FUN_010083a0();
          local_38c = (uint *)*piVar15;
          uVar22 = 0x13de598;
          FUN_00f1a3a0();
          local_14._0_1_ = 0x73;
          switch(local_2b4) {
          case 0:
            pcVar20 = s_COMMON_TXT_NORMAL_01ace66c;
            break;
          case 1:
            pcVar20 = s_TOOLTIP_SEALMASTER_BRONZE_01af7570;
            break;
          case 2:
            pcVar20 = s_TOOLTIP_SEALMASTER_SILVER_01af758c;
            break;
          case 3:
            pcVar20 = s_TOOLTIP_SEALMASTER_GOLD_01af7608;
            break;
          case 4:
            pcVar20 = s_TOOLTIP_SEALMASTER_PLATINUM_01af7620;
            break;
          case 5:
            pcVar20 = s_TOOLTIP_SEALMASTER_MASTER_01af75dc;
            break;
          default:
            goto switchD_013de5a8_default;
          }
          FUN_00f19d60(pcVar20);
          uVar21 = FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
          FUN_00f19c80(uVar21);
          thunk_FUN_00f1a330();
switchD_013de5a8_default:
          FUN_00f1a3a0();
          local_14 = CONCAT31(local_14._1_3_,0x74);
          uVar6 = (uint)local_2aa;
          puVar18 = local_38c;
          uVar22 = FUN_00f19ea0();
          puVar8 = local_2d8;
          uVar21 = 0x13de660;
          FUN_01500f80(puVar8,u__s__d__d_01af802c,uVar22,puVar18,uVar6);
          if (local_278 == (uint *)(uint)local_2b4) {
            puVar13 = (undefined8 *)FUN_00f4eeb0();
            local_3b8 = *(undefined1 **)(puVar13 + 1);
            puVar10 = &local_3c0;
            local_3c0 = *puVar13;
          }
          else {
            puVar10 = (undefined8 *)FUN_00f4eeb0();
          }
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          uVar6 = local_22c;
          puVar10 = &local_274;
          piVar11 = (int *)0x13de72e;
          FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
          FUN_012338a0(uVar6);
          thunk_FUN_00f1a330();
          thunk_FUN_00f1a330();
          local_14 = CONCAT31(local_14._1_3_,0x70);
          FUN_013e3280();
          FUN_013e20d0();
          uVar19 = CONCAT44(0x13de777,puVar10);
          cVar3 = FUN_013e20f0(_local_238);
        } while (cVar3 == '\0');
      }
      puVar8 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar8;
      local_26c = *(undefined1 **)(puVar8 + 1);
      local_394 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x75;
      if (local_394 == (uint *)0x0) {
        local_22c = 0;
      }
      else {
        FUN_010b9e20(0x2c);
        local_22c = FUN_010baa10();
      }
      FUN_00f1a3a0();
      local_14._0_1_ = 0x76;
      iVar7 = (int)local_3b0 + (int)local_2a0;
      FUN_00f19d60();
      FUN_015ce480();
      local_14._0_1_ = 0x77;
      FUN_00f19ea0(iVar7);
      uVar27 = 0x13de84e;
      FUN_01500f80(local_2d8,u__s____d_01af026c);
      local_14._0_1_ = 0x76;
      thunk_FUN_00f1a330();
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      uVar6 = local_22c;
      puVar8 = &local_274;
      uVar22 = 0x13de897;
      FUN_01230c40(puVar8,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(uVar6);
      local_14._0_1_ = 0x70;
      thunk_FUN_00f1a330();
      uVar19 = CONCAT44(0x13de8b6,puVar8);
      local_394 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x78;
      if (local_394 == (uint *)0x0) {
        local_22c = 0;
      }
      else {
        uVar19 = CONCAT44(0x13de8d2,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_22c = FUN_010baa10();
      }
      uVar9 = 0x13de8f6;
      FUN_00f1a3a0();
      puVar18 = local_298;
      local_14._0_1_ = 0x79;
      FUN_00f19d60(s_SEALMASTER_REGISTERED_QUANTITY_01af80c4);
      FUN_015ce480(local_294,uVar21,uVar27,uVar22,uVar19,uVar9);
      local_14._0_1_ = 0x7a;
      uVar22 = FUN_00f19ea0();
      uVar27 = 0x13de941;
      FUN_01500f80(local_2d8,u__s____d_01af026c,uVar22,puVar18);
      local_14._0_1_ = 0x79;
      thunk_FUN_00f1a330();
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      uVar6 = local_22c;
      puVar8 = &local_274;
      uVar22 = 0x13de98a;
      FUN_01230c40(puVar8,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(uVar6);
      local_14._0_1_ = 0x70;
      thunk_FUN_00f1a330();
      uVar19 = CONCAT44(0x13de9af,puVar8);
      local_394 = (uint *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0x7b;
      if (local_394 == (uint *)0x0) {
        local_22c = 0;
      }
      else {
        uVar19 = CONCAT44(0x13de9cb,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_22c = FUN_010baa10();
      }
      uVar9 = 0x13de9ef;
      FUN_00f1a3a0();
      local_14._0_1_ = 0x7c;
      iVar7 = (int)local_298 + (int)local_2a0 + (int)local_3b0;
      FUN_00f19d60(s_SEALMASTER_TOTAL_QUANTITY_01af8074);
      FUN_015ce480(local_294,uVar21,uVar27,uVar22,uVar19,uVar9);
      local_14._0_1_ = 0x7d;
      uVar22 = FUN_00f19ea0();
      puVar8 = (undefined8 *)0x13dea46;
      FUN_01500f80(local_2d8,u__s____d_01af026c,uVar22,iVar7);
      local_14 = CONCAT31(local_14._1_3_,0x7c);
      thunk_FUN_00f1a330();
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      uVar6 = local_22c;
      puVar10 = &local_274;
      piVar11 = (int *)0x13dea8f;
      FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
      param_1 = local_3ac;
      FUN_012338a0(uVar6);
      thunk_FUN_00f1a330();
    }
    local_14._0_1_ = 1;
    FUN_013e11c0();
  }
  local_38c = *(uint **)(param_1 + 0x68);
  local_2ec[0] = FUN_01660720(&local_38c);
  local_3f0 = *(undefined4 *)(param_1 + 0x68);
  cVar3 = FUN_01660a20(&local_3f0);
  if ((cVar3 != '\0') && (0 < local_2ec[0])) {
    FUN_013c2850(1);
    local_394 = (uint *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x7e;
    if (local_394 == (uint *)0x0) {
      local_234 = (uint *)0x0;
    }
    else {
      FUN_010b9e20(0x2c);
      local_234 = (uint *)FUN_010baa10();
    }
    local_14 = CONCAT31(local_14._1_3_,1);
    local_244 = 9;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    local_38c = (uint *)FUN_0164a550(local_2ec);
    local_278 = (uint *)FUN_0164a5f0(local_2ec);
    _local_238 = (short *)FUN_013e2090();
    local_3b0 = (undefined8 *)(param_1 + 0x114);
    local_29c = (uint *)0x0;
    puVar18 = (uint *)0x0;
    do {
      puVar17 = local_29c;
      FUN_013e11e0(4);
      FUN_010c3d00(&local_2a0);
      cVar3 = FUN_00f681c0();
      if (cVar3 == '\0') {
        local_298 = (uint *)0x0;
      }
      else {
        FUN_00f22260();
        local_390 = puVar17;
        local_298 = (uint *)FUN_0121d0e0(&local_390);
      }
      if (0 < (int)_local_238) {
        local_3b4 = _local_238;
        puVar17 = puVar18;
        do {
          FUN_013e20b0();
          iVar7 = FUN_013e2080();
          iVar12 = FUN_00f22260();
          puVar18 = (uint *)((int)puVar17 + 1);
          if (*(int *)(iVar7 + 4) != iVar12) {
            puVar18 = puVar17;
          }
          local_3b4 = (short *)((int)local_3b4 + -1);
          puVar17 = puVar18;
        } while (local_3b4 != (short *)0x0);
        local_3b4 = (short *)0x0;
        puVar17 = local_29c;
      }
      local_29c = (uint *)((int)puVar17 + 1);
    } while ((int)local_29c < 0x11);
    local_298 = puVar18;
    FUN_00f1a3a0();
    local_14 = CONCAT31(local_14._1_3_,0x7f);
    FUN_013e11e0(4);
    FUN_010c3d00(&local_230);
    uVar22 = FUN_013e20a0(&local_394);
    cVar3 = FUN_013e2070(uVar22);
    psVar16 = _local_238;
    param_1 = local_3ac;
    if (cVar3 == '\0') {
      psVar28 = _local_238;
      puVar18 = local_298;
      FUN_013e2080();
      uVar22 = FUN_00f19ea0();
      puVar8 = local_2d8;
      uVar21 = 0x13ded40;
      FUN_01500f80(puVar8,u__s___d__d__01af8090,uVar22,puVar18,psVar28);
    }
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    puVar10 = &local_274;
    piVar11 = (int *)0x13ded78;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(local_234);
    FUN_013e11e0(4);
    FUN_010c3d00(&local_22c);
    local_29c = (uint *)0x0;
    if (0 < (int)psVar16) {
      do {
        local_394 = (uint *)FUN_016830c0(0x2c);
        local_14._0_1_ = 0x80;
        if (local_394 == (uint *)0x0) {
          local_278 = (uint *)0x0;
        }
        else {
          FUN_010b9e20(0x2c);
          local_278 = (uint *)FUN_010baa10();
        }
        local_14._0_1_ = 0x7f;
        FUN_01250240(&DAT_01ac5dd8);
        iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x26;
        local_244 = 9;
        puVar10 = (undefined8 *)FUN_00f4eeb0();
        puVar18 = (uint *)0x0;
        local_274 = *puVar10;
        local_26c = *(undefined1 **)(puVar10 + 1);
        do {
          if (*(int *)(param_1 + 0x114) == 0) {
            piVar11 = (int *)0x0;
          }
          else {
            local_390 = puVar18;
            piVar11 = (int *)FUN_0121d0e0(&local_390);
          }
          if (*(int *)(local_22c + 0xc) == *piVar11) {
            local_3b8 = &DAT_3f7ae148;
            local_274 = CONCAT44(DAT_01b02744,DAT_01b02744);
            local_26c = &DAT_3f7ae148;
          }
          puVar18 = (uint *)((int)puVar18 + 1);
        } while ((int)puVar18 < 0x11);
        if (*(int *)(param_1 + 300) == 0) goto LAB_013e0ec5;
        local_3b4 = *(short **)(local_22c + 0xc);
        iVar7 = FUN_015db910(&local_3b4);
        if (iVar7 == 0) goto LAB_013e0ec5;
        FUN_00f1a3a0();
        local_14._0_1_ = 0x81;
        uVar22 = FUN_00f19ea0();
        FUN_01500f80(local_304,&DAT_01aec624,uVar22);
        FUN_013e20c0();
        uVar22 = FUN_00f19ea0();
        FUN_01250240(uVar22);
        puVar18 = local_278;
        puVar10 = &local_274;
        piVar11 = (int *)0x13def64;
        FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
        FUN_012338a0(puVar18);
        local_14 = CONCAT31(local_14._1_3_,0x7f);
        thunk_FUN_00f1a330();
        local_29c = (uint *)((int)local_29c + 1);
      } while ((int)local_29c < (int)_local_238);
    }
    local_394 = (uint *)FUN_013e2090();
    local_29c = (uint *)0x0;
    if (0 < (int)local_394) {
      local_278 = (uint *)(param_1 + 0x18);
      local_3c0 = 0x3f4ccccd3f5ededf;
      local_3b8 = (undefined1 *)0x3e40c0c1;
      do {
        local_390 = (uint *)FUN_016830c0(0x2c);
        local_14._0_1_ = 0x82;
        if (local_390 == (uint *)0x0) {
          local_234 = (uint *)0x0;
        }
        else {
          memset(local_390,0,0x2c);
          local_234 = (uint *)FUN_010baa10();
        }
        local_14._0_1_ = 0x7f;
        local_244 = 7;
        FUN_00f1a040();
        uVar22 = 0x13df060;
        FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        local_38c = local_234;
        uVar27 = 0x13df07c;
        FUN_00f58160();
        uVar9 = 0x2c;
        uVar26 = CONCAT44(0x13df083,uVar27);
        local_39c = (void *)FUN_016830c0(0x2c);
        local_14._0_1_ = 0x83;
        if (local_39c == (void *)0x0) {
          local_38c = (uint *)0x0;
        }
        else {
          uVar26 = ZEXT48(local_39c);
          uVar22 = 0x13df0a0;
          memset(local_39c,0,0x2c);
          uVar9 = 0x13df0aa;
          local_38c = (uint *)FUN_010baa10();
        }
        local_14._0_1_ = 0x7f;
        local_244 = 9;
        local_274 = local_3c0;
        local_26c = local_3b8;
        FUN_00f19d60(s_SETITEM_SET_EFFECT_01af8118);
        FUN_015ce480(local_3d8,uVar21,puVar8,uVar22,uVar26,uVar9);
        local_14._0_1_ = 0x84;
        uVar21 = FUN_00f19ea0();
        FUN_00f1a2b0(uVar21);
        local_14._0_1_ = 0x86;
        FUN_00f1a330();
        piVar11 = local_230 + 3;
        FUN_00f1a2b0(u__value1__01ad2720);
        local_14._0_1_ = 0x87;
        FUN_01501660(local_304,local_334,piVar11);
        local_14._0_1_ = 0x86;
        FUN_00f1a330();
        psVar16 = (short *)FUN_00f19ea0();
        do {
          sVar5 = *psVar16;
          psVar16 = psVar16 + 1;
        } while (sVar5 != 0);
        FUN_00f1a040();
        puVar18 = local_38c;
        if ((int)local_298 < local_230[3]) {
          local_3e8 = (undefined1 *)0x3f000000;
          puStack_3e4 = (undefined1 *)0x3f000000;
          local_3e0 = (undefined1 *)0x3f000000;
        }
        else {
          local_3e8 = &DAT_3f7ae148;
          puStack_3e4 = &DAT_3f7ae148;
          local_3e0 = &DAT_3f7ae148;
        }
        local_274 = CONCAT44(puStack_3e4,local_3e8);
        local_26c = local_3e0;
        local_244 = 9;
        iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x98;
        local_38c = puVar18;
        FUN_00f58160();
        if (*(int *)(param_1 + 0x144) == 0) {
LAB_013df634:
          FUN_00f1a330();
          goto LAB_013e0ec5;
        }
        local_3b4 = (short *)local_230[10];
        iVar7 = FUN_015d17e0(&local_3b4);
        if (iVar7 == 0) goto LAB_013df634;
        FUN_00f221a0(iVar7 + 0xf8);
        local_14._0_1_ = 0x88;
        FUN_0124fea0();
        local_14._0_1_ = 0x89;
        local_3a0 = &DAT_3f7ae148;
        FUN_0124ff30(DAT_01bfe468 + 8,9,CONCAT44(DAT_01b02744,DAT_01b02744),&DAT_3f7ae148);
        puVar8 = (undefined8 *)FUN_00f4eeb0();
        local_388 = *puVar8;
        local_380 = *(undefined4 *)(puVar8 + 1);
        local_2c0 = 0;
        FUN_0114def0();
        local_14._0_1_ = 0x8a;
        FUN_0114b1c0();
        local_14._0_1_ = 0x8b;
        local_279 = '\0';
        local_2e4 = 0;
        local_3b0 = (undefined8 *)0x12c;
        uVar21 = FUN_00f19ea0();
        FUN_00f1a2b0(uVar21);
        local_14._0_1_ = 0x8c;
        puVar8 = &local_2e4;
        uVar21 = 0x13df3c7;
        FUN_0114b510(puVar8,local_334,&local_3b0,local_358,&local_279);
        local_14 = CONCAT31(local_14._1_3_,0x8e);
        FUN_00f1a330();
        _local_238 = (short *)0x0;
        FUN_010c3d00(local_238);
        cVar3 = *(char *)((int)_local_238 + 0xd);
        while (cVar3 == '\0') {
          FUN_010c3f60(&local_388);
          puVar18 = (uint *)FUN_01157240();
          if (puVar18 != (uint *)0x0) {
            local_38c = puVar18;
            FUN_00f58160();
          }
          std::
          _Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
          ::operator++((_Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
                        *)local_238);
          param_1 = local_3ac;
          cVar3 = *(char *)((int)_local_238 + 0xd);
        }
        local_230 = (int *)*local_230;
        local_2a0 = (undefined8 *)FUN_016830c0(0x2c);
        local_14._0_1_ = 0x8f;
        if (local_2a0 == (undefined8 *)0x0) {
          local_38c = (uint *)0x0;
        }
        else {
          memset(local_2a0,0,0x2c);
          local_38c = (uint *)FUN_010baa10();
        }
        local_14._0_1_ = 0x8e;
        FUN_00f1a040();
        puVar10 = &local_274;
        piVar11 = (int *)0x13df4e0;
        iVar7 = FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
        *(undefined4 *)(iVar7 + 8) = 0x98;
        FUN_010c3ca0();
        local_14._0_1_ = 0x90;
        FUN_0114b460();
        FUN_00f1d670();
        local_35c = 0;
        FUN_00f1a330();
        FUN_00f1a330();
        local_14 = CONCAT31(local_14._1_3_,0x7f);
        FUN_00f1a330();
        local_29c = (uint *)((int)local_29c + 1);
      } while ((int)local_29c < (int)local_394);
    }
    local_14._0_1_ = 1;
    FUN_00f1a330();
  }
  iVar7 = local_398;
  if (*(int *)(local_398 + 0x114) != 0) {
    FUN_013c2850(1);
    local_3a0 = &DAT_3f7ae148;
    local_274 = CONCAT44(DAT_01b02744,DAT_01b02744);
    local_244 = 9;
    local_26c = &DAT_3f7ae148;
    uVar22 = FUN_00f19ea0();
    puVar10 = (undefined8 *)0x118;
    piVar11 = (int *)(param_1 + 0x10);
    puVar8 = (undefined8 *)0x13df5d8;
    FUN_012543f0(piVar11,0x118,uVar22,&local_274);
  }
  sVar5 = *(short *)(iVar7 + 0x18);
  if (sVar5 == 0x5b) {
    FUN_013c2850(1);
    uVar22 = 0x2c;
    uVar19 = CONCAT44(0x13e0216,puVar10);
    local_39c = (void *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x91;
    if (local_39c == (void *)0x0) {
      local_234 = (uint *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13e0232,(int)uVar19);
      FUN_010b9e20(0x2c);
      uVar22 = 0x13e0239;
      local_234 = (uint *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_274 = CONCAT44(DAT_01b026d8,DAT_01b0273c);
    local_3a0 = (undefined1 *)0x3f19999a;
    local_244 = 9;
    local_26c = (undefined1 *)0x3f19999a;
    FUN_00f19d60(s_TOOLTIP_ITEM_REFUND_COST_01af812c);
    FUN_015ce480(local_3d8,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x92;
    psVar16 = (short *)FUN_00f19ea0();
    do {
      sVar5 = *psVar16;
      psVar16 = psVar16 + 1;
    } while (sVar5 != 0);
    FUN_00f1a040();
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    uVar19 = CONCAT44(DAT_01b02744,DAT_01b02744);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    uVar6 = local_234[4];
    local_3a0 = &DAT_3f7ae148;
    uVar22 = 0;
    puVar8 = (undefined8 *)0x2;
    uVar21 = 9;
    FUN_01231af0(1,*(undefined4 *)(local_398 + 0x50),0,uVar19,&DAT_3f7ae148,9,2,0,0,DAT_01bee88c,
                 DAT_01bee890);
    local_394 = local_234;
    local_234[4] = uVar6;
    uVar27 = 0x13e038c;
    FUN_00f58160();
    uVar9 = 0x2c;
    uVar19 = CONCAT44(0x13e0393,uVar27);
    local_39c = (void *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x93;
    if (local_39c == (void *)0x0) {
      local_38c = (uint *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13e03af,(int)uVar19);
      FUN_010b9e20(0x2c);
      uVar9 = 0x13e03b6;
      local_38c = (uint *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_274 = CONCAT44(DAT_01b026d8,DAT_01b0273c);
    local_3a0 = (undefined1 *)0x3f19999a;
    local_244 = 9;
    local_26c = (undefined1 *)0x3f19999a;
    FUN_00f19d60(s_TOOLTIP_ITEM_SCAN_COST_01af80e4);
    FUN_015ce480(local_3d8,uVar21,puVar8,uVar22,uVar19,uVar9);
    local_14._0_1_ = 0x94;
    psVar16 = (short *)FUN_00f19ea0();
    do {
      sVar5 = *psVar16;
      psVar16 = psVar16 + 1;
    } while (sVar5 != 0);
    FUN_00f1a040();
    local_14._0_1_ = 1;
    FUN_00f1a330();
    puVar18 = local_38c;
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    uVar22 = DAT_01b02744;
    local_3a0 = &DAT_3f7ae148;
    *(undefined4 *)(iVar7 + 8) = 0x72;
    local_274 = CONCAT44(uVar22,uVar22);
    local_244 = 9;
    local_26c = &DAT_3f7ae148;
    FUN_00f1a040();
    piVar11 = (int *)0x13e04e6;
    FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    local_390 = puVar18;
    puVar10 = (undefined8 *)0x13e04fe;
    FUN_00f58160();
    iVar7 = local_398;
  }
  else if (sVar5 == 0x5c) {
    FUN_013c2850(1);
    uVar19 = CONCAT44(0x13dff1c,puVar10);
    local_39c = (void *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x95;
    if (local_39c == (void *)0x0) {
      local_38c = (uint *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13dff38,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_38c = (uint *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar22 = 0x13dff82;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_REFUND_COST_01af812c);
    FUN_015ce480(local_3d8,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x96;
    uVar21 = FUN_00f19ea0();
    FUN_01250240(uVar21);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    iVar7 = FUN_01232610(&local_2e4);
    uVar22 = DAT_01bee890;
    uVar21 = DAT_01bee88c;
    local_278 = *(uint **)(iVar7 + 4);
    puVar8 = (undefined8 *)FUN_00f4eeb0();
    puVar18 = local_38c;
    uVar25 = 0;
    uVar24 = 0;
    uVar9 = 2;
    uVar27 = 9;
    FUN_01231af0(1,*(undefined4 *)(local_398 + 0x50),0,*puVar8,*(undefined4 *)(puVar8 + 1),9,2,0,0,
                 uVar21,uVar22);
    FUN_01232640(local_278);
    FUN_012338a0(puVar18);
    uVar19 = CONCAT44(0x13e009a,uVar25);
    local_39c = (void *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x97;
    if (local_39c == (void *)0x0) {
      local_234 = (uint *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13e00b6,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_234 = (uint *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar21 = 0x13e0100;
    puVar8 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar8;
    local_26c = *(undefined1 **)(puVar8 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_SCAN_COST_01af80e4);
    FUN_015ce480(local_3d8,uVar27,uVar9,uVar24,uVar19,uVar21);
    local_14._0_1_ = 0x98;
    uVar21 = FUN_00f19ea0();
    FUN_01250240(uVar21);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    uVar27 = DAT_01bee890;
    uVar22 = DAT_01bee88c;
    puVar13 = (undefined8 *)FUN_00f4eeb0();
    iVar7 = local_398;
    puVar10 = (undefined8 *)0x0;
    piVar11 = (int *)0x0;
    puVar8 = (undefined8 *)0x2;
    uVar21 = 9;
    FUN_01231af0(1,*(undefined4 *)(local_398 + 0x4c),0,*puVar13,*(undefined4 *)(puVar13 + 1),9,2,0,0
                 ,uVar22,uVar27);
    FUN_01232640(local_278);
    param_1 = local_3ac;
    FUN_012338a0(local_234);
  }
  else if (sVar5 == 0x5d) {
    FUN_013c2850(1);
    uVar19 = CONCAT44(0x13df607,puVar10);
    local_39c = (void *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x99;
    if (local_39c == (void *)0x0) {
      local_38c = (uint *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13df623,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_38c = (uint *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar22 = 0x13df683;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_REFUND_COST_01af812c);
    FUN_015ce480(local_3d8,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0x9a;
    uVar21 = FUN_00f19ea0();
    FUN_01250240(uVar21);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    iVar7 = FUN_01232610(&local_2e4);
    uVar22 = DAT_01bee890;
    uVar21 = DAT_01bee88c;
    local_278 = *(uint **)(iVar7 + 4);
    puVar8 = (undefined8 *)FUN_00f4eeb0();
    puVar18 = local_38c;
    uVar25 = 0;
    uVar24 = 0;
    uVar9 = 2;
    uVar27 = 9;
    FUN_01231af0(1,*(undefined4 *)(local_398 + 0x50),0,*puVar8,*(undefined4 *)(puVar8 + 1),9,2,0,0,
                 uVar21,uVar22);
    FUN_01232640(local_278);
    FUN_012338a0(puVar18);
    uVar19 = CONCAT44(0x13df79b,uVar25);
    local_39c = (void *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0x9b;
    if (local_39c == (void *)0x0) {
      local_234 = (uint *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13df7b7,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_234 = (uint *)FUN_010baa10();
    }
    local_14._0_1_ = 1;
    local_244 = 9;
    uVar21 = 0x13df801;
    puVar8 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar8;
    local_26c = *(undefined1 **)(puVar8 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_SCAN_COST_01af80e4);
    FUN_015ce480(local_3d8,uVar27,uVar9,uVar24,uVar19,uVar21);
    local_14._0_1_ = 0x9c;
    uVar21 = FUN_00f19ea0();
    FUN_01250240(uVar21);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
    *(undefined4 *)(iVar7 + 8) = 0x72;
    uVar27 = DAT_01bee890;
    uVar22 = DAT_01bee88c;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    iVar7 = local_398;
    uVar9 = 0;
    piVar11 = (int *)0x0;
    puVar8 = (undefined8 *)0x2;
    uVar21 = 9;
    FUN_01231af0(1,*(undefined4 *)(local_398 + 0x4c),0,*puVar10,*(undefined4 *)(puVar10 + 1),9,2,0,0
                 ,uVar22,uVar27);
    FUN_01232640(local_278);
    param_1 = local_3ac;
    FUN_012338a0(local_234);
    uVar19 = CONCAT44(0x13df90b,uVar9);
    FUN_013c2850(1);
    if (*(int *)(param_1 + 0x158) != 0) {
      local_394 = *(uint **)(param_1 + 0x68);
      uVar19 = CONCAT44(0x13df92e,(int)uVar19);
      cVar3 = FUN_015d1e20(&local_394);
      if (cVar3 != '\0') {
        _local_238 = (short *)CONCAT13(*(undefined1 *)(param_1 + 0x78),local_238);
        local_394 = *(uint **)(param_1 + 0x68);
        puVar10 = (undefined8 *)0x13df961;
        local_390 = (uint *)FUN_0161e820();
        if (local_390 != (uint *)0x0) {
          local_278 = local_390 + 1;
          uVar22 = 0x13df98b;
          FUN_015db6e0();
          local_14._0_1_ = 0x9d;
          local_298 = (uint *)(uint)(ushort)local_390[2];
          uVar19 = CONCAT44(0x13df9a6,uVar22);
          local_39c = (void *)FUN_016830c0(0x2c);
          local_14._0_1_ = 0x9e;
          if (local_39c == (void *)0x0) {
            local_38c = (uint *)0x0;
          }
          else {
            uVar19 = CONCAT44(0x13df9c2,(int)uVar19);
            FUN_010b9e20(0x2c);
            local_38c = (uint *)FUN_010baa10();
          }
          local_14._0_1_ = 0x9d;
          local_244 = 9;
          uVar22 = 0x13dfa04;
          puVar10 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          FUN_00f19d60(s_TOOLTIP_ITEM_REQUIRE_DATA_01af80fc);
          FUN_015ce480(local_3d8,uVar21,puVar8,piVar11,uVar19,uVar22);
          local_14._0_1_ = 0x9f;
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          local_14._0_1_ = 0x9d;
          thunk_FUN_00f1a330();
          puVar18 = local_38c;
          iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
          *(undefined4 *)(iVar7 + 8) = 0x72;
          puVar10 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          puVar10 = &local_274;
          uVar22 = 0x13dfaeb;
          FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
          FUN_012338a0(puVar18);
          uVar19 = CONCAT44(0x13dfafb,puVar10);
          local_39c = (void *)FUN_016830c0(0x2c);
          local_14._0_1_ = 0xa0;
          if (local_39c == (void *)0x0) {
            local_234 = (uint *)0x0;
          }
          else {
            uVar19 = CONCAT44(0x13dfb17,(int)uVar19);
            FUN_010b9e20(0x2c);
            local_234 = (uint *)FUN_010baa10();
          }
          local_14._0_1_ = 0x9d;
          local_244 = 9;
          uVar27 = 0x13dfb61;
          puVar10 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          FUN_00f19d60(s_TOOLTIP_ITEM_REQUIRE_QUANTITY_01af8180);
          FUN_015ce480(local_3d8,uVar21,puVar8,uVar22,uVar19,uVar27);
          local_14._0_1_ = 0xa1;
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          local_14._0_1_ = 0x9d;
          thunk_FUN_00f1a330();
          uVar22 = 0x13dfbdf;
          iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
          *(undefined4 *)(iVar7 + 8) = 0x72;
          FUN_00f6db50();
          local_390 = (uint *)0x64;
          uVar19 = CONCAT44(*local_278 / 100,0x13dfc14);
          local_390 = (uint *)FUN_00f629b0();
          if ((local_298 == (uint *)0x0) || ((int)local_390 < (int)local_298)) {
            puVar10 = (undefined8 *)FUN_00f4eeb0();
            local_274 = *puVar10;
            local_26c = *(undefined1 **)(puVar10 + 1);
            uVar27 = 0x13dfd6f;
            FUN_00f1a3a0();
            local_14._0_1_ = 0xa4;
            FUN_00f19d60(s_TOOLTIP_ITEM_INSUFFICIENT_INGRED_01af8148);
            FUN_015ce480(local_3d8,uVar21,puVar8,uVar22,uVar19,uVar27);
            local_14._0_1_ = 0xa5;
            uVar22 = FUN_00f19ea0();
            puVar8 = (undefined8 *)0x13dfdb9;
            FUN_01500f80(local_304,u__d___s__01af8170,local_298,uVar22);
            local_14._0_1_ = 0xa4;
          }
          else {
            puVar10 = (undefined8 *)FUN_00f4eeb0();
            local_274 = *puVar10;
            local_26c = *(undefined1 **)(puVar10 + 1);
            FUN_01250270(local_298);
            uVar19 = CONCAT44(DAT_01bee88c,&local_274);
            uVar22 = 0x13dfc8e;
            FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
            uVar27 = 0x13dfca7;
            puVar10 = (undefined8 *)FUN_00f4eeb0();
            local_274 = *puVar10;
            local_26c = *(undefined1 **)(puVar10 + 1);
            FUN_00f19d60(s_TOOLTIP_ITEM_ENABLE_COUNT_01af81a0);
            FUN_015ce480(local_304,uVar21,puVar8,uVar22,uVar19,uVar27);
            local_14._0_1_ = 0xa2;
            local_390 = (uint *)((int)local_390 / (int)local_298);
            FUN_00f1a2b0(u__Count__01ace788);
            local_14._0_1_ = 0xa3;
            FUN_01501660(local_304,local_294,&local_390);
            local_14._0_1_ = 0xa2;
          }
          thunk_FUN_00f1a330();
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          puVar18 = local_234;
          puVar10 = &local_274;
          piVar11 = (int *)0x13dfe02;
          FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
          FUN_012338a0(puVar18);
          thunk_FUN_00f1a330();
          local_14._0_1_ = 1;
          thunk_FUN_00f1a330();
          iVar7 = local_398;
        }
        goto LAB_013e0504;
      }
    }
    local_244 = 9;
    uVar22 = 0x13dfe52;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_NOT_POSSIBLE_HATCH_01af81f4);
    FUN_015ce480(local_3d8,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14._0_1_ = 0xa6;
    uVar22 = FUN_00f19ea0();
    FUN_01250240(uVar22);
    local_14._0_1_ = 1;
    thunk_FUN_00f1a330();
    local_39c = (void *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0xa7;
    if (local_39c == (void *)0x0) {
      uVar22 = 0;
    }
    else {
      FUN_010b9e20(0x2c);
      uVar22 = FUN_010baa10();
    }
    local_14._0_1_ = 1;
    puVar10 = &local_274;
    piVar11 = (int *)0x13dfefe;
    FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
    FUN_012338a0(uVar22);
    iVar7 = local_398;
  }
LAB_013e0504:
  iVar12 = *(int *)(param_1 + 0x70);
  iVar1 = *(int *)(param_1 + 0x74);
  if ((iVar12 == 0x32) && (iVar1 == 0)) {
    if (DAT_01bfb8cc != 0) {
      puVar10 = (undefined8 *)0x13e0531;
      piVar15 = (int *)FUN_01293cd0();
      if ((piVar15 != (int *)0x0) && (cVar3 = (**(code **)(*piVar15 + 0x24))(), cVar3 != '\0')) {
        FUN_013c2850(1);
        if ((*(int *)(iVar7 + 0x50) == 0) || (*(ushort *)(iVar7 + 0x18) / 10 == 9)) {
          uVar19 = CONCAT44(0x13e0776,puVar10);
          local_39c = (void *)FUN_016830c0(0x2c);
          local_14._0_1_ = 0xab;
          if (local_39c == (void *)0x0) {
            local_38c = (uint *)0x0;
          }
          else {
            uVar19 = CONCAT44(0x13e0792,(int)uVar19);
            FUN_010b9e20(0x2c);
            local_38c = (uint *)FUN_010baa10();
          }
          local_14._0_1_ = 1;
          local_244 = 9;
          uVar22 = 0x13e07d7;
          puVar10 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          FUN_00f19d60(s_TOOLTIP_ITEM_NOT_FOR_SALE_01af81d8);
          FUN_015ce480(local_3d8,uVar21,puVar8,piVar11,uVar19,uVar22);
          local_14._0_1_ = 0xac;
          uVar22 = FUN_00f19ea0();
          FUN_01250240(uVar22);
          local_14._0_1_ = 1;
          thunk_FUN_00f1a330();
          puVar18 = local_38c;
          puVar10 = &local_274;
          piVar11 = (int *)0x13e0855;
          FUN_01230c40(puVar10,DAT_01bee88c,DAT_01bee890);
          FUN_012338a0(puVar18);
        }
        else {
          uVar19 = CONCAT44(0x13e057b,puVar10);
          local_39c = (void *)FUN_016830c0(0x2c);
          local_14._0_1_ = 0xa8;
          if (local_39c == (void *)0x0) {
            local_38c = (uint *)0x0;
          }
          else {
            uVar19 = CONCAT44(0x13e0597,(int)uVar19);
            FUN_010b9e20(0x2c);
            local_38c = (uint *)FUN_010baa10();
          }
          local_14._0_1_ = 1;
          local_244 = 9;
          uVar22 = 0x13e05e1;
          puVar10 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar10;
          local_26c = *(undefined1 **)(puVar10 + 1);
          FUN_00f19d60(s_TOOLTIP_ITEM_SELLING_PRICE_01af8214);
          FUN_015ce480(local_3d8,uVar21,puVar8,piVar11,uVar19,uVar22);
          local_14._0_1_ = 0xa9;
          uVar21 = FUN_00f19ea0();
          FUN_01250240(uVar21);
          local_14._0_1_ = 1;
          thunk_FUN_00f1a330();
          iVar7 = FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
          *(undefined4 *)(iVar7 + 8) = 0x72;
          uVar22 = DAT_01bee890;
          uVar21 = DAT_01bee88c;
          puVar8 = (undefined8 *)FUN_00f4eeb0();
          puVar18 = local_38c;
          uVar25 = 0;
          uVar24 = 0;
          uVar9 = 2;
          uVar27 = 9;
          FUN_01231af0(1,*(undefined4 *)(local_398 + 0x50),0,*puVar8,*(undefined4 *)(puVar8 + 1),9,2
                       ,0,0,uVar21,uVar22);
          FUN_01232630(6);
          param_1 = local_3ac;
          uVar19 = CONCAT44(0x13e06d7,uVar25);
          FUN_012338a0(puVar18);
          local_244 = 9;
          uVar21 = 0x13e06fa;
          puVar8 = (undefined8 *)FUN_00f4eeb0();
          local_274 = *puVar8;
          local_26c = *(undefined1 **)(puVar8 + 1);
          FUN_00f19d60(s_TOOLTIP_ITEM_STORE_EXPLAIN_01af81bc);
          FUN_015ce480(local_3d8,uVar27,uVar9,uVar24,uVar19,uVar21);
          local_14._0_1_ = 0xaa;
          puVar10 = (undefined8 *)FUN_00f19ea0();
          piVar11 = (int *)0x118;
          puVar8 = (undefined8 *)(param_1 + 0x10);
          uVar21 = 0x13e075b;
          FUN_012547c0(puVar8,0x118,puVar10,&local_274,0xffffffff);
          local_14._0_1_ = 1;
          thunk_FUN_00f1a330();
        }
      }
    }
    if ((*(int *)(param_1 + 0x78) == 1) && (*(int *)(param_1 + 0x7c) == 0)) {
      uVar19 = CONCAT44(0x13e087b,puVar10);
      FUN_013c2850(1);
      local_244 = 9;
      uVar22 = 0x13e089e;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_TOOLTIP_ITEM_SLOT_HAS_EXPIRED_IN_01af8278);
      FUN_015ce480(local_294,uVar21,puVar8,piVar11,uVar19,uVar22);
      local_14 = CONCAT31(local_14._1_3_,0xad);
      uVar22 = FUN_00f19ea0();
      piVar11 = (int *)0x118;
      puVar8 = (undefined8 *)(param_1 + 0x10);
      uVar21 = 0x13e08ff;
      FUN_012547c0(puVar8,0x118,uVar22,&local_274,0xffffffff);
      goto LAB_013e0a6d;
    }
  }
  else if ((iVar12 == 0x47) && (iVar1 == 0)) {
    if ((*(int *)(param_1 + 0x78) == 1) && (*(int *)(param_1 + 0x7c) == 0)) {
      uVar19 = CONCAT44(0x13e0937,puVar10);
      FUN_013c2850(1);
      local_244 = 9;
      uVar22 = 0x13e095a;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_TOOLTIP_ITEM_SLOT_HAS_EXPIRED_WA_01af82a0);
      FUN_015ce480(local_304,uVar21,puVar8,piVar11,uVar19,uVar22);
      local_14 = CONCAT31(local_14._1_3_,0xae);
      uVar22 = FUN_00f19ea0();
      piVar11 = (int *)0x118;
      puVar8 = (undefined8 *)(param_1 + 0x10);
      uVar21 = 0x13e09bb;
      FUN_012547c0(puVar8,0x118,uVar22,&local_274,0xffffffff);
LAB_013e0a6d:
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
    }
  }
  else if ((iVar12 == 0x97) && (iVar1 == 0)) {
    uVar19 = CONCAT44(0x13e09e3,puVar10);
    FUN_013c2850(1);
    local_244 = 9;
    uVar22 = 0x13e0a06;
    puVar10 = (undefined8 *)FUN_00f4eeb0();
    local_274 = *puVar10;
    local_26c = *(undefined1 **)(puVar10 + 1);
    FUN_00f19d60(s_TOOLTIP_ITEM_CASH_WAREHOUSE_EXPL_01af8230);
    FUN_015ce480(local_2b8,uVar21,puVar8,piVar11,uVar19,uVar22);
    local_14 = CONCAT31(local_14._1_3_,0xaf);
    uVar22 = FUN_00f19ea0();
    piVar11 = (int *)0x118;
    puVar8 = (undefined8 *)(param_1 + 0x10);
    uVar21 = 0x13e0a67;
    FUN_012547c0(puVar8,0x118,uVar22,&local_274,0xffffffff);
    goto LAB_013e0a6d;
  }
  if (*(short *)(local_398 + 0x18) == 0x5d) {
    local_279 = '\0';
    FUN_014414a0();
    if (local_279 == '\x01') {
      local_29c = (uint *)0x0;
      FUN_014414a0();
      local_278 = (uint *)0x0;
      uVar22 = 0x13e0af4;
      FUN_014414a0();
      FUN_013c2850(1);
      uVar19 = CONCAT44(0x13e0b04,uVar22);
      local_39c = (void *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0xb0;
      if (local_39c == (void *)0x0) {
        local_38c = (uint *)0x0;
      }
      else {
        uVar19 = CONCAT44(0x13e0b20,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_38c = (uint *)FUN_010baa10();
      }
      local_14._0_1_ = 1;
      uVar22 = 0x13e0b60;
      puVar10 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar10;
      local_26c = *(undefined1 **)(puVar10 + 1);
      FUN_00f19d60(s_TOOLTIP_ITEM_WAITING_TO_BE_HATCH_01af8254);
      FUN_015ce480(local_3d8,uVar21,puVar8,piVar11,uVar19,uVar22);
      local_14._0_1_ = 0xb1;
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
      puVar18 = local_38c;
      uVar22 = 0x13e0bde;
      FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      FUN_012338a0(puVar18);
      local_394 = local_278;
      uVar27 = 0x13e0c0c;
      FUN_015db970();
      local_14._0_1_ = 0xb2;
      uVar19 = CONCAT44(0x13e0c17,uVar27);
      local_39c = (void *)FUN_016830c0(0x2c);
      local_14._0_1_ = 0xb3;
      if (local_39c == (void *)0x0) {
        local_38c = (uint *)0x0;
      }
      else {
        uVar19 = CONCAT44(0x13e0c33,(int)uVar19);
        FUN_010b9e20(0x2c);
        local_38c = (uint *)FUN_010baa10();
      }
      uVar27 = 0x13e0c57;
      FUN_00f1a3a0();
      puVar18 = local_29c;
      local_14._0_1_ = 0xb4;
      FUN_00f19d60(s_TOOLTIP_ITEM_HATCH_LEVEL_01af8308);
      FUN_015ce480(local_3d8,uVar21,puVar8,uVar22,uVar19,uVar27);
      local_14._0_1_ = 0xb5;
      uVar22 = FUN_00f19ea0();
      uVar27 = FUN_00f19ea0();
      uVar21 = 0x13e0cb0;
      FUN_01500f80(local_2d8,u__s____s__d_01af8324,uVar27,uVar22,puVar18);
      local_14._0_1_ = 0xb4;
      thunk_FUN_00f1a330();
      puVar8 = (undefined8 *)FUN_00f4eeb0();
      local_274 = *puVar8;
      local_26c = *(undefined1 **)(puVar8 + 1);
      uVar22 = FUN_00f19ea0();
      FUN_01250240(uVar22);
      puVar18 = local_38c;
      FUN_01230c40(&local_274,DAT_01bee88c,DAT_01bee890);
      param_1 = local_3ac;
      FUN_012338a0(puVar18);
      thunk_FUN_00f1a330();
      local_14._0_1_ = 1;
      thunk_FUN_00f1a330();
    }
  }
  if ((*(int *)(param_1 + 0x88) != 0) && (cVar3 = FUN_00f63990(), cVar3 != '\0')) {
    FUN_013c2850(1);
    FUN_0124fea0();
    local_14._0_1_ = 0xb6;
    puVar8 = (undefined8 *)FUN_00f4eeb0();
    uVar19 = *puVar8;
    uVar27 = 9;
    uVar22 = 0x13e0dc2;
    FUN_0124fef0(9,uVar19,*(undefined4 *)(puVar8 + 1));
    uVar19 = CONCAT44(0x13e0dc9,(int)uVar19);
    local_39c = (void *)FUN_016830c0(0x2c);
    local_14._0_1_ = 0xb7;
    if (local_39c == (void *)0x0) {
      local_38c = (uint *)0x0;
    }
    else {
      uVar19 = CONCAT44(0x13e0de5,(int)uVar19);
      FUN_010b9e20(0x2c);
      local_38c = (uint *)FUN_010baa10();
    }
    local_14._0_1_ = 0xb6;
    uVar9 = 0x13e0e20;
    puVar8 = (undefined8 *)FUN_00f4eeb0();
    local_388 = *puVar8;
    local_380 = *(undefined4 *)(puVar8 + 1);
    FUN_00f19d60(s_TOOLTIP_TRADE_LIMITED_MESSAGE_01af82c8);
    FUN_015ce480(local_3d8,uVar21,uVar22,uVar27,uVar19,uVar9);
    local_14._0_1_ = 0xb8;
    uVar21 = FUN_00f19ea0();
    FUN_01250240(uVar21);
    local_14._0_1_ = 0xb6;
    thunk_FUN_00f1a330();
    puVar18 = local_38c;
    FUN_01230c40(&local_388,DAT_01bee88c,DAT_01bee890);
    local_394 = puVar18;
    FUN_00f58160();
    local_35c = 0;
LAB_013e0ec5:
    FUN_00f1a330();
  }
LAB_013e0eca:
  FUN_00f1a330();
  local_248 = 0;
  FUN_00f1a330();
  ExceptionList = local_1c;
  return;
}


