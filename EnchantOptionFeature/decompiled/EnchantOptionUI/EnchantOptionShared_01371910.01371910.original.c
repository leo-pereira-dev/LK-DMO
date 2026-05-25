
/* ============================================================ */
/* Function: FUN_01371910 */
/* Entry: 01371910 */
/* Signature: undefined FUN_01371910() */
/* ============================================================ */


void FUN_01371910(undefined4 *param_1,char *param_2,char *param_3)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  char *pcVar6;
  char *pcVar7;
  uint uVar8;
  
  pcVar6 = param_3;
  pcVar7 = param_2;
  do {
    cVar1 = *pcVar7;
    pcVar7 = pcVar7 + 1;
  } while (cVar1 != '\0');
  uVar8 = (int)pcVar7 - (int)(param_2 + 1);
  iVar2 = *(int *)(param_3 + 0x10);
  if (*(uint *)(param_3 + 0x14) - iVar2 < uVar8) {
    pcVar6 = (char *)FUN_01371ba0(uVar8,param_3,0,param_2,uVar8);
  }
  else {
    *(uint *)(param_3 + 0x10) = iVar2 + uVar8;
    pcVar7 = param_3;
    if (0xf < *(uint *)(param_3 + 0x14)) {
      pcVar7 = *(char **)param_3;
    }
    param_3 = (char *)uVar8;
    if ((pcVar7 < param_2 + uVar8) && (param_2 <= pcVar7 + iVar2)) {
      if (param_2 < pcVar7) {
        param_3 = (char *)((int)pcVar7 - (int)param_2);
      }
      else {
        param_3 = (char *)0x0;
      }
    }
    memcpy(pcVar7 + uVar8,pcVar7,iVar2 + 1);
    memcpy(pcVar7,param_2,(size_t)param_3);
    memcpy(pcVar7 + (int)param_3,param_2 + (int)param_3 + uVar8,uVar8 - (int)param_3);
  }
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  uVar3 = *(undefined4 *)(pcVar6 + 4);
  uVar4 = *(undefined4 *)(pcVar6 + 8);
  uVar5 = *(undefined4 *)(pcVar6 + 0xc);
  *param_1 = *(undefined4 *)pcVar6;
  param_1[1] = uVar3;
  param_1[2] = uVar4;
  param_1[3] = uVar5;
  *(undefined8 *)(param_1 + 4) = *(undefined8 *)(pcVar6 + 0x10);
  pcVar6[0x10] = '\0';
  pcVar6[0x11] = '\0';
  pcVar6[0x12] = '\0';
  pcVar6[0x13] = '\0';
  pcVar6[0x14] = '\x0f';
  pcVar6[0x15] = '\0';
  pcVar6[0x16] = '\0';
  pcVar6[0x17] = '\0';
  *pcVar6 = '\0';
  return;
}


