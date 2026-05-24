
/* ============================================================ */
/* Function: FUN_011a7b60 */
/* Entry: 011a7b60 */
/* Signature: undefined FUN_011a7b60() */
/* ============================================================ */


void __thiscall FUN_011a7b60(int *param_1,int param_2)

{
  short sVar1;
  code *pcVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 ***pppuVar5;
  int *piVar6;
  undefined1 local_60 [24];
  undefined4 **local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  uint local_34;
  int local_30 [3];
  int *local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019900e5;
  local_1c = ExceptionList;
  ExceptionList = &local_1c;
  if (param_1[7] != 0) {
    iVar3 = __RTDynamicCast(*(undefined4 *)(param_1[7] + 8),0,&DAT_01bc4a94,&DAT_01bc5e04,0,
                            DAT_01bbb1c0 ^ (uint)&stack0xfffffff0);
    if (iVar3 != 0) {
      if (*(int *)(iVar3 + 8) != *(int *)(param_2 + 4)) {
        *(int *)(iVar3 + 8) = *(int *)(param_2 + 4);
      }
      if (*(short *)(iVar3 + 0xc) != *(short *)(param_2 + 8)) {
        *(short *)(iVar3 + 0xc) = *(short *)(param_2 + 8);
      }
      if (*(short *)(iVar3 + 4) != *(short *)(param_2 + 10)) {
        *(short *)(iVar3 + 4) = *(short *)(param_2 + 10);
      }
      if ((int *)param_1[4] != (int *)0x0) {
        (**(code **)(*(int *)param_1[4] + 0x40))(0);
      }
      if ((int *)param_1[5] != (int *)0x0) {
        (**(code **)(*(int *)param_1[5] + 0x40))(*(undefined1 *)(param_2 + 0x24));
      }
      if ((*(int *)(iVar3 + 8) == 0) || (*(short *)(iVar3 + 0xc) == 0)) {
        *(undefined2 *)(iVar3 + 4) = 0;
        *(undefined4 *)(iVar3 + 8) = 0;
        *(undefined2 *)(iVar3 + 0xc) = 0;
        local_24 = (int *)*param_1;
        if (local_24 != (int *)0x0) {
          local_38 = 0;
          pcVar2 = *(code **)(*local_24 + 0x58);
          local_48 = (undefined4 ***)0x0;
          uStack_44 = 0;
          uStack_40 = 0;
          uStack_3c = 0;
          local_34 = 0;
          FUN_00f19cc0(s_SealMaster_sealmaster_bg_01_tga_01ae104c,0x1f);
          local_14 = 0;
          pppuVar5 = &local_48;
          if (0xf < local_34) {
            pppuVar5 = (undefined4 ***)local_48;
          }
          (*pcVar2)(pppuVar5,1);
          local_14 = 0xffffffff;
          FUN_00f1a420();
        }
        if ((int *)param_1[1] != (int *)0x0) {
          (**(code **)(*(int *)param_1[1] + 0x40))(0);
        }
        if ((int *)param_1[4] != (int *)0x0) {
          (**(code **)(*(int *)param_1[4] + 0x40))(0);
        }
        if ((int *)param_1[5] != (int *)0x0) {
          (**(code **)(*(int *)param_1[5] + 0x40))(1);
        }
        if ((int *)param_1[2] != (int *)0x0) {
          (**(code **)(*(int *)param_1[2] + 0x40))(0);
        }
        if ((int *)param_1[3] != (int *)0x0) {
          (**(code **)(*(int *)param_1[3] + 0x40))(0);
        }
      }
      else {
        if ((int *)param_1[1] != (int *)0x0) {
          (**(code **)(*(int *)param_1[1] + 0x40))(1);
          piVar6 = (int *)(param_2 + 0xc);
          if (0xf < *(uint *)(param_2 + 0x20)) {
            piVar6 = (int *)*piVar6;
          }
          if (*(int *)(param_1[1] + 0x28) != 0) {
            FUN_0123fcd0(piVar6,param_2 + 0x28,1);
          }
        }
        sVar1 = *(short *)(iVar3 + 0xc);
        if ((int *)param_1[4] != (int *)0x0) {
          (**(code **)(*(int *)param_1[4] + 0x40))(1);
          local_24 = (int *)param_1[4];
          local_30[1] = 0;
          local_30[0] = (int)sVar1;
          puVar4 = (undefined4 *)FUN_015027f0(local_60,local_30);
          local_14 = 1;
          if (7 < (uint)puVar4[5]) {
            puVar4 = (undefined4 *)*puVar4;
          }
          (**(code **)(*(int *)((int)local_24 + 0x14) + 0x10))(puVar4);
          local_14 = 0xffffffff;
          FUN_00f1a330();
        }
        (**(code **)(*(int *)*param_1 + 0x58))(s_DigiCombine_DigiCombine_CardFron_01ae106c,1);
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


