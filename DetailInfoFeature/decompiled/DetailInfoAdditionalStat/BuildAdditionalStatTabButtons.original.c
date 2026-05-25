
/* ============================================================ */
/* Function: FUN_011d14f0 */
/* Entry: 011d14f0 */
/* Signature: undefined FUN_011d14f0() */
/* ============================================================ */


void __fastcall FUN_011d14f0(int param_1)

{
  char cVar1;
  undefined4 *puVar2;
  int iVar3;
  void **ppvVar4;
  uint uVar5;
  int *piVar6;
  int *piVar7;
  int *local_24;
  undefined4 local_20;
  int *local_1c;
  int local_18;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01992db7;
  uVar5 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  local_14 = *(int *)(param_1 + 0x84);
  cVar1 = *(char *)((int)*(int **)(local_14 + 4) + 0xd);
  ppvVar4 = &local_10;
  piVar6 = *(int **)(local_14 + 4);
  local_18 = param_1;
  local_10 = ExceptionList;
  while (ExceptionList = ppvVar4, cVar1 == '\0') {
    FUN_00f1d770((int *)(param_1 + 0x84),piVar6[2]);
    piVar7 = (int *)*piVar6;
    FUN_017986fc(piVar6,0x18);
    ppvVar4 = ExceptionList;
    piVar6 = piVar7;
    cVar1 = *(char *)((int)piVar7 + 0xd);
  }
  *(int *)(local_14 + 4) = local_14;
  *(int *)local_14 = local_14;
  *(int *)(local_14 + 8) = local_14;
  *(undefined4 *)(param_1 + 0x88) = 0;
  piVar6 = (int *)FUN_016830c0(0x84,uVar5);
  local_8 = 0;
  if (piVar6 == (int *)0x0) {
    piVar6 = (int *)0x0;
  }
  else {
    local_1c = piVar6;
    memset(piVar6,0,0x84);
    FUN_01245550();
    piVar6[0x1d] = (int)&DAT_01ae319c;
    piVar6[0x1e] = 0;
    piVar6[0x1d] = (int)&DAT_01ae33f4;
    local_8 = CONCAT31(local_8._1_3_,1);
    *piVar6 = (int)&DAT_01ae2afc;
    piVar6[0x1d] = (int)&DAT_01ae2af0;
    *(undefined1 *)(piVar6 + 0x1f) = 0;
    piVar6[0x20] = 0;
    FUN_011d1200();
  }
  local_8 = 0xffffffff;
  local_1c = piVar6;
  (**(code **)(*piVar6 + 0x2c))(*(undefined4 *)(local_18 + 0x40),0xd,0x43,0x116,0x19a);
  piVar6[0x12] = local_18;
  local_24 = (int *)(local_18 + 0x58);
  local_14 = *local_24;
  if (*(int *)(local_18 + 0x5c) == 0x15555555) {
                    /* WARNING: Subroutine does not return */
    std::_Xlength_error(s_list_too_long_01abdaa4);
  }
  local_8 = 2;
  local_20 = 0;
  piVar7 = operator_new(0xc);
  iVar3 = local_14;
  local_8 = 0xffffffff;
  piVar7[2] = (int)piVar6;
  *(int *)(local_18 + 0x5c) = *(int *)(local_18 + 0x5c) + 1;
  local_14 = 0;
  puVar2 = *(undefined4 **)(iVar3 + 4);
  *piVar7 = iVar3;
  piVar7[1] = (int)puVar2;
  *(int **)(iVar3 + 4) = piVar7;
  *puVar2 = piVar7;
  *(undefined1 *)(piVar6 + 0x1a) = 0;
  FUN_011d1a10(&local_24,&local_14,&local_1c);
  ExceptionList = local_10;
  return;
}


