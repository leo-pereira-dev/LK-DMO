
/* ============================================================ */
/* Function: FUN_010a78d0 */
/* Entry: 010a78d0 */
/* Signature: undefined FUN_010a78d0() */
/* ============================================================ */


void __fastcall FUN_010a78d0(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  int *piVar3;
  int iVar4;
  float fVar5;
  int *piVar6;
  undefined4 *puVar7;
  undefined8 *puVar8;
  int *piVar9;
  float fVar10;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined8 local_58;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 *local_44;
  undefined4 local_40;
  undefined8 local_3c;
  undefined4 local_34;
  int local_30;
  uint local_2c;
  uint local_28;
  undefined4 *local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01974c9d;
  local_1c = ExceptionList;
  ExceptionList = &local_1c;
  local_30 = param_1;
  FUN_010a7f40(DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
  if (*(int *)(param_1 + 0xa8) != 0) {
    puVar7 = (undefined4 *)FUN_015cd180();
    piVar9 = *(int **)*puVar7;
    if (*(char *)((int)piVar9 + 0xd) == '\0') {
      local_24 = (undefined4 *)(param_1 + 0x50);
      do {
        local_2c = (uint)*(ushort *)(piVar9 + 6);
        local_28 = (uint)*(ushort *)(piVar9 + 7);
        puVar8 = (undefined8 *)FUN_01012bc0(&uStack_7c,piVar9 + 4);
        local_3c = *puVar8;
        if ((*(char *)(*(int *)(puVar8 + 1) + 0xd) != '\0') ||
           (*(byte *)(piVar9 + 4) < *(byte *)(*(int *)(puVar8 + 1) + 0xe))) {
          if (local_24[1] == 0xaaaaaaa) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          uVar2 = *local_24;
          local_44 = local_24;
          local_14 = 0;
          local_40 = 0;
          puVar7 = operator_new(0x18);
          local_14 = 0xffffffff;
          *(char *)((int)puVar7 + 0xe) = (char)piVar9[4];
          *(short *)(puVar7 + 4) = (short)local_2c;
          *(short *)((int)puVar7 + 0x12) = (short)local_28;
          *(undefined2 *)(puVar7 + 5) = 0;
          *puVar7 = uVar2;
          puVar7[1] = uVar2;
          puVar7[2] = uVar2;
          *(undefined2 *)(puVar7 + 3) = 0;
          local_40 = 0;
          FUN_00f1b3e0((undefined4)local_3c,local_3c._4_4_,puVar7);
        }
        piVar3 = (int *)piVar9[2];
        if (*(char *)((int)piVar3 + 0xd) == '\0') {
          cVar1 = *(char *)(*piVar3 + 0xd);
          piVar9 = piVar3;
          piVar3 = (int *)*piVar3;
          while (cVar1 == '\0') {
            cVar1 = *(char *)(*piVar3 + 0xd);
            piVar9 = piVar3;
            piVar3 = (int *)*piVar3;
          }
        }
        else {
          cVar1 = *(char *)(piVar9[1] + 0xd);
          piVar6 = (int *)piVar9[1];
          piVar3 = piVar9;
          while ((piVar9 = piVar6, cVar1 == '\0' && (piVar3 == (int *)piVar9[2]))) {
            cVar1 = *(char *)(piVar9[1] + 0xd);
            piVar6 = (int *)piVar9[1];
            piVar3 = piVar9;
          }
        }
        param_1 = local_30;
      } while (*(char *)((int)piVar9 + 0xd) == '\0');
    }
  }
  FUN_010a8280();
  DAT_01bb97a0 = 0;
  if (DAT_01bfb2a0 != 0) {
    FUN_00f54dc0();
    FUN_00f547c0(s_UnionCombine_mp3_01ad2dd4);
  }
  local_64 = 0x3f800000;
  local_5c = 0x42340000;
  local_4c = 0;
  *(int *)(param_1 + 0xc) = DAT_01bfc3b4[0x46];
  local_68 = 0x43e10000;
  local_60 = 0x47c35000;
  local_48 = 0xbfc90fdb;
  *(int *)(param_1 + 0x10) = DAT_01bfc3b4[0x47];
  *(undefined1 *)(DAT_01bfc3b4 + 0x5d) = 0;
  *(undefined1 *)(DAT_01bfc3b4 + 0x48) = 0;
  local_50 = DAT_01beef20;
  local_58 = DAT_01beef18;
  uStack_7c = 0;
  uStack_78 = 0;
  uStack_74 = 0;
  FUN_014791e0(&local_68);
  fVar5 = DAT_01b02a08;
  DAT_01bfc3b4[0x57] = 0;
  piVar9 = DAT_01bfc3b4;
  fVar10 = fVar5;
  if ((((char)DAT_01bfc3b4[0x48] == '\x01') &&
      (fVar10 = (float)DAT_01bfc3b4[0x46], (float)DAT_01bfc3b4[0x46] <= fVar5)) &&
     (fVar10 = (float)DAT_01bfc3b4[0x47], fVar5 <= (float)DAT_01bfc3b4[0x47])) {
    fVar10 = fVar5;
  }
  DAT_01bfc3b4[0x4a] = (int)fVar10;
  iVar4 = *piVar9;
  local_34 = 0;
  *(ulonglong *)(iVar4 + 0x50) = (ulonglong)((uint)fVar10 ^ DAT_01b03420) << 0x20;
  *(undefined4 *)(iVar4 + 0x58) = 0;
  DAT_01bfc3b4[0x4d] = 0x43e10000;
  fVar10 = (float)DAT_01bfc3b4[0x46];
  if ((fVar5 < fVar10) || (fVar10 = (float)DAT_01bfc3b4[0x47], fVar10 < fVar5)) {
    DAT_01bfc3b4[0x4d] = (int)fVar10;
  }
  FUN_01692c40(0,0);
  FUN_010272b0();
  local_28 = 0;
  FUN_00ffcb30(&local_28,&DAT_01bfe910);
  ExceptionList = local_1c;
  return;
}


