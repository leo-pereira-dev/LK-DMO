
/* ============================================================ */
/* Function: FUN_0125f1d0 */
/* Entry: 0125f1d0 */
/* Signature: undefined FUN_0125f1d0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_0125f1d0(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int *piVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 uVar6;
  int iVar7;
  undefined4 *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199b006;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_28 = (undefined4 *)FUN_016830c0(0x94,local_24);
  local_14 = 0;
  if (local_28 == (undefined4 *)0x0) {
    iVar2 = 0;
    uVar6 = extraout_ECX;
  }
  else {
    memset(local_28,0,0x94);
    iVar2 = FUN_0122b460();
    uVar6 = extraout_ECX_00;
  }
  local_14 = 0xffffffff;
  *(int *)(param_1 + 0xb4) = iVar2;
  if (iVar2 != 0) {
    FUN_0122b6f0(*(undefined4 *)(param_1 + 0x40),0xc9,0x68,0x2a,0x2a,0,uVar6);
    *(undefined1 *)(*(int *)(param_1 + 0xb4) + 0x8c) = 0;
    FUN_012611d0(8,param_1,FUN_01261020,0);
    FUN_012472c0(*(undefined4 *)(param_1 + 0xb4));
    FUN_0122c320();
    local_28 = (undefined4 *)FUN_016830c0(0x2c);
    local_14 = 1;
    if (local_28 == (undefined4 *)0x0) {
      local_28 = (undefined4 *)0x0;
    }
    else {
      memset(local_28,0,0x2c);
      local_28 = (undefined4 *)FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (local_28 != (undefined4 *)0x0) {
      iVar2 = *(int *)(param_1 + 0xa8);
      iVar7 = *(int *)(iVar2 + 0xb8);
      if (((iVar7 == 0) && (*(short *)(iVar2 + 0xbc) == 0)) ||
         (*(int *)(iVar2 + 0x14c) == 0x7fffffff)) {
        iVar7 = 0;
      }
      *(bool *)(param_1 + 0xc0) = iVar7 != 0;
      piVar3 = (int *)FUN_01231410(10,iVar7,1,0x20,0x20,DAT_01bee88c,DAT_01bee890);
      *(int **)(param_1 + 0xbc) = piVar3;
      if (piVar3 != (int *)0x0) {
        (**(code **)(*piVar3 + 0x54))(1);
      }
      puVar4 = (undefined4 *)FUN_016830c0(0x1c);
      if (puVar4 != (undefined4 *)0x0) {
        *puVar4 = 0;
        puVar4[1] = 0;
        puVar4[2] = 0;
        puVar4[3] = 0;
        *(undefined8 *)(puVar4 + 4) = 0;
        puVar4[6] = 0;
        *puVar4 = &DAT_01ae673c;
        puVar4[1] = 0;
        puVar4[2] = 0;
        puVar4[3] = 0xffffffff;
        *(undefined2 *)(puVar4 + 4) = 0;
        puVar4[5] = 0;
        *(undefined2 *)(puVar4 + 6) = 1;
        FUN_0122b3e0(local_28);
        iVar2 = *(int *)(param_1 + 0xb4);
        iVar7 = 0;
        puVar1 = *(undefined4 **)(iVar2 + 100);
        for (puVar5 = (undefined4 *)*puVar1; puVar5 != puVar1; puVar5 = (undefined4 *)*puVar5) {
          if ((undefined4 *)puVar5[2] == puVar4) {
            if (iVar7 != -1) {
              ExceptionList = local_1c;
              return;
            }
            break;
          }
          iVar7 = iVar7 + 1;
        }
        if (*(int *)(iVar2 + 0x78) < (int)puVar4[5]) {
          *(undefined4 *)(iVar2 + 0x78) = puVar4[5];
          puVar5 = (undefined4 *)*puVar1;
          if (puVar5 != puVar1) {
            do {
              *(undefined4 *)(puVar5[2] + 0x14) = *(undefined4 *)(iVar2 + 0x78);
              puVar5 = (undefined4 *)*puVar5;
            } while (puVar5 != *(undefined4 **)(iVar2 + 100));
          }
        }
        else {
          puVar4[5] = *(int *)(iVar2 + 0x78);
        }
        local_28 = puVar4;
        FUN_00f55430(&local_28);
        FUN_0122c760();
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


