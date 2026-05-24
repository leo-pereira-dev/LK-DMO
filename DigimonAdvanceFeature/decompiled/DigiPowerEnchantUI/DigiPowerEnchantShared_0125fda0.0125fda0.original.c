
/* ============================================================ */
/* Function: FUN_0125fda0 */
/* Entry: 0125fda0 */
/* Signature: undefined FUN_0125fda0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0125fda0(int param_1)

{
  int *piVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 ****ppppuVar4;
  int iVar5;
  undefined1 *local_48;
  int local_40;
  undefined4 ***local_3c [5];
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0197b59d;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  iVar5 = *(int *)(param_1 + 0xcc);
  if (iVar5 != 0) {
    local_24 = uVar2;
    FUN_01232880(1);
    *(undefined4 *)(iVar5 + 0x20c) = 0xffffffff;
    *(undefined4 *)(iVar5 + 0x210) = 0;
    (**(code **)(**(int **)(param_1 + 0xcc) + 0x1c))(1,uVar2);
    iVar5 = *(int *)(param_1 + 0xa8);
    if ((((*(int *)(iVar5 + 0xfd) != 0) || (*(short *)(iVar5 + 0x101) != 0)) &&
        (*(int *)(iVar5 + 0x150) != 0x7fffffff)) && (iVar5 != -0xfd)) {
      iVar5 = iVar5 + 0x122;
      local_40 = 0;
      do {
        if ((*(short *)(iVar5 + -0x10) != 0) && (*(int *)(param_1 + 0xcc) != 0)) {
          FUN_0125ffe0();
          local_14 = 0;
          ppppuVar4 = local_3c;
          if (7 < local_28) {
            ppppuVar4 = (undefined4 ****)local_3c[0];
          }
          local_48 = &DAT_3f7ae148;
          FUN_012281c0(ppppuVar4,local_40,CONCAT44(DAT_01b02744,DAT_01b02744),&DAT_3f7ae148);
          local_14 = 0xffffffff;
          FUN_00f1a330();
        }
        local_40 = local_40 + 1;
        iVar5 = iVar5 + 2;
      } while (local_40 < 8);
      iVar5 = *(int *)(param_1 + 0xcc);
      if ((0 < *(int *)(iVar5 + 0x1e8)) && (*(int *)(iVar5 + 0x20c) != 0)) {
        *(undefined4 *)(iVar5 + 0x20c) = 0;
        piVar1 = (int *)**(int **)(iVar5 + 0x1e4);
        if (piVar1 == *(int **)(iVar5 + 0x1e4)) {
          uVar3 = 0;
        }
        else {
          uVar3 = piVar1[2];
        }
        *(undefined4 *)(iVar5 + 0x210) = uVar3;
        FUN_00f1b390();
        if ((((local_48[0xd] == '\0') && (*(uint *)(local_48 + 0x10) < 4)) &&
            (local_48 != *(undefined1 **)(iVar5 + 4))) && (*(int **)(local_48 + 0x14) != (int *)0x0)
           ) {
          (**(code **)(**(int **)(local_48 + 0x14) + 4))((undefined4 *)(iVar5 + 0x20c));
        }
      }
      uVar3 = FUN_01228800(0);
      iVar5 = *(int *)(param_1 + 0xa8);
      if (((*(int *)(iVar5 + 0xb8) != 0) || (*(short *)(iVar5 + 0xbc) != 0)) &&
         ((*(int *)(iVar5 + 0x14c) != 0x7fffffff &&
          ((*(int *)(iVar5 + 0x144) == 4 || (*(int *)(iVar5 + 0x144) == 7)))))) {
        *(undefined4 *)(iVar5 + 0x154) = uVar3;
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


