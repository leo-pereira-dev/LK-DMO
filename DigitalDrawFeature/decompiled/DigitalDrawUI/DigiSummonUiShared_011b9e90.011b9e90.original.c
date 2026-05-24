
/* ============================================================ */
/* Function: FUN_011b9e90 */
/* Entry: 011b9e90 */
/* Signature: undefined FUN_011b9e90() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011b9e90(int param_1)

{
  char cVar1;
  int iVar2;
  int local_c;
  undefined4 local_8;
  uint local_4;
  
  local_4 = DAT_01bbb1c0 ^ (uint)&local_c;
  local_c = **(int **)(param_1 + 0x7c);
  cVar1 = *(char *)(local_c + 0xd);
  while (cVar1 == '\0') {
    if (((*(int *)(local_c + 0x20) == 0) ||
        (iVar2 = *(int *)(*(int *)(local_c + 0x20) + 0x24), iVar2 == 0)) ||
       (*(char *)(iVar2 + 0x88) == '\0')) {
      local_8 = *(undefined4 *)(param_1 + 0x78);
      FUN_011ba600(&local_8);
    }
    std::
    _Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
    ::operator++((_Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
                  *)&local_c);
    cVar1 = *(char *)(local_c + 0xd);
  }
  if (*(int **)(param_1 + 0x98) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x98) + 0x1c))(0);
  }
  if (*(int **)(param_1 + 0x9c) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x9c) + 0x1c))(0);
  }
  if (*(int **)(param_1 + 0x90) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x90) + 0x1c))(0);
  }
  if (*(int **)(param_1 + 0x94) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x94) + 0x1c))(0);
  }
  if (*(int **)(param_1 + 0xa0) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0xa0) + 0x1c))(0);
  }
  if (*(int **)(param_1 + 0x94) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x94) + 0x1c))(1);
  }
  return;
}


