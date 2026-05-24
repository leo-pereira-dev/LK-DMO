
/* ============================================================ */
/* Function: FUN_01371ba0 */
/* Entry: 01371ba0 */
/* Signature: undefined FUN_01371ba0() */
/* ============================================================ */


undefined4 * __thiscall
FUN_01371ba0(undefined4 *param_1,uint param_2,undefined4 param_3,size_t param_4,void *param_5,
            size_t param_6)

{
  int iVar1;
  void *pvVar2;
  uint uVar3;
  size_t _Size;
  void *pvVar4;
  void *local_14;
  void *local_10;
  void *local_c;
  uint local_8;
  void *local_4;
  
  local_c = param_5;
  iVar1 = param_1[4];
  if (0x7fffffffU - iVar1 < param_2) {
                    /* WARNING: Subroutine does not return */
    FUN_00f1a480();
  }
  local_10 = (void *)(iVar1 + param_2);
  local_14 = (void *)((uint)local_10 | 0xf);
  uVar3 = param_1[5];
  if (local_14 < (void *)0x80000000) {
    if (0x7fffffff - (uVar3 >> 1) < uVar3) {
      local_14 = (void *)0x7fffffff;
    }
    else {
      pvVar2 = (void *)((uVar3 >> 1) + uVar3);
      if (local_14 < pvVar2) {
        local_14 = pvVar2;
      }
    }
  }
  else {
    local_14 = (void *)0x7fffffff;
  }
  local_8 = uVar3;
  local_4 = (void *)FUN_00f1a660(param_1,&local_14);
  param_1[4] = local_10;
  _Size = (iVar1 - param_4) + 1;
  param_1[5] = local_14;
  local_14 = (void *)((int)local_4 + param_4);
  local_10 = (void *)((int)local_14 + param_6);
  if (uVar3 < 0x10) {
    memcpy(local_4,param_1,param_4);
    memcpy(local_14,local_c,param_6);
    memcpy(local_10,(void *)((int)param_1 + param_4),_Size);
    *param_1 = local_4;
    return param_1;
  }
  pvVar2 = (void *)*param_1;
  memcpy(local_4,pvVar2,param_4);
  memcpy(local_14,local_c,param_6);
  memcpy(local_10,(void *)((int)pvVar2 + param_4),_Size);
  uVar3 = local_8 + 1;
  pvVar4 = pvVar2;
  if (0xfff < uVar3) {
    pvVar4 = *(void **)((int)pvVar2 + -4);
    uVar3 = local_8 + 0x24;
    if (0x1f < (uint)((int)pvVar2 + (-4 - (int)pvVar4))) {
                    /* WARNING: Subroutine does not return */
      _invalid_parameter_noinfo_noreturn();
    }
  }
  FUN_017986fc(pvVar4,uVar3);
  *param_1 = local_4;
  return param_1;
}


