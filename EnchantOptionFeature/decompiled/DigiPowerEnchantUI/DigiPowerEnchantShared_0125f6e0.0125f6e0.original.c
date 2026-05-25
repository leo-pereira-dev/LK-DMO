
/* ============================================================ */
/* Function: FUN_0125f6e0 */
/* Entry: 0125f6e0 */
/* Signature: undefined FUN_0125f6e0() */
/* ============================================================ */


void __fastcall FUN_0125f6e0(int param_1)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  undefined1 *puVar4;
  char cVar5;
  undefined4 uVar6;
  int iVar7;
  float fVar8;
  undefined8 uVar9;
  char *pcVar10;
  float local_30;
  float fStack_2c;
  void *local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199b044;
  local_1c = ExceptionList;
  ExceptionList = &local_1c;
  puVar4 = &stack0xfffffffc;
  if (*(int **)(param_1 + 0xd0) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xd0) + 0x18))(0,DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
    puVar4 = puStack_20;
  }
  puStack_20 = puVar4;
  if (*(int *)(param_1 + 0xd8) == 0) {
    local_24 = (void *)FUN_016830c0(0x54);
    local_14 = 0;
    if (local_24 == (void *)0x0) {
      uVar6 = 0;
    }
    else {
      memset(local_24,0,0x54);
      uVar6 = FUN_0125b110();
    }
    local_14 = 0xffffffff;
    *(undefined4 *)(param_1 + 0xd8) = uVar6;
  }
  cVar5 = FUN_0125b300();
  if ((cVar5 == '\0') && (*(undefined4 **)(param_1 + 0xd8) != (undefined4 *)0x0)) {
    (**(code **)**(undefined4 **)(param_1 + 0xd8))(1);
    *(undefined4 *)(param_1 + 0xd8) = 0;
  }
  iVar7 = *(int *)(param_1 + 0xa8);
  pvVar1 = *(void **)(iVar7 + 0xfd);
  if (((((pvVar1 != (void *)0x0) || (*(short *)(iVar7 + 0x101) != 0)) &&
       (*(int *)(iVar7 + 0x150) != 0x7fffffff)) &&
      ((local_24 = pvVar1, *(int *)(iVar7 + 0x208) != 0 &&
       (iVar7 = FUN_015db910(&local_24), iVar7 != 0)))) &&
     ((DAT_01bfb2a8 != 0 &&
      ((iVar2 = *(int *)(DAT_01bfb2a8 + 8), iVar2 != 0 &&
       (iVar3 = *(int *)(iVar2 + 0x154), iVar3 != 0)))))) {
    switch(*(undefined2 *)(iVar7 + 0x18)) {
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1b:
    case 0x35:
      uVar9 = *(undefined8 *)(iVar2 + 0xd4);
      fVar8 = *(float *)(iVar2 + 0xdc);
      pcVar10 = s_system_Accessory_Digivice_nif_01af3694;
      break;
    default:
      goto switchD_0125f837_caseD_1a;
    case 0x1c:
      uVar9 = *(undefined8 *)(iVar2 + 0xd4);
      fVar8 = *(float *)(iVar2 + 0xdc);
      pcVar10 = s_system_Accessory_Necklace_nif_01af3654;
      break;
    case 0x1d:
      uVar9 = *(undefined8 *)(iVar2 + 0xd4);
      fVar8 = *(float *)(iVar2 + 0xdc);
      pcVar10 = s_system_Accessory_Ring_nif_01af36b4;
      break;
    case 0x1e:
    case 0x21:
      uVar9 = *(undefined8 *)(iVar2 + 0xd4);
      fVar8 = *(float *)(iVar2 + 0xdc);
      pcVar10 = s_system_Accessory_Earring_nif_01af3674;
    }
    local_30 = (float)uVar9;
    fStack_2c = (float)((ulonglong)uVar9 >> 0x20);
    FUN_015b7090(pcVar10,CONCAT44(fStack_2c + 0.0,local_30 + 0.0),fVar8 - DAT_01b02a20,0x3f800000);
switchD_0125f837_caseD_1a:
    iVar7 = *(int *)(iVar3 + 0x48) + -1;
    if (iVar7 != -1) {
      uVar6 = *(undefined4 *)(*(int *)(iVar3 + 0x58) + iVar7 * 4);
      goto LAB_0125f91a;
    }
  }
  uVar6 = 0;
LAB_0125f91a:
  *(undefined4 *)(param_1 + 0xdc) = uVar6;
  iVar7 = *(int *)(param_1 + 0xb0);
  if (iVar7 != 0) {
    *(undefined1 *)(iVar7 + 0xe4) = 1;
    *(undefined4 *)(iVar7 + 0xe8) = 0;
  }
  if (*(int **)(param_1 + 0xbc) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xbc) + 0x40))(0);
  }
  if (*(int **)(param_1 + 0xc4) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xc4) + 0x40))(0);
  }
  if (DAT_01bfb32c != (int *)0x0) {
    cVar5 = (**(code **)(*DAT_01bfb32c + 4))(s_System_JewelSuccess_wav_01ae8424);
    if (cVar5 != '\0') {
      (**(code **)(*DAT_01bfb32c + 8))(s_System_JewelSuccess_wav_01ae8424);
    }
    (**(code **)*DAT_01bfb32c)(s_System_JewelSuccess_wav_01ae8424);
  }
  ExceptionList = local_1c;
  return;
}


