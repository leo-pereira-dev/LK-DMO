
/* ============================================================ */
/* Function: FUN_012b1590 */
/* Entry: 012b1590 */
/* Signature: undefined FUN_012b1590() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 __thiscall FUN_012b1590(int param_1,uint *param_2,char param_3)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  void *pvVar5;
  uint *puVar6;
  char *pcVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  uint local_70;
  uint local_6c;
  uint *local_68;
  uint *local_64;
  int *local_60;
  undefined1 local_5c [16];
  int local_4c;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined8 local_30;
  void *local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_019a027d;
  local_1c = ExceptionList;
  uVar2 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  local_68 = param_2;
  local_24 = uVar2;
  iVar3 = FUN_012b0820(param_2);
  if (iVar3 == 0) {
    if ((((*(char *)(param_1 + 0x130) == '\0') && (DAT_01bfb2b4 != 0)) && (DAT_01bfb2b4 != -0x28))
       && (*param_2 < 200)) {
      puVar6 = (uint *)(DAT_01bfb2b4 + 0x58 + *param_2 * 0x45);
      if ((puVar6 != (uint *)0x0) && (local_70 = *puVar6, *(int *)(param_1 + 0x140) != 0)) {
        local_64 = puVar6;
        local_60 = (int *)FUN_015db910(&local_70);
        if (local_60 != (int *)0x0) {
          if (param_3 == '\0') {
            if ((*(int *)(param_1 + 0x148) != 0) &&
               (cVar1 = FUN_015ecc20(*puVar6,*(undefined1 *)((int)puVar6 + 9)), cVar1 != '\0')) {
              if ((DAT_01bfb2b4 != 0) && (DAT_01bfb2b4 != -0x28)) {
                FUN_00f633b0(*(undefined4 *)(param_1 + 0x48));
              }
              FUN_012b1f10(uVar2);
              *(uint *)(param_1 + 0x48) = *local_68;
LAB_012b17e9:
              if ((DAT_01bfb2b4 != 0) && (DAT_01bfb2b4 != -0x28)) {
                FUN_00f63330(*local_68);
              }
              FUN_012b0a90((short)local_60[0x17],puVar6);
              local_28 = (void *)*puVar6;
              piVar4 = local_60 + 0x35;
              local_60 = (int *)(uint)*(ushort *)(local_60 + 0x17);
              FUN_00f221a0(piVar4);
              local_14 = 1;
              if (local_4c == 0) {
                FUN_00f1a040(&DAT_01acce68,3);
              }
              local_44 = 0;
              uStack_40 = 0;
              uStack_3c = 0;
              uStack_38 = 0;
              local_34 = 0;
              local_14 = CONCAT31(local_14._1_3_,2);
              if (param_3 == '\0') {
                FUN_00f5dab0(param_1 + 0x48);
                local_64 = (uint *)FUN_00f5dab0(&local_60);
              }
              else {
                FUN_00f5dab0(param_1 + 0x78);
                local_64 = (uint *)FUN_00f5dab0(&local_60);
              }
              piVar4 = (int *)((int)local_64 + 8);
              uVar2 = local_4c * 2 + 4;
              local_60 = (int *)((int)local_64 + 4);
              if ((uint)((*(int *)((int)local_64 + 0xc) - *piVar4) - *local_60) < uVar2) {
                FUN_00f26b00(*local_60 + uVar2);
              }
              FUN_00f5e1e0(local_5c,piVar4,local_60);
              FUN_00f5dab0(&local_28);
              FUN_00f709c0(&param_3);
              local_6c = 0x1a;
              FUN_00ffcb30(&local_6c,&local_44);
              FUN_00f1e050();
              FUN_00f1a330();
              ExceptionList = local_1c;
              return 1;
            }
            uVar8 = 0;
            uVar9 = 0;
            uVar10 = 0;
            uVar11 = 0;
            uVar13 = 0;
            uVar12 = 0;
            FUN_00f19cc0(s_DATA_TRANSFER_NO_DATA_EQUIPMENT_01aeb144,0x1f);
            FUN_010052f0(uVar8,uVar9,uVar10,uVar11,uVar13,uVar12);
          }
          else {
            uVar2 = *(uint *)(param_1 + 0x48);
            if ((((DAT_01bfb2b4 == 0) || (DAT_01bfb2b4 + 0x28 == 0)) ||
                (*(uint *)(DAT_01bfb2b4 + 0x44) <= uVar2)) ||
               (((199 < uVar2 ||
                 (piVar4 = (int *)(uVar2 * 0x45 + 0x30 + DAT_01bfb2b4 + 0x28), piVar4 == (int *)0x0)
                 ) || ((*piVar4 == 0 && ((short)piVar4[1] == 0)))))) {
              piVar4 = (int *)0x0;
            }
            iVar3 = *(int *)(param_1 + 0x148);
            local_30 = 0;
            if (iVar3 == 0) {
              local_30._0_4_ = (void *)iVar3;
              local_30._4_4_ = iVar3;
              pvVar5 = operator_new(0x38);
              *(void **)pvVar5 = pvVar5;
              *(void **)((int)pvVar5 + 4) = pvVar5;
              *(void **)((int)pvVar5 + 8) = pvVar5;
              *(undefined2 *)((int)pvVar5 + 0xc) = 0x101;
              local_30 = CONCAT44(local_30._4_4_,pvVar5);
            }
            else {
              FUN_015ec8b0(&local_30,*piVar4,*(undefined1 *)((int)piVar4 + 9));
              pvVar5 = (void *)local_30;
            }
            local_14 = 0;
            local_6c = *local_64;
            FUN_01271bc0(&local_28,&local_6c);
            puVar6 = local_64;
            if (local_28 == pvVar5) {
              uVar8 = 0x1f;
              pcVar7 = s_DATA_TRANSFER_NO_DATA_EQUIPMENT_01aeb144;
            }
            else {
              if ((int)(uint)*(byte *)((int)local_64 + 9) < *(int *)((int)local_28 + 0x18)) {
                if ((DAT_01bfb2b4 != 0) && (DAT_01bfb2b4 != -0x28)) {
                  FUN_00f633b0(*(undefined4 *)(param_1 + 0x78));
                }
                *(uint *)(param_1 + 0x78) = *local_68;
                local_14 = 0xffffffff;
                FUN_0115a360();
                goto LAB_012b17e9;
              }
              uVar8 = 0x19;
              pcVar7 = s_DATA_TRANSFER_GRADE_ERROR_01aeb19c;
            }
            uVar9 = 0;
            uVar10 = 0;
            uVar11 = 0;
            uVar13 = 0;
            uVar12 = 0;
            uVar14 = 0;
            FUN_00f19cc0(pcVar7,uVar8);
            FUN_010052f0(uVar9,uVar10,uVar11,uVar13,uVar12,uVar14);
            FUN_0115a360();
          }
        }
      }
    }
  }
  else {
    FUN_013bdff0(0,0,0,0);
  }
  ExceptionList = local_1c;
  return 0;
}


