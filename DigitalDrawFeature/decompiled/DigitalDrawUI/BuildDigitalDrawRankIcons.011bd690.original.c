
/* ============================================================ */
/* Function: FUN_011bd690 */
/* Entry: 011bd690 */
/* Signature: undefined FUN_011bd690() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __thiscall FUN_011bd690(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  char cVar1;
  undefined4 *puVar2;
  undefined1 *puVar3;
  int iVar4;
  void *_Dst;
  int iVar5;
  int *piVar6;
  int *piVar7;
  int local_30;
  int local_2c;
  int local_28;
  uint local_24;
  undefined1 *puStack_20;
  void *local_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  
  puStack_20 = &stack0xfffffffc;
  local_14 = 0xffffffff;
  puStack_18 = &LAB_0199171f;
  local_1c = ExceptionList;
  local_24 = DAT_01bbb1c0 ^ (uint)&stack0xfffffff0;
  ExceptionList = &local_1c;
  puVar3 = &stack0xfffffffc;
  if (((*(int *)(param_1 + 0x18) != 0) &&
      (iVar4 = *(int *)(*(int *)(param_1 + 0x18) + 0x24), puVar3 = &stack0xfffffffc, iVar4 != 0)) &&
     (iVar4 = __RTDynamicCast(*(undefined4 *)(iVar4 + 0x34),0,&DAT_01bc4a94,&DAT_01bc6058,0,local_24
                             ), puVar3 = puStack_20, iVar4 != 0)) {
    *(undefined4 *)(iVar4 + 4) = *param_2;
  }
  puStack_20 = puVar3;
  iVar4 = *(int *)(param_1 + 0x24);
  if (iVar4 == 0) {
    FUN_00f5d420();
  }
  else {
    if (*(int *)(iVar4 + 4) != 0) {
      FUN_01230b60(param_1 + 0x1c);
    }
    FUN_00f5d420();
    if (*(int *)(iVar4 + 4) != 0) {
      local_2c = 0x23;
      local_30 = 0x136;
      cVar1 = *(char *)(*(int *)*param_3 + 0xd);
      iVar4 = *(int *)*param_3;
      while (cVar1 == '\0') {
        local_28 = iVar4;
        _Dst = (void *)FUN_016830c0(0xbc);
        local_14 = 0;
        if (_Dst == (void *)0x0) {
          iVar5 = 0;
        }
        else {
          memset(_Dst,0,0xbc);
          iVar5 = FUN_0123d6c0();
        }
        local_14 = 0xffffffff;
        if (iVar5 != 0) {
          FUN_0123d7c0(0,DAT_01bee88c,DAT_01bee890,0x20,0x1c,
                       s_CommonUI_digimon_rank_new_dds_01ae1a14,0,1);
          FUN_0123d9a0(0x48,0x3e);
          if (*(char *)(iVar4 + 0xe) < '\x01') {
            iVar4 = 0;
          }
          else {
            iVar4 = *(char *)(iVar4 + 0xe) + -1;
          }
          piVar6 = (int *)FUN_01231700(iVar5,iVar4,local_2c,local_30,DAT_01bee88c,DAT_01bee890);
          if (piVar6 != (int *)0x0) {
            (**(code **)(*piVar6 + 0x54))(1);
            iVar4 = *(int *)(param_1 + 0x1c);
            if (*(int *)(param_1 + 0x20) == 0x15555555) {
                    /* WARNING: Subroutine does not return */
              std::_Xlength_error(s_list_too_long_01abdaa4);
            }
            local_14 = 1;
            piVar7 = operator_new(0xc);
            local_14 = 0xffffffff;
            piVar7[2] = (int)piVar6;
            *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + 1;
            puVar2 = *(undefined4 **)(iVar4 + 4);
            *piVar7 = iVar4;
            local_2c = local_2c + 0x20;
            piVar7[1] = (int)puVar2;
            *(int **)(iVar4 + 4) = piVar7;
            *puVar2 = piVar7;
            if (0x8c < local_2c) {
              local_30 = local_30 + 0x1c;
              local_2c = 0x23;
            }
          }
        }
        std::
        _Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
        ::operator++((_Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
                      *)&local_28);
        iVar4 = local_28;
        cVar1 = *(char *)(local_28 + 0xd);
      }
    }
  }
  ExceptionList = local_1c;
  return;
}


