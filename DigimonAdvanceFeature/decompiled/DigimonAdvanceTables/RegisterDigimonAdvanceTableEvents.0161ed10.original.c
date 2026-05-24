
/* ============================================================ */
/* Function: FUN_0161ed10 */
/* Entry: 0161ed10 */
/* Signature: undefined FUN_0161ed10() */
/* ============================================================ */


undefined4 __thiscall FUN_0161ed10(int param_1,int param_2)

{
  char cVar1;
  int *piVar2;
  undefined4 *puVar3;
  int *piVar4;
  int iVar5;
  int *piVar6;
  undefined1 local_80 [12];
  undefined1 local_74 [8];
  int local_6c;
  undefined4 local_68;
  undefined4 uStack_64;
  int local_5c;
  undefined4 local_58;
  undefined *local_54;
  uint local_50;
  int iStack_4c;
  int iStack_48;
  int iStack_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  uint local_38;
  int iStack_34;
  int iStack_30;
  int iStack_2c;
  int local_28;
  int iStack_24;
  int local_20;
  int local_1c;
  int local_18;
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019cc835;
  local_10 = ExceptionList;
  if (param_2 == 0) {
    return 0;
  }
  ExceptionList = &local_10;
  local_18 = param_1;
  __RTDynamicCast(param_2,0,&DAT_01bca164,&DAT_01bc2824,0,DAT_01bbb1c0 ^ (uint)&stack0xfffffffc);
  if (*(int *)(param_1 + 0x20) == 0) {
    puVar3 = (undefined4 *)FUN_01670b40();
    piVar6 = *(int **)*puVar3;
    if (*(char *)((int)piVar6 + 0xd) == '\0') {
      local_1c = local_18 + 0x1c;
      do {
        _Find_lower_bound<>(local_74,piVar6 + 4);
        if ((*(char *)(local_6c + 0xd) != '\0') ||
           (iVar5 = local_6c, (uint)piVar6[4] < *(uint *)(local_6c + 0x10))) {
          iVar5 = *(int *)(local_18 + 0x1c);
        }
        local_20 = *(int *)(local_18 + 0x1c);
        if (iVar5 == local_20) {
          memset(&local_54,0,0x34);
          local_54 = &DAT_01a19e68;
          local_40 = 0;
          iStack_4c = 0;
          iStack_48 = 0;
          iStack_44 = 0;
          uStack_3c = 7;
          iStack_34 = 0;
          iStack_30 = 0;
          iStack_2c = 0;
          local_50 = 0;
          local_28 = 0;
          iStack_24 = 7;
          local_38 = 0;
          local_8 = 0;
          puVar3 = (undefined4 *)_Find_lower_bound<>(local_80,piVar6 + 4);
          iVar5 = puVar3[2];
          local_68 = *puVar3;
          uStack_64 = puVar3[1];
          if ((*(char *)(iVar5 + 0xd) == '\0') && (*(uint *)(iVar5 + 0x10) <= (uint)piVar6[4])) {
            local_11 = '\0';
          }
          else {
            if (*(int *)(local_1c + 4) == 0x38e38e3) {
                    /* WARNING: Subroutine does not return */
              FUN_00f1b5f0();
            }
            local_5c = local_1c;
            local_8 = CONCAT31(local_8._1_3_,1);
            local_58 = 0;
            piVar4 = operator_new(0x48);
            local_58 = 0;
            piVar4[4] = piVar6[4];
            piVar4[5] = (int)&DAT_01a19e68;
            piVar4[10] = 0;
            piVar4[0xb] = 0;
            piVar4[6] = local_50;
            piVar4[7] = iStack_4c;
            piVar4[8] = iStack_48;
            piVar4[9] = iStack_44;
            *(ulonglong *)(piVar4 + 10) = CONCAT44(uStack_3c,local_40);
            local_50 = local_50 & 0xffff0000;
            piVar4[0xc] = local_38;
            piVar4[0xd] = iStack_34;
            piVar4[0xe] = iStack_30;
            piVar4[0xf] = iStack_2c;
            local_38 = local_38 & 0xffff0000;
            piVar4[0x10] = local_28;
            piVar4[0x11] = iStack_24;
            local_28 = 0;
            *piVar4 = local_20;
            piVar4[1] = local_20;
            piVar4[2] = local_20;
            local_40 = 0;
            uStack_3c = 7;
            iStack_24 = 7;
            *(undefined2 *)(piVar4 + 3) = 0;
            iVar5 = FUN_015b0610(local_68,uStack_64,piVar4);
            local_11 = '\x01';
          }
          local_8 = 0xffffffff;
          thunk_FUN_00f1a330();
          thunk_FUN_00f1a330();
          if (local_11 == '\0') {
            ExceptionList = local_10;
            return 0;
          }
        }
        piVar4 = piVar6 + 6;
        if ((int *)(iVar5 + 0x18) != piVar4) {
          if (7 < (uint)piVar6[0xb]) {
            piVar4 = (int *)*piVar4;
          }
          FUN_00f1a040(piVar4,piVar6[10]);
        }
        piVar4 = piVar6 + 0xc;
        if ((int *)(iVar5 + 0x30) != piVar4) {
          if (7 < (uint)piVar6[0x11]) {
            piVar4 = (int *)*piVar4;
          }
          FUN_00f1a040(piVar4,piVar6[0x10]);
        }
        piVar4 = (int *)piVar6[2];
        if (*(char *)((int)piVar4 + 0xd) == '\0') {
          cVar1 = *(char *)(*piVar4 + 0xd);
          piVar6 = piVar4;
          piVar4 = (int *)*piVar4;
          while (cVar1 == '\0') {
            cVar1 = *(char *)(*piVar4 + 0xd);
            piVar6 = piVar4;
            piVar4 = (int *)*piVar4;
          }
        }
        else {
          cVar1 = *(char *)(piVar6[1] + 0xd);
          piVar2 = (int *)piVar6[1];
          piVar4 = piVar6;
          while ((piVar6 = piVar2, cVar1 == '\0' && (piVar4 == (int *)piVar6[2]))) {
            cVar1 = *(char *)(piVar6[1] + 0xd);
            piVar2 = (int *)piVar6[1];
            piVar4 = piVar6;
          }
        }
      } while (*(char *)((int)piVar6 + 0xd) == '\0');
    }
  }
  else {
    piVar6 = (int *)**(int **)(param_1 + 0x1c);
    cVar1 = *(char *)((int)piVar6 + 0xd);
    while (cVar1 == '\0') {
      FUN_0166a6a0(piVar6 + 4,piVar6 + 6,piVar6 + 0xc);
      piVar4 = (int *)piVar6[2];
      if (*(char *)((int)piVar4 + 0xd) == '\0') {
        cVar1 = *(char *)(*piVar4 + 0xd);
        piVar6 = piVar4;
        piVar4 = (int *)*piVar4;
        while (cVar1 == '\0') {
          cVar1 = *(char *)(*piVar4 + 0xd);
          piVar6 = piVar4;
          piVar4 = (int *)*piVar4;
        }
      }
      else {
        cVar1 = *(char *)(piVar6[1] + 0xd);
        piVar2 = (int *)piVar6[1];
        piVar4 = piVar6;
        while ((piVar6 = piVar2, cVar1 == '\0' && (piVar4 == (int *)piVar6[2]))) {
          cVar1 = *(char *)(piVar6[1] + 0xd);
          piVar2 = (int *)piVar6[1];
          piVar4 = piVar6;
        }
      }
      cVar1 = *(char *)((int)piVar6 + 0xd);
    }
  }
  ExceptionList = local_10;
  return 1;
}


