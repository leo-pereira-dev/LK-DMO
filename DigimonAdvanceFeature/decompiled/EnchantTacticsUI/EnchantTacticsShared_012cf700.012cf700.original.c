
/* ============================================================ */
/* Function: FUN_012cf700 */
/* Entry: 012cf700 */
/* Signature: undefined FUN_012cf700() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_012cf700(int param_1)

{
  uint uVar1;
  short sVar2;
  int iVar3;
  void *pvVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 auStack_3c [4];
  int local_38;
  undefined1 local_34 [28];
  undefined8 local_18;
  undefined4 local_10;
  uint local_c;
  
  iVar5 = DAT_01bfb2b4;
  local_c = DAT_01bbb1c0 ^ (uint)auStack_3c;
  *(undefined1 *)(param_1 + 8) = 1;
  *(undefined1 *)(param_1 + 0x180) = 0;
  if (*(int *)(param_1 + 0x164) != 0) {
    FUN_00f70840(*(undefined4 *)(param_1 + 0x168));
  }
  if (*(int *)(param_1 + 0x16c) != 0) {
    FUN_00f70840(*(undefined4 *)(param_1 + 0x170));
  }
  if (*(int *)(param_1 + 0x174) != 0) {
    uVar6 = 0;
    uVar7 = 0;
    uVar8 = 0;
    uVar9 = 0;
    uVar10 = 0;
    uVar11 = 0;
    uVar12 = 0;
    FUN_00f65090(0,0);
    FUN_013f3620(uVar6,uVar7,uVar8,uVar9,uVar10,uVar11,uVar12);
    iVar5 = DAT_01bfb2b4;
  }
  if (*(undefined4 **)(param_1 + 0x164) == (undefined4 *)0x0) {
    local_10 = 0;
  }
  else {
    local_10 = **(undefined4 **)(param_1 + 0x164);
  }
  local_18 = 0;
  if (((*(int *)(param_1 + 0x198) != 0) &&
      (iVar3 = FUN_015db910(&local_10), iVar5 = DAT_01bfb2b4, iVar3 != 0)) &&
     (*(int *)(param_1 + 0x19c) != 0)) {
    local_38 = (uint)*(ushort *)(iVar3 + 0x18) * 100 + (uint)*(ushort *)(iVar3 + 0x1a);
    local_18 = FUN_0161e6e0(&local_38);
    iVar5 = DAT_01bfb2b4;
  }
  if (iVar5 == 0) {
    iVar5 = 0;
  }
  else {
    iVar5 = iVar5 + 0x28;
    if (iVar5 != 0) {
      FUN_00f61da0(local_34,(int)local_18,local_18._4_4_);
    }
  }
  if ((local_18._4_4_ != 0) || ((int)local_18 != 0)) {
    FUN_014414a0(0x296,&local_18);
  }
  if (iVar5 != 0) {
    if (*(uint *)(param_1 + 0x168) < 200) {
      pvVar4 = (void *)(*(uint *)(param_1 + 0x168) * 0x45 + 0x30 + iVar5);
    }
    else {
      pvVar4 = (void *)0x0;
    }
    if ((pvVar4 != (void *)0x0) &&
       (sVar2 = *(short *)((int)pvVar4 + 4) + -1, *(short *)((int)pvVar4 + 4) = sVar2, sVar2 == 0))
    {
      memset(pvVar4,0,0x45);
    }
  }
  uVar1 = *(uint *)(param_1 + 0x170);
  *(undefined4 *)(param_1 + 0x164) = 0;
  *(undefined4 *)(param_1 + 0x168) = 0x7fffffff;
  if ((uVar1 != 0x7fffffff) && (iVar5 != 0)) {
    if (uVar1 < 200) {
      pvVar4 = (void *)(uVar1 * 0x45 + 0x30 + iVar5);
    }
    else {
      pvVar4 = (void *)0x0;
    }
    if ((pvVar4 != (void *)0x0) &&
       (sVar2 = *(short *)((int)pvVar4 + 4) + -1, *(short *)((int)pvVar4 + 4) = sVar2, sVar2 == 0))
    {
      memset(pvVar4,0,0x45);
    }
  }
  *(undefined4 *)(param_1 + 0x16c) = 0;
  *(undefined4 *)(param_1 + 0x170) = 0x7fffffff;
  FUN_012ce6a0();
  return;
}


