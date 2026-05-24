
/* ============================================================ */
/* Function: FUN_012cfb00 */
/* Entry: 012cfb00 */
/* Signature: undefined FUN_012cfb00() */
/* ============================================================ */


int * __thiscall FUN_012cfb00(int param_1,int *param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  
  piVar2 = param_2;
  *param_2 = 0;
  param_2[1] = 0;
  iVar3 = FUN_0105ff70(s_State_AT_01ad1294,8);
  if (iVar3 == 0) {
    iVar3 = *(int *)(param_1 + 0x34);
    iVar1 = *(int *)(param_1 + 0xd8);
    *param_2 = *(int *)(param_1 + 0xd4) + *(int *)(param_1 + 0x30);
    param_2[1] = iVar3 + iVar1;
    return param_2;
  }
  piVar4 = param_2 + 1;
  iVar3 = FUN_0105ff70(s_State_BL_01ad12a0,8);
  if (iVar3 == 0) {
    iVar3 = *(int *)(param_1 + 0x34);
    iVar1 = *(int *)(param_1 + 0xe8);
    *param_2 = *(int *)(param_1 + 0xe4) + *(int *)(param_1 + 0x30);
    *piVar4 = iVar3 + iVar1;
    return param_2;
  }
  param_2 = param_2 + 1;
  iVar3 = FUN_0105ff70(s_State_BL_01ad12a0,8);
  if (iVar3 == 0) {
    iVar3 = *(int *)(param_1 + 0x34);
    iVar1 = *(int *)(param_1 + 0xf8);
    *piVar2 = *(int *)(param_1 + 0xf4) + *(int *)(param_1 + 0x30);
    *piVar4 = iVar3 + iVar1;
    return piVar2;
  }
  iVar3 = FUN_0105ff70(s_State_BL_01ad12a0,8);
  if (iVar3 == 0) {
    iVar3 = *(int *)(param_1 + 0x34);
    iVar1 = *(int *)(param_1 + 0x108);
    *piVar2 = *(int *)(param_1 + 0x104) + *(int *)(param_1 + 0x30);
    *param_2 = iVar3 + iVar1;
  }
  return piVar2;
}


