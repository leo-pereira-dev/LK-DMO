
/* ============================================================ */
/* Function: FUN_01272200 */
/* Entry: 01272200 */
/* Signature: undefined FUN_01272200() */
/* ============================================================ */


undefined4 __fastcall FUN_01272200(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_18;
  undefined1 local_14 [8];
  int local_c;
  
  if (DAT_01bfe470 != 0) {
    piVar1 = (int *)(DAT_01bfe470 + 4);
    local_18 = 0x2b;
    iVar3 = param_1 + 0x74;
    FUN_00f1d720(local_14,&local_18);
    if (((*(char *)(local_c + 0xd) == '\0') && (*(int *)(local_c + 0x10) < 0x2c)) &&
       (local_c != *piVar1)) {
      uVar2 = *(undefined4 *)(local_c + 0x14);
    }
    else {
      uVar2 = 0;
    }
    uVar2 = __RTDynamicCast(uVar2,0,&DAT_01bc3cf8,&DAT_01bc6a20,0);
    *(undefined4 *)(param_1 + 0x78) = uVar2;
    if (*(int *)(param_1 + 0x78) != 0) {
      local_18 = 0x19;
      if (param_1 == 0) {
        iVar3 = 0;
      }
      FUN_00ff8bc0(&local_18,iVar3);
      local_18 = 10;
      FUN_00ff8bc0(&local_18,iVar3);
      local_18 = 0xb;
      FUN_00ff8bc0(&local_18,iVar3);
      local_18 = 0xf;
      FUN_00ff8bc0(&local_18,iVar3);
      local_18 = 0x14;
      FUN_00ff8bc0(&local_18,iVar3);
      local_18 = 0x16;
      FUN_00ff8bc0(&local_18,iVar3);
      local_18 = 0x17;
      FUN_00ff8bc0(&local_18,iVar3);
      local_18 = 0x18;
      FUN_00ff8bc0(&local_18,iVar3);
      return 1;
    }
  }
  return 0;
}


