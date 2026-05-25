
/* ============================================================ */
/* Function: FUN_0125ffe0 */
/* Entry: 0125ffe0 */
/* Signature: undefined FUN_0125ffe0() */
/* ============================================================ */


undefined4 * FUN_0125ffe0(undefined4 *param_1,ushort param_2)

{
  uint uVar1;
  int iVar2;
  undefined4 *puVar3;
  float in_XMM3_Da;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  wchar_t *pwVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  double dVar11;
  undefined1 local_40 [28];
  undefined4 local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  puStack_18 = &LAB_0199b201;
  local_1c = ExceptionList;
  uVar1 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[5] = 7;
  *(undefined2 *)param_1 = 0;
  local_14 = 0;
  local_24 = 1;
  if (param_2 < 0x3ea) {
    if (param_2 != 0x3e9) {
      switch(param_2) {
      case 1:
        break;
      case 2:
        goto switchD_01260074_caseD_2;
      case 3:
        goto switchD_01260074_caseD_3;
      case 4:
        goto switchD_01260074_caseD_4;
      case 5:
        goto switchD_01260074_caseD_5;
      case 6:
        goto switchD_01260074_caseD_6;
      case 7:
        goto switchD_01260074_caseD_7;
      case 8:
        goto switchD_01260074_caseD_8;
      case 9:
        goto switchD_01260074_caseD_9;
      case 10:
        goto switchD_01260074_caseD_a;
      case 0xb:
        goto switchD_01260074_caseD_b;
      case 0xc:
        goto switchD_01260074_caseD_c;
      case 0xd:
        goto switchD_01260074_caseD_d;
      case 0xe:
        goto switchD_01260074_caseD_e;
      case 0xf:
        goto switchD_01260074_caseD_f;
      case 0x10:
        goto switchD_01260074_caseD_10;
      case 0x11:
        goto switchD_01260074_caseD_11;
      case 0x12:
        goto switchD_01260074_caseD_12;
      default:
        goto switchD_01260074_caseD_13;
      case 0x65:
        goto switchD_01260074_caseD_65;
      case 0x66:
        goto switchD_01260074_caseD_66;
      case 0x67:
        goto switchD_01260074_caseD_67;
      case 0x68:
        goto switchD_01260074_caseD_68;
      case 0x69:
        goto switchD_01260074_caseD_69;
      case 0x6a:
        goto switchD_01260074_caseD_6a;
      case 0x6b:
        goto switchD_01260074_caseD_6b;
      case 0x6c:
        goto switchD_01260074_caseD_6c;
      case 0x6d:
        goto switchD_01260074_caseD_6d;
      case 0x6e:
        goto switchD_01260074_caseD_6e;
      case 0x6f:
        goto switchD_01260074_caseD_6f;
      case 0x70:
        goto switchD_01260074_caseD_70;
      case 0x71:
        goto switchD_01260074_caseD_71;
      case 0x72:
        goto switchD_01260074_caseD_72;
      }
    }
switchD_01260074_caseD_1:
    iVar2 = (int)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_ATTACK_01ae7ccc,0x11);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9);
    local_14 = 1;
    goto LAB_01260104;
  }
  if (param_2 < 0x7d2) {
    if (param_2 != 0x7d1) {
      switch(param_2) {
      case 0x3ea:
        goto switchD_01260074_caseD_2;
      case 0x3eb:
        goto switchD_01260074_caseD_3;
      case 0x3ec:
        goto switchD_01260074_caseD_4;
      case 0x3ed:
        goto switchD_01260074_caseD_5;
      case 0x3ee:
        goto switchD_01260074_caseD_6;
      case 0x3ef:
        goto switchD_01260074_caseD_7;
      case 0x3f0:
        goto switchD_01260074_caseD_8;
      case 0x3f1:
        goto switchD_01260074_caseD_9;
      case 0x3f2:
        goto switchD_01260074_caseD_a;
      case 0x3f3:
        goto switchD_01260074_caseD_b;
      case 0x3f4:
        goto switchD_01260074_caseD_c;
      case 0x3f5:
        goto switchD_01260074_caseD_d;
      case 0x3f6:
        goto switchD_01260074_caseD_e;
      case 0x3f7:
        goto switchD_01260074_caseD_f;
      case 0x3f8:
        goto switchD_01260074_caseD_10;
      case 0x3f9:
        goto switchD_01260074_caseD_11;
      case 0x3fa:
        goto switchD_01260074_caseD_12;
      default:
        goto switchD_01260074_caseD_13;
      case 0x44d:
        goto switchD_01260074_caseD_65;
      case 0x44e:
        goto switchD_01260074_caseD_66;
      case 0x44f:
        goto switchD_01260074_caseD_67;
      case 0x450:
        goto switchD_01260074_caseD_68;
      case 0x451:
        goto switchD_01260074_caseD_69;
      case 0x452:
        goto switchD_01260074_caseD_6a;
      case 0x453:
        goto switchD_01260074_caseD_6b;
      case 0x454:
        goto switchD_01260074_caseD_6c;
      case 0x455:
        goto switchD_01260074_caseD_6d;
      case 0x456:
        goto switchD_01260074_caseD_6e;
      case 0x457:
        goto switchD_01260074_caseD_6f;
      case 0x458:
        goto switchD_01260074_caseD_70;
      case 0x459:
        goto switchD_01260074_caseD_71;
      case 0x45a:
        goto switchD_01260074_caseD_72;
      }
    }
    goto switchD_01260074_caseD_1;
  }
  if (param_2 < 0xbba) {
    if (param_2 != 0xbb9) {
      switch(param_2) {
      case 0x7d2:
        goto switchD_01260074_caseD_2;
      case 0x7d3:
        goto switchD_01260074_caseD_3;
      case 0x7d4:
        goto switchD_01260074_caseD_4;
      case 0x7d5:
        goto switchD_01260074_caseD_5;
      case 0x7d6:
        goto switchD_01260074_caseD_6;
      case 0x7d7:
        goto switchD_01260074_caseD_7;
      case 0x7d8:
        goto switchD_01260074_caseD_8;
      case 0x7d9:
        goto switchD_01260074_caseD_9;
      case 0x7da:
        goto switchD_01260074_caseD_a;
      case 0x7db:
        goto switchD_01260074_caseD_b;
      case 0x7dc:
        goto switchD_01260074_caseD_c;
      case 0x7dd:
        goto switchD_01260074_caseD_d;
      case 0x7de:
        goto switchD_01260074_caseD_e;
      case 0x7df:
        goto switchD_01260074_caseD_f;
      case 0x7e0:
        goto switchD_01260074_caseD_10;
      case 0x7e1:
        goto switchD_01260074_caseD_11;
      case 0x7e2:
        goto switchD_01260074_caseD_12;
      default:
        goto switchD_01260074_caseD_13;
      case 0x835:
        goto switchD_01260074_caseD_65;
      case 0x836:
        goto switchD_01260074_caseD_66;
      case 0x837:
        goto switchD_01260074_caseD_67;
      case 0x838:
        goto switchD_01260074_caseD_68;
      case 0x839:
        goto switchD_01260074_caseD_69;
      case 0x83a:
        goto switchD_01260074_caseD_6a;
      case 0x83b:
        goto switchD_01260074_caseD_6b;
      case 0x83c:
        goto switchD_01260074_caseD_6c;
      case 0x83d:
        goto switchD_01260074_caseD_6d;
      case 0x83e:
        goto switchD_01260074_caseD_6e;
      case 0x83f:
        goto switchD_01260074_caseD_6f;
      case 0x840:
        goto switchD_01260074_caseD_70;
      case 0x841:
        goto switchD_01260074_caseD_71;
      case 0x842:
        goto switchD_01260074_caseD_72;
      }
    }
    goto switchD_01260074_caseD_1;
  }
  switch(param_2) {
  case 0xbba:
    goto switchD_01260074_caseD_2;
  case 0xbbb:
switchD_01260074_caseD_3:
    iVar2 = (int)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_MAX_HP_01ae7d0c,0x11);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9);
    local_14 = 3;
    goto LAB_01260104;
  case 0xbbc:
