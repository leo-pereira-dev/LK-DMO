
/* ============================================================ */
/* Function: FUN_014484b0 */
/* Entry: 014484b0 */
/* Signature: undefined FUN_014484b0() */
/* ============================================================ */


void __fastcall FUN_014484b0(int param_1)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  
  if (*(int *)(DAT_01bfb2c4 + 0x27c) == 0) {
    (**(code **)(**(int **)(param_1 + 0x154) + 0x24))((*(int **)(param_1 + 0x154))[4]);
    return;
  }
  if (*(int *)(param_1 + 0x344) != 0) {
    piVar2 = *(int **)(*(int *)(param_1 + 0x344) + 4);
    iVar10 = 0;
    iVar7 = 0;
    piVar6 = (int *)*piVar2;
    if (piVar6 != piVar2) {
      iVar9 = 0;
      iVar11 = iVar10;
      iVar12 = 0;
      do {
        iVar3 = piVar6[5];
        iVar10 = iVar3;
        if (iVar3 <= iVar11) {
          iVar10 = iVar11;
        }
        iVar7 = iVar12 + 1;
        if (iVar3 < 0xc) {
          iVar7 = iVar12;
        }
        iVar8 = iVar9 + 1;
        if (iVar3 < 0xf) {
          iVar8 = iVar9;
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
          piVar5 = (int *)piVar6[1];
          piVar4 = piVar6;
          while ((piVar6 = piVar5, cVar1 == '\0' && (piVar4 == (int *)piVar6[2]))) {
            cVar1 = *(char *)(piVar6[1] + 0xd);
            piVar5 = (int *)piVar6[1];
            piVar4 = piVar6;
          }
        }
        iVar9 = iVar8;
        iVar11 = iVar10;
        iVar12 = iVar7;
      } while (piVar6 != piVar2);
      if (1 < iVar8) {
        (**(code **)(**(int **)(param_1 + 0x154) + 0x24))((*(int **)(param_1 + 0x154))[4]);
      }
      if (iVar8 == 2) {
        FUN_015b7840(1,s_system_LevelEffect_Red_K16_nif_01afe618,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar8 == 3) {
        FUN_015b7840(1,s_system_LevelEffect_Enchant_K45_n_01afe6c4,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar8 == 4) {
        FUN_015b7840(1,s_system_LevelEffect_Enchant_K60_n_01afe6e8,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
    }
    switch(iVar7) {
    case 1:
      goto switchD_0144861e_caseD_1;
    case 2:
    case 3:
    case 4:
      FUN_015b7840(1,s_system_LevelEffect_Red_K15_nif_01afe6a4,*(undefined1 *)(DAT_01bfb2c4 + 0x2e0)
                  );
      return;
    default:
      if (iVar10 < 4) {
        (**(code **)(**(int **)(param_1 + 0x154) + 0x24))((*(int **)(param_1 + 0x154))[4]);
        return;
      }
      if (iVar10 < 5) {
        FUN_015b7840(1,s_system_LevelEffect_Sky_K04_nif_01afe74c,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar10 < 6) {
        FUN_015b7840(1,s_system_LevelEffect_Sky_K05_nif_01afe76c,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar10 < 7) {
        FUN_015b7840(1,s_system_LevelEffect_Sky_K06_nif_01afe70c,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar10 < 8) {
        FUN_015b7840(1,s_system_LevelEffect_Blue_K07_nif_01afe72c,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar10 < 9) {
        FUN_015b7840(1,s_system_LevelEffect_Blue_K08_nif_01afe7d4,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar10 < 10) {
        FUN_015b7840(1,s_system_LevelEffect_Blue_K09_nif_01afe7f4,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar10 < 0xb) {
        FUN_015b7840(1,s_system_LevelEffect_Pupple_K10_ni_01afe78c,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
      if (iVar10 < 0xc) {
        FUN_015b7840(1,s_system_LevelEffect_Pupple_K11_ni_01afe7b0,
                     *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
        return;
      }
switchD_0144861e_caseD_1:
      FUN_015b7840(1,s_system_LevelEffect_Pupple_K12_ni_01afe680,
                   *(undefined1 *)(DAT_01bfb2c4 + 0x2e0));
    }
  }
  return;
}


