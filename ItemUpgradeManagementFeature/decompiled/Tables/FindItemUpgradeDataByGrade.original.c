
/* ============================================================ */
/* Function: FUN_015ec600 */
/* Entry: 015ec600 */
/* Signature: undefined FUN_015ec600() */
/* ============================================================ */


void * __thiscall FUN_015ec600(int param_1,void *param_2,undefined4 param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 local_1c [8];
  int local_14;
  undefined1 local_10 [12];
  
  uVar1 = FUN_00f64ef0(&param_3);
  iVar2 = _Find_last<>(local_10,&param_3,uVar1);
  iVar2 = *(int *)(iVar2 + 4);
  if (iVar2 == 0) {
    iVar2 = *(int *)(param_1 + 0x18);
  }
  if (iVar2 != *(int *)(param_1 + 0x18)) {
    FUN_015ae4e0(local_1c,&param_4);
    if (((*(char *)(local_14 + 0xd) == '\0') && (*(int *)(local_14 + 0x10) <= param_4)) &&
       (local_14 != *(int *)(iVar2 + 0xc))) {
      FUN_015ebf00(local_14 + 0x14);
      return param_2;
    }
  }
  memset(param_2,0,0x2c);
  FUN_015ebf60();
  return param_2;
}