switchD_01260074_caseD_4:
    iVar2 = (int)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_MAX_DS_01ae7d48,0x11);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9);
    local_14 = 4;
    goto LAB_01260104;
  case 0xbbd:
switchD_01260074_caseD_5:
    iVar2 = (int)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_SKILLAP_01ae7d34,0x12);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9);
    local_14 = 5;
    goto LAB_01260104;
  case 0xbbe:
switchD_01260074_caseD_6:
    dVar11 = (double)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_ATTRIBUTEAP_01ae7d78,0x16);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 6;
    goto LAB_01260293;
  case 0xbbf:
switchD_01260074_caseD_7:
    dVar11 = (double)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_CRITICAL_01ae7d90,0x13);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 7;
    goto LAB_01260293;
  case 0xbc0:
switchD_01260074_caseD_8:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_CRITICAL_DAMAGE_01ae7de0,0x1a);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 8;
    break;
  case 0xbc1:
switchD_01260074_caseD_9:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_ATTACK_SPEED_01ac5194,0x17);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 9;
    break;
  case 0xbc2:
switchD_01260074_caseD_a:
    dVar11 = (double)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_AVOID_01ae7dfc,0x10);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 10;
    goto LAB_01260293;
  case 0xbc3:
switchD_01260074_caseD_b:
    dVar11 = (double)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_BLOCK_01ac523c,0x10);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0xb;
LAB_01260293:
    if (7 < (uint)puVar3[5]) {
      puVar3 = (undefined4 *)*puVar3;
    }
    pwVar8 = u____s_____2f___01ae845c;
    goto LAB_01260b54;
  case 0xbc4:
switchD_01260074_caseD_c:
    iVar2 = (int)in_XMM3_Da;
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_HIT_RATE_01ae7e48,0x13);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9);
    local_14 = 0xc;
