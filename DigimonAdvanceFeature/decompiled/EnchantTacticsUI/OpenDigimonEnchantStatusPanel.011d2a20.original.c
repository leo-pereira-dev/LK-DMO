
/* ============================================================ */
/* Function: FUN_011d2a20 */
/* Entry: 011d2a20 */
/* Signature: undefined FUN_011d2a20() */
/* ============================================================ */


void __thiscall
FUN_011d2a20(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6)

{
  int iVar1;
  undefined8 local_30;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  int iStack_1c;
  code *pcStack_18;
  undefined4 uStack_14;
  
  uStack_14 = param_6;
  pcStack_18 = (code *)param_5;
  iStack_1c = param_4;
  uStack_20 = param_3;
  uStack_24 = 1;
  local_28 = 0;
  local_30 = 1;
  FUN_012459a0(param_2,0);
  uStack_14 = 0x263;
  pcStack_18 = (code *)0xfd;
  iStack_1c = 0xfffffff8;
  uStack_20 = 4;
  uStack_24 = 1;
  local_30 = DAT_01bb9b2c;
  local_28 = DAT_01bb9b34;
  FUN_01246800(s_DetailInfo_TamerStatus_Win_03_bg_01ae2b68,&local_30);
  uStack_14 = 0x135;
  pcStack_18 = (code *)0xa2;
  iStack_1c = 0x58;
  uStack_20 = 0x112;
  uStack_24 = 1;
  local_30 = DAT_01bb9b2c;
  local_28 = DAT_01bb9b34;
  FUN_01246800(s_DetailInfo_Digimon_Status_Win_tg_01ae2b44,&local_30);
  uStack_14 = 0x11d2abe;
  FUN_011d2b30();
  uStack_14 = 0x11d2ac5;
  FUN_011d3690();
  uStack_14 = 0x11d2acc;
  FUN_011d3b00();
  uStack_14 = 0x11d2ad3;
  FUN_011d30b0();
  uStack_14 = 0x11d2ada;
  FUN_011d4320();
  uStack_14 = 0x11d2ae1;
  FUN_011d4bd0();
  uStack_14 = param_2;
  pcStack_18 = (code *)0x11d2aeb;
  FUN_011d50f0();
  uStack_14 = 0x1a;
  pcStack_18 = (code *)0x0;
  iStack_1c = 0x1a;
  uStack_20 = 0x1a;
  uStack_24 = 0x3e;
  local_28 = 0x19a;
  local_30 = 0x100000017;
  iVar1 = FUN_012462c0(s_DetailInfo_zoom_in_aa_tga_01ae2cc8);
  *(int *)(param_1 + 0x94) = iVar1;
  if (iVar1 != 0) {
    uStack_14 = 0;
    pcStack_18 = FUN_011d5d40;
    uStack_24 = 0x11d2b24;
    iStack_1c = param_1;
    FUN_011d5e00();
  }
  return;
}


