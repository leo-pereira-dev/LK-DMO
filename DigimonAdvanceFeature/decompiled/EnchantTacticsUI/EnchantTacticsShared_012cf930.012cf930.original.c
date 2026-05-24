
/* ============================================================ */
/* Function: FUN_012cf930 */
/* Entry: 012cf930 */
/* Signature: undefined FUN_012cf930() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

void __fastcall FUN_012cf930(int param_1)

{
  int iVar1;
  int local_40 [12];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0196707d;
  local_10 = ExceptionList;
  local_40[0xb] = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  if (*(char *)(param_1 + 0x180) == '\x01') {
    FUN_013bdff0(0,0,0,0);
  }
  else {
    switch(*(undefined4 *)(param_1 + 0x178)) {
    case 0:
      local_40[0] = 0;
      break;
    case 1:
      local_40[0] = 1;
      break;
    case 2:
      local_40[0] = 2;
      break;
    case 3:
      local_40[0] = 4;
      break;
    case 4:
      local_40[0] = 6;
    }
    if (*(char *)(param_1 + 0x180) == '\0') {
      *(undefined1 *)(param_1 + 0x180) = 1;
      local_40[0] = local_40[0] + 1;
      local_40[9] = 0;
      local_40[10] = 0;
      local_40[1] = 0;
      local_40[2] = 0;
      local_40[3] = 0;
      local_40[4] = 0;
      local_40[5] = 0;
      local_40[6] = 0;
      local_40[7] = 0;
      local_40[8] = 0;
      FUN_014f08d0(local_40[0xb]);
      local_8 = 0;
      FUN_014f0c50(0x43b,0x200);
      FUN_00f26b60(local_40,4);
      FUN_00f26b60(param_1 + 0x170,4);
      FUN_014f0ab0(0x43b);
      iVar1 = *DAT_01bfe460;
      if ((((iVar1 != 0) && (*(char *)(iVar1 + 0x4f0) != '\x01')) &&
          (*(char *)(iVar1 + 0x544) == '\x03')) && (*(char *)(iVar1 + 0x545) == '\x02')) {
        FUN_014e3b80(local_40 + 1,0);
      }
      FUN_00f26790();
      *(undefined1 *)(param_1 + 8) = 0;
    }
  }
  ExceptionList = local_10;
  return;
}


