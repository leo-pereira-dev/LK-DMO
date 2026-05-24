
/* ============================================================ */
/* Function: FUN_0134f9e0 */
/* Entry: 0134f9e0 */
/* Signature: undefined FUN_0134f9e0() */
/* ============================================================ */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0134f9e0(int param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  bool bVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined1 local_3c [24];
  undefined1 *local_24;
  undefined1 *puStack_20;
  undefined1 *local_1c;
  int local_18;
  void *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019a9955;
  local_10 = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  *(undefined2 *)(param_1 + 0x74) = 0x101;
  FUN_012457d0(s_SealMaster_sealmaster_window_tga_01af1d78,1,0,0,1,DAT_01bc07ec / 2 + -0x19a,
               DAT_01bc07f0 / 2 + -0x140,800,600);
  local_24 = &DAT_3f7ae148;
  puStack_20 = &DAT_3f7ae148;
  local_1c = &DAT_3f7ae148;
  uVar8 = 0;
  uVar10 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar9 = 0;
  FUN_00f19cc0(s_SEALMASTER_SEAL_TITLE_01af1dbc,0x15);
  puVar2 = (undefined4 *)FUN_015ce480(local_3c,uVar8,uVar10,uVar5,uVar6,uVar7,uVar9);
  local_8 = 0;
  if (7 < (uint)puVar2[5]) {
    puVar2 = (undefined4 *)*puVar2;
  }
  FUN_012461a0(puVar2,0xb,local_24,puStack_20,local_1c,0,0xd);
  local_8 = 0xffffffff;
  FUN_00f1a330(uVar1);
  iVar3 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,0x2f7,0xd,0x1e,0x1c,0,0x1c);
  *(int *)(param_1 + 0xb8) = iVar3;
  if (iVar3 != 0) {
    FUN_01356ef0(1,param_1,&LAB_01354100,0);
  }
  local_18 = 0x4f;
  local_14 = (void *)0x0;
  do {
    iVar3 = 0xf0;
    while( true ) {
      FUN_01246720(s_SealMaster_Sealmaster_bg_00_tga_01af1d9c,1,iVar3,local_18,0x66,0xad);
      iVar3 = iVar3 + 0x66;
      bVar4 = local_14 == (void *)0x4;
      local_14 = (void *)((int)local_14 + 1);
      if (bVar4) break;
      if (9 < (int)local_14) {
        uVar10 = 0x94;
        uVar8 = 0x134fb74;
        local_14 = (void *)FUN_016830c0(0x94);
        local_8 = 1;
        if (local_14 == (void *)0x0) {
          iVar3 = 0;
        }
        else {
          uVar8 = 0;
          memset(local_14,0,0x94);
          uVar10 = 0x134fb9e;
          iVar3 = FUN_012360f0();
        }
        local_8 = 0xffffffff;
        *(int *)(param_1 + 0xac) = iVar3;
        if (iVar3 != 0) {
          FUN_012363b0(*(undefined4 *)(param_1 + 0x40),0x17,0x3d,0xca,0x1d6,uVar8,uVar10);
          iVar3 = *(int *)(param_1 + 0xac);
          *(undefined4 *)(iVar3 + 0x74) = 0;
          *(undefined4 *)(iVar3 + 0x78) = 0;
          FUN_01356ef0(5,param_1,FUN_01350f40,0);
          FUN_01356ef0(2,param_1,FUN_01351020,0);
          *(undefined4 *)(*(int *)(param_1 + 0xac) + 0x8c) = 0x1f;
          *(undefined1 *)(*(int *)(param_1 + 0xac) + 0x91) = 1;
          local_14 = (void *)FUN_016830c0(0xec);
          local_8 = 2;
          if (local_14 == (void *)0x0) {
            iVar3 = 0;
          }
          else {
            memset(local_14,0,0xec);
            iVar3 = FUN_01247d90();
          }
          local_8 = 0xffffffff;
          if (iVar3 != 0) {
            FUN_01248090(5,0,2,0,0,0,0x10,0x1d6,0x10,0xf,_DAT_01b03170,uRam01b03174,uRam01b03178,
                         uRam01b0317c);
            *(undefined1 *)(iVar3 + 0xdc) = 1;
            FUN_012363e0(iVar3);
          }
          FUN_012472c0(*(undefined4 *)(param_1 + 0xac));
          FUN_013510f0();
          FUN_01351620();
          FUN_01351b50();
          FUN_01350330();
        }
        local_14 = (void *)FUN_016830c0(0xac);
        local_8 = 3;
        if (local_14 == (void *)0x0) {
          iVar3 = 0;
          uVar8 = extraout_ECX;
        }
        else {
          memset(local_14,0,0xac);
          iVar3 = FUN_01228950();
          uVar8 = extraout_ECX_00;
        }
        local_8 = 0xffffffff;
        *(int *)(param_1 + 0xb0) = iVar3;
        if (iVar3 != 0) {
          FUN_01228c50(*(undefined4 *)(param_1 + 0x40),0xf8,0x5e,0x208,0x150,0x10,0x10,0x56,0x98,0,2
                       ,0,uVar8,0);
          *(undefined1 *)(*(int *)(param_1 + 0xb0) + 0x94) = 0;
          local_14 = (void *)FUN_016830c0(0xec);
          local_8 = 4;
          if (local_14 == (void *)0x0) {
            iVar3 = 0;
          }
          else {
            memset(local_14,0,0xec);
            iVar3 = FUN_01247d90();
          }
          local_8 = 0xffffffff;
          if (iVar3 != 0) {
            FUN_01248090(5,0,2,0,0,0,0x10,0x16a,0x10,0xf,_DAT_01b03170,uRam01b03174,uRam01b03178,
                         uRam01b0317c);
            *(undefined1 *)(iVar3 + 0xdc) = 1;
            FUN_0122a200(iVar3);
          }
          FUN_01356ef0(5,param_1,&LAB_01353ad0,0);
          FUN_01356ef0(10,param_1,FUN_01353b10,0);
          FUN_01356ef0(6,param_1,FUN_01353c00,0);
          FUN_012472c0(*(undefined4 *)(param_1 + 0xb0));
        }
        FUN_01352080();
        FUN_01354110();
        FUN_01354bb0();
        *(undefined1 *)(param_1 + 0xf9) = 1;
        if (*(int *)(param_1 + 0xb0) != 0) {
          FUN_0122b110(&LAB_01356420);
          FUN_0122aa80(0);
          FUN_0122a600();
        }
        FUN_01353c90();
        FUN_01356910();
        ExceptionList = local_10;
        return;
      }
    }
    local_18 = local_18 + 0xa8;
  } while( true );
}


