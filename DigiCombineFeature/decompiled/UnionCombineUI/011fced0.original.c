
/* ============================================================ */
/* Function: FUN_011fced0 */
/* Entry: 011fced0 */
/* Signature: undefined FUN_011fced0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_011fced0(undefined4 *param_1,int param_2)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined1 uVar6;
  uint uVar7;
  undefined4 *puVar8;
  void *pvVar9;
  int iVar10;
  undefined4 *_Dst;
  int *piVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined8 uVar14;
  undefined4 uVar15;
  int local_60;
  void *local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 *local_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  int *piStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01991d08;
  local_1c = ExceptionList;
  uVar7 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if ((param_2 != 0) &&
     (local_24 = uVar7, puVar8 = (undefined4 *)FUN_016830c0(0x28,uVar7), puVar8 != (undefined4 *)0x0
     )) {
    *puVar8 = 0;
    puVar8[1] = 0;
    puVar8[2] = 0;
    puVar8[3] = 0;
    puVar8[4] = 0;
    puVar8[5] = 0;
    puVar8[6] = 0;
    puVar8[7] = 0;
    *(undefined8 *)(puVar8 + 8) = 0;
    *puVar8 = &DAT_01ae66e0;
    puVar8[1] = 0;
    puVar8[2] = 0;
    puVar8[3] = 0xffffffff;
    *(undefined1 *)(puVar8 + 4) = 0;
    uVar15 = DAT_01bee890;
    puVar8[5] = DAT_01bee88c;
    puVar8[6] = uVar15;
    *(undefined2 *)(puVar8 + 7) = 0x101;
    puVar8[8] = 0;
    puVar8[9] = 0;
    pvVar9 = (void *)FUN_016830c0(0x2c,uVar7);
    local_14 = 0;
    if (pvVar9 == (void *)0x0) {
      iVar10 = 0;
    }
    else {
      memset(pvVar9,0,0x2c);
      iVar10 = FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (iVar10 != 0) {
      local_4c = (void *)0x0;
      local_48 = 0;
      local_4c = operator_new(0x24);
      *(void **)local_4c = local_4c;
      *(void **)((int)local_4c + 4) = local_4c;
      *(void **)((int)local_4c + 8) = local_4c;
      *(undefined2 *)((int)local_4c + 0xc) = 0x101;
      local_44 = 0;
      local_40 = (undefined4 *)0x0;
      local_14 = 1;
      _Dst = (undefined4 *)FUN_016830c0();
      local_14._0_1_ = 2;
      if (_Dst == (undefined4 *)0x0) {
        _Dst = (undefined4 *)0x0;
      }
      else {
        memset(_Dst,0,0xbc);
        FUN_0123de10();
        *_Dst = &DAT_01ae721c;
        *(undefined2 *)(_Dst + 0x28) = 0;
        *(undefined1 *)((int)_Dst + 0xa2) = 0;
        _Dst[0x29] = 1;
        _Dst[0x2a] = 1;
        _Dst[0x2b] = 0xffffffff;
        _Dst[0x2c] = 0x3f800000;
        _Dst[0x2d] = 0x3f800000;
        _Dst[0x2e] = 0;
      }
      local_14._0_1_ = 1;
      if (_Dst != (undefined4 *)0x0) {
        FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x30,0x44,s_CommonUI_arrow_Right_tga_01ad6e94,0,1);
        FUN_0123d9a0();
        piVar11 = (int *)FUN_01231700(_Dst,0,0xfffffff5,0x12,0x30,0x44);
        if (piVar11 != (int *)0x0) {
          (**(code **)(*piVar11 + 0x54))(1);
        }
      }
      pvVar9 = (void *)FUN_016830c0(0xa0);
      local_14._0_1_ = 3;
      if (pvVar9 == (void *)0x0) {
        local_60 = 0;
      }
      else {
        memset(pvVar9,0,0xa0);
        local_60 = FUN_0123de10();
      }
      uVar5 = DAT_01bee890;
      uVar4 = DAT_01bee88c;
      uVar15 = DAT_01bb9b34;
      uVar14 = DAT_01bb9b2c;
      local_14._0_1_ = 1;
      uVar6 = (undefined1)local_14;
      local_14._0_1_ = 1;
      if (local_60 != 0) {
        FUN_00f19f10();
        local_2c = 0;
        uStack_38 = 0;
        piStack_34 = (int *)0x0;
        uStack_30 = 0;
        local_28 = (undefined4 *)0xf;
        local_3c = 0;
        local_14._0_1_ = 4;
        FUN_015010a0(&local_3c,&DAT_01ae7290,&DAT_01bfbd40,
                     s_DigiCombine_DigiCombine_CardBack_01ae1a84);
        FUN_0124ef30(&local_3c);
        uVar13 = 0;
        uVar12 = FUN_0124f150(0,uVar14,uVar15);
        FUN_0123e330(0,uVar4,uVar5,0x43,0x5f,uVar12,uVar13,uVar14,uVar15);
        local_14._0_1_ = 1;
        FUN_00f1a420();
        piVar11 = (int *)FUN_01230f40(local_60,0x2c,7,0x43,0x5f);
        uVar6 = (undefined1)local_14;
        if (piVar11 != (int *)0x0) {
          (**(code **)(*piVar11 + 0x54))(1);
          uVar6 = (undefined1)local_14;
        }
      }
      local_14._0_1_ = uVar6;
      FUN_012288c0(iVar10);
      uStack_38 = *param_1;
      piStack_34 = (int *)0x0;
      uStack_30 = 0;
      local_40 = puVar8;
      piStack_34 = operator_new(0x24);
      *piStack_34 = (int)piStack_34;
      piStack_34[1] = (int)piStack_34;
      piStack_34[2] = (int)piStack_34;
      *(undefined2 *)(piStack_34 + 3) = 0x101;
      local_14._0_1_ = 5;
      iVar10 = FUN_012003e0();
      piStack_34[1] = iVar10;
      uStack_30 = local_48;
      piVar11 = (int *)piStack_34[1];
      if (*(char *)((int)piVar11 + 0xd) == '\0') {
        cVar1 = *(char *)(*piVar11 + 0xd);
        piVar2 = (int *)*piVar11;
        while (cVar1 == '\0') {
          cVar1 = *(char *)(*piVar2 + 0xd);
          piVar11 = piVar2;
          piVar2 = (int *)*piVar2;
        }
        *piStack_34 = (int)piVar11;
        iVar10 = *(int *)(piStack_34[1] + 8);
        cVar1 = *(char *)(iVar10 + 0xd);
        iVar3 = piStack_34[1];
        while (cVar1 == '\0') {
          cVar1 = *(char *)(*(int *)(iVar10 + 8) + 0xd);
          iVar3 = iVar10;
          iVar10 = *(int *)(iVar10 + 8);
        }
        piStack_34[2] = iVar3;
      }
      else {
        *piStack_34 = (int)piStack_34;
        piStack_34[2] = (int)piStack_34;
      }
      local_2c = local_44;
      local_28 = local_40;
      local_14._0_1_ = 6;
      FUN_011c68f0();
      local_14 = CONCAT31(local_14._1_3_,1);
      FUN_010b4670(&piStack_34);
      FUN_0122a090(puVar8);
      FUN_010b4670(&local_4c);
    }
  }
  ExceptionList = local_1c;
  return;
}


