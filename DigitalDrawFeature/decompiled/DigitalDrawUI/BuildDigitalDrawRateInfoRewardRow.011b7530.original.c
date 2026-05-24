
/* ============================================================ */
/* Function: FUN_011b7530 */
/* Entry: 011b7530 */
/* Signature: undefined FUN_011b7530() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall
FUN_011b7530(undefined4 *param_1,undefined4 *param_2,undefined4 *param_3,char *param_4,
            short *param_5)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  uint uVar4;
  void *pvVar5;
  undefined4 uVar6;
  int *piVar7;
  undefined4 *puVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined4 *puVar11;
  undefined4 extraout_ECX;
  short *******pppppppsVar12;
  int iVar13;
  undefined4 uVar14;
  undefined8 uVar15;
  undefined4 uVar16;
  int local_9c;
  int local_94;
  undefined4 *local_84;
  undefined8 local_80;
  undefined1 *local_78;
  undefined4 local_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined1 local_5c;
  undefined4 local_58;
  int local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  short ******ppppppsStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_01991220;
  local_1c = ExceptionList;
  uVar4 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  if (param_1[0x1f] != 0) {
    local_84 = param_1;
    local_24 = uVar4;
    pvVar5 = (void *)FUN_016830c0(0x2c,uVar4);
    local_14 = 0;
    if (pvVar5 == (void *)0x0) {
      local_9c = 0;
    }
    else {
      memset(pvVar5,0,0x2c);
      local_9c = FUN_010baa10();
    }
    local_14 = 0xffffffff;
    if (local_9c != 0) {
      pvVar5 = (void *)FUN_016830c0(0xa0,uVar4);
      local_14 = 1;
      if (pvVar5 == (void *)0x0) {
        local_94 = 0;
      }
      else {
        memset(pvVar5,0,0xa0);
        local_94 = FUN_0123de10();
      }
      uVar3 = DAT_01bee890;
      uVar2 = DAT_01bee88c;
      uVar16 = DAT_01bb9b34;
      uVar15 = DAT_01bb9b2c;
      local_14 = 0xffffffff;
      if (local_94 != 0) {
        FUN_00f19f10();
        local_2c = 0;
        uStack_38 = 0;
        uStack_34 = 0;
        uStack_30 = 0;
        local_28 = 0xf;
        ppppppsStack_3c = (short ******)0x0;
        local_14 = 2;
        FUN_015010a0(&ppppppsStack_3c,&DAT_01ae7290,&DAT_01bfbd40,s_Random_box_frame_png_01adaaec);
        FUN_0124ef30(&ppppppsStack_3c);
        uVar14 = 0;
        uVar6 = FUN_0124f150(0,uVar15,uVar16);
        FUN_0123e330(0,uVar2,uVar3,0x33,0x35,uVar6,uVar14,uVar15,uVar16);
        local_14 = 0xffffffff;
        FUN_00f1a420();
        piVar7 = (int *)FUN_01230f40(local_94,0x29,3,DAT_01bee88c,DAT_01bee890);
        if (piVar7 != (int *)0x0) {
          (**(code **)(*piVar7 + 0x54))(1);
        }
      }
      FUN_01231410(10,*param_2,1,0x28,0x28,0x2f,9);
      puVar8 = (undefined4 *)FUN_016830c0(0xbc);
      local_14 = 3;
      if (puVar8 == (undefined4 *)0x0) {
        puVar8 = (undefined4 *)0x0;
      }
      else {
        memset(puVar8,0,0xbc);
        FUN_0123de10();
        *puVar8 = &DAT_01ae721c;
        *(undefined2 *)(puVar8 + 0x28) = 0;
        *(undefined1 *)((int)puVar8 + 0xa2) = 0;
        puVar8[0x29] = 1;
        puVar8[0x2a] = 1;
        puVar8[0x2b] = 0xffffffff;
        puVar8[0x2c] = 0x3f800000;
        puVar8[0x2d] = 0x3f800000;
        puVar8[0x2e] = 0;
      }
      local_14 = 0xffffffff;
      if (puVar8 != (undefined4 *)0x0) {
        FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x24,0x22,s_CommonUI_digimon_rank_new_dds_01ae1a14,
                     0,1);
        FUN_0123d9a0();
        if (*param_4 < '\x01') {
          iVar9 = 0;
        }
        else {
          iVar9 = *param_4 + -1;
        }
        piVar7 = (int *)FUN_01231700(puVar8,iVar9,0x29,3,DAT_01bee88c,DAT_01bee890);
        if (piVar7 != (int *)0x0) {
          (**(code **)(*piVar7 + 0x54))(1);
        }
      }
      local_64 = 0;
      local_60 = 7;
      uStack_70 = 0;
      uStack_6c = 0;
      uStack_68 = 0;
      local_74 = 0;
      local_48 = 0;
      local_14 = 4;
      local_80 = CONCAT44(DAT_01b02744,DAT_01b02744);
      local_54 = DAT_01bfe468 + 8;
      local_44 = 0;
      local_50 = 0xb;
      local_78 = &DAT_3f7ae148;
      local_5c = 1;
      local_4c = 0;
      local_58 = 4;
      uStack_34 = 0;
      uStack_30 = 0;
      local_2c = 0;
      local_28 = 0;
      uStack_40 = 0;
      ppppppsStack_3c = (short ******)0x0;
      uStack_38 = 0;
      ppppppsStack_3c = operator_new(0xc);
      *ppppppsStack_3c = (short *****)ppppppsStack_3c;
      ppppppsStack_3c[1] = (short *****)ppppppsStack_3c;
      uStack_34 = CONCAT31(uStack_34._1_3_,1);
      uStack_30 = 0;
      local_2c = 0;
      local_28 = 0;
      local_14 = CONCAT31(local_14._1_3_,5);
      if (7 < (uint)param_3[5]) {
        param_3 = (undefined4 *)*param_3;
      }
      FUN_01254f60(&local_80,&local_44,400,param_3,&local_80,extraout_ECX);
      FUN_01230c40(&local_80,0x70,0x1c);
      FUN_012327f0();
      local_14 = 0xffffffff;
      local_54 = 0;
      FUN_00f1a330();
      local_2c = 0;
      local_28 = 0;
      ppppppsStack_3c = (short ******)0x0;
      uStack_38 = 0;
      uStack_34 = 0;
      uStack_30 = 0;
      FUN_00f1a200();
      local_14 = 6;
      pppppppsVar12 = &ppppppsStack_3c;
      if (7 < local_28) {
        pppppppsVar12 = (short *******)ppppppsStack_3c;
      }
      FUN_01500f80(&ppppppsStack_3c,pppppppsVar12,(int)*param_5 / 100,(int)*param_5 % 100);
      FUN_00f53390();
      local_64 = 0;
      local_60 = 7;
      uStack_70 = 0;
      uStack_6c = 0;
      uStack_68 = 0;
      local_74 = 0;
      local_48 = 0;
      local_14 = CONCAT31(local_14._1_3_,7);
      local_54 = DAT_01bfe468 + 8;
      pppppppsVar12 = &ppppppsStack_3c;
      if (7 < local_28) {
        pppppppsVar12 = (short *******)ppppppsStack_3c;
      }
      local_50 = 0xb;
      local_80 = CONCAT44(DAT_01b026d8,DAT_01b0273c);
      local_78 = (undefined1 *)0x3f19999a;
      local_5c = 1;
      local_4c = 0;
      local_58 = 6;
      do {
        sVar1 = *(short *)pppppppsVar12;
        pppppppsVar12 = (short *******)((int)pppppppsVar12 + 2);
      } while (sVar1 != 0);
      FUN_00f1a040();
      FUN_01230c40(&local_80,700,0x1c);
      local_54 = 0;
      FUN_00f1a330();
      local_14 = 0xffffffff;
      FUN_00f1a330();
      puVar8 = (undefined4 *)FUN_016830c0(0x1c);
      if (puVar8 == (undefined4 *)0x0) {
        FUN_011133d0();
        FUN_01683120();
      }
      else {
        *puVar8 = 0;
        puVar8[1] = 0;
        puVar8[2] = 0;
        puVar8[3] = 0;
        *(undefined8 *)(puVar8 + 4) = 0;
        puVar8[6] = 0;
        *puVar8 = &DAT_01ae673c;
        puVar8[1] = 0;
        puVar8[2] = 0;
        puVar8[3] = 0xffffffff;
        *(undefined2 *)(puVar8 + 4) = 0;
        puVar8[5] = 0;
        *(undefined2 *)(puVar8 + 6) = 1;
        FUN_0122b3e0(local_9c);
        puVar10 = operator_new(8);
        *puVar10 = &DAT_01ae1a38;
        puVar10[1] = *param_2;
        if ((undefined4 *)puVar8[2] != (undefined4 *)0x0) {
          (*(code *)**(undefined4 **)puVar8[2])(1);
        }
        puVar8[2] = puVar10;
        iVar13 = 0;
        iVar9 = local_84[0x1f];
        puVar10 = *(undefined4 **)(iVar9 + 100);
        for (puVar11 = (undefined4 *)*puVar10; puVar11 != puVar10; puVar11 = (undefined4 *)*puVar11)
        {
          if ((undefined4 *)puVar11[2] == puVar8) {
            if (iVar13 != -1) {
              ExceptionList = local_1c;
              return;
            }
            break;
          }
          iVar13 = iVar13 + 1;
        }
        if (*(int *)(iVar9 + 0x78) < (int)puVar8[5]) {
          *(undefined4 *)(iVar9 + 0x78) = puVar8[5];
          puVar11 = (undefined4 *)*puVar10;
          if (puVar11 != puVar10) {
            do {
              *(undefined4 *)(puVar11[2] + 0x14) = *(undefined4 *)(iVar9 + 0x78);
              puVar11 = (undefined4 *)*puVar11;
            } while (puVar11 != *(undefined4 **)(iVar9 + 100));
          }
        }
        else {
          puVar8[5] = *(int *)(iVar9 + 0x78);
        }
        local_84 = puVar8;
        FUN_00f55430(&local_84);
        FUN_0122c760();
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


