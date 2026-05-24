
/* ============================================================ */
/* Function: FUN_011ac950 */
/* Entry: 011ac950 */
/* Signature: undefined FUN_011ac950() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int * __thiscall FUN_011ac950(uint param_1,int *param_2,short *param_3)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  char cVar4;
  bool bVar5;
  char *pcVar6;
  undefined4 uVar7;
  int *local_1c;
  undefined1 local_18 [8];
  int local_10;
  uint local_8;
  uint local_4;
  
  local_4 = DAT_01bbb1c0 ^ (uint)&local_1c;
  local_1c = param_2;
  local_8 = param_1;
  FUN_00f41dd0(local_18,param_3);
  iVar3 = local_10;
  if (((((*(char *)(local_10 + 0xd) == '\0') && (*(short *)(local_10 + 0x10) <= *param_3)) &&
       (local_10 != *(int *)(param_1 + 0x20))) &&
      ((uVar1 = *(uint *)(local_10 + 0x18), uVar1 != 0 || (*(short *)(local_10 + 0x1c) != 0)))) &&
     ((*(int *)(local_10 + 0x60) != 0 && (*(char *)(local_10 + 0x65) != '\0')))) {
    if (*(char *)(local_10 + 100) == '\0') {
      iVar2 = *(int *)(*(int *)(local_8 + 0x30) + 4);
      local_8 = uVar1;
      if (iVar2 != 0) {
        local_1c = (int *)(iVar2 + 0x14);
        FUN_00f1b390(local_18,&local_8);
        if (((*(char *)(local_10 + 0xd) == '\0') && (*(uint *)(local_10 + 0x10) <= uVar1)) &&
           (local_10 != *local_1c)) {
          bVar5 = *(int *)(local_10 + 0x44) == 5;
        }
        else {
          bVar5 = false;
        }
        if (bVar5) {
          uVar7 = 0x21;
          pcVar6 = s_EXTRA_INVEN_SEAL_ALREADY_MASTERE_01ae1388;
          goto LAB_011aca79;
        }
        local_1c = (int *)(int)(short)(*param_3 + 11000);
        cVar4 = FUN_0108eae0(&local_1c);
        if (cVar4 != '\0') {
          uVar7 = 0;
          *(undefined1 *)(iVar3 + 100) = 1;
          *(undefined1 *)param_2 = 1;
          pcVar6 = &DAT_01abd9ec;
          goto LAB_011aca7c;
        }
      }
      goto LAB_011aca72;
    }
    uVar7 = 0x12;
    pcVar6 = s_COMMON_NOT_USE_NOW_01ad0cf0;
  }
  else {
LAB_011aca72:
    uVar7 = 0;
    pcVar6 = &DAT_01abd9ec;
  }
LAB_011aca79:
  *(undefined1 *)param_2 = 0;
LAB_011aca7c:
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[4] = 0;
  param_2[5] = 0;
  param_2[6] = 0;
  FUN_00f19cc0(pcVar6,uVar7);
  return param_2;
}


