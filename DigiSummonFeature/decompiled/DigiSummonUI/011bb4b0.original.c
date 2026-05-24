
/* ============================================================ */
/* Function: FUN_011bb4b0 */
/* Entry: 011bb4b0 */
/* Signature: undefined FUN_011bb4b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 __fastcall FUN_011bb4b0(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 local_3c [24];
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  void *local_18;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019914cf;
  local_10 = ExceptionList;
  local_14 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  cVar1 = FUN_011b41c0(local_14);
  if (cVar1 == '\0') {
    ExceptionList = local_10;
    return 0;
  }
  local_18 = (void *)0x1;
  FUN_00ff8bc0(&local_18,-(uint)(param_1 != 0) & param_1 + 0x74U);
  FUN_012457d0(0,0,3,3,1,DAT_01bee88c,DAT_01bee890,0x400,0x300);
  uVar2 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_DIGISUMMON_TITLE_01ae1f98,0x10);
  uVar2 = FUN_015ce480(local_3c,uVar2,uVar4,uVar5,uVar6,uVar7,uVar8);
  local_8 = 0;
  FUN_011bba60(uVar2);
  local_8 = 0xffffffff;
  FUN_00f1a330();
  iVar3 = FUN_012462c0(s_Random_box_button_arrow_l_tga_01adae40,4,1,0xf,0x14b,0x49,0x2e,0,0x5c);
  *(int *)(param_1 + 0x88) = iVar3;
  if (iVar3 != 0) {
    FUN_011bd920(1,param_1,&LAB_011bd2f0,0);
  }
  iVar3 = FUN_012462c0(s_Random_box_button_arrow_r_tga_01adae20,4,1,0x3a8,0x14b,0x49,0x2e,0,0x5c);
  *(int *)(param_1 + 0x8c) = iVar3;
  if (iVar3 != 0) {
    FUN_011bd920(1,param_1,&LAB_011bd310,0);
  }
  if (*(int *)(param_1 + 0x84) == 0) {
    local_18 = (void *)FUN_016830c0(0xac);
    local_8 = 1;
    if (local_18 == (void *)0x0) {
      iVar3 = 0;
      uVar2 = extraout_ECX;
    }
    else {
      memset(local_18,0,0xac);
      iVar3 = FUN_01228950();
      uVar2 = extraout_ECX_00;
    }
    local_8 = 0xffffffff;
    *(int *)(param_1 + 0x84) = iVar3;
    if (iVar3 == 0) goto LAB_011bb748;
    FUN_01228bd0(*(undefined4 *)(param_1 + 0x40),0x7a,0x96,0x30c,400,0xf,0,0xfa,400,0,2,uVar2,3);
    local_1c = 0x103;
    local_18 = (void *)0x19d;
    local_24 = 0xfffffffc;
    local_20 = 0xfffffff9;
    FUN_01228eb0(s_Random_box_over_frame_png_01adae78,&local_24,&local_1c);
    *(undefined1 *)(*(int *)(param_1 + 0x84) + 0x94) = 0;
    FUN_011bd920(3,param_1,FUN_011bd1e0,0);
    FUN_011bd920(10,param_1,&LAB_011bd370,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0x84));
  }
  else {
    FUN_0122a180();
    iVar3 = *(int *)(param_1 + 0x7c);
    FUN_011b6c40(param_1 + 0x7c,*(undefined4 *)(iVar3 + 4));
    *(int *)(iVar3 + 4) = iVar3;
    *(int *)iVar3 = iVar3;
    *(int *)(iVar3 + 8) = iVar3;
    *(undefined4 *)(param_1 + 0x80) = 0;
  }
  local_18 = (void *)0x0;
  do {
    FUN_011bbb20(&local_18);
    local_18 = (void *)((int)local_18 + 1);
  } while ((int)local_18 < 3);
LAB_011bb748:
  FUN_011bc4e0();
  FUN_01130f50(extraout_ECX_01);
  ExceptionList = local_10;
  return 1;
}


