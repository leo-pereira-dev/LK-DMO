
/* ============================================================ */
/* Function: FUN_011bd1e0 */
/* Entry: 011bd1e0 */
/* Signature: undefined FUN_011bd1e0() */
/* ============================================================ */


/* WARNING: Type propagation algorithm not settling */

void __thiscall FUN_011bd1e0(int param_1,undefined4 param_2,int param_3)

{
  char cVar1;
  int iVar2;
  char cStack_21;
  int aiStack_20 [2];
  undefined *puStack_18;
  int iStack_14;
  char cStack_10;
  undefined1 auStack_c [8];
  int iStack_4;
  
  if (param_3 == 0) {
    return;
  }
  iVar2 = __RTDynamicCast(*(undefined4 *)(param_3 + 8),0,&DAT_01bc4a94,&DAT_01bc6058,0);
  if (iVar2 != 0) {
    aiStack_20[0] = *(int *)(iVar2 + 4);
    cVar1 = *(char *)(iVar2 + 8);
    puStack_18 = &DAT_01ae1ff4;
    aiStack_20[1] = 1;
    iStack_14 = aiStack_20[0];
    cStack_10 = cVar1;
    FUN_00f1d720(auStack_c,aiStack_20 + 1);
    if ((((*(char *)(iStack_4 + 0xd) == '\0') && (*(int *)(iStack_4 + 0x10) < 2)) &&
        (iStack_4 != *(int *)(param_1 + 0x7c))) &&
       ((*(int *)(iStack_4 + 0x38) != 0 &&
        (iVar2 = __RTDynamicCast(*(undefined4 *)(*(int *)(iStack_4 + 0x38) + 8),0,&DAT_01bc4a94,
                                 &DAT_01bc6058,0), iVar2 != 0)))) {
      cStack_21 = *(char *)(iVar2 + 8);
    }
    else {
      cStack_21 = '\0';
    }
    if ((aiStack_20[0] == 0) || (cVar1 == '\0')) {
      FUN_011bcad0(&iStack_14);
      cStack_21 = '\0';
    }
    else {
      if (cVar1 != cStack_21) {
        aiStack_20[0] = *(int *)(param_3 + 0xc) + -1;
        FUN_011bc7d0(&cStack_21,aiStack_20);
      }
      FUN_011bcad0(&iStack_14);
      cStack_21 = '\x01';
    }
    FUN_011bcd50(&cStack_21);
    return;
  }
  return;
}


