
/* ============================================================ */
/* Function: FUN_0125e500 */
/* Entry: 0125e500 */
/* Signature: undefined FUN_0125e500() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int __fastcall FUN_0125e500(int param_1)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  undefined1 auStack_98 [8];
  int iStack_90;
  int iStack_8c;
  int iStack_88;
  int local_84;
  int iStack_80;
  int iStack_7c;
  int iStack_78;
  int iStack_74;
  int iStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined1 uStack_2c;
  uint local_14;
  
  local_14 = DAT_01bbb1c0 ^ (uint)auStack_98;
  local_84 = FUN_01287d50();
  if (local_84 == 0) {
    return 0;
  }
  if ((*(int **)(param_1 + 0xb4) != (int *)0x0) &&
     (iVar4 = (**(code **)(**(int **)(param_1 + 0xb4) + 0x48))
                        (*(undefined4 *)(DAT_01bfe474 + 8),*(undefined4 *)(DAT_01bfe474 + 0xc)),
     iVar4 != 0)) {
    iVar4 = *(int *)(*(int *)(param_1 + 0xb4) + 0x70);
    if (iVar4 == 0) {
      return local_84;
    }
    iVar1 = *(int *)(param_1 + 0xa8);
    piVar6 = (int *)(iVar1 + 0xb8);
    if ((*piVar6 == 0) && (*(short *)(iVar1 + 0xbc) == 0)) {
      return local_84;
    }
    if (*(int *)(iVar1 + 0x14c) == 0x7fffffff) {
      return local_84;
    }
    if (piVar6 == (int *)0x0) {
      return local_84;
    }
    iStack_8c = *piVar6;
    uStack_6c = *(undefined4 *)(iVar1 + 0xbc);
    uStack_68 = *(undefined4 *)(iVar1 + 0xc0);
    uStack_64 = *(undefined4 *)(iVar1 + 0xc4);
    uStack_60 = *(undefined4 *)(iVar1 + 200);
    uStack_5c = *(undefined4 *)(iVar1 + 0xcc);
    uStack_58 = *(undefined4 *)(iVar1 + 0xd0);
    uStack_54 = *(undefined4 *)(iVar1 + 0xd4);
    uStack_50 = *(undefined4 *)(iVar1 + 0xd8);
    uStack_4c = *(undefined4 *)(iVar1 + 0xdc);
    uStack_48 = *(undefined4 *)(iVar1 + 0xe0);
    uStack_44 = *(undefined4 *)(iVar1 + 0xe4);
    uStack_40 = *(undefined4 *)(iVar1 + 0xe8);
    uStack_3c = *(undefined4 *)(iVar1 + 0xec);
    uStack_38 = *(undefined4 *)(iVar1 + 0xf0);
    uStack_34 = *(undefined4 *)(iVar1 + 0xf4);
    uStack_30 = *(undefined4 *)(iVar1 + 0xf8);
    uStack_2c = *(undefined1 *)(iVar1 + 0xfc);
    if (DAT_01bfe47c == (int *)0x0) {
      return local_84;
    }
    iStack_88 = *DAT_01bfe47c;
    if (iStack_88 == 0) {
      return local_84;
    }
    iVar4 = *(int *)(iVar4 + 4);
    iStack_90 = iStack_8c >> 0x1f;
    iStack_70 = iStack_8c;
    if ((iVar4 == 0) || (piVar6 = *(int **)(iVar4 + 4), piVar2 = (int *)*piVar6, piVar2 == piVar6))
    {
      iStack_7c = DAT_01bee890;
      iStack_80 = DAT_01bee88c;
    }
    else {
      (**(code **)(*(int *)piVar2[2] + 0x48))(&iStack_80);
    }
    FUN_013c19c0(iStack_80,iStack_7c,0x20,0x20,0x168,1,iStack_8c,iStack_90,0x32,0,0,0,0,0,&iStack_70
                 ,0,0);
    return local_84;
  }
  if ((*(int **)(param_1 + 0xb8) == (int *)0x0) ||
     (iVar4 = (**(code **)(**(int **)(param_1 + 0xb8) + 0x48))
                        (*(undefined4 *)(DAT_01bfe474 + 8),*(undefined4 *)(DAT_01bfe474 + 0xc)),
     iVar4 == 0)) {
    if ((*(int **)(param_1 + 0xcc) != (int *)0x0) &&
       (iVar4 = (**(code **)(**(int **)(param_1 + 0xcc) + 0x44))(), iVar4 != -1)) {
      return local_84;
    }
    if ((*(int **)(param_1 + 0xd0) != (int *)0x0) &&
       (iVar4 = (**(code **)(**(int **)(param_1 + 0xd0) + 0x44))(), iVar4 == 4)) {
      return local_84;
    }
    if ((*(int **)(param_1 + 0xd4) != (int *)0x0) &&
       (iVar4 = (**(code **)(**(int **)(param_1 + 0xd4) + 0x44))(), iVar4 == 4)) {
      return local_84;
    }
    iVar4 = DAT_01bfe474;
    iStack_7c = *(int *)(param_1 + 0x34);
    iStack_80 = *(int *)(param_1 + 0x30);
    iStack_78 = iStack_80 + 300;
    iStack_74 = iStack_7c + 0x24;
    iVar5 = FUN_00f83b00(DAT_01bfe474 + 8);
    iVar1 = DAT_01bfe474;
    if ((((iVar5 == 1) && (*(int *)(iVar4 + 4) != 10)) && (*(char *)(param_1 + 0x1d) != '\0')) &&
       (*(int *)(DAT_01bfe474 + 4) == 1)) {
      *(undefined1 *)(param_1 + 0x7d) = 1;
      uVar3 = *(undefined4 *)(iVar1 + 0xc);
      *(undefined4 *)(param_1 + 0x80) = *(undefined4 *)(iVar1 + 8);
      *(undefined4 *)(param_1 + 0x84) = uVar3;
    }
    return local_84;
  }
  iVar4 = *(int *)(*(int *)(param_1 + 0xb8) + 0x70);
  if (iVar4 == 0) {
    return local_84;
  }
  iVar1 = *(int *)(param_1 + 0xa8);
  if ((*(int *)(iVar1 + 0xfd) == 0) && (*(short *)(iVar1 + 0x101) == 0)) {
    return local_84;
  }
  if (*(int *)(iVar1 + 0x150) == 0x7fffffff) {
    return local_84;
  }
  if ((int *)(iVar1 + 0xfd) == (int *)0x0) {
    return local_84;
  }
  iStack_8c = *(int *)(iVar1 + 0xfd);
  uStack_6c = *(undefined4 *)(iVar1 + 0x101);
  uStack_68 = *(undefined4 *)(iVar1 + 0x105);
  uStack_64 = *(undefined4 *)(iVar1 + 0x109);
  uStack_60 = *(undefined4 *)(iVar1 + 0x10d);
  uStack_5c = *(undefined4 *)(iVar1 + 0x111);
  uStack_58 = *(undefined4 *)(iVar1 + 0x115);
  uStack_54 = *(undefined4 *)(iVar1 + 0x119);
  uStack_50 = *(undefined4 *)(iVar1 + 0x11d);
  uStack_4c = *(undefined4 *)(iVar1 + 0x121);
  uStack_48 = *(undefined4 *)(iVar1 + 0x125);
  uStack_44 = *(undefined4 *)(iVar1 + 0x129);
  uStack_40 = *(undefined4 *)(iVar1 + 0x12d);
  uStack_3c = *(undefined4 *)(iVar1 + 0x131);
  uStack_38 = *(undefined4 *)(iVar1 + 0x135);
  uStack_34 = *(undefined4 *)(iVar1 + 0x139);
  uStack_30 = *(undefined4 *)(iVar1 + 0x13d);
  uStack_2c = *(undefined1 *)(iVar1 + 0x141);
  if (DAT_01bfe47c == (int *)0x0) {
    return local_84;
  }
  iStack_90 = *DAT_01bfe47c;
  if (iStack_90 == 0) {
    return local_84;
  }
  iVar4 = *(int *)(iVar4 + 4);
  iStack_88 = iStack_8c >> 0x1f;
  iStack_70 = iStack_8c;
  if ((iVar4 == 0) || (piVar6 = *(int **)(iVar4 + 4), piVar2 = (int *)*piVar6, piVar2 == piVar6)) {
    iStack_7c = DAT_01bee890;
    iStack_80 = DAT_01bee88c;
  }
  else {
    (**(code **)(*(int *)piVar2[2] + 0x48))(&iStack_80);
  }
  FUN_013c19c0(iStack_80,iStack_7c,0x20,0x20,0x168,1,iStack_8c,iStack_88,0x32,0,0,0,0,0,&iStack_70,0
               ,0);
  return local_84;
}


