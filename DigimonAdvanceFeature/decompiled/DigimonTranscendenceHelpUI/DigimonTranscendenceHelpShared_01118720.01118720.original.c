
/* ============================================================ */
/* Function: FUN_01118720 */
/* Entry: 01118720 */
/* Signature: undefined FUN_01118720() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_01118720(int param_1)

{
  int *piVar1;
  char cVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int *piVar5;
  int *piVar6;
  uint uVar7;
  int iVar8;
  int *piVar9;
  int iStack_48;
  int local_3c;
  int local_38;
  undefined4 local_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 local_24;
  int local_20;
  undefined8 local_1c;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_01986645;
  local_10 = ExceptionList;
  uVar7 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  local_1c = 0;
  local_38 = param_1;
  local_14 = uVar7;
  FUN_01116790();
  local_8 = 0;
  piVar9 = (int *)*(int *)local_1c;
  if (*(char *)((int)piVar9 + 0xd) == '\0') {
    piVar1 = (int *)(param_1 + 0xa0);
    do {
      iStack_48 = (int)((ulonglong)*(undefined8 *)(piVar9 + 4) >> 0x20);
      local_3c = iStack_48;
      FUN_00f1d720(&uStack_28,&local_3c);
      if (((*(char *)(local_20 + 0xd) != '\0') || (iStack_48 < *(int *)(local_20 + 0x10))) ||
         (local_20 == *piVar1)) goto LAB_011188fc;
      puVar3 = *(undefined4 **)(*(int *)(local_38 + 0x90) + 100);
      puVar4 = (undefined4 *)*puVar3;
      while( true ) {
        if (puVar4 == puVar3) goto LAB_011188fc;
        iVar8 = puVar4[2];
        if (*(int *)(iVar8 + 0xc) == iStack_48) break;
        puVar4 = (undefined4 *)*puVar4;
      }
      if ((iVar8 == 0) ||
         (iVar8 = __RTDynamicCast(*(undefined4 *)(iVar8 + 8),0,&DAT_01bc4a94,&DAT_01bc5080,0,uVar7),
         param_1 = local_38, iVar8 == 0)) goto LAB_011188fc;
      *(int *)(local_38 + 0x98) = *(int *)(local_38 + 0x98) + 1;
      *(int *)(iVar8 + 8) = *(int *)(iVar8 + 8) + 1;
      *(int *)(local_38 + 0x94) = *(int *)(local_38 + 0x94) + *(int *)(iVar8 + 0x10);
      FUN_01118930();
      piVar5 = (int *)piVar9[2];
      if (*(char *)((int)piVar5 + 0xd) == '\0') {
        cVar2 = *(char *)(*piVar5 + 0xd);
        piVar9 = piVar5;
        piVar5 = (int *)*piVar5;
        while (cVar2 == '\0') {
          cVar2 = *(char *)(*piVar5 + 0xd);
          piVar9 = piVar5;
          piVar5 = (int *)*piVar5;
        }
      }
      else {
        cVar2 = *(char *)(piVar9[1] + 0xd);
        piVar6 = (int *)piVar9[1];
        piVar5 = piVar9;
        while ((piVar9 = piVar6, cVar2 == '\0' && (piVar5 == (int *)piVar9[2]))) {
          cVar2 = *(char *)(piVar9[1] + 0xd);
          piVar6 = (int *)piVar9[1];
          piVar5 = piVar9;
        }
      }
    } while (*(char *)((int)piVar9 + 0xd) == '\0');
  }
  local_24 = 0;
  local_20 = 7;
  uStack_30 = 0;
  uStack_2c = 0;
  uStack_28 = 0;
  local_34 = 0;
  local_8 = CONCAT31(local_8._1_3_,1);
  FUN_01500f80(&local_34,u_L__2f___01ad9100,(double)((float)*(int *)(param_1 + 0x94) / DAT_01b02970)
              );
  (**(code **)(**(int **)(param_1 + 0x9c) + 0x10))();
  FUN_00f1a330();
LAB_011188fc:
  FUN_011169f0();
  ExceptionList = local_10;
  return;
}


