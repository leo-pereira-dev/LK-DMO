
/* ============================================================ */
/* Function: FUN_01087760 */
/* Entry: 01087760 */
/* Signature: undefined FUN_01087760() */
/* ============================================================ */


void __fastcall FUN_01087760(int param_1)

{
  int *piVar1;
  int iVar2;
  float fVar3;
  float fVar4;
  undefined4 auStack_60 [3];
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined8 uStack_24;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  *(undefined4 *)(DAT_01bfb2d8 + 0x58) = 6;
  if (DAT_01bfb2a8 != 0) {
    piVar1 = *(int **)(DAT_01bfb2a8 + 8);
    if (piVar1 != (int *)0x0) {
      piVar1[0x22f] = 0;
      (**(code **)(*piVar1 + 0x70))();
    }
    if (*(int **)(DAT_01bfb2a8 + 0x14) != (int *)0x0) {
      (**(code **)(**(int **)(DAT_01bfb2a8 + 0x14) + 0x70))();
    }
  }
  DAT_01bb97a0 = 0;
  if (DAT_01bfb2a0 != 0) {
    FUN_00f54dc0();
    FUN_00f547c0(s_Enchant_Room_mp3_01ac8de8);
  }
  uStack_30 = 0x3f800000;
  uStack_28 = 0x42340000;
  uStack_18 = 0;
  *(int *)(param_1 + 0xc) = DAT_01bfc3b4[0x46];
  uStack_34 = 0x43e10000;
  uStack_2c = 0x47c35000;
  uStack_14 = 0xbfc90fdb;
  *(int *)(param_1 + 0x10) = DAT_01bfc3b4[0x47];
  *(undefined1 *)(DAT_01bfc3b4 + 0x5d) = 0;
  *(undefined1 *)(DAT_01bfc3b4 + 0x48) = 0;
  uStack_1c = DAT_01beef20;
  uStack_24 = DAT_01beef18;
  uStack_50 = 0;
  uStack_4c = 0;
  uStack_48 = 0;
  uStack_44 = 0;
  FUN_014791e0(&uStack_34);
  fVar3 = DAT_01b02a08;
  DAT_01bfc3b4[0x57] = 0;
  piVar1 = DAT_01bfc3b4;
  fVar4 = fVar3;
  if ((((char)DAT_01bfc3b4[0x48] == '\x01') &&
      (fVar4 = (float)DAT_01bfc3b4[0x46], (float)DAT_01bfc3b4[0x46] <= fVar3)) &&
     (fVar4 = (float)DAT_01bfc3b4[0x47], fVar3 <= (float)DAT_01bfc3b4[0x47])) {
    fVar4 = fVar3;
  }
  DAT_01bfc3b4[0x4a] = (int)fVar4;
  iVar2 = *piVar1;
  uStack_54 = 0;
  *(ulonglong *)(iVar2 + 0x50) = CONCAT44((uint)fVar4 ^ DAT_01b03420,uStack_50);
  *(undefined4 *)(iVar2 + 0x58) = 0;
  DAT_01bfc3b4[0x4d] = 0x43e10000;
  fVar4 = (float)DAT_01bfc3b4[0x46];
  if ((fVar3 < fVar4) || (fVar4 = (float)DAT_01bfc3b4[0x47], fVar4 < fVar3)) {
    DAT_01bfc3b4[0x4d] = (int)fVar4;
  }
  FUN_01692c40(0,0);
  FUN_0102cdd0();
  auStack_60[0] = 0;
  FUN_00ffcb30(auStack_60,&DAT_01bfe8ac);
  FUN_010883d0();
  return;
}


