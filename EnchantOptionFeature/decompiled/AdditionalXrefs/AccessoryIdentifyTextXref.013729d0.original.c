
/* ============================================================ */
/* Function: FUN_013729d0 */
/* Entry: 013729d0 */
/* Signature: undefined FUN_013729d0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_013729d0(int param_1)

{
  short sVar1;
  int iVar2;
  char cVar3;
  uint uVar4;
  short *psVar5;
  int *piVar6;
  undefined4 *puVar7;
  short *psVar8;
  float10 fVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined1 local_7c [8];
  undefined4 local_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  void *local_64;
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
  int local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199af47;
  local_1c = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_24 = uVar4;
  FUN_012457d0(0,0,0,0,1,(DAT_01bc07ec + -0xc0) / 2,
               (int)((float)(DAT_01bc07f0 + -0x3e) * DAT_01b026d0),0xc0,0x3e);
  local_44 = 0;
  local_40 = 7;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  local_54 = 0;
  local_28 = 0;
  local_14 = 0;
  local_34 = DAT_01bfe468 + 8;
  uStack_68 = 0x3dcccccd;
  local_30 = 0xc;
  local_60 = CONCAT44(DAT_01b0256c,DAT_01b02744);
  local_58 = 0x3dcccccd;
  local_3c = 1;
  local_2c = 0;
  local_38 = 1;
  uVar10 = 0;
  uVar11 = 0;
  uVar12 = 0;
  uVar13 = 0;
  uVar14 = 0;
  uVar15 = 0;
  FUN_00f19cc0(s_ACCESSORY_IDENTIFY_IDENTIFYING_01ae82cc,0x1e);
  psVar5 = (short *)FUN_015ce480(local_7c,uVar10,uVar11,uVar12,uVar13,uVar14,uVar15);
  local_14._0_1_ = 1;
  if (7 < *(uint *)(psVar5 + 10)) {
    psVar5 = *(short **)psVar5;
  }
  psVar8 = psVar5;
  do {
    sVar1 = *psVar8;
    psVar8 = psVar8 + 1;
  } while (sVar1 != 0);
  FUN_00f1a040(psVar5,(int)psVar8 - (int)(psVar5 + 1) >> 1);
  local_14 = (uint)local_14._1_3_ << 8;
  FUN_00f1a330(uVar4);
  FUN_01246bb0(&local_60,1,0x60,0);
  local_14 = 0xffffffff;
  local_34 = 0;
  FUN_00f1a330();
  local_64 = (void *)FUN_016830c0(0x1ac);
  local_14 = 2;
  if (local_64 == (void *)0x0) {
    piVar6 = (int *)0x0;
  }
  else {
    memset(local_64,0,0x1ac);
    piVar6 = (int *)FUN_012435e0();
  }
  local_14 = 0xffffffff;
  *(int **)(param_1 + 0xb0) = piVar6;
  if (piVar6 != (int *)0x0) {
    (**(code **)(*piVar6 + 0xc))(*(undefined4 *)(param_1 + 0x40),0,0x14,0xc0,0x20,0);
    FUN_01377e70(1,param_1,&LAB_0125f6d0,0);
    FUN_012426f0(s_Accessory_Identify_Identify_BarB_01ae8310);
    FUN_012427e0(s_Accessory_Identify_Identify_Bar__01ae82ec);
    FUN_01242970(s_Accessory_Identify_Identify_BarM_01ae8354);
    iVar2 = *(int *)(param_1 + 0xb0);
    *(undefined4 *)(iVar2 + 0xc0) = 0;
    *(undefined4 *)(iVar2 + 0xdc) = 0;
    uStack_68 = uRam01b02ddc;
    uStack_6c = uRam01b02dd8;
    uStack_70 = uRam01b02dd4;
    local_74 = _DAT_01b02dd0;
    puVar7 = (undefined4 *)(iVar2 + 0xb0);
    if (7 < *(uint *)(iVar2 + 0xc4)) {
      puVar7 = (undefined4 *)*puVar7;
    }
    *(undefined2 *)puVar7 = 0;
    FUN_01242ab0(&local_74);
    *(undefined4 *)(*(int *)(param_1 + 0xb0) + 0xd4) = 0x3c23d70a;
    piVar6 = *(int **)(param_1 + 0xb0);
    piVar6[0x3b] = 1;
    piVar6[0x3c] = 0;
    fVar9 = (float10)fmaxf(0,0x3fd56042);
    piVar6[0x32] = (int)(float)fVar9;
    FUN_01243010();
    (**(code **)(*piVar6 + 0x48))();
    FUN_01242bf0();
    *(undefined1 *)(*(int *)(param_1 + 0xb0) + 0xe4) = 0;
    FUN_012472c0(*(undefined4 *)(param_1 + 0xb0));
  }
  if (DAT_01bfb32c != (int *)0x0) {
    cVar3 = (**(code **)(*DAT_01bfb32c + 4))(s_System_System_return_01_wav_01ae8338);
    if (cVar3 != '\0') {
      (**(code **)(*DAT_01bfb32c + 8))(s_System_System_return_01_wav_01ae8338);
    }
    (**(code **)*DAT_01bfb32c)(s_System_System_return_01_wav_01ae8338);
  }
  ExceptionList = local_1c;
  return;
}


