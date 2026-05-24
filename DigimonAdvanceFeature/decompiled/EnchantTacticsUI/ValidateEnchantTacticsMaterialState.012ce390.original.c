
/* ============================================================ */
/* Function: FUN_012ce390 */
/* Entry: 012ce390 */
/* Signature: undefined FUN_012ce390() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

uint __fastcall FUN_012ce390(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  undefined4 *puVar6;
  uint local_24;
  undefined8 local_20;
  uint local_18;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0197792d;
  local_10 = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  iVar1 = *(int *)(param_1 + 0x18c);
  iVar5 = *(int *)(param_1 + 400);
  if (iVar1 != *(int *)(param_1 + 400)) {
    *(int *)(param_1 + 400) = iVar1;
    iVar5 = iVar1;
  }
  if (*(int *)(param_1 + 0x19c) == 0) {
    local_24 = iVar5 - iVar1 >> 2;
  }
  else {
    local_20 = 0;
    local_18 = uVar4;
    puVar6 = operator_new(0xc);
    *puVar6 = puVar6;
    puVar6[1] = puVar6;
    local_8 = 0;
    local_20._0_4_ = puVar6;
    FUN_0161e5d0(&stack0x00000004,&local_20);
    local_24 = local_20._4_4_;
    if ((uint)(*(int *)(param_1 + 0x194) - *(int *)(param_1 + 0x18c) >> 2) < local_20._4_4_) {
      if (0x3fffffff < local_20._4_4_) {
                    /* WARNING: Subroutine does not return */
        FUN_00f19c70(uVar4);
      }
      FUN_01007cf0(&local_24);
    }
    puVar3 = (undefined4 *)local_20;
    for (puVar6 = (undefined4 *)*(undefined4 *)local_20; puVar6 != puVar3;
        puVar6 = (undefined4 *)*puVar6) {
      puVar2 = *(undefined4 **)(param_1 + 400);
      if (puVar2 == *(undefined4 **)(param_1 + 0x194)) {
        FUN_01007d90(puVar2,puVar6 + 2);
      }
      else {
        *puVar2 = puVar6[2];
        *(int *)(param_1 + 400) = *(int *)(param_1 + 400) + 4;
      }
    }
    local_24 = *(int *)(param_1 + 400) - *(int *)(param_1 + 0x18c) >> 2;
    *(undefined4 *)((undefined4 *)local_20)[1] = 0;
    puVar6 = (undefined4 *)*(undefined4 *)local_20;
    while (puVar6 != (undefined4 *)0x0) {
      puVar3 = (undefined4 *)*puVar6;
      FUN_017986fc(puVar6,0xc);
      puVar6 = puVar3;
    }
    FUN_017986fc((undefined4 *)local_20,0xc);
  }
  ExceptionList = local_10;
  return local_24;
}


