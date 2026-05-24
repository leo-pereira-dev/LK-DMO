
/* ============================================================ */
/* Function: FUN_011b6e50 */
/* Entry: 011b6e50 */
/* Signature: undefined FUN_011b6e50() */
/* ============================================================ */


undefined1 __fastcall FUN_011b6e50(int *param_1)

{
  char cVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 extraout_ECX;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 local_38 [24];
  undefined1 *local_20;
  undefined1 *puStack_1c;
  undefined4 local_18;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01987ced;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  cVar1 = (**(code **)(*param_1 + 0x20))(DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  if (cVar1 == '\0') {
    ExceptionList = local_10;
    return 0;
  }
  FUN_012457d0(0,0,3,3,1,0x70,0x54,800,600);
  local_14 = 0x3f800000;
  FUN_01247160(s_Random_box_popup_rate_popup_rate_01adaa58,&DAT_01ad33f4,&local_14,extraout_ECX,0x56
               ,0x56,0,0x1c,0,0xffffffe4);
  local_20 = &DAT_3f7ae148;
  puStack_1c = &DAT_3f7ae148;
  local_18 = 0;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar8 = 0;
  FUN_00f19cc0(s_RANDOM_BOX_TXT_RATEINFO_01adaa9c,0x17);
  puVar2 = (undefined4 *)FUN_015ce480(local_38,uVar4,uVar5,uVar6,uVar7,uVar8);
  local_8 = 0;
  if (7 < (uint)puVar2[5]) {
    puVar2 = (undefined4 *)*puVar2;
  }
  FUN_012461a0(puVar2,0x13,CONCAT44(puStack_1c,local_20),local_18,0,10);
  local_8 = 0xffffffff;
  FUN_00f1a330();
  iVar3 = FUN_012462c0(s_CommonUI_CommonUI_close_tga_01ad3144,0x17,1,0x2fd,5,0x1e,0x1c,0,0x1c);
  param_1[0x20] = iVar3;
  FUN_011b71a0();
  FUN_011b7390();
  ExceptionList = local_10;
  return 1;
}


