
/* ============================================================ */
/* Function: FUN_011dc210 */
/* Entry: 011dc210 */
/* Signature: undefined FUN_011dc210() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011dc210(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  short ****ppppsVar7;
  undefined4 uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  short ***local_d4;
  undefined4 uStack_d0;
  undefined4 uStack_cc;
  undefined4 uStack_c8;
  undefined8 local_c4;
  short ***local_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  undefined4 uStack_b0;
  undefined8 local_ac;
  int local_a4;
  undefined8 local_a0;
  undefined1 *local_98;
  int local_90;
  undefined8 local_8c;
  undefined1 *local_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined1 local_68;
  undefined4 local_64;
  int local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  short ***pppsStack_48;
  undefined4 uStack_44;
  undefined4 local_40;
  undefined4 local_3c;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01993a84;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_90 = 0;
  local_a4 = param_1;
  local_98 = (undefined1 *)FUN_016830c0(0xa0,local_24);
  local_14 = 0;
  if (local_98 == (void *)0x0) {
    local_90 = 0;
  }
  else {
    memset(local_98,0,0xa0);
    local_90 = FUN_0123de10();
  }
  uVar10 = DAT_01bb9b34;
  local_14 = 0xffffffff;
  if (local_90 != 0) {
    uVar2 = *(undefined4 *)(param_1 + 0x40);
    local_a0 = DAT_01bb9b2c;
    FUN_00f19f10();
    local_40 = 0;
    uStack_4c = 0;
    pppsStack_48 = (short ***)0x0;
    uStack_44 = 0;
    local_3c = 0xf;
    local_50 = 0;
    local_14 = 1;
    FUN_015010a0(&local_50,&DAT_01ae7290,&DAT_01bfbd40,s_DetailInfo_Storage_M_field_a_tga_01ae2bbc);
    FUN_0124ef30(&local_50);
    iVar4 = local_90;
    uVar8 = 0;
    uVar9 = local_a0;
    uVar5 = FUN_0124f150(0,local_a0,uVar10);
    FUN_0123e330(uVar2,0xb,10,0xf3,0x2f,uVar5,uVar8,uVar9,uVar10);
    local_14 = 0xffffffff;
    FUN_00f1a420();
    iVar3 = local_a4;
    FUN_012472c0(iVar4);
    local_98 = (undefined1 *)FUN_016830c0(0xa0);
    local_14 = 2;
    if (local_98 == (void *)0x0) {
      local_90 = 0;
    }
    else {
      memset(local_98,0,0xa0);
      local_90 = FUN_0123de10();
    }
    uVar10 = DAT_01bb9b34;
    local_14 = 0xffffffff;
    if (local_90 != 0) {
      uVar2 = *(undefined4 *)(iVar3 + 0x40);
      local_a0 = DAT_01bb9b2c;
      FUN_00f19f10();
      local_40 = 0;
      uStack_4c = 0;
      pppsStack_48 = (short ***)0x0;
      uStack_44 = 0;
      local_3c = 0xf;
      local_50 = 0;
      local_14 = 3;
      FUN_015010a0(&local_50,&DAT_01ae7290,&DAT_01bfbd40,s_DetailInfo_TamerStatus_Digimon_T_01ae2b90
                  );
      FUN_0124ef30(&local_50);
      iVar4 = local_90;
      uVar8 = 0;
      uVar9 = local_a0;
      uVar5 = FUN_0124f150(0,local_a0,uVar10);
      FUN_0123e330(uVar2,100,0x3d,0x41,0x41,uVar5,uVar8,uVar9,uVar10);
      local_14 = 0xffffffff;
      FUN_00f1a420();
      iVar3 = local_a4;
      FUN_012472c0(iVar4);
      uVar10 = DAT_01bb9b34;
      local_90 = *(undefined4 *)(*(int *)(iVar3 + 0x78) + 0x58);
      local_a0 = DAT_01bb9b2c;
      FUN_015bfd90();
      local_14 = 4;
      ppppsVar7 = &local_d4;
      if (0xf < local_c4._4_4_) {
        ppppsVar7 = (short ****)local_d4;
      }
      FUN_01246800(ppppsVar7,ppppsVar7,local_a0,uVar10,0,0x68,0x41,0x39,0x39);
      FUN_00f1a420();
      local_70 = 0;
      local_6c = 7;
      uStack_7c = 0;
      uStack_78 = 0;
      uStack_74 = 0;
      local_80 = 0;
      local_54 = 0;
      local_14 = 5;
      local_60 = DAT_01bfe468 + 8;
      local_98 = &DAT_3f7ae148;
      local_84 = &DAT_3f7ae148;
      local_8c = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_50 = DAT_01b02744;
      uStack_4c = 0;
      pppsStack_48 = (short ***)0x0;
      uStack_44 = 0;
      local_5c = 0xb;
      local_68 = 1;
      local_58 = 0;
      local_64 = 1;
      FUN_00f221a0(*(int *)(iVar3 + 0x78) + 0x3c);
      local_14._0_1_ = 6;
      ppppsVar7 = &local_d4;
      if (7 < local_c4._4_4_) {
        ppppsVar7 = (short ****)local_d4;
      }
      do {
        sVar1 = *(short *)ppppsVar7;
        ppppsVar7 = (short ****)((int)ppppsVar7 + 2);
      } while (sVar1 != 0);
      FUN_00f1a040();
      local_14._0_1_ = 5;
      FUN_00f1a330();
      FUN_01246bb0(&local_8c,1,0x84,0x18);
      local_60 = DAT_01bfe468 + 8;
      local_8c = CONCAT44(local_50,local_50);
      local_84 = &DAT_3f7ae148;
      local_5c = 0xb;
      local_68 = 1;
      local_58 = 0;
      local_64 = 1;
      local_14._0_1_ = 7;
      uStack_b8 = 0;
      uStack_b4 = 0;
      uStack_b0 = 0;
      local_ac._0_4_ = 0;
      local_ac._4_4_ = 7;
      local_bc = (short ***)0x0;
      local_98 = (undefined1 *)(uint)*(ushort *)(*(int *)(iVar3 + 0x78) + 0x54);
      local_90 = 7;
      FUN_015011b0();
      FUN_00f19cc0(s_COMMON_TXT_SHORT_LEVEL_01ac5990,0x16);
      FUN_015ce480();
      local_14._0_1_ = 8;
      puVar6 = (undefined4 *)FUN_00f53390();
      local_d4 = (short ***)*puVar6;
      uStack_d0 = puVar6[1];
      uStack_cc = puVar6[2];
      uStack_c8 = puVar6[3];
      local_c4 = *(undefined8 *)(puVar6 + 4);
      puVar6[4] = 0;
      puVar6[5] = 7;
      *(undefined2 *)puVar6 = 0;
      local_90 = 0xf;
      local_a0 = local_c4;
      pppsStack_48 = local_d4;
      uStack_44 = uStack_d0;
      local_40 = uStack_cc;
      local_3c = uStack_c8;
      FUN_00f1a330();
      local_c4 = 0x700000000;
      local_bc = pppsStack_48;
      uStack_b8 = uStack_44;
      uStack_b4 = local_40;
      uStack_b0 = local_3c;
      local_d4 = (short ***)((uint)local_d4 & 0xffff0000);
      local_ac = local_a0;
      FUN_00f1a330();
      local_14._0_1_ = 7;
      FUN_00f1a330();
      ppppsVar7 = &local_bc;
      if (7 < local_ac._4_4_) {
        ppppsVar7 = (short ****)local_bc;
      }
      do {
        sVar1 = *(short *)ppppsVar7;
        ppppsVar7 = (short ****)((int)ppppsVar7 + 2);
      } while (sVar1 != 0);
      FUN_00f1a040();
      local_14 = CONCAT31(local_14._1_3_,5);
      FUN_00f1a330();
      FUN_01246bb0(&local_8c,1,0x84,0x82);
      local_60 = 0;
      FUN_00f1a330();
    }
  }
  ExceptionList = local_1c;
  return;
}


