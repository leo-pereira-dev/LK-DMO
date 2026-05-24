
/* ============================================================ */
/* Function: FUN_0164cc40 */
/* Entry: 0164cc40 */
/* Signature: undefined FUN_0164cc40() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_0164cc40(int *param_1,int *param_2,uint *param_3)

{
  uint *puVar1;
  byte *pbVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  char cVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  undefined4 *puVar10;
  int *piVar11;
  undefined4 uVar12;
  uint uVar13;
  size_t _Size;
  undefined4 *puVar14;
  byte bVar15;
  bool bVar16;
  uint local_30;
  uint local_1c;
  byte local_15;
  uint local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_019caa9d;
  local_10 = ExceptionList;
  uVar7 = DAT_01bbb1c0 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  uVar8 = param_2[1] - *param_2;
  uVar9 = *param_3;
  uVar13 = uVar8 - uVar9;
  local_1c = 0;
  if (uVar13 < 4) {
LAB_0164ce2c:
    uVar12 = 0;
  }
  else {
    local_14 = uVar7;
    if (uVar8 <= uVar9) {
LAB_0164ce4f:
                    /* WARNING: Subroutine does not return */
      FUN_00f3fcb0(uVar7);
    }
    puVar1 = (uint *)(*param_2 + uVar9);
    if (puVar1 == (uint *)0x0) {
      memset(&local_1c,0,uVar13);
      piVar11 = _errno();
      *piVar11 = 0x16;
      _invalid_parameter_noinfo();
      uVar9 = local_1c;
    }
    else {
      uVar9 = *puVar1;
    }
    *param_3 = *param_3 + 4;
    local_30 = 0;
    if (uVar9 != 0) {
      do {
        uVar8 = *param_3;
        uVar13 = param_2[1] - *param_2;
        local_15 = 0;
        _Size = uVar13 - uVar8;
        if (_Size == 0) goto LAB_0164ce2c;
        if (uVar13 <= uVar8) goto LAB_0164ce4f;
        pbVar2 = (byte *)(*param_2 + uVar8);
        if (pbVar2 == (byte *)0x0) {
          memset(&local_15,0,_Size);
          piVar11 = _errno();
          *piVar11 = 0x16;
          _invalid_parameter_noinfo();
          bVar15 = local_15;
        }
        else {
          bVar15 = *pbVar2;
        }
        *param_3 = *param_3 + 1;
        cVar6 = FUN_0102b400(param_2,param_3);
        if (cVar6 == '\0') goto LAB_0164ce2c;
        bVar16 = false;
        puVar3 = (undefined4 *)*param_1;
        puVar10 = (undefined4 *)puVar3[1];
        cVar6 = *(char *)((int)puVar10 + 0xd);
        puVar14 = puVar3;
        puVar5 = puVar10;
        while (puVar4 = puVar10, cVar6 == '\0') {
          bVar16 = bVar15 <= *(byte *)(puVar4 + 4);
          if (bVar16) {
            puVar10 = (undefined4 *)*puVar4;
            puVar14 = puVar4;
          }
          else {
            puVar10 = (undefined4 *)puVar4[2];
          }
          cVar6 = *(char *)((int)puVar10 + 0xd);
          puVar5 = puVar4;
        }
        if ((*(char *)((int)puVar14 + 0xd) != '\0') || (bVar15 < *(byte *)(puVar14 + 4))) {
          if (param_1[1] == 0x7ffffff) {
                    /* WARNING: Subroutine does not return */
            FUN_00f1b5f0();
          }
          local_8 = 0;
          piVar11 = operator_new(0x20);
          local_8 = 0xffffffff;
          *(undefined2 *)(piVar11 + 6) = 0;
          *(undefined2 *)(piVar11 + 7) = 0;
          *(undefined2 *)((int)piVar11 + 0x1a) = 0;
          *(byte *)(piVar11 + 4) = bVar15;
          piVar11[5] = (int)&DAT_01a1b548;
          *piVar11 = (int)puVar3;
          piVar11[1] = (int)puVar3;
          piVar11[2] = (int)puVar3;
          *(undefined2 *)(piVar11 + 3) = 0;
          FUN_015b0610(puVar5,bVar16,piVar11);
        }
        local_30 = local_30 + 1;
      } while (local_30 < uVar9);
    }
    uVar12 = 1;
  }
  ExceptionList = local_10;
  return uVar12;
}


