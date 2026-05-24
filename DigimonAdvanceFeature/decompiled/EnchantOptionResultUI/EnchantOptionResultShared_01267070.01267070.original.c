
/* ============================================================ */
/* Function: FUN_01267070 */
/* Entry: 01267070 */
/* Signature: undefined FUN_01267070() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01267070(int *param_1)

{
  int iVar1;
  undefined4 uStack_c;
  undefined1 auStack_8 [4];
  uint local_4;
  
  local_4 = DAT_01bbb1c0 ^ (uint)&uStack_c;
  if (param_1[0x57] != 0) {
    iVar1 = __RTDynamicCast(*(undefined4 *)(param_1[0x57] + 0x34),0,&DAT_01bc4a94,&DAT_01bc69f4,0);
    if (iVar1 != 0) {
      iVar1 = *(int *)(iVar1 + 4);
      if (iVar1 == 0) {
        FUN_01033ec0();
      }
      else {
        if (iVar1 == 1) {
          auStack_8[0] = 1;
          FUN_014414a0(0x1cc,auStack_8);
          uStack_c = 7;
          FUN_00ffcb30(&uStack_c,&DAT_01bfe760);
          return;
        }
        if (iVar1 == 2) {
          uStack_c = (**(code **)(*param_1 + 0x28))();
          FUN_010328b0(&uStack_c);
          return;
        }
      }
    }
  }
  return;
}