LAB_01260104:
    if (7 < (uint)puVar3[5]) {
      puVar3 = (undefined4 *)*puVar3;
    }
    FUN_01500f80(param_1,u____s____d___01ae840c,puVar3,iVar2,uVar1);
    goto LAB_01260b5d;
  case 0xbc5:
switchD_01260074_caseD_d:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_AP_RATIO_01ae7e34,0x13);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0xd;
    break;
  case 0xbc6:
switchD_01260074_caseD_e:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_DP_RATIO_01ae7e5c,0x13);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0xe;
    break;
  case 0xbc7:
switchD_01260074_caseD_f:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_MAX_HP_RATIO_01ae7ea8,0x17);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0xf;
    break;
  case 0xbc8:
switchD_01260074_caseD_10:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_MAX_DS_RATIO_01ae7e90,0x17);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x10;
    break;
  case 0xbc9:
switchD_01260074_caseD_11:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_SKILL_AP_RATIO_01ae7edc,0x19);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x11;
    break;
  case 0xbca:
switchD_01260074_caseD_12:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_FINAL_AP_RATIO_01ae7ec0,0x19);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x12;
    break;
  default:
    goto switchD_01260074_caseD_13;
  case 0xc1d:
switchD_01260074_caseD_65:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_DATA_ATTRIBUTE_DAMAGE_01ac5900,0x20);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x13;
    break;
  case 0xc1e:
switchD_01260074_caseD_66:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_VACCINE_ATTRIBUTE_DAM_01ac5924,0x23);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x14;
    break;
  case 0xc1f:
switchD_01260074_caseD_67:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_VIRUS_ATTRIBUTE_DAMAG_01ac5948,0x21);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x15;
    break;
  case 0xc20:
switchD_01260074_caseD_68:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_UNKNOWN_ATTRIBUTE_DAM_01ac596c,0x23);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x16;
    break;
  case 0xc21:
switchD_01260074_caseD_69:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_ICE_ATTRIBUTE_DAMAGE_01ac577c,0x1f);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x17;
    break;
  case 0xc22:
switchD_01260074_caseD_6a:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_WATER_ATTRIBUTE_DAMAG_01ac579c,0x21);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x18;
    break;
  case 0xc23:
switchD_01260074_caseD_6b:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_FIRE_ATTRIBUTE_DAMAGE_01ac57c0,0x20);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x19;
    break;
  case 0xc24:
switchD_01260074_caseD_6c:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_EARTH_ATTRIBUTE_DAMAG_01ac57e4,0x21);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x1a;
    break;
  case 0xc25:
switchD_01260074_caseD_6d:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_WIND_ATTRIBUTE_DAMAGE_01ac5808,0x20);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x1b;
    break;
  case 0xc26:
switchD_01260074_caseD_6e:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_WOOD_ATTRIBUTE_DAMAGE_01ac582c,0x20);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x1c;
    break;
  case 0xc27:
switchD_01260074_caseD_6f:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_LIGHT_ATTRIBUTE_DAMAG_01ac5850,0x21);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x1d;
    break;
  case 0xc28:
switchD_01260074_caseD_70:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_DARKNESS_ATTRIBUTE_DA_01ac5874,0x24);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x1e;
    break;
  case 0xc29:
switchD_01260074_caseD_71:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    puStack_20 = &stack0xfffffffc;
    FUN_00f19cc0(s_COMMON_TXT_THUNDER_ATTRIBUTE_DAM_01ac589c,0x23);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x1f;
    break;
  case 0xc2a:
switchD_01260074_caseD_72:
    dVar11 = (double)(in_XMM3_Da / DAT_01b02970);
    uVar4 = 0;
    uVar5 = 0;
    uVar6 = 0;
    uVar7 = 0;
    uVar9 = 0;
    uVar10 = 0;
    FUN_00f19cc0(s_COMMON_TXT_STEEL_ATTRIBUTE_DAMAG_01ac58c0,0x21);
    puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9,uVar10);
    local_14 = 0x20;
  }
  if (7 < (uint)puVar3[5]) {
    puVar3 = (undefined4 *)*puVar3;
  }
  pwVar8 = u____s____5_2f___01ae843c;
LAB_01260b54:
  FUN_01500f80(param_1,pwVar8,puVar3,dVar11);
LAB_01260b5d:
  FUN_00f1a330();
switchD_01260074_caseD_13:
  ExceptionList = local_1c;
  return param_1;
switchD_01260074_caseD_2:
  iVar2 = (int)in_XMM3_Da;
  uVar4 = 0;
  uVar5 = 0;
  uVar6 = 0;
  uVar7 = 0;
  uVar9 = 0;
  puStack_20 = &stack0xfffffffc;
  FUN_00f19cc0(s_COMMON_TXT_DEFENSE_01ae7d20,0x12);
  puVar3 = (undefined4 *)FUN_015ce480(local_40,uVar4,uVar5,uVar6,uVar7,uVar9);
  local_14 = 2;
  goto LAB_01260104;
}


