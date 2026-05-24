
/* ============================================================ */
/* Function: FUN_011f51b0 */
/* Entry: 011f51b0 */
/* Signature: undefined FUN_011f51b0() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011f51b0(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint local_c [2];
  
  local_c[0] = DAT_01bbb1c0 ^ (uint)local_c;
  cVar1 = *(char *)(param_1 + 0x74);
  *(undefined2 *)(param_1 + 0x74) = 0x101;
  if (cVar1 == '\0') {
    iVar2 = DAT_01bc07f0 + -0x12a;
    iVar3 = DAT_01bc07ec + -0x3b8 >> 1;
    *(int *)(param_1 + 0x30) = iVar3;
    *(int *)(param_1 + 0x34) = iVar2;
    FUN_012457d0(s_Breakthrough_Breakthrough_Main_B_01ae4508,0,6,10,0,iVar3,iVar2,0x3b8,0x12a);
    FUN_011f5250();
    FUN_011f5e30();
    FUN_011f6540();
    FUN_011f6890();
    FUN_011f6ba0();
  }
  return;
}


